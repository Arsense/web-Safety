// AclSet.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "AclSet.h"

//owner add
#include "GuiRes.h"
#include "NetTimeSheet.h"
#include "SetNet.h"
#include "SetTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CAclSet dialog

extern CAclFile		m_AclFile;
CNetTimeSheet		*dlgNetTimeSheet	= NULL;
CFileDialog			*dlgFile			= NULL;

XACL				CAclSet::m_tmpAcl;
BOOL				CAclSet::m_EnableComboApplication = TRUE;
CString				CAclSet::m_sPathName;

CAclSet::CAclSet(CWnd* pParent /*=NULL*/)
	: CDialog(CAclSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAclSet)
	//}}AFX_DATA_INIT
}


void CAclSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAclSet)
	DDX_Control(pDX, IDC_EDIT_MEMO,			m_EditMemo);
	DDX_Control(pDX, IDC_EDIT_SERVICE_PORT, m_EditServicePort);
	DDX_Control(pDX, IDC_COMBO_DIRECTION,	m_ComboDirection);
	DDX_Control(pDX, IDC_COMBO_ACCESS_TIME, m_ComboAccessTime);
	DDX_Control(pDX, IDC_COMBO_SERVICE_TYPE, m_ComboServiceType);
	DDX_Control(pDX, IDC_COMBO_ACTION,		m_ComboAction);
	DDX_Control(pDX, IDC_COMBO_REMOTE_NET,	m_ComboRemoteNet);
	DDX_Control(pDX, IDC_COMBO_APPLICATION, m_ComboApplication);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAclSet, CDialog)
	//{{AFX_MSG_MAP(CAclSet)
	ON_BN_CLICKED(IDC_BUTTON_NET,	OnButtonNet)
	ON_BN_CLICKED(IDC_BUTTON_TIME,	OnButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_APPLICATION,		OnButtonApplication)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVICE_TYPE,	OnSelchangeComboServiceType)
	ON_CBN_SELCHANGE(IDC_COMBO_APPLICATION, OnSelchangeComboApplication)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CAclSet message handlers

void CAclSet::OnButtonNet() 
{
	ShowNetTimeSheet(1);
}
void CAclSet::OnButtonTime() 
{
	ShowNetTimeSheet(2);
}

void CAclSet::ShowNetTimeSheet(int nID)
{
	int	tmpNetIndex = m_ComboRemoteNet.GetCurSel();
	int tmpTimeIndex= m_ComboAccessTime.GetCurSel();

	dlgNetTimeSheet = new CNetTimeSheet(GUI_NET_TIME_SHEET_CAPTION);
	CSetNet			mSetNet;
	CSetTime		mSetTime;

	dlgNetTimeSheet->AddPage(&mSetNet);
	dlgNetTimeSheet->AddPage(&mSetTime);

	if(nID == 1)
	{
		dlgNetTimeSheet->SetActivePage(&mSetNet);
		m_ComboRemoteNet.GetWindowText(mSetNet.m_TreeText);
	}	
	else if(nID == 2)
	{
		dlgNetTimeSheet->SetActivePage(&mSetTime);
		m_ComboAccessTime.GetWindowText(mSetTime.m_TreeText);
	}
	else	
		return;

	int		iRet	= dlgNetTimeSheet->DoModal();
	delete	dlgNetTimeSheet;
	dlgNetTimeSheet = NULL;

	if(iRet == IDCANCEL)
		return;

	if(tmpNetIndex != mSetNet.iTreeIndex)
		m_ComboRemoteNet.SetCurSel(mSetNet.iTreeIndex);

	if(tmpTimeIndex != mSetTime.m_iTreeIndex)
		m_ComboAccessTime.SetCurSel(mSetTime.m_iTreeIndex);
}

