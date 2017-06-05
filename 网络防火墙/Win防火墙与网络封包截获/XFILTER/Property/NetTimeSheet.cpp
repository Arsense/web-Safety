// NetTimeSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Property.h"
#include "NetTimeSheet.h"

//owner add
#include "GuiRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================================
// CNetTimeSheet

extern CAclFile		m_AclFile;
extern CPropertyApp theApp;

IMPLEMENT_DYNAMIC(CNetTimeSheet, CPropertySheet)

CNetTimeSheet::CNetTimeSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CNetTimeSheet::CNetTimeSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CNetTimeSheet::~CNetTimeSheet()
{
}


BEGIN_MESSAGE_MAP(CNetTimeSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CNetTimeSheet)
	ON_BN_CLICKED(ID_APPLY_NOW, OnApply)
	ON_BN_CLICKED(IDOK,			OnOK)
	ON_BN_CLICKED(IDCANCEL,		OnCancel)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================================
// CNetTimeSheet message handlers

BOOL CNetTimeSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	InitDlgResource();

	return bResult;
}

/*---------------------------------------------------------------------------------------------
	initialize dialog's resource
*/
void CNetTimeSheet::InitDlgResource()
{
	SetDlgItemText(IDOK,		GUI_BUTTON_OK);
	SetDlgItemText(IDCANCEL,	GUI_BUTTON_CANCEL);
	SetDlgItemText(IDHELP,		GUI_BUTTON_HELP);
	SetDlgItemText(ID_APPLY_NOW,GUI_BUTTON_APPLY);
}
void CNetTimeSheet::OnOK()
{
	OnApply();
	CPropertySheet::EndDialog(IDOK);
}

void CNetTimeSheet::OnApply()
{
	if(!GetDlgItem(ID_APPLY_NOW)->IsWindowEnabled())
		return;

	if(Apply() != 0)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_SAVE_ACL_IP_TIME_ERROR);
		return;
	}

	if(!theApp.InitDllData())
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR);
		return;
	}

	EnableButton(FALSE);
}

BOOL CNetTimeSheet::OnCancel()
{
	if(!GetDlgItem(ID_APPLY_NOW)->IsWindowEnabled())
	{
		CPropertySheet::EndDialog(IDCANCEL);
		return TRUE;
	}

	if(AfxMessageBox(GUI_ACL_MESSAGE_ACL_ASK_SAVE, MB_YESNO) == IDYES)
	{
		OnApply();
		CPropertySheet::EndDialog(IDCANCEL);
		return FALSE;
	}

	if(m_AclFile.ReadAcl() != XERR_SUCCESS)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_ACL_READ_ERROR);
		return FALSE;
	}

	if(!theApp.InitDllData())
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR);
		return FALSE;
	}

	CPropertySheet::EndDialog(IDCANCEL);

	return TRUE;
}

int CNetTimeSheet::EnableButton(BOOL bEnable)
{
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(bEnable);

	return 0;
}

int CNetTimeSheet::Apply()
{
	return m_AclFile.SaveAcl();
}


void CNetTimeSheet::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if(nID == SC_CLOSE)
	{
		OnCancel();	
		return;
	}

	CPropertySheet::OnSysCommand(nID, lParam);
}
