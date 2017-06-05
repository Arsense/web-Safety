// MySerialComDlg.h : header file
//

#if !defined(AFX_MYSERIALCOMDLG_H__C5549B80_E38C_4833_8CBB_98488ECABF04__INCLUDED_)
#define AFX_MYSERIALCOMDLG_H__C5549B80_E38C_4833_8CBB_98488ECABF04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "MyComm.h"

/////////////////////////////////////////////////////////////////////////////
// CMySerialComDlg dialog
#include "MyCom.h"


class CMySerialComDlg : public CDialog
{
// Construction
public:
	CMySerialComDlg(CWnd* pParent = NULL);	// standard constructor
	CComStatus  m_ComConfig;
	BOOL		m_open;
// Dialog Data
	//{{AFX_DATA(CMySerialComDlg)
	enum { IDD = IDD_MYSERIALCOM_DIALOG };
	CString	m_com;
	CString	m_receive;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySerialComDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMySerialComDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSet();
	afx_msg void OnSend();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSERIALCOMDLG_H__C5549B80_E38C_4833_8CBB_98488ECABF04__INCLUDED_)
