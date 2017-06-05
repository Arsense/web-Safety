/*——————————————————————————————————————

	文件：		TcpIpDog.cpp

	说明:
	——————————————————————————
		Winsock 钩子，用来截获 Winsock 调用从而拦截
	TCP/IP封包，并做相应处理。这里将发送接收的封包
	作为调试信息输出
	——————————————————————————

	工程：		PacketCapture
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/21
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	——————————————————————————
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买本软件源代码的用户被默认授权
	可以在自己的程序中使用本软件的部分代码，但作者不对
	代码产生的任何后果负责。
	
		使用了本软件代码的程序只能以可执行文件形式发布，
	未经特别许可，不能将含有本软件源代码的源程序以任何
	形式发布。
	——————————————————————————	
*/
//——————————————————————————————————————
// include header file and global variables

#include "stdafx.h"
#include "overlapped.h"

//
// 进程间全局变量
//
#pragma data_seg(".inidata")
	int				m_iDllCount		= 0;
#pragma data_seg()

//
// 进程内全局变量
//
CRITICAL_SECTION	gCriticalSection;			// 代码段保护变量
WSPPROC_TABLE		NextProcTable   ;			// 保存30个服务提供者指针
TCHAR				m_sProcessName[MAX_PATH];	// 保存当前进程名称
COverlapped			m_Overlapped;				// 保存重叠操作相关信息的类

//
// 自定义注册表分支用来保存系统基础服务提供者的路径
//
#define REG_INSTALL_KEY		\
	_T("SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Xstudio_Packet_Capture")

//——————————————————————————————————————
// 私有函数

void PrintSocket(SOCKET s, DWORD bytes, TCHAR *sExt)
{
	SOCKADDR_IN local_addr, remote_addr;
	int local_addr_len	= sizeof(local_addr);
	int remote_addr_len = sizeof(remote_addr);
	getsockname(s, (SOCKADDR*)&local_addr, &local_addr_len);

	getpeername(s, (SOCKADDR*)&remote_addr, &remote_addr_len);

	CString sTemp;
	sTemp.Format("    Socket %u %s\n", s, sExt);
	OutputDebugString(sTemp);
	sTemp.Format("    %u.%u.%u.%u:%u -> %u.%u.%u.%u:%u transfered %u bytes\n"
		, local_addr.sin_addr.S_un.S_un_b.s_b1
		, local_addr.sin_addr.S_un.S_un_b.s_b2
		, local_addr.sin_addr.S_un.S_un_b.s_b3
		, local_addr.sin_addr.S_un.S_un_b.s_b4
		, ntohs(local_addr.sin_port)
		, remote_addr.sin_addr.S_un.S_un_b.s_b1
		, remote_addr.sin_addr.S_un.S_un_b.s_b2
		, remote_addr.sin_addr.S_un.S_un_b.s_b3
		, remote_addr.sin_addr.S_un.S_un_b.s_b4
		, ntohs(remote_addr.sin_port)
		, bytes
		);
	OutputDebugString(sTemp);
}

void PrintReturnCode(int iReturnCode, LPINT lpErrno)
{
	CString sOutput , sTemp, sTemp1;
	switch(iReturnCode)
	{
	case SOCKET_ERROR:
		sTemp = "SOCKET_ERROR";
		break;
	case 0:
		sTemp = "SOCKET_SUCCESS";
		break;
	default:
		sTemp = "OTHER";
		break;
	}
	switch(*lpErrno)
	{
	case WSA_IO_PENDING:
		sTemp1 = "WSA_IO_PENDING";
		break;
	default:
		sTemp1 = "OTHER";
		break;
	}
	sOutput.Format("    ReturnCode: %s(%d); *lpErrno: %s(%d)\n"
		, sTemp, iReturnCode, sTemp1, *lpErrno);
	OutputDebugString(sOutput);
}

