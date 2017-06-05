// DirectDriver.cpp : implementation file
//

#include "stdafx.h"
#include "Neighbor.h"
#include "DirectDriver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectDriver dialog


CDirectDriver::CDirectDriver(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectDriver::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirectDriver)
	m_localname = _T("");
	m_remotename = _T("");
	m_username = _T("anonymouse");
	m_password = _T("anonymouse");
	//}}AFX_DATA_INIT
}


void CDirectDriver::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectDriver)
	DDX_Text(pDX, IDC_LOCALNAME, m_localname);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_REMOTENAME, m_remotename);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirectDriver, CDialog)
	//{{AFX_MSG_MAP(CDirectDriver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectDriver message handlers

void CDirectDriver::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
