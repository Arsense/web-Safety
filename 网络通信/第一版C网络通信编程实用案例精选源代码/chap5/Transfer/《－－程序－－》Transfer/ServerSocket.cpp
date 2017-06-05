// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Transfer.h"
#include "ServerSocket.h"
#include "TransferDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
	m_dlg=NULL;
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

void CServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(m_dlg);
	m_dlg->HandleAccept();
	CSocket::OnAccept(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(m_dlg);
	m_dlg->HandleClose();
	CSocket::OnClose(nErrorCode);
}

void CServerSocket::Init(CTransferDlg *dlg)
{
	ASSERT(dlg);
	m_dlg=dlg;
}
