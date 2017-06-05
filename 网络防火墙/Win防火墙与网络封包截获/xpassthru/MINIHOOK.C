/*——————————————————————————————————————
	文件：miniport.c
	工程：xpassthru
	概述：演示NDIS中间驱动程序输出封包结构信息
	版权所有(c) 2001-2002 X 工作室
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

NDIS_STATUS
MPRegisterAsMiniport(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
)
{
	NDIS_STATUS						Status;
	NDIS_MINIPORT_CHARACTERISTICS	MiniportStruct;

	NdisZeroMemory(&MiniportStruct, sizeof(NDIS_MINIPORT_CHARACTERISTICS));

	MiniportStruct.MajorNdisVersion = 4;
	MiniportStruct.MinorNdisVersion = 0;
	MiniportStruct.Reserved			= 0;
	MiniportStruct.HaltHandler				= MPHalt;
	MiniportStruct.InitializeHandler		= MPInitialize;
	MiniportStruct.QueryInformationHandler	= MPQueryInformation;
	MiniportStruct.SetInformationHandler	= MPSetInformation;
	MiniportStruct.ResetHandler				= MPReset;
	MiniportStruct.TransferDataHandler		= MPTransferData;

    MiniportStruct.ReconfigureHandler		= NULL;
    MiniportStruct.DisableInterruptHandler	= NULL;
    MiniportStruct.EnableInterruptHandler	= NULL;
    MiniportStruct.HandleInterruptHandler	= NULL;
    MiniportStruct.ISRHandler				= NULL;
	MiniportStruct.AllocateCompleteHandler	= NULL;

	MiniportStruct.CheckForHangHandler		= NULL;
	MiniportStruct.SendHandler				= MPSend;
	MiniportStruct.ReturnPacketHandler		= MPReturnPacket;
	//————————————————————————————————————
	//	尽管Send或者SendPackets函数指针应该指定，但是如果SendPackets指针被
	//	指定则SendHandler函数将被忽略，所以只需保留一个。
	//
	MiniportStruct.SendPacketsHandler		= MPSendPackets;

	Status = NdisIMRegisterLayeredMiniport(
		WrapperHandle, &MiniportStruct, sizeof(MiniportStruct), &DriverHandle);

	DbgPrint("<== NdisIMRegisterLayeredMiniport,the return code is %u.\n", Status);
	return Status;
}

/*——————————————————————————————————————
	Miniport设备的初始化函数，初始化状态值，并加入全局列表已备调用
	详情请参阅DDK帮助文档关于MiniportInitialize函数的解释
*/
NDIS_STATUS
MPInitialize(
	OUT PNDIS_STATUS	OpenErrorStatus,
	OUT PUINT			SelectedMediumIndex,
	IN	PNDIS_MEDIUM	MediumArray,
	IN	UINT			MediumArraySize,
	IN	NDIS_HANDLE		MiniportAdapterHandle,
	IN	NDIS_HANDLE		WrapperConfigurationContext
)
{
	UINT			i;
	PADAPT			pAdapt;
	NDIS_STATUS		Status		 = NDIS_STATUS_FAILURE;
	NDIS_STATUS		BundleStatus = NDIS_STATUS_FAILURE;
	NDIS_STRING		BundleUniString;
	KIRQL			OldIrql;

	DBGPRINT("==>Passthru Initialize Miniport\n");

	//————————————————————————————————————
	//	首先通过MiniportAdapterHandle句柄得到adapter结构并保存MiniportHandle
	//
	pAdapt = NdisIMGetDeviceContext(MiniportAdapterHandle);
	pAdapt->MiniportHandle = MiniportAdapterHandle;

	//————————————————————————————————————
	//	通过循环语句验证Medium的合法性，并设置当前选择的Medium索引值。
	//
	for (i = 0; i < MediumArraySize; i++)
	{
		if (MediumArray[i] == pAdapt->Medium)
		{
			*SelectedMediumIndex = i;
			break;
		}
	}
	if (i == MediumArraySize)
		return(NDIS_STATUS_UNSUPPORTED_MEDIA);

	//————————————————————————————————————
	//	设置MiniportAdapter的属性
	//
	NdisMSetAttributesEx(MiniportAdapterHandle, 
						 pAdapt,
						 0,			// 检查时允许挂起的时间，单位为秒。
						 NDIS_ATTRIBUTE_IGNORE_PACKET_TIMEOUT		|
							NDIS_ATTRIBUTE_IGNORE_REQUEST_TIMEOUT	|
							NDIS_ATTRIBUTE_INTERMEDIATE_DRIVER		|
							NDIS_ATTRIBUTE_DESERIALIZE				|
							NDIS_ATTRIBUTE_NO_HALT_ON_SUSPEND,
						 0);

	//————————————————————————————————————
	//	设置Miniport和Protocol设备的状态为NdisDeviceStateD0
	//	NdisDeviceStateD0表示设备电源管理准备就绪已经激活，默认值为ON。
	//
	pAdapt->MPDeviceState = NdisDeviceStateD0;
	pAdapt->PTDeviceState = NdisDeviceStateD0;

	//————————————————————————————————————
	//	开始加载列表并绑定标识，设置为默认选项，miniport作为主miniport
	//
	pAdapt->isSecondary		=	FALSE;	// 默认为主不是副
	pAdapt->pPrimaryAdapt	=	pAdapt;	// 默认主adapter为自己
	pAdapt->pSecondaryAdapt	=	pAdapt;	// 默认副adapter也为自己

	//————————————————————————————————————
	//	调用自定义函数MPBundleSearchAndSetSecondary设置下一个adapter
	//
	BundleStatus  =	MPBundleSearchAndSetSecondary (pAdapt);

	//————————————————————————————————————
	//	将本身加入到全局的Adapter列表，在操作全局变量时，为了使多进程间同步
	//	使用KeAcquireSpinLock和KeReleaseSpinLock进行加锁和解锁。
	//
	KeAcquireSpinLock (&pAdapt->SpinLock, &OldIrql);
	pAdapt->Next	= pAdaptList;
	pAdaptList		= pAdapt;
	KeReleaseSpinLock (&pAdapt->SpinLock, OldIrql);
		
	//————————————————————————————————————
	//	当前中间驱动程序初始化完成，设置返回的状态值。
	//
	Status = NDIS_STATUS_SUCCESS;
	DBGPRINT("<== Passthru Initialize Miniport\n");
	return Status;
}

