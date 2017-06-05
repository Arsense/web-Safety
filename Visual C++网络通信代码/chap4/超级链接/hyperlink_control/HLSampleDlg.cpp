// HLSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HLSample.h"
#include "HLSampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHLSampleDlg dialog

CHLSampleDlg::CHLSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHLSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHLSampleDlg)
	//}}AFX_DATA_INIT
}

void CHLSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHLSampleDlg)
	DDX_Control(pDX, IDC_HYPERLINK, m_HyperLink);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHLSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CHLSampleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLSampleDlg message handlers

//初始化例题
BOOL CHLSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//设定链接地址
	m_HyperLink.SetURL(_T("mailto:busywxp@msn.com"));
	m_HyperLink.SetUnderline(FALSE);

	return TRUE;  
}

