
#include "stdafx.h"
#include "NetTrafficButton.h"
#include "NetTrafficButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 自身的一个指针，这是为回调函数服务的
	CNetTrafficButtonDlg* me;
// 记录选择的接口号
	int SelectedInterface;
// 一个回调函数
	void CALLBACK interfaceHasChanged(int interfacenumber);


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

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
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetTrafficButtonDlg Dialogfield

CNetTrafficButtonDlg::CNetTrafficButtonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetTrafficButtonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetTrafficButtonDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//为回调函数服务的
	me = this;
}

void CNetTrafficButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetTrafficButtonDlg)
	DDX_Control(pDX, IDC_BUTTON3, m_cOutgoing);
	DDX_Control(pDX, IDC_BUTTON2, m_cIncoming);
	DDX_Control(pDX, IDC_BUTTON1, m_cTotalTraffic);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetTrafficButtonDlg, CDialog)
	//{{AFX_MSG_MAP(CNetTrafficButtonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON4, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetTrafficButtonDlg Message handler

BOOL CNetTrafficButtonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	// 使用第一个发现的网络连接
	int SelectedInterface	= 0;	
	// 每秒钟刷新一次网络状态
	UINT timerresolution	= 1000;	
	// To fake the Taskmanager....
	UINT gridresolution		= 100;	
	
	// 监视的接口
	m_cTotalTraffic.SetInterfaceNumber(SelectedInterface);
	// 这个button关注的时总流量
	m_cTotalTraffic.SelectTrafficType(MFTrafficButton::Traffic_Total);			
	// 设置得到通知的回调函数
	m_cTotalTraffic.SetInterfaceNumberNotificationFunction(interfaceHasChanged);
	//设置更新速率
	m_cTotalTraffic.SetUpdateSpeed(timerresolution, gridresolution);

	m_cOutgoing.SetInterfaceNumber(SelectedInterface);
	m_cOutgoing.SelectTrafficType(MFTrafficButton::Traffic_Outgoing);
	m_cOutgoing.SetInterfaceNumberNotificationFunction(interfaceHasChanged);
	m_cOutgoing.SetUpdateSpeed(timerresolution, gridresolution);

	m_cIncoming.SetInterfaceNumber(SelectedInterface);
	m_cIncoming.SelectTrafficType(MFTrafficButton::Traffic_Incoming);
	m_cIncoming.SetInterfaceNumberNotificationFunction(interfaceHasChanged);
	m_cIncoming.SetUpdateSpeed(timerresolution, gridresolution);

	
	return TRUE;  
}

void CNetTrafficButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CNetTrafficButtonDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CNetTrafficButtonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//回调函数
void CALLBACK interfaceHasChanged(int interfacenumber)
{
	SelectedInterface = interfacenumber;
	CString text;
	text.Format("NetTrafficButton using Interface: %d",SelectedInterface);
	me->SetWindowText(text);
}

void CNetTrafficButtonDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg about;
	about.DoModal();
	::AfxGetMainWnd();
}
