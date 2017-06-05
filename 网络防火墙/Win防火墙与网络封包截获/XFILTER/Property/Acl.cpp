// Acl.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "Acl.h"

//owner add
#include "GuiRes.h"
#include "AclSet.h"
#include "MainSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcl property page

CAclSet*			dlgAclSet	= NULL;
extern CAclFile		m_AclFile;
extern CPropertyApp theApp;
extern CMainSheet	*dlg;

IMPLEMENT_DYNCREATE(CAcl, CPropertyPage)

// static class member of variable
CListCtrl	CAcl::m_ListAcl;

CAcl::CAcl() : CPropertyPage(CAcl::IDD)
{
	//{{AFX_DATA_INIT(CAcl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAcl::~CAcl()
{
}

void CAcl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcl)
	DDX_Control(pDX, IDC_ACL_LIST		, m_ListAcl);
	DDX_Control(pDX, IDC_ACL_RADIO_PASS	, m_RadioPass);
	DDX_Control(pDX, IDC_ACL_RADIO_QUERY, m_RadioQuery);
	DDX_Control(pDX, IDC_ACL_RADIO_DENY	, m_RadioDeny);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcl, CPropertyPage)
	//{{AFX_MSG_MAP(CAcl)
	ON_BN_CLICKED(IDC_ACL_BUTTON_ADD		, OnAclButtonAdd)
	ON_BN_CLICKED(IDC_ACL_BUTTON_EDIT		, OnAclButtonEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ACL_LIST	, OnItemchangedAclList)
	ON_NOTIFY(NM_DBLCLK, IDC_ACL_LIST		, OnDblclkAclList)
	ON_NOTIFY(NM_CLICK, IDC_ACL_LIST		, OnClickAclList)
	ON_BN_CLICKED(IDC_ACL_BUTTON_DELETE		, OnAclButtonDelete)
	ON_BN_CLICKED(IDC_ACL_RADIO_DENY		, OnAclRadioDeny)
	ON_BN_CLICKED(IDC_ACL_RADIO_PASS		, OnAclRadioPass)
	ON_BN_CLICKED(IDC_ACL_RADIO_QUERY		, OnAclRadioQuery)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CAcl message handlers

BOOL CAcl::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	InitDlgResource();

	return TRUE;
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CAcl::InitDlgResource()
{
	SetWindowText(GUI_TAB_ACL);
	SetDlgItemText(IDC_ACL_APPLICATION_SET_LABLE	,GUI_ACL_APPLICATION_SET_LABLE);
	SetDlgItemText(IDC_ACL_RADIO_PASS				,GUI_ACL_RADIO_PASS);
	SetDlgItemText(IDC_ACL_RADIO_QUERY				,GUI_ACL_RADIO_QUERY);
	SetDlgItemText(IDC_ACL_RADIO_DENY				,GUI_ACL_RADIO_DENY);
	SetDlgItemText(IDC_ACL_SET_LABLE				,GUI_ACL_SET_LABLE);
	SetDlgItemText(IDC_ACL_BUTTON_ADD				,GUI_ACL_BUTTON_ADD);
	SetDlgItemText(IDC_ACL_BUTTON_EDIT				,GUI_ACL_BUTTON_EDIT);
	SetDlgItemText(IDC_ACL_BUTTON_DELETE			,GUI_ACL_BUTTON_DELETE);

	InitList(&m_ListAcl);
	
	Refresh();
}

void CAcl::Refresh()
{
	m_ListAcl.DeleteAllItems();
	m_RadioPass.SetCheck(FALSE);
	m_RadioQuery.SetCheck(FALSE);
	m_RadioDeny.SetCheck(FALSE);

	for(DWORD i = 0; i < m_AclFile.mAclHeader.ulAclCount; i++)
		ListAddOne(m_AclFile.mpAcl + i, !i);

	m_iType = m_AclFile.GetBit(m_AclFile.mAclHeader.bSet,4,2);	//bit 5, 6

	if(m_iType == 0)
		m_RadioPass	.SetCheck(TRUE);
	else if(m_iType == 1)
		m_RadioQuery.SetCheck(TRUE);
	else if(m_iType == 2)
		m_RadioDeny	.SetCheck(TRUE);
	else
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ACL_FILE_SET_ERROR);
		return;
	}
}

