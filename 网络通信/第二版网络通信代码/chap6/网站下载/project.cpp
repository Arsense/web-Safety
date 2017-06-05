/*
	Project.cpp : implementation of the CProjectDlg class

	Implements the project creation dialog.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include <ctype.h>

#include "SiteSnag.h"
#include "Project.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDlg dialog


// Constructor
CProjectDlg::CProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectDlg)
	//}}AFX_DATA_INIT
}

// Data field binding for MFC
void CProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CProjectDlg)
	ON_BN_CLICKED(IDC_HELPBTN, OnHelpbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDlg message handlers

// Handles OK button processing
void CProjectDlg::OnOK() 
{
	m_ProjectEdit.GetWindowText(m_strProjectName);

	CDialog::OnOK();
}

// Initialization -- handles the WM_INITDIALOG message
BOOL CProjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Subclass the project name so we can validate it
	m_ProjectEdit.SubclassDlgItem(IDC_PROJECT_NAME,this);

	// Set the title of the dialog -- this allows us to use the same dialog
	// to rename a project as well.
	if(!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CProjectEdit -- Subclass for the project name field

// Constructor
CProjectEdit::CProjectEdit()
{
}

// Destructor
CProjectEdit::~CProjectEdit()
{
}

BEGIN_MESSAGE_MAP(CProjectEdit, CEdit)
	//{{AFX_MSG_MAP(CProjectEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectEdit message handlers

// Filter out all of the characters but alpha-numeric, space and backspace
void CProjectEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(isalnum(nChar) || isspace(nChar) || nChar == '\b')
		CEdit::OnChar(nChar,nRepCnt,nFlags);
	else ::MessageBeep(0);
}

// Call online help
void CProjectDlg::OnHelpbtn() 
{
	if(m_strTitle.IsEmpty())
		AfxGetApp()->WinHelp(1001,HELP_CONTEXT);
	else AfxGetApp()->WinHelp(1005,HELP_CONTEXT);
}
