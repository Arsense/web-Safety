
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Login.h"

#include <Mmsystem.h>

#pragma comment(lib,"Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILEMAX  17520
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

#define  WM_SOCKET_READ  WM_USER+2
static DWORD WINAPI RecvFileThread(LPVOID lPParam);
static DWORD WINAPI SendFileThread(LPVOID lPParam);
typedef struct _SENDINFOR_    
{
	int  iType;       //0
	char szName[20];  //123
	char szMsg[400];  //“” 
}SENDINFOR;

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CClientDlg 对话框




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_EditShow(_T(""))
	, m_bOnly(FALSE)
	, m_bMe(FALSE)
	, m_EditSend(_T(""))
{
	//m_EditShow = _T("");
	//m_EditSend = _T("");

	//m_bMe = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_EditShow = _T("");
		m_dwFileSize = 0;
		//*******************************************************************************
    //初始化像皮筋类,新增的resizeMiddle 类型
	m_rectTracker.m_nStyle=CMyTracker::resizeMiddle|CMyTracker::solidLine;  
	m_rectTracker.m_rect.SetRect(-1,-2,-3,-4);
	//设置矩形颜色
	m_rectTracker.SetRectColor(RGB(10,100,130));
	//设置矩形调整时光标,默认的太小了,弄了个大点的
	m_rectTracker.SetResizeCursor(IDC_CURSOR6,IDC_CURSOR5,IDC_CURSOR2,IDC_CURSOR3,IDC_CURSOR4);

    
    
	//一系列的数据初始化
	m_bDraw=FALSE;
	m_bFirstDraw=FALSE;
	m_bQuit=FALSE;
	m_bShowMsg=FALSE;
    m_startPt=0;
    
	//获取屏幕分辩率
	m_xScreen = GetSystemMetrics(SM_CXSCREEN);//主显示监视器的画面，以像素为单位的宽度。这是通过调用获得的值相同 GetDeviceCaps如下：GetDeviceCaps（hdcPrimaryMonitor，HORZRES
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);//主显示监视器的画面，以像素为单位的高度。这是通过调用获得的值相同 GetDeviceCaps如下：GetDeviceCaps（hdcPrimaryMonitor，VERTRES） 。

	//截取屏幕到位图中
	CRect rect(0, 0,m_xScreen,m_yScreen);
	m_pBitmap=CBitmap::FromHandle(CopyScreenToBitmap(&rect));
    
	//初始化刷新窗口区域 m_rgn
    m_rgn.CreateRectRgn(0,0,50,50);
	m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);  
//*******************************************************************************



}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USER, m_ComBoUser);
	DDX_Control(pDX, IDC_LIST_USER, m_ListUser);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_EditShow);
	DDX_Check(pDX, IDC_CHECK_ONLY, m_bOnly);
	DDX_Check(pDX, IDC_CHECK_ME, m_bMe);
	DDX_Text(pDX, IDC_EDIT_SEND, m_EditSend);
	DDX_Control(pDX, IDC_BUTTON_END, m_ButtonCancle);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_BUTTON_RECV, m_ButtonRecv);
	DDX_Control(pDX, IDC_CATCHSCREEN, m_tipEdit);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_SOCKET_READ,OnSocketRead)
	ON_BN_CLICKED(IDC_BUTTON_SHAKE, &CClientDlg::OnButtonShake)
	ON_BN_CLICKED(IDC_BUTTON_TRANS, &CClientDlg::OnButtonTrans)
	ON_CBN_SELCHANGE(IDC_COMBO_USER, &CClientDlg::OnCbnSelchangeComboUser)
	ON_EN_CHANGE(IDC_EDIT_SEND, &CClientDlg::OnEnChangeEditSend)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientDlg::OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECV, &CClientDlg::OnButtonRecv)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//**************************************************************************
	//把对化框设置成全屏顶层窗口
	SetWindowPos(&wndTopMost,0,0,m_xScreen,m_yScreen,SWP_SHOWWINDOW);
    
	//移动操作提示窗口
	CRect rect;
	m_tipEdit.GetWindowRect(&rect);
    m_tipEdit.MoveWindow(10,10,rect.Width(),rect.Height());

	//显示操作提示窗口文字
	DrawTip();
	
	//捕获按键消息窗口,将对话框的句柄传递到CCatchScreenApp中
	((CClientApp *)AfxGetApp())->m_hwndDlg=m_hWnd;
