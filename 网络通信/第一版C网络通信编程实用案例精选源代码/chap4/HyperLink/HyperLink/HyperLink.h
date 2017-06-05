// HyperLink.h : main header file for the HYPERLINK application
//

#if !defined(AFX_HYPERLINK_H__7B034FA4_F02F_49BC_8B68_75A9A4AFDCC9__INCLUDED_)
#define AFX_HYPERLINK_H__7B034FA4_F02F_49BC_8B68_75A9A4AFDCC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkApp:
// See HyperLink.cpp for the implementation of this class
//

class CHyperLinkApp : public CWinApp
{
public:
	CHyperLinkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLinkApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHyperLinkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINK_H__7B034FA4_F02F_49BC_8B68_75A9A4AFDCC9__INCLUDED_)
