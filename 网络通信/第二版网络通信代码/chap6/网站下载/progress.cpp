/*
	Progress.cpp : implementation of the CProgress class

	Implements the statistics (progress) window that SiteSnagger displays
	at the bottom of its main window.

	Author: Steven E. Sipe
*/

#include "stdafx.h"
#include "sitesnag.h"
#include "progress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgress dialog


// Constructor
CProgress::CProgress(CWnd* pParent /*=NULL*/)
			: CDialog()
{
	//{{AFX_DATA_INIT(CProgress)
	//}}AFX_DATA_INIT

	// Create the window
	Create(CProgress::IDD, pParent);
	m_bAborted = FALSE;
}

// Handles field binding for MFC
void CProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress)
	DDX_Control(pDX, IDC_LEVEL, m_Level);
	DDX_Control(pDX, IDC_ACTION, m_Action);
	DDX_Control(pDX, IDC_QUEUED_FILES, m_QueuedFiles);
	DDX_Control(pDX, IDC_FILES_DOWNLOADED, m_FilesDownloaded);
	DDX_Control(pDX, IDC_PAGES_DOWNLOADED, m_PagesDownloaded);
	DDX_Control(pDX, IDC_KB_DOWNLOADED, m_KBDownloaded);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProgress, CDialog)
	//{{AFX_MSG_MAP(CProgress)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgress message handlers


// Handles initialization of the statistics window (WM_INITDIALOG)
BOOL CProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Sets the action field (i.e. Downloading xyz)
void CProgress::SetActionTitle(LPCTSTR lpszText)
{
	m_Action.SetWindowText(lpszText);
}

// Sets the queued page count
void CProgress::SetQueuedFiles(int nNumber)
{
	CString strText;
	strText.Format("%d",nNumber);

	m_QueuedFiles.SetWindowText(strText);
}

// Sets the downloaded file count
void CProgress::SetDownloadedFiles(int nNumber)
{
	CString strText;
	strText.Format("%d",nNumber);

	m_FilesDownloaded.SetWindowText(strText);
}

// Sets the downloaded HTML pages count
void CProgress::SetDownloadedPages(int nNumber)
{
	CString strText;
	strText.Format("%d",nNumber);

	m_PagesDownloaded.SetWindowText(strText);
}

// Sets the KB downloaded count
void CProgress::SetKBDownloaded(int nNumber)
{
	CString strText;
	strText.Format("%dK",nNumber/1024);

	m_KBDownloaded.SetWindowText(strText);
}

// Sets the current level
void CProgress::SetLevel(int nLevel)
{
	CString strText;
	strText.Format("%d",nLevel);

	m_Level.SetWindowText(strText);
}

// Stretches the IDC_ACTION field so that it takes up the entire width of
// the progress window
void CProgress::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(::IsWindow(m_Action.GetSafeHwnd()))
	{
		RECT rc;
		GetWindowRect(&rc);
		m_Action.SetWindowPos(NULL,-1,-1,cx-5,rc.bottom-rc.top,SWP_NOMOVE);
	}
}
