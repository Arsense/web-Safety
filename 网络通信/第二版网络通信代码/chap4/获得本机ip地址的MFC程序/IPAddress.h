// IPAddress.h : main header file for the IPADDRESS application
//

#if !defined(AFX_IPADDRESS_H__859FA759_1659_11D2_9153_0060080C48E1__INCLUDED_)
#define AFX_IPADDRESS_H__859FA759_1659_11D2_9153_0060080C48E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIPAddressApp:
// See IPAddress.cpp for the implementation of this class
//

class CIPAddressApp : public CWinApp
{
public:
	CIPAddressApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPAddressApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIPAddressApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPADDRESS_H__859FA759_1659_11D2_9153_0060080C48E1__INCLUDED_)
