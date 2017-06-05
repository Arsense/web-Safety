/*
	SiteSnag.cpp : implementation of the CSnaggerApp class

	Implements the main application class, derived from CWinApp.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include <direct.h>
#include <io.h>

#include "SiteSnag.h"

#include "Frame.h"
#include "Document.h"
#include "View.h"
#include "inet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSnaggerApp

BEGIN_MESSAGE_MAP(CSnaggerApp, CWinApp)
	//{{AFX_MSG_MAP(CSnaggerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

// Determine if we are running the new shell (i.e. Windows 95 or Windows NT 4.0
// or later)
BOOL CSnaggerApp::HasNewShell()
{
	// Get the Windows Version
	DWORD dwVersion = GetVersion();

	// Windows NT?
	if(dwVersion < 0x80000000) 
	{
		// Yes, Windows NT 4.0 or greater?
		if(LOBYTE(LOWORD(dwVersion)) >= 4) 
			return TRUE; 
	} // Windows 95?
	else if(LOBYTE(LOWORD(dwVersion)) >= 4) 
			return TRUE;

   return FALSE;  // Win16, 32s or NT3.x.
}

// Determine if SiteSnager is already running by using a Mutex object -- returns
// TRUE if already running, FALSE otherwise.  Note that a mutex is created
// with the name of the application
BOOL CSnaggerApp::AlreadyRunning()
{
	BOOL bFound = FALSE;

	// Try to create a mutex with the app's name
	HANDLE hMutexOneInstance = ::CreateMutex(NULL,TRUE,_T(AfxGetAppName()));

	// Already there...means that we are already running an instance
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
		bFound = TRUE;

	// Release the mutex
	if(hMutexOneInstance)
		::ReleaseMutex(hMutexOneInstance);

	return(bFound);
}

/////////////////////////////////////////////////////////////////////////////
// CSnaggerApp construction

CSnaggerApp::CSnaggerApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSnaggerApp object

CSnaggerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSnaggerApp initialization

// Initialize the app -- check to see if is already running, make sure we're
// running the right OS
BOOL CSnaggerApp::InitInstance()
{
	// Only allow one instance of our application to execute
	if(AlreadyRunning())
	{
		AfxMessageBox(IDS_ALREADY_RUNNING,MB_OK|MB_ICONWARNING);
		return(FALSE);
	}

	// Only run with the new shell --- we need a system tray
	if(!HasNewShell())
	{
		AfxMessageBox(IDS_WRONG_SHELL,MB_OK|MB_ICONSTOP);
		return(FALSE);
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	int nSaveCmdShow = m_nCmdShow;
	m_nCmdShow &= ~SW_SHOW;

	// Setup the registry key, we only use this to store the MRU list
	SetRegistryKey(_T("PC Magazine\\SiteSnagger"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSnaggerDoc),
		RUNTIME_CLASS(CSnaggerFrame),       // main SDI frame window
		RUNTIME_CLASS(CSnaggerView));
	AddDocTemplate(pDocTemplate);

	// Parse the command line 
	if(!ProcessCommandLine())
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->SetWindowPos(NULL,-1,-1,620,360,SWP_NOMOVE);
	m_pMainWnd->CenterWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// Allows access to the applications Most Recently Used (MRU) file list
void CSnaggerApp::UpdateMRU(CCmdUI *pCmdUI)
{
	CWinApp::OnUpdateRecentFileMenu(pCmdUI);
}

// Process the command line arguments passed in
//
// √¸¡Ó––¥¶¿Ì
//
BOOL CSnaggerApp::ProcessCommandLine()
{
	CString strURL;
	CSnagOptions Options;

	// Create the projects directory (if not in command line mode)
	if(__argc < 2)
		mkdir("Projects");

	// Create an empty document -- this ensures that the tree control will
	// always exist
	OnFileNew();

	// Was a document name specified?
	if(__argc > 1)
	{
		// Make sure that the Projects subdirectory exists, if not, then
		// we're running from the wrong directory or there is no project
		if(access("Projects",0) != 0)
		{
			AfxMessageBox("The working directory must be the same as the SiteSnagger program directory.",
										MB_ICONERROR|MB_OK);
			return(FALSE);
		}

		// Add the "Projects" subdirectory if the user didn't specify a 
		// subdirectory location
		CString strFileName;
		CString strPath = CInet::SplitFileName(__argv[1],CInet::PATH);
		if(strPath.IsEmpty())
			strFileName = CString("Projects\\");
		strFileName += __argv[1];

		// Try to open the document that the user specified on the command line
		CSnaggerDoc *pDoc = (CSnaggerDoc *) m_pDocManager->OpenDocumentFile(strFileName);

		// Didn't work, get out
		if(pDoc == NULL)
			return(FALSE);

		// Do we have a URL???
		if(__argc > 2)
		{
			// Yes, this means that it it automatic mode
			strURL = __argv[2];

			// Set some default options (these will override the current 
			// project options)
			Options.nMaxDepth = 2;
			Options.nMaxPages = 0;
			Options.bOffsiteLinks = FALSE;
			Options.bMultimedia = TRUE;
			Options.bFixupLinks = TRUE;
			Options.bContents = TRUE;

			for(int i = 3; i < __argc; i++)
			{
				LPCTSTR lpszParam = __targv[i];
				if(*lpszParam == '-' || *lpszParam == '/')
				{
					// Skip the flag
					++lpszParam;

					switch(tolower(*lpszParam))
					{
						// Levels
						case 'l':
							Options.nMaxDepth = atoi(lpszParam+1)%(MAX_LEVELS+1);
							if(Options.nMaxDepth <= 0)
								Options.nMaxDepth = 0;
							break;

						// Pages
						case 'p':
							Options.nMaxPages = atoi(lpszParam+1);
							if(Options.nMaxPages < 0)
								Options.nMaxPages = 0;
							break;

						// Offsite links						
						case 'o':
							Options.bOffsiteLinks = *(lpszParam+1) == '+';
							break;

						// Multimedia 
						case 'm':
							Options.bMultimedia = *(lpszParam+1) == '+';
							break;

						// Fixup for browsing
						case 'b':
							Options.bFixupLinks = *(lpszParam+1) == '+';
							break;

						// Generate table of contents
						case 'c':
							Options.bContents = *(lpszParam+1) == '+';
							break;
					}
				}
			}

			// Setup the document and view for snagging
			POSITION pos = pDoc->GetFirstViewPosition();
			CSnaggerView* pView = (CSnaggerView *) pDoc->GetNextView(pos);

			pDoc->SetAutoMode(TRUE);
			pDoc->SetOptions(Options);
			CString strPath = CInet::SplitFileName(pDoc->GetPathName(),
						CInet::DRIVE|CInet::PATH|CInet::FNAME)+"\\";
			pView->ClearProject(strPath);
			pView->InitTree(strURL);
			pView->SetSnagging(TRUE);

			// Start the snagging operation
			pDoc->RecursiveDownload(strURL);
			pDoc->SetModifiedFlag(TRUE);
		}
	}

	return(TRUE);
}

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
		// No message handlers
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

// App command to run the dialog
void CSnaggerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSnaggerApp commands

void CSnaggerApp::OnHelpContents() 
{
	WinHelp(0,HELP_FINDER);
}

