// MailerDlg.h : header file
//

#if !defined(AFX_MAILERDLG_H__74764E06_5206_11D6_9CE4_EE1F83ADA179__INCLUDED_)
#define AFX_MAILERDLG_H__74764E06_5206_11D6_9CE4_EE1F83ADA179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMailerDlg dialog

class CMailerDlg : public CDialog
{
// Construction
public:
	CMailerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMailerDlg)
	enum { IDD = IDD_MAILER_DIALOG };
	CString	m_From;
	CString	m_Message;
	CString	m_Server;
	CString	m_To;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMailerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILERDLG_H__74764E06_5206_11D6_9CE4_EE1F83ADA179__INCLUDED_)