void PrintProtocolInfo(
	IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
	IN	TCHAR				*sPathName
)
{
	OutputDebugString("  ProtocolInfo:\n");
	CString sTemp, sOutput;
	sTemp.Format("    LibraryPath: %s\n", sPathName);
	OutputDebugString(sTemp);

	sTemp.Format("    dwServiceFlags1: 0x%08X; %d\n"
		, pProtocolInfo->dwServiceFlags1
		, pProtocolInfo->dwServiceFlags1
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwServiceFlags2: 0x%08X; %d\n"
		, pProtocolInfo->dwServiceFlags2
		, pProtocolInfo->dwServiceFlags2
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwServiceFlags3: 0x%08X; %d\n"
		, pProtocolInfo->dwServiceFlags3
		, pProtocolInfo->dwServiceFlags3
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwServiceFlags4: 0x%08X; %d\n"
		, pProtocolInfo->dwServiceFlags4
		, pProtocolInfo->dwServiceFlags4
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwProviderFlags: 0x%08X; %d\n"
		, pProtocolInfo->dwProviderFlags
		, pProtocolInfo->dwProviderFlags
		);
	OutputDebugString(sTemp);

	sTemp.Format("    ProviderId: %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n"
		, pProtocolInfo->ProviderId.Data1
		, pProtocolInfo->ProviderId.Data2
		, pProtocolInfo->ProviderId.Data3
		, pProtocolInfo->ProviderId.Data4[0]
		, pProtocolInfo->ProviderId.Data4[1]
		, pProtocolInfo->ProviderId.Data4[2]
		, pProtocolInfo->ProviderId.Data4[3]
		, pProtocolInfo->ProviderId.Data4[4]
		, pProtocolInfo->ProviderId.Data4[5]
		, pProtocolInfo->ProviderId.Data4[6]
		, pProtocolInfo->ProviderId.Data4[7]
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwCatalogEntryId: 0x%08X; %d\n"
		, pProtocolInfo->dwCatalogEntryId
		, pProtocolInfo->dwCatalogEntryId
		);
	OutputDebugString(sTemp);

	sTemp.Format("    ProtocolChain.ChainLen: 0x%08X; %d\n"
		, pProtocolInfo->ProtocolChain.ChainLen
		, pProtocolInfo->ProtocolChain.ChainLen
		);
	OutputDebugString(sTemp);

	sTemp.Empty();
	OutputDebugString("    ProtocolChain.ChainEntries: ");
	for(int i = 0; i < MAX_PROTOCOL_CHAIN; i++)
	{
		CString s = "";
		if(i == MAX_PROTOCOL_CHAIN - 1)
			s.Format("0x%08X\n", pProtocolInfo->ProtocolChain.ChainEntries[i]);
		else
			s.Format("0x%08X->", pProtocolInfo->ProtocolChain.ChainEntries[i]);
		sTemp += s;
	}
	OutputDebugString(sTemp);

	sTemp.Format("    iVersion: 0x%08X; %d\n"
		, pProtocolInfo->iVersion
		, pProtocolInfo->iVersion
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iAddressFamily: 0x%08X; %d\n"
		, pProtocolInfo->iAddressFamily
		, pProtocolInfo->iAddressFamily
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iMaxSockAddr: 0x%08X; %d\n"
		, pProtocolInfo->iMaxSockAddr
		, pProtocolInfo->iMaxSockAddr
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iMinSockAddr: 0x%08X; %d\n"
		, pProtocolInfo->iMinSockAddr
		, pProtocolInfo->iMinSockAddr
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iSocketType: 0x%08X; %d\n"
		, pProtocolInfo->iSocketType
		, pProtocolInfo->iSocketType
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iProtocol: 0x%08X; %d\n"
		, pProtocolInfo->iProtocol
		, pProtocolInfo->iProtocol
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iProtocolMaxOffset: 0x%08X; %d\n"
		, pProtocolInfo->iProtocolMaxOffset
		, pProtocolInfo->iProtocolMaxOffset
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iNetworkByteOrder: 0x%08X; %d\n"
		, pProtocolInfo->iNetworkByteOrder
		, pProtocolInfo->iNetworkByteOrder
		);
	OutputDebugString(sTemp);

	sTemp.Format("    iSecurityScheme: 0x%08X; %d\n"
		, pProtocolInfo->iSecurityScheme
		, pProtocolInfo->iSecurityScheme
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwMessageSize: 0x%08X; %d\n"
		, pProtocolInfo->dwMessageSize
		, pProtocolInfo->dwMessageSize
		);
	OutputDebugString(sTemp);

	sTemp.Format("    dwProviderReserved: 0x%08X; %d\n"
		, pProtocolInfo->dwProviderReserved
		, pProtocolInfo->dwProviderReserved
		);
	OutputDebugString(sTemp);

	sTemp = pProtocolInfo->szProtocol;
	OutputDebugString("    szProtocol: ");
	OutputDebugString(sTemp);
	OutputDebugString("\n");
}

