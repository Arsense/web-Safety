// MainFrame.cpp : implementation file
//

#include "stdafx.h"
#include "..\Property.h"
#include "MainFrame.h"
#include "..\MainSheet.h"
#include "..\..\common\debug.h"
#include "..\GuiRes.h"
#include "..\MainSheet.h"
#include "..\..\common\xlogfile.h"
#include "..\Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CMainFrame

extern CPropertyApp theApp;
extern CAclFile		m_AclFile;
CMainSheet			*dlg = NULL;
CSystemTray			m_TrayIcon;
BOOL				m_IsSplash		= FALSE;
int					m_MessageIndex	= 0;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICON_NOTIFY,     OnTrayNotification)
	ON_MESSAGE(WM_SESSION_NOTIFY,  OnSessionNotification)
	ON_MESSAGE(WM_QUERY_ACL_NOTIFY,  OnAclNotification)
	ON_MESSAGE(WM_NET_MESSAGE,  OnShowNetMessage)
END_MESSAGE_MAP()

//=============================================================================================
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_TrayIcon.Create(this, WM_ICON_NOTIFY, GUI_APP_CAPTION, theApp.LoadIcon(IDI_SMALL_PASS), IDR_MAINFRAME))
		return -1;

	theApp.SetTrayIcon();

	if(!m_TrayIcon.SetMenuDefaultItem(2,TRUE))
		return -1;
	
	DWORD dwFlags;
	if(InternetGetConnectedState(&dwFlags, 0))
		theApp.m_DownloadCommand.InitNetCommand();
	else
		SetTimer(1, 600000, NULL);

	if(!theApp.m_RegisterRequest.m_IsUploaded)
		SetTimer(2, 300000, NULL);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	return 0;
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDR_MAINFRAME && lParam == WM_LBUTTONDOWN)
	{
		if(m_MessageIndex != 0)
		{
			int tmpIndex = m_MessageIndex;
			m_MessageIndex = 0;
			AfxMessageBox(theApp.m_sMessage[tmpIndex], MB_ICONINFORMATION);
			return 0;
		}

		lParam = WM_LBUTTONDBLCLK;
	}

    return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CMainFrame::InitMainSheet(int iActivePage, int IsShow)
{
	theApp.SetTrayIcon();

	if(dlg != NULL)
	{
		if(iActivePage >= 0)
			dlg->SetActivePage(iActivePage);

		if(!dlg->IsWindowVisible())
			dlg->ShowWindow(SW_SHOW);
		
		dlg->SetForegroundWindow();
		return;
	}

	dlg = new CMainSheet(GUI_APP_CAPTION);
	if(iActivePage >= 0)
		dlg->SetActivePage(iActivePage);
	dlg->DoModal();
	delete dlg;
	dlg = NULL;

	ODS(_T("XFILTER: release dlg = null"))
}

LRESULT CMainFrame::OnSessionNotification(UINT wParam, LONG lParam)
{
	static CXLogFile m_LogFile;

	if(theApp.m_XfIoControl == NULL)
		return XERR_SUCCESS;

	XFILTER_IO_CONTROL	ioControl;
	ioControl.isession	= wParam;
	ioControl.session	= new SESSION;

	theApp.m_XfIoControl(IO_CONTROL_GET_SESSION, &ioControl);

	if(ioControl.session->s == 0)
		return XERR_SUCCESS;

	CPacketMonitor::InsertOneList(
		CPacketMonitor::m_pListPacketMonitor, 
		ioControl.session,
		CPacketMonitor::IsMonitor,
		CPacketMonitor::IsScroll
		);
	
	if(CAclFile::GetBit(m_AclFile.mAclHeader.bSet, 0))
		m_LogFile.AddLog(ioControl.session, m_AclFile.mAclHeader.uiLogSize);

	if(ioControl.session->bAction == ACL_ACTION_DENY)
	{	
		if(CAclFile::GetBit(m_AclFile.mAclHeader.bSet, 2))
			MessageBeep(0xFFFFFFFF);

		if(!m_IsSplash && CAclFile::GetBit(m_AclFile.mAclHeader.bSet, 3))
		{
			DWORD		dwThreadId;
			::CreateThread(NULL, 0, SplashIcon, 0, 0, &dwThreadId);
		}
	}

	delete(ioControl.session);

	return XERR_SUCCESS;
}

LRESULT CMainFrame::OnAclNotification(UINT wParam, LONG lParam)
{
	if(wParam < 0 || wParam > MAX_QUERY_SESSION)
		return XERR_INVALID_PARAMETER;

	XFILTER_IO_CONTROL ioControl;

	ioControl.isession = wParam;

	theApp.m_XfIoControl(IO_CONTROL_GET_QUERY_SESSION, &ioControl);

	//
	// v1.0.2 2001-12-25 modify
	//
	if(CAcl::AddAclQuery(ioControl.sPathName, FALSE))
		ioControl.iWorkMode = 1; //TRUE
	else
		ioControl.iWorkMode = 0; //FALSE
	
	theApp.m_XfIoControl(IO_CONTROL_SET_QUERY_SESSION, &ioControl);

	return XERR_SUCCESS;
}

DWORD WINAPI SplashIcon(LPVOID pVoid)
{
	m_IsSplash = TRUE;

	for(int i = 0; i < 3; i++)
	{
		m_TrayIcon.SetIcon(IDI_SMALL_ALERT);
		if(i == 2)
			continue;
		Sleep(180);
		m_TrayIcon.SetIcon(IDI_SMALL_NULL);
		Sleep(400);
	}
	
	m_IsSplash = FALSE;

	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		DWORD dwFlags;
		if(InternetGetConnectedState(&dwFlags, 0))
		{
			theApp.m_DownloadCommand.InitNetCommand();
			KillTimer(nIDEvent);
		}
	}
	else if(nIDEvent == 2)
	{
		if(theApp.m_DownloadCommand.PreUpload())
			KillTimer(nIDEvent);
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnShowNetMessage(UINT wParam, LONG lParam)
{
	if(wParam <= 0 || wParam > MAX_NET_COMMAND)
		return XERR_INVALID_PARAMETER;

 	ODS(theApp.m_sMessage[wParam]);
	
	static DWORD	dwThreadId = NULL;
	if(dwThreadId == NULL)
		::CreateThread(NULL, 0, SplashMessage, 0, 0, &dwThreadId);

	return XERR_SUCCESS;
}

DWORD WINAPI SplashMessage(LPVOID pVoid)
{
	for(int i = 0; i < MAX_NET_COMMAND; i++)
	{
		if(theApp.m_sMessage[i][0] != '\0')
		{
			m_MessageIndex = i;

			m_IsSplash = TRUE;
			while(m_MessageIndex)
			{
				m_TrayIcon.SetIcon(IDI_SMALL_MESSAGE);
				Sleep(180);
				m_TrayIcon.SetIcon(IDI_SMALL_NULL);
				Sleep(400);
			}
			theApp.SetTrayIcon();
			m_IsSplash = FALSE;
		}
	}
	
	ODS("Exit Thread SplashMessage...");
	return 0;
}
