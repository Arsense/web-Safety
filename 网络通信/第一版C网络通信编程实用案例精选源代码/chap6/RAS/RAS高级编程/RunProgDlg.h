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

#if !defined(AFX_RUNPROGDLG_H__E38206A6_0C76_11D3_9865_98738738BC40__INCLUDED_)
#define AFX_RUNPROGDLG_H__E38206A6_0C76_11D3_9865_98738738BC40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunProgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunProgDlg dialog

#define RUN_ONLINE  0
#define RUN_HANGUP  1
#define RUN_CONNECT 2

class CRunProgDlg : public CDialog
{
// Construction
public:
	CRunProgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRunProgDlg)
	enum { IDD = IDD_RUN_PROGRAM };
	CString	m_sName;
	int		m_iRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunProgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunProgDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNPROGDLG_H__E38206A6_0C76_11D3_9865_98738738BC40__INCLUDED_)
