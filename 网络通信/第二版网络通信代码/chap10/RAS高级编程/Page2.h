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

#if !defined(AFX_PAGE2_H__298DD350_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_PAGE2_H__298DD350_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2 dialog

class CPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2)

public:
	CString GetPassword(BOOL b = FALSE);
	BOOL bWindowActivated;
	static CRITICAL_SECTION sCriticalSection;
	static CPage2 * ziz;
	BOOL bThreadExitImmediately;
	int iThreadWorkingNow;
	static UINT RunPrograms(LPVOID lpThreadParameter);
	int RunConditionS2I(CString s);
	CString RunConditionI2S(int i);
	BOOL LoadPrograms(CString Provider);
	BOOL InitPropertyPage();
	BOOL Reload();
	BOOL SaveCurrentUNP();
	BOOL LoadCurrentUNP();
	CPage2();
	~CPage2();

// Dialog Data
	//{{AFX_DATA(CPage2)
	enum { IDD = IDD_DIALOG2 };
//	CListCtrl	m_listProgs;
	CString	m_sUserName;
	CString	m_sPassword;
	CString	m_sSoundLogin;
	CString	m_sSoundLogout;
	CString	m_sConnectionName;
	BOOL	m_bHidePassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnLoadLogin();
//	afx_msg void OnLoadLogout();
//	afx_msg void OnTestLogin();
//	afx_msg void OnTestLogout();
	afx_msg void OnKillfocusEditPassword();
//	afx_msg void OnKillfocusEditSoundLogin();
//	afx_msg void OnKillfocusEditSoundLogout();
	afx_msg void OnKillfocusEditUserName();
//	afx_msg void OnButtonProgAdd();
//	afx_msg void OnButtonProgEdit();
//	afx_msg void OnButtonProgDel();
	afx_msg void OnHidepwd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE2_H__298DD350_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