/*——————————————————————————————————————
	查询驱动或者网卡的性能和状态。详情请参阅DDK文档关于
	MiniportQueryInformation的说明。
*/
NDIS_STATUS
MPQueryInformation(
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	NDIS_OID		Oid,
	IN	PVOID			InformationBuffer,
	IN	ULONG			InformationBufferLength,
	OUT PULONG			BytesWritten,
	OUT PULONG			BytesNeeded
)
{
	//————————————————————————————————————
	// 调用自定义函数MPQueryOrSetInformation查询驱动或者网卡的性能和状态。
	//
	return MPQueryOrSetInformation(
						TRUE,
						MiniportAdapterContext,
						Oid,
						InformationBuffer,
						InformationBufferLength,
						BytesWritten,
						BytesNeeded
						);
}

/*——————————————————————————————————————
	设置驱动或者网卡的性能和状态。详情请参阅DDK文档关于
	MiniportSetInformation的说明。
*/
NDIS_STATUS
MPSetInformation(
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	NDIS_OID		Oid,
	IN	PVOID			InformationBuffer,
	IN	ULONG			InformationBufferLength,
	OUT PULONG			BytesRead,
	OUT PULONG			BytesNeeded
)
{
	//————————————————————————————————————
	// 调用自定义函数MPQueryOrSetInformation设置驱动或者网卡的性能和状态。
	//
	return MPQueryOrSetInformation(
						FALSE,
						MiniportAdapterContext,
						Oid,
						InformationBuffer,
						InformationBufferLength,
						BytesRead,
						BytesNeeded
						);
}

