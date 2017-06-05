/*
	Urldlg.cpp : implementation of the CUrlDlg class

	Allows the user to specify the URL of the site to snag.	

	Author: Steven E. Sipe
*/

#include "stdafx.h"
#include "sitesnag.h"
#include "URLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CURLDlg dialog


// Constructor
CURLDlg::CURLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CURLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CURLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nMaxPages = 0;
	m_nMaxLevels = 0;
	m_bGetMultimedia = FALSE;
}

// Handles field binding for MFC
void CURLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CURLDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CURLDlg, CDialog)
	//{{AFX_MSG_MAP(CURLDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLDlg message handlers

// Handles the OK button
void CURLDlg::OnOK() 
{
	// Get the specified URL
	GetDlgItem(IDC_EDIT)->GetWindowText(m_strURL);

	CDialog::OnOK();
}

// Handles initialization -- fills in the information fields displayed on the
// snag screen
BOOL CURLDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		AfxMessageBox("ÏÈ½øµÄOnInitDialog");
	// Set the current URL
	SetDlgItemText(IDC_EDIT,m_strURL);

	// Set the info fields also -- max levels and so forth
	SetDlgItemInt(IDC_MAX_LEVELS,m_nMaxLevels,FALSE);
	if(m_nMaxPages)
		SetDlgItemInt(IDC_MAX_PAGES,m_nMaxPages,FALSE);
	else SetDlgItemText(IDC_MAX_PAGES,"No limit");

	SetDlgItemText(IDC_GET_MEDIA,m_bGetMultimedia?"Yes":"No");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Handles the help button
