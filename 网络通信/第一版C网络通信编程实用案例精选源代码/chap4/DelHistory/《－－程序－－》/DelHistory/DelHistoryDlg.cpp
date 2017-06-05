// DelHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DelHistory.h"
#include "DelHistoryDlg.h"

#include <wininet.h>
#include <UrlHist.h>
#include "shlwapi.h"

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
// CDelHistoryDlg dialog

CDelHistoryDlg::CDelHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDelHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDelHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDelHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelHistoryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDelHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CDelHistoryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DEL_HISTORY, OnDelHistory)
	ON_BN_CLICKED(IDC_DEL_COOKIES, OnDelCookies)
	ON_BN_CLICKED(IDC_DEL_INTERNET_TEMP_FILE, OnDelInternetTempFile)
	ON_BN_CLICKED(IDC_DEL_HISTORY_ADDRESS, OnDelHistoryAddress)
	ON_BN_CLICKED(IDC_DEL_HISTORY_PASSWORD, OnDelHistoryPassword)
	ON_BN_CLICKED(IDC_DEL_FAVORITE, OnDelFavorite)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelHistoryDlg message handlers

BOOL CDelHistoryDlg::OnInitDialog()
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

void CDelHistoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDelHistoryDlg::OnPaint() 
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
HCURSOR CDelHistoryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDelHistoryDlg::OnDelHistory() 
{
	// TODO: Add your control notification handler code here
	TCHAR szPath[MAX_PATH];	
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_RECENT, FALSE))
	{
		EmptyDirectory(szPath);
	}
	SHDeleteKey(HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs"));

}
BOOL CDelHistoryDlg::EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, 
							 BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else
		{
			DeleteFile(sFullPath);
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);

	return TRUE;
}


BOOL CDelHistoryDlg::DeleteUrlCache(DEL_CACHE_TYPE type)
{
    BOOL bRet = FALSE;
	HANDLE hEntry;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
    DWORD dwEntrySize;

 	//delete the files
	dwEntrySize = 0;
    hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
    hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}

	do
    {
		if (type == File &&
			!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == Cookie &&
			(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}

		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete [] lpCacheEntry; 
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	}
	while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete [] lpCacheEntry; 
	}
    return bRet;
}
void CDelHistoryDlg::OnDelCookies() 
{
	// TODO: Add your control notification handler code here
	TCHAR szPath[MAX_PATH];	
	DeleteUrlCache(Cookie);
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{//得到目录，并清空
		EmptyDirectory(szPath);
	}
}

void CDelHistoryDlg::OnDelInternetTempFile() 
{
	// TODO: Add your control notification handler code here
    TCHAR szPath[MAX_PATH];
	DeleteUrlCache(File);
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
	{  //得到临时目录，并清空它.
		EmptyDirectory(szPath);
	}
}

void CDelHistoryDlg::OnDelHistoryAddress() 
{
	// TODO: Add your control notification handler code here
	//浏览器地址栏历史地址的清除
	SHDeleteKey(HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}

void CDelHistoryDlg::OnDelHistoryPassword() 
{
	// 清除自动密码历史记录
	SHDeleteKey(HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\IntelliForms"));

}

void CDelHistoryDlg::OnDelFavorite() 
{
	//清收藏夹中的内容
	TCHAR szPath[MAX_PATH];	
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_FAVORITES, FALSE))
	{	//得到目录，并清空 
		EmptyDirectory(szPath);
	}
}

void CDelHistoryDlg::OnButtonHelp() 
{
	// TODO: Add your control notification handler code here
	CDialog dlg(IDD_ABOUTBOX);
	dlg.DoModal();
}
