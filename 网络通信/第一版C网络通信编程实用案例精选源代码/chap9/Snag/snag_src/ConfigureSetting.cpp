#include "stdafx.h"
#include "SiteDownload.h"
#include "ConfigureSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog


// Constructor
CConfigureDlg::CConfigureDlg(CConfigure *pOptions, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureDlg)
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

void CConfigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureDlg)
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

BEGIN_MESSAGE_MAP(CConfigureDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigureDlg)
	ON_BN_CLICKED(IDC_NO_LIMIT, OnNoLimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg message handlers

BOOL CConfigureDlg::OnInitDialog() 
{
	if(m_pOptions->nMaxPages == 0)
	{
		GetDlgItem(IDC_MAX_PAGES)->EnableWindow(FALSE);
		m_NoLimit = TRUE;
		m_MaxPages = 100;
	}
	else m_MaxPages = m_pOptions->nMaxPages;

	m_MaxLevels = m_pOptions->nMaxDepth;
	m_FixupHTML = m_pOptions->bFixupLinks;
	m_GenerateTOC = m_pOptions->bContents;
	m_GetMedia = m_pOptions->bMultimedia;
	m_OffsiteLinks = m_pOptions->bOffsiteLinks;
	m_UseProxy = m_pOptions->bUseProxy;
	m_ProxyServer = m_pOptions->strProxyName;

	CDialog::OnInitDialog();

	
	return TRUE;  
}

void CConfigureDlg::OnOK() 
{
	UpdateData();

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

	CDialog::OnOK();
}

void CConfigureDlg::OnNoLimit() 
{
	UpdateData();
	GetDlgItem(IDC_MAX_PAGES)->EnableWindow(!m_NoLimit);
}

//将数据串行化
void CConfigure::Serialize(CArchive& ar)
{
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
		bUseProxy = FALSE;
		strProxyName.Empty();
		ar >> bMultimedia;
		ar >> nMaxDepth;
		ar >> nMaxPages;
		ar >> bFixupLinks;
		ar >> bContents;
		ar >> bOffsiteLinks;
	}
}

