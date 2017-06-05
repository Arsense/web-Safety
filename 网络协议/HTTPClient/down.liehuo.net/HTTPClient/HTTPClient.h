// HTTPClient.h : main header file for the HTTPCLIENT application
//

#if !defined(AFX_HTTPCLIENT_H__CB54C436_EF5C_48A5_875D_D61FA462AB58__INCLUDED_)
#define AFX_HTTPCLIENT_H__CB54C436_EF5C_48A5_875D_D61FA462AB58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientApp:
// See HTTPClient.cpp for the implementation of this class
//

class CHTTPClientApp : public CWinApp
{
public:
	CHTTPClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTTPClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHTTPClientApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPCLIENT_H__CB54C436_EF5C_48A5_875D_D61FA462AB58__INCLUDED_)
