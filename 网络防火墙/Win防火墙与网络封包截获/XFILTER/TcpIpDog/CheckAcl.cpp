//=============================================================================================
/*
	文件：		CheckAcl.cpp

	说明:
	---------------------------------------------------
		控管规则访问控制，通过控管规则的检查认证，确定
	Socket 连接是否允许通过。
	---------------------------------------------------

	工程：		Xfilter 个人防火墙
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/21
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	---------------------------------------------------
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
	---------------------------------------------------	
*/
//=============================================================================================

#include "stdafx.h"
#include "CheckAcl.h"
#include "TcpIpDog.h"

//=============================================================================================
// share data

#pragma data_seg(".inidata")
	int			m_iWorkMode		= XF_PASS_ALL;
	BOOL		m_bAclIsChange	= FALSE;
	HWND		m_GuiHwnd		= NULL;
	BOOL		m_bIsWin9x		= FALSE;
#pragma data_seg()

#pragma bss_seg(".uinidata")
	XACL_FILE	m_AclFile;
	SESSION		m_SessionBuf[MAX_SESSION_BUFFER];
	TCHAR		m_sGuiPathName[MAX_PATH];
	TCHAR		m_sSystemPath[MAX_PATH];
	TCHAR		m_sWin9xSys1[MAX_PATH];
	TCHAR		m_sWin2kSys1[MAX_PATH];
	TCHAR		m_sWin2kSys2[MAX_PATH];	// v1.0.2 add 2001-12-22
#pragma bss_seg()

CRITICAL_SECTION	m_csWorkMode;
CRITICAL_SECTION	m_csAclChangeMode;
CRITICAL_SECTION	m_csSetProcessName;
CRITICAL_SECTION	m_csSystemVersion;
CRITICAL_SECTION	m_csSetAcl;

//=============================================================================================
// extern globals variable
extern TCHAR	m_sProcessName[MAX_PATH];

//=============================================================================================
// initialize class function and pre-destroy class function.

CCheckAcl::CCheckAcl()
{
	InitializeCriticalSection(&m_csWorkMode);
	InitializeCriticalSection(&m_csSetAcl);
	InitializeCriticalSection(&m_csAclChangeMode);
	InitializeCriticalSection(&m_csSetProcessName);
	InitializeCriticalSection(&m_csSystemVersion);

	InitializeCriticalSection(&m_csSession);

	m_SessionCount = 0;
}

CCheckAcl::~CCheckAcl()
{
	FinallySession();
	delete[](m_Session);
}

BOOL CCheckAcl::SetWindowsVersion()
{
	EnterCriticalSection(&m_csSystemVersion);
	{
		OSVERSIONINFO VerInfo;  
		VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&VerInfo);

		if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			m_bIsWin9x = TRUE;
		}
		else if(VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
			(VerInfo.dwMajorVersion == 4 || VerInfo.dwMajorVersion == 5))
		{
			m_bIsWin9x = FALSE;
		}

		GetSystemDirectory(m_sSystemPath, MAX_PATH);

		_tcscpy(m_sWin9xSys1, m_sSystemPath);
		_tcscat(m_sWin9xSys1, _T("\\icsmgr.exe"));
		_tcscpy(m_sWin2kSys1, m_sSystemPath);
		_tcscat(m_sWin2kSys1, _T("\\services.exe"));

		// v1.0.2 add 2001-12-22
		_tcscpy(m_sWin2kSys2, m_sSystemPath);
		_tcscat(m_sWin2kSys2, _T("\\INETSRV\\INETINFO.EXE"));
	}
	LeaveCriticalSection(&m_csSystemVersion);
	
	return TRUE; 
}

//=============================================================================================
// Static function, XFILTER.EXE to change the work mode and ACL information, 
// it's operate by XfIoControl.

int CCheckAcl::IsWin9x()
{
	return m_bIsWin9x;
}

int CCheckAcl::SetGuiProcessName(const TCHAR *sPathName)
{
	EnterCriticalSection(&m_csSetProcessName);
	{
		_tcscpy(m_sGuiPathName, sPathName);
	}
	LeaveCriticalSection(&m_csSetProcessName);
	return XERR_SUCCESS;
}

