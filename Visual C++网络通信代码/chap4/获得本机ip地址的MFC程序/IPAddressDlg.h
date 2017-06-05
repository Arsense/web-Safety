// IPAddressDlg.h : header file
//

#if !defined(AFX_IPADDRESSDLG_H__859FA75B_1659_11D2_9153_0060080C48E1__INCLUDED_)
#define AFX_IPADDRESSDLG_H__859FA75B_1659_11D2_9153_0060080C48E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CIPAddressDlg dialog

class CIPAddressDlg : public CDialog
{
// Construction
public:
	CIPAddressDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIPAddressDlg)
	enum { IDD = IDD_IPADDRESS_DIALOG };
	CString	m_sHostName;
	CString	m_sIPAddress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPAddressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIPAddressDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int StartUp();
	int CleanUp();
	int GetLocalHostName(CString& sHostName);
	int GetIPAddress(const CString& sHostName, CString& sIPAddress);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPADDRESSDLG_H__859FA75B_1659_11D2_9153_0060080C48E1__INCLUDED_)
