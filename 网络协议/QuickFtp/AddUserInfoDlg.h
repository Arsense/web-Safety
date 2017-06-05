#if !defined(AFX_ADDUSERINFODLG_H__A504FBC9_19CA_446E_9C4E_84D0B9A77056__INCLUDED_)
#define AFX_ADDUSERINFODLG_H__A504FBC9_19CA_446E_9C4E_84D0B9A77056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddUserInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddUserInfoDlg dialog
#include "resource.h"

class CAddUserInfoDlg : public CDialog
{
// Construction
public:
	CAddUserInfoDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_strTitle;  //对话框标题变量
// Dialog Data
	//{{AFX_DATA(CAddUserInfoDlg)
	enum { IDD = IDD_ADD_USERINFO };
	CString	m_strPath;
	BOOL	m_bAllowCreateDirectory;
	BOOL	m_bAllowDelete;
	BOOL	m_bAllowDownload;
	BOOL	m_bAllowRename;
	BOOL	m_bAllowUpload;
	CString	m_strUserName;
	CString	m_strUserPass;
	CString	m_strUserPassAgain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddUserInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddUserInfoDlg)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDUSERINFODLG_H__A504FBC9_19CA_446E_9C4E_84D0B9A77056__INCLUDED_)