/*——————————————————————————————————————
	查询或者设置驱动或者网卡的性能和状态。
*/
NDIS_STATUS
MPQueryOrSetInformation(
	IN  BOOLEAN			IsQuery,
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	NDIS_OID		Oid,
	IN	PVOID			InformationBuffer,
	IN	ULONG			InformationBufferLength,
	OUT PULONG			BytesWrittenOrRead,
	OUT PULONG			BytesNeeded
)
{
	PADAPT		pAdapt = (PADAPT)MiniportAdapterContext;
	NDIS_STATUS	Status = NDIS_STATUS_FAILURE;

	DBGPRINT("==>Passthru Miniport MPQueryInformation\n");

	//————————————————————————————————————
	// 这个循环语句并不是真正的循环执行，因为碰到它的条件语句while(FALSE)
	// 循环就会结束，这样做的目的是碰到break语句可以直接跳出循环继续执行，
	// 从而可以忽略break语句之后循环语句之内语句的执行。
	//
	do
	{
		if(IsQuery)
		{
			//————————————————————————————————
			// 这个OID是OID_PNP_QUERY_POWER，返回成功。
			//
			if (Oid == OID_PNP_QUERY_POWER)
			{
				Status = NDIS_STATUS_SUCCESS;
				break;
			}
		}
		else
		{
			//————————————————————————————————
			//	这个OID是OID_PNP_SET_POWER，调用自定义函数
			//	MPProcessSetPowerOid 设置是否阻断或者允许引入的请求。
			//
			if (Oid == OID_PNP_SET_POWER)
			{
				MPProcessSetPowerOid( &Status, pAdapt, InformationBuffer
					, InformationBufferLength, BytesWrittenOrRead, BytesNeeded);
				break;
			}
		}

		//——————————————————————————————————
		// 如果miniport的状态不是NdisDeviceStateD0，表示没有准备就绪返回失败。
		//
		if (pAdapt->MPDeviceState 
			> NdisDeviceStateD0 || pAdapt->StandingBy == TRUE)
		{
			Status = NDIS_STATUS_FAILURE;
			break;
		}
		
		//——————————————————————————————————
		// 根据Oid调用自定义函数MPIsSendOID判断是否为发送状态，如果是发送
		// 状态，将pAdapt指针设置为下一个adapter的指针。
		//
		if (IsQuery && MPIsSendOID(Oid))pAdapt = pAdapt->pSecondaryAdapt;

		//——————————————————————————————————
		// 设置adapter的结构数据。
		//
		if(IsQuery)
		{
			pAdapt->Request.RequestType = NdisRequestQueryInformation;
			pAdapt->Request.DATA.QUERY_INFORMATION.Oid = Oid;
			pAdapt->Request.DATA.QUERY_INFORMATION.InformationBuffer 
										= InformationBuffer;
			pAdapt->Request.DATA.QUERY_INFORMATION.InformationBufferLength 
										= InformationBufferLength;
		}
		else
		{
			pAdapt->Request.RequestType = NdisRequestSetInformation;
			pAdapt->Request.DATA.SET_INFORMATION.Oid = Oid;
			pAdapt->Request.DATA.SET_INFORMATION.InformationBuffer 
										= InformationBuffer;
			pAdapt->Request.DATA.SET_INFORMATION.InformationBufferLength 
										= InformationBufferLength;
		}
		pAdapt->BytesNeeded			= BytesNeeded;
		pAdapt->BytesReadOrWritten	= BytesWrittenOrRead;
		pAdapt->OutstandingRequests = TRUE;

		//——————————————————————————————————
		// 如果Protocol设备没有准备就绪则请求不能向下转发，只能设置成Pending
		// 状态，表示未转发。
		//
		if (pAdapt->PTDeviceState > NdisDeviceStateD0)
		{
			pAdapt->QueuedRequest = TRUE;
			Status = NDIS_STATUS_PENDING;
			break;
		}

		//——————————————————————————————————
		// 默认处理通常直接调用系统函数NdisRequest将请求向Miniport下层转发。
		//
		NdisRequest(&Status, pAdapt->BindingHandle,	&pAdapt->Request);

		//——————————————————————————————————
		// 如果转发返回成功，则将结果返回到发出请求的调用者。
		//
		if (Status == NDIS_STATUS_SUCCESS)
		{
			if(IsQuery)
			{
				*BytesWrittenOrRead 
					= pAdapt->Request.DATA.QUERY_INFORMATION.BytesWritten;
				*BytesNeeded 
					= pAdapt->Request.DATA.QUERY_INFORMATION.BytesNeeded;
			}
			else
			{
				*BytesWrittenOrRead 
					= pAdapt->Request.DATA.SET_INFORMATION.BytesRead;
				*BytesNeeded 
					= pAdapt->Request.DATA.SET_INFORMATION.BytesNeeded;
			}
		}

		//——————————————————————————————————
		// 如果Oid == OID_PNP_CAPABILITIES并且NdisRequest查询成功，调用自定义
		// 函数MPQueryPNPCapbilities填充adapter结构中必须的数值。
		//
		if (IsQuery && Oid == OID_PNP_CAPABILITIES 
			&& Status == NDIS_STATUS_SUCCESS)
			MPQueryPNPCapbilities(pAdapt, &Status);

		//——————————————————————————————————
		// 如果状态不是NDIS_STATUS_PENDING，设置OutstandingRequests为FALSE
		//
		if (Status != NDIS_STATUS_PENDING)
			pAdapt->OutstandingRequests = FALSE;

	} while (FALSE);

	return(Status);
}

