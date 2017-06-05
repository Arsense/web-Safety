// FTPServer.h : main header file for the FTPSERVER application
//

#if !defined(AFX_FTPSERVER_H__3F4F6387_5F3E_41E5_BB28_8894A19E6CE4__INCLUDED_)
#define AFX_FTPSERVER_H__3F4F6387_5F3E_41E5_BB28_8894A19E6CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "LogFile.h"

/////////////////////////////////////////////////////////////////////////////
// CFTPServerApp:
// See FTPServer.cpp for the implementation of this class
//

class CFTPServerApp : public CWinApp
{
public:
	CFTPServerApp();
	CLogFile m_LogFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFTPServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSERVER_H__3F4F6387_5F3E_41E5_BB28_8894A19E6CE4__INCLUDED_)
