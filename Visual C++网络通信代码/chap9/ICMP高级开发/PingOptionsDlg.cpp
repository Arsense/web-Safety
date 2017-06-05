// PingOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EWDPing.h"
#include "PingOptionsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPingOptionsDlg dialog


CPingOptionsDlg::CPingOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPingOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPingOptionsDlg)
	m_BufferSize = 0;
	m_DefaultHost = _T("");
	m_PingTimeout = 0;
	m_MaxHops = 0;
	//}}AFX_DATA_INIT
}


void CPingOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPingOptionsDlg)
	DDX_Text(pDX, IDC_BufferSize, m_BufferSize);
	DDX_Text(pDX, IDC_DefaultHost, m_DefaultHost);
	DDX_Text(pDX, IDC_PingTimeout, m_PingTimeout);
	DDX_Text(pDX, IDC_MaxHops, m_MaxHops);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPingOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CPingOptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingOptionsDlg message handlers

void CPingOptionsDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

