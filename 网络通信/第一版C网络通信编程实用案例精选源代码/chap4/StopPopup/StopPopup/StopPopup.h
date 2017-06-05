// StopPopup.h : main header file for the STOPPOPUP application
//

#if !defined(AFX_STOPPOPUP_H__2784C6A3_6453_4101_A5A1_327EF6261244__INCLUDED_)
#define AFX_STOPPOPUP_H__2784C6A3_6453_4101_A5A1_327EF6261244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStopPopupApp:
// See StopPopup.cpp for the implementation of this class
//

class CStopPopupApp : public CWinApp
{
public:
	CStopPopupApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStopPopupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStopPopupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPPOPUP_H__2784C6A3_6453_4101_A5A1_327EF6261244__INCLUDED_)
