// Example2_ChatRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Example2_ChatRoom.h"
#include "Example2_ChatRoomDlg.h"
#include "afxsock.h"

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
// CExample2_ChatRoomDlg dialog

CExample2_ChatRoomDlg::CExample2_ChatRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExample2_ChatRoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExample2_ChatRoomDlg)
	m_sInputString = _T("");
	m_sShowString = _T("");
	m_uPort = 4000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExample2_ChatRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExample2_ChatRoomDlg)
	DDX_Text(pDX, IDC_INPUTTEXT, m_sInputString);
	DDV_MaxChars(pDX, m_sInputString, 1024);
	DDX_Text(pDX, IDC_SHOWTEXT, m_sShowString);
	DDX_Text(pDX, IDC_PORT, m_uPort);
	DDV_MinMaxUInt(pDX, m_uPort, 1024, 100000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExample2_ChatRoomDlg, CDialog)
	//{{AFX_MSG_MAP(CExample2_ChatRoomDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_INPUTTEXT,OnInputText)
	ON_BN_CLICKED(IDC_CONNECTSERVER, OnConnectserver)
	ON_BN_CLICKED(IDC_SETSERVER, OnSetserver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExample2_ChatRoomDlg message handlers

BOOL CExample2_ChatRoomDlg::OnInitDialog()
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
	if(AfxSocketInit(NULL)==0)
	{
		AfxMessageBox("CSocket Inital Error");
	}
	BYTE f0,f1,f2,f3;
	CString name;
	CClientSocket::GetLocalHostName(name);
	CClientSocket::GetIpAddress(name,f0,f1,f2,f3);
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->SetAddress(f0,f1,f2,f3);
	m_bInit=false;
	m_bClient=false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExample2_ChatRoomDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExample2_ChatRoomDlg::OnPaint() 
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
HCURSOR CExample2_ChatRoomDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExample2_ChatRoomDlg::OnInputText()
{
	if(!m_bInit)
	{
		AfxMessageBox("Not Connection to The Server");
		return;
	}
	CString in;
	CMessg msg;
	GetDlgItemText(IDC_INPUTTEXT,in);
	if(in.GetLength()<1)
	{
		return;
	}
	if(in.GetAt(in.GetLength()-1)=='\n')
	{
		in.TrimRight(" ");
		SetDlgItemText(IDC_INPUTTEXT,"");
		if(in.GetLength()>2)
		{
			m_sMsgList+=in;
			
			SetDlgItemText(IDC_SHOWTEXT,m_sMsgList);
			int m_iLineCurrentPos=((CEdit *)(GetDlgItem(IDC_SHOWTEXT)))->GetLineCount();
			((CEdit *)(GetDlgItem(IDC_SHOWTEXT)))->LineScroll(m_iLineCurrentPos);
			msg.m_strText=in;
			if(!m_bClient)
			{
				POSITION pos;
				for(pos=m_connectionList.GetHeadPosition();pos!=NULL;)
				{
					CClientSocket * t= (CClientSocket *)m_connectionList.GetNext(pos);
					t->SendMessage(&msg);
				}
			}
			else
			{
				m_clientsocket.SendMessage(&msg);
			}
		}
	}
}



void CExample2_ChatRoomDlg::ProcessPendingAccept()
{
	CClientSocket* pSocket = new CClientSocket();

	if (m_pListenSocket.Accept(*pSocket))
	{
		CMessg msg;
		msg.m_strText="一个游客进入聊天室了";
		m_sShowString+="一个游客进入聊天室了\n";
		POSITION pos;
		for(pos=m_connectionList.GetHeadPosition();pos!=NULL;)
		{
			CClientSocket * t= (CClientSocket *)m_connectionList.GetNext(pos);
			t->SendMessage(&msg);
		}
		pSocket->Init(this);
		m_connectionList.AddTail(pSocket);
	}
	else
		delete pSocket;
}


void CExample2_ChatRoomDlg::ClearContent()
{
	if(m_bClient)
	{
		m_clientsocket.Close();
	}
	else
	{
		m_pListenSocket.Close();
		m_connectionList.RemoveAll();
	}
	m_bInit=false;
	m_sMsgList="";
	SetDlgItemText(IDC_INPUTTEXT,"");
	SetDlgItemText(IDC_SHOWTEXT,"");
}

void CExample2_ChatRoomDlg::OnClearconnection() 
{
	ClearContent();
}

void CExample2_ChatRoomDlg::CloseSessionSocket()
{
	if(!m_bClient)
	{
		for(POSITION pos=m_connectionList.GetHeadPosition();pos!=NULL;)
		{
			POSITION t_pos=pos;
			CClientSocket * t = (CClientSocket*)m_connectionList.GetNext(pos);
			if(t->m_bClose==true)
			{
				m_connectionList.RemoveAt(t_pos);
			}
		}
	}
}

void CExample2_ChatRoomDlg::OnConnectserver() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit)
	{
		BYTE f0,f1,f2,f3;
		CString name;
		((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->GetAddress(f0,f1,f2,f3);
		CString ip;
		ip.Format("%d.%d.%d.%d",f0,f1,f2,f3);
		m_bClient=true;
		m_clientsocket.Create();
		if(m_clientsocket.Connect(ip,GetDlgItemInt(IDC_PORT)))
		{
			m_clientsocket.Init(this);
			SetDlgItemText(IDC_SHOWTEXT,"Client Connection Succeed");
			m_bInit=true;
		}
		else
		{
			m_clientsocket.Close();
			AfxMessageBox("client connection failed");
			m_bInit=false;
		}
	}	
}

void CExample2_ChatRoomDlg::OnSetserver() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit)
	{
		m_bClient=false;
		m_bInit=true;
		if(m_pListenSocket.Init(GetDlgItemInt(IDC_PORT),this)==FALSE)
		{
			m_bInit=false;
			return;
		}
	}
	
}
