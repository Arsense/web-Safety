///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////


// Afd.h : main header file for the AFD application
//

#if !defined(AFX_AFD_H__298DD345_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_AFD_H__298DD345_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "AfdDlg.h"
#include "AlexfPerf.h"

/////////////////////////////////////////////////////////////////////////////
// CAfdApp:
// See Afd.cpp for the implementation of this class
//

#define PERF_ConnectSpeed	0
#define PERF_BytesRecvd		1
#define PERF_BytesXmit		2
#define PERF_ARRAY			3

class CAfdApp : public CWinApp
{
public:
	CString GetPassword(BOOL b = FALSE);
	CAfdDlg * pDlg;

	CAlexfPerf * pPerf[PERF_ARRAY];
	BOOL RunProgramsOnEvent(int iCondition);
	CString GetCurrentIPAddress();
	CString GetCurrentProviderName();
	BOOL IsConnected();
	BOOL SetLastCallText(CString sss);
	BOOL PushBigButton();
	BOOL StartSession();
	BOOL FinishSession();
	BOOL PostDialMessage(UINT unMsg, WPARAM rasconnstate, DWORD dwError);
	BOOL NotifyProviderChanged();
	CString sIniFileName;
	CString GetIniString(CString sec, CString key, CString str);
	BOOL SetIniString(CString sec, CString key, CString str);
	BOOL LOG(UINT nID);
	BOOL LOG(CString sss);

	CAfdApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAfdApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAfdApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		// DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFD_H__298DD345_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)