//**************************************************************************
	CLogin  LoginDlg;           //创建登陆对话框

	if (LoginDlg.DoModal()==IDOK)     
	{

		//LoginDlg 获得ServerIP 姓名

		CString strName;
		CString strIP;



		strName = LoginDlg.m_strName;        //把对话框上的名字赋值给strName

		ULONG ulServerIP;                //127.0.0.1
		ulServerIP = inet_addr(LoginDlg.m_strIP);//字符型IP转32位IP



		m_ClientSocket = socket(PF_INET,SOCK_STREAM,0);    //通信套接字
		if(m_ClientSocket==INVALID_SOCKET)          //没有创建成功
		{
			AfxMessageBox("Create SOCKET Failed");
			WSACleanup(); 
			closesocket(m_ClientSocket);
			CClientDlg::OnCancel();
		}


		//初始服务器信息
		m_ServerAddr.sin_family = AF_INET;
		m_ServerAddr.sin_addr.s_addr = ulServerIP;
		m_ServerAddr.sin_port = htons(9527);//short 网络字节序


		//链接
		int bOk =
			connect(m_ClientSocket,(sockaddr*)&m_ServerAddr,sizeof(m_ServerAddr));      

		if(bOk==SOCKET_ERROR)           //连接失败
		{
			MessageBox("连接过程发生错误！\n请确保IP输入正确无误！",NULL,MB_OK);
			CDialog::OnCancel();
		}
		else
		{
			SendMsg(0,strName,"");   //0 数据的类型  向服务器发送第一个消息 姓名
		}
		WSAAsyncSelect(m_ClientSocket,m_hWnd,WM_SOCKET_READ,FD_READ|FD_CLOSE);
		//注册一个自定义消息处理从服务器来的数据包请求和服务器关闭


		PlaySound(MAKEINTRESOURCE(IDR_WAVE),                //生成声音
			AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT); //AfxGetResourceHandle用于获取当前资源模块句柄
		SetWindowText(strName);
	
	}

	else
	{
		CDialog::OnCancel();
	}

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
		//ASSERT(bNameValid);
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

	m_ComBoUser.AddString("大家");        //发送里显示大家
	m_ComBoUser.SetWindowText("大家");   //在下拉式菜单里刷出大家


	CButton* Temp; 
	HBITMAP  hBitmap;
	Temp =(CButton *)GetDlgItem(IDC_BUTTON_TRANS);
	hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_SEND));
	Temp->SetBitmap(hBitmap);


	Temp =(CButton *)GetDlgItem(IDC_BUTTON_SHAKE);
	hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_LIGHT));
	Temp->SetBitmap(hBitmap);


	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		//**************************************************************************
		CPaintDC dc(this);
         
		//显示截取矩形大小信息
		if(m_bShowMsg&&m_bFirstDraw)
		{
			//得到当前矩形大小
			CRect rect;
			m_rectTracker.GetTrueRect(&rect);
			//传递CPaintDC 是为了不在了窗口上画信息
			DrawMessage(rect,&dc);
		}

		//画出像皮筋矩形
		if(m_bFirstDraw)
		{
			m_rectTracker.Draw(&dc);
		}

//*************************************************************************
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


VOID CClientDlg::SendMsg(int iType,CString strName,CString strMsg)
{
	SENDINFOR SendInfor = {0};
	SendInfor.iType = iType;


	memcpy(SendInfor.szName,strName.LockBuffer(),strName.GetLength()+1);
	memcpy(SendInfor.szMsg,strMsg.LockBuffer(),strMsg.GetLength()+1);

	int bOk = send(m_ClientSocket,(char*)&SendInfor,sizeof(SENDINFOR),0);
	if(bOk==SOCKET_ERROR)
	{
		MessageBox("发送过程中发生一个错误！",NULL,MB_OK);
		return;
	}
}

LRESULT CClientDlg::OnSocketRead(WPARAM wParam,LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam);

	switch (iEvent)
	{
	case FD_CLOSE:
		{
			break;
		}
	case FD_READ:
		{
			OnRecive(wParam); 
			break;
		}
	default:break;
	}

	return 0;
}

