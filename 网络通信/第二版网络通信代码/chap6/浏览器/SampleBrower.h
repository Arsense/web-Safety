// SampleBrower.h : main header file for the SAMPLEBROWER application
//

#if !defined(AFX_SAMPLEBROWER_H__675A13D8_6C76_42E5_906D_B322C7BC0341__INCLUDED_)
#define AFX_SAMPLEBROWER_H__675A13D8_6C76_42E5_906D_B322C7BC0341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerApp:
// See SampleBrower.cpp for the implementation of this class
//

class CSampleBrowerApp : public CWinApp
{
public:
	CSampleBrowerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleBrowerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSampleBrowerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEBROWER_H__675A13D8_6C76_42E5_906D_B322C7BC0341__INCLUDED_)
