// MyTapiDlg.h : header file
//

#if !defined(AFX_MYTAPIDLG_H__ED883B99_C418_44A4_A12C_9A84A0BA98FF__INCLUDED_)
#define AFX_MYTAPIDLG_H__ED883B99_C418_44A4_A12C_9A84A0BA98FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyTapiDlg dialog
#include "Tapi.h"

class CMyTapiDlg : public CDialog
{
// Construction
public:
	void ShutDown();
	void Drop();
	HANDLE m_hModem;
	void GetID();
	BOOL m_bhCallValid;
	LPHCALL m_hCall;
	LONG m_lRequestID;
	void Call(CString number);
	LPHLINE m_hLine;
	void OpenLine();
	LPLINEDEVCAPS GetDevCaps(DWORD device,DWORD tapiversion);
	DWORD m_dwLineToUse;
	void NegotiateTAPIVersion();
	DWORD m_dwTapiVersion;
	void InitLineEx();
	static void CALLBACK TapiCallbackFunc(DWORD device,DWORD msg,DWORD callbackinstance ,DWORD param1, DWORD param2,DWORD param3);
	DWORD m_dwNumLines;
	LPHLINEAPP m_hTAPI;
	void InitLine();
	BOOL m_bError;
	CMyTapiDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CMyTapiDlg)
	enum { IDD = IDD_MYTAPI_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTapiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyTapiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAssitPhone();
	afx_msg void OnBasicPhone();
	afx_msg void OnHangUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTAPIDLG_H__ED883B99_C418_44A4_A12C_9A84A0BA98FF__INCLUDED_)
