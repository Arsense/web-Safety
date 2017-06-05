// MyPortScannerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyPortScanner.h"
#include "MyPortScannerDlg.h"
#include <afxmt.h>
#include "Afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CCriticalSection global_criticalsection;

UINT MyScanThread(LPVOID pParam);
struct THREADPARAM
{
	unsigned int m_BBeginSegAddress[4];
	unsigned int m_BEndSegAddress[4];
	CMyPortScannerDlg *const dlg;
	THREADPARAM(CMyPortScannerDlg *d):dlg(d)
	{
		dlg->SetWindowText("Win!");
	}
};
THREADPARAM *param=NULL;

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
// CMyPortScannerDlg dialog

CMyPortScannerDlg::CMyPortScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyPortScannerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyPortScannerDlg)
	m_uPortBegin = 1;
	m_uPortEnd = 100;
	m_uThreadNum = 1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyPortScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPortScannerDlg)
	DDX_Text(pDX, IDC_PORT_BEGIN, m_uPortBegin);
	DDV_MinMaxUInt(pDX, m_uPortBegin, 1, 30000);
	DDX_Text(pDX, IDC_PORT_END, m_uPortEnd);
	DDX_Text(pDX, IDC_THREAD_NUM, m_uThreadNum);
	DDV_MinMaxUInt(pDX, m_uThreadNum, 1, 1024);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPortScannerDlg, CDialog)
	//{{AFX_MSG_MAP(CMyPortScannerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BEGIN_SCAN, OnBeginScan)
	ON_BN_CLICKED(IDC_STOP_SCAN, OnStopScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPortScannerDlg message handlers

BOOL CMyPortScannerDlg::OnInitDialog()
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
		AfxMessageBox("Sorry,socket 失败!");
	}
	param = new THREADPARAM(this);
	m_bStop=FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyPortScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyPortScannerDlg::OnPaint() 
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
HCURSOR CMyPortScannerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyPortScannerDlg::OnBeginScan() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	unsigned int b,e,i;
	CIPAddressCtrl * ctrl=(CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BEGIN);
	BYTE a[4];

	ctrl->GetAddress(a[0],a[1],a[2],a[3]);
	for(i=0;i<4;i++)
		m_BBeginAddress[i]=(unsigned int)a[i];
	ctrl=(CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_END);
	ctrl->GetAddress(a[0],a[1],a[2],a[3]);
	for(i=0;i<4;i++)
		m_BEndAddress[i]=(unsigned int)a[i];


	b=(m_BBeginAddress[0]<<24)+(m_BBeginAddress[1]<<16)+
		(m_BBeginAddress[2]<<8)+m_BBeginAddress[3];
	e=(m_BEndAddress[0]<<24)+(m_BEndAddress[1]<<16)+
		(m_BEndAddress[2]<<8)+ m_BEndAddress[3];
	if(b>e)
	{
		AfxMessageBox("请重新设置扫描的IP地址，起始地址应小于终止地址!");
		return;
	}

	int d = (e-b)/m_uThreadNum;
	int p = (e-b)%m_uThreadNum;
	unsigned int w=b;

	if(d==0)	//地址数少于线程数
	{
		for(i=0;i<e-b+1;i++)
		{
			param->m_BBeginSegAddress[0]=(w&(255<<24))>>24;
			param->m_BBeginSegAddress[1]=(w&(255<<16))>>16;
			param->m_BBeginSegAddress[2]=(w&(255<<8))>>8;
			param->m_BBeginSegAddress[3]=w&255;
			w+=d;
			param->m_BEndSegAddress[0]=(w&(255<<24))>>24;
			param->m_BEndSegAddress[1]=(w&(255<<16))>>16;
			param->m_BEndSegAddress[2]=(w&(255<<8))>>8;
			param->m_BEndSegAddress[3]=w&255;
			AfxBeginThread(MyScanThread,(LPVOID)param);
		}
		return;
	}

	//地址数大于等于线程数
	for(i=1;i<=m_uThreadNum;i++)
	{
		TRACE("%d --%d\n",w&(255<<16),(w&(255<<16))>>16);
		TRACE("%d --%d\n",w&(255<<8),(w&(255<<16))>>16);
		TRACE("%d \n",w&255);

		param->m_BBeginSegAddress[0]=(w&(255<<24))>>24;
		param->m_BBeginSegAddress[1]=(w&(255<<16))>>16;
		param->m_BBeginSegAddress[2]=(w&(255<<8))>>8;
		param->m_BBeginSegAddress[3]=w&255;

		if(i==m_uThreadNum)
			w=w+d+p;
		else
			w+=d;

		TRACE("%d --%d\n",w&(255<<16),(w&(255<<16))>>16);
		TRACE("%d --%d\n",w&(255<<8),(w&(255<<16))>>16);
		TRACE("%d \n",w&255);

		param->m_BEndSegAddress[0]=(w&(255<<24))>>24;
		param->m_BEndSegAddress[1]=(w&(255<<16))>>16;
		param->m_BEndSegAddress[2]=(w&(255<<8))>>8;
		param->m_BEndSegAddress[3]=w&255;
		AfxBeginThread(MyScanThread,(LPVOID)param);
	}
}

