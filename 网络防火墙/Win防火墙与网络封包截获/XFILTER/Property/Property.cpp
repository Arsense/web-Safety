// Property.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Property.h"

#include "GuiRes.h"
#include ".\MainFrame\MainFrame.h"
#include ".\MainSheet.h"
#include "AclSet.h"
#include "NetTimeSheet.h"
#include "NetIPAria.h"
#include "Acl.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// The one and only CPropertyApp object

CPropertyApp			theApp;
CAclFile				m_AclFile;
extern CMainSheet		*dlg;
extern CAclSet			*dlgAclSet;
extern CNetTimeSheet	*dlgNetTimeSheet;
extern CFileDialog		*dlgFile;
extern CNetIPAria		*dlgNetIPAria;
extern CSystemTray		m_TrayIcon;

//=============================================================================================
// CPropertyApp

BEGIN_MESSAGE_MAP(CPropertyApp, CWinApp)
	//{{AFX_MSG_MAP(CPropertyApp)
	ON_COMMAND(ID_APP_EXIT,		OnAppExit)
	ON_COMMAND(ID_ABOUT,		OnAbout)
	ON_COMMAND(ID_ACL_SET,		OnAclSet)
	ON_COMMAND(ID_LOG_QUERY,	OnLogQuery)
	ON_COMMAND(ID_SYSTEM_SET,	OnSystemSet)
	ON_UPDATE_COMMAND_UI(ID_ABOUT,			OnUpdateAbout)
	ON_UPDATE_COMMAND_UI(ID_ACL_SET,		OnUpdateAclSet)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT,		OnUpdateAppExit)
	ON_UPDATE_COMMAND_UI(ID_LOG_QUERY,		OnUpdateLogQuery)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_SET,		OnUpdateSystemSet)
	ON_COMMAND(ID_CONTROL_FRAME,			OnControlFrame)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_FRAME,	OnUpdateControlFrame)
	ON_COMMAND(ID_PACKET_MONITOR,			OnPacketMonitor)
	ON_UPDATE_COMMAND_UI(ID_PACKET_MONITOR, OnUpdatePacketMonitor)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//=============================================================================================
// CPropertyApp construction

CPropertyApp::CPropertyApp()
{
	m_EnablePacketMonitor	= TRUE;
	m_EnableQueryLog		= TRUE;
	m_EnableAcl				= TRUE;
	m_EnableSystemSet		= TRUE;
	m_EnableAbout			= TRUE;
	m_EnableExit			= TRUE;
	m_EnableContolFrame		= TRUE;

	m_XfIoControl			= NULL;
}

//=============================================================================================
// CPropertyApp initialization

BOOL CPropertyApp::InitInstance()
{
	TCHAR sPathName[MAX_PATH];
	_stprintf(sPathName, _T("%s%s"), m_AclFile.GetAppPath(), XFILTER_SERVICE_DLL_NAME);

	TCHAR sTemp[MAX_PATH];
	_stprintf(sTemp, _T("%s%s"), m_AclFile.GetAppPath(), XFILTER_HELP_FILE_NAME);
	theApp.m_pszHelpFilePath = _tcsdup(sTemp);

	if(_tcscmp(theApp.m_lpCmdLine, _T("-install")) == 0)
	{
		int iRet;
		if((iRet = m_Install.InstallProvider(sPathName)) != XERR_SUCCESS
			&& iRet != XERR_PROVIDER_ALREADY_INSTALL)
			AfxMessageBox(GUI_ACL_MESSAGE_INSTALL_FAILED);
		SetAutoStart(FALSE);
		m_RegisterRequest.UserReg();
		return FALSE;
	}

	if(_tcscmp(theApp.m_lpCmdLine, _T("-remove")) == 0)
	{
		m_Install.RemoveProvider();
		m_Install.DeleteReg(HKEY_LOCAL_MACHINE, REG_AUTO_START_KEY, REG_AUTO_START_ITEM);
		return FALSE;
	}

	if(!m_Install.IsWinsock2())
	{
		AfxMessageBox(GUI_ACL_MESSAGE_NO_WINSOCK2);
		return FALSE;
	}

	CWnd *PrevCWnd = CWnd::FindWindow(NULL, GUI_APP_CAPTION);

	if (PrevCWnd != NULL) 
		return FALSE;

	if(m_AclFile.ReadAcl() != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ACL_READ_ERROR);
		return FALSE;
	}

	if(m_AclFile.GetBit(m_AclFile.mAclHeader.bSet, 6))
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}

	if(!theApp.m_RegisterRequest.InitRegister())
		return FALSE;

	if(!m_Install.IsInstalled(sPathName) && m_Install.InstallProvider(sPathName) != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INSTALL_FAILED);
		return FALSE;
	}

	if ((m_hTcpIpDog = LoadLibrary(sPathName)) == NULL)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_DLL_NOT_FOUND);
		return FALSE;
	}

	m_XfIoControl	= (XF_IO_CONTROL)GetProcAddress(m_hTcpIpDog, _T("XfIoControl"));

	if (m_XfIoControl == NULL)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_FUNCTION_NOT_FOUNT);
		return FALSE;
	}
	
	if(!InitDllData())
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR);
		return FALSE;
	}
		
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	pFrame->Create(NULL,GUI_APP_CAPTION);
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();

	

	XFILTER_IO_CONTROL ioControl;
	ioControl.hwnd = pFrame->m_hWnd;
	m_XfIoControl(IO_CONTROL_SET_GUI_INSTANCE, &ioControl);

	return TRUE;
}

