// ServerSocket.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerSocket.h"
#include "ServerSocketDlg.h"
#include "ClientSocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WSA_VERSION  MAKEWORD(2,0)
#define MAX_HOSTNAME 256
#define MAX_HOSTADDR 40

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp

BEGIN_MESSAGE_MAP(CServerSocketApp, CWinApp)
	//{{AFX_MSG_MAP(CServerSocketApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp construction

CServerSocketApp::CServerSocketApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nLinkMode = 0; // server
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerSocketApp object

CServerSocketApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp initialization

BOOL CServerSocketApp::InitInstance()
{
	WSADATA		WSAData = { 0 };
	//启动winsock
	if ( 0 != WSAStartup( WSA_VERSION, &WSAData ) )
	{
		// 如果有错，则提示
		// WinSock DLL.
		if ( LOBYTE( WSAData.wVersion ) != LOBYTE(WSA_VERSION) ||
			 HIBYTE( WSAData.wVersion ) != HIBYTE(WSA_VERSION) )
			 ::MessageBox(NULL, _T("Incorrect version of Winsock.dll found"), _T("Error"), MB_OK);
		//关闭清除
		WSACleanup( );
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

	//分析命令行参数，如果为/s则表示服务器程序
	//如果为/c，则表示客户端程序
	ParseCommandLineArgs();

	//定义两个对话框变量
	CServerSocketDlg dlg1;
	CClientSocketDlg dlg2;
	switch( m_nLinkMode )
	{
		default:
		case 0:
		m_pMainWnd = &dlg1; // 服务器
			break;
		case 1:
		m_pMainWnd = &dlg2; // 客户端
			break;
	}


	//弹出对话框
	int nResponse = ((CDialog*)m_pMainWnd)->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		// dismissed with OK
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

int CServerSocketApp::ExitInstance() 
{
	// Terminate use of the WS2_32.DLL
	//卸在winsock库
	WSACleanup();
	
	return CWinApp::ExitInstance();
}

void CServerSocketApp::ParseCommandLineArgs()
{
	//获得命令行
	CString strCmdLine = (LPCTSTR) GetCommandLine();

	if (!strCmdLine.IsEmpty())
	{
		//全部转化成大写
		strCmdLine.MakeUpper();
		int nPos = 0;
		do {
			//查找空格
			nPos = strCmdLine.Find(TCHAR(' '));
			if (nPos>0)
			{
				//删除第一个空格前的字符
				strCmdLine.Delete( 0, nPos+1);
				CString strCurrent = strCmdLine;
				//查找第二个空格
				int nNextPos = strCmdLine.Find(TCHAR(' '));
				if (nNextPos > 0)
					strCurrent = strCmdLine.Left( nNextPos );
				//如果命令行参数是/SERVER或者是/S，则设定模式是0
				if (strCurrent == _T("/SERVER") || strCurrent == _T("/S"))
					m_nLinkMode = 0;
				//否则设定为1
				else if (strCurrent == _T("/CLIENT") || strCurrent == _T("/C"))
					m_nLinkMode = 1;
			}
		} while( nPos != -1);
	}
}