void GetRightEntryIdItem(
	IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
	OUT	TCHAR				*sItem
)
{
	if(pProtocolInfo->ProtocolChain.ChainLen <= 1)
		_stprintf(sItem, _T("%u"), pProtocolInfo->dwCatalogEntryId);
	else
		_stprintf(sItem, _T("%u"), pProtocolInfo->ProtocolChain
			.ChainEntries[pProtocolInfo->ProtocolChain.ChainLen - 1]);
}

BOOL GetHookProvider(
	IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
	OUT	TCHAR				*sPathName
)
{
	TCHAR sItem[21];
	GetRightEntryIdItem(pProtocolInfo, sItem);

	HKEY	hSubkey;
	DWORD	ulDateLenth	= MAX_PATH;
	TCHAR	sTemp[MAX_PATH];

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE
		, REG_INSTALL_KEY, 0, KEY_ALL_ACCESS, &hSubkey) != ERROR_SUCCESS)
		return FALSE;
	if (RegQueryValueEx(hSubkey, sItem, 0, NULL, (BYTE*)sTemp, &ulDateLenth)
		|| ExpandEnvironmentStrings(sTemp, sPathName, ulDateLenth) == 0)
		return FALSE;
	if(sPathName[0] == '\0' && sTemp[0] != '\0')
		_tcscpy(sPathName, sTemp);
	RegCloseKey(hSubkey);

	return TRUE;
}

//——————————————————————————————————————
// Winsock 2 服务提供者钩子函数

void CALLBACK CompletionRoutine (
	IN    DWORD				dwError, 
	IN    DWORD				cbTransferred, 
	IN    LPWSAOVERLAPPED	lpOverlapped, 
	IN    DWORD				dwFlags 
)
{
	ODS(_T("CompletionRoutine ..."));

	int iIndex = m_Overlapped.FindOverlapped(lpOverlapped);
	if(iIndex < 0)
		return;

	CString sTemp;
	if(m_Overlapped.m_OverlappedRecorder[iIndex].FunctionType == 1)//WSPRecvFrom
	{
		PrintSocket(m_Overlapped.m_OverlappedRecorder[iIndex].s
			, cbTransferred, "WSPRecvFrom");
		OutputDebugString("    <==WSPRecvFrom WSA_IO_PENDING\n");
	}
	else if(m_Overlapped.m_OverlappedRecorder[iIndex].FunctionType == 0) //WSPRecv
	{
		PrintSocket(m_Overlapped.m_OverlappedRecorder[iIndex].s
			, cbTransferred, "WSPRecv");
		OutputDebugString("    <==WSPRecv WSA_IO_PENDING\n");
	}

	if(m_Overlapped.m_OverlappedRecorder[iIndex].lpCompletionRoutine != NULL)
	{
		m_Overlapped.m_OverlappedRecorder[iIndex].lpCompletionRoutine(dwError
			, cbTransferred, lpOverlapped, dwFlags);
	}

	m_Overlapped.DeleteOverlapped(iIndex);
}

SOCKET WSPAPI WSPSocket(
	int			af,                               
	int			type,                             
	int			protocol,                         
	LPWSAPROTOCOL_INFOW lpProtocolInfo,   
	GROUP		g,                              
	DWORD		dwFlags,                        
	LPINT		lpErrno
)
{
	ODS(_T("WSPSocket ..."));
	return NextProcTable.lpWSPSocket(af, type
		, protocol, lpProtocolInfo, g, dwFlags, lpErrno);
}

int WSPAPI WSPCloseSocket(
	SOCKET		s,
	LPINT		lpErrno
)
{
	ODS(_T("WSPCloseSocket ..."));
	return NextProcTable.lpWSPCloseSocket(s, lpErrno);
}