VOID CClientDlg::OnRecive(WPARAM wParam)
{
	SENDINFOR  Data = {0};

	int iRet = recv(m_ClientSocket,(char*)&Data,sizeof(Data),0);

	//0        关羽     
	//消息类型 用户名称 信息内容

	//0        关羽     []进入聊天室

	//分析数据包
	switch(Data.iType)
	{

	case 20:
		{
			m_EditShow+="\r\n";
			m_EditShow+=Data.szMsg;

			UpdateData(FALSE);

			break;
		}
	case 6:
		{
			m_EditShow+="\r\n";
			m_EditShow+=Data.szMsg;

			UpdateData(FALSE);


			//11
			//创建一个发送数据的线程

			CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendFileThread,this,0,NULL);


			break;
		}
	case 7:
		{

			m_EditShow+="\r\n";
			m_EditShow+=Data.szMsg;
			UpdateData(FALSE);

			HideItem();
			break;
		}
	case  5:    //11  5    ->12
		{
			AnalyseString(Data.szMsg);    //5  12 192.168.0.100|11|1.txt|5|

			CString strFileSize;
			strFileSize = KindChange(m_dwOtherFileSize);

			strFileSize="("+strFileSize+")";


			m_EditShow+="\r\n\r\n";
			m_EditShow+=m_strOtherName;
			m_EditShow+="请求给你发送文件 ";
			m_EditShow+=m_strOtherFileName;
			m_EditShow+=strFileSize;


			//更新控件

			GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_SHOW);

			m_ButtonCancle.ShowWindow(SW_SHOW);
			m_ButtonCancle.SetWindowText("拒绝");
			m_ButtonRecv.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_TRANS)->ShowWindow(SW_HIDE);



			GetDlgItem(IDC_STATIC_RECV)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_RECV)->SetWindowText("接受文件：");
			GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(m_strOtherFileName);
			GetDlgItem(IDC_STATIC_FILESIZE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FILESIZE)->SetWindowText(strFileSize);

			GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC)->SetWindowText("0%");

			GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("速度: 0");
			if(this->IsWindowVisible()!=1)
			{
				::SetForegroundWindow(m_hWnd);
				::ShowWindow(m_hWnd,SW_SHOW);				
			}



			UpdateData(FALSE);

			break;
		}
	case -1:
		{

			m_EditShow+="\r\n";
			m_EditShow+=Data.szMsg;	



			//更新控件信息
			int Index = m_ListUser.FindString(0,Data.szName);

			if (Index<0)
			{
				break;
			}
			m_ListUser.DeleteString(Index);


			Index = m_ComBoUser.FindString(0,Data.szName);

			if (Index<0)
			{
				break;
			}
			m_ComBoUser.DeleteString(Index);



			UpdateData(FALSE);



			break;
		}
	case 120:
	case 110:
		{

			m_EditShow+="\r\n";
			m_EditShow+=Data.szMsg;	
			UpdateData(FALSE);

			Shaking();

			break;
		}
	case 1:   //大家说
	case 2:   //私聊
		{

			UpdateData(TRUE);
			if (m_bMe==TRUE)
			{
				if (Data.szName[0]!=0&&strcmp(Data.szName,m_strName.LockBuffer())==0)
				{


					m_EditShow+="\r\n";
					m_EditShow+=Data.szMsg;

				}
				UpdateData(FALSE);
				break;
			}


			m_EditShow+="\r\n";
			m_EditShow+=Data.szMsg;


			UpdateData(FALSE);

			break;
		}
	case 0:   //New People
		{

			m_ListUser.AddString(Data.szName);
			m_ComBoUser.AddString(Data.szName);


			if (Data.szMsg[0]!=NULL)       //当前数据包给老用户更新新的用户
			{
				m_EditShow+=Data.szMsg;    //EditBox 添加消息

				UpdateData(FALSE);
			}



			break;
		}
	}




	PlaySound(MAKEINTRESOURCE(IDR_WAVE),
		AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
	SetWindowText(m_strName);





	HWND hWnd = ::GetForegroundWindow();

	if (hWnd!=this->m_hWnd)
	{
		this->FlashWindow(1);
	}

}




