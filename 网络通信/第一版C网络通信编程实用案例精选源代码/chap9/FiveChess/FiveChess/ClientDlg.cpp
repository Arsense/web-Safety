// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiveChess.h"
#include "ClientDlg.h"
#include "FiveChessView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog


CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	m_iPort = 5000;
	
	//}}AFX_DATA_INIT
}


void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_address);
	DDX_Text(pDX, IDC_SERVERPORT, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 1024, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_BN_CLICKED(IDC_BUTTON, OnButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

void CClientDlg::OnButton() 
{
	// TODO: Add your control notification handler code here
	m_iPort=GetDlgItemInt(IDC_SERVERPORT);
	CIPAddressCtrl *ip=(CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS));
	ip->GetAddress(m_bIP[0],m_bIP[1],m_bIP[2],m_bIP[3]);

	this->EndDialog(2000);
}

BOOL CClientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i=0;i<4;i++)
	{
		m_bIP[i]=((CFiveChessView *)((CMainFrame*)::AfxGetMainWnd())->GetActiveView())->m_bIP[i];
	}
	CIPAddressCtrl *ip=(CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS));
	ip->SetAddress(m_bIP[0],m_bIP[1],m_bIP[2],m_bIP[3]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
