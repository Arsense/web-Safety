#if !defined(AFX_ABOUT_H__EA34CD41_8FD9_4995_9FB1_723F420439CA__INCLUDED_)
#define AFX_ABOUT_H__EA34CD41_8FD9_4995_9FB1_723F420439CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// About.h : header file
//
#include ".\HyperLink\HyperLink.h"	

class CColorStatic : public CStatic
{
public:
	COLORREF m_nColor;
	CColorStatic(COLORREF nColor = RGB(255, 0, 0));	
	void SetColor(COLORREF nColor = RGB(255, 0, 0));	
protected:
    //{{AFX_MSG(CHyperLink)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//=============================================================================================
// CAbout dialog

class CAbout : public CPropertyPage
{
	DECLARE_DYNCREATE(CAbout)

// Construction
public:
	CAbout();
	~CAbout();

// Dialog Data
	//{{AFX_DATA(CAbout)
	enum { IDD = IDD_ABOUT };
	CColorStatic	m_StaticInfo1;
	CHyperLink		m_LinkEmail;
	CHyperLink		m_LinkUrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAbout)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	void		InitDlgResource();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUT_H__EA34CD41_8FD9_4995_9FB1_723F420439CA__INCLUDED_)
