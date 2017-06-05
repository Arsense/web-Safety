// Wait.cpp : implementation file
//

#include "stdafx.h"
#include "SiteSnag.h"
#include "Wait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSnagWait dialog


CSnagWait::CSnagWait(LPCTSTR lpszMessage, CWnd* pParent /*=NULL*/)
	: CDialog()
{
	//{{AFX_DATA_INIT(CSnagWait)
	m_Message = lpszMessage;
	//}}AFX_DATA_INIT

	Create(CSnagWait::IDD, pParent);
}


void CSnagWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSnagWait)
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSnagWait, CDialog)
	//{{AFX_MSG_MAP(CSnagWait)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnagWait message handlers

BOOL CSnagWait::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

