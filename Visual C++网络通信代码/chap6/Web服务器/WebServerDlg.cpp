// WebServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebServer.h"
#include "WebServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebServerDlg dialog

CWebServerDlg::CWebServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebServerDlg)
	//初始化参数
	m_szHomeDir = _T("");
	m_szDefIndex = _T("");
	m_Port = 81;
	m_PTO = 10;
	m_szStatus = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebServerDlg)
	DDX_Control(pDX, IDC_VISITORS, m_nVisitors);
	DDX_Control(pDX, IDC_BYTESRECV, m_nBytesRecv);
	DDX_Control(pDX, IDC_BYTESENT, m_nBytesSent);
	DDX_Control(pDX, IDC_REQUESTS, m_nRequests);
	DDX_Control(pDX, IDC_ACTIVECONN, m_nActiveConn);
	DDX_Text(pDX, IDC_HOMEDIR, m_szHomeDir);
	DDX_Text(pDX, IDC_DEFINDEXFILE, m_szDefIndex);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDV_MinMaxInt(pDX, m_Port, 1, 65535);
	DDX_Text(pDX, IDC_PTO, m_PTO);
	DDV_MinMaxInt(pDX, m_PTO, 0, 10000);
	DDX_Text(pDX, IDC_STATUS, m_szStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWebServerDlg, CDialog)
	//{{AFX_MSG_MAP(CWebServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_HOMEDIRBROWSE, OnHomedirbrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebServerDlg message handlers

BOOL CWebServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_bRun = FALSE;
	//保存设置
	RestoreSettings();
	//启动服务
	OnStart();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWebServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWebServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWebServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




//启动服务子程序
void CWebServerDlg::OnStart()
{
	//如果已经在运行
	if(m_bRun)
	{
		AfxMessageBox("服务器已经在运行");
		return;
	}

	UpdateData();

	//否则启动服务
	m_bRun = WebServer.Start(LPCTSTR(m_szHomeDir), LPCTSTR(m_szDefIndex), m_Port, m_PTO * 1000);
	if(m_bRun)
		nTimerID = SetTimer(TIMER_ID_1, TIMER_TO_1, NULL);
	m_szStatus = "程序服务中...";
	UpdateData(FALSE);
}





//停止服务
void CWebServerDlg::OnStop() 
{
	if(m_bRun)
	{
		//调用Shutdown来关闭服务
		BOOL bResult = WebServer.Shutdown();
		KillTimer(nTimerID);
		m_bRun = FALSE;
		m_szStatus = "目前没有提供服务.";
		UpdateData(FALSE);
		return;
	}
	AfxMessageBox("服务没有启动");
}





//退出程序
void CWebServerDlg::OnClose() 
{
	
	//保存设置
	SaveSettings();
	OnStop();
	CDialog::OnClose();
}






void CWebServerDlg::OnOK() 
{
	SaveSettings();
	OnStop();
	CDialog::OnOK();
}



//定时器处理程序,主要是显示服务器数据
void CWebServerDlg::OnTimer(UINT nIDEvent) 
{
	StatisticsTag st;
	CString szTemp;
	
	WebServer.GetStats(st);

	szTemp.Format("%d", st.nClientsConnected);
	m_nActiveConn.SetWindowText(szTemp);

	szTemp.Format("%.1f", st.nTotalRecv);
	m_nBytesRecv.SetWindowText(szTemp);
	
	szTemp.Format("%.1f", st.nTotalSent);
	m_nBytesSent.SetWindowText(szTemp);

	szTemp.Format("%d", st.nTotalHits);
	m_nRequests.SetWindowText(szTemp);

	szTemp.Format("%d", st.nVisitors);
	m_nVisitors.SetWindowText(szTemp);

	CDialog::OnTimer(nIDEvent);
}


//重新启动服务
void CWebServerDlg::OnReset() 
{
	OnStop();
	WebServer.Reset();
	UpdateData();
	OnStart();
}


//设置服务根目录
void CWebServerDlg::OnHomedirbrowse() 
{
	BROWSEINFO	bi;
	char folder_name[MAX_PATH];
	char dir_name[MAX_PATH];
	LPMALLOC lpMalloc; 
	
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = folder_name;
	bi.lpszTitle = "请选择目录";
	bi.ulFlags = BIF_EDITBOX | BIF_STATUSTEXT;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if(pidl)
	{
		SHGetPathFromIDList(pidl, dir_name);
		m_szHomeDir = dir_name;
		UpdateData(FALSE);
	}
	
	if(!SHGetMalloc(&lpMalloc) && (lpMalloc != NULL)) 
	{ 
		if(pidl != NULL) 
		{ 
			lpMalloc->Free(pidl); 
		}  
		lpMalloc->Release(); 
	} 
}


//保存设置
void CWebServerDlg::SaveSettings()
{
	CWinApp* pApp = AfxGetApp();
	
	UpdateData();
	pApp->WriteProfileString("Settings", "Server Root", m_szHomeDir);
	pApp->WriteProfileString("Settings", "Defindex", m_szDefIndex);
	pApp->WriteProfileInt("Settings", "Port", m_Port);
	pApp->WriteProfileInt("Settings", "PTO", m_PTO);
}


//获取设置
void CWebServerDlg::RestoreSettings()
{
	CWinApp* pApp = AfxGetApp();
	//默认服务器根目录是"C:\\ServerRoot"
	m_szHomeDir = pApp->GetProfileString("Settings", "Server Root", "C:\\ServerRoot");
	m_szDefIndex = pApp->GetProfileString("Settings", "Defindex", "index.htm");
	m_Port = pApp->GetProfileInt("Settings", "Port", 80);
	m_PTO = pApp->GetProfileInt("Settings", "PTO", 10);
	UpdateData(FALSE);
}
