// myWinInet_1.h : main header file for the MYWININET_1 application
//

#if !defined(AFX_MYWININET_1_H__9F6D143A_75D0_4F1E_B771_D28C43FD1C50__INCLUDED_)
#define AFX_MYWININET_1_H__9F6D143A_75D0_4F1E_B771_D28C43FD1C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyWinInet_1App:
// See myWinInet_1.cpp for the implementation of this class
//

class CMyWinInet_1App : public CWinApp
{
public:
	CMyWinInet_1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWinInet_1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyWinInet_1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWININET_1_H__9F6D143A_75D0_4F1E_B771_D28C43FD1C50__INCLUDED_)