/*——————————————————————————————————————
	填充adapter结构中pAdapt->Request.DATA.QUERY_INFORMATION.InformationBuffer
	和其它一些状态值。
*/
VOID
MPQueryPNPCapbilities(
	IN OUT	PADAPT			pAdapt,
	OUT		PNDIS_STATUS	pStatus
)
{
	PNDIS_PNP_CAPABILITIES			pPNPCapabilities;
	PNDIS_PM_WAKE_UP_CAPABILITIES	pPMstruct;

	DBGPRINT("==>Passthru Miniport MPQueryPNPCapbilities\n");

	if (pAdapt->Request.DATA.QUERY_INFORMATION.InformationBufferLength 
		>= sizeof(NDIS_PNP_CAPABILITIES))
	{
		//——————————————————————————————————
		// 先将InformationBuffer强制转换成PNDIS_PNP_CAPABILITIES类型，然后
		// 将指针赋值到pPMstruct，通过pPMstruct设置InformationBuffer的数据。
		//
		pPNPCapabilities = (PNDIS_PNP_CAPABILITIES)
			(pAdapt->Request.DATA.QUERY_INFORMATION.InformationBuffer);
		pPMstruct= & pPNPCapabilities->WakeUpCapabilities;
		pPMstruct->MinMagicPacketWakeUp = NdisDeviceStateUnspecified;
		pPMstruct->MinPatternWakeUp		= NdisDeviceStateUnspecified;
		pPMstruct->MinLinkChangeWakeUp	= NdisDeviceStateUnspecified;

		//——————————————————————————————————
		// 设置pAdapt结构的数值和设备状态
		//
		*pAdapt->BytesReadOrWritten = sizeof(NDIS_PNP_CAPABILITIES);
		*pAdapt->BytesNeeded = 0;
		pAdapt->MPDeviceState = NdisDeviceStateD0;
		pAdapt->PTDeviceState = NdisDeviceStateD0;

		*pStatus = NDIS_STATUS_SUCCESS;
	}
	else
	{
		*pAdapt->BytesNeeded= sizeof(NDIS_PNP_CAPABILITIES);
		*pStatus = NDIS_STATUS_RESOURCES;
	}
}