int CCheckAcl::SetGuiWnd(HWND hwnd)
{
	EnterCriticalSection(&m_csWorkMode);
	{
		m_GuiHwnd = hwnd;
	}
	LeaveCriticalSection(&m_csWorkMode);
	return XERR_SUCCESS;
}

HWND CCheckAcl::GetGuiWnd()
{
	return m_GuiHwnd;
}

int CCheckAcl::SetWorkMode(int iWorkMode)
{
	EnterCriticalSection(&m_csWorkMode);
	{
		m_iWorkMode = iWorkMode;
	}
	LeaveCriticalSection(&m_csWorkMode);

	return XERR_SUCCESS;
}

int CCheckAcl::GetWorkMode()
{
	return m_iWorkMode;
}

int CCheckAcl::SetAcl(XACL_FILE AclFile)
{
	EnterCriticalSection(&m_csSetAcl);
	{
		m_bAclIsChange = TRUE;

		SetWorkMode(CXCommon::GetBit(AclFile.mAclHeader.bSet, 4, 2));
		m_AclFile	= AclFile;

		m_bAclIsChange = FALSE;
	}
	LeaveCriticalSection(&m_csSetAcl);
	return XERR_SUCCESS;
}

BOOL CCheckAcl::SetAclToChangedMode(BOOL IsChange)
{
	EnterCriticalSection(&m_csAclChangeMode);
	{
		m_bAclIsChange = IsChange;
	}
	LeaveCriticalSection(&m_csAclChangeMode);
	
	return TRUE;
}

//=============================================================================================
// Check rule operation. it's return access value with the ACL.

BOOL CCheckAcl::IsLocalIP(DWORD *ip)
{
	BYTE IsLocalIP[4];
	memcpy(IsLocalIP, ip, sizeof(DWORD));

	if(*ip == 0 || IsLocalIP[3] == 127)
		return TRUE;

	ODS(m_sWin9xSys1);
	ODS(m_sWin2kSys1);

	static CString Win9xSys1 = m_sWin9xSys1;
	static CString Win2kSys1 = m_sWin2kSys1;
	static CString Win2kSys2 = m_sWin2kSys2; // v1.0.2 add 2001-12-22

	if(m_bIsWin9x)
	{
		if(Win9xSys1.CompareNoCase(m_sProcessName) == 0)
			return TRUE;
	}
	else
	{
		if(Win2kSys1.CompareNoCase(m_sProcessName) == 0
			|| Win2kSys2.CompareNoCase(m_sProcessName) == 0) // v1.0.2 add 2001-12-22
		{
			ODS(_T("Is Win2000 System Process ..."));
			return TRUE;
		}
	}

	ODS(_T("Not Is Win2000 System Process ..."));
	return FALSE;
}

int CCheckAcl::GetAccessInfo(SESSION *session)
{
	int		iRet;

	iRet = GetAccessFromAcl(session);

	if(iRet != XF_PASS)
		session->bAction = ACL_ACTION_DENY;
	else
		session->bAction = ACL_ACTION_PASS;

	return iRet;
}

int CCheckAcl::GetAccessFromWorkMode()
{
	if(m_bAclIsChange)
		return XF_UNKNOWN;

	if(m_iWorkMode == XF_PASS_ALL)
		return XF_PASS;

	if(m_iWorkMode == XF_DENY_ALL)
		return XF_DENY;

	if(m_iWorkMode != XF_QUERY_ALL)
		return XF_UNKNOWN;

	return XF_FILTER;
}

