#if !defined(AFX_SYSTEMSET_H__8B4E5ED0_D8A5_402B_A958_683A7FFC0B16__INCLUDED_)
#define AFX_SYSTEMSET_H__8B4E5ED0_D8A5_402B_A958_683A7FFC0B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemSet.h : header file
//

//=============================================================================================
// CSystemSet dialog

class CSystemSet : public CPropertyPage
{
	DECLARE_DYNCREATE(CSystemSet)

// Construction
public:
	CSystemSet();
	~CSystemSet();

// Dialog Data
	//{{AFX_DATA(CSystemSet)
	enum { IDD = IDD_SYSTEM_SET };
	CButton	m_CheckSplash;
	CComboBox	m_ListLogSize;
	CButton	m_CheckLog;
	CButton	m_CheckAlertSpeaker;
	CButton	m_CheckAutoStart;
	CButton	m_CheckAlertDialog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSystemSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSystemSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSystemSetCheckLog();
	afx_msg void OnSystemSetCheckAutostart();
	afx_msg void OnSystemSetCheckAlertPcspeaker();
	afx_msg void OnSystemSetCheckAlertDialog();
	afx_msg void OnSelchangeSystemSetListLogSize();
	afx_msg void OnSystemSetCheckSplash();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner add

public:
	void InitDlgResource();
	void Refresh();
	void SetCheckValue(int iIndex, BOOL bValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMSET_H__8B4E5ED0_D8A5_402B_A958_683A7FFC0B16__INCLUDED_)
