/*
	Options.cpp : COptions Dialog implementation file

	Implements the options dialog display.	

	Author: Steven E. Sipe
*/

#include "stdafx.h"
#include "SiteSnag.h"
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


// Constructor
COptionsDlg::COptionsDlg(CSnagOptions *pOptions, CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_FixupHTML = FALSE;
	m_GetMedia = FALSE;
	m_Log = FALSE;
	m_MaxLevels = 1;
	m_MaxPages = 0;
	m_OffsiteLinks = FALSE;
	m_NoLimit = FALSE;
	m_UseProxy = FALSE;
	m_ProxyServer = _T("");
	//}}AFX_DATA_INIT

	m_pOptions = pOptions;
}

// Handles moving data to and from dialogbox fields
void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_SPIN2, m_PageSpin);
	DDX_Control(pDX, IDC_SPIN1, m_LevelSpin);
	DDX_Check(pDX, IDC_FIXUP_HTML, m_FixupHTML);
	DDX_Check(pDX, IDC_GENERATE_TOC, m_GenerateTOC);
	DDX_Check(pDX, IDC_GET_MEDIA, m_GetMedia);
	DDX_Text(pDX, IDC_MAX_LEVELS, m_MaxLevels);
	DDV_MinMaxInt(pDX, m_MaxLevels, 1, MAX_LEVELS);
	DDX_Text(pDX, IDC_MAX_PAGES, m_MaxPages);
	DDV_MinMaxInt(pDX, m_MaxPages, 1, 9999);
	DDX_Check(pDX, IDC_OFFSITE_LINKS, m_OffsiteLinks);
	DDX_Check(pDX, IDC_NO_LIMIT, m_NoLimit);
	DDX_Check(pDX, IDC_USE_PROXY, m_UseProxy);
	DDX_Text(pDX, IDC_PROXY_SERVER, m_ProxyServer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_NO_LIMIT, OnNoLimit)
	ON_BN_CLICKED(IDC_HELPBTN, OnHelpbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

// Sets up the initial data display for the dialog
BOOL COptionsDlg::OnInitDialog() 
{
	// Set max pages, or check No Limit if max pages is 0
	if(m_pOptions->nMaxPages == 0)
	{
		GetDlgItem(IDC_MAX_PAGES)->EnableWindow(FALSE);
		m_NoLimit = TRUE;
		m_MaxPages = 100;
	}
	else m_MaxPages = m_pOptions->nMaxPages;

	// Set other options
	m_MaxLevels = m_pOptions->nMaxDepth;
	m_FixupHTML = m_pOptions->bFixupLinks;
	m_GenerateTOC = m_pOptions->bContents;
	m_GetMedia = m_pOptions->bMultimedia;
	m_OffsiteLinks = m_pOptions->bOffsiteLinks;
	m_UseProxy = m_pOptions->bUseProxy;
	m_ProxyServer = m_pOptions->strProxyName;

	// Initialize the dialog using the values set above
	CDialog::OnInitDialog();

	// Set limits for the spin controls
	m_PageSpin.SetRange(1,9999);
	m_LevelSpin.SetRange(1,MAX_LEVELS);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Handles the OK button
void COptionsDlg::OnOK() 
{
	// Get the latest field values from the dialog
	UpdateData();

	// Save the new values
	m_pOptions->nMaxDepth = m_MaxLevels;
	if(m_NoLimit)
		m_pOptions->nMaxPages = 0;
	else m_pOptions->nMaxPages = m_MaxPages;
	m_pOptions->bFixupLinks = m_FixupHTML;
	m_pOptions->bContents = m_GenerateTOC;
	m_pOptions->bMultimedia = m_GetMedia;
	m_pOptions->bOffsiteLinks = m_OffsiteLinks;
	m_pOptions->bUseProxy = m_UseProxy;
	m_pOptions->strProxyName = m_ProxyServer;

	// Get rid of the dialog
	CDialog::OnOK();
}

// Handles the No Limit checkbox
void COptionsDlg::OnNoLimit() 
{
	UpdateData();
	GetDlgItem(IDC_MAX_PAGES)->EnableWindow(!m_NoLimit);
}

// Options persistence
void CSnagOptions::Serialize(CArchive& ar)
{
	// Storing to disk??
	if (ar.IsStoring())
	{
		ar << bMultimedia;
		ar << nMaxDepth;
		ar << nMaxPages;
		ar << bFixupLinks;
		ar << bContents;
		ar << bOffsiteLinks;
		ar << bUseProxy;
		ar << strProxyName;
	}
	else
	{
		ar >> bMultimedia;
		ar >> nMaxDepth;
		ar >> nMaxPages;
		ar >> bFixupLinks;
		ar >> bContents;
		ar >> bOffsiteLinks;

		// Load or set defaults for new options
		if(m_lFileVersion > 0x100)
		{
			ar >> bUseProxy;
			ar << strProxyName;
		}
		else
		{
			bUseProxy = FALSE;
			strProxyName.Empty();
		}
	}
}

void COptionsDlg::OnHelpbtn() 
{
	AfxGetApp()->WinHelp(1002,HELP_CONTEXT);
}