BOOL CPropertyApp::InitDllData()
{
	if(m_XfIoControl == NULL)
		return FALSE;

	XFILTER_IO_CONTROL ioControl;

	if((ioControl.iWorkMode = CAclFile::GetBit(m_AclFile.mAclHeader.bSet, 4, 2)) != XF_QUERY_ALL)
	{
		if(m_XfIoControl(IO_CONTROL_SET_WORK_MODE, &ioControl) != XERR_SUCCESS)
			return FALSE;

		SetTrayIcon();
		return TRUE;
	}

	ioControl.AclFile.mAclHeader		= m_AclFile.mAclHeader;
	ioControl.AclFile.mAclIntranetIP	= m_AclFile.mAclIntranetIP;
	memcpy(ioControl.AclFile.mAclTime	, m_AclFile.mAclTime, ACL_TIME_LENTH * ACL_HEADER_TIME_COUNT) ;
	memcpy(ioControl.AclFile.mpAcl		, m_AclFile.mpAcl	, ACL_ACL_LENTH * m_AclFile.mAclHeader.ulAclCount);
	memcpy(ioControl.AclFile.mpAclCustomIP		, m_AclFile.mpAclCustomIP	, ACL_IP_LENTH * m_AclFile.mAclHeader.ulCustomIPCount);
	memcpy(ioControl.AclFile.mpAclDistrustIP	, m_AclFile.mpAclDistrustIP	, ACL_IP_LENTH * m_AclFile.mAclHeader.ulDistrustIPCount);
	memcpy(ioControl.AclFile.mpAclTrustIP		, m_AclFile.mpAclTrustIP	, ACL_IP_LENTH * m_AclFile.mAclHeader.ulTrustIPCount);

	if(m_XfIoControl(IO_CONTROL_SET_ACL, &ioControl) != XERR_SUCCESS)
		return FALSE;

	SetTrayIcon();
	return TRUE;
}

void CPropertyApp::ExitApp()
{
	if(CheckDlg()) 
		return;

	if(dlg != NULL)
	{
		if(dlg->GetDlgItem(ID_APPLY_NOW)->IsWindowEnabled())
			dlg->OnCancel();

		dlg->EndDialog(IDCANCEL);
	}

	m_pMainWnd->DestroyWindow();
}

/*---------------------------------------------------------------------------------------------
	Return Value: 
		TRUE:	had some sub_dialogs visible 
		FALSE:	no sub_dialogs visible
*/
BOOL CPropertyApp::CheckDlg(BOOL ShowMessage)
{
	if(	CheckDlgVisible		(dlgNetIPAria	, ShowMessage)
		|| CheckDlgVisible	(dlgNetTimeSheet, ShowMessage)
		|| CheckDlgVisible	(dlgFile		, ShowMessage)
		|| CheckDlgVisible	(dlgAclSet		, ShowMessage)
		)
		return TRUE;

	return FALSE;
}
BOOL CPropertyApp::CheckDlgVisible(void* pdlg, BOOL ShowMessage)
{
	if(pdlg == NULL) 
		return FALSE;

	CDialog* mdlg = (CDialog*)pdlg;

	if(ShowMessage)
		AfxMessageBox(GUI_ACL_MESSAGE_PLEASE_CLOSE_SUB_WINDOW);

	mdlg->SetFocus();

	return TRUE;
}

void CPropertyApp::OnAppExit() 
{
	if(!m_EnableExit) 
		return;

	ExitApp();
}

void CPropertyApp::OnPacketMonitor() 
{
	if(!m_EnablePacketMonitor)
		return;

	CMainFrame::InitMainSheet();
}

void CPropertyApp::OnLogQuery() 
{
	if(!m_EnableQueryLog) 
		return;

	CMainFrame::InitMainSheet(1);
}

void CPropertyApp::OnAclSet() 
{
	if(!m_EnableAcl) 
		return;

	CMainFrame::InitMainSheet(2);
}

