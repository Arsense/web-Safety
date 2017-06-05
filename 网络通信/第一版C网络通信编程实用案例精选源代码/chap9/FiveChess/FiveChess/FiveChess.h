// FiveChess.h : main header file for the FIVECHESS application
//

#if !defined(AFX_FIVECHESS_H__B1749FCD_A7A7_4ECA_97CA_B9FC5A07CE2F__INCLUDED_)
#define AFX_FIVECHESS_H__B1749FCD_A7A7_4ECA_97CA_B9FC5A07CE2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFiveChessApp:
// See FiveChess.cpp for the implementation of this class
//

class CFiveChessApp : public CWinApp
{
public:
	CFiveChessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiveChessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFiveChessApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIVECHESS_H__B1749FCD_A7A7_4ECA_97CA_B9FC5A07CE2F__INCLUDED_)
