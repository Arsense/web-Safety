// MyPortScannerDlg.h : header file
//

#if !defined(AFX_MYPORTSCANNERDLG_H__0518F53C_56C2_415E_B542_973B39480A54__INCLUDED_)
#define AFX_MYPORTSCANNERDLG_H__0518F53C_56C2_415E_B542_973B39480A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyPortScannerDlg dialog

class CMyPortScannerDlg : public CDialog
{
// Construction
public:
	unsigned int m_BEndAddress[4];
	unsigned int m_BBeginAddress[4];

	CString m_SResult;
	BOOL	m_bStop;
	CMyPortScannerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyPortScannerDlg)
	enum { IDD = IDD_MYPORTSCANNER_DIALOG };
	UINT	m_uPortBegin;
	UINT	m_uPortEnd;
	unsigned int		m_uThreadNum;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPortScannerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyPortScannerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBeginScan();
	afx_msg void OnStopScan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPORTSCANNERDLG_H__0518F53C_56C2_415E_B542_973B39480A54__INCLUDED_)
