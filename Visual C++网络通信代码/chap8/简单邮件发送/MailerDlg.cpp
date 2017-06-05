// MailerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mailer.h"
#include "MailerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailerDlg dialog

CMailerDlg::CMailerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailerDlg)
	m_From = _T("you@xx.xx");
	m_Message = _T(":)");
	m_Server = _T("xxx.xxx.xxx.xxx");
	m_To = _T("to@xx.xx");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMailerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailerDlg)
	DDX_Text(pDX, IDC_EDIT_FROM, m_From);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_Server);
	DDX_Text(pDX, IDC_EDIT_TO, m_To);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailerDlg, CDialog)
	//{{AFX_MSG_MAP(CMailerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailerDlg message handlers

BOOL CMailerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMailerDlg::OnPaint() 
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
HCURSOR CMailerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 单击"发送"按钮后发送邮件到目标地址
void CMailerDlg::OnOK() 
{
	UpdateData(TRUE);
	//得到服务器地址，不过注意这里最好是IP地址
	CString Serv = "HELO " + m_Server + "\r\n";
	//发送者的地址
	CString From = "MAIL FROM:<" + m_From + ">\r\n";
	//收信者地址
	CString To = "RCPT TO:<" + m_To + ">\r\n";
	//发送邮件的文本内容
	CString Text = m_Message + "\r\n\r\n.\r\n";

	char *MailMessage[] = 
	{
		Serv.GetBuffer(1),
		From.GetBuffer(1),
		To.GetBuffer(1),
		"DATA\r\n",
		Text.GetBuffer(1),
		"QUIT\r\n",
		NULL
	};

	WSADATA	Wsa;
	
	//进行WINSOCK的设置
	WSAStartup(0x0101,&Wsa);
	SOCKET s = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN	sin;
	sin.sin_addr.s_addr = inet_addr(m_Server);
	sin.sin_family = AF_INET;
	//注意邮件服务器的侦听端口
	sin.sin_port = htons(25);

	if(connect(s,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR)
		MessageBox("Error: can't connect to server :(","Error",MB_OK|MB_ICONERROR);


	int iLength = 0;
	int iEnd = 0;
	char sBuff[255] = "";
	int iMsg = 0;
	
	//循环发送内容，直到要发送的内容发送完毕
	do
	{
		iLength = recv(s,(LPSTR)sBuff+iEnd, sizeof(sBuff)-iEnd,0);
		iEnd += iLength;
		sBuff[iEnd] = '\0';
		send(s,(LPSTR)MailMessage[iMsg],strlen(MailMessage[iMsg]),0);
		iMsg++;
	}while(MailMessage[iMsg]);
	
	//关闭连接
	closesocket(s);
	WSACleanup();
}

void CMailerDlg::OnAbout() 
{

}
