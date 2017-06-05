// MySerialComDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySerialCom.h"
#include "MySerialComDlg.h"

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
// CMySerialComDlg dialog

CMySerialComDlg::CMySerialComDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySerialComDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySerialComDlg)
	m_com = _T("COM1");
	m_receive = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CMySerialComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySerialComDlg)
	DDX_Text(pDX, IDC_EDIT1, m_com);
	DDV_MaxChars(pDX, m_com, 4);
	DDX_Text(pDX, IDC_RECEIVE_EDIT, m_receive);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMySerialComDlg, CDialog)
	//{{AFX_MSG_MAP(CMySerialComDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySerialComDlg message handlers

BOOL CMySerialComDlg::OnInitDialog()
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
    SetTimer(1,500,0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMySerialComDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMySerialComDlg::OnPaint() 
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
		// TODO: Add extra initialization here

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMySerialComDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMySerialComDlg::OnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_com.MakeUpper();
	if(m_com!="COM1"&&m_com!="COM2"&&m_com!="COM3"&&m_com!="COM4")
	{
		AfxMessageBox("不正确的串口号，请重新输入串口号!");
		m_com="COM1";
		UpdateData();
		return;
	}
	if(m_open)
	{
		m_ComConfig.CloseConnection();
		m_open=FALSE;
	}
	char a[2]={m_com[3],'\0'};
	int i=atoi(a);
	m_ComConfig.Set(i);
	if(m_ComConfig.OpenConnection()==FALSE)
	{
		AfxMessageBox("无法打开该串口!");
		m_com="COM1";
		UpdateData(FALSE);
		return;
	}
	else
	{
		AfxMessageBox("该串口已经被成功打开!");
	}
	m_open=TRUE;
}

void CMySerialComDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	char buffer [1024];
	int length=GetDlgItemText(IDC_SEND_EDIT,buffer,1024);

	if(m_open)
	{
	  	if(::WriteCommBlock(m_ComConfig,buffer,length)==FALSE)
		{
			AfxMessageBox("没有连接串口线!");
		}
	}
	else
	{
		AfxMessageBox("该串口没有正确打开，请先设置串口!");
	}
}

void CMySerialComDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	char buffer[1024];
	buffer[0]='\0';
	if(::ReadCommBlock(m_ComConfig,buffer,1024)!=0)
	{
		m_receive+=buffer;
 		UpdateData(FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CMySerialComDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreCreateWindow(cs);
}

void CMySerialComDlg::OnOK() 
{
	// TODO: Add extra validation here
	KillTimer(1);
	if(m_ComConfig.IsConnected())
		m_ComConfig.CloseConnection();
	CDialog::OnOK();
}
