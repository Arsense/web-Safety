// AddUserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "AddUserInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddUserInfoDlg dialog


CAddUserInfoDlg::CAddUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddUserInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddUserInfoDlg)
	m_strPath = _T("");
	m_bAllowCreateDirectory = FALSE;
	m_bAllowDelete = FALSE;
	m_bAllowDownload = FALSE;
	m_bAllowRename = FALSE;
	m_bAllowUpload = FALSE;
	m_strUserName = _T("");
	m_strUserPass = _T("");
	m_strUserPassAgain = _T("");
	//}}AFX_DATA_INIT
}


void CAddUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddUserInfoDlg)
	DDX_Text(pDX, IDC_PATH, m_strPath);
	DDX_Check(pDX, IDC_CREATE_DIR, m_bAllowCreateDirectory);
	DDX_Check(pDX, IDC_DELETE, m_bAllowDelete);
	DDX_Check(pDX, IDC_DOWNLOAD, m_bAllowDownload);
	DDX_Check(pDX, IDC_RENAME, m_bAllowRename);
	DDX_Check(pDX, IDC_UPLOAD, m_bAllowUpload);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_USERPASS, m_strUserPass);
	DDX_Text(pDX, IDC_EDIT_USERPASSAGAIN, m_strUserPassAgain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddUserInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CAddUserInfoDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddUserInfoDlg message handlers

void CAddUserInfoDlg::OnBrowse() 
{
	CString strDir = BrowseForFolder(m_hWnd, "选择目录:", BIF_RETURNONLYFSDIRS);
	if (!strDir.IsEmpty())
	{
		//m_strPath = strDir;
		//UpdateData(FALSE);//刚开始是UpdateData(FALSE),点击选择文件目录时选择目录后会刷新各个控件中的数据
		//但是不刷新选择目录后又不会显示出来!
		GetDlgItem(IDC_PATH)->SetWindowText(strDir);//还是使用这种方法靠谱！
	}	
}

BOOL CAddUserInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
