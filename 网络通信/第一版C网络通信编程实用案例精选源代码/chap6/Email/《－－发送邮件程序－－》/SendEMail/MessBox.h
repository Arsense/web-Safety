#if !defined(AFX_MESSBOX_H__A7E11AFF_630C_4ADE_9301_23862918C7E0__INCLUDED_)
#define AFX_MESSBOX_H__A7E11AFF_630C_4ADE_9301_23862918C7E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessBox dialog

class CMessBox : public CDialog
{
// Construction
public:
	CMessBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessBox)
	enum { IDD = IDD_MESSAGE };
	CString	m_sMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSBOX_H__A7E11AFF_630C_4ADE_9301_23862918C7E0__INCLUDED_)
