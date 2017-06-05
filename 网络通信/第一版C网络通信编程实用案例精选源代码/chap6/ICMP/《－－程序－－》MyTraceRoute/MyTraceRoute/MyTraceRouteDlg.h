// MyTraceRouteDlg.h : header file
//

#if !defined(AFX_MYTRACEROUTEDLG_H__363DF639_4A4C_4D17_BF15_E9678227E995__INCLUDED_)
#define AFX_MYTRACEROUTEDLG_H__363DF639_4A4C_4D17_BF15_E9678227E995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyTraceRouteDlg dialog

class CMyTraceRouteDlg : public CDialog
{
// Construction
public:
	CString m_result;
	CMyTraceRouteDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyTraceRouteDlg)
	enum { IDD = IDD_MYTRACEROUTE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTraceRouteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyTraceRouteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTracert();
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTRACEROUTEDLG_H__363DF639_4A4C_4D17_BF15_E9678227E995__INCLUDED_)