int CCheckAcl::GetAccessFromAcl(SESSION *mSession)
{
	if(m_AclFile.mAclHeader.sSignature[0] == 0
		|| _tcscmp(m_sGuiPathName, m_sProcessName) == 0
		|| IsLocalIP(&mSession->ulRemoteIP)
		)
		return XF_PASS;

	int		iRet;

	if((iRet = GetAccessFromWorkMode()) != XF_FILTER)
		return iRet;

	BOOL	IsOne	= TRUE;
	DWORD	iIndex	= 0;
	BYTE	bAction = ACL_ACTION_PASS;
	
COMPARE:

	if(!IsOne) iIndex ++;

	iIndex = FindAcl(m_sProcessName, iIndex);

	if(iIndex >= m_AclFile.mAclHeader.ulAclCount)
	{
		if(IsOne)
		{
			if(m_bIsWin9x)
			{
				if(!QueryAccess())
					return XF_DENY;
				else
					return XF_PASS;
			}
			else 
			{
				//
				// 2001-12-25 modify
				// return XF_QUERY;
				// chage to
				//
				if(!QueryAccess())
					return XF_DENY;
				else
					return XF_PASS;
			}
		}
		else
		{
			if(bAction == ACL_ACTION_DENY)
				return XF_PASS;
			else
				return XF_DENY;
		}
	}

	if(IsOne) IsOne = FALSE;
	bAction = m_AclFile.mpAcl[iIndex].bAction;

	if(m_AclFile.mpAcl[iIndex].bDirection != ACL_DIRECTION_IN_OUT
		&& mSession->bDirection	!= m_AclFile.mpAcl[iIndex].bDirection)
			goto COMPARE;

	if(m_AclFile.mpAcl[iIndex].bServiceType != ACL_SERVICE_TYPE_ALL
		&& mSession->bProtocol != m_AclFile.mpAcl[iIndex].bServiceType)
			goto COMPARE;

	if(m_AclFile.mpAcl[iIndex].bAccessTimeType != ACL_TIME_TYPE_ALL
		&& FindTime(mSession->tStartTime) != m_AclFile.mpAcl[iIndex].bAccessTimeType)
			goto COMPARE;

	if(m_AclFile.mpAcl[iIndex].bRemoteNetType != ACL_NET_TYPE_ALL
		&& FindIP(mSession->ulRemoteIP) != m_AclFile.mpAcl[iIndex].bRemoteNetType)
			goto COMPARE;

	if(m_AclFile.mpAcl[iIndex].uiServicePort != ACL_SERVICE_PORT_ALL
		&& mSession->uiPort != m_AclFile.mpAcl[iIndex].uiServicePort)
			goto COMPARE;
	
	if(m_AclFile.mpAcl[iIndex].bAction == ACL_ACTION_DENY)
		return XF_DENY;

	return XF_PASS;
}

DWORD CCheckAcl::FindAcl(CString sApplication, DWORD iStart)
{
	DWORD		iIndex = 0;

	for(iIndex = iStart; iIndex < m_AclFile.mAclHeader.ulAclCount; iIndex ++)
		if(sApplication.CompareNoCase(m_AclFile.mpAcl[iIndex].sApplication) == 0)
			break;

	return iIndex;
}

int CCheckAcl::FindTime(CTime time)
{
	for(DWORD i = 0; i < m_AclFile.mAclHeader.ulTimeCount; i ++)
	{
		if(CXCommon::GetBit(m_AclFile.mAclTime[i].bWeekDay,time.GetDayOfWeek() - 1) != 1)
			continue;

		if(m_AclFile.mAclTime[i].tStartTime == m_AclFile.mAclTime[i].tEndTime)
			return i + 1;

		CTime t = time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond();

		if(m_AclFile.mAclTime[i].tStartTime < m_AclFile.mAclTime[i].tEndTime)
		{
			if(t >= m_AclFile.mAclTime[i].tStartTime && t <= m_AclFile.mAclTime[i].tEndTime)
				return i + 1;
		}
		else
		{
			if(t >= m_AclFile.mAclTime[i].tStartTime || t <= m_AclFile.mAclTime[i].tEndTime)
				return i + 1;
		}
	}

	return ACL_TIME_TYPE_ALL;
}

