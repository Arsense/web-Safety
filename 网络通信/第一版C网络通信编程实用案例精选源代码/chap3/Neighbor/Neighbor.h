// Neighbor.h : main header file for the NEIGHBOR application
//

#if !defined(AFX_NEIGHBOR_H__04C5EDBB_7AF2_410B_B290_83963A2958BD__INCLUDED_)
#define AFX_NEIGHBOR_H__04C5EDBB_7AF2_410B_B290_83963A2958BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNeighborApp:
// See Neighbor.cpp for the implementation of this class
//

class CNeighborApp : public CWinApp
{
public:
	CNeighborApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeighborApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNeighborApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEIGHBOR_H__04C5EDBB_7AF2_410B_B290_83963A2958BD__INCLUDED_)
