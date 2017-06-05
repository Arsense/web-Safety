// InternetDownload.h : main header file for the INTERNETDOWNLOAD application
//

#if !defined(AFX_INTERNETDOWNLOAD_H__23204DE9_C056_424B_B0CF_2F851E7E49E7__INCLUDED_)
#define AFX_INTERNETDOWNLOAD_H__23204DE9_C056_424B_B0CF_2F851E7E49E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInternetDownloadApp:
// See InternetDownload.cpp for the implementation of this class
//

class CInternetDownloadApp : public CWinApp
{
public:
	CInternetDownloadApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInternetDownloadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInternetDownloadApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNETDOWNLOAD_H__23204DE9_C056_424B_B0CF_2F851E7E49E7__INCLUDED_)
