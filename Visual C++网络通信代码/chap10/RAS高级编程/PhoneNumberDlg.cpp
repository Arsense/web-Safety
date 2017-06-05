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

// PhoneNumberDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Afd.h"
#include "PhoneNumberDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumberDlg dialog


CPhoneNumberDlg::CPhoneNumberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhoneNumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneNumberDlg)
	m_sNumber = _T("");
	m_sComments = _T("");
	//}}AFX_DATA_INIT
}


void CPhoneNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneNumberDlg)
	DDX_Text(pDX, IDC_EDIT1, m_sNumber);
	DDX_Text(pDX, IDC_EDIT2, m_sComments);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhoneNumberDlg, CDialog)
	//{{AFX_MSG_MAP(CPhoneNumberDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumberDlg message handlers

BOOL CPhoneNumberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;
}

void CPhoneNumberDlg::OnOK() 
{
	UpdateData();
	CDialog::OnOK();
}
