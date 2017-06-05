// About.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "About.h"

//owner add
#include "GuiRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CAbout property page

extern CAclFile		m_AclFile;
extern CPropertyApp theApp;

IMPLEMENT_DYNCREATE(CAbout, CPropertyPage)

CAbout::CAbout() : CPropertyPage(CAbout::IDD)
{
	//{{AFX_DATA_INIT(CAbout)
	//}}AFX_DATA_INIT
}

CAbout::~CAbout()
{
}

void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	DDX_Control(pDX, IDC_ABOUT_LABLE_INFO, m_StaticInfo1);
	DDX_Control(pDX, IDC_ABOUT_LABLE_EMAIL, m_LinkEmail);
	DDX_Control(pDX, IDC_ABOUT_LABLE_WEB_ADDRESS, m_LinkUrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CPropertyPage)
	//{{AFX_MSG_MAP(CAbout)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CAbout message handlers

BOOL CAbout::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitDlgResource();

	return TRUE;  
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CAbout::InitDlgResource()
{
	SetWindowText(GUI_TAB_ABOUT);

	CString		s;

	s			.Format(_T("mailto:%s"), m_AclFile.mAclHeader.sEmail);
	m_LinkEmail	.SetURL(s);
	m_LinkUrl	.SetURL(m_AclFile.mAclHeader.sWebURL);

	s.Format(GUI_ABOUT_LABLE_ACCREDIT_TO, 
		theApp.m_RegisterRequest.m_UserInfo.sName[0] == '\0' ?
		theApp.m_RegisterRequest.m_UserInfo.sEmail : theApp.m_RegisterRequest.m_UserInfo.sName
		);

	SetDlgItemText(IDC_ABOUT_LABLE_ACCREDIT_TO	, s);
	
	s.Format(GUI_ABOUT_LABLE_COPYRIGHT1, 
		ACL_HEADER_VERSION, 
		ACL_HEADER_MAJOR,
		ACL_HEADER_MINOR
		);

	SetDlgItemText(IDC_ABOUT_LABLE_TITLE		, GUI_ABOUT_LABLE_TITLE);
	SetDlgItemText(IDC_ABOUT_LABLE_COPYRIGHT1	, s);
	SetDlgItemText(IDC_ABOUT_LABLE_COPYRIGHT2	, GUI_ABOUT_LABLE_COPYRIGHT2);
	SetDlgItemText(IDC_ABOUT_LABLE_COPYRIGHT3	, GUI_ABOUT_LABLE_COPYRIGHT3);
	SetDlgItemText(IDC_ABOUT_LABLE_WEB_ADDRESS_LABLE, GUI_ABOUT_LABLE_WEB_ADDRESS_LABLE);
	SetDlgItemText(IDC_ABOUT_LABLE_EMAIL_LABLE		, GUI_ABOUT_LABLE_EMAIL_LABLE);
	SetDlgItemText(IDC_ABOUT_LABLE_WEB_ADDRESS	, m_AclFile.mAclHeader.sWebURL);
	SetDlgItemText(IDC_ABOUT_LABLE_EMAIL		, m_AclFile.mAclHeader.sEmail);
	SetDlgItemText(IDC_ABOUT_LABLE_WARNING		, GUI_ABOUT_LABLE_WARNING);
	SetDlgItemText(IDC_ABOUT_LABLE_INFO			, GUI_ABOUT_LABLE_INFO);
	SetDlgItemText(IDC_ABOUT_LABLE_AUTHOR		, GUI_ABOUT_LABLE_AUTHOR);
	SetDlgItemText(IDC_ABOUT_LABLE_COPYRIGHT	, GUI_ABOUT_LABLE_COPYRIGHT);
}

BOOL CAbout::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.WinHelp(8, HH_HELP_CONTEXT);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
    //{{AFX_MSG_MAP(CHyperLink)
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CColorStatic::CColorStatic(COLORREF nColor)
{
	m_nColor = nColor;
}

void CColorStatic::SetColor(COLORREF nColor)
{
	m_nColor = nColor;
}

HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    pDC->SetTextColor(m_nColor);

    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}
