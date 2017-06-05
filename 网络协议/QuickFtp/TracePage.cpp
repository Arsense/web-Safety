// TracePage.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "TracePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTracePage dialog


CTracePage::CTracePage(CWnd* pParent /*=NULL*/)
	: CDialog(CTracePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTracePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTracePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTracePage)
	DDX_Control(pDX, IDC_TRACE, m_TraceList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTracePage, CDialog)
	//{{AFX_MSG_MAP(CTracePage)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TRACE_CLEAR, OnTraceClear)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDTRACELINE, OnAddTraceLine)//自定义消息响应!!!!!!!!!!!!!
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTracePage message handlers

void CTracePage::AddTraceLine(int nLevel, LPCTSTR pstrFormat, ...)
{
	CString str;

	//format and write the data we were given
	va_list args;
	va_start(args,pstrFormat);
	str.FormatV(pstrFormat,args);

	try
	{
		//create new message
		CLogMsg *pLogMsg = new CLogMsg;

		GetLocalTime(&pLogMsg->m_sysTime);
		pLogMsg->m_nLevel = nLevel;
		pLogMsg->m_strText = str;

		m_QueueLock.Lock();
		m_LogQueue.AddTail(pLogMsg);
		m_QueueLock.Unlock();

		//schedule log action
		PostMessage(WM_ADDTRACELINE);
	}
	catch (...)
	{
	}
}

LRESULT CTracePage::OnAddTraceLine(WPARAM, LPARAM)
{
	CLogMsg *pLogMsg;

	try
	{
		//get first message in the queue
		pLogMsg = (CLogMsg*)m_LogQueue.RemoveHead();

		switch(pLogMsg->m_nLevel)
		{
			case 1:
				m_TraceList.AddString(pLogMsg->m_strText,RGB(0,130,0));
			    break;
			case 2:
				m_TraceList.AddString(pLogMsg->m_strText,RGB(0,0,255));
			    break;
			case 3:
				m_TraceList.AddString(pLogMsg->m_strText,RGB(255,0,0));
			    break;
			default:
				m_TraceList.AddString(pLogMsg->m_strText,RGB(0,0,0));
				break;
		}
		delete pLogMsg;
	}
	catch (...)
	{
		//something is wrong...
	}
	return TRUE;
}

void CTracePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_TRACE)))
	{
		CRect rect;
		GetClientRect(rect);
		m_TraceList.MoveWindow(rect);
	}	
}

//点击右键弹出菜单(清除)
//WM_CONTEXTMENU 的响应函数
//This message notifies a window that the user clicked the right mouse button (right-clicked) in the window.
void CTracePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_TRACE_MENU));
	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);	
}

//点击右键弹出菜单(清除)，点击清除菜单项的响应函数在工程中无法添加，只能自己手工添加了
void CTracePage::OnTraceClear() 
{
	m_TraceList.ResetContent();	
}
