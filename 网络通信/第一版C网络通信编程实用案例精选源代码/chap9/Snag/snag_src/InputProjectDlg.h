
// Project.h : header file
//

#if !defined(__PROJECT_H__)
#define __PROJECT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CProjectNameEdit subclass

class CProjectNameEdit : public CEdit
{
// Construction
public:
	CProjectNameEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectNameEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProjectNameEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProjectNameEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInputProjectDlg dialog

class CInputProjectDlg : public CDialog
{
protected:
	CString m_strProjectName;
	CProjectNameEdit m_ProjectEdit;
	CString m_strTitle;

public:
	CString GetProjectName() { return m_strProjectName; } 
	void SetTitle(CString& strTitle) { m_strTitle = strTitle; }

// Construction
public:
	CInputProjectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputProjectDlg)
	enum { IDD = IDD_PROJECT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputProjectDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__PROJECT_H__)
