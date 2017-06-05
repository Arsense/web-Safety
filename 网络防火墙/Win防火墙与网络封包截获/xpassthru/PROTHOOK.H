#ifndef PROTHOOK_H
#define PROTHOOK_H

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	Protocol �犢愃�方圻侏蕗苧
*/

extern
NDIS_STATUS
PtRegisterAsProtocol(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
	);

extern
VOID
PtOpenAdapterComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				Status,
	IN	NDIS_STATUS				OpenErrorStatus
	);

extern
VOID
PtCloseAdapterComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				Status
	);

extern
VOID
PtResetComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				Status
	);

extern
VOID
PtRequestComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_REQUEST			NdisRequest,
	IN	NDIS_STATUS				Status
	);

extern
VOID
PtStatus(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_STATUS				GeneralStatus,
	IN	PVOID					StatusBuffer,
	IN	UINT					StatusBufferSize
	);

extern
VOID
PtStatusComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext
	);

extern
VOID
PtSendComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status
	);

extern
VOID
PtTransferDataComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_PACKET			Packet,
	IN	NDIS_STATUS				Status,
	IN	UINT					BytesTransferred
	);

extern
NDIS_STATUS
PtReceive(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_HANDLE				MacReceiveContext,
	IN	PVOID					HeaderBuffer,
	IN	UINT					HeaderBufferSize,
	IN	PVOID					LookAheadBuffer,
	IN	UINT					LookaheadBufferSize,
	IN	UINT					PacketSize
	);

extern
VOID
PtReceiveComplete(
	IN	NDIS_HANDLE				ProtocolBindingContext
	);

extern
INT
PtReceivePacket(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNDIS_PACKET			Packet
	);

extern
VOID
PtBindAdapter(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				BindContext,
	IN	PNDIS_STRING			DeviceName,
	IN	PVOID					SystemSpecific1,
	IN	PVOID					SystemSpecific2
	);

extern
VOID
PtUnbindAdapter(
	OUT	PNDIS_STATUS			Status,
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	NDIS_HANDLE				UnbindContext
	);
	
VOID
PtUnload(
	IN	PDRIVER_OBJECT			DriverObject
	);

extern 
NDIS_STATUS
PtPNPHandler(
	IN	NDIS_HANDLE				ProtocolBindingContext,
	IN	PNET_PNP_EVENT			pNetPnPEvent
	);

NDIS_STATUS
PtPnPNetEventReconfigure(
	IN	PADAPT			pAdapt,
	IN	PCWSTR			pBundleString
	);	

NDIS_STATUS 
PtPnPNetEventSetPower (
	IN PADAPT					pAdapt,
	IN	PNET_PNP_EVENT			pNetPnPEvent
	);

#endif //PROTHOOK_H