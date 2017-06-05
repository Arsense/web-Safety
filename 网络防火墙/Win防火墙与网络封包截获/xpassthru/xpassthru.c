/*——————————————————————————————————————
	文件：xpassthru.c
	工程：xpassthru
	概述：演示NDIS中间驱动程序输出封包结构信息
	版权所有(c) 2001-2002 X 工作室
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

/*——————————————————————————————————————
	用NDIS_INIT_FUNCTION宏标识DriverEntry函数只运行一次
*/
#pragma NDIS_INIT_FUNCTION(DriverEntry)

NDIS_PHYSICAL_ADDRESS	HighestAcceptableMax \
						= NDIS_PHYSICAL_ADDRESS_CONST(-1, -1);
NDIS_HANDLE				ProtHandle		= NULL;
NDIS_HANDLE				DriverHandle	= NULL;
NDIS_MEDIUM				MediumArray[3]	=
						{
							NdisMedium802_3,	// Ethernet
							NdisMedium802_5,	// Token-ring
							NdisMediumFddi		// Fddi
						};
PADAPT					pAdaptList		= NULL;
NDIS_HANDLE				WrapperHandle	= NULL;

NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
)
{
	NDIS_STATUS						Status;
	PNDIS_CONFIGURATION_PARAMETER	Param;

	DBGPRINT("==> DriverEntry...\n");

	NdisMInitializeWrapper(&WrapperHandle, DriverObject, RegistryPath, NULL);

	Status = MPRegisterAsMiniport(DriverObject, RegistryPath);
	ASSERT(Status == NDIS_STATUS_SUCCESS);

	NdisMRegisterUnloadHandler(WrapperHandle, PtUnload);

	Status = PtRegisterAsProtocol(DriverObject, RegistryPath);
	ASSERT(Status == NDIS_STATUS_SUCCESS);

	NdisIMAssociateMiniport(DriverHandle, ProtHandle);
	DBGPRINT("<== NdisIMAssociateMiniport.\n");

	return(Status);
}
