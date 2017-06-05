/*——————————————————————————————————————
	文件：recv.c
	工程：xpassthru
	概述：演示NDIS中间驱动程序输出封包结构信息
	版权所有(c) 2001-2002 X 工作室
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

NDIS_STATUS
PtReceive(
	IN  NDIS_HANDLE			ProtocolBindingContext,
	IN  NDIS_HANDLE			MacReceiveContext,
	IN  PVOID				HeaderBuffer,
	IN  UINT				HeaderBufferSize,
	IN  PVOID				LookAheadBuffer,
	IN  UINT				LookAheadBufferSize,
	IN  UINT				PacketSize
)
{
	PADAPT			pAdapt =(PADAPT)ProtocolBindingContext;
	PNDIS_PACKET	MyPacket, Packet;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;

	DBGPRINT("==> Passthru Protocol PtReceive\n");
	DbgPrint("    CurrentAdapterHandle: %u\n", ProtocolBindingContext);
	DbgPrint("    MacReceiveHandle: %u\n", MacReceiveContext);
	DbgPrint("    HeaderBufferPointer: %u\n", HeaderBuffer);
	DbgPrint("    HeaderBufferSize: %u\n", HeaderBufferSize);
	DbgPrint("    LookAheadBufferPointer: %u\n", LookAheadBuffer);
	DbgPrint("    LookAheadBufferSize: %u\n", LookAheadBufferSize);
	DbgPrint("    PacketSize: %u\n", PacketSize);

	if(!pAdapt->MiniportHandle)
	{
		Status = NDIS_STATUS_FAILURE;
	}
	else do
	{
		if(pAdapt->isSecondary)
		{
			 DBGPRINT("PASSTHRU GETTING RECIEVES ON SECONDARY\n");
			 ASSERT(0);
		}

		Packet = NdisGetReceivedPacket(pAdapt->BindingHandle, MacReceiveContext);

		if(Packet != NULL)
		{
			 DBGPRINT("==> Passthru Protocol PtReceive RePacket...\n");

			 NdisDprAllocatePacket(&Status
				 , &MyPacket, pAdapt->RecvPacketPoolHandle);

			 if(Status == NDIS_STATUS_SUCCESS)
			 {
				MyPacket->Private.Head = Packet->Private.Head;
				MyPacket->Private.Tail = Packet->Private.Tail;

				NDIS_SET_ORIGINAL_PACKET(
					  MyPacket, NDIS_GET_ORIGINAL_PACKET(Packet));
				NDIS_SET_PACKET_HEADER_SIZE(MyPacket, HeaderBufferSize);

				NdisGetPacketFlags(MyPacket) = NdisGetPacketFlags(Packet);

				NDIS_SET_PACKET_STATUS(MyPacket, NDIS_STATUS_RESOURCES);

				NdisMIndicateReceivePacket(pAdapt->MiniportHandle, &MyPacket, 1);

				ASSERT(NDIS_GET_PACKET_STATUS(MyPacket) == NDIS_STATUS_RESOURCES);

				PrintPacket(MyPacket);

				NdisDprFreePacket(MyPacket);
	    		DBGPRINT("<== Passthru Protocol PtReceive RePacket Success...\n");
				break;
			 }
		}

		pAdapt->IndicateRcvComplete = TRUE;
		switch(pAdapt->Medium)
		{
		  case NdisMedium802_3:
			 NdisMEthIndicateReceive(pAdapt->MiniportHandle,
											 MacReceiveContext,
											 HeaderBuffer,
											 HeaderBufferSize,
											 LookAheadBuffer,
											 LookAheadBufferSize,
											 PacketSize);
			 break;

		  case NdisMedium802_5:
			 NdisMTrIndicateReceive(pAdapt->MiniportHandle,
											MacReceiveContext,
											HeaderBuffer,
											HeaderBufferSize,
											LookAheadBuffer,
											LookAheadBufferSize,
											PacketSize);
			 break;

		  case NdisMediumFddi:
			 NdisMFddiIndicateReceive(pAdapt->MiniportHandle,
											  MacReceiveContext,
											  HeaderBuffer,
											  HeaderBufferSize,
											  LookAheadBuffer,
											  LookAheadBufferSize,
											  PacketSize);
			 break;

		  default:
			 ASSERT(0);
			 break;
		}

	} while(FALSE);

	DBGPRINT("<== Passthru Protocol PtReceive\n");

	return Status;
}

VOID
PtReceiveComplete(
	IN	NDIS_HANDLE		ProtocolBindingContext
)
{
	PADAPT		pAdapt =(PADAPT)ProtocolBindingContext;

	if(pAdapt->isSecondary)
	{
		  DBGPRINT("PASSTHRU GETTING RECEIVES ON SECONDARY\n");
		  ASSERT(0);
	}

	if((pAdapt->MiniportHandle != NULL) && pAdapt->IndicateRcvComplete)
	{
		switch(pAdapt->Medium)
		{
		  case NdisMedium802_3:
			NdisMEthIndicateReceiveComplete(pAdapt->MiniportHandle);
			break;

		  case NdisMedium802_5:
			NdisMTrIndicateReceiveComplete(pAdapt->MiniportHandle);
			break;

		  case NdisMediumFddi:
			NdisMFddiIndicateReceiveComplete(pAdapt->MiniportHandle);
			break;

		  default:
			ASSERT(0);
			break;
		}
	}
	pAdapt->IndicateRcvComplete = FALSE;

	DBGPRINT("<== Passthru Protocol PtReceiveComplete\n");
}

INT
PtReceivePacket(
	IN	NDIS_HANDLE			ProtocolBindingContext,
	IN	PNDIS_PACKET		Packet
	)
{
	PADAPT			pAdapt =(PADAPT)ProtocolBindingContext;
	NDIS_STATUS		Status;
	PNDIS_PACKET	MyPacket;
	PRSVD			Resvd;

	DBGPRINT("==> Passthru Protocol PtReceivePacket\n");

	if(!pAdapt->MiniportHandle)	return 0;

	if(pAdapt->isSecondary)
	{
		  DBGPRINT("PASSTHRU GETTING RECEIVES ON SECONDARY\n");
		  ASSERT(0);
	}

	NdisDprAllocatePacket(&Status, &MyPacket, pAdapt->RecvPacketPoolHandle);

	if(Status == NDIS_STATUS_SUCCESS)
	{
		Resvd =(PRSVD)(MyPacket->MiniportReserved);
		Resvd->OriginalPkt = Packet;

		MyPacket->Private.Head = Packet->Private.Head;
		MyPacket->Private.Tail = Packet->Private.Tail;

		NDIS_SET_ORIGINAL_PACKET(
			MyPacket, NDIS_GET_ORIGINAL_PACKET(Packet));

		NdisGetPacketFlags(MyPacket) = NdisGetPacketFlags(Packet);

		Status = NDIS_GET_PACKET_STATUS(Packet);

		NDIS_SET_PACKET_STATUS(MyPacket, Status);
		NDIS_SET_PACKET_HEADER_SIZE(
			MyPacket, NDIS_GET_PACKET_HEADER_SIZE(Packet));

		NdisMIndicateReceivePacket(pAdapt->MiniportHandle, &MyPacket, 1);

		PrintPacket(MyPacket);

		if(Status == NDIS_STATUS_RESOURCES)
		{
			NdisDprFreePacket(MyPacket);
		}

		return((Status != NDIS_STATUS_RESOURCES) ? 1 : 0);
	}
	else
	{
		return(0);
	}
}

VOID
MPReturnPacket(
	IN	NDIS_HANDLE		MiniportAdapterContext,
	IN	PNDIS_PACKET	Packet
)
{
	PADAPT			pAdapt = (PADAPT)MiniportAdapterContext;
	PNDIS_PACKET	MyPacket;
	PRSVD			Resvd;

	DBGPRINT("==> Passthru Miniport MPReturnPacket\n");

	PrintPacket(Packet);

	Resvd = (PRSVD)(Packet->MiniportReserved);
	MyPacket = Resvd->OriginalPkt;

	NdisFreePacket(Packet);
	NdisReturnPackets(&MyPacket, 1);
}


