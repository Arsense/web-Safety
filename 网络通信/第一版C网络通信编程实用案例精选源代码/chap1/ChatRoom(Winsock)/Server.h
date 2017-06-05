// Server.h: interface for the CServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVER_H__28D5187F_175B_45F0_A2C3_1DA78D6B4E2A__INCLUDED_)
#define AFX_SERVER_H__28D5187F_175B_45F0_A2C3_1DA78D6B4E2A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock.h"

class CExample1Dlg;

class CServer  
{
public:
	CServer();
	virtual ~CServer();
private:
	void ServerInit();
public:
	BOOL InitAndListen(HWND hwnd,UINT port);
public:
	SOCKET m_hSocket;
	sockaddr_in m_addr;
	UINT m_uPort;
    HWND m_hWnd;
};

#endif // !defined(AFX_SERVER_H__28D5187F_175B_45F0_A2C3_1DA78D6B4E2A__INCLUDED_)
