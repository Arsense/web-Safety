// CSocket.h : main header file for the CSOCKET application
//

#if !defined(AFX_CSOCKET_H__4D03539E_AACA_46E3_9DAC_A05BCBF2C8A6__INCLUDED_)
#define AFX_CSOCKET_H__4D03539E_AACA_46E3_9DAC_A05BCBF2C8A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCSocketApp:
// See CSocket.cpp for the implementation of this class
//

class CCSocketApp : public CWinApp
{
public:
	CCSocketApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSocketApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCSocketApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSOCKET_H__4D03539E_AACA_46E3_9DAC_A05BCBF2C8A6__INCLUDED_)