int WSPAPI WSPConnect(
	SOCKET			s,
	const struct	sockaddr FAR * name,
	int				namelen,
	LPWSABUF		lpCallerData,
	LPWSABUF		lpCalleeData,
	LPQOS			lpSQOS,
	LPQOS			lpGQOS,
	LPINT			lpErrno
)
{
	ODS(_T("WSPConnect ..."));
	return NextProcTable.lpWSPConnect(s, name, namelen, lpCallerData
		, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
}

SOCKET WSPAPI WSPAccept(
	SOCKET			s,
	struct sockaddr FAR *addr,
	LPINT			addrlen,
	LPCONDITIONPROC	lpfnCondition,
	DWORD			dwCallbackData,
	LPINT			lpErrno
)
{
	ODS(_T("WSPAccept ..."));
	return NextProcTable.lpWSPAccept(s, addr, addrlen, lpfnCondition
		, dwCallbackData, lpErrno);
}

int WSPAPI WSPSend(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesSent,
	DWORD			dwFlags,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("WSPSend ..."));

	PrintSocket(s, *lpNumberOfBytesSent, "WSPSend");

	return NextProcTable.lpWSPSend(s, lpBuffers, dwBufferCount
		, lpNumberOfBytesSent, dwFlags, lpOverlapped
		, lpCompletionRoutine, lpThreadId, lpErrno);

}

int WSPAPI WSPSendTo(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesSent,
	DWORD			dwFlags,
	const struct sockaddr FAR * lpTo,
	int				iTolen,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("WSPSendTo ..."));

	PrintSocket(s, *lpNumberOfBytesSent, "WSPSendTo");

	return NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount
		, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped
		, lpCompletionRoutine, lpThreadId, lpErrno);

}

int WSPAPI WSPRecv(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesRecvd,
	LPDWORD			lpFlags,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("WSPRecv ..."));

	BOOL IsSetCompletionRoutine = FALSE;
	if(lpOverlapped && lpCompletionRoutine && m_Overlapped.AddOverlapped(s
		, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags
		, lpOverlapped, lpCompletionRoutine, NULL, NULL, 0/*WSPRecv*/)
		)
	{
		OutputDebugString("    ==>WSPRecv WSA_IO_PENDING\n");
		lpCompletionRoutine		= CompletionRoutine;
		IsSetCompletionRoutine	= TRUE;
	}

	int iRet = NextProcTable.lpWSPRecv(s, lpBuffers, dwBufferCount
		, lpNumberOfBytesRecvd, lpFlags, lpOverlapped
		, lpCompletionRoutine, lpThreadId, lpErrno);

	PrintReturnCode(iRet, lpErrno);

	if(iRet == SOCKET_ERROR || IsSetCompletionRoutine == TRUE)
		return iRet;

	PrintSocket(s, *lpNumberOfBytesRecvd, "WSPRecv");

	return iRet;
}

int WSPAPI WSPRecvFrom (
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesRecvd,
	LPDWORD			lpFlags,
	struct sockaddr FAR * lpFrom,
	LPINT			lpFromlen,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("WSPRecvFrom ..."));

	BOOL IsSetCompletionRoutine = FALSE;
	if(lpOverlapped && lpCompletionRoutine && m_Overlapped.AddOverlapped(s
		, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags
		, lpOverlapped, lpCompletionRoutine, lpFrom, lpFromlen, 1/*WSPRecvFrom*/)
		)
	{
		OutputDebugString("    ==>WSPRecvFrom WSA_IO_PENDING\n");
		lpCompletionRoutine		= CompletionRoutine;
		IsSetCompletionRoutine	= TRUE;
	}

	int iRet = NextProcTable.lpWSPRecvFrom(s, lpBuffers, dwBufferCount
		, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen
		, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);

	PrintReturnCode(iRet, lpErrno);

	if(iRet == SOCKET_ERROR || IsSetCompletionRoutine == TRUE)
		return iRet;

	PrintSocket(s, *lpNumberOfBytesRecvd, "WSPRecvFrom");

	return iRet;
}

int WSPAPI WSPAddressToString (
  LPSOCKADDR	lpsaAddress,            
  DWORD			dwAddressLength,             
  LPWSAPROTOCOL_INFOW lpProtocolInfo,   
  LPWSTR		lpszAddressString,          
  LPDWORD		lpdwAddressStringLength,   
  LPINT			lpErrno                      
)
{
	ODS(_T("WSPAddressToString ..."));
	return NextProcTable.lpWSPAddressToString(lpsaAddress
		, dwAddressLength, lpProtocolInfo
		, lpszAddressString, lpdwAddressStringLength, lpErrno);
}

