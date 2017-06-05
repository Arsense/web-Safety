// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__44BB73C4_A341_483A_B9FC_CBE166C354E6__INCLUDED_)
#define AFX_SERVERDLG_H__44BB73C4_A341_483A_B9FC_CBE166C354E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <WINSOCK2.H>

#pragma comment(lib,"ws2_32.lib")
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

#define MAX 20

typedef struct _PEOPLEINFOR_ 
{
	CString  strName;
	CString  strIp;
}PEOPLEINFOR;


typedef struct _SENDINFOR_
{
	int  iType;       //0
	char szName[20];  //123
	char szMsg[400];  //°∞°± 
}SENDINFOR;

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
    VOID InitNotify();
	VOID CServerDlg::InitSocket();
	VOID CServerDlg::GetHostInfor();
	VOID CServerDlg::GetCurrentTime();
// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CStatic	m_strPeople;
	CStatic	m_strHost;
	CString	m_strShow;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    NOTIFYICONDATAA m_nid;    //œµÕ≥Õ–≈Ã
	SOCKET   m_sListen; 
	SOCKET   m_sClient[MAX];
	PEOPLEINFOR  m_PeopleInfor[MAX];
	sockaddr_in  m_ServerAddr;
	CString  m_Timer; 
	ULONG    m_People;

	
    VOID CServerDlg::OnlineOrOutline(int iIndex,CString YesOrNo);
	VOID CServerDlg::OnRecive(WPARAM wParam);
	VOID CServerDlg::OnAccept();
	VOID CServerDlg::OnClose(WPARAM wParam);
	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam); 
	afx_msg LRESULT OnServerAccept(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnDlgExit();
	afx_msg void OnDlgShow();
	afx_msg void OnDlgHide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};






//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__44BB73C4_A341_483A_B9FC_CBE166C354E6__INCLUDED_)
