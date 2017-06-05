// PacketMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "PacketMonitor.h"

//owner add
#include "GuiRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CPacketMonitor property page

extern CAclFile		m_AclFile;
extern CPropertyApp theApp;

BOOL		CPacketMonitor::IsScroll	= TRUE;
BOOL		CPacketMonitor::IsMonitor	= TRUE;
CListCtrl	*CPacketMonitor::m_pListPacketMonitor = NULL;

IMPLEMENT_DYNCREATE(CPacketMonitor, CPropertyPage)

CPacketMonitor::CPacketMonitor() : CPropertyPage(CPacketMonitor::IDD)
{
}

CPacketMonitor::~CPacketMonitor()
{
}

void CPacketMonitor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketMonitor)
	DDX_Control(pDX, IDC_LIST_PACKET_MONITOR, m_ListPacketMonitor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPacketMonitor, CPropertyPage)
	//{{AFX_MSG_MAP(CPacketMonitor)
	ON_BN_CLICKED(IDC_BUTTON_MONITOR, OnButtonMonitor)
	ON_BN_CLICKED(IDC_BUTTON_SCROLL, OnButtonScroll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CPacketMonitor message handlers

BOOL CPacketMonitor::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	InitDlgResource();
	
	return TRUE;  
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CPacketMonitor::InitDlgResource()
{
	SetDlgItemText(IDC_PACKET_MONITOR_LABLE,	GUI_PACKET_MONITOR_LABLE);
	SetDlgItemText(IDC_BUTTON_SCROLL,			GUI_PACKET_MONITOR_TOOLBAR_STOP_SCROLL);
	SetDlgItemText(IDC_BUTTON_MONITOR,			GUI_PACKET_MONITOR_TOOLBAR_STOP_MONITOR);
	SetDlgItemText(IDC_BUTTON_CLEAR,			GUI_PACKET_MONITOR_TOOLBAR_CLEAR);

	m_pListPacketMonitor = &m_ListPacketMonitor;

	InitList(&m_ListPacketMonitor);
}

//=============================================================================================
// static function

void CPacketMonitor::InitList(CListCtrl *mList)
{
	ListView_SetExtendedListViewStyle(
		mList->m_hWnd, 
		LVS_EX_GRIDLINES  | LVS_EX_FULLROWSELECT
		); 

	mList->InsertColumn(0, GUI_PACKET_MONITOR_LIST_ACTION,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_ACTION_SIZE 
		);

	mList->InsertColumn(1, GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME_SIZE + 2
		);

	mList->InsertColumn(2, GUI_PACKET_MONITOR_LIST_PROTOCOL,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_PROTOCOL_SIZE + 10
		);

	mList->InsertColumn(3, GUI_PACKET_MONITOR_LIST_IN_DATA,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_IN_DATA_SIZE - 8
		);

	mList->InsertColumn(4, GUI_PACKET_MONITOR_LIST_OUT_DATA,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_OUT_DATA_SIZE - 8
		);

	mList->InsertColumn(5, GUI_PACKET_MONITOR_LIST_IP_PORT,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_IP_PORT_SIZE + 32
		);

	mList->InsertColumn(6, GUI_PACKET_MONITOR_LIST_APPLICATION,
		LVCFMT_LEFT,
		GUI_PACKET_MONITOR_LIST_APPLICATION_SIZE + MAX_PATH * 4
		);
}

int	CPacketMonitor::InsertOneList(
	CListCtrl * mList, 
	SESSION *session, 
	BOOL mIsAdd, 
	BOOL mIsScroll
)
{
	if(mList == NULL)
		return XERR_LOG_INVALID_LIST;

	if(!mIsAdd)
		return XERR_LOG_NOT_MONITOR;

	if(session == NULL || session->s == 0)
		return XERR_LOG_INVALID_SESSION;

	CString		tmpStr;
	int			iIndex = mList->GetItemCount();

	mList->InsertItem(iIndex, GUI_ACTION[session->bAction]);

	tmpStr.Format(_T("%s - %s"),
		session->tStartTime.Format(_T("%m/%d %H:%M:%S")),
		session->tEndTime.Format(_T("%H:%M:%S"))
		);

	mList->SetItemText(iIndex, 1, tmpStr);
	mList->SetItemText(iIndex, 2, GUI_SERVICE_TYPE[session->bProtocol]);

	tmpStr.Format(_T("%u"),
		session->ulRecvData
		);

	mList->SetItemText(iIndex, 3, tmpStr);

	tmpStr.Format(_T("%u"),
		session->ulSendData
		);

	mList->SetItemText(iIndex, 4, tmpStr);

	tmpStr.Format(_T("%s,%u %s %s,%u"), 
		CAclFile::DIPToSIP(&session->ulLocalIP),
		session->uiLocalPort,
		GUI_DIRECTION_EX[session->bDirection],
		CAclFile::DIPToSIP(&session->ulRemoteIP),
		session->uiPort
		);

	mList->SetItemText(iIndex, 5, tmpStr);

	tmpStr.Format(_T("%s :: %s"),
		session->sPathName,
		session->sMemo[0] == '\0' ? CAclFile::DIPToSIP(&session->ulRemoteIP) : session->sMemo 
		);

	mList->SetItemText(iIndex, 6, tmpStr);

	if(mIsScroll)
		mList->EnsureVisible(iIndex, TRUE);

	return XERR_SUCCESS;
}


void CPacketMonitor::OnButtonScroll() 
{
	IsScroll = !IsScroll;

	SetDlgItemText(IDC_BUTTON_SCROLL, IsScroll ? GUI_PACKET_MONITOR_TOOLBAR_STOP_SCROLL 
		: GUI_PACKET_MONITOR_TOOLBAR_START_SCROLL);
}

void CPacketMonitor::OnButtonMonitor() 
{
	IsMonitor = !IsMonitor;

	SetDlgItemText(IDC_BUTTON_MONITOR, IsMonitor ? GUI_PACKET_MONITOR_TOOLBAR_STOP_MONITOR 
		: GUI_PACKET_MONITOR_TOOLBAR_START_MONITOR);
}


void CPacketMonitor::OnButtonClear() 
{
	m_ListPacketMonitor.DeleteAllItems();
}

BOOL CPacketMonitor::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.WinHelp(4, HH_HELP_CONTEXT);

	return TRUE;
}