void CClientDlg::OnButtonShake()
{

	UpdateData(TRUE);
	CString  Data;
	CString Temp;
	m_ComBoUser.GetWindowText(Temp);
	if(Temp==m_strName)
	{
		MessageBox("不能给自己发消息");
		return;
	}

	else
	{
		if (Temp=="大家")
		{


			GetCurrentTime();
			Data+=m_Timer.LockBuffer();
			Data+=m_strName;
			Data+="对大家发送了一个抖动窗口";	

			SendMsg(110,"",Data);


			m_EditShow+="\r\n";
			m_EditShow+=m_Timer.LockBuffer();
			m_EditShow+="你对大家发送了一个抖动窗口";


			m_EditSend = "";

			UpdateData(FALSE);


		}
		else
		{

			GetCurrentTime();
			Data+=m_Timer.LockBuffer();
			Data+=m_strName;
			Data+="对";
			Data+=Temp;
			Data+="发送了一个抖动窗口";	

			SendMsg(120,Temp,Data);


			m_EditShow+="\r\n";
			m_EditShow+=m_Timer.LockBuffer();
			m_EditShow+="你对";
			m_EditShow+=Temp;
			m_EditShow+="发送了一个抖动窗口";


			m_EditSend = "";

			UpdateData(FALSE);


		}
	}


	Shaking();


}


void CClientDlg::OnButtonTrans()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CClientDlg::OnCbnSelchangeComboUser()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CClientDlg::OnEnChangeEditSend()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CClientDlg::OnButtonSend()
{
	CString  Data;
	UpdateData(true);
	if(m_EditSend=="")
	{
		MessageBox("消息不能为空");
		return;
	}

	CString Temp;
	m_ComBoUser.GetWindowText(Temp);
	if(Temp==m_strName)
	{
		MessageBox("不能给自己发消息");
		return;
	}
	else
	{

		if (Temp=="大家")
		{
			//构建数据包
			GetCurrentTime();
			Data+=m_Timer.LockBuffer();
			Data+=m_strName;
			Data+="对大家说:";
			Data+=m_EditSend;


			SendMsg(1,"",Data);

			m_EditShow+="\r\n";
			m_EditShow+=m_Timer.LockBuffer();
			m_EditShow+="你对大家说:";
			m_EditShow+=m_EditSend;

			UpdateData(FALSE);
		}

		else
		{


			if (m_bOnly==TRUE)   //私聊
			{


				GetCurrentTime();
				Data+=m_Timer.LockBuffer();
				Data+=m_strName;
				Data+="对你说:";
				Data+=m_EditSend;

				SendMsg(2,Temp,Data);


				m_EditShow+="\r\n";
				m_EditShow+=m_Timer.LockBuffer();
				m_EditShow+="你对";
				m_EditShow+=Temp;
				m_EditShow+="说:";
				m_EditShow+=m_EditSend;


				UpdateData(FALSE);

			}
			else     
			{
				GetCurrentTime();
				Data+=m_Timer.LockBuffer();
				Data+=m_strName;
				Data+="对";
				Data+=Temp;
				Data+="说:";
				Data+=m_EditSend;


				SendMsg(1,Temp,Data);

				m_EditShow+="\r\n";
				m_EditShow+=m_Timer.LockBuffer();
				m_EditShow+="你对";
				m_EditShow+=Temp;
				m_EditShow+="说:";
				m_EditShow+=m_EditSend;

				UpdateData(FALSE);
			}
		}


	}

	m_EditSend = "";

	UpdateData(FALSE);
}
void CClientDlg::Shaking()
{
	int ty=3;
	CRect   m_rect; 
	this->ShowWindow(SW_SHOW);
	::SetForegroundWindow(m_hWnd);
	::GetWindowRect(m_hWnd,&m_rect);  
	int recordy=m_rect.left;
	int recordx=m_rect.top;
	for(int i=0;i<3;i++)
	{
		m_rect.left=recordy;
		m_rect.top=recordx;
		m_rect.top = m_rect.top + ty;  
		m_rect.left = m_rect.left - ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.top = m_rect.top -ty;
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.top = m_rect.top -ty;
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.left=m_rect.left+ty;
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.left=m_rect.left+ty;
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.top = m_rect.top + ty;  
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.top=m_rect.top+ty;
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		m_rect.top=m_rect.top+ty;
		::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
		::SetWindowPos( m_hWnd,NULL,recordy,recordx,0,0,SWP_NOSIZE);Sleep(3);
	}
}
void CClientDlg::HideItem()
{
	GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_HIDE);
	m_ButtonCancle.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RECV)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_FILESIZE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_TRANS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_RECV)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
}
static DWORD WINAPI SendFileThread(LPVOID lPParam)  //11
{

	CClientDlg* ClientDlg = (CClientDlg*)lPParam;

	//要读文件内容发送数据  fopen  fread 
	CFile File;   
	if(!File.Open(ClientDlg->m_strFileName,CFile::modeRead)) 
	{ 
		AfxMessageBox("打开文件出错!");     //发送给另外一个客户端
		ClientDlg->GetDlgItem(IDC_BUTTON_TRANS)->ShowWindow(SW_SHOW);
		ClientDlg->HideItem();
		return 0; 
	} 

	else
	{

		//等待一个链接
		CSocket sListen;
		sListen.Create(9999);
		sListen.Listen(1);
		CSocket sClient;
		sListen.Accept(sClient);  //Block cin 


		ClientDlg->m_Progress.SetRange(0,100);


		CString strName;
		ClientDlg->m_ComBoUser.GetWindowText(strName);
		ClientDlg->SendMsg(20,strName,"开始接收数据.......");


		//读文件  发送文件内容
		DWORD ByteTotal = 0;
		DWORD dwTemp  = 0;
		DWORD dwTemp1 = 0;
		CString strTemp;
		CString strTemp1;
		char Buffer[FILEMAX] = {0}; 
		while (1)
		{
			DWORD dwLen = 0;
			dwLen = File.Read(Buffer,FILEMAX);  

			if(dwLen==0)
			{
				break;
			}

			dwLen = sClient.Send(Buffer,dwLen);

			if(dwLen==SOCKET_ERROR)
			{ 
				AfxMessageBox("发送失败！"); 
				break;
			}

			ByteTotal+=dwLen;
			dwTemp = int(ByteTotal*100.0/ClientDlg->m_dwFileSize);
			if(dwTemp1!=dwTemp)
			{
				dwTemp1=dwTemp;
				ClientDlg->m_Progress.SetPos(dwTemp);
			}
			strTemp.Format("%d",dwTemp);
			if(strTemp1!=strTemp)
			{
				strTemp1=strTemp;
				strTemp=strTemp+"%";
				ClientDlg->GetDlgItem(IDC_STATIC)->SetWindowText(strTemp); 
			}

		}


		sClient.Close();
		sListen.Close();
		File.Close();


		ClientDlg->m_Progress.SetPos(0);
		ClientDlg->HideItem();


		if(ByteTotal==ClientDlg->m_dwFileSize)
		{			
			ClientDlg->SendMsg(20,strName,"文件接收完成!");
		}
	}

	return 0;
}

