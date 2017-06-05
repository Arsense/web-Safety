// MyTraceRoute.h : main header file for the MYTRACEROUTE application
//

#if !defined(AFX_MYTRACEROUTE_H__22C0FF2C_9193_4378_B985_047C18EFA296__INCLUDED_)
#define AFX_MYTRACEROUTE_H__22C0FF2C_9193_4378_B985_047C18EFA296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyTraceRouteApp:
// See MyTraceRoute.cpp for the implementation of this class
//

class CMyTraceRouteApp : public CWinApp
{
public:
	CMyTraceRouteApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTraceRouteApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyTraceRouteApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTRACEROUTE_H__22C0FF2C_9193_4378_B985_047C18EFA296__INCLUDED_)
