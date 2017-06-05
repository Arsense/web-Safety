// Example1.h : main header file for the EXAMPLE1 application
//

#if !defined(AFX_EXAMPLE1_H__8A8A7F79_B0BF_45E0_81A2_E4D1912DD583__INCLUDED_)
#define AFX_EXAMPLE1_H__8A8A7F79_B0BF_45E0_81A2_E4D1912DD583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExample1App:
// See Example1.cpp for the implementation of this class
//

class CExample1App : public CWinApp
{
public:
	CExample1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExample1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExample1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLE1_H__8A8A7F79_B0BF_45E0_81A2_E4D1912DD583__INCLUDED_)
