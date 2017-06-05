#ifndef MINIHOOK_H
#define MINIHOOK_H
	
/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	Miniport �犢愃�方圻侏蕗苧
*/

extern
NDIS_STATUS
MPRegisterAsMiniport(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
	);

NDIS_STATUS
MPInitialize(
	OUT PNDIS_STATUS			OpenErrorStatus,
	OUT PUINT					SelectedMediumIndex,
	IN	PNDIS_MEDIUM			MediumArray,
	IN	UINT					MediumArraySize,
	IN	NDIS_HANDLE				MiniportAdapterHandle,
	IN	NDIS_HANDLE				WrapperConfigurationContext
	);

NDIS_STATUS
MPSendOnePacket(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PNDIS_PACKET			Packet,
	IN	UINT					Flags
	);

VOID
MPSendPackets(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
	);

NDIS_STATUS
MPSend(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PNDIS_PACKET			Packet,
	IN	UINT					Flags
	);

NDIS_STATUS
MPQueryInformation(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_OID				Oid,
	IN	PVOID					InformationBuffer,
	IN	ULONG					InformationBufferLength,
	OUT PULONG					BytesWritten,
	OUT PULONG					BytesNeeded
	);

NDIS_STATUS
MPSetInformation(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_OID				Oid,
	IN	PVOID					InformationBuffer,
	IN	ULONG					InformationBufferLength,
	OUT PULONG					BytesRead,
	OUT PULONG					BytesNeeded
	);

NDIS_STATUS
MPQueryOrSetInformation(
	IN  BOOLEAN					IsQuery,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_OID				Oid,
	IN	PVOID					InformationBuffer,
	IN	ULONG					InformationBufferLength,
	OUT PULONG					BytesWrittenOrRead,
	OUT PULONG					BytesNeeded
	);

VOID
MPReturnPacket(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PNDIS_PACKET			Packet
	);

NDIS_STATUS
MPTransferData(
	OUT PNDIS_PACKET			Packet,
	OUT PUINT					BytesTransferred,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				MiniportReceiveContext,
	IN	UINT					ByteOffset,
	IN	UINT					BytesToTransfer
	);

VOID
MPHalt(
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

NDIS_STATUS
MPReset(
	OUT PBOOLEAN				AddressingReset,
	IN	NDIS_HANDLE				MiniportAdapterContext
	);

VOID
MPQueryPNPCapbilities(  
	OUT	PADAPT					MiniportProtocolContext, 
	OUT	PNDIS_STATUS			Status
	);

NDIS_STATUS
MPSetMiniportSecondary ( 
	IN	PADAPT					Secondary, 
	IN	PADAPT					Primary
	);

BOOLEAN
MPIsSendOID (
	IN NDIS_OID					oid
	);

NDIS_STATUS 
MPPromoteSecondary ( 
	IN	PADAPT					pAdapt 
	);

NDIS_STATUS 
MPBundleSearchAndSetSecondary (
	IN	PADAPT					pAdapt 
	);

VOID
MPProcessSetPowerOid(
	IN OUT PNDIS_STATUS      pNdisStatus,
	IN  PADAPT					pAdapt,
	IN	PVOID					InformationBuffer,
	IN	ULONG					InformationBufferLength,
	OUT PULONG					BytesRead,
	OUT PULONG					BytesNeeded
    );

#endif //MINIHOOK.H
