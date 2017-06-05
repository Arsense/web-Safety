// InternetDownloadDlg.h : header file
//

#if !defined(AFX_INTERNETDOWNLOADDLG_H__B933E35F_6FE1_4BF3_91D8_249EB2064D18__INCLUDED_)
#define AFX_INTERNETDOWNLOADDLG_H__B933E35F_6FE1_4BF3_91D8_249EB2064D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInternetDownloadDlg dialog

class CInternetDownloadDlg : public CDialog
{
// Construction
public:
	CInternetDownloadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInternetDownloadDlg)
	enum { IDD = IDD_INTERNETDOWNLOAD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInternetDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	int InternetGetFile (CString szUrl,CString szFileName);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInternetDownloadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNETDOWNLOADDLG_H__B933E35F_6FE1_4BF3_91D8_249EB2064D18__INCLUDED_)
