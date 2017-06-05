// MSCOMMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSCOMM.h"
#include "MSCOMMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ConfigureDlg.h"

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
// CMSCOMMDlg dialog

CMSCOMMDlg::CMSCOMMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMSCOMMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMSCOMMDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMSCOMMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSCOMMDlg)
	DDX_Control(pDX, IDC_MSCOMM1, m_Comm);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMSCOMMDlg, CDialog)
	//{{AFX_MSG_MAP(CMSCOMMDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_RECEIVE, OnStartReceive)
	ON_BN_CLICKED(IDC_STOP_RECEIVE, OnStopReceive)
	ON_BN_CLICKED(IDC_SEND_DATA, OnSendData)
	ON_BN_CLICKED(IDC_SET_COMM, OnSetComm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSCOMMDlg message handlers

BOOL CMSCOMMDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMSCOMMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMSCOMMDlg::OnPaint() 
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
HCURSOR CMSCOMMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CMSCOMMDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMSCOMMDlg)
	ON_EVENT(CMSCOMMDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMSCOMMDlg::OnOnCommMscomm1() 
{
	// TODO: Add your control notification handler code here
	VARIANT m_input1; 
	COleSafeArray m_input2; 
	LONG length,i; 
	BYTE data[1024]; 
	CString str; 
	
	//接收缓冲区内字符 
	if(m_Comm.GetCommEvent()== 2)  
	{ 
		m_comdata.Empty(); 
		//读取缓冲区内的数据 
		m_input1=m_Comm.GetInput();  
		//将VARIANT型变量转换为ColeSafeArray型变量 
		m_input2=m_input1;  
		//确定数据长度 
		length=m_input2.GetOneDimSize();  
		//将数据转换为BYTE型数组 
		for(i=0;i< length;i++) 
			m_input2.GetElement(&i,data+i);  
		data[i]='\0';
		//将数组转换为CString型变量 
		str.Format("%s",data);
		m_comdata=m_comdata+str+"\n";
	
	/*在上面的程序中，先将VARIANT型变量 
	转换为ColeSafeArray型变量，再将其转换为 
	BYTE型数组，然后将数组转换为CString型变量， 
	以满足不同的变量类型显示数据的需要。*/ 
	}
}


void CMSCOMMDlg::OnStartReceive() 
{
	// TODO: Add your control notification handler code here
	//判断串口的状态，如果是关闭状态，则打开
	if(m_Comm.GetPortOpen()==FALSE)
	{
		//打开串口 
		m_Comm.SetPortOpen(TRUE);  
	}
}

void CMSCOMMDlg::OnStopReceive() 
{
	// TODO: Add your control notification handler code here
	//判断串口的状态，如果是打开状态，则关闭 
	if(m_Comm.GetPortOpen()==TRUE)
	{
		m_Comm.SetPortOpen(FALSE);  
	}
}

void CMSCOMMDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
	//判断串口的状态，如果是关闭状态，则打开
	if(m_Comm.GetPortOpen()==FALSE)
	{
		//打开串口 
		m_Comm.SetPortOpen(TRUE);  
	}

	VARIANT input1; 
	COleSafeArray input2; 
	LONG length,i; 
	CString str; 
	
	GetDlgItemText(IDC_SENDDATA_EDIT,str);
	length=str.GetLength();
	for(i=0;i<length;i++)
	{
		BYTE a=str.GetAt(i);
		input2.PutElement(&i,&a);  
	}
	input1=input2;
	m_Comm.SetOutput(input1);

	
}

void CMSCOMMDlg::OnSetComm() 
{
	// TODO: Add your control notification handler code here
	
	CConfigureDlg config;

	config.m_COMPort=m_Comm.GetCommPort();
	config.m_DTREnable=m_Comm.GetDTREnable();
	config.m_HandShaking=m_Comm.GetHandshaking();
	config.m_InBufferSize=m_Comm.GetInBufferSize();
	config.m_InputMode=m_Comm.GetInputMode();
	config.m_NULLDiscard=m_Comm.GetNullDiscard();
	config.m_OutBufferSize=m_Comm.GetOutBufferSize();
	config.m_ParityReplace=m_Comm.GetParityReplace();
	config.m_RThreshold=m_Comm.GetRThreshold();
	config.m_RTSEnable=m_Comm.GetRTSEnable();
	config.m_Setting=m_Comm.GetSettings();
	config.m_SThreshold=m_Comm.GetSThreshold();

	if(config.DoModal()==IDOK)
	{
		m_Comm.SetCommPort(config.m_COMPort);
		m_Comm.SetDTREnable(config.m_DTREnable);
		m_Comm.SetHandshaking(config.m_HandShaking);
		m_Comm.SetInBufferSize(config.m_InBufferSize);
		m_Comm.SetInputMode(config.m_InputMode);
		m_Comm.SetNullDiscard(config.m_NULLDiscard);
		m_Comm.SetOutBufferSize(config.m_OutBufferSize);
		m_Comm.SetParityReplace(config.m_ParityReplace);
		m_Comm.SetRThreshold(config.m_RThreshold);
		m_Comm.SetRTSEnable(config.m_RTSEnable);
		m_Comm.SetSettings(config.m_Setting);
		m_Comm.SetSThreshold(config.m_SThreshold);
	}

}
