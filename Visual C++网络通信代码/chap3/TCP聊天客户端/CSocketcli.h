// CSocketcli.h : main header file for the CSOCKETCLI application
//

#if !defined(AFX_CSOCKETCLI_H__69E66DAA_8373_4750_8B03_E76D014B461E__INCLUDED_)
#define AFX_CSOCKETCLI_H__69E66DAA_8373_4750_8B03_E76D014B461E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCSocketcliApp:
// See CSocketcli.cpp for the implementation of this class
//

class CCSocketcliApp : public CWinApp
{
public:
	CCSocketcliApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSocketcliApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCSocketcliApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSOCKETCLI_H__69E66DAA_8373_4750_8B03_E76D014B461E__INCLUDED_)
