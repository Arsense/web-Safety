// SetNet.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "SetNet.h"

//onwer add
#include "GuiRes.h"
#include "NetIPAria.h"
#include "NetTimeSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CSetNet property page

extern CAclFile		m_AclFile;
XACL_IP				m_tmpIP;
XACL_IP				*m_pIP;
CNetTimeSheet		*mNetTimeSheet;
CNetIPAria			*dlgNetIPAria	= NULL;

IMPLEMENT_DYNCREATE(CSetNet, CPropertyPage)

CSetNet::CSetNet() : CPropertyPage(CSetNet::IDD)
{
	iListIndex	= 0;
	iTreeIndex	= 0;
	m_TreeText	= GUI_SET_NET_TREE_0;
}

CSetNet::~CSetNet()
{
}

void CSetNet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetNet)
	DDX_Control(pDX, IDC_SET_NET_TREE, m_TreeSetNet);
	DDX_Control(pDX, IDC_SET_NET_LIST, m_ListSetNet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetNet, CPropertyPage)
	//{{AFX_MSG_MAP(CSetNet)
	ON_BN_CLICKED(IDC_SET_NET_BUTTON_ADD,		OnSetNetButtonAdd)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SET_NET_TREE, OnSelchangedSetNetTree)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SET_NET_LIST,OnItemchangedSetNetList)
	ON_BN_CLICKED(IDC_SET_NET_BUTTON_EDIT,		OnSetNetButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_SET_NET_LIST,		OnDblclkSetNetList)
	ON_BN_CLICKED(IDC_SET_NET_BUTTON_DELETE,	OnSetNetButtonDelete)
	ON_NOTIFY(NM_CLICK, IDC_SET_NET_LIST,		OnClickSetNetList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CSetNet message handlers

BOOL CSetNet::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitDlgResource();
	mNetTimeSheet = (CNetTimeSheet*)this->GetOwner();
	
	return TRUE;  
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CSetNet::InitDlgResource()
{
	SetDlgItemText(IDC_SET_NET_NET_LABLE	,GUI_SET_NET_NET_LABLE);
	SetDlgItemText(IDC_SET_NET_BUTTON_ADD	,GUI_SET_NET_BUTTON_ADD);
	SetDlgItemText(IDC_SET_NET_BUTTON_EDIT	,GUI_SET_NET_BUTTON_EDIT);
	SetDlgItemText(IDC_SET_NET_BUTTON_DELETE,GUI_SET_NET_BUTTON_DELETE);

	m_TreeSetNet.InsertItem(GUI_SET_NET_TREE_0);
	m_TreeSetNet.InsertItem(GUI_SET_NET_TREE_1);
	m_TreeSetNet.InsertItem(GUI_SET_NET_TREE_2);
	m_TreeSetNet.InsertItem(GUI_SET_NET_TREE_3);
	m_TreeSetNet.InsertItem(GUI_SET_NET_TREE_4);

	InitList(&m_ListSetNet);
	SelectTreeItem(m_TreeText);
}
void CSetNet::InitList(CListCtrl *mList)
{
	mList->SetExtendedStyle	(LVS_EX_GRIDLINES  | LVS_EX_FULLROWSELECT);
	mList->InsertColumn		(0, GUI_SET_NET_LIST_START_IP, LVCFMT_LEFT,143);
	mList->InsertColumn		(1, GUI_SET_NET_LIST_END_IP	, LVCFMT_LEFT,143);
}	

void CSetNet::OnSelchangedSetNetTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	SelectTreeItemOperate();

	*pResult = 0;
}