/*——————————————————————————————————————
	设置pAdapt的StandingBy数值，来决定阻断或者允许引入的请求。
*/
VOID
MPProcessSetPowerOid(
	IN OUT PNDIS_STATUS     pNdisStatus,
	IN  PADAPT				pAdapt,
	IN	PVOID				InformationBuffer,
	IN	ULONG				InformationBufferLength,
	OUT PULONG				BytesRead,
	OUT PULONG				BytesNeeded
)
{
	NDIS_DEVICE_POWER_STATE NewDeviceState;

	DBGPRINT ("==>MPProcessSetPowerOid"); 
	ASSERT (InformationBuffer != NULL);
	NewDeviceState	= (*(PNDIS_DEVICE_POWER_STATE)InformationBuffer);
	*pNdisStatus	= NDIS_STATUS_FAILURE;

	do 
	{
		//——————————————————————————————————
		// 检查是否是无效的长度
		//
		if (InformationBufferLength < sizeof(NDIS_DEVICE_POWER_STATE))
		{
			*pNdisStatus = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		//——————————————————————————————————
		// 检查是否是无效的设备状态
		//
		if ((pAdapt->MPDeviceState > NdisDeviceStateD0) 
			&& (NewDeviceState != NdisDeviceStateD0))
		{
			ASSERT (!(pAdapt->MPDeviceState > NdisDeviceStateD0) 
				&& (NewDeviceState != NdisDeviceStateD0));

			*pNdisStatus = NDIS_STATUS_FAILURE;
			break;
		}	

		//——————————————————————————————————
		// 如果miniport从On(D0)状态转换成Low Power状态(>D0),设置StandingBy
		// 为TRUE，阻挡所有引入的请求。
		//
		if (pAdapt->MPDeviceState == NdisDeviceStateD0 
			&& NewDeviceState > NdisDeviceStateD0)
			pAdapt->StandingBy = TRUE;

		//——————————————————————————————————
		// 如果miniport从Low Power状态(>D0)转换成On(D0)状态,设置StandingBy
		// 为FALSE，所有引入的请求将挂起，直到物理Miniport打开(turns ON)。
		//
		if (pAdapt->MPDeviceState > NdisDeviceStateD0 
			&&  NewDeviceState == NdisDeviceStateD0)
			pAdapt->StandingBy = FALSE;
		
		//——————————————————————————————————
		// 更新pAdapt结构中的状态成员变量值
		//
		pAdapt->MPDeviceState = NewDeviceState;
		
		*pNdisStatus = NDIS_STATUS_SUCCESS;
	

	} while (FALSE);	
		
	if (*pNdisStatus == NDIS_STATUS_SUCCESS)
	{
		*BytesRead	= sizeof(NDIS_DEVICE_POWER_STATE);
		*BytesNeeded = 0;
	}
	else
	{
		*BytesRead = 0;
		*BytesNeeded = sizeof (NDIS_DEVICE_POWER_STATE);
	}

	DBGPRINT ("<==MPProcessSetPowerOid"); 
}


/*——————————————————————————————————————
	Miniport必需的函数，当设备停止时释放相应的资源。详情请参阅DDK文档关于
	MiniportHalt函数的解释。
*/
VOID
MPHalt(
	IN	NDIS_HANDLE	MiniportAdapterContext
)
{
	PADAPT			pAdapt = (PADAPT)MiniportAdapterContext;
	NDIS_STATUS		Status;
	PADAPT			pCursor, *ppCursor;
	PADAPT			pPromoteAdapt = NULL;
	KIRQL			OldIrql;

	DBGPRINT("==>Passthru Miniport MPHalt\n");
	
	//————————————————————————————————————
	// 从全局列表中移除pAdapt。
	//
	// 在操作全局列表时必须保持多进程间同步
	//
	KeAcquireSpinLock (&pAdapt->SpinLock, &OldIrql);

	//————————————————————————————————————
	// 从列表中移除pAdapt。
	//
	for (ppCursor = &pAdaptList; *ppCursor != NULL; ppCursor = &(*ppCursor)->Next)
	{
		if (*ppCursor == pAdapt)
		{
			*ppCursor = pAdapt->Next;
			break;
		}
	}

	//————————————————————————————————————
	// 从列表里移除所有指向pAdapt的指针。
	//
	for (pCursor = pAdaptList; pCursor != NULL; pCursor = pCursor->Next)
	{
		//——————————————————————————————————
		// 在全局列表里的指针或许无效，首先对Primary进行检查
		//
		if (pCursor->pPrimaryAdapt == pAdapt)
		{
			ASSERT (pCursor->isSecondary == TRUE);
			pPromoteAdapt = pCursor;
		}

		//——————————————————————————————————
		// 检查下一个
		//
		if (pCursor->pSecondaryAdapt == pAdapt)
		{
			ASSERT(pCursor->isSecondary == FALSE); 
			pCursor->pSecondaryAdapt = pCursor;
		}
	}
	KeReleaseSpinLock (&pAdapt->SpinLock, OldIrql);

	//————————————————————————————————————
	// 如果miniport需要保留pPromoteAdapt调用自定义函数MPPromoteSecondary
	// 保留实例。
	//
	if (pPromoteAdapt != NULL)
		MPPromoteSecondary(pPromoteAdapt);

	//————————————————————————————————————
	// 如果有一个无效的绑定，关闭miniport下面的protocol。
	//
	if (pAdapt->BindingHandle != NULL)
	{
		//——————————————————————————————————
		// 关闭miniport下面的protocol并等待结束。
		//
		NdisResetEvent(&pAdapt->Event);
		NdisCloseAdapter(&Status, pAdapt->BindingHandle);
		if (Status == NDIS_STATUS_PENDING)
		{
			NdisWaitEvent(&pAdapt->Event, 0);
			Status = pAdapt->Status;
		}
		ASSERT (Status == NDIS_STATUS_SUCCESS);
		pAdapt->BindingHandle = NULL;
	}

	//————————————————————————————————————
	// 释放资源
	//
	NdisFreePacketPool(pAdapt->SendPacketPoolHandle);
	NdisFreePacketPool(pAdapt->RecvPacketPoolHandle);
	NdisFreeMemory(pAdapt->BundleUniString.Buffer, MAX_BUNDLEID_LENGTH,0);
	NdisFreeMemory(pAdapt, sizeof(ADAPT), 0);

	DBGPRINT("<==Passthru Minport Halt\n");
}

/*——————————————————————————————————————
	Miniport必需的函数，复位操作，这里我们什么都不需要做。详情请参阅DDK
	文档关于MiniportReset的说明。
*/
NDIS_STATUS
MPReset(
	OUT PBOOLEAN				AddressingReset,
	IN	NDIS_HANDLE				MiniportAdapterContext
)
{
	PADAPT	pAdapt = (PADAPT)MiniportAdapterContext;
	DBGPRINT("==>Passthru Miniport MPReset\n");
	*AddressingReset = FALSE;
	return(NDIS_STATUS_SUCCESS);
}

/*——————————————————————————————————————
	遍历全局列表查找并设置第二个设备的绑定
*/
NDIS_STATUS
MPBundleSearchAndSetSecondary(
	IN	PADAPT			pAdapt
)
{
	NDIS_STATUS		Status		= NDIS_STATUS_FAILURE;
	NDIS_STRING		NoBundle	= NDIS_STRING_CONST ("<no-bundle>");
	PADAPT			pCursor		= NULL;
	PADAPT			pPrimary	= NULL;
	KIRQL			OldIrql;

	DBGPRINT("==>Passthru Miniport MPBundleSearchAndSetSecondary\n");
	
	do
	{
		//——————————————————————————————————
		// 如果bundle == '<bundle id>'那么这个miniport将不是任何绑定的一部分
		//
		if (NdisEqualUnicodeString(&NoBundle, &pAdapt->BundleUniString, TRUE))
		{
			Status = NDIS_STATUS_SUCCESS;
			break;
		}

		//——————————————————————————————————
		// 如果绑定标识为空字符串，这个miniport不是任何绑定的一部分。
		//
		if (pAdapt->BundleUniString.Length == 0)
		{
			Status = NDIS_STATUS_SUCCESS;
			break;
		}

		//——————————————————————————————————
		// 加同步锁。
		//
		KeAcquireSpinLock (&pAdapt->SpinLock, &OldIrql);

		//——————————————————————————————————
		// 查找全局链表中具有相同绑定ID的设备。
		//
		for (pCursor = pAdaptList; pCursor != NULL; pCursor = pCursor->Next)
		{
			if (pCursor == pAdapt)
			{
				//——————————————————————————————
				// 如果指向自己跳过。
				//
				continue;
			}

			//————————————————————————————————
			// 如果匹配将第二个设置为当前的pAdapt。
			//
			if (NdisEqualUnicodeString(
				&pCursor->BundleUniString, &pAdapt->BundleUniString, TRUE))
			{
				//——————————————————————————————
				// 确认这是一个绑定的主设备。
				//
				ASSERT (pCursor->pSecondaryAdapt 
					== pCursor && pCursor->isSecondary == FALSE);
				pPrimary = pCursor;
				break;
			}
		}

		//——————————————————————————————————
		// 解同步锁。
		//
		KeReleaseSpinLock (&pAdapt->SpinLock, OldIrql);

		//——————————————————————————————————
		// 调用自定义函数MPSetMiniportSecondary进行设置。
		//
		if (pPrimary != NULL)
		{
			Status = MPSetMiniportSecondary (pAdapt, pPrimary);
			ASSERT (Status == NDIS_STATUS_SUCCESS);
		}

		//——————————————————————————————————
		// 成功的搜索了列表尽管没有找到绑定仍然返回成功。
		//
		Status = NDIS_STATUS_SUCCESS;

	} while (FALSE) ;

	return Status;
}

/*——————————————————————————————————————
	调用NdisMSetMiniportSecondary设置绑定并且改变成员变量的状态值
*/
NDIS_STATUS
MPSetMiniportSecondary (
	IN	PADAPT		Secondary,
	IN	PADAPT		Primary
	)
{
	NDIS_STATUS	Status = NDIS_STATUS_SUCCESS;
	DBGPRINT("==>Passthru Miniport MPSetMiniportSecondary\n");
	
	//————————————————————————————————————
	// 保证主设备不是其它绑定的一部分
	//
	ASSERT (Primary != Secondary);
	ASSERT (Primary->isSecondary == 0);
	ASSERT (Primary->pSecondaryAdapt == Primary);

	DBGPRINT ("Calling NdisMSetSecondary API on the two handles\n");
	Status = NdisMSetMiniportSecondary(Secondary->MiniportHandle,
									   Primary->MiniportHandle);
	ASSERT (Status == NDIS_STATUS_SUCCESS);

	if (Status == NDIS_STATUS_SUCCESS)
	{
		//——————————————————————————————————
		// 初始化LBFO变量来记录当前状态
		//
		Secondary->isSecondary		= TRUE;
		Secondary->pPrimaryAdapt	= Primary;
		Primary->pSecondaryAdapt	= Secondary;

		//——————————————————————————————————
		// 保证其它一些状态变量也由正确的状态值
		//
		Secondary->pSecondaryAdapt	= Secondary;
		Primary->pPrimaryAdapt		= Primary;
		Primary->isSecondary		= FALSE;
	}

	return Status;
}

/*——————————————————————————————————————
	从第二个设备升迁为主设备，保留升迁设备的实例
*/
NDIS_STATUS
MPPromoteSecondary(
	IN	PADAPT		pAdapt
)
{
	NDIS_STATUS Status = NdisMPromoteMiniport(pAdapt->MiniportHandle);
	ASSERT (Status == NDIS_STATUS_SUCCESS);
	if (Status == NDIS_STATUS_SUCCESS)
	{
		pAdapt->isSecondary		= FALSE;
		pAdapt->pPrimaryAdapt	= pAdapt;
		pAdapt->pSecondaryAdapt = pAdapt;
	}
	DBGPRINT ("<== MPPromoteMiniport\n");
	return Status;
}

/*——————————————————————————————————————
	根据OID判断是发送还是接收
*/
BOOLEAN
MPIsSendOID (
	IN	NDIS_OID	Oid
)
{
	BOOLEAN fIsSend = FALSE;	// 默认是一个接收OID
	DBGPRINT("==>Passthru Miniport MPIsSendOID\n");
	
	switch (Oid)
	{
		//——————————————————————————————————
		// 如果需要发送OID，设置fIsSend为TRUE;
		//
		case OID_GEN_TRANSMIT_BUFFER_SPACE :
		case OID_GEN_TRANSMIT_BLOCK_SIZE :
		case OID_GEN_MAXIMUM_TOTAL_SIZE :
		case OID_GEN_XMIT_OK :
		case OID_GEN_XMIT_ERROR :
		case OID_GEN_DIRECTED_BYTES_XMIT :
		case OID_GEN_DIRECTED_FRAMES_XMIT :
		case OID_GEN_MULTICAST_BYTES_XMIT :
		case OID_GEN_MULTICAST_FRAMES_XMIT :
		case OID_GEN_BROADCAST_BYTES_XMIT :
		case OID_GEN_BROADCAST_FRAMES_XMIT :
		case OID_GEN_TRANSMIT_QUEUE_LENGTH :
			fIsSend = TRUE;
			break;

		default:
			fIsSend = FALSE;
			break;
	}

	return fIsSend;
}

