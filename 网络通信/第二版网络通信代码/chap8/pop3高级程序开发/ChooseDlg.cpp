// ChooseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pop3.h"
#include "pop3Dlg.h"
#include "ViewDlg.h"
#include "ChooseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChooseDlg dialog


CChooseDlg::CChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseDlg)
	DDX_Control(pDX, IDC_MSGLIST, ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseDlg message handlers

void CChooseDlg::AddToList(CString s)
{
ctlList.AddString((LPCSTR)s);
}

BOOL CChooseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CPop3Dlg* par;
	par=(CPop3Dlg*)GetParent(); //获得父窗体
	for(int i=0;i<par->pop3.GetRetrMsgNum();i++)
	{
		AddToList(par->pop3.GetMsgSubject(i));
	}
	ctlList.SetCurSel(0);
	return TRUE;  
	              
}

void CChooseDlg::OnOK() 
{
	// TODO: Add extra validation here
	CPop3Dlg* par;
	par=(CPop3Dlg*)GetParent();
	CViewDlg dlg; 
	//获得信件的基本属性以及信件内容
	dlg.text=par->pop3.GetMsgStuff(ctlList.GetCurSel());
	dlg.text+=par->pop3.GetMsgBody(0);
	dlg.DoModal();
	//CDialog::OnOK();
}
