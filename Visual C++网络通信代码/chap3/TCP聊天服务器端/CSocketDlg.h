// CSocketDlg.h : header file
//

#if !defined(AFX_CSOCKETDLG_H__2DFDFAF0_3473_43E6_A5CB_DBB8531B370E__INCLUDED_)
#define AFX_CSOCKETDLG_H__2DFDFAF0_3473_43E6_A5CB_DBB8531B370E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCSocketDlg dialog

class CCSocketDlg : public CDialog
{
// Construction
public:
	CCSocketDlg(CWnd* pParent = NULL);	// standard constructor
	~CCSocketDlg();
// Dialog Data
	//{{AFX_DATA(CCSocketDlg)
	enum { IDD = IDD_CSOCKET_DIALOG };
	CButton	m_button;
	CListCtrl	m_list;
	CEdit	m_edit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCSocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	SOCKET sock,msgsock[50],clisock;
	int addlen;
	sockaddr_in serv;
	int count;
	int getcount();
	void sendtoall(SOCKET,char*);

};
UINT thread(LPVOID);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSOCKETDLG_H__2DFDFAF0_3473_43E6_A5CB_DBB8531B370E__INCLUDED_)
