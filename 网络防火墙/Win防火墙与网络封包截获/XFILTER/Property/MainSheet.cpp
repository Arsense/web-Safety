// MainSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "MainSheet.h"

//owner add
#include "GuiRes.h"
#include "acl.h"
#include "systemset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CMainSheet

extern CAclFile		m_AclFile;
extern CPropertyApp theApp;

IMPLEMENT_DYNAMIC(CMainSheet, CPropertySheet)

CMainSheet::CMainSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMainSheet::CMainSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_PacketMonitor);
	AddPage(&m_LogQuery);
	AddPage(&m_Acl);
	AddPage(&m_SystemSet);
	AddPage(&m_About);
	SetLogoText(GUI_APP_LOGO);
}

CMainSheet::~CMainSheet()
{
}


BEGIN_MESSAGE_MAP(CMainSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMainSheet)
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CMainSheet message handlers

BOOL CMainSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetLogoFont("Arial");
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	InitDlgResource();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW); 

	return bResult;
}

void CMainSheet::OnPaint() 
{
	CPaintDC dc(this);

	if(m_LogoText.IsEmpty())
		return;

	CRect		rectTabCtrl;
	CRect		rectOk;
	CRect		rectText;

	GetTabControl()	->GetWindowRect(rectTabCtrl);
	ScreenToClient(rectTabCtrl);
	GetDlgItem(IDOK)->GetWindowRect(rectOk);
	ScreenToClient(rectOk);
	dc.SetBkMode(TRANSPARENT);

	rectText.left		= rectTabCtrl.left;
	rectText.top		= rectOk.top;
	rectText.bottom		= rectOk.bottom;
	rectText.right		= rectOk.left;
	CFont*		OldFont = dc.SelectObject(&m_FontLogo);
	COLORREF	OldColor= dc.SetTextColor(GetSysColor(COLOR_3DHILIGHT));

	dc.DrawText		(m_LogoText, rectText + CPoint(1, 1), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	dc.SetTextColor	(GetSysColor(COLOR_3DSHADOW));
	dc.DrawText		(m_LogoText, rectText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	dc.SetTextColor	(OldColor);
	dc.SelectObject	(OldFont);
}

BOOL CMainSheet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	switch(nID)
	{
	case IDOK:
		if(OnApply())	
			return TRUE;

		ShowWindow(SW_HIDE);
		return TRUE;

	case IDCANCEL:
		OnCancel();
		ShowWindow(SW_HIDE);
		return TRUE;

	case ID_APPLY_NOW:
		if(OnApply())	
			return TRUE;
		break;

	case IDHELP:
		break;
	}

	return CPropertySheet::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainSheet::OnApply()
{
	if(!GetDlgItem(ID_APPLY_NOW)->IsWindowEnabled())	//exit and not do with system
		return FALSE;								

	if(m_AclFile.SaveAcl() != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_SAVE_ACL_ERROR);
		return TRUE;
	}

	theApp.SetAutoStart();

	if(!theApp.InitDllData())
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR);
		return TRUE;
	}

	GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);
	return FALSE;
}

void CMainSheet::OnCancel()
{
	if(!GetDlgItem(ID_APPLY_NOW)->IsWindowEnabled())
		return;

	if(AfxMessageBox(GUI_ACL_MESSAGE_ACL_ASK_SAVE, MB_YESNO) == IDYES)
	{
		OnApply();
		return;
	}

	GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);

	if(m_AclFile.ReadAcl() != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ACL_READ_ERROR);
		return;
	}

	CAcl *dlgAcl = (CAcl*)this->GetPage(2);
	dlgAcl->Refresh();

	CSystemSet *dlgSystemSet = (CSystemSet*)this->GetPage(3);
	dlgSystemSet->Refresh();
}

void CMainSheet::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if(nID == SC_CLOSE)
	{
		OnCancel();
		ShowWindow(SW_HIDE);
		return;
	}

	CPropertySheet::OnSysCommand(nID, lParam);
}

//---------------------------------------------------------------------------------------------
//set logo text

void CMainSheet::SetLogoText(CString Text)
{
	m_LogoText	= Text;
}

//---------------------------------------------------------------------------------------------
//set logo font

void CMainSheet::SetLogoFont(
	CString			Name,
	int				nHeight,
	int				nWeight,
	BYTE			bItalic,
	BYTE			bUnderline
)
{
	if(m_FontLogo.m_hObject)
		m_FontLogo.Detach();

	m_FontLogo.CreateFont(0,0,0,0,nWeight,bItalic,bUnderline,0,0,0,0,0,0,Name);
}

//---------------------------------------------------------------------------------------------
//initialize dialog's resource

void CMainSheet::InitDlgResource()
{
	SetDlgItemText(IDOK,GUI_BUTTON_OK);
	SetDlgItemText(IDCANCEL,GUI_BUTTON_CANCEL);
	SetDlgItemText(IDHELP,GUI_BUTTON_HELP);
	SetDlgItemText(ID_APPLY_NOW,GUI_BUTTON_APPLY);
}