BOOL CAclSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	InitDlgResource();
	
	if(!m_EnableComboApplication)
	{
		ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW); 
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	
	return TRUE;  
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CAclSet::InitDlgResource()
{
	SetWindowText(GUI_ACL_SET_CAPTION);
	SetDlgItemText(IDOK							,GUI_BUTTON_OK);
	SetDlgItemText(IDCANCEL						,GUI_BUTTON_CANCEL);
	SetDlgItemText(IDC_ACL_APPLICATION_LABLE	,GUI_ACL_APPLICATION_LABLE);
	SetDlgItemText(IDC_ACL_REMOTE_NET_LABLE		,GUI_ACL_REMOTE_NET_LABLE);
	SetDlgItemText(IDC_ACL_ACCESS_TIME_LABLE	,GUI_ACL_ACCESS_TIME_LABLE);
	SetDlgItemText(IDC_ACL_ACTION_LABLE			,GUI_ACL_ACTION_LABLE);
	SetDlgItemText(IDC_ACL_SUPER_SET_LABLE		,GUI_ACL_SUPER_SET_LABLE);
	SetDlgItemText(IDC_ACL_DERECTION_LABLE		,GUI_ACL_DERECTION_LABLE);
	SetDlgItemText(IDC_ACL_SERVICE_TYPE_LABLE	,GUI_ACL_SERVICE_TYPE_LABLE);
	SetDlgItemText(IDC_ACL_SERVICE_PORT_LABLE	,GUI_ACL_SERVICE_PORT_LABLE);
	SetDlgItemText(IDC_ACL_MEMO_LABLE			,GUI_ACL_MEMO_LABLE);

	m_ComboRemoteNet.InsertString(0, GUI_SET_NET_TREE_0);
	m_ComboRemoteNet.InsertString(1, GUI_SET_NET_TREE_1);
	m_ComboRemoteNet.InsertString(2, GUI_SET_NET_TREE_2);
	m_ComboRemoteNet.InsertString(3, GUI_SET_NET_TREE_3);
	m_ComboRemoteNet.InsertString(4, GUI_SET_NET_TREE_4);

	m_ComboAccessTime.InsertString(0, GUI_SET_TIME_TREE_0);
	m_ComboAccessTime.InsertString(1, GUI_SET_TIME_TREE_1);
	m_ComboAccessTime.InsertString(2, GUI_SET_TIME_TREE_2);
	m_ComboAccessTime.InsertString(3, GUI_SET_TIME_TREE_3);
	m_ComboAccessTime.InsertString(4, GUI_SET_TIME_TREE_4);
	m_ComboAccessTime.InsertString(5, GUI_SET_TIME_TREE_5);
	m_ComboAccessTime.InsertString(6, GUI_SET_TIME_TREE_6);

	m_ComboAction.InsertString(0, GUI_ACL_ACTION_0);
	m_ComboAction.InsertString(1, GUI_ACL_ACTION_1);

	m_ComboDirection.InsertString(0, GUI_ACL_DERECTION_0);
	m_ComboDirection.InsertString(1, GUI_ACL_DERECTION_1);
	m_ComboDirection.InsertString(2, GUI_ACL_DERECTION_2);

	m_ComboServiceType.InsertString(0, GUI_ACL_SERVICE_TYPE_0);
	m_ComboServiceType.InsertString(1, GUI_ACL_SERVICE_TYPE_1);
	m_ComboServiceType.InsertString(2, GUI_ACL_SERVICE_TYPE_2);
	m_ComboServiceType.InsertString(3, GUI_ACL_SERVICE_TYPE_3);
	m_ComboServiceType.InsertString(4, GUI_ACL_SERVICE_TYPE_4);
	m_ComboServiceType.InsertString(5, GUI_ACL_SERVICE_TYPE_5);
	m_ComboServiceType.InsertString(6, GUI_ACL_SERVICE_TYPE_6);
	m_ComboServiceType.InsertString(7, GUI_ACL_SERVICE_TYPE_7);
	m_ComboServiceType.InsertString(8, GUI_ACL_SERVICE_TYPE_8);

	int i, n = m_AclFile.mAclHeader.ulAclCount;

	for(i = 0; i < n; i++)
	{
		if(m_ComboApplication.FindString(0,m_AclFile.mpAcl[i].sApplication) != CB_ERR)
			continue;
		m_ComboApplication.InsertString(m_ComboApplication.GetCount(),m_AclFile.mpAcl[i].sApplication);
	}

	if(m_EnableComboApplication)
	{
		if(_tcscmp(m_tmpAcl.sApplication, _T("")) == 0)
		{
			SetDlgItemText(IDC_ACL_BASE_SET_LABLE, GUI_ACL_BASE_SET_LABLE);
		}
		else
		{
			SetDlgItemText(IDC_ACL_BASE_SET_LABLE, m_tmpAcl.sApplication);
			m_ComboApplication	.SetWindowText(m_tmpAcl.sApplication);
		}
	}
	else
	{
		CString tmpStr;
		tmpStr.Format("%s %s", m_sPathName, GUI_ACL_QUERY_INFO);
		SetDlgItemText(IDC_ACL_BASE_SET_LABLE, tmpStr);
		m_ComboApplication	.SetWindowText(m_sPathName);
	}
	
	GetDlgItem(IDC_BUTTON_APPLICATION)->EnableWindow(m_EnableComboApplication);
	m_ComboApplication	.EnableWindow(m_EnableComboApplication);
	m_ComboRemoteNet	.SetCurSel(m_tmpAcl.bRemoteNetType);
	m_ComboAccessTime	.SetCurSel(m_tmpAcl.bAccessTimeType);
	m_ComboAction		.SetCurSel(m_tmpAcl.bAction);
	m_ComboDirection	.SetCurSel(m_tmpAcl.bDirection);
	m_ComboServiceType	.SetCurSel(m_tmpAcl.bServiceType);

	OnSelchangeComboServiceType();

	CString				tmpStr;
	tmpStr				.Format(_T("%u"), m_tmpAcl.uiServicePort);
	m_EditServicePort	.SetLimitText(5);
	m_EditServicePort	.SetWindowText(tmpStr);
	m_EditMemo			.SetLimitText(50);
	m_EditMemo			.SetWindowText(m_tmpAcl.sMemo);
}

