///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONENUMBERDLG_H__FE853825_F027_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_PHONENUMBERDLG_H__FE853825_F027_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneNumberDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumberDlg dialog

class CPhoneNumberDlg : public CDialog
{
// Construction
public:
	CPhoneNumberDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhoneNumberDlg)
	enum { IDD = IDD_PHONE_NUMBER };
	CString	m_sNumber;
	CString	m_sComments;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhoneNumberDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONENUMBERDLG_H__FE853825_F027_11D2_9864_D86B908C7AFE__INCLUDED_)