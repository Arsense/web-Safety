// MailPostDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MailPost.h"
#include "MailPostDlg.h"

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
// CMailPostDlg dialog

CMailPostDlg::CMailPostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailPostDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailPostDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMailPostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailPostDlg)
	DDX_Control(pDX, IDC_LIST_MAIL, m_lstMail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailPostDlg, CDialog)
	//{{AFX_MSG_MAP(CMailPostDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAIL, OnOpenMail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailPostDlg message handlers

BOOL CMailPostDlg::OnInitDialog()
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
	CString strText;
	this->SetDlgItemInt(IDC_PORT,110,FALSE);
	strText = _T("序号");
	m_lstMail.InsertColumn(0,strText,LVCFMT_CENTER,48);
	strText = _T("邮件主题");
	m_lstMail.InsertColumn(1,strText,LVCFMT_CENTER,318);
	strText = _T("大小");
	m_lstMail.InsertColumn(2,strText,LVCFMT_CENTER,48);

	CWnd *pWnd;
	pWnd = this->GetDlgItem(IDC_REVERSE);
	pWnd->EnableWindow(FALSE);
	pWnd = this->GetDlgItem(IDC_DELETE);
	pWnd->EnableWindow(FALSE);
	pWnd = this->GetDlgItem(IDC_DISCONNECT);
	pWnd->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMailPostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMailPostDlg::OnPaint() 
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
HCURSOR CMailPostDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMailPostDlg::OnReceive() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_ADDRESS,m_sAddr);
	GetDlgItemText(IDC_USERNAME,m_sUser);
	GetDlgItemText(IDC_PASSWORD,m_sPassword);

	if (m_sAddr.IsEmpty()||m_sUser.IsEmpty()||m_sPassword.IsEmpty())
	{
		AfxMessageBox(_T("对不起，你输入的pop3信息不全，请重新检查!"));
		return;
	}


	if (!m_pop3.Connect(m_sAddr,m_sUser,m_sPassword))
	{
		// 连接失败
		AfxMessageBox(_T("无法与服务器建立连接!"));
		return;
	}
	

	// 进行一次空操作
	if (!m_pop3.Noop())
	{
		AfxMessageBox(m_pop3.GetErrorString());
		return;
	}
	
	Refresh();

	// 更新界面
	GetDlgItem(IDC_REVERSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DISCONNECT)->EnableWindow(TRUE);
}

void CMailPostDlg::OnReverse() 
{
	// TODO: Add your control notification handler code here
	UINT nMails = m_pop3.GetMailCount();
	for (UINT nIndex = 0;nIndex < nMails;nIndex++)
	{
		if (m_lstMail.GetItemState(nIndex,LVIS_SELECTED))
			m_lstMail.SetItemState(nIndex,FALSE,LVIS_SELECTED);
		else
			m_lstMail.SetItemState(nIndex,TRUE,LVIS_SELECTED);
	}
}

void CMailPostDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstMail.GetFirstSelectedItemPosition();
	while (pos)
	{
		if (!m_pop3.Delete(m_lstMail.GetNextSelectedItem(pos) + 1))
		{
			return;
		}
	}
}

void CMailPostDlg::OnDisconnect() 
{
	// TODO: Add your control notification handler code here
	if (!m_pop3.Disconnect())
	{
		return;
	}

	// 重置用户界面状态
	GetDlgItem(IDC_REVERSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DISCONNECT)->EnableWindow(FALSE);
	m_lstMail.DeleteAllItems();
}

void CMailPostDlg::OnOpenMail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (m_lstMail.GetSelectedCount() != 1)
	{
		AfxMessageBox("请明确要打开的email!");
		return;
	}

	POSITION pos = m_lstMail.GetFirstSelectedItemPosition();
	UINT nIndex = m_lstMail.GetNextSelectedItem(pos);
	CString strMail;
	m_pop3.RetrieveMail(nIndex + 1,strMail);
	this->SetDlgItemText(IDC_EDIT_TEXT,strMail);

	*pResult = 0;
}

void CMailPostDlg::Refresh()
{
	// 查询邮箱状态
	if (!m_pop3.GetState())
	{
		// 发送失败
		AfxMessageBox(m_pop3.GetErrorString());
		return;
	}

	// 列举邮件大小
	if (!m_pop3.List())
	{
		// 发送失败
		AfxMessageBox(m_pop3.GetErrorString());
		return;
	}

	// 接收邮件的主题
	if (!m_pop3.ListHeader())
	{
		// 列举邮件主题失败
		AfxMessageBox(m_pop3.GetErrorString());
		return;
	}

	m_lstMail.DeleteAllItems();
	UINT nIndex,nMails = m_pop3.GetMailCount();
	CString strText;
	for (nIndex = 0;nIndex < nMails;nIndex++)
	{
		strText.Format("%d",nIndex + 1);
		m_lstMail.InsertItem(nIndex,strText);
		m_lstMail.SetItemText(nIndex,1,m_pop3.GetMailHeader(nIndex));
		strText.Format("%d",m_pop3.GetMsgSize(nIndex));
		m_lstMail.SetItemText(nIndex,2,strText);
	}
}
