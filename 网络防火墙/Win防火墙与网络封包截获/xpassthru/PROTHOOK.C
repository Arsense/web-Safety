/*——————————————————————————————————————
	文件：protocol.c
	工程：xpassthru
	概述：演示NDIS中间驱动程序输出封包结构信息
	版权所有(c) 2001-2002 X 工作室
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

#define MAX_PACKET_POOL_SIZE 0x0000FFFF
#define MIN_PACKET_POOL_SIZE 0x000000FF

NDIS_STATUS
PtRegisterAsProtocol(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
)
{
	NDIS_STATUS						Status;
	NDIS_PROTOCOL_CHARACTERISTICS	ProtocolStruct;
	NDIS_STRING						Name;

	NdisZeroMemory(&ProtocolStruct, sizeof(NDIS_PROTOCOL_CHARACTERISTICS));
	ProtocolStruct.MajorNdisVersion = 4;
	ProtocolStruct.MinorNdisVersion = 0;
	NdisInitUnicodeString(&Name, L"xfilter");	// Protocol名称
	ProtocolStruct.Name				= Name;
	ProtocolStruct.OpenAdapterCompleteHandler	= PtOpenAdapterComplete;
	ProtocolStruct.CloseAdapterCompleteHandler	= PtCloseAdapterComplete;
	ProtocolStruct.SendCompleteHandler			= PtSendComplete;
	ProtocolStruct.TransferDataCompleteHandler	= PtTransferDataComplete;
	ProtocolStruct.ResetCompleteHandler			= PtResetComplete;
	ProtocolStruct.RequestCompleteHandler		= PtRequestComplete;
	ProtocolStruct.ReceiveHandler				= PtReceive;
	ProtocolStruct.ReceiveCompleteHandler		= PtReceiveComplete;
	ProtocolStruct.StatusHandler				= PtStatus;
	ProtocolStruct.StatusCompleteHandler		= PtStatusComplete;
	ProtocolStruct.BindAdapterHandler			= PtBindAdapter;
	ProtocolStruct.UnbindAdapterHandler			= PtUnbindAdapter;
	ProtocolStruct.UnloadHandler				= NULL;
	ProtocolStruct.ReceivePacketHandler			= PtReceivePacket;
	ProtocolStruct.PnPEventHandler				= PtPNPHandler;

	NdisRegisterProtocol(&Status, &ProtHandle
		, &ProtocolStruct, sizeof(NDIS_PROTOCOL_CHARACTERISTICS));

	DBGPRINT("<== NdisRegisterProtocol.\n");
	return Status;
}

/*——————————————————————————————————————
	被NDIS调用将Protocol驱动绑定到下层的miniport设备。详情请参阅DDK文档关于
	ProtocolBindAdapter函数的解释。
*/
VOID
PtBindAdapter(
	OUT PNDIS_STATUS			Status,
	IN  NDIS_HANDLE				BindContext,
	IN  PNDIS_STRING			DeviceName,
	IN  PVOID					SystemSpecific1,
	IN  PVOID					SystemSpecific2
)
{
	NDIS_HANDLE			ConfigHandle = NULL;
	PNDIS_CONFIGURATION_PARAMETER	Param;
	NDIS_STRING			DeviceStr	= NDIS_STRING_CONST("UpperBindings");
	PADAPT				pAdapt		= NULL;
	NDIS_STATUS			Sts;
	UINT				MediumIndex;

	PNDIS_CONFIGURATION_PARAMETER	BundleParam;
	NDIS_STRING		BundleStr = NDIS_STRING_CONST("BundleId");
	NDIS_STATUS		BundleStatus;
	
	DBGPRINT("==> Passthru Protocol Initialize PtBindAdapter\n");

	do
	{
		//——————————————————————————————————
		// 打开Protocol配置得到句柄ConfigHandle
		//
		NdisOpenProtocolConfiguration(Status, &ConfigHandle, SystemSpecific1);
		if (*Status != NDIS_STATUS_SUCCESS)	break;

		//——————————————————————————————————
		// 利用句柄ConfigHandle读出配置参数到Param结构
		//
		NdisReadConfiguration(Status, &Param
			, ConfigHandle, &DeviceStr, NdisParameterString);
		if (*Status != NDIS_STATUS_SUCCESS)	break;

		//——————————————————————————————————
		// 为Adapter结构分配内存
		//
		NdisAllocateMemoryWithTag(&pAdapt, sizeof(ADAPT), TAG);
		if (pAdapt == NULL)
		{
			*Status = NDIS_STATUS_RESOURCES;
		  	break;
		}

		//——————————————————————————————————
		// 初始化Adapter结构数据为0
		//
		NdisZeroMemory(pAdapt, sizeof(ADAPT));

		//——————————————————————————————————
		// 初始化pAdapt->BundleUniString.Buffer，这个缓冲区将用来保存从注册
		// 表读出来的数据。
		//
		NdisAllocateMemoryWithTag( 
			&(pAdapt->BundleUniString.Buffer), MAX_BUNDLEID_LENGTH ,TAG);
		if (pAdapt->BundleUniString.Buffer == NULL)
		{
		  	*Status = NDIS_STATUS_RESOURCES;
		  	break;
		}

		pAdapt->BundleUniString.MaximumLength = MAX_BUNDLEID_LENGTH ;

		NdisReadConfiguration(&BundleStatus
			, &BundleParam, ConfigHandle, &BundleStr, NdisParameterString);

		if (BundleStatus == NDIS_STATUS_SUCCESS)
		{
			//————————————————————————————————
			// 复制绑定标识到自己初始化的缓冲区pAdapt->BundleUniString
			//
		  	ASSERT(pAdapt->BundleUniString.MaximumLength  
				>= BundleParam->ParameterData.StringData.Length);

		  	pAdapt->BundleUniString.Length 
				= BundleParam->ParameterData.StringData.Length;

		  	RtlCopyUnicodeString(
				&pAdapt->BundleUniString, &BundleParam->ParameterData.StringData);
		}
		else 
		{
			//————————————————————————————————
			// 没有在注册表里找到绑定ID，设定BundleUniString为"<no-bundle>"
			//
			NDIS_STRING NoBundle = NDIS_STRING_CONST ("<no-bundle>");
			RtlCopyUnicodeString(&pAdapt->BundleUniString, &NoBundle);
		}

		//——————————————————————————————————
		// 初始化Event并加同步锁
		//
		NdisInitializeEvent(&pAdapt->Event);
		KeInitializeSpinLock(&pAdapt->SpinLock);

		//——————————————————————————————————
		// 为发送封包分配内存缓冲池
		//
		NdisAllocatePacketPoolEx(Status,
		  	 &pAdapt->SendPacketPoolHandle,
		  	 MIN_PACKET_POOL_SIZE,
		  	 MAX_PACKET_POOL_SIZE - MIN_PACKET_POOL_SIZE,
		  	 sizeof(RSVD));
		if (*Status != NDIS_STATUS_SUCCESS)	break;

		//——————————————————————————————————
		// 为接收封包分配内存缓冲池
		//
		NdisAllocatePacketPoolEx(Status,
		  	 &pAdapt->RecvPacketPoolHandle,
		  	 MIN_PACKET_POOL_SIZE,
		  	 MAX_PACKET_POOL_SIZE - MIN_PACKET_POOL_SIZE,
		  	 sizeof(RSVD));
		if (*Status != NDIS_STATUS_SUCCESS)	break;

		//——————————————————————————————————
		// 打开底层adapter设备并且完成初始化
		//
		NdisOpenAdapter(Status,	&Sts, &pAdapt->BindingHandle, &MediumIndex,
		  	MediumArray, sizeof(MediumArray)/sizeof(NDIS_MEDIUM), ProtHandle,
		  	pAdapt,	DeviceName,	0, NULL);
		if(*Status == NDIS_STATUS_PENDING)
		{
		  	NdisWaitEvent(&pAdapt->Event, 0);
		  	*Status = pAdapt->Status;
		}
		if(*Status != NDIS_STATUS_SUCCESS) break;
		pAdapt->Medium = MediumArray[MediumIndex];

		//——————————————————————————————————
		// 告诉NDIS初始化我们的miniport设备
		//
		NdisIMInitializeDeviceInstanceEx(
			DriverHandle, &Param->ParameterData.StringData, pAdapt);

	} while(FALSE);

	if (ConfigHandle != NULL)
		NdisCloseConfiguration(ConfigHandle);

	if (*Status != NDIS_STATUS_SUCCESS)
	{
		if (pAdapt != NULL)
		{
			if (pAdapt->SendPacketPoolHandle != NULL)
				 NdisFreePacketPool(pAdapt->SendPacketPoolHandle);

			if (pAdapt->RecvPacketPoolHandle != NULL)
				 NdisFreePacketPool(pAdapt->RecvPacketPoolHandle);

			NdisFreeMemory(pAdapt, sizeof(ADAPT), 0);
		}
	}

	DBGPRINT("<== Passthru Protocol Initialize\n");
}

