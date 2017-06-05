

#if !defined(AFX_PAGE4_H__298DD352_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_PAGE4_H__298DD352_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page4.h : header file
//

#include "AFDialPanel.h"

/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog

class CPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage4)

// Construction
public:
	UINT uTimer;
	DWORD dwInitBytesSent;
	DWORD dwInitBytesRecv;
	CTime TimeBeginSession;
	CString sTimeTotalLast;
	BOOL bWindowActivated;

	BOOL InitPropertyPage();
	BOOL StartSession();
	BOOL FinishSession();
	BOOL SetStatisticData();
	CString ConvercTStoS(CTimeSpan ts);
	BOOL LoadTimeINI();
	BOOL SaveTimeINI();
	CTimeSpan ParseSavedTime(CString sss);
	CPage4();
	~CPage4();

// Dialog Data
	//{{AFX_DATA(CPage4)
	enum { IDD = IDD_DIALOG4 };
	CAFDialPanel m_PaneModem;
	CString	m_sRecv;
	CString	m_sSent;
	CString	m_sSpeed;
	CString	m_sTimeCur;
	CString	m_sTimeTot;
	CString	m_sConnectionName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage4)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage4)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClean();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE4_H__298DD352_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