int WSPAPI WSPAsyncSelect (
  SOCKET		s,            
  HWND			hWnd,           
  unsigned int	wMsg,   
  long			lEvent,         
  LPINT			lpErrno        
)
{
	ODS(_T("WSPAsyncSelect ..."));
	return NextProcTable.lpWSPAsyncSelect(s, hWnd, wMsg, lEvent, lpErrno);
}
 
int WSPAPI WSPBind (
  SOCKET		s,                           
  const struct	sockaddr FAR * name,   
  int			namelen,                        
  LPINT			lpErrno                       
)
{
	ODS(_T("WSPBind ..."));
	return NextProcTable.lpWSPBind(s, name, namelen, lpErrno);
}

int WSPAPI WSPCancelBlockingCall (
  LPINT			lpErrno  
)
{
	ODS(_T("WSPCancelBlockingCall ..."));
	return NextProcTable.lpWSPCancelBlockingCall(lpErrno);
}

int WSPAPI WSPCleanup (
  LPINT			lpErrno  
)
{
	ODS(_T("WSPCleanup ..."));
	return NextProcTable.lpWSPCleanup(lpErrno);
}
 
int WSPAPI WSPDuplicateSocket (
  SOCKET		s,                             
  DWORD			dwProcessId,                    
  LPWSAPROTOCOL_INFOW lpProtocolInfo,   
  LPINT			lpErrno                         
)
{
	ODS(_T("WSPDuplicateSocket ..."));
	return NextProcTable.lpWSPDuplicateSocket(
		s, dwProcessId, lpProtocolInfo, lpErrno);
}

int WSPAPI WSPEnumNetworkEvents (
  SOCKET		s,                             
  WSAEVENT		hEventObject,                
  LPWSANETWORKEVENTS lpNetworkEvents,   
  LPINT			lpErrno                         
)
{
	ODS(_T("WSPEnumNetworkEvents ..."));
	return NextProcTable.lpWSPEnumNetworkEvents(
		s, hEventObject, lpNetworkEvents, lpErrno);
}

int WSPAPI WSPEventSelect (
  SOCKET		s,              
  WSAEVENT		hEventObject,   
  long			lNetworkEvents,   
  LPINT			lpErrno          
)
{
	ODS(_T("WSPEventSelect ..."));
	return NextProcTable.lpWSPEventSelect(
		s, hEventObject, lNetworkEvents, lpErrno);
}

BOOL WSPAPI WSPGetOverlappedResult (
  SOCKET		s,                       
  LPWSAOVERLAPPED lpOverlapped,   
  LPDWORD		lpcbTransfer,           
  BOOL			fWait,                     
  LPDWORD		lpdwFlags,              
  LPINT			lpErrno                   
)
{
	ODS(_T("WSPGetOverlappedResult ..."));
	return NextProcTable.lpWSPGetOverlappedResult(s, lpOverlapped
		, lpcbTransfer, fWait, lpdwFlags, lpErrno);
}

int WSPAPI WSPGetPeerName (
  SOCKET		s,                     
  struct		sockaddr FAR * name,   
  LPINT			namelen,                
  LPINT			lpErrno                 
)
{
	return NextProcTable.lpWSPGetPeerName(s, name, namelen, lpErrno);
}

int WSPAPI WSPGetSockName (
  SOCKET		s,                     
  struct		sockaddr FAR * name,   
  LPINT			namelen,                
  LPINT			lpErrno                 
)
{
	return NextProcTable.lpWSPGetSockName(s, name, namelen, lpErrno);
}

int WSPAPI WSPGetSockOpt (
  SOCKET		s,            
  int			level,           
  int			optname,         
  char FAR *	optval,   
  LPINT			optlen,        
  LPINT			lpErrno        
)
{
	ODS(_T("WSPGetSockOpt ..."));
	return NextProcTable.lpWSPGetSockOpt(
		s, level, optname, optval, optlen, lpErrno);
}

BOOL WSPAPI WSPGetQOSByName (
  SOCKET		s,             
  LPWSABUF		lpQOSName,   
  LPQOS			lpQOS,          
  LPINT			lpErrno         
)
{
	ODS(_T("WSPGetQOSByName ..."));
	return NextProcTable.lpWSPGetQOSByName(s, lpQOSName, lpQOS, lpErrno);
}

