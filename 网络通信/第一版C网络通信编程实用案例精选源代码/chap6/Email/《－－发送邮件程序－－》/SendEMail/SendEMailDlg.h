// SendEMailDlg.h : header file
//

#if !defined(AFX_SENDEMAILDLG_H__E40D4553_A94A_4C79_AFB6_69299D8B9C09__INCLUDED_)
#define AFX_SENDEMAILDLG_H__E40D4553_A94A_4C79_AFB6_69299D8B9C09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSendEMailDlg dialog
#include "ESocket.h"

class CSendEMailDlg : public CDialog
{
// Construction
public:
	CString m_sReceivedData;
	CESocket m_socket;
	CString m_sSendString[6];
	void ReceiveMessage(int count);
	CSendEMailDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSendEMailDlg)
	enum { IDD = IDD_SENDEMAIL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendEMailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSendEMailDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSend();
	afx_msg void OnHelp();
	afx_msg void OnShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDEMAILDLG_H__E40D4553_A94A_4C79_AFB6_69299D8B9C09__INCLUDED_)
