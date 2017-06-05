// myWinInet_1Dlg.h : header file
//

#if !defined(AFX_MYWININET_1DLG_H__D330BD93_9226_4E72_B200_EFFAE46F0784__INCLUDED_)
#define AFX_MYWININET_1DLG_H__D330BD93_9226_4E72_B200_EFFAE46F0784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyWinInet_1Dlg dialog

class CMyWinInet_1Dlg : public CDialog
{
// Construction
public:
	CMyWinInet_1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyWinInet_1Dlg)
	enum { IDD = IDD_MYWININET_1_DIALOG };
	CString	m_SiteInfo;
	CString	m_SiteName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWinInet_1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyWinInet_1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWININET_1DLG_H__D330BD93_9226_4E72_B200_EFFAE46F0784__INCLUDED_)
