// Example1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Example1.h"
#include "Example1Dlg.h"
#include "winsock.h"


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
// CExample1Dlg dialog

CExample1Dlg::CExample1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExample1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExample1Dlg)
	m_uPort = 4000;
	m_sShowText = _T("");
	m_sInputText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExample1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExample1Dlg)
	DDX_Control(pDX, IDC_SERVERIP, m_ServerIp);
	DDX_Text(pDX, IDC_PORT, m_uPort);
	DDV_MinMaxUInt(pDX, m_uPort, 1024, 100000);
	DDX_Text(pDX, IDC_SHOWTEXT, m_sShowText);
	DDX_Text(pDX, IDC_INPUTTEXT, m_sInputText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExample1Dlg, CDialog)
	//{{AFX_MSG_MAP(CExample1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SETSERVER, OnSetserver)
	ON_EN_UPDATE(IDC_INPUTTEXT,OnInputText)
	ON_MESSAGE(SER_MESSAGE,OnServerMessage)
	ON_MESSAGE(CLI_MESSAGE,OnClientMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExample1Dlg message handlers

BOOL CExample1Dlg::OnInitDialog()
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
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
	{
		TRACE("Initilize Error!\n");  //初始化失败
		AfxMessageBox("Failed in initial socket");
	}
//	m_connectionList.AssertValid();
	m_connectionList.RemoveAll();
	ASSERT(m_connectionList.GetCount()==0);
	m_bInit=FALSE;
	m_bClient=FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExample1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExample1Dlg::OnPaint() 
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
HCURSOR CExample1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExample1Dlg::OnConnect() 
{
	
	// TODO: Add your control notification handler code here
	BYTE  f0,f1,f2,f3;
	m_ServerIp.GetAddress(f0,f1,f2,f3);
	CString add;
	add.Format("%d.%d.%d.%d",f0,f1,f2,f3);
	
	if(m_client.InitAndConnet(m_hWnd,m_uPort,add)==FALSE)
		return;
}

void CExample1Dlg::OnSetserver() 
{
	
	// TODO: Add your control notification handler code here
	if(m_server.InitAndListen(this->m_hWnd,m_uPort)==FALSE)
	{
		return;
	}
	else
	{
		m_bInit=TRUE;
		m_bClient=FALSE;
		m_sShowText="Server has been set ok!\n";
		GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_sShowText);
	}
}

//Server消息相应函数
LRESULT CExample1Dlg::OnServerMessage(WPARAM wParam, LPARAM lParam)
{

	SOCKET socket,ts;
	int length,i,j;
	CString str="";
	CEdit * output=NULL;
	char s[1024];
	int len;
	switch(lParam)
	{
		case FD_ACCEPT:
			socket= accept(m_server.m_hSocket,NULL,NULL);
			length=m_connectionList.GetCount();
			for(i=0;i<length;i++)
			{
				ts=m_connectionList.GetAt(m_connectionList.FindIndex(i));
				s[0]=NULL;
				strcat(s,"一个游客进入了聊天室");
				send(ts,s,strlen(s),0);
			}
			m_sShowText+="\n";
			m_sShowText+="一个游客进入了聊天室了";
			GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_sShowText);
			output=(CEdit *)GetDlgItem(IDC_SHOWTEXT);
			//随时跟踪滚动条的位置
			output->LineScroll(output->GetLineCount());
			m_connectionList.AddHead(socket);
			return 0;

		case FD_READ:
			length=m_connectionList.GetCount();
			for(i=0;i<length;i++)
			{
				socket=m_connectionList.GetAt(m_connectionList.FindIndex(i));
				if(socket==wParam)
				{
					len=recv(socket,s,1024,0);
					s[len]=NULL;
					//将一个客户端发送的消息传递到其他客户端去
					for(j=0;j<length;j++)
					{
						socket=m_connectionList.GetAt(m_connectionList.FindIndex(j));
						if(socket!=wParam)
						{
							send(socket,s,strlen(s),0);
						}
					}
					m_sShowText=m_sShowText+s;
					GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_sShowText);
					CEdit * output=(CEdit *)GetDlgItem(IDC_SHOWTEXT);
					//随时跟踪滚动条的位置
					output->LineScroll(output->GetLineCount());
					return 0;
				}
			}
			return 0;

		case FD_WRITE:
			return 0;

		case FD_CLOSE:
			return 0;

		default:
			m_sShowText=m_sShowText+"An networking error has occured with a client";
			GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_sShowText);
			CEdit * output=(CEdit *)GetDlgItem(IDC_SHOWTEXT);
			//随时跟踪滚动条的位置
			output->LineScroll(output->GetLineCount());
			return 0;
	}
}

LRESULT CExample1Dlg::OnClientMessage(WPARAM wParam, LPARAM lParam)
{
	CEdit * output=NULL;
	char s[1024];
	int len;
	switch(lParam)
	{
		case FD_CONNECT:
			len=GetLastError();
			if(len!=0)
			{
				AfxMessageBox("Error in Connecting");
			}
			else
			{
				m_bInit=TRUE;
				m_bClient=TRUE;
				m_sShowText="Having connected to server!\n";
				GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_sShowText);
			}
			return 0;
		case FD_READ:
			len=recv(m_client.m_hSocket,s,1024,0);
			s[len]=NULL;
			m_sShowText+=s;
			//随时跟踪滚动条的位置 
			output=(CEdit *)GetDlgItem(IDC_SHOWTEXT);
			output->SetWindowText(m_sShowText);
			output->LineScroll(output->GetLineCount());
			return 0;
		case FD_WRITE:
			return 0;
		case FD_CLOSE:
			return 0;
		default:
			output=(CEdit *)GetDlgItem(IDC_SHOWTEXT);
			output->SetWindowText("An network error has occured,the connection is dropped");
			closesocket(m_client.m_hSocket);
			m_bInit=false;
			return 0;
	}
}

void CExample1Dlg::OnInputText()
{
	if(!m_bInit)
	{
		AfxMessageBox("The session has not been set correctly");
		return;
	}
	CString in;
	GetDlgItem(IDC_INPUTTEXT)->GetWindowText(in);
	//输入框中只有回车键被按下，所以不发送回车键字符
	if(in.GetLength()<1)
	{
		return;
	}

	if(in.GetAt(in.GetLength()-1)=='\n')
	{
		//in.TrimRight(" ")用来消去无用的空格键字符
		in.TrimRight(" ");
		//将输入框中的字符清空,表示字符已经被发送
		GetDlgItem(IDC_INPUTTEXT)->SetWindowText("");
		if(in.GetLength()>2)
		{
			m_sShowText+=in;
			//将输入的话显示到显示窗口中
			GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_sShowText);		
			CEdit * output=(CEdit *)GetDlgItem(IDC_SHOWTEXT);
			//随时跟踪滚动条的位置
			output->LineScroll(output->GetLineCount());

			//服务器端
			if(!m_bClient)
			{
				int length=m_connectionList.GetCount();
				SOCKET  socket;
				for(int i=0;i<length;i++)
				{
					socket=m_connectionList.GetAt(m_connectionList.FindIndex(i));
					send(socket,in.GetBuffer(0),in.GetLength(),0);
				}
			}
			//客户端
			else
			{
				m_client.SendString(in);
			}
		}
	}
	
}

