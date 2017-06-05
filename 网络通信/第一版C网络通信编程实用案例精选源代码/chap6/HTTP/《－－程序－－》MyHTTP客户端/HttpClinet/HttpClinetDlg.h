// HttpClinetDlg.h : header file
//

#if !defined(AFX_HTTPCLINETDLG_H__40559E9C_6BED_4902_83C7_95C66FD2529E__INCLUDED_)
#define AFX_HTTPCLINETDLG_H__40559E9C_6BED_4902_83C7_95C66FD2529E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHttpClinetDlg dialog

class CHttpClinetDlg : public CDialog
{
// Construction
public:
	CHttpClinetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHttpClinetDlg)
	enum { IDD = IDD_HTTPCLINET_DIALOG };
	CString	m_URL;
	CString	m_infor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpClinetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHttpClinetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGet();
	afx_msg void OnPost();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPCLINETDLG_H__40559E9C_6BED_4902_83C7_95C66FD2529E__INCLUDED_)
