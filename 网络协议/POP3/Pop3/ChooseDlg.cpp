// ChooseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Pop3.h"
#include "ChooseDlg.h"
#include "afxdialogex.h"
#include "Pop3Dlg.h"
#include "MsgViewDlg.h"


// CChooseDlg 对话框

IMPLEMENT_DYNAMIC(CChooseDlg, CDialogEx)

CChooseDlg::CChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChooseDlg::IDD, pParent)
{

}

CChooseDlg::~CChooseDlg()
{
}

void CChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSGLIST, m_List);
}


BEGIN_MESSAGE_MAP(CChooseDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CChooseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CChooseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChooseDlg 消息处理程序


BOOL CChooseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CPop3Dlg* par;
	par=(CPop3Dlg*)GetParent(); //获得父窗体
	for(int i=0;i<par->Pop3.GetRetrMsgNum();i++)
	{
		AddToList(par->Pop3.GetMsgSubject(i));
	}
	m_List.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CChooseDlg::AddToList(CString s)
{
	
	m_List.AddString((LPCTSTR)s);
}


void CChooseDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CDialogEx::OnCancel();
}



void CChooseDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CPop3Dlg* par;
	par=(CPop3Dlg*)GetParent();
	CMsgViewDlg dlg; 
	//获得信件的基本属性以及信件内容
	dlg.text=par->Pop3.GetMsgStuff(m_List.GetCurSel());
	dlg.text+=par->Pop3.GetMsgBody(0);
	dlg.DoModal();
	CDialogEx::OnOK();
}
