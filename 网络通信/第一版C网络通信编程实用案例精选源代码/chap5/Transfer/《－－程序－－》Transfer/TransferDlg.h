// TransferDlg.h : header file
//

#if !defined(AFX_TRANSFERDLG_H__E9C1F88B_5F4B_4D1F_83D4_56FCE16B90AA__INCLUDED_)
#define AFX_TRANSFERDLG_H__E9C1F88B_5F4B_4D1F_83D4_56FCE16B90AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog
#include "ServerSocket.h"
#include "TransferSocket.h"


class CTransferDlg : public CDialog
{
// Construction
public:
	CTransferDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTransferDlg)
	enum { IDD = IDD_TRANSFER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void HandleClose();
	void HandleAccept();
	CStringArray m_LocalShareFiles;
	CStringArray m_RemoteShareFiles;
	CStringArray m_NeedTransferFiles;
	CStringArray m_NeedReceiveFiles;
	BOOL m_bClient;
	BOOL m_bOnLine;
	int  m_iPort;
	CString m_sIP;
	CServerSocket m_ServerSocket;
	CTransferSocket m_TransferSocket;
//	CList<CTransferSocket *,CTransferSocket *> m_TransferSockets;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTransferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnAddsharefile();
	afx_msg void OnDelsharefile();
	afx_msg void OnDownloadsharefile();
	afx_msg void OnSetnetwork();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERDLG_H__E9C1F88B_5F4B_4D1F_83D4_56FCE16B90AA__INCLUDED_)
