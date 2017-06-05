// FTPServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FTPServerApp.h"
#include "ApplicationDlg.h"
#include "FTPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFTPServerApp

BEGIN_MESSAGE_MAP(CFTPServerApp, CWinApp)
	//{{AFX_MSG_MAP(CFTPServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPServerApp construction

CFTPServerApp::CFTPServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFTPServerApp object

CFTPServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFTPServerApp initialization

CFTPServer theServer;

BOOL CFTPServerApp::InitInstance()
{
	SetRegistryKey(_T("Bai FTP Server"));

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
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

	//int nLogLevel = GetProfileInt("Settings", "LogLevel" , 1);
	int nLogLevel = 3;
	if (nLogLevel)
	{
		// create configuration filename
		CString strFileName;

		GetAppDir(strFileName);
		strFileName += "ftptrace.txt";

		m_LogFile.SetLogLevel(nLogLevel);

		// open log file
		m_LogFile.Open((LPCTSTR)strFileName);
	}

	CApplicationDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
