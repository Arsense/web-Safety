#if !defined(AFX_LOGQUERY_H__03AB82D5_05D2_4AA0_8EBE_B7CA2DEA7FA1__INCLUDED_)
#define AFX_LOGQUERY_H__03AB82D5_05D2_4AA0_8EBE_B7CA2DEA7FA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogQuery.h : header file
//

//=============================================================================================
// CLogQuery dialog

class CLogQuery : public CPropertyPage
{
	DECLARE_DYNCREATE(CLogQuery)

// Construction
public:
	CLogQuery();
	~CLogQuery();

// Dialog Data
	//{{AFX_DATA(CLogQuery)
	enum { IDD = IDD_LOG_QUERY };
	CDateTimeCtrl	m_EndTime;
	CDateTimeCtrl	m_EndDate;
	CDateTimeCtrl	m_StartTime;
	CDateTimeCtrl	m_StartDate;
	CListCtrl	m_ListLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLogQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLogQuery)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogQueryButtonQuery();
	afx_msg void OnLogQueryButtonDelete();
	afx_msg void OnLogQueryButtonBack();
	afx_msg void OnLogQueryButtonNext();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
// owner add

public:
	void	InitDlgResource();
	CTime	GetTime(BOOL IsStart = TRUE);
	int		ShowRecordPage();
	void	SetButton(BOOL IsBack = FALSE, BOOL IsNext = FALSE);
	void	SetPageLable(DWORD ulRecordCount = 0);

public:
	void	*m_pLogFile;
	int		m_iPageCount;
	int		m_iCurrentPage;
	DWORD	m_ulStartPosition;
	DWORD	m_ulRecordCount;
	DWORD	m_ulChangeDirectionIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGQUERY_H__03AB82D5_05D2_4AA0_8EBE_B7CA2DEA7FA1__INCLUDED_)
