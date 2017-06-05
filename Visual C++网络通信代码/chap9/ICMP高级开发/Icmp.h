// Icmp.h : header file
//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//								The CIcmp class									//
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
//					Icmp Class - Derived from CSocket class						//
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

#pragma pack (4)

//////////////////////////////////////////////////////////////////
//																//
//																//
//////////////////////////////////////////////////////////////////

#define MAXBFRSIZE		2048

#define MAX_HOPS		30

typedef SOCKET * LPSOCKET;

typedef in_addr * LPINADDR;

//////////////////////////////////////////////////////////////////
//																//
//							IP Header							//
//				Implementation of RFC791 IP Header				//
//																//
//////////////////////////////////////////////////////////////////

typedef struct _IpHeader 
{
	unsigned int		HeaderLength:4;		// IP首部长度
	unsigned int		Version:4;			// IP的版本
	unsigned char		TypeOfService;		// 服务类型
	unsigned short		TotalLength;		// 总的数据包大小
	unsigned short		Identification;		// 特殊标识符
	unsigned short		FragmentationFlags; // 标志
	unsigned char		TTL;				// Time To Live
	unsigned char		Protocol;           // 协议 (TCP, UDP等)
	unsigned short		CheckSum;			// 检验和
	unsigned int		sourceIPAddress;	// 源地址
	unsigned int		destIPAddress;		// 目标地址

} IpHeader;

typedef IpHeader FAR * LPIpHeader;

#define IpHeaderLength sizeof(IpHeader)

//////////////////////////////////////////////////////////////////
//																//
//						ICMP header								//
//			Implementation of RFC792 ICMP Header				//
//																//
//////////////////////////////////////////////////////////////////
typedef struct _IcmpHeader
{
  BYTE			IcmpType;			//ICMP报文类型
  BYTE			IcmpCode;			//ICMP代码 
  USHORT		IcmpChecksum;		//检验和
  USHORT		IcmpId;				//标识符
  USHORT		IcmpSeq;			//序列号
  ULONG			IcmpTimestamp;		// 时间戳，非标准字段
} IcmpHeader;

typedef IcmpHeader FAR * LPIcmpHeader;

#define IcmpHeaderLength sizeof(IcmpHeader)

//////////////////////////////////////////////////////////////////

#define MAX_PACKET			2000 + IcmpHeaderLength

#define ICMP_ECHO			8
#define ICMP_ECHOREPLY		0

#define ICMP_MIN			8 // minimum 8 byte icmp packet (just header)

/////////////////////////////////////////////////////////////////////////////
// CIcmp command target

class CIcmp : public CSocket
{
// Attributes
public:

	BOOL OpenNewSocket(HWND hWnd, unsigned int NotificationMessage, long NotifyEvents);
	BOOL OpenNewSocket(HWND hWnd, unsigned int NotificationMessage, long NotifyEvents, int AFamily, int AType, int AProtocol);

	int CloseIcmpSocket(void);

	BOOL Connect(int ReceiveTimeout, int SendTimeout);
	BOOL Connect(LPINT ReceiveTimeout, LPINT SendTimeout, int AFamily, int AType, int AProtocol);

	int SetTTL(int TTL);
	
	int SetAsynchNotification(HWND hWnd, unsigned int Message, long Events);
	
	int Receive(LPSTR pIcmpBuffer, int IcmpBufferSize);

	unsigned long GetIPAddress (LPSTR iHostName);
	
	int Ping (LPSTR pIcmpBuffer, int IcmpBufferSize);

	unsigned short IcmpChecksum(unsigned short FAR *lpBuf, int Len);

	void DisplayError(CString ErrorType, CString FunctionName);
	
// Operations
public:
	CIcmp(void);
	CIcmp(CIcmp &copy);
	~CIcmp(void);

//	CIcmp(CIcmp NewIcmp);

// Overrides
public:
private:

public:
	//
	//		I/O Buffer Pointers
	//

	LPIcmpHeader		pIcmpHeader;
	LPIpHeader			pIpHeader;

	SOCKET				icmpSocket;

	SOCKADDR_IN			icmpSockAddr;
	SOCKADDR_IN			rcvSockAddr;

	DWORD				icmpRoundTripTime;

	DWORD				icmpPingSentAt;
	DWORD				icmpPingReceivedAt;

	int					icmpRcvLen;

	int					icmpHops;
	int					icmpMaxHops;

	int					icmpCurSeq;
	int					icmpCurId;
	
	int					icmpPingTimer;
	
	int					icmpSocketError;
	int					icmpSocketErrorMod;
	
	unsigned long		icmpHostAddress;

protected:
};

typedef CIcmp FAR * LPIcmp;

/////////////////////////////////////////////////////////////////////////////