/*——————————————————————————————————————
	在PtBindAdapter里调用NdisOpenAdapter完成绑定后执行，用来解除阻塞状态。
	详情请参阅DDK文档关于ProtocolOpenAdapterComplete函数的解释。
*/
VOID
PtOpenAdapterComplete(
	IN  NDIS_HANDLE			 ProtocolBindingContext,
	IN  NDIS_STATUS			 Status,
	IN  NDIS_STATUS			 OpenErrorStatus
)
{
	PADAPT pAdapt	=(PADAPT)ProtocolBindingContext;
	pAdapt->Status	= Status;
	DBGPRINT("==> Passthru PtOpenAdapterComplete\n");
	NdisSetEvent(&pAdapt->Event);
}

/*——————————————————————————————————————
	当我需要解除与底层adapter设备的绑定时由NDIS调用，这个函数使用miniport的
	HaltHandler共享功能函数，这段代码应该保证NdisCloseAdapter和NdisFreeMemory
	这两个函数只能被调用一次。
*/
VOID
PtUnbindAdapter(
	OUT PNDIS_STATUS		Status,
	IN  NDIS_HANDLE			ProtocolBindingContext,
	IN  NDIS_HANDLE			UnbindContext
	)
{
	PADAPT		pAdapt			= (PADAPT)ProtocolBindingContext;
	NDIS_HANDLE BindingHandle	= pAdapt->BindingHandle;

	DBGPRINT("==> Passthru PtUnbindAdapter\n");

	if (pAdapt->QueuedRequest == TRUE)
	{
		pAdapt->QueuedRequest = FALSE;
		PtRequestComplete (pAdapt, &pAdapt->Request, NDIS_STATUS_FAILURE );
	}
	
	//————————————————————————————————————
	// 调用NDIS移除设备句柄，通常我们需要在HaltHandler做许多工作。
	//
	// 如果passthru的miniport的halt handler已经被调用或者如果中间设备根本没有
	// 初始化，这个句柄将为NULL。
	//
	if(pAdapt->MiniportHandle != NULL)
	{
		*Status = NdisIMDeInitializeDeviceInstance(pAdapt->MiniportHandle);
		if(*Status != NDIS_STATUS_SUCCESS) *Status = NDIS_STATUS_FAILURE;
	}
	else
	{
		//——————————————————————————————————
		// 关闭底层绑定并且释放内存
		//
		if(pAdapt->BindingHandle != NULL)
		{
			NdisResetEvent(&pAdapt->Event);
			NdisCloseAdapter(Status, pAdapt->BindingHandle);

			//————————————————————————————————
			// 等待结束
			//
			if(*Status == NDIS_STATUS_PENDING)
			{
				 NdisWaitEvent(&pAdapt->Event, 0);
				 *Status = pAdapt->Status;
			}
		}
		else
		{
			//————————————————————————————————
			// Miniport句柄和绑定句柄不应该为NULL，为NULL则设置错误状态。
			//
			*Status = NDIS_STATUS_FAILURE;
			ASSERT(0);
		}

		//——————————————————————————————————
		// 在这里释放内存
		//
		NdisFreeMemory(pAdapt, sizeof(ADAPT), 0);
	}

	DBGPRINT("<==Passthru UnbindAdapter\n");
}