int CCheckAcl::FindIP(DWORD IP)
{
	DWORD i = 0;

	if(IP >= m_AclFile.mAclIntranetIP.ulStartIP && IP <= m_AclFile.mAclIntranetIP.ulEndIP)
		return ACL_NET_TYPE_INTRANET;

	for(i = 0; i< m_AclFile.mAclHeader.ulDistrustIPCount; i++)
		if(IP >= m_AclFile.mpAclDistrustIP[i].ulStartIP && IP <= m_AclFile.mpAclDistrustIP[i].ulEndIP)
			return ACL_NET_TYPE_DISTRUST;

	for(i = 0; i< m_AclFile.mAclHeader.ulTrustIPCount; i++)
		if(IP >= m_AclFile.mpAclTrustIP[i].ulStartIP && IP <= m_AclFile.mpAclTrustIP[i].ulEndIP)
			return ACL_NET_TYPE_TRUST;

	for(i = 0; i< m_AclFile.mAclHeader.ulCustomIPCount; i++)
		if(IP >= m_AclFile.mpAclCustomIP[i].ulStartIP && IP <= m_AclFile.mpAclCustomIP[i].ulEndIP)
			return ACL_NET_TYPE_CUSTOM;

	return ACL_NET_TYPE_ALL;
}

//=============================================================================================
// session operation. session include the socket connection info.

int CCheckAcl::GetSessionAndSetSessionNull(SESSION *session, int iIndex)
{
	*session = m_SessionBuf[iIndex];
	m_SessionBuf[iIndex].s = 0;
	return XERR_SUCCESS;
}

BOOL CCheckAcl::InitializeSession(SESSION* session)
{
	ODS(_T("Initialize Session ..."));

	session->bDirection		= ACL_DIRECTION_IN_OUT;
	session->bProtocol 		= ACL_SERVICE_TYPE_ALL;
	session->bAction		= ACL_ACTION_PASS;
	session->tStartTime 	= 0;
	session->uiPort 		= 0;
	session->ulRemoteIP 	= 0;
	session->ulSendData		= 0;
	session->ulRecvData		= 0;
	session->sMemo[0]		= '\0';
	session->uiLocalPort	= 0;
	session->ulLocalIP		= 0;

	return TRUE;
}

int CCheckAcl::CreateSession(SOCKET s, int nProtocol)
{
	ODS("XFILTER.DLL: Create Session...");

	EnterCriticalSection(&m_csSession);
	{
		for(int i = 0; i < m_SessionCount; i++)
		{
			if(m_Session[i].s == s)
			{
				LeaveCriticalSection(&m_csSession);
				return XERR_SESSION_ALREDAY_EXISTS;
			}
		}

		SESSION *tmpSession = new SESSION[m_SessionCount];
		memcpy(tmpSession, m_Session, m_SessionCount * SESSION_LENTH);
		delete[](m_Session);

		m_Session = new SESSION[m_SessionCount + 1];
		memcpy(m_Session, tmpSession, m_SessionCount * SESSION_LENTH);
		delete[](tmpSession);

		InitializeSession(m_Session + m_SessionCount);
		m_Session[m_SessionCount].s			= s;
		m_Session[m_SessionCount].bProtocol = nProtocol;
		m_Session[m_SessionCount].tStartTime = CTime::GetCurrentTime();
		_tcscpy(m_Session[m_SessionCount].sPathName, m_sProcessName);

		m_SessionCount ++;
	}
	LeaveCriticalSection(&m_csSession);

	DP1("Session Count: %u\n", m_SessionCount);

	return m_SessionCount - 1;	// return m_SessionCount will have a bug, this value 
								// used in new session index. It must - 1 .
}

int CCheckAcl::DeleteSession(SOCKET s)
{
	ODS("XFILTER.DLL: Delete Session...");

	EnterCriticalSection(&m_csSession);
	{
		for(int i = 0; i < m_SessionCount; i++)
		{
			if(m_Session[i].s == s)
			{
				SendSessionToApp(m_Session + i);

				memcpy(m_Session + i, m_Session + i + 1, (m_SessionCount - i - 1) * SESSION_LENTH);

				break;
			}
		}

		if(i >= m_SessionCount)
		{
			LeaveCriticalSection(&m_csSession);
			return XERR_SESSION_NOT_EXISTS;
		}

		m_SessionCount -- ;
	}
	LeaveCriticalSection(&m_csSession);

	DP1("Session Count: %u\n", m_SessionCount);

	return XERR_SUCCESS;
}

