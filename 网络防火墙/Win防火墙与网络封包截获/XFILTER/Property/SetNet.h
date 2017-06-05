#if !defined(AFX_SETNET_H__DA9BA92A_FC25_4867_A3DF_A3A2B0BDC50A__INCLUDED_)
#define AFX_SETNET_H__DA9BA92A_FC25_4867_A3DF_A3A2B0BDC50A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetNet.h : header file
//

//=============================================================================================
// CSetNet dialog

class CSetNet : public CPropertyPage
{
	DECLARE_DYNCREATE(CSetNet)

// Construction
public:
	CSetNet();
	~CSetNet();

// Dialog Data
	//{{AFX_DATA(CSetNet)
	enum { IDD = IDD_SET_NET };
	CTreeCtrl	m_TreeSetNet;
	CListCtrl	m_ListSetNet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetNet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetNetButtonAdd();
	afx_msg void OnSelchangedSetNetTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedSetNetList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetNetButtonEdit();
	afx_msg void OnDblclkSetNetList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetNetButtonDelete();
	afx_msg void OnClickSetNetList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	static void InitList		(CListCtrl * mList);
	void		InitDlgResource	();
	int			NetTextToIndex	(CString sNetText);
	int			SelectTreeItem	(CString sNetText);
	int			SelectTreeItemOperate();
	int			EnableButton	(BOOL bEnable,BOOL bEnableEx = FALSE);
	int			ListAddIP		(void* mpAclIP,DWORD mCount);
	int			ListAddOneIP	(DWORD iIndex, void* mpAclIP, BOOL isSelected = FALSE);

public:
	int			iTreeIndex;
	int			iListIndex;
	CString		m_TreeText;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETNET_H__DA9BA92A_FC25_4867_A3DF_A3A2B0BDC50A__INCLUDED_)
