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

// AfdDlg.h : header file
//

#if !defined(AFX_AFDDLG_H__298DD347_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_AFDDLG_H__298DD347_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#include "trayicon.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPage1;
class CPage2;
class CPage3;
class CPage4;
class CPage5;

/////////////////////////////////////////////////////////////////////////////
// CAfdDlg dialog

class CAfdDlg : public CDialog
{
// Construction
public:
	BOOL Hide();
	BOOL m_bStartMinimized;
	CTrayIcon Tray;
	CPropertySheet m_dlgPropSheet;
	CPage1 * page1;
	CPage2 * page2;
	CPage3 * page3;
	CPage4 * page4;
	CPage5 * page5;
	CAfdDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAfdDlg)
	enum { IDD = IDD_AFD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAfdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAfdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnHide();
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShow();
	afx_msg void OnNcPaint();
	afx_msg void OnTrayDial();
	afx_msg void OnUpdateTrayDial(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFDDLG_H__298DD347_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
