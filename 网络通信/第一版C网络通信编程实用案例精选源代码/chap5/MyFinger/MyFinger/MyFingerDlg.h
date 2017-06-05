// MyFingerDlg.h : header file
//

#if !defined(AFX_MYFINGERDLG_H__1920F870_0649_4685_A720_6D844B5EBF99__INCLUDED_)
#define AFX_MYFINGERDLG_H__1920F870_0649_4685_A720_6D844B5EBF99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyFingerDlg dialog

class CMyFingerDlg : public CDialog
{
// Construction
public:
	CMyFingerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyFingerDlg)
	enum { IDD = IDD_MYFINGER_DIALOG };
	CString	m_sHost;
	UINT	m_uPort;
	CString	m_sCommand;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFingerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyFingerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGo();
	afx_msg void OnQueryWhois();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFINGERDLG_H__1920F870_0649_4685_A720_6D844B5EBF99__INCLUDED_)