void CPropertyApp::OnSystemSet() 
{
	if(!m_EnableSystemSet) 
		return;

	CMainFrame::InitMainSheet(3);
}

void CPropertyApp::OnAbout() 
{
	if(!m_EnableAbout) 
		return;

	CMainFrame::InitMainSheet(4);
}

void CPropertyApp::OnControlFrame() 
{
	if(!m_EnableContolFrame) 
		return;

	if(CheckDlg(FALSE))	
		return;

	CMainFrame::InitMainSheet(-1);
}

void CPropertyApp::OnUpdateAbout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnableAbout);
	pCmdUI->SetText(GUI_MENU_ABOUT);
}

void CPropertyApp::OnUpdateAclSet(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnableAcl);
	pCmdUI->SetText(GUI_MENU_ACL);
}

void CPropertyApp::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnableExit);
	pCmdUI->SetText(GUI_MENU_EXIT);
}

void CPropertyApp::OnUpdateLogQuery(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnableQueryLog);
	pCmdUI->SetText(GUI_MENU_LOG_QUERY);
}

void CPropertyApp::OnUpdatePacketMonitor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnablePacketMonitor);
	pCmdUI->SetText(GUI_MENU_PACKET_MONITOR);
}

void CPropertyApp::OnUpdateSystemSet(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnableSystemSet);
	pCmdUI->SetText(GUI_MENU_SYSTEM_SET);
}

void CPropertyApp::OnUpdateControlFrame(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EnableContolFrame);
	pCmdUI->SetText(GUI_MENU_CONTROL_FRAME);
}

int CPropertyApp::ExitInstance() 
{
	ODS(_T("XFILTER.EXE: ExitInstance Starting..."));

	m_TrayIcon.HideIcon();

 	if(m_XfIoControl != NULL)
	{
		XFILTER_IO_CONTROL	ioControl;
		ioControl.iWorkMode = XF_PASS_ALL;
		ioControl.hwnd		= NULL;

		m_XfIoControl(IO_CONTROL_SET_WORK_MODE, &ioControl);
		m_XfIoControl(IO_CONTROL_SET_GUI_INSTANCE, &ioControl);
	}

	if(m_hTcpIpDog != NULL)
		FreeLibrary(m_hTcpIpDog);

	m_hTcpIpDog	= NULL;

	while(m_DownloadCommand.m_IsConnecting || m_RegisterRequest.m_IsConnecting)
		Sleep(1000);

	ODS(_T("XFILTER.EXE: ExitInstance Over..."));
	return CWinApp::ExitInstance();
}

BOOL CPropertyApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CPropertyApp::SetAutoStart(BOOL IsCheckFromAcl)
{
	BOOL IsCheck	= TRUE;

	if(IsCheckFromAcl)
		IsCheck = m_AclFile.GetBit(m_AclFile.mAclHeader.bSet, 1);

	if(IsCheck)
	{
		TCHAR tmpStr[MAX_PATH];
		strcpy(tmpStr, m_AclFile.GetAppPath(FALSE, NULL, TRUE));

		m_Install.SaveReg(REG_AUTO_START_ITEM, (BYTE*)tmpStr, _tcslen(tmpStr)
			, HKEY_LOCAL_MACHINE, REG_AUTO_START_KEY, REG_SZ);
		return;
	}
	
	m_Install.DeleteReg(HKEY_LOCAL_MACHINE, REG_AUTO_START_KEY, REG_AUTO_START_ITEM);
}

void CPropertyApp::SetTrayIcon()
{
	int iWorkMode = XF_PASS_ALL;

	if(m_XfIoControl != NULL)
		iWorkMode = m_XfIoControl(IO_CONTROL_GET_WORK_MODE, NULL);

	if(iWorkMode == XF_PASS_ALL)
		m_TrayIcon.SetIcon(IDI_SMALL_PASS);
	else if(iWorkMode == XF_QUERY_ALL)
		m_TrayIcon.SetIcon(IDI_SMALL_QUERY);
	else if(iWorkMode == XF_DENY_ALL)
		m_TrayIcon.SetIcon(IDI_SMALL_DENY);
	else
		m_TrayIcon.SetIcon(IDI_SMALL_PASS);
}

void CPropertyApp::WinHelp(DWORD dwData, UINT nCmd) 
{	
	HWND hHtmlHelp = HtmlHelp(
		theApp.m_pMainWnd->m_hWnd, 
		theApp.m_pszHelpFilePath, 
		nCmd, 
		dwData
		);

	if(hHtmlHelp == 0)
		AfxMessageBox(GUI_MESSAGE_OPEN_HELP_FAILED);
}