int CCheckAcl::SetSession(SESSION *session, BYTE bDirection, UINT uiPort, DWORD ulRemoteIP)
{
	if(session->bProtocol == ACL_SERVICE_TYPE_TCP)
	{
		if(uiPort == ACL_SERVICE_PORT_FTP)
			session->bProtocol = ACL_SERVICE_TYPE_FTP;
		else if(uiPort == ACL_SERVICE_PORT_HTTP1 || uiPort == ACL_SERVICE_PORT_HTTP2)
			session->bProtocol = ACL_SERVICE_TYPE_HTTP;
		else if(uiPort == ACL_SERVICE_PORT_TELNET)
			session->bProtocol = ACL_SERVICE_TYPE_TELNET;
		else if(uiPort == ACL_SERVICE_PORT_NNTP)
			session->bProtocol = ACL_SERVICE_TYPE_NNTP;
		else if(uiPort == ACL_SERVICE_PORT_POP3)
			session->bProtocol = ACL_SERVICE_TYPE_POP3;
		else if(uiPort == ACL_SERVICE_PORT_SMTP)
			session->bProtocol = ACL_SERVICE_TYPE_SMTP;
	}

	session->bDirection		= bDirection;
	session->uiPort			= uiPort;
	session->ulRemoteIP		= htonl(ulRemoteIP);

#if _DEBUG
	CString sOut;
	sOut.Format(_T("SOCKET: %u, Process: %s, Direction: %s, Protocol: %s:%u, Start Time: %s, Port: %u, Remote IP Address: %s"), 
		session->s,
		session->sPathName,
		GUI_DIRECTION[session->bDirection],
		GUI_SERVICE_TYPE[session->bProtocol], session->bProtocol,
		session->tStartTime.Format(_T("%Y-%m-%d %H:%M:%S")),
		session->uiPort,
		CXCommon::DIPToSIP(&session->ulRemoteIP)
		);
	ODS(sOut);
#endif

	return XERR_SUCCESS;
}

int CCheckAcl::SetSessionEx(SESSION *session, BYTE bDirection, const TCHAR *pMemo, int ByteCount, BOOL isSend)
{
	if(session->ulLocalIP == 0)
	{
		SOCKADDR_IN			inetAddr;
		int					nNameLength	= sizeof(inetAddr);

		if(getsockname(session->s, (SOCKADDR*)&inetAddr, &nNameLength) == 0)
		{
			session->uiLocalPort = ntohs(inetAddr.sin_port);

			memcpy(&session->ulLocalIP, &inetAddr.sin_addr, 4);
			session->ulLocalIP	= htonl(session->ulLocalIP);
		}
	}

	if(bDirection != ACL_DIRECTION_NOT_SET && session->bDirection != bDirection)
		session->bDirection		= bDirection;

	if(pMemo != NULL && session->sMemo[0] == '\0' )
		_tcscpy(session->sMemo, pMemo);

	if(ByteCount > 0)
	{
		if(isSend)
			session->ulSendData += ByteCount;
		else
			session->ulRecvData += ByteCount;
	}

	return XERR_SUCCESS;
}

int CCheckAcl::FindSession(SOCKET s)
{
	int		i;

	for(i = 0; i < m_SessionCount; i++)
		if(m_Session[i].s == s)
			break;

	return i;
}

int CCheckAcl::FinallySession()
{
	ODS(_T("XFILTER.DLL: FinallySession ..."));

	for(int i = 0; i < m_SessionCount; i ++)
		SendSessionToApp(m_Session + i);

	return XERR_SUCCESS;
}

