// ViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pop3.h"
#include "ViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog


CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewDlg)
	text = _T("");
	//}}AFX_DATA_INIT
}


void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDlg)
	DDX_Text(pDX, IDC_MSGTEXT, text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
	//{{AFX_MSG_MAP(CViewDlg)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDlg message handlers

void CViewDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE);
	if(dlg.DoModal()==IDOK)
	{
	CFile file(dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	file.Write((LPCSTR)text,text.GetLength());
	file.Close();
	}
}
