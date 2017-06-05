// SerialCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialComm.h"
#include "SerialCommDlg.h"

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
// CSerialCommDlg dialog

CSerialCommDlg::CSerialCommDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSerialCommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialCommDlg)
	m_szSend = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialCommDlg)
	DDX_Control(pDX, IDC_DATARX, m_DataRx);
	DDX_Text(pDX, IDC_SEND, m_szSend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialCommDlg, CDialog)
//{{AFX_MSG_MAP(CSerialCommDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_READAVAILABLE, OnReadavailable)
	ON_BN_CLICKED(IDC_READ_N, OnReadN)
	ON_BN_CLICKED(IDC_READ_1, OnRead1)
	ON_BN_CLICKED(IDC_READ_UPTO, OnReadUpto)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_EN_CHANGE(IDC_CHAR, OnChangeChar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialCommDlg message handlers

BOOL CSerialCommDlg::OnInitDialog()
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

	SetDlgItemInt(IDC_N,5);
	CComboBox *com=(CComboBox*)GetDlgItem(IDC_COMSELECT);
	com->AddString("COM1");
	com->AddString("COM2");
	com->AddString("COM3");
	com->AddString("COM4");
	com->SetCurSel(0);
	m_bReady=FALSE;
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialCommDlg::OnPaint() 
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
HCURSOR CSerialCommDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialCommDlg::OnOpen() 
{
	CComboBox *com=(CComboBox*)GetDlgItem(IDC_COMSELECT);
	int i=com->GetCurSel()+1;
	CString scom;
	scom.Format("%d",i);
	scom="COM"+scom;
	if(m_theCommPort.Init (LPCSTR(scom),9600,0,1,8)==S_OK)
	{
		m_bReady=TRUE;
	}
	m_theCommPort.Start ();

}

void CSerialCommDlg::OnWrite() 
{
	
	UpdateData ();
	if(m_bReady==FALSE)
		AfxMessageBox("串口没有正确设置");
	else
	{
	   	m_theCommPort.Write(m_szSend, m_szSend.GetLength () );	
	}
}

void CSerialCommDlg::OnReadavailable() 
{
	if(m_bReady==FALSE)
		AfxMessageBox("串口没有正确设置");
	else
	{
		std::string szData;
		m_theCommPort.ReadAvailable (szData);	
		m_DataRx.SetWindowText ( szData.c_str() );
	}
}

void CSerialCommDlg::OnReadN() 
{
	if(m_bReady==FALSE)
		AfxMessageBox("串口没有正确设置");
	else
	{
		std::string szData;
		m_theCommPort.Read_N (szData,GetDlgItemInt(IDC_N),-1);
		m_DataRx.SetWindowText ( szData.c_str() );
	}	
}

void CSerialCommDlg::OnRead1() 
{
	if(m_bReady==FALSE)
		AfxMessageBox("串口没有正确设置");
	else
	{
		std::string szData;
		m_theCommPort.Read_N (szData,1,-1);
		m_DataRx.SetWindowText ( szData.c_str() );
	}
}

void CSerialCommDlg::OnReadUpto() 
{
	if(m_bReady==FALSE)
		AfxMessageBox("串口没有正确设置");
	else
	{
		std::string szData;
		long alCount;
		CString a;
		GetDlgItemText(IDC_CHAR,a);
		m_theCommPort.Read_Upto (szData,a[0],&alCount,-1);
		m_DataRx.SetWindowText ( szData.c_str() );
	}
}

void CSerialCommDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CSerialCommDlg::OnChangeChar() 
{
	CString a;
	static char oldchar=NULL;
	GetDlgItemText(IDC_CHAR,a);
	if(a.GetLength()==0)
		return;
	if(a.GetLength()==1)
	{
		oldchar=a[0];
		return;
	}
	if(oldchar!=a[1])
		oldchar=a[1];
	else
		oldchar=a[0];
	a=oldchar;
	SetDlgItemText(IDC_CHAR,a);
}