int CCheckAcl::SendSessionToApp(SESSION *session)
{
#if !defined(_DEBUG)

	if(IsLocalIP(&session->ulRemoteIP))
		return XERR_SUCCESS;

#endif

	if(m_GuiHwnd != NULL && m_AclFile.mAclHeader.sSignature[0] != 0)
	{
		int		iIndex;

		for(iIndex = 0; iIndex < MAX_SESSION_BUFFER; iIndex++)
		{
			if(m_SessionBuf[iIndex].s == 0)
			{
				session->tEndTime = CTime::GetCurrentTime();
				m_SessionBuf[iIndex] = *session;
				break;
			}
		}

		//
		// 2001-12-24 修改，增加对PostMessage的错误判断，如果发送失败
		// 直接设置缓冲区标志为无效，丢弃封包往应用程序的发送
		//
		if(iIndex < MAX_SESSION_BUFFER
			&& !::PostMessage(m_GuiHwnd, WM_SESSION_NOTIFY, iIndex, NULL))
			m_SessionBuf[iIndex].s = 0;
	}

#if _DEBUG
	
	CString  tmpStr;

	tmpStr.Format(
		_T("SOCKET: %u, Action: %s, Direction: %s, Protocol: %s, Start Time: %s, End Time: %s,\
		Local Port: %u, Remotoe Port: %u, Local IP Address: %s, Remote IP Address: %s,\
		In amount data: %u, Out amout Data: %u, \nMemo: %s, sProcess: %s"), 
		session->s,
		GUI_ACTION[session->bAction],
		GUI_DIRECTION[session->bDirection],
		GUI_SERVICE_TYPE[session->bProtocol],
		session->tStartTime.Format(_T("%Y-%m-%d %H:%M:%S")),
		CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")),
		session->uiLocalPort,
		session->uiPort,
		CXCommon::DIPToSIP(&session->ulLocalIP),
		CXCommon::DIPToSIP(&session->ulRemoteIP),
		session->ulSendData,
		session->ulRecvData,
		session->sMemo,
		session->sPathName
		);

	ODS(tmpStr);

#endif
	
	session->sMemo[0]		= '\0';
	session->ulRecvData		= 0;
	session->ulSendData		= 0;

	return XERR_SUCCESS;
}

//=============================================================================================
// check the hook function, set session value and return access info.

int CCheckAcl::CheckStartup()
{
	if(m_iWorkMode != XF_QUERY_ALL 
		|| _tcscmp(m_sGuiPathName, m_sProcessName) == 0
		|| m_AclFile.mAclHeader.sSignature[0] == 0
		)
		return XF_PASS;

	if(m_sProcessName[0] == 0)
		return XERR_ACCESS_INVALID_PROCESS;

 	DWORD iIndex = FindAcl(m_sProcessName, 0);

	if(iIndex >= m_AclFile.mAclHeader.ulAclCount)
		return XF_QUERY;

	return XF_PASS;
}

void CCheckAcl::CheckSocket(SOCKET s, int af, int type, int protocol)
{
	if (af != AF_INET)
		return;

	WORD			wProtocol	= ACL_SERVICE_TYPE_ALL;	

	if (protocol == IPPROTO_IP)
	{
		if (type == SOCK_STREAM)
			wProtocol = ACL_SERVICE_TYPE_TCP;
		else if (type == SOCK_DGRAM)
			wProtocol = ACL_SERVICE_TYPE_UDP;
	}
	else if (protocol == IPPROTO_TCP)
		wProtocol	= ACL_SERVICE_TYPE_TCP;
	else if (protocol == IPPROTO_UDP)
		wProtocol	= ACL_SERVICE_TYPE_UDP;

	CreateSession(s, wProtocol);
}

void CCheckAcl::CheckCloseSocket(SOCKET s)
{
	DeleteSession(s);
}

int CCheckAcl::CheckConnect(SOCKET s, const struct sockaddr FAR *name, int namelen)
{
	int		iIndex;

	if((iIndex = FindSession(s)) >= m_SessionCount)
		return XF_PASS;
	
	SOCKADDR_IN	*pInetAddr	= (SOCKADDR_IN*)name;
	WORD		wPort		= ntohs(pInetAddr->sin_port);
	DWORD		*pRemoteIp	= (DWORD*)&pInetAddr->sin_addr;

	SetSession(m_Session + iIndex, ACL_DIRECTION_OUT, wPort, *pRemoteIp);

	return GetAccessInfo(m_Session + iIndex);
}

