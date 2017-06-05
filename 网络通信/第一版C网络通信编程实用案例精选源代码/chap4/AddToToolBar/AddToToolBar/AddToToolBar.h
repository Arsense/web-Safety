// AddToToolBar.h : main header file for the ADDTOTOOLBAR application
//

#if !defined(AFX_ADDTOTOOLBAR_H__52B4D337_7712_4124_B196_67F86DEF78FF__INCLUDED_)
#define AFX_ADDTOTOOLBAR_H__52B4D337_7712_4124_B196_67F86DEF78FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAddToToolBarApp:
// See AddToToolBar.cpp for the implementation of this class
//

class CAddToToolBarApp : public CWinApp
{
public:
	CAddToToolBarApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddToToolBarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAddToToolBarApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDTOTOOLBAR_H__52B4D337_7712_4124_B196_67F86DEF78FF__INCLUDED_)