static DWORD WINAPI RecvFileThread(LPVOID lPParam) //12
{
	//访问CClientDlg 的成员
	CClientDlg* ClientDlg = (CClientDlg*)lPParam;


	//另存为Dlg
	CFileDialog Dlg(FALSE,NULL,NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"所有文件 (*.*)|*.*||");

	//	CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,  
	//		"所有文件 (*.*)|*.*||"); 

	strcpy(Dlg.m_ofn.lpstrFile, 
		ClientDlg->m_strOtherFileName.GetBuffer(ClientDlg->m_strOtherFileName.GetLength()));

	if (Dlg.DoModal()!=IDOK)
	{
		return -1;
	}

	else
	{
		ClientDlg->GetDlgItem(IDC_BUTTON_RECV)->ShowWindow(SW_HIDE);//接收按钮 
		ClientDlg->m_ButtonCancle.SetWindowText("中止");


		CSocket FileSocket;   
		if(FileSocket.Create(9999)==SOCKET_ERROR)
		{
			FileSocket.Create();
		}


		ClientDlg->GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(Dlg.GetFileName());

		ClientDlg->m_Progress.SetRange(0,100);



		//11-->12   12    6  11  对方已接受请求，建立连接.......
		ClientDlg->SendMsg(6,ClientDlg->m_strOtherName,"对方已接受请求，建立连接.......");


		//链接通信Socket
		while(FileSocket.Connect(ClientDlg->m_strOtherIP,9999)==0)
		{
			Sleep(50);   
		}



		CFile File; 
		if(!File.Open(Dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite))
		{ 
			AfxMessageBox("打开文件出错！"); 
			return 0; 
		} 	

		ClientDlg->SendMsg(20,ClientDlg->m_strOtherName,"开始传输数据.......");


		DWORD ByteTotal = 0;
		DWORD dwTemp  = 0;
		DWORD dwTemp1 = 0;
		CString strTemp;
		CString strTemp1;
		//接受文件  创建新文件
		char Buffer[FILEMAX] = {0}; 
		while (1)
		{

			DWORD dwLen = 0;
			dwLen = FileSocket.Receive(Buffer,FILEMAX);			
			if(dwLen==SOCKET_ERROR)
			{ 
				AfxMessageBox("接收失败"); 
				break;
			}

			if(dwLen==0)
			{
				break;
			}

			File.Write(Buffer,dwLen);

			ByteTotal+=dwLen;
			dwTemp = int(ByteTotal*100.0/ClientDlg->m_dwOtherFileSize);
			if(dwTemp1!=dwTemp)
			{
				dwTemp1=dwTemp;
				ClientDlg->m_Progress.SetPos(dwTemp);
			}
			strTemp.Format("%d",dwTemp);
			if(strTemp1!=strTemp)
			{
				strTemp1=strTemp;
				strTemp=strTemp+"%";
				ClientDlg->GetDlgItem(IDC_STATIC)->SetWindowText(strTemp); 
			}

		}

		FileSocket.Close();	
		File.Close();
		ClientDlg->m_Progress.SetPos(0);
		ClientDlg->HideItem();

		if(ByteTotal==ClientDlg->m_dwFileSize)
		{
			ClientDlg->SendMsg(20,ClientDlg->m_strOtherName,"文件发送完成!");
		}

	}

	return 0;

}
void CClientDlg::AnalyseString(char* szBuffer)
{

	char* szTemp = NULL;
	char* szStart = szBuffer;
	int j = 0;
	while (1)
	{


		szTemp = strstr(szStart,"|");  //192.168.0.100\011\01.txt\05\0 

		if (szTemp==NULL)
		{
			break;
		}

		*szTemp = '\0';


		if (j==0)
		{
			m_strOtherIP = szStart;
		}

		else if (j==1)
		{
			m_strOtherName = szStart;

		}

		else if (j==2)
		{
			m_strOtherFileName = szStart;

		}

		else if (j==3)
		{

			m_dwOtherFileSize = atoi(szStart);   //char int

		}


		szTemp++;
		szStart = szTemp;
		j++;
	}
}

