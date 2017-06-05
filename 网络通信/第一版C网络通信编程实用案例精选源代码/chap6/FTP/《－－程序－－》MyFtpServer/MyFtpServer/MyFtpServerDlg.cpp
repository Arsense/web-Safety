// MyFtpServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFtpServer.h"
#include "MyFtpServerDlg.h"
#include "AccountDialog.h"
#include "Server.h"
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
// CMyFtpServerDlg dialog

CMyFtpServerDlg::CMyFtpServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyFtpServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyFtpServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyFtpServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFtpServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyFtpServerDlg, CDialog)
	//{{AFX_MSG_MAP(CMyFtpServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_ACCOUNT, OnAddAccount)
	ON_BN_CLICKED(IDC_DEL_ACCOUNT, OnDelAccount)
	ON_BN_CLICKED(IDC_REFRESH_PORT, OnRefreshPort)
	ON_BN_CLICKED(IDC_START_SERVER, OnStartServer)
	ON_BN_CLICKED(IDC_REFRESH_DEFAULTDIRECTORY, OnRefreshDefaultdirectory)
	ON_BN_CLICKED(IDC_ACCOUNT_INFOR, OnAccountInfor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFtpServerDlg message handlers

BOOL CMyFtpServerDlg::OnInitDialog()
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
	m_port=21;
	m_default_directory="c:\\temp";
	SetDlgItemInt(IDC_PORT,m_port);
	SetDlgItemText(IDC_DEFAULT_DIRECTORY,m_default_directory);

	CAccount a;
	a.account="anonymous";
	a.password="anonymous";
	a.directory=m_default_directory;
	m_AccountArray.Add(a);
	CListBox *list=(CListBox *)GetDlgItem(IDC_ACCOUNT_LIST);
	list->AddString(a.account);
	
	if(AfxSocketInit(NULL)==FALSE)
	{
		AfxMessageBox("sorry, socket cannot init!");
		return FALSE;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyFtpServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyFtpServerDlg::OnPaint() 
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
HCURSOR CMyFtpServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyFtpServerDlg::OnAddAccount() 
{
	// TODO: Add your control notification handler code here
	CAccountDialog dlg;
	dlg.m_directory=m_default_directory;
	while(dlg.DoModal()==IDOK)
	{
		if(dlg.m_user==""||dlg.m_password==""||dlg.m_directory=="")
		{
			AfxMessageBox("帐号,密码,FTP访问路径不能为空!");
		}
		else
		{
			int length=m_AccountArray.GetSize();
			BOOL exist=FALSE;
			for(int i=0;i<length;i++)
			{
				if(m_AccountArray[i].account==dlg.m_user)
				{
					AfxMessageBox("该帐号已经存在，请用其它帐号!");
					exist=TRUE;
					break;
				}
			}
			if(exist==TRUE)
				continue;
			CAccount a;
			a.account=dlg.m_user;
			a.password=dlg.m_password;
			a.directory=dlg.m_directory;
			m_AccountArray.Add(a);
			CListBox * list=(CListBox*)GetDlgItem(IDC_ACCOUNT_LIST);
			list->AddString(a.account);
			break;
		}
	}
}

void CMyFtpServerDlg::OnDelAccount() 
{
	// TODO: Add your control notification handler code here
	CListBox * list=(CListBox *)GetDlgItem(IDC_ACCOUNT_LIST);
	int sel=list->GetCurSel();
	if(sel!=LB_ERR)
	{
		CString account;
		list->GetText(sel,account);
		int length=m_AccountArray.GetSize();
		for(int i=0;i<length;i++)
		{
			if(m_AccountArray[i].account==account)
			{
				for(int j=i+1;j<length;j++)
				{
					m_AccountArray[j-1]=m_AccountArray[j];
				}
				m_AccountArray.RemoveAt(length-1);
				break;
			}
			ASSERT(i!=length-1);
		}
		list->DeleteString(sel);
	}
	else
	{
		if(list->GetCount()!=0)
		{
			AfxMessageBox("要想删除帐号，请先从左边的列表中选择要删除的项!");
		}
		else
		{
			AfxMessageBox("没有可以删除的帐号!");
		}
	}
}

void CMyFtpServerDlg::OnRefreshPort() 
{
	// TODO: Add your control notification handler code here
	int port=GetDlgItemInt(IDC_PORT);
	if(port<=0)
	{
		AfxMessageBox("不正确的端口号!");
	}
	else
	{
		m_port=port;
	}
}

void CMyFtpServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CMyFtpServerDlg::OnStartServer() 
{
	// TODO: Add your control notification handler code here
	static bool start=TRUE;
	CAccount a;
	if(start)
	{
		int i,length;
		CListBox *list=(CListBox*)GetDlgItem(IDC_ACCOUNT_LIST);
		i=list->GetCurSel();
		if(i!=LB_ERR)
		{
			start=FALSE;
			CString account;
			list->GetText(i,account);
			length=m_AccountArray.GetSize();
			for(int i=0;i<length;i++)
			{
				if(m_AccountArray[i].account==account)
				{
					a=m_AccountArray[i];
					break;
				}
			}
			GetDlgItem(IDC_START_SERVER)->SetWindowText("终止服务!");
			//---开始服务
			CString welcome;
			GetDlgItemText(IDC_WELCOME,welcome);
			m_server.SetSerConfigInfor(LPCSTR(a.account),LPCSTR(a.password),LPCSTR(a.directory),LPCSTR(welcome),(UINT)GetDlgItemInt(IDC_PORT));
			AfxBeginThread(ServerThread,LPVOID(&m_server));
		}
		else
		{
			AfxMessageBox("请先选择可以登陆的用户!");
			start=TRUE;
			return;
		}
	}
	else
	{
		start=TRUE;
		m_server.m_bStop=TRUE;
		GetDlgItem(IDC_START_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_SERVER)->SetWindowText("开始服务!");
		Sleep(1000);
		GetDlgItem(IDC_START_SERVER)->EnableWindow(TRUE);
	}
}

void CMyFtpServerDlg::OnRefreshDefaultdirectory() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_DEFAULT_DIRECTORY,m_default_directory);
}

