// HyperLinkDlg.h : header file
//

#if !defined(AFX_HYPERLINKDLG_H__3857C8C8_5816_4061_BC16_F4DC479D93A2__INCLUDED_)
#define AFX_HYPERLINKDLG_H__3857C8C8_5816_4061_BC16_F4DC479D93A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkDlg dialog
#include "HyperLinker.h"

class CHyperLinkDlg : public CDialog
{
// Construction
public:
	CHyperLinkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHyperLinkDlg)
	enum { IDD = IDD_HYPERLINK_DIALOG };
	CHyperLinker	m_net;
	CHyperLinker	m_localfile;
	CHyperLinker	m_linker;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHyperLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINKDLG_H__3857C8C8_5816_4061_BC16_F4DC479D93A2__INCLUDED_)
