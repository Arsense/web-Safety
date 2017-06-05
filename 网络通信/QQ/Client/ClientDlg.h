// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__21849DE9_DC51_4433_B20E_71BDA2A6746B__INCLUDED_)
#define AFX_CLIENTDLG_H__21849DE9_DC51_4433_B20E_71BDA2A6746B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	SOCKET  m_ClientSocket;
	SOCKADDR_IN  m_ServerAddr;
	CString  m_strName;
	CString  m_Timer;
	VOID CClientDlg::SendMsg(int iType,CString strName,CString strMsg);
	VOID CClientDlg::OnRecive(WPARAM wParam);
    void CClientDlg::Shaking();
	VOID CClientDlg::GetCurrentTime();
	void CClientDlg::AnalyseString(char* szBuffer);
	void CClientDlg::HideItem();
	CString m_strOtherIP;
	CString m_strOtherName;
	CString m_strOtherFileName;
    DWORD   m_dwOtherFileSize;




	CString m_strFilePath;
	CString m_strFileName;
	DWORD   m_dwFileSize;
	CString m_strFileSize;
	DWORD   m_ByteTotal;
	DWORD   m_Temp;
	CString CClientDlg::KindChange(DWORD dwFileSize);
// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CButton	m_ButtonRecv;
	CButton	m_ButtonCancle;
	CProgressCtrl	m_Progress;
	CComboBox	m_ComBoUser;
	CListBox	m_ListUser;
	CString	m_EditShow;
	CString	m_EditSend;
	BOOL	m_bOnly;
	BOOL	m_bMe;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSocketRead(WPARAM wParam,LPARAM lParam);
	afx_msg void OnButtonTrans();
	afx_msg void OnButtonShake();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonRecv();
	afx_msg void OnDblclkListUser();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__21849DE9_DC51_4433_B20E_71BDA2A6746B__INCLUDED_)