void CMyPortScannerDlg::OnStopScan() 
{
	// TODO: Add your control notification handler code here
	m_bStop=TRUE;
}


UINT MyScanThread(LPVOID pParam)
{
	THREADPARAM* param=(THREADPARAM*)pParam;
	ASSERT(pParam);
	CMyPortScannerDlg *dlg=param->dlg;
	int currentip[4];
	UINT port;
	CString ip;
	CString result;
	unsigned int b,e,c;

	b=(param->m_BBeginSegAddress[0]<<24)+(param->m_BBeginSegAddress[1]<<16)+
		(param->m_BBeginSegAddress[2]<<8)+param->m_BBeginSegAddress[3];
	e=(param->m_BEndSegAddress[0]<<24)+(param->m_BEndSegAddress[1]<<16)+
		(param->m_BEndSegAddress[2]<<8)+param->m_BEndSegAddress[3];
	for(c=b;c<=e;c++)
	{
		currentip[0]=(c&(255<<24))>>24;
		currentip[1]=(c&(255<<16))>>16;
		currentip[2]=(c&(255<<8))>>8;
		currentip[3]=c&255;
		ip.Format("%d.%d.%d.%d",currentip[0],currentip[1],currentip[2],currentip[3]);
		result="";
		dlg->SetWindowText("开始扫描 "+ip);
		for(port=dlg->m_uPortBegin;port<=dlg->m_uPortEnd;port++)
		{
			if(dlg->m_bStop)
			{
				global_criticalsection.Lock();
				dlg->m_SResult+=result;
				dlg->SetDlgItemText(IDC_RESULT,dlg->m_SResult);
				global_criticalsection.Unlock();
				break;
			}
			CSocket socket;
			if(socket.Create()==FALSE)
			{
				result+="socket create error!\n";
				dlg->SetDlgItemText(IDC_RESULT,result);
				AfxMessageBox("socket create error!");
				return 0;
			}
			if(socket.Connect(ip,port)==FALSE)
			{
				socket.Close();
				continue;
			}
			char buffer[256];
			memset(buffer,'\0',256);
			if(socket.Receive(buffer,256,0)==SOCKET_ERROR)
			{
				socket.Close();
				continue;
			}
			CString p;
			p.Format("%d",port);
			result=result+ip+" 端口:"+p+"的响应信息为:"+buffer+"\r\n";
			global_criticalsection.Lock();
			dlg->m_SResult+=result;
			dlg->SetDlgItemText(IDC_RESULT,dlg->m_SResult);
			global_criticalsection.Unlock();
		}
	}
	dlg->SetWindowText("扫描结束!");
	return 0;
}

