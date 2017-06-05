#if !defined(AFX_SETTIME_H__F665A087_09C3_42F0_9222_955BA6545AA6__INCLUDED_)
#define AFX_SETTIME_H__F665A087_09C3_42F0_9222_955BA6545AA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTime.h : header file
//

//=============================================================================================
// CSetTime dialog

class CSetTime : public CPropertyPage
{
	DECLARE_DYNCREATE(CSetTime)

// Construction
public:
	CSetTime();
	~CSetTime();

// Dialog Data
	//{{AFX_DATA(CSetTime)
	enum { IDD = IDD_SET_TIME };
	CButton	m_Check0;
	CButton	m_Check1;
	CButton	m_Check2;
	CButton	m_Check3;
	CButton	m_Check4;
	CButton	m_Check5;
	CButton	m_Check6;
	CDateTimeCtrl	m_TimeStart;
	CDateTimeCtrl	m_TimeEnd;
	CTreeCtrl	m_TreeSetTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetTime)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedSetTimeTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetTimeCheckSunday();
	afx_msg void OnSetTimeCheckMonday();
	afx_msg void OnSetTimeCheckTuesday();
	afx_msg void OnSetTimeCheckThursday();
	afx_msg void OnSetTimeCheckSaturday();
	afx_msg void OnSetTimeCheckFriday();
	afx_msg void OnDatetimechangeSetTimeTimeEnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeSetTimeTimeStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetTimeCheckWednesday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	void		InitDlgResource	();
	int			TextToIndex		(CString sText);
	int			EnableButton	(BOOL bEnable);
	int			SelectTreeItemOperate();
	int			SetValue		(void* acltime);
	int			SetAclValue		(int iIndex,BOOL isTrue);
	int			SetTimeValue	(int iType);
	int			SelectTreeItem	(CString sText);

public:
	int			m_iTreeIndex;
	CString		m_TreeText;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTIME_H__F665A087_09C3_42F0_9222_955BA6545AA6__INCLUDED_)
