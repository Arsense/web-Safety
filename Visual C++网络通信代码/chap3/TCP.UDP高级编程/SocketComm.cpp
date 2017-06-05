///////////////////////////////////////////////////////////////////////////////
//	File:		SocketComm.cpp
//	Version:	1.1
//
//	Author:		Ernest Laurentin
//	E-mail:		elaurentin@sympatico.ca
//
//	Implementation of the CSocketComm and associated classes.
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is
//	not sold for profit without the authors written consent, and
//	providing that this notice and the authors name and all copyright
//	notices remains intact.
//
//	An email letting me know how you are using it would be nice as well.
//
//	This file is provided "as is" with no expressed or implied warranty.
//	The author accepts no liability for any damage/loss of business that
//	this c++ class may cause.
//
//	Version history
//
//	1.0	- Initial release.
//	1.1 - Add support for Smart Addressing mode
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include <crtdbg.h>
#include "SocketComm.h"

const DWORD DEFAULT_TIMEOUT = 100L;

///////////////////////////////////////////////////////////////////////////////
// SockAddrIn Struct

///////////////////////////////////////////////////////////////////////////////
// Copy
SockAddrIn& SockAddrIn::Copy(const SockAddrIn& sin)
{
	memcpy(&this->sockAddrIn, &sin.sockAddrIn, Size());
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// IsEqual
bool SockAddrIn::IsEqual(const SockAddrIn& sin)
{
	// Is it Equal? - ignore 'sin_zero'
	return (memcmp(&this->sockAddrIn, &sin.sockAddrIn, Size()-sizeof(sockAddrIn.sin_zero)) == 0);
}

///////////////////////////////////////////////////////////////////////////////
// IsGreater
bool SockAddrIn::IsGreater(const SockAddrIn& sin)
{
	// Is it Greater? - ignore 'sin_zero'
	return (memcmp(&this->sockAddrIn, &sin.sockAddrIn, Size()-sizeof(sockAddrIn.sin_zero)) > 0);
}

///////////////////////////////////////////////////////////////////////////////
// IsLower
bool SockAddrIn::IsLower(const SockAddrIn& sin)
{
	// Is it Lower? - ignore 'sin_zero'
	return (memcmp(&this->sockAddrIn, &sin.sockAddrIn, Size()-sizeof(sockAddrIn.sin_zero)) < 0);
}

///////////////////////////////////////////////////////////////////////////////
// CreateFrom
bool SockAddrIn::CreateFrom(LPCTSTR sAddr, LPCTSTR sService)
{
	sockAddrIn.sin_addr.s_addr = htonl( CSocketComm::GetIPAddress(sAddr) );
	sockAddrIn.sin_port = htons( CSocketComm::GetPortNumber( sService ) );
	sockAddrIn.sin_family = AF_INET;
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// Construct & Destruct

//构造函数
CSocketComm::CSocketComm() :
	m_hComm(INVALID_HANDLE_VALUE), m_hMutex(NULL), m_bServer(false),
	m_bBroadcast(false), m_hThread(NULL)
{

}

CSocketComm::~CSocketComm()
{
	//停止通信
	StopComm();
}

///////////////////////////////////////////////////////////////////////////////
// Members
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// IsOpen
bool CSocketComm::IsOpen() const
{
	return ( INVALID_HANDLE_VALUE != m_hComm );
}


///////////////////////////////////////////////////////////////////////////////
//判断是否启动
bool CSocketComm::IsStart() const
{
	return ( NULL != m_hThread );
}


///////////////////////////////////////////////////////////////////////////////
// IsServer
bool CSocketComm::IsServer() const
{
	return m_bServer;
}


///////////////////////////////////////////////////////////////////////////////
// IsBroadcast
bool CSocketComm::IsBroadcast() const
{
	return m_bBroadcast;
}


///////////////////////////////////////////////////////////////////////////////
// IsSmartAddressing
bool CSocketComm::IsSmartAddressing() const
{
	return m_bSmartAddressing;
}


///////////////////////////////////////////////////////////////////////////////
// GetSocket

//获得socket句柄
SOCKET CSocketComm::GetSocket() const
{
	return (SOCKET) m_hComm;
}


///////////////////////////////////////////////////////////////////////////////
// LockList

//进程互斥
void CSocketComm::LockList()
{
	if (NULL != m_hMutex)
		WaitForSingleObject(m_hMutex, INFINITE);
}


///////////////////////////////////////////////////////////////////////////////
// UnlockList

//解开进程互斥
void CSocketComm::UnlockList()
{
	if (NULL != m_hMutex)
		ReleaseMutex(m_hMutex);
}


///////////////////////////////////////////////////////////////////////////////
// AddToList
void CSocketComm::AddToList(const SockAddrIn& saddr_in)
{
	LockList();
	m_AddrList.insert( m_AddrList.end(), saddr_in );
	UnlockList();
}

///////////////////////////////////////////////////////////////////////////////
// RemoveFromList
void CSocketComm::RemoveFromList(const SockAddrIn& saddr_in)
{
	LockList();
	m_AddrList.remove( saddr_in );
	UnlockList();
}

///////////////////////////////////////////////////////////////////////////////
// SetServerState
void CSocketComm::SetServerState(bool bServer)
{
	if (!IsStart())
		m_bServer = bServer;
}


///////////////////////////////////////////////////////////////////////////////
// SetSmartAddressing : Address is included with message

//设定
void CSocketComm::SetSmartAddressing(bool bSmartAddressing)
{
	if (!IsStart())
		m_bSmartAddressing = bSmartAddressing;
}

///////////////////////////////////////////////////////////////////////////////
// OnDataReceived
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				This function is PURE Virtual, you MUST overwrite it.  This is
//				called every time new data is available.
// PARAMETERS:
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount)
{
}


///////////////////////////////////////////////////////////////////////////////
// OnEvent
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				This function reports events & errors
// PARAMETERS:
//		UINT uEvent: can be one of the event value EVT_(events)
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::OnEvent(UINT uEvent)
{
}

///////////////////////////////////////////////////////////////////////////////
// GetPortNumber
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Returns a port number based on service name or port number string
// PARAMETERS:
//	LPCTSTR strServiceName: Service name or port string
///////////////////////////////////////////////////////////////////////////////

//获得端口号
USHORT CSocketComm::GetPortNumber( LPCTSTR strServiceName )
{
	LPSERVENT	lpservent;
	USHORT		nPortNumber = 0;

	if ( _istdigit( strServiceName[0] ) ) {
		nPortNumber = (USHORT) _ttoi( strServiceName );
	}
	else {
#ifdef _UNICODE
		char pstrService[HOSTNAME_SIZE];
		WideCharToMultiByte(CP_ACP, 0, pstrService, -1, strServiceName, sizeof(pstrService), NULL, NULL );
#else
		LPCTSTR pstrDevice = strServiceName;
#endif
		// 转化网络字节到主机字节
		if ( (lpservent = getservbyname( pstrDevice, NULL )) != NULL )
			nPortNumber = ntohs( lpservent->s_port );
	}

	return nPortNumber;
}


///////////////////////////////////////////////////////////////////////////////
// GetIPAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Returns an IP address.
//			- It tries to convert the string directly
//			- If that fails, it tries to resolve it as a hostname
// PARAMETERS:
//	LPCTSTR strHostName: host name to get IP address
///////////////////////////////////////////////////////////////////////////////

//获取IP地址，如果失败，则返回一个主机名
ULONG CSocketComm::GetIPAddress( LPCTSTR strHostName )
{
	LPHOSTENT	lphostent;
	ULONG		uAddr = INADDR_NONE;
	//AfxMessageBox(strHostName);
	if ( NULL != strHostName )
	{
#ifdef _UNICODE
		char strHost[HOSTNAME_SIZE] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, strHostName, -1, strHost, sizeof(strHost), NULL, NULL );
#else
		LPCTSTR strHost = strHostName;
#endif
		// 转化成标准ip地址
		//AfxMessageBox(strHost);
		uAddr = inet_addr( strHostName );

		if ( (INADDR_NONE == uAddr) && (strcmp( strHost, "255.255.255.255" )) )
		{
			// 获得机器名
			if ( lphostent = gethostbyname( strHost ) )
				uAddr = *((ULONG *) lphostent->h_addr_list[0]);
		}
	}
	
	return ntohl( uAddr );
}


