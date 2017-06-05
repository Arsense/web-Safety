// myWinInet_1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "myWinInet_1.h"
#include "myWinInet_1Dlg.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyWinInet_1Dlg dialog

CMyWinInet_1Dlg::CMyWinInet_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyWinInet_1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyWinInet_1Dlg)
	m_SiteInfo = _T("");
	m_SiteName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyWinInet_1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyWinInet_1Dlg)
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_SiteInfo);
	DDX_Text(pDX, IDC_EDIT_SITENAME, m_SiteName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyWinInet_1Dlg, CDialog)
	//{{AFX_MSG_MAP(CMyWinInet_1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyWinInet_1Dlg message handlers

BOOL CMyWinInet_1Dlg::OnInitDialog()
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
	//加入初始化信息
	m_SiteName="http://www.sohu.com";
	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyWinInet_1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyWinInet_1Dlg::OnPaint() 
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
HCURSOR CMyWinInet_1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}





void CMyWinInet_1Dlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
	// TODO: Add extra validation here
	//让控件和对应的变量之间进行数据交换，现在将控件数据传给对应变量
	UpdateData(true);
	//m_SiteInfo为控件ID_EDIT_CONTENT读应的变量
	m_SiteInfo="";
	//将变量值传给对应控件

	UpdateData(false);
	//CDialog::OnOK();
	CInternetSession mySession(NULL,0);
	CHttpFile* myHttpFile=NULL;
	m_SiteInfo="连接到站点"+m_SiteName+"\r\n";
	UpdateData(false);

	CString myData;
	myHttpFile=(CHttpFile*)mySession.OpenURL(m_SiteName);
	while(myHttpFile->ReadString(myData))
	{
		m_SiteInfo=m_SiteInfo+"\r\n";
		m_SiteInfo+=myData;
	}
	myHttpFile->Close ;
	mySession.Close ;
	UpdateData(false);


}
