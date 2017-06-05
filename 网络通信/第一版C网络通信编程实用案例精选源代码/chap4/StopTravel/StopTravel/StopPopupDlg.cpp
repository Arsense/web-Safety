// StopPopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StopPopup.h"
#include "StopPopupDlg.h"
#include "Shlobj.h"
#include "fstream.h"
#include <afxtempl.h>

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
// CStopPopupDlg dialog

CStopPopupDlg::CStopPopupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStopPopupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStopPopupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStopPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStopPopupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStopPopupDlg, CDialog)
	//{{AFX_MSG_MAP(CStopPopupDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_BN_CLICKED(IDC_DEL_SITE, OnDelSite)
	ON_BN_CLICKED(IDC_ADD_SITE, OnAddSite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStopPopupDlg message handlers

BOOL CStopPopupDlg::OnInitDialog()
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
	CListBox * listbox=(CListBox *)GetDlgItem(IDC_SITE_LIST);
	ifstream in("SiteList.txt",ios::in);
	if(in.fail())
	{
		in.close();
		ofstream out("SiteList.txt",ios::out|ios::trunc);
		if(out.fail())
		{
			out.close();
			AfxMessageBox("sorry, cannot create file sitelist.txt!");
			return FALSE;
		}
		out.close();
	}
	else
	{
		char site[1024];
		site[0]='\0';
		in>>site;
		while(TRUE)
		{
			if(strlen(site)==0)
			{
				break;
			}
			listbox->AddString(site);
			site[0]='\0';
			in>>site;
		}
		in.close();
	}	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStopPopupDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStopPopupDlg::OnPaint() 
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
HCURSOR CStopPopupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStopPopupDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
    ZeroMemory( &pi, sizeof(pi) );

	ShellExecute(NULL,NULL,"shell.bat",NULL,NULL,SW_HIDE);

	CString strCommand="regsvr32 c:\\winnt\\iehelper.dll";
	BeginWaitCursor();
    // Start the child process. 
	if( !CreateProcess( NULL, // No module name (use command line). 
        (LPSTR)LPCSTR(strCommand), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
    {
        AfxMessageBox( "创建进程失败!" );
    }
	
    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );
	EndWaitCursor();
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

void CStopPopupDlg::OnGo() 
{
	// TODO: Add your control notification handler code here
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
    ZeroMemory( &pi, sizeof(pi) );

	ShellExecute(NULL,NULL,"shell.bat",NULL,NULL,SW_HIDE);
	CString strCommand="regsvr32 /u c:\\winnt\\iehelper.dll";

	BeginWaitCursor();
    // Start the child process. 
	if( !CreateProcess( NULL, // No module name (use command line). 
        (LPSTR)LPCSTR(strCommand), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
    {
        AfxMessageBox( "创建进程失败!" );
    }
	
    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );
	EndWaitCursor();
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

void CStopPopupDlg::OnDelSite() 
{
	// TODO: Add your control notification handler code here
	CListBox * listbox=(CListBox *)GetDlgItem(IDC_SITE_LIST);
	int i;
	CString temp;
	i=listbox->GetCurSel();
	listbox->DeleteString(i);
	ofstream out("SiteList.txt",ios::out);
	if(out.fail())
		return;

	int nCount=listbox->GetCount();
	for(i=0;i<nCount;i++)
	{
		listbox->GetText(i,temp);
		out<<temp<<endl;
	}
	out.close();
}

void CStopPopupDlg::OnAddSite() 
{
	// TODO: Add your control notification handler code here
	CListBox * listbox=(CListBox *)GetDlgItem(IDC_SITE_LIST);
	char site[1024];
	GetDlgItemText(IDC_SITE_EDIT,site,1024);
	listbox->AddString(site);
	ofstream out("SiteList.txt",ios::out|ios::app);
	if(out.fail())
		return;
	out.seekp(0,ios::end);
	out<<site<<endl;
	out.close();
}