/*——————————————————————————————————————
	这是一个可选函数，在这里调用NdisDeregisterProtocol释放执行
	NdisRegisterProtocol注册protocol时使用的资源.
*/
VOID
PtUnload(
	IN	PDRIVER_OBJECT		DriverObject
	)
{
	NDIS_STATUS Status;
	NdisDeregisterProtocol(&Status, ProtHandle);
	DBGPRINT("<== Passthru Protocol PtUnload\n");
}

/*——————————————————————————————————————
	关闭adapter完成后的处理函数
*/
VOID
PtCloseAdapterComplete(
	IN	NDIS_HANDLE			ProtocolBindingContext,
	IN	NDIS_STATUS			Status
	)
{
	PADAPT pAdapt	= (PADAPT)ProtocolBindingContext;
	pAdapt->Status	= Status;
	NdisSetEvent(&pAdapt->Event);
	DBGPRINT("<== Passthru Protocol PtCloseAdapterComplete\n");
}

/*——————————————————————————————————————
	复位操作完成后的处理函数，由于Passthru不需要复位，所以这里什么都没有做。
*/
VOID
PtResetComplete(
	IN  NDIS_HANDLE			ProtocolBindingContext,
	IN  NDIS_STATUS			Status
	)
{
	PADAPT	pAdapt =(PADAPT)ProtocolBindingContext;
	ASSERT(0);
	DBGPRINT("<== Passthru Protocol PtResetComplete\n");
}

