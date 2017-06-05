// MyPortScanner.h : main header file for the MYPORTSCANNER application
//

#if !defined(AFX_MYPORTSCANNER_H__485620D0_B9E2_4BC2_8522_0C9DFC4F3886__INCLUDED_)
#define AFX_MYPORTSCANNER_H__485620D0_B9E2_4BC2_8522_0C9DFC4F3886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyPortScannerApp:
// See MyPortScanner.cpp for the implementation of this class
//

class CMyPortScannerApp : public CWinApp
{
public:
	CMyPortScannerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPortScannerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyPortScannerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPORTSCANNER_H__485620D0_B9E2_4BC2_8522_0C9DFC4F3886__INCLUDED_)
