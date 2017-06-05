// Login.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogin)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogin)
	DDX_Control(pDX, IDC_IPADDRESS, m_ControlIP);
	DDX_Text(pDX, IDC_EDIT, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	//{{AFX_MSG_MAP(CLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

BOOL CLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);



	ULONG   uIp   =   inet_addr("127.0.0.1");
	m_ControlIP.SetAddress(htonl(uIp));  //IP 控件设置数据







	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogin::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);

	BYTE f1,f2,f3,f4;
	m_ControlIP.GetAddress(f1,f2,f3,f4);
	m_strIP.Format("%d.%d.%d.%d",f1,f2,f3,f4);
	

	CDialog::OnOK();
}
