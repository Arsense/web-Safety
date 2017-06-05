// MSCOMMDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm1.h"
//}}AFX_INCLUDES

#if !defined(AFX_MSCOMMDLG_H__F4C72F98_5333_461F_96D2_6BA9B674C40B__INCLUDED_)
#define AFX_MSCOMMDLG_H__F4C72F98_5333_461F_96D2_6BA9B674C40B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMSCOMMDlg dialog
class CMSCOMMDlg : public CDialog
{
// Construction
public:
	CMSCOMMDlg(CWnd* pParent = NULL);	// standard constructor
	CString m_comdata;

// Dialog Data
	//{{AFX_DATA(CMSCOMMDlg)
	enum { IDD = IDD_MSCOMM_DIALOG };
	CMSComm	m_Comm;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSCOMMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMSCOMMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOnCommMscomm1();
	afx_msg void OnStartReceive();
	afx_msg void OnStopReceive();
	afx_msg void OnSendData();
	afx_msg void OnSetComm();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSCOMMDLG_H__F4C72F98_5333_461F_96D2_6BA9B674C40B__INCLUDED_)