int CSetNet::SelectTreeItemOperate()
{
	DWORD	i		= 0;
	CString sNetText= m_TreeSetNet.GetItemText(m_TreeSetNet.GetSelectedItem());
	iTreeIndex		= NetTextToIndex(sNetText);

	m_ListSetNet.DeleteAllItems();

	switch(iTreeIndex)
	{
	case 0:
		EnableButton(FALSE);
		m_ListSetNet.InsertItem(0,GUI_NET_IP_ARIA_ALL);
		m_ListSetNet.SetItemText(0,1,GUI_NET_IP_ARIA_ALL);
		break;

	case 1:
		EnableButton(FALSE);
		ListAddIP(&m_AclFile.mAclIntranetIP,	m_AclFile.mAclHeader.ulIntranetIPCount);
		break;

	case 2:
		EnableButton(TRUE);
		ListAddIP(m_AclFile.mpAclDistrustIP,	m_AclFile.mAclHeader.ulDistrustIPCount);
		break;

	case 3:
		EnableButton(TRUE);
		ListAddIP(m_AclFile.mpAclTrustIP,		m_AclFile.mAclHeader.ulTrustIPCount);
		break;

	case 4:
		EnableButton(TRUE);
		ListAddIP(m_AclFile.mpAclCustomIP,		m_AclFile.mAclHeader.ulCustomIPCount);
		break;

	default:
		break;
	}

	m_ListSetNet.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);

	return 0;
}

int CSetNet::ListAddIP(void* mpAclIP,DWORD mCount)
{
	DWORD	i = 0;

	for(i = 0; i < mCount; i++)	
		ListAddOneIP(i, mpAclIP);

	return 0;
}

int CSetNet::ListAddOneIP(DWORD iIndex, void* mpAclIP, BOOL isSelected)
{
	XACL_IP		*mAclIP = (XACL_IP*)mpAclIP;

	if(isSelected)
	{
		m_ListSetNet.InsertItem(iIndex,		m_AclFile.DIPToSIP(&mAclIP->ulStartIP));
		m_ListSetNet.SetItemText(iIndex, 1,	m_AclFile.DIPToSIP(&mAclIP->ulEndIP));
		m_ListSetNet.EnsureVisible(iIndex,	TRUE);
		m_ListSetNet.SetItemState(iIndex,	LVIS_SELECTED,LVIS_SELECTED);
	}
	else
	{
		m_ListSetNet.InsertItem(iIndex,		m_AclFile.DIPToSIP(&mAclIP[iIndex].ulStartIP));
		m_ListSetNet.SetItemText(iIndex, 1, m_AclFile.DIPToSIP(&mAclIP[iIndex].ulEndIP));
	}

	return 0;
}

int CSetNet::EnableButton(BOOL bEnable,BOOL bEnableEx)
{
	GetDlgItem(IDC_SET_NET_BUTTON_ADD)		->EnableWindow(bEnable);
	GetDlgItem(IDC_SET_NET_BUTTON_EDIT)		->EnableWindow(bEnableEx);
	GetDlgItem(IDC_SET_NET_BUTTON_DELETE)	->EnableWindow(bEnableEx);

	return 0;
}

int CSetNet::NetTextToIndex(CString sNetText)
{
	if(sNetText.Compare(GUI_SET_NET_TREE_0) == 0)		return 0;
	if(sNetText.Compare(GUI_SET_NET_TREE_1) == 0)		return 1;
	if(sNetText.Compare(GUI_SET_NET_TREE_2) == 0)		return 2;
	if(sNetText.Compare(GUI_SET_NET_TREE_3) == 0)		return 3;
	if(sNetText.Compare(GUI_SET_NET_TREE_4) == 0)		return 4;

	return -1;
}

int CSetNet::SelectTreeItem(CString sNetText)
{
	int i = 0 , iItemCount	= m_TreeSetNet.GetCount();
	HTREEITEM	mItem		= m_TreeSetNet.GetFirstVisibleItem();

	if(sNetText.Compare(m_TreeSetNet.GetItemText(mItem)) == 0)	
		return i;

	for(i = 1; i < iItemCount; i++)
	{
		mItem = m_TreeSetNet.GetNextSiblingItem(mItem);

		if(sNetText.Compare(m_TreeSetNet.GetItemText(mItem)) == 0)
		{
			m_TreeSetNet.SelectItem(mItem);
			return i;
		}
	}

	return -1;
}