int WSPAPI WSPIoctl (
  SOCKET		s,                                                
  DWORD			dwIoControlCode,                                   
  LPVOID		lpvInBuffer,                                      
  DWORD			cbInBuffer,                                        
  LPVOID		lpvOutBuffer,                                     
  DWORD			cbOutBuffer,                                       
  LPDWORD		lpcbBytesReturned,                               
  LPWSAOVERLAPPED lpOverlapped,                            
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,  
  LPWSATHREADID lpThreadId,                                
  LPINT			lpErrno                                            
)
{
	ODS(_T("WSPIoctl ..."));
	return NextProcTable.lpWSPIoctl(s, dwIoControlCode, lpvInBuffer
		, cbInBuffer, lpvOutBuffer, cbOutBuffer, lpcbBytesReturned
		, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

SOCKET WSPAPI WSPJoinLeaf (
  SOCKET		s,                           
  const struct	sockaddr FAR * name,   
  int			namelen,                        
  LPWSABUF		lpCallerData,              
  LPWSABUF		lpCalleeData,              
  LPQOS			lpSQOS,                       
  LPQOS			lpGQOS,                       
  DWORD			dwFlags,                      
  LPINT			lpErrno                       
)
{
	ODS(_T("WSPJoinLeaf ..."));
	return NextProcTable.lpWSPJoinLeaf(s, name, namelen, lpCallerData
		, lpCalleeData, lpSQOS, lpGQOS, dwFlags, lpErrno);
}

int WSPAPI WSPListen (
  SOCKET		s,      
  int			backlog,   
  LPINT			lpErrno  
)
{
	ODS(_T("WSPListen ..."));
	return NextProcTable.lpWSPListen(s, backlog, lpErrno);
}

int WSPAPI WSPRecvDisconnect (
  SOCKET		s,                           
  LPWSABUF		lpInboundDisconnectData,   
  LPINT			lpErrno                       
)
{
	ODS(_T("WSPRecvDisconnect ..."));
	return NextProcTable.lpWSPRecvDisconnect(s, lpInboundDisconnectData, lpErrno);
}

int WSPAPI WSPSelect (
  int			nfds,                             
  fd_set FAR *	readfds,                 
  fd_set FAR *	writefds,                
  fd_set FAR *	exceptfds,               
  const struct	timeval FAR * timeout,   
  LPINT			lpErrno                         
)
{
	ODS(_T("WSPSelect ..."));
	return NextProcTable.lpWSPSelect(nfds
		, readfds, writefds, exceptfds, timeout, lpErrno);
}
 
int WSPAPI WSPSendDisconnect (
  SOCKET		s,                            
  LPWSABUF		lpOutboundDisconnectData,   
  LPINT			lpErrno                        
)
{
	ODS(_T("WSPSendDisconnect ..."));
	return NextProcTable.lpWSPSendDisconnect(
		s, lpOutboundDisconnectData, lpErrno);
}

int WSPAPI WSPSetSockOpt (
  SOCKET		s,                  
  int			level,                 
  int			optname,               
  const char FAR * optval,   
  int			optlen,                
  LPINT			lpErrno              
)
{
	ODS(_T("WSPSetSockOpt ..."));
	return NextProcTable.lpWSPSetSockOpt(
		s, level, optname, optval, optlen, lpErrno);
}

int WSPAPI WSPShutdown (
  SOCKET		s,      
  int			how,       
  LPINT			lpErrno  
)
{
	ODS(_T("WSPShutdown ..."));
	return NextProcTable.lpWSPShutdown(s, how, lpErrno);
}

int WSPAPI WSPStringToAddress (
  LPWSTR		AddressString,                 
  INT			AddressFamily,                    
  LPWSAPROTOCOL_INFOW lpProtocolInfo,   
  LPSOCKADDR	lpAddress,                 
  LPINT			lpAddressLength,                
  LPINT			lpErrno                         
)
{
	ODS(_T("WSPStringToAddress ..."));
	return NextProcTable.lpWSPStringToAddress(AddressString, AddressFamily
		, lpProtocolInfo, lpAddress, lpAddressLength, lpErrno);
}
 
//——————————————————————————————————————
// Dll 入口函数

BOOL WINAPI DllMain(
	HINSTANCE	hModule, 
    DWORD		ul_reason_for_call, 
    LPVOID		lpReserved
)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
 		GetModuleFileName(NULL, m_sProcessName, MAX_PATH);

		InitializeCriticalSection(&gCriticalSection);

		EnterCriticalSection(&gCriticalSection);
		{
			m_iDllCount ++;
			DP1("DllMain Attach Count %d", m_iDllCount);
		}
		LeaveCriticalSection(&gCriticalSection);

		ODS2(m_sProcessName,_T(" Loading ..."));
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		EnterCriticalSection(&gCriticalSection);
		{
			m_iDllCount -- ;
			DP1("DllMain Attach Count %d", m_iDllCount);
		}
		LeaveCriticalSection(&gCriticalSection);

		ODS2(m_sProcessName,_T(" Exit ..."));
	}

	return TRUE;
}

