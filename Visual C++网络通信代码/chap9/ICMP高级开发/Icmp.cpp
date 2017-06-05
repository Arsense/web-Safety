// Icmp.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//					Implementation of the CIcmp class							//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			1998. EarthWalk Designs Software, by Jay Wheeler.					//
//			All inquiries and/or comments to Jay@EarthWalkDesigns.com			//
//			Latest version can be downloaded from:								//
//																				//
//				http://www.earthwalkdesigns.com									//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Designed using Microsoft VisualC++ 4.2 and MFC						//
//			Tested on WindowsNT 4.0 Workstation and Server						//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			This software is released into the public domain as is and			//
//			without warranty.													//
//																				//
//			Use it in good health, and please let me know if it was useful.		//
//			If you make improvements or fixed problems, please drop me an		//
//			e-mail describing your changes.  I will try to incorporate			//
//			them into this package so that others may benefit from your			//
//			improvements.														//
//																				//
//			Enjoy!																//
//																				// 
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Icmp Class - Derived from CSocket class								//
//																				//
//		Provides the following class methods for WindowsNT 4.0 systems			//
//		utilizing the WindowsNT Winsock 2 interface to the Internet.			//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//		OpenNewSocket															//
//			When called with no arguements, opens a SOCKET connection to a		//
//			RAW socket with ICMP protocol.										//
//																				//
//			Requires a handle to a message processor which will process the		//
//			socket events, the message number to process, and a mask giving		//
//			the events to process.												//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		OpenNewSocket															//
//			Opens a SOCKET connection to a socket of specified family, type		//
//			and protocol														//
//																				//
//			Requires a handle to a message processor which will process the		//
//			socket events, the message number to process, and a mask giving		//
//			the events to process.												//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		Connect																	//
//			When called with 2 arguements, connects an Icmp object to a RAW		//
//			socket with ICMP protocol.											//
//																				//
//			Requires 2 integer values specifying the receive and transmit		//
//			timeouts															//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		Connect																	//
//			When called with 5 arguements, connects an Icmp object to a socket	//
//			of specified family, type and protocol.								//
//																				//
//			Requires 2 integer values specifying the receive and transmit		//
//			timeouts															//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		CloseIcmpSocket															//
//			Closes a socket associated with an Icmp object.						//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		SetAsynchNotification													//
//			Sets the window (process) to notify when a network event on the		//
//			socket associated with the Icmp object occurs.						//
//																				//
//			Requires the message number to send to the window, and the events	//
//			to notify on.														//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		SetTTL																	//
//			Set the TTL option on the socket associated with an Icmp object.	//
//			TTL specifies the number of hops (gateways) to query in the			//
//			Internet tree before a response is returned.  Should be set to		//
//			icmpMaxHops to get a response from the host.						//
//			By incrementing the TTL from 1 to (number of hops to final			//
//			destination), a TraceRoute function can be performed.				//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		Ping																	//
//			Sends an ICMP ping message to the host indicated in the Icmp		//
//			variable icmpSockAddr.  The Icmp variables icmpCurSeq and			//
//			icmpCurId are sent in the buffer referenced by pIcmpBuffer.  The	//
//			referenced buffer must be large	enough to hold DataLen bytes plus	//
//			the size of the ICMP message header.								//
//																				//
//			The Icmp variable icmpPingSentAt is set to the time value at the	//
//			time the ping message is sent.										//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		IcmpChecksum															//
//			Computes the icmp checksum of the Len data in the buffer referenced	//
//			by lpBuf.															//
//																				//
//			Returns an unsigned integer value of the computed checksum.			//
//																				//
//		GetIPAddress															//
//			Returns the value of the IP address of the host pointed by			//
//			iHostName as an unsigned long in network byte order, as described	//
//			in inet_aton														//
//																				//
//			Sets iHostName to the value of the host name returned in			//
//			icmpHostEnt.														//
//																				//
//			Requires the message number to send to the window, and the events	//
//			to notify on.														//
//																				//
//			Sets Icmp variable icmpSockAddr fields appropriately, leaving the	//
//			IP address in icmpSockAddr.sin_addr, and the type of address in		//
//			icmpSockAddr.sin_family.  Also sets icmpHostEnt fields appropriately//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		Receive																	//
//																				//
//			Reads the socket associated with the Icmp object and returns the	//
//			number of bytes read, or SOCKET_ERROR, if an error occurs.			//
//																				//
//			Requires the address of the buffer to store the incoming message	//
//			and an integer buffer size.  The buffer must be large enough to		//
//			contain the incoming message.  In the case of an ICMP ping echo		//
//			response, the size would be the size of the outgoing message plus	//
//			the size of an ICMP header plus the size of an IP header.			//
//																				//
//			Sets icmpPingReceivedAt to the time the receive command is issued,	//
//			icmpRoundTripTime to the difference between icmpPingSentAt and		//
//			icmpPingReceivedAt, icmpCurSeq to the incoming sequence number		//
//			and icmpCurId to the incoming id field.								//
//																				//
//			Sets rcvSockAddr fields appropriately.								//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//		DisplayError															//
//			Displays the provided ErrorType message prior to the decoded error	//
//			message in icmpSocketError and icmpSocketErrorMod.					//
//																				//
//			Sets the message box title to FunctionName.							//
//																				//
//			The message is displayed in a modal box, requiring an OK response	//
//			before the thread containing this method can progress.				//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//		Useage:																	//
//			To use this class, copy the files Icmp.cpp, Icmp.h and Icmp.rc		//
//			to the same directory as your project.	Include Icmp.cpp and		//
//			Icmp.rc in your	project.  This will cause an error notification		//
//			that only one .rc file can be active...ignore it, the next step		//
//			will fix it.  Open the file Icmp.rc, select the string table, and	//
//			highlight all of the messages.  Copy the resource strings to the	//
//			clipboard (copy button).  Open the projects' string table and paste	//
//		    the strings into the next available slot (paste button).  Close		//
//			both string tables and remove (delete) the file Icmp.rc from your	//
//			project.															//
//																				//
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Icmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
/////////////////////////////////////////////////////////////////////////////
// CIcmp

