// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "ServerSocket.h"
#include "FiveChess.h"
#include "FiveChessView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

BOOL CServerSocket::Init(UINT port, CFiveChessView* view)
{
	m_uPort=port;
	m_view=view;
	if(Create(m_uPort)==FALSE)
	{
		AfxMessageBox("Server Socket Create Error");
		return FALSE;
	}
	if(this->Listen()==FALSE)
	{
		AfxMessageBox("Server Listen Error");
		return FALSE;
	}

	return TRUE;
}

void CServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnAccept(nErrorCode);
	m_view->ProcessPendingAccept();
}
