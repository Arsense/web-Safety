// MailPost.h : main header file for the MAILPOST application
//

#if !defined(AFX_MAILPOST_H__DE4FB127_C73E_4823_98B2_BB1234D51B40__INCLUDED_)
#define AFX_MAILPOST_H__DE4FB127_C73E_4823_98B2_BB1234D51B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMailPostApp:
// See MailPost.cpp for the implementation of this class
//

class CMailPostApp : public CWinApp
{
public:
	CMailPostApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailPostApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMailPostApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILPOST_H__DE4FB127_C73E_4823_98B2_BB1234D51B40__INCLUDED_)
