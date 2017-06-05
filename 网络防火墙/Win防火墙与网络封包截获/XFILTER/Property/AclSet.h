#if !defined(AFX_ACLSET_H__8CBEEEE6_4568_49E6_B9DF_A595B7915A1E__INCLUDED_)
#define AFX_ACLSET_H__8CBEEEE6_4568_49E6_B9DF_A595B7915A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AclSet.h : header file
//

//=============================================================================================
// CAclSet dialog

class CAclSet : public CDialog
{
// Construction
public:
	CAclSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAclSet)
	enum { IDD = IDD_DIALOG_ACL };
	CEdit	m_EditMemo;
	CEdit	m_EditServicePort;
	CComboBox	m_ComboDirection;
	CComboBox	m_ComboAccessTime;
	CComboBox	m_ComboServiceType;
	CComboBox	m_ComboAction;
	CComboBox	m_ComboRemoteNet;
	CComboBox	m_ComboApplication;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAclSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAclSet)
	afx_msg void OnButtonNet();
	afx_msg void OnButtonTime();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApplication();
	virtual void OnOK();
	afx_msg void OnSelchangeComboServiceType();
	afx_msg void OnSelchangeComboApplication();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	void	ShowNetTimeSheet(int nID);
	void	InitDlgResource();

public:
	static  XACL	m_tmpAcl;
	static	BOOL	m_EnableComboApplication;
	static	CString m_sPathName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACLSET_H__8CBEEEE6_4568_49E6_B9DF_A595B7915A1E__INCLUDED_)
