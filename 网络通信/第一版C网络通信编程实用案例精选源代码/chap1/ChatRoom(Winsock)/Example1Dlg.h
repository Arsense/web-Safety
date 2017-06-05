// Example1Dlg.h : header file
//

#if !defined(AFX_EXAMPLE1DLG_H__C0F5FDF4_B722_4D2E_910B_CA551035131E__INCLUDED_)
#define AFX_EXAMPLE1DLG_H__C0F5FDF4_B722_4D2E_910B_CA551035131E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Server.h"
#include "Client.h"
#include <afxtempl.h>

#define SER_MESSAGE WM_USER + 100
#define CLI_MESSAGE WM_USER + 101

typedef CList <SOCKET,SOCKET&> SOCKET_ARRAY;

/////////////////////////////////////////////////////////////////////////////
// CExample1Dlg dialog

class CExample1Dlg : public CDialog
{
// Construction
public:
	CExample1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExample1Dlg)
	enum { IDD = IDD_EXAMPLE1_DIALOG };
	CIPAddressCtrl	m_ServerIp;
	UINT	m_uPort;
	CString	m_sShowText;
	CString	m_sInputText;
	//}}AFX_DATA
public:
	CServer m_server;
	CClient m_client;
	BOOL m_bInit;
	BOOL m_bClient;
	SOCKET_ARRAY m_connectionList;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExample1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExample1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSetserver();
	afx_msg void OnInputText();
	afx_msg LRESULT OnServerMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientMessage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLE1DLG_H__C0F5FDF4_B722_4D2E_910B_CA551035131E__INCLUDED_)
