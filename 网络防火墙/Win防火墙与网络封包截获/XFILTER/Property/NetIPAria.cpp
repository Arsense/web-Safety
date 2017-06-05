// NetIPAria.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "NetIPAria.h"

//owner add
#include "GuiRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CNetIPAria dialog

CNetIPAria::CNetIPAria(CWnd* pParent /*=NULL*/)
	: CDialog(CNetIPAria::IDD, pParent)
{
	iState		= 3;
	ulStartIP	= 0;
	ulEndIP		= 0;
}


void CNetIPAria::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetIPAria)
	DDX_Control(pDX, IDC_NET_IP_ARIA_IP_END,	m_IPEnd);
	DDX_Control(pDX, IDC_NET_IP_ARIA_IP_START,	m_IPStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetIPAria, CDialog)
	//{{AFX_MSG_MAP(CNetIPAria)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CNetIPAria message handlers

BOOL CNetIPAria::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	InitDlgResource();

	return TRUE; 
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CNetIPAria::InitDlgResource()
{
	SetWindowText (GUI_NET_IP_ARIA_CAPTION);

	SetDlgItemText(IDC_NET_IP_ARIA_LABLE,			GUI_NET_IP_ARIA_LABLE);
	SetDlgItemText(IDC_NET_IP_ARIA_LABLE_START_IP,	GUI_NET_IP_ARIA_LABLE_START_IP);
	SetDlgItemText(IDC_NET_IP_ARIA_LABLE_END_IP,	GUI_NET_IP_ARIA_LABLE_END_IP);
	SetDlgItemText(IDOK,							GUI_BUTTON_OK);
	SetDlgItemText(IDCANCEL,						GUI_BUTTON_CANCEL_EX);

	if(iState == 1)//edit
	{
		m_IPStart.SetAddress(ulStartIP);
		m_IPEnd.SetAddress(ulEndIP);
	}
}

void CNetIPAria::OnOK() 
{
	DWORD	tmpStartIP = 0 , tmpEndIP = 0;

	m_IPStart	.GetAddress(tmpStartIP);
	m_IPEnd		.GetAddress(tmpEndIP);

	if(tmpStartIP == ulStartIP && tmpEndIP == ulEndIP)
	{
		CDialog::OnCancel();
		return;
	}

	if(tmpStartIP > tmpEndIP)
	{
		AfxMessageBox(GUI_NET_IP_ARIA_MESSAGE_INVALID_IP_ARIA);
		m_IPStart.SetFocus();
		return;
	}

	ulStartIP	= tmpStartIP;
	ulEndIP		= tmpEndIP;	

	CDialog::OnOK();

	return;
}
