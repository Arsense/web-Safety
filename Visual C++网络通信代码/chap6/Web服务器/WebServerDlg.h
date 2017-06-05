// WebServerDlg.h : header file
//

#if !defined(AFX_WEBSERVERDLG_H__8F4AD293_7AED_4ED5_A263_0ED2ED7032C8__INCLUDED_)
#define AFX_WEBSERVERDLG_H__8F4AD293_7AED_4ED5_A263_0ED2ED7032C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWebServerDlg dialog

#include "HTTPServer.h"

#define TIMER_ID_1	1
#define TIMER_TO_1	500

class CWebServerDlg : public CDialog
{
public:
	UINT	nTimerID;
	BOOL	m_bRun;
// Construction
public:
	void RestoreSettings();
	void SaveSettings();
	CWebServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWebServerDlg)
	enum { IDD = IDD_WEBSERVER_DIALOG };
	CStatic	m_nVisitors;
	CStatic	m_nBytesRecv;
	CStatic	m_nBytesSent;
	CStatic	m_nRequests;
	CStatic	m_nActiveConn;
	CString	m_szHomeDir;
	CString	m_szDefIndex;
	int		m_Port;
	int		m_PTO;
	CString	m_szStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CHTTPServer WebServer;

	// Generated message map functions
	//{{AFX_MSG(CWebServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReset();
	afx_msg void OnHomedirbrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBSERVERDLG_H__8F4AD293_7AED_4ED5_A263_0ED2ED7032C8__INCLUDED_)
