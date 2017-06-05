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

#if !defined(AFX_PAGE1_H__298DD34F_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_PAGE1_H__298DD34F_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page1.h : header file
//

#include <ras.h>
#include <raserror.h>

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog

class CPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1)

// Construction
public:
	BOOL CheckingConnected();
	BOOL CheckingDialing();
	BOOL StartDial();
	BOOL bSpeedChecked;			// надо 1 раз проверить скорость
	BOOL bAuthenticationBegin;	// надо 1 раз запустить внешние программы
	CString sCurrentProvider;
	BOOL bDialing;			// dial button in "pressed" stage
	BOOL bDialNow;			// нельзя входить в OnTimer (Запущена RasDial, ждем ответа)
							// иначе таймер запущен, но звонить не надо
	BOOL bConnected;		// в онлайне
	BOOL bInOnTimer;		// в обработчике онтаймер
	BOOL bPhone;			// Курсор в дереве стоит на телефонах
	CTime tBeginDial;		// момент запуска rasDia, нужен для timeout
	UINT uTimer;			// for set timer

	BOOL InitPropertyPage();
	BOOL CheckExternalConnect();
	CString GetIPAddress();
	BOOL QueryConnections();
	BOOL QueryPhones(CString sEntryName);
	BOOL GetActiveConnectionParam(CString & Conn, CString & Phon);
	CString GetRasConnState(RASCONNSTATE rasconn);
	BOOL HangUp(BOOL bExitApp);
	CString GetPhoneListSelection();
	BOOL LoadDefautPhoneNumber(CString sEntryName);
	BOOL ParseArgs(CString sss);
	BOOL PublicOnDial();
	void SetButtonsState();
	CPage1();
	~CPage1();

// Dialog Data
	//{{AFX_DATA(CPage1)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_PhoneList;
	CProgressCtrl	m_ProgressTimeout;
	CComboBox	m_Providers;
	CString	m_StatusText;
	CString	m_LastCallText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonProvAdd();
	afx_msg void OnButtonProvEdit();
	afx_msg void OnButtonProvDel();
	afx_msg void OnButtonTelAdd();
	afx_msg void OnButtonTelEdit();
	afx_msg void OnButtonTelDel();
	afx_msg void OnDial();
	afx_msg void OnSelchangeComboProvider();
	afx_msg void OnClickListPhones(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDialNextPhone();
	afx_msg LRESULT OnDialFunc(WPARAM w, LPARAM l);
	virtual void OnOK();
//	afx_msg void OnButtonDial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1_H__298DD34F_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
