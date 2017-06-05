// SendEMailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SendEMail.h"
#include "SendEMailDlg.h"
#include "MessBox.h"

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
// CSendEMailDlg dialog

CSendEMailDlg::CSendEMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendEMailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendEMailDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendEMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendEMailDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSendEMailDlg, CDialog)
	//{{AFX_MSG_MAP(CSendEMailDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_SEND, OnSend)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_BN_CLICKED(ID_SHOW, OnShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendEMailDlg message handlers

BOOL CSendEMailDlg::OnInitDialog()
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
		AfxMessageBox("Socket Load Error");
	}

	if(m_socket.Create()==FALSE)
	{
		AfxMessageBox("Socket Create Error");
	}
	m_socket.Init(this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSendEMailDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSendEMailDlg::OnPaint() 
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
HCURSOR CSendEMailDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSendEMailDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_SMTP_ADDRESS,m_sSendString[0]);
	GetDlgItemText(IDC_FROM_ADDRESS,m_sSendString[1]);
	GetDlgItemText(IDC_TO_ADDRESS,m_sSendString[2]);

	m_sSendString[0].TrimLeft(" ");
	m_sSendString[0].TrimRight(" ");
	m_sSendString[1].TrimLeft(" ");
	m_sSendString[1].TrimRight(" ");
	m_sSendString[2].TrimLeft(" ");
	m_sSendString[2].TrimRight(" ");

	if(m_sSendString[0].IsEmpty())
	{
		AfxMessageBox("请输入SMTP服务器地址(IP)");
		return;
	}
	if(m_sSendString[1].IsEmpty())
	{
		AfxMessageBox("请输入你的EMail地址");
		return;
	}
	if(m_sSendString[2].IsEmpty())
	{
		AfxMessageBox("请输入收件人的EMail地址");
		return;
	}
	m_sSendString[0]="HELO " + m_sSendString[0] + "\r\n";
	m_sSendString[1]="MAIL FROM:" +m_sSendString[1] + "\r\n";
	m_sSendString[2]= "RCPT TO:" + m_sSendString[2] + "\r\n";

	m_sSendString[3]="DATA\r\n";

	GetDlgItemText(IDC_EMAIL_CONTENT,m_sSendString[4]);
	m_sSendString[4]=m_sSendString[4]+ "\r\n\r\n.\r\n";

	m_sSendString[5]="QUIT\r\n";

	CString temp;
	GetDlgItemText(IDC_SMTP_ADDRESS,temp);

	if(m_socket.Connect(LPCSTR(temp),25)==FALSE)
	{
		AfxMessageBox("Error connect to the SMTP Server");
		int i=GetLastError();
		switch(i)
		{
			case WSAENETDOWN:
				AfxMessageBox("WSAENETDOWN");
				break;
			case WSAEADDRINUSE:
				AfxMessageBox("WSAEADDRINUSE");
				break;
			case WSAEINPROGRESS   :
				AfxMessageBox("WSAEINPROGRESS   ");
				break;
			case WSAEADDRNOTAVAIL   :
				AfxMessageBox("WSAEADDRNOTAVAIL   ");
				break;
			case WSAECONNREFUSED      :
				AfxMessageBox("WSAECONNREFUSED      ");
				break;
			case WSAEDESTADDRREQ   :
				AfxMessageBox("WSAEDESTADDRREQ   ");
				break;
			case WSAEINVAL:
				AfxMessageBox("WSAEINVAL   ");
				break;
			case WSAETIMEDOUT:
				AfxMessageBox("WSAETIMEDOUT   ");
				break;
			case WSAEWOULDBLOCK:
				AfxMessageBox("WSAEWOULDBLOCK");
				break;
			default:
				break;
		}
	}
	m_sReceivedData="";
}

void CSendEMailDlg::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CDialog dlg(IDD_ABOUTBOX);
	dlg.DoModal();
}

void CSendEMailDlg::ReceiveMessage(int count)
{
	if(count>=6)
	{
		AfxMessageBox("Unknown Received Data");
		m_socket.Close();
		return;
	}
	char sBuffer[255];
	int len=m_socket.Receive(sBuffer,sizeof(sBuffer));
	sBuffer[len]=NULL;
	m_sReceivedData=m_sReceivedData+sBuffer;
	m_sReceivedData=m_sReceivedData+m_sSendString[count];
	m_socket.Send(m_sSendString[count],m_sSendString[count].GetLength());				
}

void CSendEMailDlg::OnShow() 
{
	// TODO: Add your control notification handler code here
	CMessBox dlg;
	dlg.m_sMessage=m_sReceivedData;
	dlg.DoModal();
}
