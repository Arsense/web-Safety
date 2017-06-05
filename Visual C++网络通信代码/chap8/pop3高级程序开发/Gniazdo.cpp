// Gniazdo.cpp : implementation file
//

#include "stdafx.h"
#include "pop3Dlg.h"
#include "Gniazdo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBase4Pop3

CBase4Pop3::CBase4Pop3()
{
}

CBase4Pop3::~CBase4Pop3()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CBase4Pop3, CAsyncSocket)
	//{{AFX_MSG_MAP(CBase4Pop3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CBase4Pop3 member functions

void CBase4Pop3::Set(CDialog * pWnd)  //set the pointer
{
  m_pWnd=pWnd;
}

void CBase4Pop3::OnAccept(int err)		  
{															  
  if(err==0) //free of any error, dispatch message
	  ((DLG)m_pWnd)->Dispatch(S_ACCEPT);
}

void CBase4Pop3::OnConnect(int err)
{
  if(err==0)
	  ((DLG)m_pWnd)->Dispatch(S_CONNECT);
}

void CBase4Pop3::OnSend(int err)
{

}

void CBase4Pop3::OnClose(int err)
{

if(err==0)
  ((DLG)m_pWnd)->Dispatch(S_CLOSE);
}

void CBase4Pop3::OnReceive(int err)
{
  if(err==0)
	  ((DLG)m_pWnd)->Dispatch(S_RECEIVE);
}
