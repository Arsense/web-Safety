// AboutHelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modem.h"
#include "AboutHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutHelpDlg dialog


CAboutHelpDlg::CAboutHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutHelpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutHelpDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutHelpDlg message handlers
