#if !defined(AFX_USERACCOUNTSDLG_H__A09BFC68_C519_4A73_B07A_2926DE28475A__INCLUDED_)
#define AFX_USERACCOUNTSDLG_H__A09BFC68_C519_4A73_B07A_2926DE28475A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserAccountsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserAccountsDlg dialog
#include "resource.h"
#include "AddUserInfoDlg.h"
#include "UserManager.h"

class CUserAccountsDlg : public CDialog
{
// Construction
public:
	CUserAccountsDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_strUserName;//暂时加上，对我来说好像没有
	CArray<CUserAccount, CUserAccount&> m_UsersArray;

// Dialog Data
	//{{AFX_DATA(CUserAccountsDlg)
	enum { IDD = IDD_USERACCOUNTS };
	CListCtrl	m_UsersList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserAccountsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserAccountsDlg)
	afx_msg void OnBtnadduseraccount();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnedituseraccount();
	afx_msg void OnBtndeluseraccount();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERACCOUNTSDLG_H__A09BFC68_C519_4A73_B07A_2926DE28475A__INCLUDED_)
