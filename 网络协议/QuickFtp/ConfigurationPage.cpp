// ConfigurationPage.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "ConfigurationPage.h"
#include "FTPServerApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage dialog


CConfigurationPage::CConfigurationPage(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigurationPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigurationPage)
	m_nPort = 0;
	m_nMaxUsers = 0;
	m_strGoodbyeMessage = _T("");
	m_bStartInTray = FALSE;
	m_nTimeout = 0;
	m_strWelcomeMessage = _T("");
	m_bLaunchOnStartup = FALSE;
	m_bAutoActivate = FALSE;
	m_nLogLevel = 0;
	m_strApplicationName = _T("");
	//}}AFX_DATA_INIT
}


void CConfigurationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigurationPage)
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDX_Text(pDX, IDC_MAX_USERS, m_nMaxUsers);
	DDX_Text(pDX, IDC_GOODBYE_MSG, m_strGoodbyeMessage);
	DDX_Check(pDX, IDC_START_IN_TRAY, m_bStartInTray);
	DDX_Text(pDX, IDC_TIMEOUT, m_nTimeout);
	DDX_Text(pDX, IDC_WELCOME_MSG, m_strWelcomeMessage);
	DDX_Check(pDX, IDC_LAUNCH_ON_STARTUP, m_bLaunchOnStartup);
	DDX_Check(pDX, IDC_AUTOACTIVATE, m_bAutoActivate);
	DDX_CBIndex(pDX, IDC_LOGLEVEL, m_nLogLevel);
	DDX_Text(pDX, IDC_APPLICATIONNAME, m_strApplicationName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigurationPage, CDialog)
	//{{AFX_MSG_MAP(CConfigurationPage)
	ON_BN_CLICKED(IDC_LAUNCH_ON_STARTUP, OnLaunchOnStartup)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_VIEW, OnView)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_START_IN_TRAY, OnStartInTray)
	ON_BN_CLICKED(IDC_AUTOACTIVATE, OnAutoactivate)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage message handlers

//保存配置信息
void CConfigurationPage::OnApply() 
{
	UpdateData();//这一句不能少，不然数据控件上的数据不能很快的和绑定的变量值绑定到一起

	// save settings
	AfxGetApp()->WriteProfileInt("Settings", "Port", m_nPort);
	AfxGetApp()->WriteProfileInt("Settings", "MaxUsers", m_nMaxUsers);
	AfxGetApp()->WriteProfileInt("Settings", "Timeout", m_nTimeout);
	AfxGetApp()->WriteProfileString("Settings", "WelcomeMsg", m_strWelcomeMessage); 
	AfxGetApp()->WriteProfileString("Settings", "GoodbyeMsg", m_strGoodbyeMessage); 
	AfxGetApp()->WriteProfileInt("Settings", "LaunchOnStartup", m_bLaunchOnStartup);
	AfxGetApp()->WriteProfileInt("Settings", "StartInTray", m_bStartInTray);
	AfxGetApp()->WriteProfileInt("Settings", "AutoActivate", m_bAutoActivate);
	AfxGetApp()->WriteProfileInt("Settings", "LogLevel", m_nLogLevel);
	AfxGetApp()->WriteProfileString("Settings", "AppName", m_strApplicationName);
}

extern CFTPServerApp theApp;

//查看日志文件内容
void CConfigurationPage::OnView() 
{
    int ExecStatus = (UINT)ShellExecute(m_hWnd, "open", "NOTEPAD.EXE", (LPCTSTR)theApp.m_LogFile.m_FileName, 0, SW_SHOW);
    switch(ExecStatus)
    {
        case 0:
            MessageBox("Systeem is out of Memory","ShellExecute Error",MB_OK | MB_ICONEXCLAMATION);
            break;
        case 2:
            MessageBox("'NOTEPAD.EXE' is not found !","ShellExecute Error",MB_OK | MB_ICONEXCLAMATION);
            break;
        default:
            if (ExecStatus <= 32)
                MessageBox("Problem with executing of NOTEPAD.EXE","ShellExecute Error",MB_OK | MB_ICONEXCLAMATION);
            break;
    }
//ShellExecute运行一个外部程序，并对外部程序有一定的控制！
}


//清除日志文件内容
void CConfigurationPage::OnClear() 
{
	theApp.m_LogFile.Clear();	
}

//在Windows启动时启动服务器
void CConfigurationPage::OnLaunchOnStartup() 
{	
	UpdateData(TRUE);
	
	if (m_bLaunchOnStartup)
	{
		char szModuleFilename[MAX_PATH];
		
		// Get application's full path.
		::GetModuleFileName(NULL, szModuleFilename, MAX_PATH);
		// create shortcut
		//CreateStartMenuShortcut(szModuleFilename, "FTP Server", "Startup");
	}
	else
	{
		// remove shortcut
		//RemoveStartMenuShortcut("FTP Server", "Startup");
	}	
	//OnSomethingChanged();
}

//启动时最小化到系统托盘
void CConfigurationPage::OnStartInTray() 
{
	
}

//服务器在启动时自动激活
void CConfigurationPage::OnAutoactivate() 
{
	
}
					
//在WM_DESTROY消息发生时被框架调用
void CConfigurationPage::OnDestroy() 
{
	UpdateData();//这一句不能少，不然数据控件上的数据不能很快的和绑定的变量值绑定到一起

	// save settings
	AfxGetApp()->WriteProfileInt("Settings", "Port", m_nPort);
	AfxGetApp()->WriteProfileInt("Settings", "MaxUsers", m_nMaxUsers);
	AfxGetApp()->WriteProfileInt("Settings", "Timeout", m_nTimeout);
	AfxGetApp()->WriteProfileString("Settings", "WelcomeMsg", m_strWelcomeMessage); 
	AfxGetApp()->WriteProfileString("Settings", "GoodbyeMsg", m_strGoodbyeMessage); 
	AfxGetApp()->WriteProfileInt("Settings", "LaunchOnStartup", m_bLaunchOnStartup);
	AfxGetApp()->WriteProfileInt("Settings", "StartInTray", m_bStartInTray);
	AfxGetApp()->WriteProfileInt("Settings", "AutoActivate", m_bAutoActivate);
	AfxGetApp()->WriteProfileInt("Settings", "LogLevel", m_nLogLevel);
	AfxGetApp()->WriteProfileString("Settings", "AppName", m_strApplicationName);

	CDialog::OnDestroy();	
}
