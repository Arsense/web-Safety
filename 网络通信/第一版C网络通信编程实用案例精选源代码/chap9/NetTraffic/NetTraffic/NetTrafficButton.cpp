
#include "stdafx.h"
#include "NetTrafficButton.h"
#include "NetTrafficButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetTrafficButtonApp

BEGIN_MESSAGE_MAP(CNetTrafficButtonApp, CWinApp)
	//{{AFX_MSG_MAP(CNetTrafficButtonApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetTrafficButtonApp Construction

CNetTrafficButtonApp::CNetTrafficButtonApp()
{
}


CNetTrafficButtonApp theApp;


BOOL CNetTrafficButtonApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif

	CNetTrafficButtonDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}

	return FALSE;
}
