// CSocketcliDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSocketcli.h"
#include "CSocketcliDlg.h"
#include <io.h>

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
// CCSocketcliDlg dialog

CCSocketcliDlg::CCSocketcliDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSocketcliDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSocketcliDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCSocketcliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSocketcliDlg)
	DDX_Control(pDX, IDC_BUTTON3, m_disconnect);
	DDX_Control(pDX, IDC_BUTTON2, m_connect);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCSocketcliDlg, CDialog)
	//{{AFX_MSG_MAP(CCSocketcliDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSocketcliDlg message handlers

BOOL CCSocketcliDlg::OnInitDialog()
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
	m_edit.SetLimitText(99);
	m_list.InsertColumn(0,"消息");
	m_list.SetColumnWidth(0,435);
	m_button1.EnableWindow(FALSE);
	m_disconnect.EnableWindow(FALSE);

	m_edit2.SetWindowText("192.168.0.3");
	count=0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSocketcliDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCSocketcliDlg::OnPaint() 
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
HCURSOR CCSocketcliDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCSocketcliDlg::OnOK() 
{
	
//	CDialog::OnOK();
}

//发送信息
void CCSocketcliDlg::OnButton1() 
{
	char buff[100];
	CSize size;
	size.cx=0;
	size.cy=30;
	//获得发送信息
	m_edit.GetWindowText(buff,99);
	m_edit.SetWindowText("");
	m_list.InsertItem(count++,buff);
	m_list.Scroll(size);
	//发送数据
	send(clisock,buff,100,0);
}


//线程
UINT thread(LPVOID v)
{
	char buff[100];
	char array[25][30]=
	{"155.245.160.151",
	 "155.245.160.152",
	 "155.245.160.153",
	 "155.245.160.154",
	 "155.245.160.155",
	 "155.245.160.156",
	 "155.245.160.157",
	 "155.245.160.158",
	 "155.245.160.159",
	 "155.245.160.160",
	 "155.245.160.161",
	 "155.245.160.162",
	 "155.245.160.163",
	 "155.245.160.164",
	 "155.245.160.165",
	 "155.245.160.166",
	 "155.245.160.167",
	 "155.245.160.168",
	 "155.245.160.169",
	 "155.245.160.170",
	 "155.245.160.171",
	 "155.245.160.172",
	 "155.245.160.173",
	 "155.245.160.174",
	 "155.245.160.175"};
	CSize size;
	size.cx=0;
	size.cy=30;
	int s=1,addcount=0;
	CCSocketcliDlg *dlg=(CCSocketcliDlg*) AfxGetApp()->GetMainWnd();
	dlg->m_connect.EnableWindow(FALSE);
	dlg->m_disconnect.EnableWindow(TRUE);
	//连接到服务器
	while(connect(dlg->clisock,(sockaddr*)&(dlg->cli),sizeof(dlg->cli)) && dlg->ee!=0)
	{
		dlg->m_edit.SetWindowText("等待.....");
		//空循环
		for (int i=0;i<=65000;i++)
			for(int j=0;j<=200;j++);
		if (addcount==25)
			addcount=0;
		dlg->cli.sin_addr.s_addr=inet_addr(array[addcount++]);
	}
		
		if (dlg->ee==1)
		dlg->m_list.InsertItem(dlg->count++,"连接成功");
		dlg->m_button1.EnableWindow(TRUE);	
	    dlg->SetForegroundWindow();

	//循环获得数据
	while(s!=SOCKET_ERROR && dlg->ee!=0)
	{
		//调用recv函数接收数据
		s=recv(dlg->clisock,buff,100,0);
	    dlg->SetForegroundWindow();
		if (s!=SOCKET_ERROR && dlg->ee!=0)
		dlg->m_list.InsertItem(dlg->count++,buff);
		dlg->m_list.Scroll(size);
	}
	//发送断开命令
	send(dlg->clisock,"Disconnected",100,0);
	dlg->m_button1.EnableWindow(FALSE);
	dlg->m_connect.EnableWindow(TRUE);
	dlg->m_disconnect.EnableWindow(FALSE);
	closesocket(dlg->clisock);
	AfxEndThread(0);
	return 0;
}
CCSocketcliDlg::~CCSocketcliDlg()
{
	send(clisock,"Disconnected",100,0);
}

//连接服务器
void CCSocketcliDlg::OnButton2() 
{
	char ipaddress[35];
	m_edit2.GetWindowText(ipaddress,30);
	cli.sin_addr.s_addr=inet_addr(ipaddress);
	cli.sin_family=AF_INET;
	cli.sin_port=5000;//htons(5000);
	//创建socket
	clisock=socket(AF_INET,SOCK_STREAM,0);	
	ee=1;
	//启动线程
	AfxBeginThread(thread,0);
	
}

void CCSocketcliDlg::OnDisconnect() 
{
	ee=0;
	closesocket(clisock);
}
