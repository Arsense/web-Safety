/*
	文件：Packet.c
	工程：xpassthru
	概述：演示NDIS中间驱动程序输出封包结构信息
	版权所有(c) 2001-2002 X 工作室
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

VOID PrintPacket(
	IN PNDIS_PACKET packet
)
{
	UINT PhysicalBufferCount;
	UINT BufferCount;
	PNDIS_BUFFER  FirstBuffer;
	UINT TotalPacketLength;

	NdisQueryPacket(packet
		, &PhysicalBufferCount
		, &BufferCount
		, &FirstBuffer
		, &TotalPacketLength
		);

	DbgPrint("    PacketPointer: %u.\n", (VOID*)packet);
	DbgPrint("    PhysicalBufferCount: %u.\n", PhysicalBufferCount);
	DbgPrint("    BufferCount: %u.\n", BufferCount);
	DbgPrint("    FirstBufferPointer: %u.\n", FirstBuffer);
	DbgPrint("    TotalPacketLenth: %u.\n", TotalPacketLength);
	PrintHeaderSize(packet);
	PrintProtocol(packet);
	PrintPacketStatus(packet);
}

UINT
PrintHeaderSize(
	IN PNDIS_PACKET packet
)
{
	UINT HeaderSize = NDIS_GET_PACKET_HEADER_SIZE(packet);
	DbgPrint("    HeaderSize: %u.\n", HeaderSize);
	return HeaderSize;
}

NDIS_STATUS
PrintPacketStatus(
	IN PNDIS_PACKET packet
)
{
	NDIS_STATUS status = NDIS_GET_PACKET_STATUS(packet);
	ANSI_STRING sStatus;
	switch(status)
	{
	case NDIS_STATUS_SUCCESS:
		NdisInitAnsiString(&sStatus, "NDIS_STATUS_SUCCESS");
		break;
	case NDIS_STATUS_PENDING:
		NdisInitAnsiString(&sStatus, "NDIS_STATUS_PENDING");
		break;
	case NDIS_STATUS_RESOURCES:
		NdisInitAnsiString(&sStatus, "NDIS_STATUS_RESOURCES");
		break;
	default:
		NdisInitAnsiString(&sStatus, "Unknown status");
		break;
	}
	DbgPrint("    Status: %s.\n", sStatus.Buffer);
	return status;
}

VOID
PrintProtocol(
	IN PNDIS_PACKET packet
)
{
	UINT dProtocolType = NDIS_GET_PACKET_PROTOCOL_TYPE(packet);
	NDIS_STRING sProtocol;
	switch(dProtocolType)
	{
	case NDIS_PROTOCOL_ID_DEFAULT:
		NdisInitUnicodeString(&sProtocol, L"DEFAULT");
		break;
	case NDIS_PROTOCOL_ID_TCP_IP :
		NdisInitUnicodeString(&sProtocol, L"TCP/IP");
		break;
	case NDIS_PROTOCOL_ID_IPX :
		NdisInitUnicodeString(&sProtocol, L"IPX");
		break;
	case NDIS_PROTOCOL_ID_NBF :
		NdisInitUnicodeString(&sProtocol, L"NBF");
		break;
	default:
		NdisInitUnicodeString(&sProtocol, L"UNKNOWN");
		break;
	}

	DbgPrint("    Protocol: %ws.\n", sProtocol.Buffer);
}
