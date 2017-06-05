// SetTime.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "SetTime.h"

//owner add
#include "GuiRes.h"
#include "NetTimeSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CSetTime property page

extern CAclFile			m_AclFile;
extern CNetTimeSheet	*mNetTimeSheet;
DWORD  m_ConstTime		= CTime(1990,1,1,0,0,0).GetTime();

IMPLEMENT_DYNCREATE(CSetTime, CPropertyPage)

CSetTime::CSetTime() : CPropertyPage(CSetTime::IDD)
{
	m_iTreeIndex	= 0;
	m_TreeText		= GUI_SET_TIME_TREE_0;
}

CSetTime::~CSetTime()
{
}

void CSetTime::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTime)
	DDX_Control(pDX, IDC_SET_TIME_CHECK_WEDNESDAY,	m_Check3);
	DDX_Control(pDX, IDC_SET_TIME_CHECK_TUESDAY,	m_Check2);
	DDX_Control(pDX, IDC_SET_TIME_CHECK_THURSDAY,	m_Check4);
	DDX_Control(pDX, IDC_SET_TIME_CHECK_SUNDAY,		m_Check0);
	DDX_Control(pDX, IDC_SET_TIME_CHECK_SATURDAY,	m_Check6);
	DDX_Control(pDX, IDC_SET_TIME_CHECK_MONDAY,		m_Check1);
	DDX_Control(pDX, IDC_SET_TIME_TIME_START,		m_TimeStart);
	DDX_Control(pDX, IDC_SET_TIME_TIME_END,			m_TimeEnd);
	DDX_Control(pDX, IDC_SET_TIME_CHECK_FRIDAY,		m_Check5);
	DDX_Control(pDX, IDC_SET_TIME_TREE,				m_TreeSetTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTime, CPropertyPage)
	//{{AFX_MSG_MAP(CSetTime)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SET_TIME_TREE,	OnSelchangedSetTimeTree)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_SUNDAY,		OnSetTimeCheckSunday)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_MONDAY,		OnSetTimeCheckMonday)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_TUESDAY,		OnSetTimeCheckTuesday)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_THURSDAY,		OnSetTimeCheckThursday)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_SATURDAY,		OnSetTimeCheckSaturday)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_FRIDAY,		OnSetTimeCheckFriday)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SET_TIME_TIME_END,	OnDatetimechangeSetTimeTimeEnd)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SET_TIME_TIME_START,	OnDatetimechangeSetTimeTimeStart)
	ON_BN_CLICKED(IDC_SET_TIME_CHECK_WEDNESDAY,		OnSetTimeCheckWednesday)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CSetTime message handlers

BOOL CSetTime::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitDlgResource();
	mNetTimeSheet = (CNetTimeSheet*)this->GetOwner();
	
	return TRUE; 
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CSetTime::InitDlgResource()
{
	SetDlgItemText(IDC_SET_TIME_WEEK_TIME_LABLE	,GUI_SET_TIME_WEEK_TIME_LABLE);
	SetDlgItemText(IDC_SET_TIME_CHECK_SUNDAY	,GUI_SET_TIME_CHECK_SUNDAY);
	SetDlgItemText(IDC_SET_TIME_CHECK_MONDAY	,GUI_SET_TIME_CHECK_MONDAY);
	SetDlgItemText(IDC_SET_TIME_CHECK_TUESDAY	,GUI_SET_TIME_CHECK_TUESDAY);
	SetDlgItemText(IDC_SET_TIME_CHECK_WEDNESDAY	,GUI_SET_TIME_CHECK_WEDNESDAY);
	SetDlgItemText(IDC_SET_TIME_CHECK_THURSDAY	,GUI_SET_TIME_CHECK_THURSDAY);
	SetDlgItemText(IDC_SET_TIME_CHECK_FRIDAY	,GUI_SET_TIME_CHECK_FRIDAY);
	SetDlgItemText(IDC_SET_TIME_CHECK_SATURDAY	,GUI_SET_TIME_CHECK_SATURDAY);
	SetDlgItemText(IDC_SET_TIME_TIME_LABLE		,GUI_SET_TIME_TIME_LABLE);
	SetDlgItemText(IDC_SET_TIME_LABLE_START_TIME,GUI_SET_TIME_LABLE_START_TIME);
	SetDlgItemText(IDC_SET_TIME_LABLE_END_TIME	,GUI_SET_TIME_LABLE_END_TIME);

	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_0);
	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_1);
	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_2);
	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_3);
	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_4);
	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_5);
	m_TreeSetTime.InsertItem(GUI_SET_TIME_TREE_6);

	SelectTreeItem(m_TreeText);
}	


void CSetTime::OnSelchangedSetTimeTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	SelectTreeItemOperate();

	*pResult = 0;
}

