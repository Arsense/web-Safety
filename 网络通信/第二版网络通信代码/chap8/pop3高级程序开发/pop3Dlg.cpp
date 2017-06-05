// pop3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "pop3.h"
#include "pop31.h"
#include "Gniazdo.h"
#include "ChooseDlg.h"
#include "pop3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPop3Dlg dialog

CPop3Dlg::CPop3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPop3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPop3Dlg)
	info = _T("");
	pass = _T("wxpwxp");
	server = _T("pop.163.net");
	user = _T("busywxp");
	del = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPop3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPop3Dlg)
	DDX_Control(pDX, IDC_INFO, ctlInfo);
	DDX_Text(pDX, IDC_INFO, info);
	DDX_Text(pDX, IDC_PASS, pass);
	DDX_Text(pDX, IDC_SERVER, server);
	DDX_Text(pDX, IDC_USER, user);
	DDX_Check(pDX, IDC_DEL, del);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPop3Dlg, CDialog)
	//{{AFX_MSG_MAP(CPop3Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONN, OnConn)
	ON_BN_CLICKED(IDC_DISC, OnDisc)
	ON_BN_CLICKED(IDC_VIEW, OnView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPop3Dlg message handlers

BOOL CPop3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CHARFORMAT cf;
	cf.cbSize=sizeof(cf);
	cf.dwMask=CFM_COLOR | CFM_FACE;
	cf.dwEffects=0;
	cf.crTextColor=RGB(255,0,0);
	strcpy(cf.szFaceName,"Verdana");
	CRichEditCtrl* re=(CRichEditCtrl*)GetDlgItem(IDC_INFO);
	re->SetDefaultCharFormat(cf);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPop3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPop3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//important function!
//receives messages from CPop3 class!
void CPop3Dlg::Dispatch(LONG param)
{

	CString s;

switch(param)
{
case S_CONNECT: //we are connected
	info+="Connected to "+server;
	info+="...\r\n";
	GetDlgItem(IDC_CONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DISC)->EnableWindow(TRUE);
	GetDlgItem(IDC_VIEW)->EnableWindow(FALSE);
	break;
case S_RECEIVE: //we are receiving some data
	pop3.GetLastMsg(s);
	info+=s;
	break; 
case S_CLOSE: //we are closing connection
	info+=pop3.GetError();
	info+="Connection closed\r\n";
	GetDlgItem(IDC_CONN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DISC)->EnableWindow(FALSE);
	break;
case S_GETNUMMSGS: //we can get number of messages
	{
	s.Format("There are %d messages\r\n",pop3.GetNumMsg());
	info+=s;
	break;
	}
case S_GETSIZEMSGS: //now size of messages
	{
	s.Format("Size is: %d\r\n",pop3.GetSizeMsg());
	info+=s;
	break;
	}
case S_ENDRETR: //we have received all of messages
	{
	s.Format("Received %d messages\r\n",pop3.GetRetrMsgNum());
	info+=s;
	if(pop3.GetRetrMsgNum()>0)
	GetDlgItem(IDC_VIEW)->EnableWindow(TRUE);
	break;
	}
}

	UpdateData(FALSE);
}

void CPop3Dlg::OnConn() 
{
	// TODO: Add your control notification handler code here
	pop3.Set(this); //设定串口以便于接收信息
	UpdateData(TRUE);
	info="";
	pop3.SetProp(user,pass); //设定用户名和密码
	pop3.DelAfterRead(del);
	pop3.Create();
	pop3.Connect((LPCSTR)server,110); //连接服务器
	UpdateData(FALSE);
}

void CPop3Dlg::OnDisc() 
{
	// TODO: Add your control notification handler code here
	pop3.Close();

}

void CPop3Dlg::OnView() 
{
	// TODO: Add your control notification handler code here
	CChooseDlg dlg;
	dlg.DoModal();
}
