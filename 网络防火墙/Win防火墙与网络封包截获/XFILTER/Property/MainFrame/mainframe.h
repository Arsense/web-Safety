#if !defined(AFX_MAINFRAME_H__B2CAA637_A0B0_47D0_805F_2C80E8304C3F__INCLUDED_)
#define AFX_MAINFRAME_H__B2CAA637_A0B0_47D0_805F_2C80E8304C3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrame.h : header file
//

#include "..\systemtray\systemtray.h"

//=============================================================================================
// CMainFrame frame

class CMainFrame : public CFrameWnd
{
protected:
	DECLARE_DYNCREATE(CMainFrame)
public:
	CMainFrame();           // protected constructor used by dynamic creation

// Attributes

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainFrame();

	// Generated message map functions
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnTrayNotification(UINT wParam, LONG lParam);
	afx_msg LONG OnSessionNotification(UINT wParam, LONG lParam);
	afx_msg LONG OnAclNotification(UINT wParam, LONG lParam);
	afx_msg LONG OnShowNetMessage(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
// owner add

public:
	static void		InitMainSheet(int iActivePage = 0, int IsShow = SW_SHOW);
};

DWORD WINAPI SplashIcon(LPVOID pVoid);
DWORD WINAPI SplashMessage(LPVOID pVoid);

//=============================================================================================

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__B2CAA637_A0B0_47D0_805F_2C80E8304C3F__INCLUDED_)
