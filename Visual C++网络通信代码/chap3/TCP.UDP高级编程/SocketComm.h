///////////////////////////////////////////////////////////////////////////////
// FILE : SocketComm.h
// Header file for CSocketComm class
// CSocketComm
//     Generic class for Socket Communication
///////////////////////////////////////////////////////////////////////////////

#ifndef _SOCKETCOMM_H_
#define _SOCKETCOMM_H_
#include <list>

#include <stdlib.h>
#if(_WIN32_WINNT >= 0x0400)
#include <winsock2.h>
#include <mswsock.h>
#else
#include <winsock.h>
#endif /* _WIN32_WINNT >=  0x0400 */
#pragma comment(lib, "ws2_32")

// Event value
#define EVT_CONSUCCESS		0x0000	// Connection established
#define EVT_CONFAILURE		0x0001	// General failure - Wait Connection failed
#define EVT_CONDROP			0x0002	// Connection dropped
#define EVT_ZEROLENGTH		0x0003	// Zero length message


#define BUFFER_SIZE		MAX_PATH
#define HOSTNAME_SIZE	MAX_PATH
#define STRING_LENGTH	40

struct SockAddrIn {
public:
	SockAddrIn() { Clear(); }
	SockAddrIn(const SockAddrIn& sin) { Copy( sin ); }
	~SockAddrIn() { }
	SockAddrIn& Copy(const SockAddrIn& sin);
	void	Clear() { memset(&sockAddrIn, 0, sizeof(sockAddrIn)); }
	bool	IsEqual(const SockAddrIn& sin);
	bool	IsGreater(const SockAddrIn& sin);
	bool	IsLower(const SockAddrIn& pm);
	bool	IsNull() const { return ((sockAddrIn.sin_addr.s_addr==0L)&&(sockAddrIn.sin_port==0)); }
	ULONG	GetIPAddr() const { return sockAddrIn.sin_addr.s_addr; }
	short	GetPort() const { return sockAddrIn.sin_port; }
	bool	CreateFrom(LPCTSTR sAddr, LPCTSTR sService);
	SockAddrIn& operator=(const SockAddrIn& sin) { return Copy( sin ); }
	bool	operator==(const SockAddrIn& sin) { return IsEqual( sin ); }
	bool	operator!=(const SockAddrIn& sin) { return !IsEqual( sin ); }
	bool	operator<(const SockAddrIn& sin)  { return IsLower( sin ); }
	bool	operator>(const SockAddrIn& sin)  { return IsGreater( sin ); }
	bool	operator<=(const SockAddrIn& sin) { return !IsGreater( sin ); }
	bool	operator>=(const SockAddrIn& sin) { return !IsLower( sin ); }
	operator LPSOCKADDR() { return (LPSOCKADDR)(&sockAddrIn); }
	size_t	Size() const { return sizeof(sockAddrIn); }
	void	SetAddr(SOCKADDR_IN* psin) { memcpy(&sockAddrIn, psin, Size()); }
	SOCKADDR_IN sockAddrIn;
};

typedef std::list<SockAddrIn> CSockAddrList;

class CSocketComm
{
public:
	CSocketComm();
	virtual ~CSocketComm();

	bool IsOpen() const;	// 判断socket是否正确
	bool IsStart() const;	// 线程是否启动
	bool IsServer() const;	// 是否以服务器形式启动
	bool IsBroadcast() const; // 是否允许UDP广播
	bool IsSmartAddressing() const;	// Is Smart Addressing mode support
	SOCKET GetSocket() const;	// 返回socket句柄
	void SetServerState(bool bServer);	// 设定运行模式是否为服务器模式
	void SetSmartAddressing(bool bSmartAddressing);	// Set Smart addressing mode
	bool GetSockName(SockAddrIn& saddr_in);	// 获得socket名即地址
	bool GetPeerName(SockAddrIn& saddr_in);	// 获得Peer Socket 名 - 地址
	void AddToList(const SockAddrIn& saddr_in);	//将地址增加到列表
	void RemoveFromList(const SockAddrIn& saddr_in);	// 从列表中删除列表
	void CloseComm();		// 关闭 Socket
	bool WatchComm();		// 启动 Socket 线程
	void StopComm();		// 停止 Socket 线程

	// 创建服务器端socket
	bool CreateSocket(LPCTSTR strServiceName, int nProtocol, int nType, UINT uOptions = 0);
	// 创建一个客户端的socket
	bool ConnectTo(LPCTSTR strDestination, LPCTSTR strServiceName, int nProtocol, int nType);

	// 事件处理函数 - 接收数据
	virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
	virtual void OnEvent(UINT uEvent);
	// 主线程函数
	virtual void Run();

	// 数据函数
	DWORD ReadComm(LPBYTE lpBuffer, DWORD dwSize, DWORD dwTimeout);
	DWORD WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout);

	// Utility functions
	static SOCKET WaitForConnection(SOCKET sock); // 等待新的连接
	static bool ShutdownConnection(SOCKET sock);  // 关闭连接
	static USHORT GetPortNumber( LPCTSTR strServiceName );	// 获得服务端口号
	static ULONG GetIPAddress( LPCTSTR strHostName );	// 获得主机IP地址
	static bool GetLocalName(LPTSTR strName, UINT nSize);	// 获得机器名
	static bool GetLocalAddress(LPTSTR strAddress, UINT nSize);	// 获得本地地址
// SocketComm - data
protected:
	HANDLE		m_hComm;		// socket句柄
	HANDLE		m_hThread;		// 线程句柄
	bool		m_bServer;		// 为真表示服务器模式
	bool		m_bSmartAddressing;	// Smart Addressing mode (true) - many listeners
	bool		m_bBroadcast;	// Broadcast mode
	CSockAddrList m_AddrList;	// Connection address list for broadcast
	HANDLE		m_hMutex;		// Mutex object
// SocketComm - function
protected:
	// 同步函数
	void LockList();			// 锁住对象
	void UnlockList();			// 解开对象

	static UINT WINAPI SocketThreadProc(LPVOID pParam);

private:
};

#endif // _SOCKETCOMM_H_