//——————————————————————————————————————
// DLL导出的函数

int WSPAPI WSPStartup(
	WORD				wVersionRequested,
	LPWSPDATA			lpWSPData,
	LPWSAPROTOCOL_INFOW	lpProtocolInfo,
	WSPUPCALLTABLE		upcallTable,
	LPWSPPROC_TABLE		lpProcTable
)
{
	ODS(_T("==> WSPStartup..."));

	TCHAR				sLibraryPath[512];
    LPWSPSTARTUP        WSPStartupFunc      = NULL;
	HMODULE				hLibraryHandle		= NULL;
    INT                 ErrorCode           = 0; 

	if (!GetHookProvider(lpProtocolInfo, sLibraryPath)
		|| (hLibraryHandle = LoadLibrary(sLibraryPath)) == NULL
		|| (WSPStartupFunc = (LPWSPSTARTUP)GetProcAddress(
			hLibraryHandle, "WSPStartup")) == NULL
		)
		return WSAEPROVIDERFAILEDINIT;

	PrintProtocolInfo(lpProtocolInfo, sLibraryPath);

	if ((ErrorCode = WSPStartupFunc(wVersionRequested, lpWSPData
		, lpProtocolInfo, upcallTable, lpProcTable)) != ERROR_SUCCESS)
		return ErrorCode;
	
	EnterCriticalSection(&gCriticalSection);

	NextProcTable = *lpProcTable;

	lpProcTable->lpWSPSocket				= WSPSocket;
	lpProcTable->lpWSPCloseSocket			= WSPCloseSocket;
	lpProcTable->lpWSPConnect				= WSPConnect;
	lpProcTable->lpWSPAccept				= WSPAccept;
	lpProcTable->lpWSPSend					= WSPSend;
	lpProcTable->lpWSPSendTo				= WSPSendTo;
	lpProcTable->lpWSPRecv					= WSPRecv;
	lpProcTable->lpWSPRecvFrom				= WSPRecvFrom;

	lpProcTable->lpWSPAddressToString		= WSPAddressToString;
	lpProcTable->lpWSPAsyncSelect 			= WSPAsyncSelect;
	lpProcTable->lpWSPBind 					= WSPBind;
	lpProcTable->lpWSPCancelBlockingCall 	= WSPCancelBlockingCall;
	lpProcTable->lpWSPCleanup 				= WSPCleanup;
	lpProcTable->lpWSPDuplicateSocket  		= WSPDuplicateSocket;
	lpProcTable->lpWSPEnumNetworkEvents 	= WSPEnumNetworkEvents;
	lpProcTable->lpWSPEventSelect 			= WSPEventSelect;
	lpProcTable->lpWSPGetOverlappedResult 	= WSPGetOverlappedResult;
	lpProcTable->lpWSPGetPeerName 			= WSPGetPeerName;
	lpProcTable->lpWSPGetQOSByName 			= WSPGetQOSByName;
	lpProcTable->lpWSPGetSockName 			= WSPGetSockName;
	lpProcTable->lpWSPGetSockOpt 			= WSPGetSockOpt;
	lpProcTable->lpWSPIoctl 				= WSPIoctl;
	lpProcTable->lpWSPJoinLeaf 				= WSPJoinLeaf;
	lpProcTable->lpWSPListen 				= WSPListen;
	lpProcTable->lpWSPRecvDisconnect 		= WSPRecvDisconnect;
	lpProcTable->lpWSPSelect 				= WSPSelect;
	lpProcTable->lpWSPSendDisconnect 		= WSPSendDisconnect;
	lpProcTable->lpWSPSetSockOpt 			= WSPSetSockOpt;
	lpProcTable->lpWSPShutdown 				= WSPShutdown;
	lpProcTable->lpWSPStringToAddress 		= WSPStringToAddress;

	LeaveCriticalSection(&gCriticalSection);

	return 0;
}
