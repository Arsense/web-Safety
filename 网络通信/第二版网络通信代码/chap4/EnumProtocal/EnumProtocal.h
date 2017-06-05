// EnumProtocal.h : main header file for the ENUMPROTOCAL application
//

#if !defined(AFX_ENUMPROTOCAL_H__8A3845BA_1342_426B_BC6C_00D1636A3B58__INCLUDED_)
#define AFX_ENUMPROTOCAL_H__8A3845BA_1342_426B_BC6C_00D1636A3B58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEnumProtocalApp:
// See EnumProtocal.cpp for the implementation of this class
//

class CEnumProtocalApp : public CWinApp
{
public:
	CEnumProtocalApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumProtocalApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEnumProtocalApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMPROTOCAL_H__8A3845BA_1342_426B_BC6C_00D1636A3B58__INCLUDED_)
