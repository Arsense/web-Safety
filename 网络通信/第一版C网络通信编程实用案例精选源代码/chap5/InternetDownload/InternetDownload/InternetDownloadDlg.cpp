// InternetDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InternetDownload.h"
#include "InternetDownloadDlg.h"
#include "Wininet.h"
#include <mmsystem.h>

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
// CInternetDownloadDlg dialog

CInternetDownloadDlg::CInternetDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInternetDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInternetDownloadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInternetDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInternetDownloadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInternetDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CInternetDownloadDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetDownloadDlg message handlers

BOOL CInternetDownloadDlg::OnInitDialog()
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

void CInternetDownloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CInternetDownloadDlg::OnPaint() 
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
HCURSOR CInternetDownloadDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInternetDownloadDlg::OnDownload() 
{
	// TODO: Add your control notification handler code here
	CString url,filename;
	GetDlgItemText(IDC_FILE_URL,url);
	GetDlgItemText(IDC_LOCAL_DIRECTORY,filename);
	if(url.IsEmpty())
	{
		AfxMessageBox("请输入Internet文件的URL!");
		return;
	}
	if(filename.IsEmpty())
	{
		AfxMessageBox("请输入需要保存的文件路径!");
		return;
	}
	if(InternetGetFile(url,filename)==0)
	{
		AfxMessageBox("下载成功!");
	}
	else
	{
		AfxMessageBox("下载失败!");
	}
}


int CInternetDownloadDlg::InternetGetFile (CString szUrl,CString szFileName)
{
	DWORD dwFlags;
	InternetGetConnectedState(&dwFlags, 0);
	CHAR strAgent[64];
	sprintf(strAgent, "Agent%ld", timeGetTime());
	HINTERNET hOpen;
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hOpen)
	{
		AfxMessageBox("Internet连接错误!");
		return -1;
	}

	DWORD dwSize;
	CHAR   szHead[] = "Accept: */*\r\n\r\n";
	VOID* szTemp[16384];
	HINTERNET  hConnect;
	CFile file;

	if ( !(hConnect = InternetOpenUrlA ( hOpen, szUrl, szHead,
		 lstrlenA (szHead), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
	{
	   AfxMessageBox("不能打开该URL!");
	   return -1;
	}

	if  (file.Open(szFileName,CFile::modeWrite|CFile::modeCreate)==FALSE )
	{
	   AfxMessageBox("不能打开本地的文件!");
	  return -1;
	}

	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;

    if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
                  (LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}

	DWORD start;
	DWORD end;
	DWORD time;
	CString tempstring;
	time = 10;
	start = timeGetTime();
	do
	{
		if (!InternetReadFile (hConnect, szTemp, 16384,  &dwSize))
		{
			AfxMessageBox("读文件出错!");
			file.Close();
			return -1;
		}
		if (dwSize==0)
			break;
		else
			file.Write(szTemp,dwSize);
		dwBytes+=dwSize;
		if(dwByteToRead)
		{
			tempstring.Format("%d%%",(dwBytes*100)/dwByteToRead);
			SetDlgItemText(IDC_PERCENT_TEXT,tempstring);
		}
		FLOAT fSpeed = 0;
		fSpeed = (float)dwBytes;
		fSpeed /= ((float)time)/1000.0f;
		fSpeed /= 1024.0f;
		tempstring.Format("%dKB/s",fSpeed);
		SetDlgItemText(IDC_SPEED_TEXT,tempstring);
		end = timeGetTime();
		time = end - start;
		if(time == 0)
			time = 10;
	}while (TRUE);

	file.Close();
	InternetCloseHandle(hOpen);

	return 0;
}