/*——————————————————————————————————————
	完成请求的操作函数，所有的OID被完整的发送到同一个请求的miniport设备
	如果Oid == OID_PNP_QUERY_POWER那么数据结构都需要以 entries = 
	NdisDeviceStateUnspecified的形式返回
*/
VOID
PtRequestComplete(
	IN  NDIS_HANDLE			ProtocolBindingContext,
	IN  PNDIS_REQUEST		NdisRequest,
	IN  NDIS_STATUS			Status
)
{
	PADAPT		pAdapt	= (PADAPT)ProtocolBindingContext;
	NDIS_OID	Oid		= pAdapt->Request.DATA.SET_INFORMATION.Oid ;

	//————————————————————————————————————
	// 根据请求来源改变pAdapt
	//
	if(MPIsSendOID(Oid))
	  pAdapt = pAdapt->pPrimaryAdapt;

	//————————————————————————————————————
	// 设置我们的请求不再紧急
	//
	pAdapt->OutstandingRequests = FALSE;

	//————————————————————————————————————
	// 如果需要，完成设置或者查询，并且为OID_PNP_CAPABILITIES缓冲区填充
	//
	switch(NdisRequest->RequestType)
	{
		case NdisRequestQueryInformation:
			ASSERT(Oid != OID_PNP_QUERY_POWER);
		
			//————————————————————————————————
			// 如果oid == OID_PNP_CAPABILITIES并且查询成功便填充必须的数值
			//
			if(Oid == OID_PNP_CAPABILITIES && Status == NDIS_STATUS_SUCCESS)
				MPQueryPNPCapbilities(pAdapt,&Status);

			*pAdapt->BytesReadOrWritten 
				= NdisRequest->DATA.QUERY_INFORMATION.BytesWritten;
			*pAdapt->BytesNeeded 
				= NdisRequest->DATA.QUERY_INFORMATION.BytesNeeded;
			NdisMQueryInformationComplete(pAdapt->MiniportHandle, Status);
			break;

		case NdisRequestSetInformation:
			ASSERT( Oid != OID_PNP_SET_POWER);
			*pAdapt->BytesReadOrWritten 
				= NdisRequest->DATA.SET_INFORMATION.BytesRead;
			*pAdapt->BytesNeeded 
				= NdisRequest->DATA.SET_INFORMATION.BytesNeeded;
			NdisMSetInformationComplete(pAdapt->MiniportHandle, Status);
			break;

		default:
			ASSERT(0);
			break;
	}
	DBGPRINT("<== Passthru Protocol PtRequestComplete\n");
}

