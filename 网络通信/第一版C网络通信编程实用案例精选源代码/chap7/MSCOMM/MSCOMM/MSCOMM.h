// MSCOMM.h : main header file for the MSCOMM application
//

#if !defined(AFX_MSCOMM_H__B19A5F1E_BD1C_4BCB_9E2B_407509362500__INCLUDED_)
#define AFX_MSCOMM_H__B19A5F1E_BD1C_4BCB_9E2B_407509362500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMSCOMMApp:
// See MSCOMM.cpp for the implementation of this class
//

class CMSCOMMApp : public CWinApp
{
public:
	CMSCOMMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSCOMMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMSCOMMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSCOMM_H__B19A5F1E_BD1C_4BCB_9E2B_407509362500__INCLUDED_)