void CAcl::InitList(CListCtrl *mList)
{
	ListView_SetExtendedListViewStyle(mList->m_hWnd	,
		LVS_EX_GRIDLINES  | LVS_EX_FULLROWSELECT
		); 
	mList->InsertColumn(0, GUI_ACL_LIST_ACTION,
		LVCFMT_LEFT, 
		GUI_ACL_LIST_ACTION_SIZE
		);
	mList->InsertColumn(1, GUI_ACL_LIST_APPLICATION	,
		LVCFMT_LEFT, 
		GUI_ACL_LIST_APPLICATION_SIZE + 40
		);
	mList->InsertColumn(2, GUI_ACL_LIST_DIRECTION,
		LVCFMT_LEFT, 
		GUI_ACL_LIST_DIRECTION_SIZE
		);
	mList->InsertColumn(3, GUI_ACL_LIST_REMOTE_NET,
		LVCFMT_LEFT, 
		GUI_ACL_LIST_REMOTE_NET_SIZE+ 30
		);
	mList->InsertColumn(4, GUI_ACL_LIST_ACCESS_TIME,
		LVCFMT_LEFT, 
		GUI_ACL_LIST_ACCESS_TIME_SIZE + 30
		);
	mList->InsertColumn(5, GUI_ACL_LIST_TYPE_PORT,
		LVCFMT_LEFT, 
		GUI_ACL_LIST_TYPE_PORT_SIZE
		);
	mList->InsertColumn(6, GUI_ACL_LIST_APPLICATION_PATH,
		LVCFMT_LEFT,
		GUI_ACL_LIST_APPLICATION_PATH_SIZE + 30
		);
	mList->InsertColumn(7, GUI_ACL_LIST_MEMO,
		LVCFMT_LEFT,
		GUI_ACL_LIST_MEMO_SIZE + 30
		);
}

int CAcl::ListAddOne(void* mpAcl, BOOL isSelected, BOOL isEdit, int EditIndex)
{
	if(m_ListAcl.m_hWnd == NULL)
		return 0;

	XACL*	mAcl	= (XACL*)mpAcl;
	int		iIndex	= m_ListAcl.GetItemCount();

	if(isEdit) 
		iIndex = EditIndex;
	else 
		m_ListAcl.InsertItem(iIndex, GUI_ACTION[mAcl->bAction]);

	m_ListAcl.SetItemText(iIndex, 0, GUI_ACTION[mAcl->bAction]);
	m_ListAcl.SetItemText(iIndex, 1, CAclFile::GetName(mAcl->sApplication));
	m_ListAcl.SetItemText(iIndex, 2, GUI_DIRECTION[mAcl->bDirection]);
	m_ListAcl.SetItemText(iIndex, 3, GUI_NET[mAcl->bRemoteNetType]);
	m_ListAcl.SetItemText(iIndex, 4, GUI_TIME[mAcl->bAccessTimeType]);

	CString tmpStr;
	tmpStr.Format(_T("%s/%u"),GUI_SERVICE_TYPE[mAcl->bServiceType], mAcl->uiServicePort);
	m_ListAcl.SetItemText(iIndex,5,tmpStr);
	m_ListAcl.SetItemText(iIndex,6, CAclFile::GetPath(mAcl->sApplication));
	m_ListAcl.SetItemText(iIndex,7,mAcl->sMemo);

	if(isSelected)
	{
		m_ListAcl.EnsureVisible(iIndex,TRUE);
		m_ListAcl.SetItemState(iIndex,LVIS_SELECTED,LVIS_SELECTED);
	}

	return 0;
}

void CAcl::OnAclButtonAdd() 
{
	if(!GetDlgItem(IDC_ACL_BUTTON_ADD)->IsWindowEnabled()) 
		return;

	if(InitAddAcl(_T("")) != 0)
		return;

	GetOwner()->GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
}

int CAcl::InitAddAcl(TCHAR* sPathName, BOOL EnableComboApplication) 
{
	if(MAX_ACL != 0 && m_AclFile.mAclHeader.ulAclCount >= MAX_ACL)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_MAX_ACL);
		return -1;
	}

	XACL m_tmpAcl;

	m_tmpAcl.bAccessTimeType	= ACL_TIME_TYPE_ALL;
	m_tmpAcl.bAction			= ACL_ACTION_PASS;
	m_tmpAcl.bDirection			= ACL_DIRECTION_IN_OUT;
	m_tmpAcl.bRemoteNetType		= ACL_NET_TYPE_ALL;
	m_tmpAcl.bServiceType		= ACL_SERVICE_TYPE_ALL;
	_tcscpy(m_tmpAcl.sApplication, _T(""));
	_tcscpy(m_tmpAcl.sMemo		, _T(""));
	m_tmpAcl.uiServicePort		= ACL_SERVICE_PORT_ALL;
	m_tmpAcl.ulAclID			= 0;
	
	CAclSet::m_EnableComboApplication	= EnableComboApplication;
	CAclSet::m_tmpAcl					= m_tmpAcl;
	CAclSet::m_sPathName				= sPathName;

	int iRet = ShowDlg();

	if(iRet == IDCANCEL)
		return IDCANCEL;

	m_tmpAcl = CAclSet::m_tmpAcl;

	if(m_AclFile.AddAcl(&m_tmpAcl,1) != 0)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ADD_ACL_ERROR);
		return -1;
	}

	if(m_ListAcl.m_hWnd != NULL)
		ListAddOne(&CAclSet::m_tmpAcl,TRUE);

	return 0;
}

