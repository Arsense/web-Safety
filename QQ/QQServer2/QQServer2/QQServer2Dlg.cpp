
// QQServer2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQServer2.h"
#include "QQServer2Dlg.h"
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


// CQQServer2Dlg 对话框




CQQServer2Dlg::CQQServer2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQQServer2Dlg::IDD, pParent)
{
	m_Timer = _T("");
	m_People = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQServer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PEOPLE, m_strPeople);
	DDX_Control(pDX, IDC_STATIC_HOST, m_strHost);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_strShow);
}

BEGIN_MESSAGE_MAP(CQQServer2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_MESSAGE(WM_SERVER_ACCEPT,OnServerAccept)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EXIT,OnDlgExit)
	ON_COMMAND(ID_SHOW,OnDlgShow)
	ON_COMMAND(ID_HIDE,OnDlgHide)
	//ON_BN_CLICKED(IDC_STATIC_HOST, OnStaticHost)
	//ON_BN_CLICKED(IDC_STATIC_PEOPLE, OnStaticPeople)
	//ON_EN_CHANGE(IDC_EDIT_SHOW, OnChangeEditShow)
END_MESSAGE_MAP()


// CQQServer2Dlg 消息处理程序

BOOL CQQServer2Dlg::OnInitDialog()
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


	InitNotify();
	SetWindowText(L"服务器"); //把窗口最上面的QQServer2改成服务器
	//1 加载Socket库
	InitSocket();  //2 初始化Socket库



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQQServer2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQQServer2Dlg::OnPaint()
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
HCURSOR CQQServer2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CQQServer2Dlg::InitNotify()
{
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = m_hWnd;
	m_nid.uID = IDI_ICON_NOTIFY; 
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP; 
	m_nid.uCallbackMessage = WM_SHOWTASK;   //自定义消息   在托盘上处理鼠标动作


	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_NOTIFY)); 
	strcpy(m_nid.szTip,"点我");//当鼠标放在上面时，所显示的内容 
	Shell_NotifyIconA(NIM_ADD,&m_nid);//在托盘区添加图标 


}
VOID CQQServer2Dlg::InitSocket()
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
		FD_ACCEPT|FD_READ|FD_CLOSE);


	for(int i=0;i<MAX;i++)
	{
		m_sClient[i] = INVALID_SOCKET;
	}

	m_strShow="Now server is working!\r\nWaiting for client's connecting .......";
	UpdateData(false);

	return;

}



VOID CQQServer2Dlg::GetHostInfor()
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
//有些迷
LRESULT CQQServer2Dlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为   

	if (wParam!=IDI_ICON_NOTIFY)
	{
		return 1;
	}


	switch(lParam)
	{
	case  WM_RBUTTONUP:
		{

			CMenu Menu;
			Menu.CreatePopupMenu();
			Menu.AppendMenu(MF_STRING, ID_SHOW,L"显示");//一种可以在指定的菜单条、下拉式菜单、子菜单或快捷菜单的末尾追加一个新菜单项的函数。
			Menu.AppendMenu(MF_STRING, ID_HIDE,L"隐藏");	
			Menu.AppendMenu(MF_STRING, ID_EXIT,L"退出");	

			CPoint Pt;
			GetCursorPos(&Pt);         //得到鼠标位置
			Menu.TrackPopupMenu(TPM_RIGHTBUTTON, Pt.x, Pt.y, this);
			Menu.DestroyMenu();        //销毁资源

			break;
		}
	}

	return 0;
}

