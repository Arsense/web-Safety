// SerialCommDlg.h : header file
//

#if !defined(AFX_SERIALCOMMDLG_H__2BF15673_84B5_4F80_B799_EB68EA016D52__INCLUDED_)
#define AFX_SERIALCOMMDLG_H__2BF15673_84B5_4F80_B799_EB68EA016D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialCommDlg dialog
#include "SerialCommHelper.h"
class CSerialCommDlg : public CDialog
{
// Construction
	CSerialCommHelper m_theCommPort;
public:
	CSerialCommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialCommDlg)
	enum { IDD = IDD_SERIALCOMM_DIALOG };
	CEdit	m_DataRx;
	CString	m_szSend;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialCommDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialCommDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnWrite();
	afx_msg void OnReadavailable();
	afx_msg void OnRead10();
	afx_msg void OnRead1();
	afx_msg void OnReadUpto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMMDLG_H__2BF15673_84B5_4F80_B799_EB68EA016D52__INCLUDED_)
