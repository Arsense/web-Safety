#if !defined(AFX_REGISTER_H__27EA90DB_7B57_430B_83E2_03DA6D66B3E3__INCLUDED_)
#define AFX_REGISTER_H__27EA90DB_7B57_430B_83E2_03DA6D66B3E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Register.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegister dialog

#include ".\HyperLink\HyperLink.h"	
#include "about.h"

class CRegister : public CDialog
{
// Construction
public:
	CRegister(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegister)
	enum { IDD = IDD_DIALOG_REGISTER };
	CColorStatic	m_LableInfo1;
	CColorStatic	m_LableInfo3;
	CEdit	m_EditQq;
	CEdit	m_EditInc;
	CDateTimeCtrl	m_TimeBirthday;
	CEdit	m_EditZip;
	CEdit	m_EditName;
	CEdit	m_EditEmail;
	CEdit	m_EditAddress;
	CEdit	m_EditDuty;
	CComboBox	m_ComboGender;
	CComboBox	m_ComboSalary;
	CComboBox	m_ComboMetier;
	CComboBox	m_ComboDegree;
	CHyperLink	m_WebStation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	void		InitDlgResource();
	void		InitCombo(CComboBox *combo, TCHAR *string[], int count, int iSelected);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTER_H__27EA90DB_7B57_430B_83E2_03DA6D66B3E3__INCLUDED_)
