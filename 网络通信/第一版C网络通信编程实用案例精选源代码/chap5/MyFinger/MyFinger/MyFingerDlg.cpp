// MyFingerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFinger.h"
#include "MyFingerDlg.h"
#include "afxsock.h"
#include <afxinet.h>

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
// CMyFingerDlg dialog

CMyFingerDlg::CMyFingerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyFingerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyFingerDlg)
	m_sHost = _T("");
	m_uPort = 79;
	m_sCommand = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyFingerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFingerDlg)
	DDX_Text(pDX, IDC_HOST, m_sHost);
	DDX_Text(pDX, IDC_PORT, m_uPort);
	DDV_MinMaxUInt(pDX, m_uPort, 1, 50000);
	DDX_Text(pDX, IDC_COMMAND, m_sCommand);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyFingerDlg, CDialog)
	//{{AFX_MSG_MAP(CMyFingerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_BN_CLICKED(IDC_QUERY_WHOIS, OnQueryWhois)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFingerDlg message handlers

BOOL CMyFingerDlg::OnInitDialog()
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
	if(AfxSocketInit(NULL)==FALSE)
	{
		AfxMessageBox("Sorry,socket init error!");
		return TRUE;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyFingerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyFingerDlg::OnPaint() 
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
HCURSOR CMyFingerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyFingerDlg::OnGo() 
{
	// TODO: Add your control notification handler code here
	//Finger method 1: 使用socket访问服务器
	//Finger method 2: 使用WinInet访问服务器

	//method 1
/*
	UpdateData();
	CString ip;
	hostent * host=gethostbyname(m_sHost);
	if(host==NULL)
	{
		AfxMessageBox("sorry, this host cannot be found at DNS!");
		return ;
	}

	struct in_addr inAddr;
	memmove(&inAddr,host->h_addr_list[0],4);

	ip.Format("%d.%d.%d.%d",inAddr.S_un.S_un_b.s_b1,inAddr.S_un.S_un_b.s_b2,inAddr.S_un.S_un_b.s_b3,inAddr.S_un.S_un_b.s_b4);
	CSocket socket;
	if(socket.Create()==FALSE)
	{
		AfxMessageBox("sorry, cannot create socket ");
		socket.Close();
		return ;
	}
	if(socket.Connect(ip,m_uPort)==FALSE)
	{
		AfxMessageBox("sorry, cannot connect to the server at the specific port!");
		socket.Close();
		return ;
	}

	CString com;
	com=m_sCommand+"\r\n";
	SetWindowText("正在发送数据到对方!");
	socket.Send(com,com.GetLength());
	SetWindowText("等待对方数据的到来!");

	char buffer[1024];
	memset(buffer,'\0',1024);
	int result;
	CWaitCursor cur;
	do
	{
		result=socket.Receive(buffer,1024);
		if(result==SOCKET_ERROR)
		{
			int d=GetLastError();
			if(d!=WSAEWOULDBLOCK)
			{
				AfxMessageBox("server cannot transfer correct information!");
				return;
			}
		}
	}while(result==SOCKET_ERROR);
	SetDlgItemText(IDC_RESULT,buffer);
	UpdateData();
*/
	//method 2:
	CString result;
	CInternetSession session;
	result+="Trying to Finger";
	result+="whitehouse.gov \r\n";
	CString host="whitehouse.gov";
	CGopherConnection *connection=NULL;
	try
	{
		connection=session.GetGopherConnection(host,NULL,NULL,79);
	}
	catch(CInternetException *pEx)
	{
		connection=NULL;
		pEx->Delete();
	}
	if(connection)
	{
		result+="Connection established.\r\n";
		CGopherLocator locator=connection->CreateLocator(NULL,NULL,GOPHER_TYPE_TEXT_FILE);
		CGopherFile	*file=connection->OpenFile(locator);
		if(file)
		{
			CString line;
			for(int i=0;i<20&&file->ReadString(line);i++)
			{
				result+=line+"\r\n";
			}
			file->Close();
			delete file;
		}
		connection->Close();
		delete connection;
	}
	else
	{
		result+="No Server Found there.\r\n";
	}
	result+="--------------------------\r\n";
	SetDlgItemText(IDC_RESULT,result);
}

void CMyFingerDlg::OnQueryWhois() 
{
	// TODO: Add your control notification handler code here

	CString result;
	CString host="rs.internic.net";
	CInternetSession session;
	result="Trying Whois for"+host+"\r\n";
	CGopherConnection *connection=NULL;
	try
	{
		connection=session.GetGopherConnection(host,NULL,NULL,43);
	}
	catch(CInternetException *pEx)
	{
		connection=NULL;
		pEx->Delete();
	}
	if(connection)
	{
		result+="connection established!\r\n";
		CGopherLocator locator =connection->CreateLocator(NULL,host,GOPHER_TYPE_TEXT_FILE);
		CGopherFile *file=connection->OpenFile(locator);
		if(file)
		{
			CString line;
			for(int i=0;i<20&&file->ReadString(line);i++)
			{
				result+=line+"\r\n";
			}
			file->Close();
			delete file;
		}
		connection->Close();
		delete connection;
	}
	else
	{
		result+="No Server Found there.";
	}
	SetDlgItemText(IDC_RESULT,result);
}