int CSetTime::SelectTreeItemOperate()
{
	DWORD		i	= 0;
	CString sText	= m_TreeSetTime.GetItemText(m_TreeSetTime.GetSelectedItem());
	m_iTreeIndex	= TextToIndex(sText);

	if(m_iTreeIndex == -1)		
		return -1;

	if(m_iTreeIndex == 0)
	{
		EnableButton(FALSE);
		SetValue(&m_AclFile.mAllTime);
	}
	else if(m_iTreeIndex > 0 || m_iTreeIndex < 7)
	{
		SetValue(&m_AclFile.mAclTime[m_iTreeIndex - 1]);
		EnableButton(TRUE);
	}

	return 0;
}

int CSetTime::SetValue(void* acltime)
{
	XACL_TIME* mAclTime = (XACL_TIME*)acltime;

	m_Check0.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,0));
	m_Check1.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,1));
	m_Check2.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,2));
	m_Check3.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,3));
	m_Check4.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,4));
	m_Check5.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,5));
	m_Check6.SetCheck(m_AclFile.GetBit(mAclTime->bWeekDay,6));

	m_TimeStart	.SetTime(&CTime(m_ConstTime + mAclTime->tStartTime.GetTime()));
	m_TimeEnd	.SetTime(&CTime(m_ConstTime + mAclTime->tEndTime.GetTime()));

	return 0;
}

int CSetTime::EnableButton(BOOL bEnable)
{
	m_Check0.EnableWindow(bEnable);
	m_Check1.EnableWindow(bEnable);
	m_Check2.EnableWindow(bEnable);
	m_Check3.EnableWindow(bEnable);
	m_Check4.EnableWindow(bEnable);
	m_Check5.EnableWindow(bEnable);
	m_Check6.EnableWindow(bEnable);

	m_TimeStart	.EnableWindow(bEnable);
	m_TimeEnd	.EnableWindow(bEnable);

	return 0;
}

int CSetTime::TextToIndex(CString sText)
{
	if(sText.Compare(GUI_SET_TIME_TREE_0) == 0)		return 0;
	if(sText.Compare(GUI_SET_TIME_TREE_1) == 0)		return 1;
	if(sText.Compare(GUI_SET_TIME_TREE_2) == 0)		return 2;
	if(sText.Compare(GUI_SET_TIME_TREE_3) == 0)		return 3;
	if(sText.Compare(GUI_SET_TIME_TREE_4) == 0)		return 4;
	if(sText.Compare(GUI_SET_TIME_TREE_5) == 0)		return 5;
	if(sText.Compare(GUI_SET_TIME_TREE_6) == 0)		return 6;

	return -1;
}

int CSetTime::SelectTreeItem(CString sText)
{
	int i = 0 , iItemCount	= m_TreeSetTime.GetCount();
	HTREEITEM mItem			= m_TreeSetTime.GetFirstVisibleItem();

	if(sText.Compare(m_TreeSetTime.GetItemText(mItem)) == 0)		
		return i;

	for(i = 1; i < iItemCount; i++)
	{
		mItem = m_TreeSetTime.GetNextSiblingItem(mItem);
		if(sText.Compare(m_TreeSetTime.GetItemText(mItem)) == 0)
		{
			m_TreeSetTime.SelectItem(mItem);
			return i;
		}
	}

	return -1;
}

void CSetTime::OnSetTimeCheckSunday() 
{
	SetAclValue(0, m_Check0.GetCheck());
}

void CSetTime::OnSetTimeCheckMonday() 
{
	SetAclValue(1, m_Check1.GetCheck());
}

void CSetTime::OnSetTimeCheckTuesday() 
{
	SetAclValue(2, m_Check2.GetCheck());
}

void CSetTime::OnSetTimeCheckWednesday() 
{
	SetAclValue(3, m_Check3.GetCheck());
}

void CSetTime::OnSetTimeCheckThursday() 
{
	SetAclValue(4, m_Check4.GetCheck());
}

void CSetTime::OnSetTimeCheckFriday() 
{
	SetAclValue(5, m_Check5.GetCheck());
}

void CSetTime::OnSetTimeCheckSaturday() 
{
	SetAclValue(6, m_Check6.GetCheck());
}

void CSetTime::OnDatetimechangeSetTimeTimeStart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetTimeValue(0);
	*pResult = 0;
}

void CSetTime::OnDatetimechangeSetTimeTimeEnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetTimeValue(1);
	*pResult = 0;
}

int CSetTime::SetAclValue(int iIndex,BOOL isTrue)
{
	m_AclFile		. SetBit(&m_AclFile.mAclTime[m_iTreeIndex - 1].bWeekDay,iIndex,isTrue);
	mNetTimeSheet	->EnableButton(TRUE);

	return 0;
}

int CSetTime::SetTimeValue(int iType)
{
	CTime t;

	switch(iType)
	{
	case 0:			//start time
		m_TimeStart	.GetTime(t);
		m_AclFile	.mAclTime[m_iTreeIndex -1].tStartTime =  t.GetTime() - m_ConstTime;
		break;

	case 1:			//end time
		m_TimeEnd	.GetTime(t);
		m_AclFile	.mAclTime[m_iTreeIndex -1].tEndTime =  t.GetTime() - m_ConstTime;
		break;

	default:
		return -1;
	}

	mNetTimeSheet->EnableButton(TRUE);

	return 0;
}
