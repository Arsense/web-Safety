// pop3Dlg.h : header file
//

#if !defined(AFX_POP3DLG_H__DDAC943B_6A47_4AC8_BAF6_E2FDA476B70B__INCLUDED_)
#define AFX_POP3DLG_H__DDAC943B_6A47_4AC8_BAF6_E2FDA476B70B__INCLUDED_

#include "pop31.h"
#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPop3Dlg dialog

class CPop3Dlg : public CDialog
{
// Construction
	
public:
	CPop3 pop3;
	void Dispatch(LONG param);
	CPop3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPop3Dlg)
	enum { IDD = IDD_POP3_DIALOG };
	CRichEditCtrl	ctlInfo;
	CString	info;
	CString	pass;
	CString	server;
	CString	user;
	BOOL	del;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPop3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPop3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConn();
	afx_msg void OnDisc();
	afx_msg void OnView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POP3DLG_H__DDAC943B_6A47_4AC8_BAF6_E2FDA476B70B__INCLUDED_)
