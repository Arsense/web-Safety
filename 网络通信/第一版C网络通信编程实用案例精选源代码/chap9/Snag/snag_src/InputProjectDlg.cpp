#include "stdafx.h"

#include <ctype.h>

#include "SiteDownload.h"
#include "InputProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputProjectDlg dialog


// Constructor
CInputProjectDlg::CInputProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputProjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputProjectDlg)
	//}}AFX_DATA_INIT
}

// Data field binding for MFC
void CInputProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputProjectDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInputProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CInputProjectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputProjectDlg message handlers

// Handles OK button processing
void CInputProjectDlg::OnOK() 
{
	m_ProjectEdit.GetWindowText(m_strProjectName);

	CDialog::OnOK();
}

// Initialization -- handles the WM_INITDIALOG message
BOOL CInputProjectDlg::OnInitDialog() 
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
// CProjectNameEdit -- Subclass for the project name field

// Constructor
CProjectNameEdit::CProjectNameEdit()
{
}

// Destructor
CProjectNameEdit::~CProjectNameEdit()
{
}

BEGIN_MESSAGE_MAP(CProjectNameEdit, CEdit)
	//{{AFX_MSG_MAP(CProjectNameEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectNameEdit message handlers

// Filter out all of the characters but alpha-numeric, space and backspace
void CProjectNameEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(isalnum(nChar) || isspace(nChar) || nChar == '\b')
		CEdit::OnChar(nChar,nRepCnt,nFlags);
	else ::MessageBeep(0);
}
