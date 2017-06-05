// DelHistoryDlg.h : header file
//

#if !defined(AFX_DELHISTORYDLG_H__BA20DE4D_56BE_4A3D_9495_44672D75017E__INCLUDED_)
#define AFX_DELHISTORYDLG_H__BA20DE4D_56BE_4A3D_9495_44672D75017E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDelHistoryDlg dialog
#define SWEEP_BUFFER_SIZE			10000


class CDelHistoryDlg : public CDialog
{
// Construction
public:
	CDelHistoryDlg(CWnd* pParent = NULL);	// standard constructor

	enum DEL_CACHE_TYPE //要删除的类型。
	 {
	 File,//表示internet临时文件
	 Cookie //表示Cookie
	 };

// Dialog Data
	//{{AFX_DATA(CDelHistoryDlg)
	enum { IDD = IDD_DELHISTORY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,   BOOL bWipeIndexDat = FALSE);
	BOOL DeleteUrlCache(DEL_CACHE_TYPE type);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDelHistoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDelHistory();
	afx_msg void OnDelCookies();
	afx_msg void OnDelInternetTempFile();
	afx_msg void OnDelHistoryAddress();
	afx_msg void OnDelHistoryPassword();
	afx_msg void OnDelFavorite();
	afx_msg void OnButtonHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELHISTORYDLG_H__BA20DE4D_56BE_4A3D_9495_44672D75017E__INCLUDED_)
