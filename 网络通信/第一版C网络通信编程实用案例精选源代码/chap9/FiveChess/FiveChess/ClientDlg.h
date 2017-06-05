#if !defined(AFX_CLIENTDLG_H__66D9B3B1_E6FA_4ABB_BABB_FDE44B9C33D7__INCLUDED_)
#define AFX_CLIENTDLG_H__66D9B3B1_E6FA_4ABB_BABB_FDE44B9C33D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_SETCLIENT };
	CIPAddressCtrl	m_address;
	int		m_iPort;
	//}}AFX_DATA
	BYTE    m_bIP[4];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	afx_msg void OnButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__66D9B3B1_E6FA_4ABB_BABB_FDE44B9C33D7__INCLUDED_)
