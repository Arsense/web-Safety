// Mailer.h : main header file for the MAILER application
//

#if !defined(AFX_MAILER_H__74764E04_5206_11D6_9CE4_EE1F83ADA179__INCLUDED_)
#define AFX_MAILER_H__74764E04_5206_11D6_9CE4_EE1F83ADA179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMailerApp:
// See Mailer.cpp for the implementation of this class
//

class CMailerApp : public CWinApp
{
public:
	CMailerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMailerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILER_H__74764E04_5206_11D6_9CE4_EE1F83ADA179__INCLUDED_)
