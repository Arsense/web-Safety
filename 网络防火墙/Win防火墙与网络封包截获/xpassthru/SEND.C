/*——————————————————————————————————————
	文件：send.c
	工程：xpassthru
	概述：演示NDIS中间驱动程序输出封包结构信息
	版权所有(c) 2001-2002 X 工作室
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

NDIS_STATUS
MPSendOnePacket(
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	PNDIS_PACKET	Packet,
	IN	UINT			Flags
)
{
	PADAPT			pAdapt = (PADAPT)MiniportAdapterContext;
	NDIS_STATUS		Status;
	PNDIS_PACKET	MyPacket;
	PRSVD			Rsvd;
	PVOID			MediaSpecificInfo		= NULL;
	ULONG			MediaSpecificInfoSize	= 0;

	if (IsIMDeviceStateOn (pAdapt) == FALSE)
		return NDIS_STATUS_FAILURE;

	NdisAllocatePacket(&Status, &MyPacket, pAdapt->SendPacketPoolHandle);

	if (Status == NDIS_STATUS_SUCCESS)
	{
		PNDIS_PACKET_EXTENSION	Old, New;

		Rsvd = (PRSVD)(MyPacket->ProtocolReserved);
		Rsvd->OriginalPkt = Packet;
		MyPacket->Private.Flags = Flags;

		MyPacket->Private.Head = Packet->Private.Head;
		MyPacket->Private.Tail = Packet->Private.Tail;
		NdisSetPacketFlags(MyPacket, NDIS_FLAGS_DONT_LOOPBACK);

		NdisMoveMemory(NDIS_OOB_DATA_FROM_PACKET(MyPacket),
					   NDIS_OOB_DATA_FROM_PACKET(Packet),
					   sizeof(NDIS_PACKET_OOB_DATA));

		NdisIMCopySendPerPacketInfo(MyPacket, Packet);
		
		NDIS_GET_PACKET_MEDIA_SPECIFIC_INFO(
			Packet,	&MediaSpecificInfo,	&MediaSpecificInfoSize);

		if (MediaSpecificInfo || MediaSpecificInfoSize)
		{
			NDIS_SET_PACKET_MEDIA_SPECIFIC_INFO(
				MyPacket, MediaSpecificInfo, MediaSpecificInfoSize);
		}

		NdisSend(&Status, pAdapt->BindingHandle, MyPacket);
		if (Status != NDIS_STATUS_PENDING)
		{
			NdisIMCopySendCompletePerPacketInfo (Packet, MyPacket);
			NdisFreePacket(MyPacket);

			PrintPacket(Packet);
		}
	}

	return(Status);
}

NDIS_STATUS
MPSend(
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	PNDIS_PACKET	Packet,
	IN	UINT			Flags
)
{
	PADAPT			pAdapt = (PADAPT)MiniportAdapterContext;
	NDIS_STATUS		Status;

	DBGPRINT("==>Passthru Miniport MPSend\n");
	ASSERT (pAdapt->pSecondaryAdapt);

	pAdapt = pAdapt->pSecondaryAdapt;

	Status = MPSendOnePacket(pAdapt, Packet, Flags);

	return(Status);
}

VOID
MPSendPackets(
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	PPNDIS_PACKET			PacketArray,
	IN	UINT					NumberOfPackets
)
{
	PADAPT			pAdapt = (PADAPT)MiniportAdapterContext;
	NDIS_STATUS		Status;
	UINT			i;

	pAdapt = pAdapt->pSecondaryAdapt;

	for (i = 0; i < NumberOfPackets; i++)
	{
		PNDIS_PACKET	Packet;
		Packet = PacketArray[i];
		Status = MPSendOnePacket(pAdapt, Packet, NdisGetPacketFlags(Packet));
		if (Status != NDIS_STATUS_PENDING)
		{
			NdisMSendComplete(
				pAdapt->pPrimaryAdapt->MiniportHandle, Packet, Status);
		}
	}
}

NDIS_STATUS
MPTransferData(
	OUT PNDIS_PACKET			Packet,
	OUT PUINT					BytesTransferred,
	IN	NDIS_HANDLE				MiniportAdapterContext,
	IN	NDIS_HANDLE				MiniportReceiveContext,
	IN	UINT					ByteOffset,
	IN	UINT					BytesToTransfer
)
{
	PADAPT		pAdapt = (PADAPT)MiniportAdapterContext;
	NDIS_STATUS	Status;

	DBGPRINT("==>Passthru Miniport MPTransferData\n");
	
	if (IsIMDeviceStateOn(pAdapt) == FALSE)
	{
		return NDIS_STATUS_FAILURE;
	}

	pAdapt = pAdapt->pPrimaryAdapt;

	NdisTransferData(&Status,
					 pAdapt->BindingHandle,
					 MiniportReceiveContext,
					 ByteOffset,
					 BytesToTransfer,
					 Packet,
					 BytesTransferred);

	if(Status == NDIS_STATUS_SUCCESS)
	{
		PrintPacket(Packet);
		DbgPrint("    BytesTransferred: %u\n", *BytesTransferred);
	}

	return(Status);
}

VOID
PtSendComplete(
	IN	NDIS_HANDLE			ProtocolBindingContext,
	IN  PNDIS_PACKET		Packet,
	IN  NDIS_STATUS			Status
)
{
	PADAPT			pAdapt =(PADAPT)ProtocolBindingContext;
	PNDIS_PACKET	Pkt;
	PRSVD			Rsvd;

	DBGPRINT("==> Passthru Protocol PtSendComplete\n");
	
	PrintPacket(Packet);

	pAdapt	= pAdapt->pPrimaryAdapt;
	Rsvd	= (PRSVD)(Packet->ProtocolReserved);
	Pkt		= Rsvd->OriginalPkt;

	NdisIMCopySendCompletePerPacketInfo (Pkt, Packet);
	NdisDprFreePacket(Packet);
	NdisMSendComplete(pAdapt->MiniportHandle, Pkt, Status);
	DBGPRINT("<== Passthru Protocol PtSendComplete\n");
}

VOID
PtTransferDataComplete(
	IN  NDIS_HANDLE			ProtocolBindingContext,
	IN  PNDIS_PACKET		Packet,
	IN  NDIS_STATUS			Status,
	IN  UINT				BytesTransferred
)
{
	PADAPT pAdapt = (PADAPT)ProtocolBindingContext;
	pAdapt = pAdapt->pPrimaryAdapt;

	DBGPRINT("<== Passthru Protocol PtTransferDataComplete\n");

	PrintPacket(Packet);

	if(pAdapt->MiniportHandle)
	{
		  NdisMTransferDataComplete(
			  pAdapt->MiniportHandle, Packet, Status, BytesTransferred);
	}
	DBGPRINT("<== Passthru Protocol PtTransferDataComplete\n");
}
