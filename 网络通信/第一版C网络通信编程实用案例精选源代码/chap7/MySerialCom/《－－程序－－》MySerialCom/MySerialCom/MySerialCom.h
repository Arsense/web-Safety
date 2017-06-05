// MySerialCom.h : main header file for the MYSERIALCOM application
//

#if !defined(AFX_MYSERIALCOM_H__C780CAD9_97F2_4835_9BD7_FE0360AB3D98__INCLUDED_)
#define AFX_MYSERIALCOM_H__C780CAD9_97F2_4835_9BD7_FE0360AB3D98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMySerialComApp:
// See MySerialCom.cpp for the implementation of this class
//

class CMySerialComApp : public CWinApp
{
public:
	CMySerialComApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySerialComApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMySerialComApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSERIALCOM_H__C780CAD9_97F2_4835_9BD7_FE0360AB3D98__INCLUDED_)
