// DelHistory.h : main header file for the DELHISTORY application
//

#if !defined(AFX_DELHISTORY_H__B5B56192_B2CF_450E_965E_40C2D6F44FDC__INCLUDED_)
#define AFX_DELHISTORY_H__B5B56192_B2CF_450E_965E_40C2D6F44FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDelHistoryApp:
// See DelHistory.cpp for the implementation of this class
//

class CDelHistoryApp : public CWinApp
{
public:
	CDelHistoryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelHistoryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDelHistoryApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELHISTORY_H__B5B56192_B2CF_450E_965E_40C2D6F44FDC__INCLUDED_)