///////////////////////////////////////////////////////////////////////////////
// GetLocalName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Get local computer name.  Something like: "mycomputer.myserver.net"
// PARAMETERS:
//	LPTSTR strName: name of the computer is returned here
//	UINT nSize: size max of buffer "strName"
///////////////////////////////////////////////////////////////////////////////

//获得本地机器名
bool CSocketComm::GetLocalName(LPTSTR strName, UINT nSize)
{
	if (strName != NULL && nSize > 0)
	{
		char strHost[HOSTNAME_SIZE] = { 0 };

		// 获得机器名
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
		{
			struct hostent* hp;
			hp = gethostbyname(strHost);
			if (hp != NULL)	{
				strcpy(strHost, hp->h_name);
			}
			// 检查缓冲区大小
			if (strlen(strHost) > nSize)
			{
				SetLastError(ERROR_INSUFFICIENT_BUFFER);
				return false;
			}

			// Unicode转化
#ifdef _UNICODE
			return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strName, nSize, NULL, NULL ));
#else
			_tcscpy(strName, strHost);
			return true;
#endif
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetLocalAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Get TCP address of local computer in dot format ex: "127.0.0.0"
// PARAMETERS:
//	LPTSTR strAddress: pointer to hold address string, must be long enough
//	UINT nSize: maximum size of this buffer
///////////////////////////////////////////////////////////////////////////////

//获取本地计算机的标准IP地址，如"127.0.0.0"
bool CSocketComm::GetLocalAddress(LPTSTR strAddress, UINT nSize)
{
	// 获得计算机本地地址
	if (strAddress != NULL && nSize > 0)
	{
		char strHost[HOSTNAME_SIZE] = { 0 };

		// 获得机器名
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
		{
			struct hostent* hp;
			hp = gethostbyname(strHost);
			if (hp != NULL && hp->h_addr_list[0] != NULL)
			{
				// 查看地址是否是4字节大小
				if ( hp->h_length < 4)
					return false;

				// 转化地址到点
				strHost[0] = 0;

				// 创建地址字符创
				sprintf(strHost, "%u.%u.%u.%u",
					(UINT)(((PBYTE) hp->h_addr_list[0])[0]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[1]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[2]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[3]));

				// 检查缓冲区是否足够
				if (strlen(strHost) > nSize)
				{
					SetLastError(ERROR_INSUFFICIENT_BUFFER);
					return false;
				}

			// Unicode转换

#ifdef _UNICODE
				return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strAddress,
					nSize, NULL, NULL ));
#else
				_tcscpy(strAddress, strHost);
				return true;
#endif
			}
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// WaitForConnection
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Wait for a network connection.  Only for connection type of socket
//				This function may fail, in this case it returns "INVALID_SOCKET"
// PARAMETERS:
//	SOCKET sock: a socket capable of receiving new connection (TCP: SOCK_STREAM)
///////////////////////////////////////////////////////////////////////////////

//等待一个网络连接，如果返回INVALID_SOCKET，表示失败
SOCKET CSocketComm::WaitForConnection(SOCKET sock)
{
	// 接收一个连接
	return accept(sock, 0, 0);
}


///////////////////////////////////////////////////////////////////////////////
// ShutdownConnection
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Shutdown a connection and close socket.  This will force all
//				transmission/reception to fail.
// PARAMETERS:
//	SOCKET sock: Socket to close
///////////////////////////////////////////////////////////////////////////////

//关闭一个连接并关闭一个socket，这将强迫所有的传输和接收失败
bool CSocketComm::ShutdownConnection(SOCKET sock)
{
	//
	shutdown(sock, SD_BOTH);
	return ( 0 == closesocket( sock ));
}


///////////////////////////////////////////////////////////////////////////////
// GetSockName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				retrieves the local name for a socket
// PARAMETERS:
//	SockAddrIn& saddr_in: object to store address
///////////////////////////////////////////////////////////////////////////////

//获得socket名称
bool CSocketComm::GetSockName(SockAddrIn& saddr_in)
{
	if (IsOpen())
	{
		int namelen = saddr_in.Size();
		return (SOCKET_ERROR != getsockname(GetSocket(), (LPSOCKADDR)saddr_in, &namelen));
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetPeerName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				retrieves the name of the peer to which a socket is connected
// PARAMETERS:
//	SockAddrIn& saddr_in: object to store address
///////////////////////////////////////////////////////////////////////////////

//获得socket要连接的地址
bool CSocketComm::GetPeerName(SockAddrIn& saddr_in)
{
	if (IsOpen())
	{
		int namelen = saddr_in.Size();
		return (SOCKET_ERROR != getpeername(GetSocket(), (LPSOCKADDR)saddr_in, &namelen));	
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// CreateSocket
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				This function creates a new socket for connection (SOCK_STREAM)
//				or an connectionless socket (SOCK_DGRAM).  A connectionless
//				socket should not call "accept()" since it cannot receive new
//				connection.  This is used as SERVER socket
// PARAMETERS:
//	LPCTSTR strServiceName: Service name or port number
//	int nProtocol: protocol to use (set to AF_INET)
//	int nType: type of socket to create (SOCK_STREAM, SOCK_DGRAM)
//	UINT uOptions: other options to use
///////////////////////////////////////////////////////////////////////////////

bool CSocketComm::CreateSocket(LPCTSTR strServiceName, int nProtocol, int nType, UINT uOptions /* = 0 */)
{
	// 如果已经打开，则返回false
	if ( IsOpen() )
		return false;

	SOCKADDR_IN sockAddr = { 0 };

	//地址设定
	SOCKET sock = socket(nProtocol, nType, 0);
	if (INVALID_SOCKET != sock)
	{
		sockAddr.sin_port = htons( GetPortNumber( strServiceName ) );
		if ( 0 != sockAddr.sin_port)
		{
			sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
			sockAddr.sin_family = nProtocol;

			if (uOptions & SO_REUSEADDR)
			{
				//设定相关选项
				BOOL optval = TRUE;
				if ( SOCKET_ERROR == setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof( BOOL ) ) )
				{
					closesocket( sock );
					return false;
				}
			}
			//如果是UDP协议
			if (nType == SOCK_DGRAM)
			{
				//如果允许广播
				if (uOptions & SO_BROADCAST)
				{
					// 允许广播
					BOOL optval = TRUE;
					if ( SOCKET_ERROR == setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof( BOOL ) ) )
					{
						closesocket( sock );
						return false;
					}

					// 设定变量
					m_bBroadcast = true;
				}

				// 如果需要广播，则需要设定进程互斥
				m_hMutex = CreateMutex(NULL, FALSE, NULL);
				if (NULL == m_hMutex)
				{
					closesocket( sock );
					return false;
				}

			}

			// 绑定一个本地地址
			if ( SOCKET_ERROR == bind(sock, (LPSOCKADDR)&sockAddr, sizeof(SOCKADDR_IN)))
			{
				closesocket( sock );
				m_bBroadcast = false;
				if (NULL != m_hMutex)
					CloseHandle( m_hMutex );
				m_hMutex = NULL;
				return false;
			}

			// 如果是TCP连接
			if (SOCK_STREAM == nType)
			{
				if ( SOCKET_ERROR == listen(sock, SOMAXCONN))
				{
					closesocket( sock );
					return false;
				}
			}

			// 保存socket
			m_hComm = (HANDLE) sock;
			return true;
		}
		else
			SetLastError(ERROR_INVALID_PARAMETER); //不能找到端口

		// 删除socket
		closesocket( sock );
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// ConnectTo
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Establish connection with a server service or port
// PARAMETERS:
//	LPCTSTR strDestination: hostname or address to connect (in .dot format)
//	LPCTSTR strServiceName: Service name or port number
//	int nProtocol: protocol to use (set to AF_INET)
//	int nType: type of socket to create (SOCK_STREAM, SOCK_DGRAM)
///////////////////////////////////////////////////////////////////////////////

//建立连接
bool CSocketComm::ConnectTo(LPCTSTR strDestination, LPCTSTR strServiceName, int nProtocol, int nType)
{
	// 如果socket已经打开
	if ( IsOpen() )
		return false;

	SOCKADDR_IN sockAddr = { 0 };

	//创建一个socket
	SOCKET sock = socket(nProtocol, nType, 0);
	if (INVALID_SOCKET != sock)
	{
		// 让socket绑定一个地址
		TCHAR strHost[HOSTNAME_SIZE] = { 0 };
		if (false == CSocketComm::GetLocalName( strHost, sizeof(strHost)/sizeof(TCHAR)))
		{
			closesocket( sock );
			return false;
		}
		//AfxMessageBox(strHost);
		sockAddr.sin_addr.s_addr = htonl( CSocketComm::GetIPAddress( strHost ) );
		sockAddr.sin_family = nProtocol;

		if ( SOCKET_ERROR == bind(sock, (LPSOCKADDR)&sockAddr, sizeof(SOCKADDR_IN)))
		{
			closesocket( sock );
			return false;
		}

		// 获得目标地址
		if ( strDestination[0]) {
			sockAddr.sin_addr.s_addr = htonl(CSocketComm::GetIPAddress( strDestination ) );
		}

		// 获得端口
		sockAddr.sin_port = htons( GetPortNumber( strServiceName ) );
		if ( 0 != sockAddr.sin_port )
		{
			// 连接服务器
			if (SOCKET_ERROR == connect( sock, (LPSOCKADDR)&sockAddr, sizeof(SOCKADDR_IN)))
			{
				closesocket( sock );
				return false;
			}

			// 保存socket
			m_hComm = (HANDLE) sock;
			return true;
		}
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// CloseComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Close Socket Communication
// PARAMETERS:
//		None
///////////////////////////////////////////////////////////////////////////////

//关闭socket
void CSocketComm::CloseComm()
{
	if (IsOpen())
	{
		//调用ShutdownConnection关闭
		ShutdownConnection((SOCKET)m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		m_bBroadcast = false;
	}
}


///////////////////////////////////////////////////////////////////////////////
// WatchComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Starts Socket Communication Working thread
// PARAMETERS:
//		None
///////////////////////////////////////////////////////////////////////////////

//启动socket通信线程
bool CSocketComm::WatchComm()
{
	//首先判断是否启动
	if (!IsStart())
	{
		//判断是否打开通信，即socket是否成功创建
		if (IsOpen())
		{
			HANDLE hThread;
			UINT uiThreadId = 0;
			//启动线程，使用_beginthreadex
			hThread = (HANDLE)_beginthreadex(NULL,	// 安全参数
									  0,	// 堆栈
						SocketThreadProc,	// 线程程序
									this,	// 线程参数
						CREATE_SUSPENDED,	//创建模式
							&uiThreadId);	// 线程ID
			//如果线程不为空
			if ( NULL != hThread)
			{
				//继续线程
				ResumeThread( hThread );
				m_hThread = hThread;
				return true;
			}
		}
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// StopComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Close Socket and Stop Communication thread
// PARAMETERS:
//		None
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::StopComm()
{
	// Close Socket
	if (IsOpen())
	{
		CloseComm();
		Sleep(50);
	}

	// Kill Thread
	if (IsStart())
	{
		if (WaitForSingleObject(m_hThread, 5000L) == WAIT_TIMEOUT)
			TerminateThread(m_hThread, 1L);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	// Clear Address list
	if (!m_AddrList.empty())
		m_AddrList.clear();

	// Destroy Synchronization objects
	if (NULL != m_hMutex)
	{
		CloseHandle( m_hMutex );
		m_hMutex = NULL;
	}

}


///////////////////////////////////////////////////////////////////////////////
// ReadComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Reads the Socket Communication
// PARAMETERS:
//		LPBYTE lpBuffer: buffer to place new data
//		DWORD dwSize: maximum size of buffer
//		DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////

//读入数据
DWORD CSocketComm::ReadComm(LPBYTE lpBuffer, DWORD dwSize, DWORD dwTimeout)
{
	_ASSERTE( IsOpen() );
	_ASSERTE( lpBuffer != NULL );

	if (lpBuffer == NULL || dwSize < 1L)
		return 0L;

	fd_set	fdRead  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = 0;
		stTime.tv_usec = dwTimeout*1000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET) m_hComm;
	// 设定描述符
	if ( !FD_ISSET( s, &fdRead ) )
		FD_SET( s, &fdRead );

	// 选择函数，设定超时时间
	DWORD dwBytesRead = 0L;
	int res = select( s+1, &fdRead, NULL, NULL, pstTime );
	if ( res > 0)
	{
		if (IsBroadcast() || IsSmartAddressing())
		{
			SOCKADDR_IN sockAddr = { 0 }; // 获得地址
			int nOffset = IsSmartAddressing() ? sizeof(sockAddr) : 0; 
			int nLen = sizeof(sockAddr);
			if ( dwSize < (DWORD) nLen)	// 缓冲区太小
			{
				SetLastError( ERROR_INVALID_USER_BUFFER );
				return -1L;
			}
			//获得数据
			res = recvfrom( s, (LPSTR)&lpBuffer[nOffset], dwSize, 0, (LPSOCKADDR)&sockAddr, &nLen);

			// clear 'sin_zero', we will ignore them with 'SockAddrIn' anyway!
			memset(&sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));
			
			if ( res >= 0)
			{
				LockList(); //锁定地址列表 

				// 删除调重复地址
				SockAddrIn sockin;
				sockin.SetAddr( &sockAddr );
				m_AddrList.remove( sockin );
				m_AddrList.insert(m_AddrList.end(), sockin);
				
				if (IsSmartAddressing())
				{
					memcpy(lpBuffer, &sockAddr, sizeof(sockAddr));
					res += sizeof(sockAddr);
				}

				UnlockList(); // 解开地址列表
			}
		}
		else
		{
			res = recv( s, (LPSTR)lpBuffer, dwSize, 0);
		}

		dwBytesRead = (DWORD)((res > 0)?(res) : (-1));
	}

	return dwBytesRead;
}


///////////////////////////////////////////////////////////////////////////////
// WriteComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Writes data to the Socket Communication
// PARAMETERS:
//		const LPBYTE lpBuffer: data to write
//		DWORD dwCount: maximum characters to write
//		DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////

//发送数据
DWORD CSocketComm::WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout)
{
	_ASSERTE( IsOpen() );
	_ASSERTE( NULL != lpBuffer );

	// 如果没有建立连接或者缓冲区为空，则返回
	if (!IsOpen() || NULL == lpBuffer)
		return 0L;
	//fd_set 是一个结构体，可以被很多的windows socket函数使用，如select，在socket2.0中使用
	//typedef struct fd_set {
	//u_int    fd_count;                 // 数量
	//SOCKET   fd_array[FD_SETSIZE];     //socket 数组
	//} fd_set;

	fd_set	fdWrite  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = 0;
		stTime.tv_usec = dwTimeout*1000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET) m_hComm;
	// 设定描述符
	if ( !FD_ISSET( s, &fdWrite ) )
		FD_SET( s, &fdWrite );

	// 选择函数设定超时时间
	DWORD dwBytesWritten = 0L;
	int res = select( s+1, NULL, &fdWrite, NULL, pstTime );
	if ( res > 0)
	{
		// 发送消息广播或者点对点发送
		if (IsBroadcast() || IsSmartAddressing())
		{
			// use offset for Smart addressing
			int nOffset = IsSmartAddressing() ? sizeof(SOCKADDR_IN) : 0;
			if (IsSmartAddressing())
			{
				if ( dwCount < sizeof(SOCKADDR_IN))	// error - buffer to small
				{
					SetLastError( ERROR_INVALID_USER_BUFFER );
					return -1L;
				}

				// 从缓冲区中获得地址
				SockAddrIn sockAddr;
				sockAddr.SetAddr((SOCKADDR_IN*) lpBuffer);

				// 获得地址然后发送
				if (sockAddr.sockAddrIn.sin_addr.s_addr != htonl(INADDR_BROADCAST))
				{
					res = sendto( s, (LPCSTR)&lpBuffer[nOffset], dwCount-nOffset, 0,
						(LPSOCKADDR)sockAddr, sockAddr.Size());
					dwBytesWritten = (DWORD)((res >= 0)?(res) : (-1));
					return dwBytesWritten;
				}
			}

			// 向所有用户广播
			LockList(); // 锁住地址列表
			
			CSockAddrList::iterator iter = m_AddrList.begin();
			for( ; iter != m_AddrList.end(); )
			{
				//循环发送信息
				res = sendto( s, (LPCSTR)&lpBuffer[nOffset], dwCount-nOffset, 0, (LPSOCKADDR)(*iter), iter->Size());
				if (res < 0)
				{
					CSockAddrList::iterator deladdr = iter;
					++iter;	// 下一个
					m_AddrList.erase( deladdr );
				}
				else
					++iter;	// 下一个
			}

			UnlockList(); // 解锁

			// UDP总是返回true
			res = (int) dwCount - nOffset;
		}
		else // 发送到单个客户端
			res = send( s, (LPCSTR)lpBuffer, dwCount, 0);

		dwBytesWritten = (DWORD)((res >= 0)?(res) : (-1));
	}

	return dwBytesWritten;
}


///////////////////////////////////////////////////////////////////////////////
// Run
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		This function runs the main thread loop
//		this implementation can be overloaded.
//      This function calls CSocketComm::OnDataReceived() (Virtual Function)
// PARAMETERS:
// NOTES:
//		You should not wait on the thread to end in this function or overloads
///////////////////////////////////////////////////////////////////////////////

//该函数由主线程来循环调用
void CSocketComm::Run()
{
	BYTE	buffer[BUFFER_SIZE];
	DWORD	dwBytes  = 0L;

	HANDLE	hThread = GetCurrentThread();
	DWORD	dwTimeout = DEFAULT_TIMEOUT;

	// 是否运行服务器模式
	if (IsServer())
	{
		//是否广播模式
		if (!IsBroadcast())
		{
			SOCKET sock = (SOCKET) m_hComm;
			sock = WaitForConnection( sock );

			// 等待新的连接
			if (sock != INVALID_SOCKET)
			{
				//关闭连接
				ShutdownConnection( (SOCKET) m_hComm);
				m_hComm = (HANDLE) sock;
				OnEvent( EVT_CONSUCCESS ); // connect
			}
			else
			{
				// 如果已经关闭则不发送事件，否则发送
				if (IsOpen())
					OnEvent( EVT_CONFAILURE ); // 等待失败
				return;
			}
		}
	}

	//如果socket已经创建
	while( IsOpen() )
	{
		// 采用阻塞式socket，等待事件通知
		dwBytes = ReadComm(buffer, sizeof(buffer), dwTimeout);

		// 如果有错误发生
		if (dwBytes == (DWORD)-1)
		{
			// 如果要关闭，则不发送事件
			if (IsOpen())
				OnEvent( EVT_CONDROP ); // 失去连接
			break;
		}

		// 是否有数据收到
		if (IsSmartAddressing() && dwBytes == sizeof(SOCKADDR_IN))
			OnEvent( EVT_ZEROLENGTH );
		else if (dwBytes > 0L)
		{
			OnDataReceived( buffer, dwBytes);
		}

		Sleep(0);
	}
}


///////////////////////////////////////////////////////////////////////////////
// SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////

//socket线程
UINT WINAPI CSocketComm::SocketThreadProc(LPVOID pParam)
{
	//reinterpret_cast用于各种指针的转化
	CSocketComm* pThis = reinterpret_cast<CSocketComm*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->Run();

	return 1L;
} 
