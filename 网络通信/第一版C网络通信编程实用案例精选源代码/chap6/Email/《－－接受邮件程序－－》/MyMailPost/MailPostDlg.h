// MailPostDlg.h : header file
//

#if !defined(AFX_MAILPOSTDLG_H__B9F99532_1593_4906_8318_FBCC7F12C709__INCLUDED_)
#define AFX_MAILPOSTDLG_H__B9F99532_1593_4906_8318_FBCC7F12C709__INCLUDED_

#include "Pop3.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMailPostDlg dialog

class CMailPostDlg : public CDialog
{
// Construction
public:
	CMailPostDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMailPostDlg)
	enum { IDD = IDD_MAILPOST_DIALOG };
	CListCtrl	m_lstMail;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailPostDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CPop3 m_pop3;
	CString m_sAddr;
	CString m_sUser;
	CString m_sPassword;

// Implementation
protected:
	void Refresh(void);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMailPostDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnReceive();
	afx_msg void OnReverse();
	afx_msg void OnDelete();
	afx_msg void OnDisconnect();
	afx_msg void OnOpenMail(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILPOSTDLG_H__B9F99532_1593_4906_8318_FBCC7F12C709__INCLUDED_)
