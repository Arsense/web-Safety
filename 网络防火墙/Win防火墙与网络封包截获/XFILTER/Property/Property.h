// Property.h : main header file for the PROPERTY application
//

#if !defined(AFX_PROPERTY_H__7DC7EC5B_1F4F_43E4_BD54_1E50EC587878__INCLUDED_)
#define AFX_PROPERTY_H__7DC7EC5B_1F4F_43E4_BD54_1E50EC587878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "..\common\xinstall.h"
#include ".\internet\internet.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyApp:
// See Property.cpp for the implementation of this class
//

class CPropertyApp : public CWinApp
{
public:
	CPropertyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPropertyApp)
	afx_msg void OnAppExit();
	afx_msg void OnAbout();
	afx_msg void OnAclSet();
	afx_msg void OnLogQuery();
	afx_msg void OnSystemSet();
	afx_msg void OnUpdateAbout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAclSet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLogQuery(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemSet(CCmdUI* pCmdUI);
	afx_msg void OnControlFrame();
	afx_msg void OnUpdateControlFrame(CCmdUI* pCmdUI);
	afx_msg void OnPacketMonitor();
	afx_msg void OnUpdatePacketMonitor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//owner add
public:
	void	ExitApp			();
	BOOL	CheckDlgVisible	(void* pdlg,  BOOL ShowMessage = TRUE);
	BOOL	CheckDlg		(BOOL ShowMessage = TRUE);
	BOOL	InitDllData		();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL	m_EnablePacketMonitor;
	BOOL	m_EnableQueryLog;
	BOOL	m_EnableAcl;
	BOOL	m_EnableSystemSet;
	BOOL	m_EnableAbout;
	BOOL	m_EnableExit;
	BOOL	m_EnableContolFrame;

	HINSTANCE			m_hTcpIpDog;
	XF_IO_CONTROL		m_XfIoControl;

	CXInstall			m_Install;
	CHttpRequest		m_DownloadCommand;
	CHttpRequest		m_RegisterRequest;
	TCHAR				m_sMessage[MAX_NET_COMMAND][MAX_NET_MESSAGE_LENTH];

public:
	void SetAutoStart(BOOL IsCheckFromAcl = TRUE);
	void SetTrayIcon();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTY_H__7DC7EC5B_1F4F_43E4_BD54_1E50EC587878__INCLUDED_)