void CMyFtpServerDlg::OnAccountInfor() 
{
	// TODO: Add your control notification handler code here
	CListBox *list=(CListBox *)GetDlgItem(IDC_ACCOUNT_LIST);
	int sel=list->GetCurSel();
	if(sel!=LB_ERR)
	{
		int i;
		CString user;
		list->GetText(sel,user);
		int length=m_AccountArray.GetSize();
		CAccount account;
		for(i=0;i<length;i++)
		{
			if(user==m_AccountArray[i].account)
			{
				account.account=m_AccountArray[i].account;
				account.directory=m_AccountArray[i].directory;
				account.password=m_AccountArray[i].password;
				break;
			}
		}
		CAccountDialog dlg;
		dlg.m_user=account.account;
		dlg.m_password=account.password;
		dlg.m_directory=account.directory;
		while(dlg.DoModal()==IDOK)
		{
			if(dlg.m_user==""||dlg.m_password==""||dlg.m_directory=="")
			{
				AfxMessageBox("更改的帐号,密码,FTP访问路径不能为空!");
				dlg.m_user=account.account;
				dlg.m_password=account.password;
				dlg.m_directory=account.directory;
				continue;
			}
			if(dlg.m_user!=account.account||dlg.m_password!=account.password||dlg.m_directory!=account.directory)
			{
				if(AfxMessageBox("确定你所作的修改吗?",MB_YESNO|MB_ICONSTOP)==IDYES)
				{
					account.account=dlg.m_user;
					account.directory=dlg.m_directory;
					account.password=dlg.m_password;
					m_AccountArray[i]=account;
					list->DeleteString(sel);
					list->AddString(account.account);
				}
			}
			break;
		}
	}
	else
	{
		if(list->GetCount()!=0)
		{
			AfxMessageBox("要想查询帐号，请先从左边的列表中选择要查询的项!");
		}
		else
		{
			AfxMessageBox("没有可查询的帐号!");
		}
	}
}
