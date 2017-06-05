#if !defined(AFX_LOGIN_H__5941F3CB_BCEB_4E76_AA86_C245D527FFFC__INCLUDED_)
#define AFX_LOGIN_H__5941F3CB_BCEB_4E76_AA86_C245D527FFFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog

class CLogin : public CDialog
{
// Construction
public:
	CLogin(CWnd* pParent = NULL);   // standard constructor
	CString m_strIP;                //127.0.0.1
// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_DIALOG_LOGIN };
	CIPAddressCtrl	m_ControlIP;   //127.0.0.1
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogin)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__5941F3CB_BCEB_4E76_AA86_C245D527FFFC__INCLUDED_)
