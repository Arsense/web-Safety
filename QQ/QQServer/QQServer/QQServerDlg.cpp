
// QQServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQServer.h"
#include "QQServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  WM_SHOWTASK   WM_USER+1
#define  WM_SERVER_ACCEPT  WM_USER+2

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CQQServerDlg 对话框




CQQServerDlg::CQQServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQQServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CQQServerDlg 消息处理程序

BOOL CQQServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitNotify();    //设置显示托盘图标
	SetWindowText(L"服务器");//该函数改变指定窗口的标题栏的文本内容（如果窗口有标题栏）。如果指定窗口是一个控件，则改变控件的文本内容
	//1 加载Socket库
	InitSocket();  //2 初始化Socket库
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQQServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQQServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQQServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



VOID CQQServerDlg::InitNotify()
{
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = m_hWnd;
	m_nid.uID = IDI_ICON_NOTIFY; 
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP; 
	m_nid.uCallbackMessage = WM_SHOWTASK;   //自定义消息   在托盘上处理鼠标动作

	//该函数从与hInstance模块相关联的可执行文件中装入lpIconName指定的图标资源,仅当图标资源还没有被装入时该函数才执行装入操作,否则只获取装入的资源句柄
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_NOTIFY)); 
	strcpy(m_nid.szTip,"点我");//当鼠标放在上面时，所显示的内容 
	Shell_NotifyIconA(NIM_ADD,&m_nid);//在托盘区添加图标  如ADD是添加一个图标，NIM_DELETE 是删除一个图标...等等

}
VOID CQQServerDlg::InitSocket()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	GetHostInfor();  


	//创建监听套接字

	m_sListen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  

	if(m_sListen==INVALID_SOCKET)
	{
		MessageBox(L"Create SOCKET Failed");
		WSACleanup(); 
		return;
	}

	//初始化网卡
	//sockaddr_in(2.2) sockaddr(1.0)
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = INADDR_ANY;
	m_ServerAddr.sin_port = htons(9527); 


	//1.0
	//绑定
	if(bind(m_sListen,
		(sockaddr*)&m_ServerAddr,sizeof(m_ServerAddr))==SOCKET_ERROR)
	{
		MessageBox(L"Bind Failed");
		WSACleanup(); 
		closesocket(m_sListen);
		return;
	}
	//监听
	if(listen(m_sListen,20)==SOCKET_ERROR)
	{
		MessageBox(L"Listen Failed");
		WSACleanup(); 
		closesocket(m_sListen);
		return;
	}

	WSAAsyncSelect(m_sListen,m_hWnd,WM_SERVER_ACCEPT,
		FD_ACCEPT|FD_READ|FD_CLOSE);  //又是那个奇葩事件


	for(int i=0;i<MAX;i++)
	{
		m_sClient[i] = INVALID_SOCKET;
	}

	m_strShow="Now server is working!\r\nWaiting for client's connecting .......";
	UpdateData(false);

	return;

}
VOID CQQServerDlg::GetHostInfor()
{
	char szHostName[128];   //获得Shine-PC                                                                            // 存储主机名
	if (gethostname(szHostName,128)==0)                                                   //正确的情况
	{
		hostent* pHost;    //Host Content
		pHost = gethostbyname(szHostName);  //网卡信息                                                   //pHost返回的是指向主机的列表
		LPCSTR psz = inet_ntoa(*(struct in_addr *)pHost->h_addr_list[0]);//得到指向ip的psz变量

		CString strTemp;
		strTemp += "主机名:";
		strTemp += szHostName;
		strTemp +=' ';

		strTemp+="IP地址:";
		strTemp+=psz;

		m_strHost.SetWindowText(strTemp);



		CString strPeople;
		strPeople.Format(L"%d",m_People);
		m_strPeople.SetWindowText(strPeople);
	}
}