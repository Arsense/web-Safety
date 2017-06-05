// NeighborInfo.h : main header file for the NEIGHBORINFO application
//

#if !defined(AFX_NEIGHBORINFO_H__49BEBB2A_6788_428D_8D25_D611BCB47BFA__INCLUDED_)
#define AFX_NEIGHBORINFO_H__49BEBB2A_6788_428D_8D25_D611BCB47BFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNeighborInfoApp:
// See NeighborInfo.cpp for the implementation of this class
//

class CNeighborInfoApp : public CWinApp
{
public:
	CNeighborInfoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeighborInfoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNeighborInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEIGHBORINFO_H__49BEBB2A_6788_428D_8D25_D611BCB47BFA__INCLUDED_)