void CAclSet::OnButtonApplication() 
{
	static TCHAR BASED_CODE szFilter[] = _T("(*.exe)|*.exe||");

	dlgFile =  new CFileDialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,szFilter);

	int iRet = dlgFile->DoModal();

	if(iRet == IDCANCEL)
	{
		delete dlgFile;
		dlgFile = NULL;
		return;
	}

	m_ComboApplication.SetWindowText(dlgFile->m_ofn.lpstrFile);

	delete  dlgFile;
	dlgFile = NULL;
}

void CAclSet::OnOK() 
{
	CString tmpStrApp, tmpStrMemo, tmpStrPort;

	m_ComboApplication	.GetWindowText(tmpStrApp);
	m_EditMemo			.GetWindowText(tmpStrMemo);
	m_EditServicePort	.GetWindowText(tmpStrPort);

	UINT	tmpPort = _ttoi(tmpStrPort);

	if(tmpPort > 65535 || tmpPort < 0)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INAVALID_PORT);
		m_EditServicePort.SetFocus();
		return;
	}

	if(	m_tmpAcl.bAccessTimeType== m_ComboAccessTime.GetCurSel()	&&
		m_tmpAcl.bAction		== m_ComboAction	.GetCurSel()	&&
		m_tmpAcl.bDirection		== m_ComboDirection	.GetCurSel()	&&
		m_tmpAcl.bRemoteNetType	== m_ComboRemoteNet	.GetCurSel()	&&
		m_tmpAcl.bServiceType	== m_ComboServiceType.GetCurSel()	&&
		m_tmpAcl.uiServicePort	== tmpPort							&&
		tmpStrApp.CompareNoCase(m_tmpAcl.sApplication)	== 0		&&
		tmpStrMemo.Compare(m_tmpAcl.sMemo)				== 0		)
	{
		CDialog::OnCancel();
		return;
	}

	if(tmpStrApp == "")
	{
		AfxMessageBox(GUI_ACL_MESSAGE_APP_PATH_ERROR);
		m_ComboApplication.SetFocus();
		return;
	}

	if((tmpStrApp.Right(1) == "\\") || (tmpStrApp.Right(1) == ":"))
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ONLY_PATH);
		m_ComboApplication.SetFocus();
		return;
	}

	if(_taccess(tmpStrApp,0) == -1)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_APP_NOT_EXSITS);
		m_ComboApplication.SetFocus();
		return;
	}

	m_tmpAcl.bAccessTimeType= m_ComboAccessTime	.GetCurSel();	
	m_tmpAcl.bAction		= m_ComboAction		.GetCurSel();
	m_tmpAcl.bDirection		= m_ComboDirection	.GetCurSel();
	m_tmpAcl.bRemoteNetType	= m_ComboRemoteNet	.GetCurSel();
	m_tmpAcl.bServiceType	= m_ComboServiceType.GetCurSel();
	m_tmpAcl.uiServicePort	= tmpPort;					

	_tcscpy(m_tmpAcl.sApplication, tmpStrApp);
	_tcscpy(m_tmpAcl.sMemo, tmpStrMemo);
	
	CDialog::OnOK();
}

void CAclSet::OnSelchangeComboServiceType() 
{
	int		iIndex	= m_ComboServiceType.GetCurSel();
	UINT	mPort	= ACL_SERVICE_PORT_ALL;
	BOOL	bEnable = TRUE;

	switch(iIndex)
	{
	case ACL_SERVICE_TYPE_ALL:
		bEnable = FALSE;		
		break;

	case ACL_SERVICE_TYPE_TCP:	
		break;

	case ACL_SERVICE_TYPE_UDP:	
		break;

	case ACL_SERVICE_TYPE_FTP:
		mPort = ACL_SERVICE_PORT_FTP;
		bEnable = FALSE;
		break;

	case ACL_SERVICE_TYPE_TELNET:
		mPort = ACL_SERVICE_PORT_TELNET;
		bEnable = FALSE;		
		break;

	case ACL_SERVICE_TYPE_HTTP:	
		break;

	case ACL_SERVICE_TYPE_NNTP:
		mPort = ACL_SERVICE_PORT_NNTP;
		bEnable = FALSE;		
		break;

	case ACL_SERVICE_TYPE_POP3:
		mPort = ACL_SERVICE_PORT_POP3;
		bEnable = FALSE;
		break;

	case ACL_SERVICE_TYPE_SMTP:
		mPort = ACL_SERVICE_PORT_SMTP;
		bEnable = FALSE;
		break;

	default:
		return;
	}
	
	CString tmpStr;
	tmpStr	.Format(_T("%u"),mPort);
	m_EditServicePort.SetWindowText(tmpStr);
	m_EditServicePort.EnableWindow(bEnable);
}

void CAclSet::OnSelchangeComboApplication() 
{
	CString tmpStr;

	m_ComboApplication.GetLBText(m_ComboApplication.GetCurSel(),tmpStr);
	SetDlgItemText(IDC_ACL_BASE_SET_LABLE, tmpStr);	
}

