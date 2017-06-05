// SMTPwithAttach.h : main header file for the SMTPWITHATTACH application
//

#if !defined(AFX_SMTPWITHATTACH_H__00BF0684_64ED_4056_BB99_E1E81BDF7EEC__INCLUDED_)
#define AFX_SMTPWITHATTACH_H__00BF0684_64ED_4056_BB99_E1E81BDF7EEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSMTPwithAttachApp:
// See SMTPwithAttach.cpp for the implementation of this class
//

class CSMTPwithAttachApp : public CWinApp
{
public:
	CSMTPwithAttachApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTPwithAttachApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSMTPwithAttachApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPWITHATTACH_H__00BF0684_64ED_4056_BB99_E1E81BDF7EEC__INCLUDED_)
