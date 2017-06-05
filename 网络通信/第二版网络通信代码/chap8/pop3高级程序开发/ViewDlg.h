#if !defined(AFX_VIEWDLG_H__079717A1_74AE_49F9_B5AC_2B2895BEDD24__INCLUDED_)
#define AFX_VIEWDLG_H__079717A1_74AE_49F9_B5AC_2B2895BEDD24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog

class CViewDlg : public CDialog
{
// Construction
public:
	CViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewDlg)
	enum { IDD = IDD_MSGVIEW };
	CString	text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewDlg)
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDLG_H__079717A1_74AE_49F9_B5AC_2B2895BEDD24__INCLUDED_)
