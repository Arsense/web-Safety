// AddToToolBarDlg.h : header file
//

#if !defined(AFX_ADDTOTOOLBARDLG_H__381A206B_CEFE_47E5_BA7C_B2C5D1C00052__INCLUDED_)
#define AFX_ADDTOTOOLBARDLG_H__381A206B_CEFE_47E5_BA7C_B2C5D1C00052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAddToToolBarDlg dialog

class CAddToToolBarDlg : public CDialog
{
// Construction
public:
	CAddToToolBarDlg(CWnd* pParent = NULL);	// standard constructor
	void AddEventSource(CString keyname,CString pathname,CString filename);
	void GenerateRadomName(CString &valuename);
// Dialog Data
	//{{AFX_DATA(CAddToToolBarDlg)
	enum { IDD = IDD_ADDTOTOOLBAR_DIALOG };
	CButton	m_BigIconButton;
	CButton	m_SmallIconButton;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddToToolBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAddToToolBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDTOTOOLBARDLG_H__381A206B_CEFE_47E5_BA7C_B2C5D1C00052__INCLUDED_)