CString CClientDlg::KindChange(DWORD dwFileSize)  
{
	float Size;
	CString strFileSize;
	if(dwFileSize<1024)
	{
		strFileSize.Format("%d字节",dwFileSize);
	}
	else if(dwFileSize<(1024*1024))
	{
		Size = dwFileSize*1.0/1024;
		strFileSize.Format("%.1fKB",Size);
	}
	else
	{
		Size=dwFileSize*1.0/(1024*1024);
		strFileSize.Format("%.1fMB",Size);
	}
	return strFileSize;
}



void CClientDlg::OnButtonRecv()
{
	//Server
	//启动接受线程    P2P   12 connect 11  Accept
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RecvFileThread,this,0,NULL);
}
//*********************增加的函数**********************************************************
//考贝屏幕,这段代码是拿来主义 呵呵
HBITMAP CClientDlg::CopyScreenToBitmap(LPRECT lpRect,BOOL bSave)
//lpRect 代表选定区域
{
	HDC       hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;
	
	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);

	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > m_xScreen)
		nX2 = m_xScreen;
	if (nY2 > m_yScreen)
		nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	if(bSave)
	{
		//创建军兼容DC,当bSave为中时把开始保存的全屏位图,按截取矩形大小保存
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_pBitmap);
        
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			dcCompatible, nX, nY, SRCCOPY);

	}
	else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	
	if(bSave)
	{
				
		if (OpenClipboard()) 
		{
        //清空剪贴板
        EmptyClipboard();
        //把屏幕内容粘贴到剪贴板上,
        //hBitmap 为刚才的屏幕位图句柄
        SetClipboardData(CF_BITMAP, hBitmap);
        //关闭剪贴板
        CloseClipboard();
      }
	}
	// 返回位图句柄

	return hBitmap;
}
//显示操作提示信息
void CClientDlg::DrawTip()
{

	//得当前坐标像素,
	CPoint pt;
	GetCursorPos(&pt);

	//当到当前R,G,B,各像素值
	COLORREF color;
	CClientDC dc(this);
	color=dc.GetPixel(pt);
	BYTE rValue,gValue,bValue;
	rValue=GetRValue(color);
	gValue=GetGValue(color);
	bValue=GetGValue(color);

	//按格式排放字符串
	CString string;
	CString strTemp;
	string.Format("\r\n\r\n\r\n ·当前像素RGB (%d,%d,%d)\r\n",rValue,gValue,bValue);

	if(!m_bDraw&&!m_bFirstDraw)
	{
		strTemp="\r\n ·按下鼠标左键不放选择截取\r\n 范围\r\n\r\n ·按ESC键或鼠标右键退出";
	}
	else
		if(m_bDraw&&m_bFirstDraw)
		{
			strTemp="\r\n ·松开鼠标左键确定截取范围\r\n\r\n ·按ESC键退出";
		}
		else
			if(m_bFirstDraw)
			{
				strTemp="\r\n ·用鼠标左键调整截取范围的\r\n 大小和位置\r\n\r\n ·截取范围内双击鼠标左键保\r\n 存图像，结束操作\r\n\r\n ·点击鼠标右键重新选择";
			}
			string+=strTemp;
			//显示到编缉框中,操作提示窗口
			m_tipEdit.SetWindowText(string);
}
//显示截取矩形信息
void CClientDlg::DrawMessage(CRect &inRect,CDC * pDC)
{
	//截取矩形大小信息离鼠标间隔
	const int space=3;

	//设置字体颜色大小

	CPoint pt;
	CPen pen(PS_SOLID,1,RGB(147,147,147));

	//dc.SetTextColor(RGB(147,147,147));
	CFont font;
	CFont * pOldFont;
	font.CreatePointFont(90,"宋体");
	pOldFont=pDC->SelectObject(&font);

	//得到字体宽度和高度
	GetCursorPos(&pt);
	int OldBkMode;
	OldBkMode=pDC->SetBkMode(TRANSPARENT);

	TEXTMETRIC tm;
	int charHeight;
	CSize size;
	int	lineLength;
	pDC->GetTextMetrics(&tm);
	charHeight = tm.tmHeight+tm.tmExternalLeading;
	size=pDC->GetTextExtent("顶点位置  ",strlen("顶点位置  "));
	lineLength=size.cx;

	//初始化矩形, 以保证写下六行文字
	CRect rect(pt.x+space,pt.y-charHeight*6-space,pt.x+lineLength+space,pt.y-space);

	//创建临时矩形
	CRect rectTemp;
	//当矩形到达桌面边缘时调整方向和大小
	if((pt.x+rect.Width())>=m_xScreen)
	{
		//桌面上方显示不下矩形
		rectTemp=rect;
		rectTemp.left=rect.left-rect.Width()-space*2;
		rectTemp.right=rect.right-rect.Width()-space*2;;
		rect=rectTemp;
	}

	if((pt.y-rect.Height())<=0)
	{
		//桌面右方显示不下矩形
		rectTemp=rect;
		rectTemp.top=rect.top+rect.Height()+space*2;;
		rectTemp.bottom=rect.bottom+rect.Height()+space*2;;
		rect=rectTemp;

	}

	//创建空画刷画矩形
	CBrush * pOldBrush;
	pOldBrush=pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));

	pDC->Rectangle(rect);
	rect.top+=2;
	//在矩形中显示文字
	CRect outRect(rect.left,rect.top,rect.left+lineLength,rect.top+charHeight);
	CString string("顶点位置");
	pDC->DrawText(string,outRect,DT_CENTER);

	outRect.SetRect(rect.left,rect.top+charHeight,rect.left+lineLength,charHeight+rect.top+charHeight);
	string.Format("(%d,%d)",inRect.left,inRect.top);
	pDC->DrawText(string,outRect,DT_CENTER);


	outRect.SetRect(rect.left,rect.top+charHeight*2,rect.left+lineLength,charHeight+rect.top+charHeight*2);
	string="矩形大小";
	pDC->DrawText(string,outRect,DT_CENTER);

	outRect.SetRect(rect.left,rect.top+charHeight*3,rect.left+lineLength,charHeight+rect.top+charHeight*3);
	string.Format("(%d,%d)",inRect.Width(),inRect.Height());
	pDC->DrawText(string,outRect,DT_CENTER);

	outRect.SetRect(rect.left,rect.top+charHeight*4,rect.left+lineLength,charHeight+rect.top+charHeight*4);
	string="光标坐标";
	pDC->DrawText(string,outRect,DT_CENTER);

	outRect.SetRect(rect.left,rect.top+charHeight*5,rect.left+lineLength,charHeight+rect.top+charHeight*5);
	string.Format("(%d,%d)",pt.x,pt.y);
	pDC->DrawText(string,outRect,DT_CENTER);

	pDC->SetBkMode(OldBkMode);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);

}

void CClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnMouseMove(nFlags, point);
}