/*——————————————————————————————————————
	调用NdisMIndicateStatus完成状态标识的传递
*/
VOID
PtStatus(
	IN  NDIS_HANDLE			ProtocolBindingContext,
	IN  NDIS_STATUS			GeneralStatus,
	IN  PVOID				StatusBuffer,
	IN  UINT				StatusBufferSize
)
{
	PADAPT	  pAdapt =(PADAPT)ProtocolBindingContext;

	//————————————————————————————————————
	// 在miniport初始化前得到的状态指示需要跳过，如果passthru没有设置为ON，
	// 我们不传递状态标识
	//
	if(pAdapt->MiniportHandle != NULL  &&
	  pAdapt->MPDeviceState == NdisDeviceStateD0 &&
	  pAdapt->PTDeviceState == NdisDeviceStateD0 )	
	{
		  NdisMIndicateStatus(pAdapt->MiniportHandle
			  , GeneralStatus, StatusBuffer, StatusBufferSize);
	}

	DBGPRINT("<== Passthru Protocol PtStatus\n");
}

/*——————————————————————————————————————
	状态标识传递完成后的处理函数
*/
VOID
PtStatusComplete(
	IN	NDIS_HANDLE			ProtocolBindingContext
)
{
	PADAPT	  pAdapt =(PADAPT)ProtocolBindingContext;

	//————————————————————————————————————
	// 在miniport初始化前得到的状态指示需要跳过
	//
	if(pAdapt->MiniportHandle != NULL  &&
	  pAdapt->MPDeviceState == NdisDeviceStateD0 &&
	  pAdapt->PTDeviceState == NdisDeviceStateD0 )	
	{
		  NdisMIndicateStatusComplete(pAdapt->MiniportHandle);
	}
	DBGPRINT("<== Passthru Protocol PtStatusComplete\n");
}

/*——————————————————————————————————————
	这是一个Protocol PNP操作函数。所有PNP相关的OID请求被送到这个函数进行处理
*/
NDIS_STATUS
PtPNPHandler(
	IN	NDIS_HANDLE		ProtocolBindingContext,
	IN	PNET_PNP_EVENT	pNetPnPEvent
)
{
	PADAPT		pAdapt  =(PADAPT)ProtocolBindingContext;
	NDIS_STATUS	Status  = NDIS_STATUS_SUCCESS;

	DBGPRINT("==> Passthru Protocol PtPNPHandler\n");

	//————————————————————————————————————
	// 用当在系统中所有的实体需要改变时这里将执行
	//

	switch(pNetPnPEvent->NetEvent)
	{
	 case  NetEventSetPower :
	    Status = PtPnPNetEventSetPower(pAdapt, pNetPnPEvent);
	    break;

	 case NetEventReconfigure :
	    Status  = PtPnPNetEventReconfigure(pAdapt, (PCWSTR)pNetPnPEvent->Buffer);
	    break;

	 default :
	    Status  = NDIS_STATUS_SUCCESS;
	    break;
	}

	return Status;
}

