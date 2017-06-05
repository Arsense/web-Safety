// MyPingDlg.h : header file
//

#if !defined(AFX_MYPINGDLG_H__BFB6136E_09C2_4723_B6FA_19BD996E0C63__INCLUDED_)
#define AFX_MYPINGDLG_H__BFB6136E_09C2_4723_B6FA_19BD996E0C63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyPingDlg dialog
#include "Ping.h"

class CMyPingDlg : public CDialog
{
// Construction
public:
	CMyPingDlg(CWnd* pParent = NULL);	// standard constructor
	CPing m_pinger;
	CString m_result;
// Dialog Data
	//{{AFX_DATA(CMyPingDlg)
	enum { IDD = IDD_MYPING_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyPingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPINGDLG_H__BFB6136E_09C2_4723_B6FA_19BD996E0C63__INCLUDED_)
