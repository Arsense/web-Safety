#if !defined(AFX_DIRECTDRIVER_H__BE378EED_A7CB_4CEC_B38F_9EF78EF3DB0E__INCLUDED_)
#define AFX_DIRECTDRIVER_H__BE378EED_A7CB_4CEC_B38F_9EF78EF3DB0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectDriver.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirectDriver dialog

class CDirectDriver : public CDialog
{
// Construction
public:
	CDirectDriver(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDirectDriver)
	enum { IDD = IDD_DIRECTDRIVER };
	CString	m_localname;
	CString	m_password;
	CString	m_remotename;
	CString	m_username;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectDriver)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDirectDriver)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTDRIVER_H__BE378EED_A7CB_4CEC_B38F_9EF78EF3DB0E__INCLUDED_)
