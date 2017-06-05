// ShortCutDlg.h : header file
//

#if !defined(AFX_SHORTCUTDLG_H__98360DBC_1AF1_405D_B805_433153C15A9C__INCLUDED_)
#define AFX_SHORTCUTDLG_H__98360DBC_1AF1_405D_B805_433153C15A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShortCutDlg dialog

class CShortCutDlg : public CDialog
{
// Construction
public:
	CShortCutDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CShortCutDlg)
	enum { IDD = IDD_SHORTCUT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortCutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CShortCutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnCreateActiveDesktop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTCUTDLG_H__98360DBC_1AF1_405D_B805_433153C15A9C__INCLUDED_)