//显示出客户端上线的时间，姓名，IP
VOID CQQServer2Dlg::OnlineOrOutline(int iIndex,CString YesOrNo)
{
	CString strTemp;

	GetCurrentTime(); 

	strTemp+=m_Timer.LockBuffer();
	strTemp+=' ';
	strTemp+="用户:";
	strTemp+= m_PeopleInfor[iIndex].strName;
	strTemp+=" ";
	strTemp+=m_PeopleInfor[iIndex].strIp;
	strTemp+= YesOrNo;


	m_strShow +="\r\n";
	m_strShow += strTemp;

	UpdateData(FALSE);
}
VOID CQQServer2Dlg::OnClose(WPARAM wParam)
{

	int i = 0;
	for (i=0;i<MAX;i++)
	{
		if (m_sClient[i]==wParam)      //搜索出是第几个用户
		{
			break;
		}
	}

	if (i==MAX)    //如果超出最大用户数，就退出
	{
		return;
	}

	OnlineOrOutline(i,L"下线");

	m_People--;     //用户数减1

	CString strPeople;
	strPeople.Format(L"%d",m_People);
	m_strPeople.SetWindowText(strPeople);



	SENDINFOR  Data;
	int j = 0;
	for (j=0;j<MAX;j++)
	{
		if (m_sClient[j]!=INVALID_SOCKET&&j!=i)
		{
			Data.iType = -1;

			CString strMsg;
			GetCurrentTime();        //这个函数是如何把时间弄到strMsg上的？
			strMsg+=m_Timer.LockBuffer();
			strMsg+=m_PeopleInfor[i].strName;
			strMsg+="退出了该聊天室";                 //

			strcpy(Data.szName,(char*)m_PeopleInfor[i].strName.LockBuffer());
			strcpy(Data.szMsg,( char*)strMsg.LockBuffer()); 
			send(m_sClient[j],(char*)&Data,sizeof(Data),0);
		}
	}


	//清空退出用户的资源
	closesocket(m_sClient[i]);
	m_sClient[i] = INVALID_SOCKET;

	m_PeopleInfor[i].strIp = "";
	m_PeopleInfor[i].strName = "";


}

void CQQServer2Dlg::OnDlgExit()
{

	SendMessage(WM_CLOSE,NULL,NULL);
}

void CQQServer2Dlg::OnDlgShow()
{
	::ShowWindow(this->m_hWnd,SW_NORMAL);
}

void CQQServer2Dlg::OnDlgHide()
{
	ShowWindow(SW_HIDE);
}
void CQQServer2Dlg::OnDestroy() 
{
	CDialog::OnDestroy();

	Shell_NotifyIconA(NIM_DELETE,&m_nid);
}

