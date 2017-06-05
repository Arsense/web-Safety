// SystemSet.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "SystemSet.h"

//owner add
#include "GuiRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//=============================================================================================
// CSystemSet property page

extern CAclFile		m_AclFile;
extern CPropertyApp theApp;

IMPLEMENT_DYNCREATE(CSystemSet, CPropertyPage)

CSystemSet::CSystemSet() : CPropertyPage(CSystemSet::IDD)
{
	//{{AFX_DATA_INIT(CSystemSet)
	//}}AFX_DATA_INIT
}

CSystemSet::~CSystemSet()
{
}

void CSystemSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemSet)
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_SPLASH, m_CheckSplash);
	DDX_Control(pDX, IDC_SYSTEM_SET_LIST_LOG_SIZE, m_ListLogSize);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_LOG, m_CheckLog);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_ALERT_PCSPEAKER, m_CheckAlertSpeaker);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_AUTOSTART, m_CheckAutoStart);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_ALERT_DIALOG, m_CheckAlertDialog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemSet, CPropertyPage)
	//{{AFX_MSG_MAP(CSystemSet)
	ON_BN_CLICKED(IDC_SYSTEM_SET_CHECK_LOG, OnSystemSetCheckLog)
	ON_BN_CLICKED(IDC_SYSTEM_SET_CHECK_AUTOSTART, OnSystemSetCheckAutostart)
	ON_BN_CLICKED(IDC_SYSTEM_SET_CHECK_ALERT_PCSPEAKER, OnSystemSetCheckAlertPcspeaker)
	ON_BN_CLICKED(IDC_SYSTEM_SET_CHECK_ALERT_DIALOG, OnSystemSetCheckAlertDialog)
	ON_LBN_SELCHANGE(IDC_SYSTEM_SET_LIST_LOG_SIZE, OnSelchangeSystemSetListLogSize)
	ON_BN_CLICKED(IDC_SYSTEM_SET_CHECK_SPLASH, OnSystemSetCheckSplash)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemSet message handlers

BOOL CSystemSet::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitDlgResource();
	
	return TRUE;  
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CSystemSet::InitDlgResource()
{
	SetDlgItemText(IDC_SYSTEM_SET_COMMON_SET_LABLE		,GUI_SYSTEM_SET_COMMON_SET_LABLE);
	SetDlgItemText(IDC_SYSTEM_SET_CHECK_LOG				,GUI_SYSTEM_SET_CHECK_LOG);
	SetDlgItemText(IDC_SYSTEM_SET_UNIT_LABLE			,GUI_SYSTEM_SET_UNIT_LABLE);
	SetDlgItemText(IDC_SYSTEM_SET_CHECK_AUTOSTART		,GUI_SYSTEM_SET_CHECK_AUTOSTART);
	SetDlgItemText(IDC_SYSTEM_SET_CHECK_SPLASH			,GUI_SYSTEM_SET_CHECK_SPLASH);
	SetDlgItemText(IDC_SYSTEM_SET_ALERT_SET_LABLE		,GUI_SYSTEM_SET_ALERT_SET_LABLE);
	SetDlgItemText(IDC_SYSTEM_SET_CHECK_ALERT_PCSPEAKER	,GUI_SYSTEM_SET_CHECK_ALERT_PCSPEAKER);
	SetDlgItemText(IDC_SYSTEM_SET_CHECK_ALERT_DIALOG	,GUI_SYSTEM_SET_CHECK_ALERT_DIALOG);

	TCHAR buf[10];
	for(int i = 1; i <= 10; i++)
		m_ListLogSize.InsertString(i-1,_itot(i,buf,10));

	Refresh();
}

void CSystemSet::Refresh()
{
	m_ListLogSize.SetCurSel(m_AclFile.mAclHeader.uiLogSize - 1);
	m_CheckLog			.SetCheck(m_AclFile.GetBit(m_AclFile.mAclHeader.bSet,0));
	m_ListLogSize		.EnableWindow(m_CheckLog.GetCheck());
	m_CheckAutoStart	.SetCheck(m_AclFile.GetBit(m_AclFile.mAclHeader.bSet,1));
	m_CheckSplash		.SetCheck(m_AclFile.GetBit(m_AclFile.mAclHeader.bSet,6));
	m_CheckAlertSpeaker	.SetCheck(m_AclFile.GetBit(m_AclFile.mAclHeader.bSet,2));
	m_CheckAlertDialog	.SetCheck(m_AclFile.GetBit(m_AclFile.mAclHeader.bSet,3));
}

void CSystemSet::OnSystemSetCheckLog() 
{
	SetCheckValue(0, m_CheckLog			.GetCheck());
	m_ListLogSize.EnableWindow(m_CheckLog.GetCheck());
}

void CSystemSet::OnSystemSetCheckAutostart() 
{
	SetCheckValue(1, m_CheckAutoStart.GetCheck());
}

void CSystemSet::OnSystemSetCheckSplash() 
{
	SetCheckValue(6, m_CheckSplash.GetCheck());
}

void CSystemSet::OnSystemSetCheckAlertPcspeaker() 
{
	SetCheckValue(2, m_CheckAlertSpeaker.GetCheck());
}

void CSystemSet::OnSystemSetCheckAlertDialog() 
{
	SetCheckValue(3, m_CheckAlertDialog	.GetCheck());
}

void CSystemSet::OnSelchangeSystemSetListLogSize() 
{
	m_AclFile.mAclHeader.uiLogSize = m_ListLogSize.GetCurSel() + 1;
	GetOwner()->GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
}

void CSystemSet::SetCheckValue(int iIndex, BOOL bValue)
{
	m_AclFile.SetBit(&m_AclFile.mAclHeader.bSet, iIndex, bValue);
	GetOwner()->GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
}

BOOL CSystemSet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.WinHelp(7, HH_HELP_CONTEXT);

	return TRUE;
}
