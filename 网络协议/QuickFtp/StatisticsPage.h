#if !defined(AFX_STATISTICSPAGE_H__EC2C52BB_26E9_4046_B6F5_B88A3D3CABA7__INCLUDED_)
#define AFX_STATISTICSPAGE_H__EC2C52BB_26E9_4046_B6F5_B88A3D3CABA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsPage dialog
#include "resource.h"

class CStatisticsPage : public CDialog
{
// Construction
public:
	void SetValue(int nType,LPCTSTR lpszValue);
	void SetValue(int nType,int nValue);
	CStatisticsPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatisticsPage)
	enum { IDD = IDD_PROPPAGE_STATISTICS };
	CListCtrl	m_StatisticsCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatisticsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSPAGE_H__EC2C52BB_26E9_4046_B6F5_B88A3D3CABA7__INCLUDED_)
