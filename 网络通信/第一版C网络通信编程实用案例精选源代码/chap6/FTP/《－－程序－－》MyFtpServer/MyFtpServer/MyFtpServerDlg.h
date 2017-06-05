// MyFtpServerDlg.h : header file
//

#if !defined(AFX_MYFTPSERVERDLG_H__1ED0EDB8_3D14_4B7A_B4FD_432DEECFFD21__INCLUDED_)
#define AFX_MYFTPSERVERDLG_H__1ED0EDB8_3D14_4B7A_B4FD_432DEECFFD21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyFtpServerDlg dialog
#include <afxtempl.h>
#include "server.h"

class CMyFtpServerDlg : public CDialog
{
// Construction
public:
	CMyFtpServerDlg(CWnd* pParent = NULL);	// standard constructor
	struct CAccount
	{
		CString account;
		CString password;
		CString directory;
	};
	CArray<CAccount,CAccount&> m_AccountArray;
	int m_port;
	CString m_default_directory;
	CServer m_server;

// Dialog Data
	//{{AFX_DATA(CMyFtpServerDlg)
	enum { IDD = IDD_MYFTPSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFtpServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyFtpServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAddAccount();
	afx_msg void OnDelAccount();
	afx_msg void OnRefreshPort();
	virtual void OnOK();
	afx_msg void OnStartServer();
	afx_msg void OnRefreshDefaultdirectory();
	afx_msg void OnAccountInfor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFTPSERVERDLG_H__1ED0EDB8_3D14_4B7A_B4FD_432DEECFFD21__INCLUDED_)
