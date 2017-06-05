// FTPSOCKET.h : main header file for the FTPSOCKET application
//

#if !defined(AFX_FTPSOCKET_H__AFA0C59F_78A5_4124_8E6C_666C73C4A547__INCLUDED_)
#define AFX_FTPSOCKET_H__AFA0C59F_78A5_4124_8E6C_666C73C4A547__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFTPSOCKETApp:
// See FTPSOCKET.cpp for the implementation of this class
//

class CFTPSOCKETApp : public CWinApp
{
public:
	CFTPSOCKETApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPSOCKETApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFTPSOCKETApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSOCKET_H__AFA0C59F_78A5_4124_8E6C_666C73C4A547__INCLUDED_)
