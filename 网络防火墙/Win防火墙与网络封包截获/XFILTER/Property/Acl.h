#if !defined(AFX_ACL_H__DFE5E4FE_2B05_4F8C_B2AB_070981556624__INCLUDED_)
#define AFX_ACL_H__DFE5E4FE_2B05_4F8C_B2AB_070981556624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Acl.h : header file
//

//=============================================================================================
// CAcl dialog

class CAcl : public CPropertyPage
{
	DECLARE_DYNCREATE(CAcl)

// Construction
public:
	CAcl();
	~CAcl();

// Dialog Data
	//{{AFX_DATA(CAcl)
	enum { IDD = IDD_ACL };
	static CListCtrl	m_ListAcl;
	CButton		m_RadioPass;
	CButton		m_RadioQuery;
	CButton		m_RadioDeny;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAcl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAcl)
	virtual BOOL OnInitDialog();
	afx_msg void OnAclButtonAdd();
	afx_msg void OnAclButtonEdit();
	afx_msg void OnItemchangedAclList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAclList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickAclList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAclButtonDelete();
	afx_msg void OnAclRadioDeny();
	afx_msg void OnAclRadioPass();
	afx_msg void OnAclRadioQuery();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	static int	ListAddOne(
		IN	void* mpAcl,
		IN	BOOL isSelected = FALSE,
		IN	BOOL isEdit		= FALSE,
		IN	int EditIndex	= 0
		);

	void	InitDlgResource	();
	void	Refresh			();
	BOOL	SetType			(int type, BOOL bit4, BOOL bit5);
	void	EnableButton	(BOOL bEnable);

public:
	static void InitList	(CListCtrl * mList);
	static int	ShowDlg		();
	static UINT	EnableMenu	(UINT bEnable);
	static int	InitAddAcl	(TCHAR* sPathName, BOOL EnableComboApplication = TRUE);
	static BOOL WINAPI AddAclQuery	(TCHAR* sPathName, BOOL EnableComboApplication = FALSE);

public:
	int		m_iListIndex;
	int		m_iType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACL_H__DFE5E4FE_2B05_4F8C_B2AB_070981556624__INCLUDED_)
