
// QQClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include"Login.h"
#include "QQClient.h"
#include "QQClientDlg.h"
#include "afxdialogex.h"
#include<afxsock.h>
#include<Mmsystem.h>
#pragma comment(lib,"Winmm.lib")


#define  WM_SOCKET_READ  WM_USER+2
static DWORD WINAPI SendFileThread(LPVOID lPParam);
static DWORD WINAPI RecvFileThread(LPVOID lPParam);
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
typedef struct _SENDINFOR_
{
	int  iType;       //0
	char szName[20];  //123
	char szMsg[400];  //“” 
}SENDINFOR;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
#define FILEMAX  17520
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
public:

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


// CQQClientDlg 对话框



CQQClientDlg::CQQClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQQClientDlg::IDD, pParent)
{
	m_Timer = " ";
	m_EditShow = _T("");
	m_EditSend = _T("");
	m_bOnly = FALSE;
	m_bMe = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		m_dwFileSize = 0;
}

void CQQClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_RECV, m_ButtonRecv);
	DDX_Control(pDX, IDC_BUTTON_END, m_ButtonCancle);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_COMBO_USER, m_ComBoUser);
	DDX_Control(pDX, IDC_LIST_USER, m_ListUser);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_EditShow);
	DDX_Text(pDX, IDC_EDIT_SEND, m_EditSend);
	DDX_Check(pDX, IDC_CHECK_ONLY, m_bOnly);
	DDX_Check(pDX, IDC_CHECK_ME, m_bMe);
}

BEGIN_MESSAGE_MAP(CQQClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET_READ,OnSocketRead)
	ON_BN_CLICKED(IDC_BUTTON_TRANS, OnButtonTrans)
	ON_BN_CLICKED(IDC_BUTTON_SHAKE, OnButtonShake)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_RECV, OnButtonRecv)
END_MESSAGE_MAP()


// CQQClientDlg 消息处理程序

BOOL CQQClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	CLogin  LoginDlg;

	if (LoginDlg.DoModal()==IDOK)
	{

		//LoginDlg 获得ServerIP 姓名

		CString strName;
		CString strIP;   //从登陆界面中得到用户姓名和IP用于客户端显示谁谁上线



		strName = LoginDlg.m_strName;

		ULONG ulServerIP;                //127.0.0.1
		ulServerIP = inet_addr(LoginDlg.m_strIP);//字符型IP转32位IP
		//(LPCSTR)(CW2A)上面无法识别就加这个


		m_ClientSocket = socket(PF_INET,SOCK_STREAM,0);
		if(m_ClientSocket==INVALID_SOCKET)
		{
			AfxMessageBox("Create SOCKET Failed");
			WSACleanup(); 
			closesocket(m_ClientSocket);
			CQQClientDlg::OnCancel();
		}


		//初始服务器信息
		m_ServerAddr.sin_family = AF_INET;
		m_ServerAddr.sin_addr.s_addr = ulServerIP;
		m_ServerAddr.sin_port = htons(9527);//short 网络字节序


		//链接
		int bOk =
			connect(m_ClientSocket,(sockaddr*)&m_ServerAddr,sizeof(m_ServerAddr));

		if(bOk==SOCKET_ERROR)
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

		 
		   PlaySound(MAKEINTRESOURCE(IDR_WAVE),   //播放有音乐的文件
			AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);//PlaySound，参数，pszSound是指定了要播放声音的字符串，该参数可以是WAVE文件的名字，或是WAV资源的名字，或是内存中声音数据的指针，或是在系统注册表WIN.INI中定义的系统事件声音。如果该参数为NULL则停止正在播放的声音。
			SetWindowText(strName); //窗口顶端显示自己的用户名
	}

	else
	{
		CDialog::OnCancel();
	}
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

	m_ComBoUser.AddString("大家");
	m_ComBoUser.SetWindowText("大家");

	CButton* Temp; 
	HBITMAP  hBitmap;
	//整个在设置那个button中的Send图标
	Temp =(CButton *)GetDlgItem(IDC_BUTTON_TRANS);  //发送图标设置
	hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_SEND));
	Temp->SetBitmap(hBitmap);     

	
	Temp =(CButton *)GetDlgItem(IDC_BUTTON_SHAKE);  //窗口震动设置
	hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_LIGHT));
	Temp->SetBitmap(hBitmap);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQQClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQQClientDlg::OnPaint()
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
HCURSOR CQQClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
LRESULT CQQClientDlg::OnSocketRead(WPARAM wParam,LPARAM lParam)  //Read  Close
{

	int iEvent = WSAGETSELECTEVENT(lParam);

	switch(iEvent) 
	{ 
	case FD_CLOSE://服务器断开事件: 
		{			

			break;
		}

	case FD_READ:              //网络数据包到达事件 
		{			
			OnRecive(wParam);    
			break;
		}
	default: break; 
	} 
	return 0;
}
VOID CQQClientDlg::OnRecive(WPARAM wParam)
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


VOID CQQClientDlg::SendMsg(int iType,CString strName,CString strMsg)
{
	SENDINFOR SendInfor = {0};
	SendInfor.iType = iType;


	memcpy(SendInfor.szName,strName.LockBuffer(),strName.GetLength()+1); //加锁保证信息安全
	memcpy(SendInfor.szMsg,strMsg.LockBuffer(),strMsg.GetLength()+1);

	int bOk = send(m_ClientSocket,(char*)&SendInfor,sizeof(SENDINFOR),0); //得到用户姓名和传的消息一并发送
	
	if(bOk==SOCKET_ERROR)
	{
		MessageBox("发送过程中发生一个错误！",NULL,MB_OK);
		return;
	}
}

