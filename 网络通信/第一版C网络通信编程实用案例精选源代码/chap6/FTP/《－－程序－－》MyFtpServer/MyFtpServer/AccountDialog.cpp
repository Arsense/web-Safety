// AccountDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyFtpServer.h"
#include "AccountDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountDialog dialog


CAccountDialog::CAccountDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountDialog)
	m_directory = _T("");
	m_password = _T("anonymous");
	m_user = _T("anonymous");
	//}}AFX_DATA_INIT
}


void CAccountDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountDialog)
	DDX_Text(pDX, IDC_DIRECTORY, m_directory);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_USER, m_user);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountDialog, CDialog)
	//{{AFX_MSG_MAP(CAccountDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountDialog message handlers

void CAccountDialog::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