CIcmp::CIcmp()
{
	pIcmpHeader = NULL;
	pIpHeader = NULL;

	icmpSocket = INVALID_SOCKET;
	
	icmpSockAddr.sin_family = 0;
	icmpSockAddr.sin_port = 0;
	icmpSockAddr.sin_addr.s_addr = 0;

	rcvSockAddr.sin_family = 0;
	rcvSockAddr.sin_port = 0;
	rcvSockAddr.sin_addr.s_addr = 0;

	icmpRoundTripTime = 0;

	icmpHops = 0;
	icmpMaxHops = MAX_HOPS;
	icmpCurSeq = 0;
	icmpCurId = 0;

	icmpRcvLen = 0;

}

CIcmp::~CIcmp()
{
	
	if (icmpSocket != INVALID_SOCKET)
		closesocket (icmpSocket);

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CIcmp, CSocket)
	//{{AFX_MSG_MAP(CIcmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
//
//		Copy constructor
//
CIcmp::CIcmp(CIcmp &copy)
{
	pIcmpHeader = copy.pIcmpHeader;
	pIpHeader = copy.pIpHeader;
	icmpSocket = copy.icmpSocket;
	icmpSockAddr = copy.icmpSockAddr;
	icmpRoundTripTime = copy.icmpRoundTripTime;
	icmpHops = copy.icmpHops;
	icmpMaxHops = copy.icmpMaxHops;
	icmpCurSeq = copy.icmpCurSeq;
	icmpCurId = copy.icmpCurId;
	icmpRcvLen = copy.icmpRcvLen;
}

