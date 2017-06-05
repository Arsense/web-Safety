// MessSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NeighborInfo.h"
#include "MessSocket.h"
#include "NeighborInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessSocket

CMessSocket::CMessSocket()
{
}

CMessSocket::~CMessSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMessSocket, CSocket)
	//{{AFX_MSG_MAP(CMessSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMessSocket member functions

void CMessSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_dlg->ReceiveMessage();	
	CSocket::OnReceive(nErrorCode);
}

void CMessSocket::Init(CNeighborInfoDlg *dlg)
{
	m_dlg=dlg;
}
