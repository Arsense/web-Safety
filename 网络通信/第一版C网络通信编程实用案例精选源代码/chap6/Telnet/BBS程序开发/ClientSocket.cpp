// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "CTelnet.h"
#include "ClientSocket.h"

#include "CTelnetView.h"
#include "CTelnetDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket(CTelnetView * pView)
{
	cView = pView;
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnClose(int nErrorCode) 
{
	CAsyncSocket::OnClose(nErrorCode);
	if(!IsWindow(cView->m_hWnd)) return;
	if(!IsWindowVisible(cView->m_hWnd)) return;
	cView->GetDocument()->OnCloseDocument();

}



//接收到数据
void CClientSocket::OnReceive(int nErrorCode) 
{
	cView->ProcessMessage(this);
}

