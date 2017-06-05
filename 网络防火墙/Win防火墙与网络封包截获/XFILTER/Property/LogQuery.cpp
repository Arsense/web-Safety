// LogQuery.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "LogQuery.h"

//owner add
#include "GuiRes.h"
#include "PacketMonitor.h"
#include "..\common\xlogfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CLogQuery property page

extern CPropertyApp theApp;
extern CAclFile		m_AclFile;

IMPLEMENT_DYNCREATE(CLogQuery, CPropertyPage)

CLogQuery::CLogQuery() : CPropertyPage(CLogQuery::IDD)
{
	m_iCurrentPage		= 0;
	m_iPageCount		= 0;
	m_ulStartPosition	= 0;
	m_ulRecordCount		= 0;
	m_pLogFile			= NULL;
}

CLogQuery::~CLogQuery()
{
}

void CLogQuery::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogQuery)
	DDX_Control(pDX, IDC_STOP_TIME, m_EndTime);
	DDX_Control(pDX, IDC_STOP_DATE, m_EndDate);
	DDX_Control(pDX, IDC_START_TIME, m_StartTime);
	DDX_Control(pDX, IDC_START_DATE, m_StartDate);
	DDX_Control(pDX, IDC_LIST_LOG, m_ListLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogQuery, CPropertyPage)
	//{{AFX_MSG_MAP(CLogQuery)
	ON_BN_CLICKED(IDC_LOG_QUERY_BUTTON_QUERY, OnLogQueryButtonQuery)
	ON_BN_CLICKED(IDC_LOG_QUERY_BUTTON_DELETE, OnLogQueryButtonDelete)
	ON_BN_CLICKED(IDC_LOG_QUERY_BUTTON_BACK, OnLogQueryButtonBack)
	ON_BN_CLICKED(IDC_LOG_QUERY_BUTTON_NEXT, OnLogQueryButtonNext)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CLogQuery message handlers

BOOL CLogQuery::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitDlgResource();
	
	return TRUE;  
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CLogQuery::InitDlgResource()
{
	SetDlgItemText(IDC_LOG_QUERY_SET_LABLE				,GUI_LOG_QUERY_SET_LABLE);
	SetDlgItemText(IDC_LOG_QUERY_SET_START_TIME_LABLE	,GUI_LOG_QUERY_SET_START_TIME_LABLE);
	SetDlgItemText(IDC_LOG_QUERY_SET_END_TIME_LABLE		,GUI_LOG_QUERY_SET_END_TIME_LABLE);
	SetDlgItemText(IDC_LOG_QUERY_BUTTON_QUERY			,GUI_LOG_QUERY_BUTTON_QUERY);
	SetDlgItemText(IDC_LOG_QUERY_BUTTON_BACK			,GUI_LOG_QUERY_BUTTON_BACK);
	SetDlgItemText(IDC_LOG_QUERY_BUTTON_NEXT			,GUI_LOG_QUERY_BUTTON_NEXT);
	SetDlgItemText(IDC_LOG_QUERY_BUTTON_DELETE			,GUI_LOG_QUERY_BUTTON_DELETE);

	SetPageLable();

	CTimeSpan	ts(ACL_MAX_TIME);
	m_StartDate.SetTime(&(CTime::GetCurrentTime() - ts));

	SetButton();

	CPacketMonitor::InitList(&m_ListLog);
}

void CLogQuery::OnLogQueryButtonQuery() 
{
	LOG_FIND	find;

	find.tStartTime = GetTime(TRUE);
	find.tEndTime	= GetTime(FALSE);

	static CXLogFile		m_file;

	m_pLogFile		= (void*)&m_file;

	if(find.tStartTime > find.tEndTime)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_START_TIME_MIN_END_TIME);
		return;
	}

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if(m_file.FindLog(&find, m_AclFile.mAclHeader.uiLogSize) != XERR_SUCCESS 
		|| find.ulRecordCount == 0)
	{
		SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
		AfxMessageBox(GUI_ACL_MESSAGE_CAN_NOT_FIND_RECORD);
		return;
	}

	SetButton();
	
	m_ulStartPosition	= find.ulStartPosition;
	m_ulRecordCount		= find.ulRecordCount;
	m_ulChangeDirectionIndex = find.ulChangeDirectionIndex;
	m_iCurrentPage		= 1;
	m_iPageCount		= m_ulRecordCount / LOG_QUERY_PAGE_SIZE;

	if((m_ulRecordCount % LOG_QUERY_PAGE_SIZE) != 0)
		m_iPageCount ++;

	if(m_iPageCount > 1)
		SetButton(FALSE, TRUE);

	ShowRecordPage();

	SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
}