int  CCheckAcl::CheckAccept(SOCKET s, SOCKET news)
{
	int		iIndex;

	if((iIndex = FindSession(s)) >= m_SessionCount)
		return XF_PASS;

	if((iIndex = CreateSession(news, ACL_SERVICE_TYPE_TCP)) == XERR_SESSION_ALREDAY_EXISTS)
		return XF_PASS;
	
	SOCKADDR_IN		addr;
	int				addrlen	= sizeof(addr);
	DWORD			ulRemoteIp;

	getpeername(news, (SOCKADDR*)&addr, &addrlen);
	memcpy(&ulRemoteIp, &addr.sin_addr, 4);
	getsockname(news, (SOCKADDR*)&addr, &addrlen);

	if (SetSession(m_Session + iIndex, ACL_DIRECTION_IN, ntohs(addr.sin_port), ulRemoteIp) != XERR_SUCCESS)
		return XF_PASS;

	return GetAccessInfo(m_Session + iIndex);
}

int CCheckAcl::CheckSend(SOCKET s, TCHAR *buf, int len, LPDWORD lpNumberOfBytesSent)
{
	int		iIndex;

	if((iIndex = FindSession(s)) >= m_SessionCount)
		return XF_PASS;

	SetSessionEx(m_Session + iIndex, ACL_DIRECTION_NOT_SET, NULL, *lpNumberOfBytesSent, TRUE);

	m_ProtocolInfo.GetProtocolInfo(m_Session + iIndex, buf, len, TRUE);

	return GetAccessInfo(m_Session + iIndex);
}

int	CCheckAcl::CheckSendTo(SOCKET s, const SOCKADDR *pTo, TCHAR *buf, int len, LPDWORD lpNumberOfBytesSent)
{
	int		iIndex;

	if((iIndex = FindSession(s)) >= m_SessionCount)
		return XF_PASS;

	if (pTo != NULL && m_Session[iIndex].bProtocol == ACL_SERVICE_TYPE_UDP )
	{
		SOCKADDR_IN	*pAddr			= (SOCKADDR_IN*)pTo;
		DWORD		*pRemoteIp		= (DWORD*)&pAddr->sin_addr;

		SetSession(m_Session + iIndex, ACL_DIRECTION_OUT, ntohs(pAddr->sin_port), *pRemoteIp);
	}

	SetSessionEx(m_Session + iIndex, ACL_DIRECTION_NOT_SET, NULL, *lpNumberOfBytesSent, TRUE);

	m_ProtocolInfo.GetProtocolInfo(m_Session + iIndex, buf, len, TRUE);

	return GetAccessInfo(m_Session + iIndex);
}

int CCheckAcl::CheckRecv(SOCKET s, TCHAR *buf, int len, LPDWORD lpNumberOfBytesRecvd)
{
	int		iIndex;

	if((iIndex = FindSession(s)) >= m_SessionCount)
		return XF_PASS;

	SetSessionEx(m_Session + iIndex, ACL_DIRECTION_NOT_SET, NULL, *lpNumberOfBytesRecvd, FALSE);

	m_ProtocolInfo.GetProtocolInfo(m_Session + iIndex, buf, len, FALSE);

	return GetAccessInfo(m_Session + iIndex);
}

int CCheckAcl::CheckRecvFrom(SOCKET s, SOCKADDR *pFrom, TCHAR *buf, int len, LPDWORD lpNumberOfBytesRecvd)
{
	int		iIndex;

	if((iIndex = FindSession(s)) >= m_SessionCount)
		return XF_PASS;

	if (pFrom != NULL && m_Session[iIndex].bProtocol == ACL_SERVICE_TYPE_UDP)
	{
		SOCKADDR_IN			*pInetAddr	= (SOCKADDR_IN*)pFrom;
		DWORD				*pRemoteIp	= (DWORD*)&pInetAddr->sin_addr;

		SetSession(m_Session + iIndex, ACL_DIRECTION_IN, ntohs(pInetAddr->sin_port), *pRemoteIp); 
	}

	SetSessionEx(m_Session + iIndex, ACL_DIRECTION_NOT_SET, NULL, *lpNumberOfBytesRecvd, FALSE);

	m_ProtocolInfo.GetProtocolInfo(m_Session + iIndex, buf, len, FALSE);

	return GetAccessInfo(m_Session + iIndex);
}