/////////////////////////////////////////////////////////////////////////////
// CIcmp member functions

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		OpenNewSocket															//
//			Opens a SOCKET connection to a RAW socket with ICMP protocol.		//
//																				//
//			Requires a handle to a message processor which will process the		//
//			socket events, the message number to process, and a mask giving		//
//			the events to process.												//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//打开一个SOCKET连接使用ICMP协议，需要一个句柄去处理socket事件，
BOOL CIcmp::OpenNewSocket(HWND hWnd, unsigned int NotificationMessage, long NotifyEvents)
{
	//调用另外一个多态函数
	return OpenNewSocket (hWnd,
						  NotificationMessage,
						  NotifyEvents,
						  AF_INET,
						  SOCK_RAW,
						  IPPROTO_ICMP);
}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		OpenNewSocket															//
//			Opens a SOCKET connection to a socket of specified family, type		//
//			and protocol														//
//																				//
//			Requires a handle to a message processor which will process the		//
//			socket events, the message number to process, and a mask giving		//
//			the events to process.												//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//打开一个连接，确定具体的family，类型和协议
BOOL CIcmp::OpenNewSocket(HWND hWnd, unsigned int NotificationMessage, long NotifyEvents, int AFamily, int AType, int AProtocol)
{

	if (icmpSocket != INVALID_SOCKET)
		CloseIcmpSocket();

	if (!Connect(&icmpPingTimer,
			    &icmpPingTimer,
			    AFamily,
			    AType,
			    AProtocol))
		return FALSE;
	//
	//		Set asynchronous notification and start message interrupt processor
	//

	if (SetAsynchNotification(hWnd,
		  					  NotificationMessage,
							  NotifyEvents) == SOCKET_ERROR)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		Connect																	//
//			Connects an Icmp object to a RAW socket with ICMP protocol.			//
//																				//
//			Requires 2 integer values specifying the receive and transmit		//
//			timeouts															//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//连接一个Icmp对象需要两个整数变量来保存接收和发送超时时间
BOOL CIcmp::Connect(int ReceiveTimeout, int SendTimeout)
{

	return Connect(&ReceiveTimeout, 
				   &SendTimeout, 
				   AF_INET, 
				   SOCK_RAW, 
				   IPPROTO_ICMP);

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		Connect																	//
//			Connects an Icmp object to a socket of specified family, type		//
//			and protocol														//
//																				//
//			Requires 2 integer values specifying the receive and transmit		//
//			timeouts, the ICMP family, ICMP socket type, and ICMP protocol.		//															//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns TRUE if successful, else FALSE.				//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////

BOOL CIcmp::Connect(LPINT ReceiveTimeout, LPINT SendTimeout, int AFamily, int AType, int AProtocol)
{

	int			Result;
	
	icmpSocket = NULL;

	icmpSocket = socket (AFamily,
						 AType,
						 AProtocol);

	if (icmpSocket == INVALID_SOCKET)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 1;
		return FALSE;
	}
	
	//
	//		设定接收超时时间
	//

	Result = setsockopt (icmpSocket, 
					     SOL_SOCKET, 
						 SO_RCVTIMEO, 
						 (char *)ReceiveTimeout, 
						 sizeof(int));
	if (Result == SOCKET_ERROR)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 2;
		closesocket (icmpSocket);
		icmpSocket = INVALID_SOCKET;
		return FALSE;
	}

	//
	//		设定发送超时时间
	//
	Result = setsockopt (icmpSocket, 
					     SOL_SOCKET, 
						 SO_SNDTIMEO, 
						 (char *)SendTimeout, 
						 sizeof(int));
	if (Result == SOCKET_ERROR)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 3;
		closesocket (icmpSocket);
		icmpSocket = INVALID_SOCKET;
		return FALSE;
	}

	icmpCurSeq = 0;
	icmpCurId = (USHORT)GetCurrentProcessId();
	icmpHops = 0;

	return TRUE;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		CloseIcmpSocket																	//
//			Closes a socket associated with an Icmp object.						//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////
int CIcmp::CloseIcmpSocket(void)
{

	if (icmpSocket == INVALID_SOCKET)
		return 0;
	int Result = closesocket(icmpSocket);
	if (Result == SOCKET_ERROR)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 1;

		icmpSocket = INVALID_SOCKET;
	}
	return Result;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		SetAsynchNotification													//
//			Sets the window (process) to notify when a network event on the		//
//			socket associated with the Icmp object occurs.						//
//																				//
//			Requires the message number to send to the window, and the events	//
//			to notify on.														//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////

//消息处理函数，当有网络事件产生的时候
int CIcmp::SetAsynchNotification(HWND hWnd, unsigned int Message, long Events)
{
	//异步选择事件的I/O方法
	int Result = WSAAsyncSelect (icmpSocket,
			 				     hWnd, 
								 Message, 
								 Events);
	if (Result == SOCKET_ERROR)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 1;

		icmpSocket = INVALID_SOCKET;
	}
	return Result;
}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		SetTTL																	//
//			Set the TTL option on the socket associated with an Icmp object.	//
//			A TTL value of non-zero specifies the number of hops to query		//
//			before a response is returned.  By incrementing the TTL from 1 to	//
//			(number of hops to final destination), a TraceRoute function		//
//			can be performed.													//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////

