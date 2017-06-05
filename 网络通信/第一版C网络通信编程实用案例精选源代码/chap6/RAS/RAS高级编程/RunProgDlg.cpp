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

// RunProgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Afd.h"
#include "RunProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunProgDlg dialog


CRunProgDlg::CRunProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunProgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunProgDlg)
	m_sName = _T("");
	m_iRadio = 0;
	//}}AFX_DATA_INIT
}


void CRunProgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunProgDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunProgDlg, CDialog)
	//{{AFX_MSG_MAP(CRunProgDlg)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunProgDlg message handlers

BOOL CRunProgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    UpdateData(FALSE);
	return TRUE;
}

void CRunProgDlg::OnOK() 
{
	UpdateData();	
	CDialog::OnOK();
}

void CRunProgDlg::OnLoad() 
{
	UpdateData();
    CString sss = m_sName;
    if (sss.IsEmpty()) sss = "*.*";
	CFileDialog dlg(TRUE, NULL, sss,
	OFN_EXPLORER | OFN_OVERWRITEPROMPT, "*.*", NULL);
	int i = dlg.DoModal();
	if (i != IDOK) return;
	m_sName = dlg.GetPathName();
	UpdateData(FALSE);
}
