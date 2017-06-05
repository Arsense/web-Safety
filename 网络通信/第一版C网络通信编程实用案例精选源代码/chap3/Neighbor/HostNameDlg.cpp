// HostNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Neighbor.h"
#include "HostNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostNameDlg dialog


CHostNameDlg::CHostNameDlg(BOOL tag,CWnd* pParent /*=NULL*/)
	: CDialog(CHostNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHostNameDlg)
	m_bHost=tag;
	m_sHostName = _T("");
	//}}AFX_DATA_INIT
}


void CHostNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHostNameDlg)
	DDX_Text(pDX, IDC_EDIT1, m_sHostName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHostNameDlg, CDialog)
	//{{AFX_MSG_MAP(CHostNameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHostNameDlg message handlers

void CHostNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}

BOOL CHostNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(!m_bHost)
	{
		SetWindowText(" ‰»ÎIPµÿ÷∑");
		SetDlgItemText(IDC_STATICEDIT,"IPµÿ÷∑");
	}
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
