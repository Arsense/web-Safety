// Example2_ChatRoomDlg.h : header file
//

#if !defined(AFX_EXAMPLE2_CHATROOMDLG_H__A2D60D19_2558_4BA4_B3F3_642E537939EB__INCLUDED_)
#define AFX_EXAMPLE2_CHATROOMDLG_H__A2D60D19_2558_4BA4_B3F3_642E537939EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExample2_ChatRoomDlg dialog
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Sound.h"
#include <afxmt.h>
class CExample2_ChatRoomDlg : public CDialog
{
// Construction
public:
	CExample2_ChatRoomDlg(CWnd* pParent = NULL);	// standard constructor
	~CExample2_ChatRoomDlg();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExample2_ChatRoomDlg)
	enum { IDD = IDD_EXAMPLE2_CHATROOM_DIALOG };
	CString	m_sInputString;
	CString	m_sShowString;
	UINT	m_uPort;
	//}}AFX_DATA

public:
	void ProcessPendingAccept();
	void CloseSessionSocket();
	void ClearContent();
	void OnClearconnection();

public:
	bool m_bInit;
	bool m_bClient;
	CClientSocket m_clientsocket;
	CServerSocket m_pListenSocket;
	CPtrList m_connectionList;
	CString m_sMsgList;
	
	CSound  m_sound;
	CMutex  m_mutex;
	BOOL    m_bIsFileClosed;
	BOOL    m_willchating;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExample2_ChatRoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExample2_ChatRoomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInputText();
	afx_msg void OnConnectserver();
	afx_msg void OnSetserver();
	afx_msg void OnSound();
	afx_msg void OnNewsend();
	afx_msg void WriteBufferFull(LPARAM lp,WPARAM wp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLE2_CHATROOMDLG_H__A2D60D19_2558_4BA4_B3F3_642E537939EB__INCLUDED_)
