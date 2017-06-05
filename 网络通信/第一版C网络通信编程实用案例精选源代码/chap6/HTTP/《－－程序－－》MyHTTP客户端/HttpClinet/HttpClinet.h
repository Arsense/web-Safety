// HttpClinet.h : main header file for the HTTPCLINET application
//

#if !defined(AFX_HTTPCLINET_H__B87FB1CA_B310_4FF3_9E0B_ACFD6A388167__INCLUDED_)
#define AFX_HTTPCLINET_H__B87FB1CA_B310_4FF3_9E0B_ACFD6A388167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHttpClinetApp:
// See HttpClinet.cpp for the implementation of this class
//

class CHttpClinetApp : public CWinApp
{
public:
	CHttpClinetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpClinetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHttpClinetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPCLINET_H__B87FB1CA_B310_4FF3_9E0B_ACFD6A388167__INCLUDED_)
