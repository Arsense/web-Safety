// SerialComm.h : main header file for the SERIALCOMM application
//

#if !defined(AFX_SERIALCOMM_H__D545EFFE_9542_48F2_BCA5_97222F84F953__INCLUDED_)
#define AFX_SERIALCOMM_H__D545EFFE_9542_48F2_BCA5_97222F84F953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSerialCommApp:
// See SerialComm.cpp for the implementation of this class
//

class CSerialCommApp : public CWinApp
{
public:
	CSerialCommApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialCommApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialCommApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMM_H__D545EFFE_9542_48F2_BCA5_97222F84F953__INCLUDED_)
