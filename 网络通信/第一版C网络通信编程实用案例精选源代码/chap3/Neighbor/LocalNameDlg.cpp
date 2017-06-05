// LocalNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Neighbor.h"
#include "LocalNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocalNameDlg dialog


CLocalNameDlg::CLocalNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLocalNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocalNameDlg)
	m_sLocalName = _T("");
	//}}AFX_DATA_INIT
}


void CLocalNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocalNameDlg)
	DDX_Text(pDX, IDC_EDIT1, m_sLocalName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocalNameDlg, CDialog)
	//{{AFX_MSG_MAP(CLocalNameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocalNameDlg message handlers

void CLocalNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
