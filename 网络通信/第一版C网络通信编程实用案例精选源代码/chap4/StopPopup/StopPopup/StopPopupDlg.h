// StopPopupDlg.h : header file
//

#if !defined(AFX_STOPPOPUPDLG_H__FEAC7D24_F06F_4B6C_853D_4ED468125DDB__INCLUDED_)
#define AFX_STOPPOPUPDLG_H__FEAC7D24_F06F_4B6C_853D_4ED468125DDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStopPopupDlg dialog

class CStopPopupDlg : public CDialog
{
// Construction
public:
	CStopPopupDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStopPopupDlg)
	enum { IDD = IDD_STOPPOPUP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStopPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStopPopupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStop();
	afx_msg void OnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPPOPUPDLG_H__FEAC7D24_F06F_4B6C_853D_4ED468125DDB__INCLUDED_)