void CSetNet::OnItemchangedSetNetList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView	= (NM_LISTVIEW*)pNMHDR;
	*pResult					= 0;

	if((iListIndex = pNMListView->iItem) == -1)		
		return;

	if(iTreeIndex != 0 && iTreeIndex != 1)			
		EnableButton(TRUE,TRUE);

	if(iTreeIndex == 2)				
		m_pIP = &m_AclFile.mpAclDistrustIP[iListIndex];
	else if(iTreeIndex == 3)		
		m_pIP = &m_AclFile.mpAclTrustIP[iListIndex];
	else if(iTreeIndex == 4)
		m_pIP = &m_AclFile.mpAclCustomIP[iListIndex];

	return;
}

void CSetNet::OnSetNetButtonAdd() 
{
	if(!GetDlgItem(IDC_SET_NET_BUTTON_ADD)->IsWindowEnabled())		
		return;

	if(MAX_IP_ARIA != 0 && m_ListSetNet.GetItemCount() >= MAX_IP_ARIA)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_MAX_IP_ARIA);
		return;
	}

	dlgNetIPAria		= new CNetIPAria;
	dlgNetIPAria->iState= 0;	//add
	int iRet			= dlgNetIPAria->DoModal();
	m_tmpIP.ulStartIP	= dlgNetIPAria->ulStartIP;
	m_tmpIP.ulEndIP		= dlgNetIPAria->ulEndIP;

	delete				dlgNetIPAria;
	dlgNetIPAria		= NULL;

	if(iRet == IDCANCEL)
		return;

	if(m_AclFile.AddAcl(&m_tmpIP,1,iTreeIndex) != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ADD_ACL_IP_ERROR);
		return;
	}

	ListAddOneIP(m_ListSetNet.GetItemCount(),&m_tmpIP,TRUE);
	mNetTimeSheet->EnableButton(TRUE);
}

void CSetNet::OnSetNetButtonEdit() 
{
	if(!GetDlgItem(IDC_SET_NET_BUTTON_EDIT)->IsWindowEnabled())		
		return;

	if(iListIndex < 0)		
		return;

	dlgNetIPAria			= new CNetIPAria;
	dlgNetIPAria->iState	= 1;	//edit
	dlgNetIPAria->ulStartIP	= m_pIP->ulStartIP ;
	dlgNetIPAria->ulEndIP	= m_pIP->ulEndIP ;

	int iRet		= dlgNetIPAria->DoModal();
	m_pIP->ulStartIP= dlgNetIPAria->ulStartIP;
	m_pIP->ulEndIP	= dlgNetIPAria->ulEndIP;
	delete			dlgNetIPAria;
	dlgNetIPAria	= NULL;

	if(iRet == IDCANCEL)		
		return;

	m_ListSetNet.SetItemText(iListIndex, 0, m_AclFile.DIPToSIP(&m_pIP->ulStartIP));
	m_ListSetNet.SetItemText(iListIndex, 1, m_AclFile.DIPToSIP(&m_pIP->ulEndIP));
	mNetTimeSheet->EnableButton(TRUE);
}

void CSetNet::OnDblclkSetNetList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if((iListIndex = pNMListView->iItem) != -1)
		OnSetNetButtonEdit();

	*pResult = 0;
}

void CSetNet::OnSetNetButtonDelete() 
{
	if(iListIndex < 0)	
		return;

	int tmpIndex = iListIndex;

	m_AclFile	.DelAcl(iListIndex, iTreeIndex);
	m_ListSetNet.DeleteItem(iListIndex);
	mNetTimeSheet->EnableButton(TRUE);

	if(m_ListSetNet.GetItemCount() <= 0)
	{
		EnableButton(TRUE,FALSE);
		return;
	}

	if(tmpIndex == m_ListSetNet.GetItemCount())
		tmpIndex -- ;

	m_ListSetNet.SetItemState(tmpIndex,LVIS_SELECTED,LVIS_SELECTED);
}

void CSetNet::OnClickSetNetList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if((iListIndex = pNMListView->iItem) == -1)
		EnableButton(iTreeIndex > 1 ? TRUE : FALSE, FALSE);

	*pResult = 0;
}
