#if !defined(AFX_ACCOUNTDIALOG_H__322EE78C_90A4_4E84_895C_1A2E1107E216__INCLUDED_)
#define AFX_ACCOUNTDIALOG_H__322EE78C_90A4_4E84_895C_1A2E1107E216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccountDialog dialog

class CAccountDialog : public CDialog
{
// Construction
public:
	CAccountDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccountDialog)
	enum { IDD = IDD_ACCOUNT };
	CString	m_directory;
	CString	m_password;
	CString	m_user;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccountDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTDIALOG_H__322EE78C_90A4_4E84_895C_1A2E1107E216__INCLUDED_)