BOOL WINAPI CAcl::AddAclQuery(TCHAR* sPathName, BOOL EnableComboApplication)
{
	ODS("XFILTER.EXE: AddAclQuery");

	int iRet = InitAddAcl(sPathName, EnableComboApplication);

	if(iRet != 0)
		return FALSE;

	if(m_AclFile.SaveAcl() != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_SAVE_ACL_ERROR);
		return FALSE;
	}

	if(!theApp.InitDllData())
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR);
		return FALSE;
	}

	if(dlg != NULL)
		dlg->GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);

	return TRUE;
}

void CAcl::OnAclButtonEdit() 
{
	if(!GetDlgItem(IDC_ACL_BUTTON_EDIT)->IsWindowEnabled())
		return;

	if(m_iListIndex < 0) 
		return;

	CAclSet::m_tmpAcl = m_AclFile.mpAcl[m_iListIndex];
	CAclSet::m_EnableComboApplication = TRUE;

	int iRet = ShowDlg();

	if(iRet == IDCANCEL) 
		return;

	m_AclFile	.mpAcl[m_iListIndex] = CAclSet::m_tmpAcl;
	ListAddOne	(&CAclSet::m_tmpAcl,FALSE,TRUE,m_iListIndex);
	GetOwner()	-> GetDlgItem(ID_APPLY_NOW) -> EnableWindow(TRUE);
}

int CAcl::ShowDlg()
{
	EnableMenu(FALSE);

	dlgAclSet	= new CAclSet;

	int iRet	= dlgAclSet->DoModal();

	delete		dlgAclSet;
	dlgAclSet	= NULL;

	EnableMenu(TRUE);

	return iRet;
}

UINT CAcl::EnableMenu(UINT bEnable)
{
	theApp.m_EnablePacketMonitor = bEnable;
	theApp.m_EnableQueryLog		 = bEnable;
	theApp.m_EnableAcl			 = bEnable;
	theApp.m_EnableSystemSet	 = bEnable;
	theApp.m_EnableAbout		 = bEnable;

	if(!CAclSet::m_EnableComboApplication)
	{
		theApp.m_EnableExit			= bEnable;
		theApp.m_EnableContolFrame	= bEnable;
	}

	return bEnable;
}

void CAcl::OnItemchangedAclList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if((m_iListIndex = pNMListView->iItem) == -1) 
		return;

	EnableButton(TRUE);
}

void CAcl::EnableButton(BOOL bEnable)
{
	GetDlgItem(IDC_ACL_BUTTON_EDIT)		->EnableWindow(bEnable);
	GetDlgItem(IDC_ACL_BUTTON_DELETE)	->EnableWindow(bEnable);
}

void CAcl::OnDblclkAclList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if((m_iListIndex = pNMListView->iItem) != -1) 
		OnAclButtonEdit();

	*pResult = 0;
}

void CAcl::OnClickAclList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if((m_iListIndex = pNMListView->iItem) == -1) 
		EnableButton(FALSE);

	*pResult = 0;
}

void CAcl::OnAclButtonDelete() 
{
	if(!GetDlgItem(IDC_ACL_BUTTON_DELETE)->IsWindowEnabled())
		return;

	if(m_iListIndex < 0)
		return;

	int tmpIndex = m_iListIndex;

	m_AclFile.DelAcl(m_iListIndex, ACL_TYPE_ACL);
	m_ListAcl.DeleteItem(m_iListIndex);
	GetOwner()->GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	if(m_ListAcl.GetItemCount() <= 0)
	{
		EnableButton(FALSE);
		return;
	}

	if(tmpIndex == m_ListAcl.GetItemCount())	
		tmpIndex -- ;

	m_ListAcl.SetItemState(tmpIndex, LVIS_SELECTED, LVIS_SELECTED);
}

void CAcl::OnAclRadioDeny() 
{
	SetType(2, TRUE, FALSE); //binary 10
}

void CAcl::OnAclRadioPass() 
{
	SetType(0, FALSE, FALSE); //binary 00
}

void CAcl::OnAclRadioQuery() 
{
	SetType(1, FALSE, TRUE); //binary 01
}

BOOL CAcl::SetType(int type, BOOL bit4, BOOL bit5)
{
	if(type == m_iType)		
		return FALSE;

	m_AclFile.SetBit(&m_AclFile.mAclHeader.bSet, 4, bit4);
	m_AclFile.SetBit(&m_AclFile.mAclHeader.bSet, 5, bit5);
	m_iType = type;
	GetOwner()->GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);

	return TRUE;
}

BOOL CAcl::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.WinHelp(6, HH_HELP_CONTEXT);

	return TRUE;
}
