// ESocket.cpp : implementation file
//

#include "stdafx.h"
#include "SendEMail.h"
#include "ESocket.h"
#include "SendEMailDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESocket

CESocket::CESocket()
{
}

CESocket::~CESocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CESocket, CSocket)
	//{{AFX_MSG_MAP(CESocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CESocket member functions

void CESocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_dlg->ReceiveMessage(m_iCount);
	m_iCount++;
	CSocket::OnReceive(nErrorCode);
}

void CESocket::Init(CSendEMailDlg *dlg)
{
	m_dlg=dlg;
	m_iCount=0;
}
