// MyFtpServer.h : main header file for the MYFTPSERVER application
//

#if !defined(AFX_MYFTPSERVER_H__FA1874A6_32D6_483A_9EAB_BDFCA7C52522__INCLUDED_)
#define AFX_MYFTPSERVER_H__FA1874A6_32D6_483A_9EAB_BDFCA7C52522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyFtpServerApp:
// See MyFtpServer.cpp for the implementation of this class
//

class CMyFtpServerApp : public CWinApp
{
public:
	CMyFtpServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFtpServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyFtpServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFTPSERVER_H__FA1874A6_32D6_483A_9EAB_BDFCA7C52522__INCLUDED_)