//FD_ACCEPT  FD_CLOSE   FD_READ
//判断用户上线类型
LRESULT CQQServer2Dlg::OnServerAccept(WPARAM wParam,LPARAM lParam)
{

	//WSAGETSELECTEVENT == LOWORD(lParam)
	int iEvent = WSAGETSELECTEVENT(lParam); //调用Winsock API函数，得到网络事件类型
	switch(iEvent) 
	{ 
	case FD_ACCEPT://客户端连接请求事件 
		{
			OnAccept();   //用户的上线请求   Connect
			break;
		}
	case FD_CLOSE://客户端断开事件: 
		{

			OnClose(wParam);   //WParam 谁被关闭
			break;
		}

	case FD_READ://网络数据包到达事件 
		{

			OnRecive(wParam);    //Send 
			break;
		}
	default: break; 
	} 
	return 0;
}
VOID CQQServer2Dlg::OnRecive(WPARAM wParam)
{
	SENDINFOR  Data = {0};

	int i = 0;
	for (i=0;i<MAX;i++)
	{
		if (m_sClient[i]==wParam)  //搜索出当前用户是第几个
		{
			break;
		}
	}

	if (i==MAX) 
	{
		return;
	}

	int iRet = recv(m_sClient[i],(char*)&Data,sizeof(Data),0);   //接收用户信息

	//0        关羽     
	//消息类型 用户名称 信息内容

	//分析数据包
	switch(Data.iType)
	{

	case 20:
	case 6:
		{
			//6  11  对方已接受请求，建立连接.......

			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET&&
					memcmp(m_PeopleInfor[j].strName.LockBuffer(),Data.szName,sizeof(PEOPLEINFOR))==0)  //LockBuffer对引用内存块进行加锁 ,使其内存不能被访问
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);      //回传给用户消息
				}
			}

			break;
		}
	case 7:    //6.7有啥区别？
		{

			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET&&
					memcpy(m_PeopleInfor[j].strName.LockBuffer(),Data.szName,sizeof(PEOPLEINFOR))==0)
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);
				}
			}



			break;
		}
	case 5:    //11 -12   
		//这个是私聊
		{
			//11--->12
			//5 12 11|1.txt|5|
			//5 12 192.168.0.100|11|1.txt|5|    --->12

			CString strTemp;	
			strTemp+=m_PeopleInfor[i].strIp;
			strTemp+="|";
			strTemp+=Data.szMsg;

			memcpy(Data.szMsg,strTemp.LockBuffer(),sizeof(PEOPLEINFOR)); //考进用户IP 和回传信息


			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET&&
					memcpy(m_PeopleInfor[j].strName.LockBuffer(),Data.szName,sizeof(PEOPLEINFOR))==0)
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);
				}
			}

			break;
		}

	case 120:   //私聊
		{

			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET&&
					memcmp(m_PeopleInfor[j].strName.LockBuffer(),Data.szName,sizeof(PEOPLEINFOR))==0)
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);
				}
			}


			break;
		}
	case 110:   //群聊
		{
			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET&&j!=i)
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);
				}
			}
			break;
		}
	case 2:
		{

			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (memcmp(m_PeopleInfor[j].strName,Data.szName,sizeof(PEOPLEINFOR))==0
					&&m_sClient[j]!=INVALID_SOCKET)
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);

					break;
				}
			}



			break;
		}
	case 1:   
		{

			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET&&j!=i)
				{
					send(m_sClient[j],(char*)&Data,sizeof(Data),0);
				}
			}
			break;
		}
	case 0:   //New People
		{
			m_PeopleInfor[i].strName = Data.szName;

			//向控件添加数据
			OnlineOrOutline(i,L"上线");


			//刷新用户链表
			//1向老的通知新的用户(自己得到自己)
			int j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET)
				{
					CString Temp;



					Data.iType = 0;
					memcpy(Data.szName,m_PeopleInfor[i].strName,sizeof(PEOPLEINFOR)); 
					//多次运用LockBuffer 就是防止聊天信息记录的时间或者数据被其他人更改
					Temp +="\r\n";
					GetCurrentTime();
					Temp+=m_Timer.LockBuffer();
					Temp+="系统消息:\r\n  ";
					Temp+=m_PeopleInfor[i].strName;
					Temp+="进入聊天室";

					strcpy(Data.szMsg,(char*)Temp.LockBuffer());   


					send(m_sClient[j],(char*)&Data,sizeof(Data),0);
				}
			}


			//向新的通知老的用户
			j = 0;
			for (j=0;j<MAX;j++)
			{
				if (m_sClient[j]!=INVALID_SOCKET && j!=i)
				{

					Data.iType = 0;
					memset(Data.szMsg,0,400);
					memcpy(Data.szName,m_PeopleInfor[j].strName,sizeof(PEOPLEINFOR)); 
					send(m_sClient[i],(char*)&Data,sizeof(Data),0);
				}
			}

			break;
		}
	}
}
VOID CQQServer2Dlg::OnAccept()
{

	int i = 0;
	for (i=0;i<MAX;i++)
	{
		if (m_sClient[i]==INVALID_SOCKET)
		{
			break;
		}
	}

	if (i==MAX)
	{
		return;
	}

	sockaddr_in  ClientAddr;
	int nLen = sizeof(sockaddr_in);    

	m_sClient[i] = accept(m_sListen,(sockaddr*)&ClientAddr,&nLen); //接收信息



	LPCSTR strIP = inet_ntoa(*(struct in_addr*)&ClientAddr.sin_addr);
	m_PeopleInfor[i].strIp = strIP;    //得到上线用户的IP并存入用户数据结构体中


	m_People++;   //用户数加1

	CString strPeople;
	strPeople.Format(L"%d",m_People);
	m_strPeople.SetWindowText(strPeople);

	UpdateData(FALSE);
}
VOID CQQServer2Dlg::GetCurrentTime()
{

	SYSTEMTIME st = {0};
	GetLocalTime(&st);    //得到当前时间
	int j = 0;
	j  = sprintf((char*)m_Timer.LockBuffer(),"%d年",st.wYear);
	j += sprintf((char*)m_Timer.LockBuffer()+j,"%d月",st.wMonth);
	j += sprintf((char*)m_Timer.LockBuffer()+j,"%d日",st.wDay);
	j += sprintf((char*)m_Timer.LockBuffer()+j,"%d时",st.wHour);

	if (st.wMinute<10)  //为什么是小于10？
	{
		int i = 0;
		j += sprintf((char*)m_Timer.LockBuffer()+j,"%d",i); 
	}
	j += sprintf((char*)m_Timer.LockBuffer()+j,"%d分钟",st.wMinute);

	if (st.wSecond<10) 
	{
		int i = 0;
		j += sprintf((char*)m_Timer.LockBuffer()+j,"%d",i); 
	}
	j += sprintf((char*)m_Timer.LockBuffer()+j,"%d秒",st.wSecond);


}
void CQQServer2Dlg::OnStaticHost() 
{
	// TODO: Add your control notification handler code here

}

void CQQServer2Dlg::OnStaticPeople() 
{
	// TODO: Add your control notification handler code here

}

void CQQServer2Dlg::OnChangeEditShow() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here

}
