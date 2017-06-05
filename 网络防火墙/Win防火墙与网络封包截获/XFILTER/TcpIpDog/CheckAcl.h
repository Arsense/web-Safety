//=============================================================================================
/*
	CheckAcl.h

	Project	: XFILTER 1.0 Personal Firewall
	Author	: Tony Zhu
	Create Date	: 2001/08/21
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/
//=============================================================================================

#ifndef CHECKACL_H
#define CHECKACL_H

#include "ProtocolInfo.h"

class CCheckAcl
{
public:
	CCheckAcl();
	virtual	~CCheckAcl();

private:
	static	BOOL IsLocalIP	(DWORD *ip);
	int		PreGetAccessInfo(SOCKET s);
	int		GetAccessInfo	(SESSION *session);
	int		GetAccessFromWorkMode();
	int		GetAccessFromAcl(SESSION *mSession);

	DWORD	FindAcl			(CString sApplication, DWORD iStart);
	int		FindTime		(CTime time);
	int		FindIP			(DWORD IP);
	int		FinallySession	();

	BOOL	InitializeSession(SESSION* session);
	int		CreateSession	(SOCKET s, int nProtocol);
	int		DeleteSession	(SOCKET s);
	int		FindSession		(SOCKET s);
	int		SetSession		(SESSION *session, BYTE bDirection, UINT uiPort, DWORD ulRemoteIP);
	int		SetSessionEx	(SESSION *session, BYTE bDirection, const TCHAR *pMemo, int ByteCount, BOOL isSend);

private:
	SESSION				*m_Session;
	int					m_SessionCount;
	CProtocolInfo		m_ProtocolInfo;
	CRITICAL_SECTION	m_csSession;

public:
	BOOL	SetWindowsVersion();
	int		CheckStartup	();
	void	CheckSocket		(SOCKET s, int af, int type, int protocol);
	void	CheckCloseSocket(SOCKET s);
	int		CheckConnect	(SOCKET s, const struct sockaddr FAR *name, int namelen);
	int		CheckAccept		(SOCKET s, SOCKET news);
	int		CheckSend		(SOCKET s, TCHAR *buf, int len, LPDWORD lpNumberOfBytesSent);
	int		CheckSendTo		(SOCKET s, const SOCKADDR *pTo, TCHAR *buf, int len, LPDWORD lpNumberOfBytesSent);
	int		CheckRecv		(SOCKET s, TCHAR *buf, int len, LPDWORD lpNumberOfBytesRecvd);
	int		CheckRecvFrom	(SOCKET s, SOCKADDR *pFrom, TCHAR *buf, int len, LPDWORD lpNumberOfBytesRecvd);

	int		SetWorkMode			(int iWorkMode);
	int		SetAcl				(XACL_FILE AclFile);
	BOOL	SetAclToChangedMode	(BOOL IsChange);
	int		SendSessionToApp	(SESSION *session);
	int		GetWorkMode			();
	int		SetGuiWnd			(HWND hwnd);
	HWND	GetGuiWnd			();
	int		SetGuiProcessName	(const TCHAR *sPathName);
	BOOL	IsWin9x				();
	int		GetSessionAndSetSessionNull(SESSION *session, int iIndex);

};

#endif