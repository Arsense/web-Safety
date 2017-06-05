
#if !defined(AFX_PAGE3_H__298DD351_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_PAGE3_H__298DD351_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog

class CPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3)

// Construction
public:
	BOOL InitPropertyPage();
	BOOL PingIP(ULONG ulAddr);
	BOOL PingIP(CString sss);
	BOOL PingISPServer();
	CTime LastTrafficCheck;
	CTime LastPingCheck;
	CTime TimeBeginSession;
	DWORD dwInitBytesSent;
	DWORD dwInitBytesRecv;
	DWORD * dwTraffic;
	UINT uTimer;
	BOOL SaveAllToIni();
	BOOL LoadAllFromIni();
	BOOL StartSession();
	BOOL FinishSession();
	CPage3();
	~CPage3();

// Dialog Data
	//{{AFX_DATA(CPage3)
	enum { IDD = IDD_DIALOG3 };
	CString	m_sLog;
	CString	m_sPing;
	CString	m_sSpeed;
	CString	m_sTermKB;
	CString	m_sTermTime;
	CString	m_sTimeout;
	int		m_iTonePulse;
	BOOL	m_bHangUp;
	BOOL	m_bLog;
	BOOL	m_bPing;
	BOOL	m_bRedial;
	BOOL	m_bSpeed;
	BOOL	m_bTerm;
	BOOL	m_bTimeout;
	BOOL	m_bInterfaceStartMinimized;
	BOOL	m_bInterfaceMinimizeOnConnect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonFileLog();
	afx_msg void OnButtonVewLog();
	afx_msg void OnCheckHangup();
	afx_msg void OnCheckLog();
	afx_msg void OnCheckPing();
	afx_msg void OnCheckRedial();
	afx_msg void OnCheckSpeed();
	afx_msg void OnCheckTerminate();
	afx_msg void OnCheckTimeout();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnKillfocusEditLog();
	afx_msg void OnKillfocusEditPing();
	afx_msg void OnKillfocusEditSpeed();
	afx_msg void OnKillfocusEditTerminate();
	afx_msg void OnKillfocusEditTermtime();
	afx_msg void OnKillfocusEditTimeout();
	afx_msg void OnCheckInterfaceStartMinimized();
	afx_msg void OnCheckInterfaceStartMinimized2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE3_H__298DD351_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
