// SendEMail.h : main header file for the SENDEMAIL application
//

#if !defined(AFX_SENDEMAIL_H__2D61066E_947A_4F1F_AB00_0A4BA8681A31__INCLUDED_)
#define AFX_SENDEMAIL_H__2D61066E_947A_4F1F_AB00_0A4BA8681A31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSendEMailApp:
// See SendEMail.cpp for the implementation of this class
//

class CSendEMailApp : public CWinApp
{
public:
	CSendEMailApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendEMailApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSendEMailApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDEMAIL_H__2D61066E_947A_4F1F_AB00_0A4BA8681A31__INCLUDED_)
