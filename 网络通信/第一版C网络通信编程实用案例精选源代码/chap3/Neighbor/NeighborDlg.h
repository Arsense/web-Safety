// NeighborDlg.h : header file
//

#if !defined(AFX_NEIGHBORDLG_H__FBCD0283_82E9_4BDE_85A0_527A8A75BB84__INCLUDED_)
#define AFX_NEIGHBORDLG_H__FBCD0283_82E9_4BDE_85A0_527A8A75BB84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNeighborDlg dialog
class CNeighborDlg : public CDialog
{

public:
// Construction
	BOOL Enumerate(LPNETRESOURCE lpNetRC_p,int depth);
	BOOL Enumerate2(LPNETRESOURCE lpNetRC_p,int depth);

	void	Redirect(const char *LocalName,const char *RemoteName,const char * UserName,const char *Password);
	void	DisConnectDirect(const char * localname);

	int GetLocalHostName(CString &sHostName);
	int GetIpAddress(const CString &sHostName, CStringArray &sIpAddress);
	int GetIpAddress(const CString &sHostName, BYTE f0[],BYTE f1[],BYTE f2[],BYTE f3[],int &count);

	CStringList	m_List;
	CStringList m_NeighborList;

public:
	CNeighborDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNeighborDlg)
	enum { IDD = IDD_NEIGHBOR_DIALOG };
	CString	m_NetListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeighborDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNeighborDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnListbutton();
	afx_msg void OnIpbutton();
	afx_msg void OnNavigator();
	afx_msg void OnRedirect();
	afx_msg void OnDisconnectdirect();
	afx_msg void OnIptohostname();
	afx_msg void OnNetsend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEIGHBORDLG_H__FBCD0283_82E9_4BDE_85A0_527A8A75BB84__INCLUDED_)
