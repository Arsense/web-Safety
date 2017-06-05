/*
	packet.h 
	Create date: 2001-12-25
*/
#ifndef PACKET_H
#define PACKET_H

VOID
PrintPacket(
	IN PNDIS_PACKET packet
);

UINT
PrintHeaderSize(
	IN PNDIS_PACKET packet
);

NDIS_STATUS
PrintPacketStatus(
	IN PNDIS_PACKET packet
);

VOID
PrintProtocol(
	IN PNDIS_PACKET packet
);


#endif //PACKET_H