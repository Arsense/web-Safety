#if !defined(AFX_ABOUTHELPDLG_H__0544444B_7938_4954_BD1B_347F52D1E94A__INCLUDED_)
#define AFX_ABOUTHELPDLG_H__0544444B_7938_4954_BD1B_347F52D1E94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutHelpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutHelpDlg dialog

class CAboutHelpDlg : public CDialog
{
// Construction
public:
	CAboutHelpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutHelpDlg)
	enum { IDD = IDD_DIALOGHELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutHelpDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTHELPDLG_H__0544444B_7938_4954_BD1B_347F52D1E94A__INCLUDED_)
