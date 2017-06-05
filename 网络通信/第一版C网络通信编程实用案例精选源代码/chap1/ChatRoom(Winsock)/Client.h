// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__EB7CB9C2_7F60_48B7_BCEB_4884DB3243B9__INCLUDED_)
#define AFX_CLIENT_H__EB7CB9C2_7F60_48B7_BCEB_4884DB3243B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winsock.h"

class CExample1Dlg;

class CClient  
{
public:
	CClient();
	virtual ~CClient();
private:
	void ClientInit();
public:
	BOOL InitAndConnet(HWND hwnd,UINT port,CString m_strServer);
public:
	void GetString(CString & str);
	void SendString(CString a);
	CString m_strServer;
	SOCKET m_hSocket;
	sockaddr_in m_addr;
	UINT m_uPort;
    HWND m_hWnd;
};

#endif // !defined(AFX_CLIENT_H__EB7CB9C2_7F60_48B7_BCEB_4884DB3243B9__INCLUDED_)
