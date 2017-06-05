// MyFinger.h : main header file for the MYFINGER application
//

#if !defined(AFX_MYFINGER_H__D9417B48_6181_4947_90E7_B4A13D85B97B__INCLUDED_)
#define AFX_MYFINGER_H__D9417B48_6181_4947_90E7_B4A13D85B97B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyFingerApp:
// See MyFinger.cpp for the implementation of this class
//

class CMyFingerApp : public CWinApp
{
public:
	CMyFingerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFingerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyFingerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFINGER_H__D9417B48_6181_4947_90E7_B4A13D85B97B__INCLUDED_)
