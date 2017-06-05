// Ping.h: interface for the CPing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PING_H__DD196130_22F6_48DA_BAFC_B799A0034437__INCLUDED_)
#define AFX_PING_H__DD196130_22F6_48DA_BAFC_B799A0034437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#define IP_RECORD_ROUTE  0x7
#define DEF_PACKET_SIZE  32 
#define MAX_PACKET       1024      // Max ICMP packet size
#define MAX_IP_HDR_SIZE  60        // Max IP header size w/options
#define ICMP_ECHO        8
#define ICMP_ECHOREPLY   0
#define ICMP_MIN         8 // Minimum 8-byte ICMP packet (header)

class CMyPingDlg;


typedef struct _iphdr 
{
    unsigned int   h_len:4;        // Length of the header
    unsigned int   version:4;      // Version of IP
    unsigned char  tos;            // Type of service
    unsigned short total_len;      // Total length of the packet
    unsigned short ident;          // Unique identifier
    unsigned short frag_and_flags; // Flags
    unsigned char  ttl;            // Time to live
    unsigned char  proto;          // Protocol (TCP, UDP etc)
    unsigned short checksum;       // IP checksum

    unsigned int   sourceIP;
    unsigned int   destIP;
} IpHeader;


typedef struct _icmphdr 
{
    BYTE   i_type;
    BYTE   i_code;                 // Type sub code
    USHORT i_cksum;
    USHORT i_id;
    USHORT i_seq;
    // This is not the standard header, but we reserve space for time
    ULONG  timestamp;
} IcmpHeader;

//
// IP option header - use with socket option IP_OPTIONS
//
typedef struct _ipoptionhdr
{
    unsigned char        code;        // Option type
    unsigned char        len;         // Length of option hdr
    unsigned char        ptr;         // Offset into options
    unsigned long        addr[9];     // List of IP addrs
} IpOptionHeader;


class CPing  
{
public:
	void SetConfigure(char * host,BOOL recordrout=FALSE,int size=DEF_PACKET_SIZE);
	void DecodeIPOptions(char *buf, int bytes);
	void Cleanup();
	void Ping(int timeout =1000);
	SOCKET m_hSocket;
	IpOptionHeader   m_ipopt;
	SOCKADDR_IN m_addrDest;
	SOCKADDR_IN m_addrFrom;
	char              *icmp_data;
    char              *recvbuf;
	 USHORT             seq_no ;
	 char *lpdest;
	int   datasize;
	BOOL m_bRecordRout;
	CMyPingDlg  * m_dlg;
	CPing(CMyPingDlg *dlg);
	virtual ~CPing();

private:
	void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN* from);
	USHORT checksum(USHORT *buffer, int size);
	void FillICMPData(char *icmp_data, int datasize);
};
#endif // !defined(AFX_PING_H__DD196130_22F6_48DA_BAFC_B799A0034437__INCLUDED_)
