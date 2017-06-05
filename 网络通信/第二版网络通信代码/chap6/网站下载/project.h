
// Project.h : header file
//

#if !defined(__PROJECT_H__)
#define __PROJECT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CProjectEdit subclass

class CProjectEdit : public CEdit
{
// Construction
public:
	CProjectEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProjectEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProjectEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CProjectDlg dialog

class CProjectDlg : public CDialog
{
protected:
	CString m_strProjectName;
	CProjectEdit m_ProjectEdit;
	CString m_strTitle;

public:
	CString GetProjectName() { return m_strProjectName; } 
	void SetTitle(CString& strTitle) { m_strTitle = strTitle; }

// Construction
public:
	CProjectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectDlg)
	enum { IDD = IDD_PROJECT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHelpbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__PROJECT_H__)
