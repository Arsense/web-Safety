// modemDlg.h : header file
//

#if !defined(AFX_MODEMDLG_H__81318247_5A98_11D2_ABDF_C05C49757B0D__INCLUDED_)
#define AFX_MODEMDLG_H__81318247_5A98_11D2_ABDF_C05C49757B0D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CModemDlg dialog

class CModemDlg : public CDialog
{
// Construction
public:
	CModemDlg(CWnd* pParent = NULL);	// standard constructor
    HANDLE hCommDev;
	int vcon,key,vconcount,ring;
	const int TIMER_ID1;
	const int TIMER_ID2;
 
// Dialog Data
	//{{AFX_DATA(CModemDlg)
	enum { IDD = IDD_MODEM_DIALOG };
	CButton	m_hangup;
	CButton	m_call;
	CEdit	m_editctrl;
	CString	m_editchar;
	CString	m_modem;
	CString	m_com;
	CString	m_phone;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

//	UINT bitmapIDs[2];
//	UINT TimerOn;
	// Generated message map functions
	//{{AFX_MSG(CModemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeEdit1();
	afx_msg void Oncall();
	afx_msg void Onhangup();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnExit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMDLG_H__81318247_5A98_11D2_ABDF_C05C49757B0D__INCLUDED_)