/*——————————————————————————————————————
	只要PNPNetEventReconfigure被触发，这个函数将被调用。这里protocol将从注
	册表读取绑定信息。
*/
NDIS_STATUS
PtPnPNetEventReconfigure(
	IN	PADAPT			pAdapt,
	IN	PCWSTR			pBundleString
)
{
	NDIS_STATUS	BundleStatus = NDIS_STATUS_SUCCESS;
	NDIS_STRING NewBundleUniString;
	
	DBGPRINT("==> Passthru Protocol PtPnPNetEventReconfigure\n");

	if(pAdapt == NULL)
	{
		NdisReEnumerateProtocolBindings (ProtHandle);		
		return BundleStatus;
	}
	if (pBundleString == NULL)
		return BundleStatus;

	NdisInitUnicodeString( &NewBundleUniString, pBundleString);

	do
	{
		//——————————————————————————————————
		// 如果绑定标识没有改变，不需要做任何处理。
		//
	    if(NdisEqualUnicodeString(
			&NewBundleUniString, &pAdapt->BundleUniString, TRUE))
	  	   break;

		//——————————————————————————————————
		// 有一个新的绑定ID，复制它并作一些需要的绑定工作。
		//
	    RtlCopyUnicodeString(&pAdapt->BundleUniString , &NewBundleUniString);

	    if(pAdapt->isSecondary)
		{
			PADAPT pPrimaryAdapt = pAdapt->pPrimaryAdapt;
			BundleStatus = MPPromoteSecondary(pAdapt);
			if(BundleStatus != NDIS_STATUS_SUCCESS)
			{
				 ASSERT(0);
				 break;
			}

			//————————————————————————————————
			// 重新设置主设备所有的成员变量。
			//
			pPrimaryAdapt->pPrimaryAdapt	= pPrimaryAdapt;
			pPrimaryAdapt->pSecondaryAdapt	= pPrimaryAdapt;
			pPrimaryAdapt->isSecondary		= FALSE;
		}
	    else
		{
			//————————————————————————————————
			// 绑定ID已经改变，如果我们绑定主设备，那吗我们需要设置第二个
			// 设备为主设备。
			//
			if(pAdapt->pSecondaryAdapt != pAdapt)
			{
				BundleStatus = MPPromoteSecondary(pAdapt->pSecondaryAdapt);
				if(BundleStatus != NDIS_STATUS_SUCCESS)
				{
				     ASSERT(0);
				     break;
				}
				pAdapt->pSecondaryAdapt = pAdapt;
				pAdapt->pPrimaryAdapt	= pAdapt;
				pAdapt->isSecondary		= FALSE ;
			}
		}

		//——————————————————————————————————
		// 如果做了一个新的绑定，需要设置当前设备为第二个设备
		//
	    BundleStatus = MPBundleSearchAndSetSecondary(pAdapt);

	} while(FALSE) ;

	DBGPRINT("<==PtPNPNetEventReconfigure\n");

	return BundleStatus;
}

/*——————————————————————————————————————
	设置电源状态到需要的级别，等待所有的发送和请求完成。
*/
NDIS_STATUS
PtPnPNetEventSetPower(
	IN	PADAPT			pAdapt,
	IN  PNET_PNP_EVENT	pNetPnPEvent
)
{
	PNDIS_DEVICE_POWER_STATE	pDeviceState; 
	NDIS_DEVICE_POWER_STATE		PrevDeviceState = pAdapt->PTDeviceState;  
	NDIS_STATUS					Status ;
	pDeviceState = (PNDIS_DEVICE_POWER_STATE)(pNetPnPEvent->Buffer);
	
	DBGPRINT("==> Passthru Protocol PtPnPNetEventSetPower\n");

	//————————————————————————————————————
	// 设置设备状态，这将阻断所有新的发送和接收请求
	//
	pAdapt->PTDeviceState = *pDeviceState;

	if(*pDeviceState > NdisDeviceStateD0)
	{
		//——————————————————————————————————
		// 如果物理miniport变成待命，所有进入的请求失败。
		//
	    if (PrevDeviceState == NdisDeviceStateD0)
           pAdapt->StandingBy = TRUE;

		//——————————————————————————————————
		// 等待直到发送完成。
		//
	    while(NdisPacketPoolUsage(pAdapt->SendPacketPoolHandle) != 0)
	  	   NdisMSleep(10);

		//——————————————————————————————————
		// 等待直到请求完成。
		//
	    while(pAdapt->OutstandingRequests == TRUE)
	  	   NdisMSleep(10);

		ASSERT(NdisPacketPoolUsage(pAdapt->SendPacketPoolHandle) == 0);
		ASSERT(pAdapt->OutstandingRequests == FALSE);
	}
	else
	{
		//——————————————————————————————————
		// protocol设备已经打开，一个挂起的请求必须完成。
		//
		if (pAdapt->QueuedRequest == TRUE)
		{
			pAdapt->QueuedRequest = FALSE;
			NdisRequest(&Status, pAdapt->BindingHandle, &pAdapt->Request);

			//————————————————————————————————
			// 下层的miniport同步的完成请求，passthru需要完成挂起的请求。
			//
			if (Status != NDIS_STATUS_PENDING)
				PtRequestComplete(pAdapt, &pAdapt->Request, Status);
		}

		//——————————————————————————————————
		// 如果物理miniport的状态为D0，清除标记
		//
		if (PrevDeviceState > NdisDeviceStateD0)
			pAdapt->StandingBy = FALSE;
	}

	Status = NDIS_STATUS_SUCCESS;

	return Status;
}
