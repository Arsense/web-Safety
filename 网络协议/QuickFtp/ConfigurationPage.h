#if !defined(AFX_CONFIGURATIONPAGE_H__6EFDF607_B640_4941_A1A1_653C006D5EA0__INCLUDED_)
#define AFX_CONFIGURATIONPAGE_H__6EFDF607_B640_4941_A1A1_653C006D5EA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigurationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage dialog
#include "resource.h"

class CConfigurationPage : public CDialog
{
// Construction
public:
	CConfigurationPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigurationPage)
	enum { IDD = IDD_PROPPAGE_CONFIGURATION };
	int		m_nPort;
	int		m_nMaxUsers;
	CString	m_strGoodbyeMessage;
	BOOL	m_bStartInTray;
	int		m_nTimeout;
	CString	m_strWelcomeMessage;
	BOOL	m_bLaunchOnStartup;
	BOOL	m_bAutoActivate;
	int		m_nLogLevel;
	CString	m_strApplicationName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigurationPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigurationPage)
	afx_msg void OnLaunchOnStartup();
	afx_msg void OnApply();
	afx_msg void OnView();
	afx_msg void OnClear();
	afx_msg void OnStartInTray();
	afx_msg void OnAutoactivate();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATIONPAGE_H__6EFDF607_B640_4941_A1A1_653C006D5EA0__INCLUDED_)