int CLogQuery::ShowRecordPage()
{
	if(m_pLogFile == NULL || m_iCurrentPage <= 0 || m_iCurrentPage > m_iPageCount)
		return XERR_LOG_NO_CAN_SHOW_RECORD;

	DWORD ulShowedCount	= (m_iCurrentPage - 1) * LOG_QUERY_PAGE_SIZE;
	DWORD ulRecordCount	= (m_iCurrentPage != m_iPageCount) ? LOG_QUERY_PAGE_SIZE : 
							  (m_ulRecordCount - ulShowedCount);
	DWORD ulStartPosition = 0;
	if(m_ulChangeDirectionIndex > 0 && ulShowedCount >= m_ulChangeDirectionIndex)
		ulStartPosition	= LOG_HEADER_LENTH + (ulShowedCount - m_ulChangeDirectionIndex) * SESSION_LENTH;
	else
		ulStartPosition	= m_ulStartPosition + ulShowedCount * SESSION_LENTH;

#if _DEBUG
	CString a;
	a.Format("ChangeDirectionIndex: %u; StartPosition: %u; m_StartPosition: %u"
		, m_ulChangeDirectionIndex
		, ulStartPosition
		, m_ulStartPosition
		);
	ODS(a);
#endif

	SetPageLable(ulRecordCount);

	CXLogFile *m_file = (CXLogFile*)m_pLogFile;

	SESSION session;

	TRY
	{
		m_ListLog.DeleteAllItems();

		m_file->m_LogFile.Seek(ulStartPosition, CFile::begin);

		for(DWORD i = 1; i <= ulRecordCount; i++)
		{
			m_file->m_LogFile.Read(&session, SESSION_LENTH);
			CPacketMonitor::InsertOneList(&m_ListLog, &session, TRUE, FALSE);

			if(m_ulChangeDirectionIndex > 0 && (ulShowedCount + i) == m_ulChangeDirectionIndex)
				m_file->m_LogFile.Seek(LOG_HEADER_LENTH, CFile::begin);
		}

	}
	CATCH( CFileException, e )
	{
		return XERR_LOG_READ_FILE_ERROR;
	}
	END_CATCH

	return XERR_SUCCESS;
}

CTime CLogQuery::GetTime(BOOL IsStart)
{
	CTime t(0), td1, tt1;

	if(IsStart)
	{
		m_StartDate.GetTime(td1);
		m_StartTime.GetTime(tt1);
	}
	else
	{
		m_EndDate.GetTime(td1);
		m_EndTime.GetTime(tt1);
	}

	CTimeSpan ts = tt1.GetHour() * 3600 + tt1.GetMinute() * 60 + tt1.GetSecond()
		- td1.GetHour() * 3600 - td1.GetMinute() * 60 - td1.GetSecond();

	t = td1 + ts;

	return t;
}

void CLogQuery::OnLogQueryButtonDelete() 
{
	m_ListLog.DeleteAllItems();	

	m_iPageCount	= 0;
	m_iCurrentPage	= 0;
	m_ulRecordCount = 0;

	SetPageLable();

	SetButton();
}

void CLogQuery::OnLogQueryButtonBack() 
{
	if(!GetDlgItem(IDC_LOG_QUERY_BUTTON_BACK)->IsWindowEnabled())
		return;

	SetButton(TRUE, TRUE);

	m_iCurrentPage --;

	if(m_iCurrentPage <= 1)
		SetButton(FALSE, TRUE);

	SetCursor(LoadCursor(NULL, IDC_WAIT));
	ShowRecordPage();
	SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
}

void CLogQuery::OnLogQueryButtonNext() 
{
	if(!GetDlgItem(IDC_LOG_QUERY_BUTTON_NEXT)->IsWindowEnabled())
		return;

	SetButton(TRUE, TRUE);
		
	m_iCurrentPage ++;

	if(m_iCurrentPage >= m_iPageCount)
		SetButton(TRUE, FALSE);

	SetCursor(LoadCursor(NULL, IDC_WAIT));
	ShowRecordPage();
	SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
}

void CLogQuery::SetButton(BOOL IsBack, BOOL IsNext)
{
	GetDlgItem(IDC_LOG_QUERY_BUTTON_BACK)->EnableWindow(IsBack);
	GetDlgItem(IDC_LOG_QUERY_BUTTON_NEXT)->EnableWindow(IsNext);
}

void CLogQuery::SetPageLable(DWORD ulRecordCount)
{
	CString		tmpStr;

	tmpStr.Format(
		GUI_LOG_QUERY_RESULT_LIST_LABLE, 
		m_iCurrentPage,
		m_iPageCount, 
		LOG_QUERY_PAGE_SIZE,
		ulRecordCount,
		m_ulRecordCount
		);

	SetDlgItemText(IDC_LOG_QUERY_RESULT_LIST_LABLE, tmpStr);

}

BOOL CLogQuery::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.WinHelp(5, HH_HELP_CONTEXT);

	return TRUE;
}