void CQQClientDlg::OnButtonTrans()
{
	UpdateData(TRUE);   

	CString Temp;
	m_ComBoUser.GetWindowText(Temp);
	if(Temp==m_strName)
	{
		MessageBox("不能给自己发送文件");
		return;
	}

	if (Temp=="大家")
	{
		MessageBox("不能给大家发送文件");
		return;
	}

	CFileDialog Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,  
		"所有文件 (*.*)|*.*||"); 


	if (Dlg.DoModal()==IDOK)
	{

		m_strFilePath = Dlg.GetPathName(); 
		m_strFileName = Dlg.GetFileName();

		CFile File(m_strFilePath,CFile::modeRead); 
		m_dwFileSize = File.GetLength();
		File.Close();

		m_strFileSize = KindChange(m_dwFileSize);

		//显示进度条

		m_Progress.ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_SHOW);

		m_ButtonCancle.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECV)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECV)->SetWindowText("发送文件：");

		GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(m_strFileName);
		GetDlgItem(IDC_STATIC_FILESIZE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FILESIZE)->SetWindowText(m_strFileSize);   //1025  1K

		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowText("0%");

		GetDlgItem(IDC_BUTTON_TRANS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("速度: 0");



		//向服务器发送数据包
		CString strMsg;
		CString strFileSize;
		strFileSize.Format("%d",m_dwFileSize);  //1025

		strMsg = m_strName+"|"+m_strFileName+"|"+strFileSize+"|";

		//11--12
		//5  12  11|1.txt|5|
		SendMsg(5,Temp,strMsg);



		strFileSize = KindChange(m_dwFileSize);

		strFileSize="("+strFileSize+")";


		m_EditShow+="\r\n\r\n";
		m_EditShow+="你请求给";
		m_EditShow+=Temp;
		m_EditShow+="发送文件 ";
		m_EditShow+=m_strFileName;
		m_EditShow+=strFileSize;

		UpdateData(FALSE);








		//5  关羽 张飞|1.txt|5|


		//		Sendto=m_strUser;
		//		IsTrans=1;
	}
}
void CQQClientDlg::OnButtonShake() 
{



	UpdateData(TRUE);
	CString  Data;
	CString Temp;
	m_ComBoUser.GetWindowText(Temp);   //得到被准备用户姓名或者说是类型
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
			Data+="对大家发送了一个抖动窗口";	//在对话框显示操作消息

			SendMsg(110,"",Data);


			m_EditShow+="\r\n";   
			m_EditShow+=m_Timer.LockBuffer();
			m_EditShow+="你对大家发送了一个抖动窗口";


			m_EditSend = "";
			//在自己窗口上更新
			UpdateData(FALSE);


		}
		else  //单独发送窗口震动
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




VOID CQQClientDlg::GetCurrentTime()
{

	SYSTEMTIME st = {0};
	GetLocalTime(&st);  
	int j = 0;
	j  = sprintf(m_Timer.LockBuffer(),"%d年",st.wYear);
	j += sprintf(m_Timer.LockBuffer()+j,"%d月",st.wMonth);
	j += sprintf(m_Timer.LockBuffer()+j,"%d日",st.wDay);
	j += sprintf(m_Timer.LockBuffer()+j,"%d时",st.wHour);

	if (st.wMinute<10) 
	{
		int i = 0;
		j += sprintf(m_Timer.LockBuffer()+j,"%d",i); 
	}
	j += sprintf(m_Timer.LockBuffer()+j,"%d分钟",st.wMinute);

	if (st.wSecond<10) 
	{
		int i = 0;
		j += sprintf(m_Timer.LockBuffer()+j,"%d",i); 
	}
	j += sprintf(m_Timer.LockBuffer()+j,"%d秒",st.wSecond);


}
void CQQClientDlg::Shaking()
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
CString CQQClientDlg::KindChange(DWORD dwFileSize)  
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
void CQQClientDlg::OnButtonSend() 
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

static DWORD WINAPI RecvFileThread(LPVOID lPParam) //12
{
	//访问CClientDlg 的成员
	CQQClientDlg* ClientDlg = (CQQClientDlg*)lPParam;


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
static DWORD WINAPI SendFileThread(LPVOID lPParam)  //11
{

	CQQClientDlg* ClientDlg = (CQQClientDlg*)lPParam;

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
void CQQClientDlg::HideItem()
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
void CQQClientDlg::AnalyseString(char* szBuffer)
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

void CQQClientDlg::OnButtonRecv() 
{                           //Server
	//启动接受线程    P2P   12 connect 11  Accept
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RecvFileThread,this,0,NULL);
}
void CQQClientDlg::OnButtonEnd() 
{

	CString strTemp;
	m_ButtonCancle.GetWindowText(strTemp);
	if (strTemp=="取消")
	{
		m_EditShow+="\r\n";
		m_EditShow+="你取消了文件传输!";
		UpdateData(FALSE);



		CString SendTo;
		m_ComBoUser.GetWindowText(SendTo);   //关羽
		SendMsg(7,SendTo,"对方取消了文件传输!");

		HideItem();
	}

	else if (strTemp=="拒绝")
	{
		m_EditShow+="\r\n";
		m_EditShow+="你拒绝了文件传输!";
		UpdateData(FALSE);


		CString SendFrom;

		SendFrom = m_strOtherName;    //5 192.168.0.100|11|1.txt|5|

		//7 11 对方拒绝了文件传输
		SendMsg(7,SendFrom,"对方拒绝了文件传输!");    //11->12    12 
		//7  张飞  对方取消了文件传输

		HideItem();
	}


	//5  12   192.168.0.111|11|1.txt|5|   ==>  m_strOtherFileName ClientDlg

}
BOOL CQQClientDlg::PreTranslateMessage(MSG* pMsg) 
{


	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
		{
			OnButtonSend();

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}