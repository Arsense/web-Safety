// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FiveChess.h"
#include "ServerDlg.h"
#include "FiveChessView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_iPort = 5000;
	m_ip = _T("");
	m_ip=((CFiveChessView *)((CMainFrame*)::AfxGetMainWnd())->GetActiveView())->m_ip;

	//}}AFX_DATA_INIT
}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Text(pDX, IDC_PORT, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 1024, 100000);
	DDX_Text(pDX, IDC_IP, m_ip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

void CServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_iPort = GetDlgItemInt(IDC_PORT);
	CDialog::OnOK();
}