//设定TTL
int CIcmp::SetTTL(int TTL)
{

	int	Result;

	Result = setsockopt (icmpSocket, IPPROTO_IP, IP_TTL, (LPSTR)&TTL, sizeof(int));

	if (Result == SOCKET_ERROR)
	{
		icmpSocketErrorMod = 1;
		icmpSocketError = WSAGetLastError();
	}
	return Result;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		Ping																	//
//			Sends an ICMP ping message to the host indicated in the Icmp		//
//			variable icmpSockAddr.  The Icmp variables icmpCurSeq and			//
//			icmpCurId are sent in the buffer referenced by pIcmpBuffer.  The	//
//			referenced buffer must be large	enough to hold DataLen bytes plus	//
//			the size of the ICMP message header.								//
//																				//
//			The Icmp variable icmpPingSentAt is set to the time value at the	//
//			time the ping message is sent.										//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////

//实现PING功能，通过发送ICMP消息
int CIcmp::Ping (LPSTR pIcmpBuffer, int DataLen)
{

	int	Result;

	int IcmpBufferSize = DataLen + IcmpHeaderLength;

	pIcmpHeader = (LPIcmpHeader)pIcmpBuffer;

	memset (pIcmpBuffer, 'E', IcmpBufferSize);
	memset (pIcmpHeader, 0, IcmpHeaderLength);

	pIcmpHeader->IcmpType = ICMP_ECHO;
	pIcmpHeader->IcmpCode = 0;
	pIcmpHeader->IcmpChecksum = 0;
	pIcmpHeader->IcmpId = icmpCurId;
	pIcmpHeader->IcmpSeq = icmpCurSeq;
	pIcmpHeader->IcmpTimestamp = GetCurrentTime();

	pIcmpHeader->IcmpChecksum = IcmpChecksum ((USHORT FAR *)pIcmpBuffer,
										 IcmpBufferSize);

	icmpPingSentAt = GetCurrentTime();
	//发送PING的信息包
	Result = sendto (icmpSocket, 
					 pIcmpBuffer,
					 IcmpBufferSize,
					 0, 
					 (LPSOCKADDR)&icmpSockAddr, 
					 sizeof icmpSockAddr);
	//获得数据

	if (Result == SOCKET_ERROR)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 1;
	}
	return Result;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		IcmpChecksum															//
//			Computes the icmp checksum of the Len data in the buffer referenced	//
//			by lpBuf.															//
//																				//
//			Returns an unsigned integer value of the computed checksum.			//
//																				//
//////////////////////////////////////////////////////////////////////////////////

//计算出检查和
unsigned short CIcmp::IcmpChecksum(unsigned short FAR *lpBuf, int Len)
{
	register long ChkSum = 0L;
	
	while (Len > 1)
	{
		ChkSum += *(lpBuf++);
		Len -= sizeof (USHORT);
	}
	
	if (Len)
		ChkSum += *(UCHAR *)lpBuf;

	ChkSum = (ChkSum & 0xffff) + (ChkSum>>16);
	ChkSum += (ChkSum >> 16);

	//
	//		A warning C4244 return conversion message is expected,
	//			so shut it off for the next line of code ONLY!
	//
#pragma warning(disable : 4244)
	
	return (~ChkSum);

#pragma warning(default : 4244)

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		GetIPAddress															//
//			Returns the value of the IP address of the host pointed by			//
//			iHostName as an unsigned long in network byte order, as described	//
//			in inet_aton														//
//																				//
//			Sets iHostName to the value of the host name returned in			//
//			icmpHostEnt.														//
//																				//
//			Requires the message number to send to the window, and the events	//
//			to notify on.														//
//																				//
//			Sets Icmp variable icmpSockAddr fields appropriately, leaving the	//
//			IP address in icmpSockAddr.sin_addr, and the type of address in		//
//			icmpSockAddr.sin_family.  Also sets icmpHostEnt fields appropriately//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//获得IP地址，需要先给定主机名
unsigned long CIcmp::GetIPAddress (LPSTR iHostName)
{

	LPHOSTENT		pIcmpHostEnt;
	unsigned long	iHostAddress;
	
	memset (&icmpSockAddr, 0, sizeof(sockaddr_in));

	pIcmpHostEnt = gethostbyname(iHostName);
	if (pIcmpHostEnt == NULL)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 1;
		iHostAddress = inet_addr(iHostName);
	}

	if ((!pIcmpHostEnt) && (iHostAddress == INADDR_NONE))
		return iHostAddress;

	if (pIcmpHostEnt != NULL)
		memcpy (&(icmpSockAddr.sin_addr),
			    pIcmpHostEnt->h_addr,
				pIcmpHostEnt->h_length);
	else
		icmpSockAddr.sin_addr.s_addr = iHostAddress;

	if (pIcmpHostEnt)
		icmpSockAddr.sin_family = pIcmpHostEnt->h_addrtype;
	else
		icmpSockAddr.sin_family = AF_INET;

	iHostName = pIcmpHostEnt->h_name;

	return icmpSockAddr.sin_addr.s_addr;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		Receive																	//
//																				//
//			Reads the socket associated with the Icmp object and returns the	//
//			number of bytes read, or SOCKET_ERROR, if an error occurs.			//
//																				//
//			Requires the address of the buffer to store the incoming message	//
//			and an integer buffer size.  The buffer must be large enough to		//
//			contain the incoming message.  In the case of an ICMP ping echo		//
//			response, the size would be the size of the outgoing message plus	//
//			the size of an ICMP header plus the size of an IP header.			//
//																				//
//			Sets icmpPingReceivedAt to the time the receive command is issued,	//
//			icmpRoundTripTime to the difference between icmpPingSentAt and		//
//			icmpPingReceivedAt, icmpCurSeq to the incoming sequence number		//
//			and icmpCurId to the incoming id field.								//
//																				//
//			Sets rcvSockAddr fields appropriately.								//
//																				//
//			Sets icmpSocketError and icmpSocketErrorMod to indicate the type	//
//			of error, and returns the integer result of the operation.			//
//																				//
//			icmpSocketError and icmpSocketErrorMod are preserved until the next	//
//			operation on this Icmp object is performed.							//
//																				//
//////////////////////////////////////////////////////////////////////////////////

//数据接收，该函数返回接收到得数据得大小或者返回错误信息
int CIcmp::Receive(LPSTR pIcmpBuffer, int IcmpBufferSize)
{
	LPSOCKADDR		pRcvSockAddr = (LPSOCKADDR)&rcvSockAddr;

	int				Result;
	int				RcvIpHdrLen;

	icmpPingReceivedAt = GetTickCount();

	icmpCurId = 0;
	
	//设定地址
	rcvSockAddr.sin_family = AF_INET;
	rcvSockAddr.sin_addr.s_addr = INADDR_ANY;
	rcvSockAddr.sin_port = 0;

	RcvIpHdrLen = sizeof rcvSockAddr;

	Result = recvfrom (icmpSocket, 
					   pIcmpBuffer, 
					   IcmpBufferSize,
					   0,
					   pRcvSockAddr,
					   &RcvIpHdrLen);
	//发生错误
	if (Result == SOCKET_ERROR)
	{
		icmpSocketError = WSAGetLastError();
		icmpSocketErrorMod = 1;
		DisplayError ("Receive","CIcmp::Receive");
		return Result;
	}

	icmpRcvLen = Result;

	pIpHeader = (LPIpHeader)pIcmpBuffer;

	RcvIpHdrLen = pIpHeader->HeaderLength * 4;
	if (Result < RcvIpHdrLen + ICMP_MIN)
	{
		//
		//	收到的字节太少
		//
		MessageBox(NULL, 
				   "Short message!", 
				   "CIcmp::Receive", 
				   MB_OK|MB_SYSTEMMODAL);
		icmpSocketErrorMod = 2;
		return Result;
	}

	pIcmpHeader = (LPIcmpHeader)(pIcmpBuffer + RcvIpHdrLen);

	icmpCurId = pIcmpHeader->IcmpId;
	icmpRoundTripTime = icmpPingReceivedAt - pIcmpHeader->IcmpTimestamp;

	if (pIcmpHeader->IcmpType != ICMP_ECHOREPLY)
	{
		//
		//	不是回显响应
		//
		return Result;
	}

	icmpCurSeq = pIcmpHeader->IcmpSeq;

	return Result;
}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//		DisplayError															//
//			Displays the provided ErrorType message prior to the decoded error	//
//			message in icmpSocketError and icmpSocketErrorMod.					//
//																				//
//			Sets the message box title to FunctionName.							//
//																				//
//			The message is displayed in a modal box, requiring an OK response	//
//			before the thread containing this method can progress.				//
//																				//
//////////////////////////////////////////////////////////////////////////////////
void CIcmp::DisplayError(CString ErrorType, CString FunctionName)
{

	CString		ErrorMessage;
	CString		OutputMessage;

	if (ErrorMessage.LoadString(icmpSocketError) == 0)
	{
		OutputMessage.Format("%s Error: %u-%u", 
						     ErrorType,
							 icmpSocketError,
							 icmpSocketErrorMod);
	}
	else
	{
		OutputMessage.Format("%s Error (%u-%u) : %s",
							 ErrorType, 
							 icmpSocketError,
							 icmpSocketErrorMod,
							 ErrorMessage);
	}

	MessageBox (NULL,OutputMessage, FunctionName, MB_OK|MB_SYSTEMMODAL);

}
