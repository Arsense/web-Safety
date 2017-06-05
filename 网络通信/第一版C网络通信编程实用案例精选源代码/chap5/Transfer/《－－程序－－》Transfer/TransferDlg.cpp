// TransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Transfer.h"
#include "TransferDlg.h"
#include <afxtempl.h>
#include "afxsock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog

CTransferDlg::CTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CTransferDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_ADDSHAREFILE, OnAddsharefile)
	ON_BN_CLICKED(IDC_DELSHAREFILE, OnDelsharefile)
	ON_BN_CLICKED(IDC_DOWNLOADSHAREFILE, OnDownloadsharefile)
	ON_BN_CLICKED(IDC_SETNETWORK, OnSetnetwork)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg message handlers

BOOL CTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	// TODO: Add extra initialization here

	//其它状态初始化
	m_bClient=FALSE;
	m_bOnLine=FALSE;

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->EnableWindow(FALSE);
	m_iPort=5000;
	SetDlgItemInt(IDC_PORT,m_iPort);

	//socket初始化
	if(AfxSocketInit(NULL)==FALSE)
	{
		AfxMessageBox("Socket Init Error");
		return FALSE;
	}
	//地址初始化
	char name[20];
	gethostname(name,20);
	hostent * host=gethostbyname(name);
	struct in_addr inAddr;
	memmove(&inAddr,host->h_addr_list[0],4);
	m_sIP=inet_ntoa(inAddr);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->SetAddress(inAddr.S_un.S_un_b.s_b1,inAddr.S_un.S_un_b.s_b2,inAddr.S_un.S_un_b.s_b3,inAddr.S_un.S_un_b.s_b4);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTransferDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTransferDlg::OnPaint() 
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
HCURSOR CTransferDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTransferDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_bClient=FALSE;
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
}

void CTransferDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_bClient=TRUE;
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
}

void CTransferDlg::OnAddsharefile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,"All Files (*.*)|*.*||",NULL);
	filedlg.m_ofn.lpstrFile = new char[1024];
	filedlg.m_ofn.lpstrFile[0]='\0';
	filedlg.m_ofn.nMaxFile=1024;
	if(filedlg.DoModal()==IDOK)
	{
		m_LocalShareFiles.RemoveAll();
		CListBox *list=(CListBox *)GetDlgItem(IDC_LOCAL_FILE_LIST);
		list->ResetContent();
		POSITION pos=filedlg.GetStartPosition();
		int i=0;
		while(pos)
		{
			CString temp=filedlg.GetNextPathName(pos);
			m_LocalShareFiles.Add(temp);
			list->AddString(temp);
		}
	}
	delete[] filedlg.m_ofn.lpstrFile;
}

void CTransferDlg::OnDelsharefile() 
{
	// TODO: Add your control notification handler code here
	CListBox *list=(CListBox *)GetDlgItem(IDC_LOCAL_FILE_LIST);
	int count=list->GetSelCount();
	CArray<int,int> selarray;
	selarray.SetSize(count);
	list->GetSelItems(count,selarray.GetData());
	for(int i=0;i<count;i++)
	{
		TRACE("%d\n",selarray[i]);
		list->DeleteString(selarray[i]);
	}
	//-----刷行localfileshare
	count=list->GetCount();
	CMessg msg;
	msg.m_iCommand=0;
	m_LocalShareFiles.RemoveAll();
	for(i=0;i<count;i++)
	{
		CString temp;
		list->GetText(i,temp);
		m_LocalShareFiles.Add(temp);
		msg.m_strText+="!";
		msg.m_strText+=temp;
	}
	if(m_bOnLine)
	{
		m_TransferSocket.SendMessage(&msg);	
	}
}

void CTransferDlg::OnDownloadsharefile() 
{
	// TODO: Add your control notification handler code here
	if(!m_bOnLine)
	{
		AfxMessageBox("你没有连接到任何其它机器上!");
		return;
	}
	CListBox *list=(CListBox *)GetDlgItem(IDC_REMOTE_FILE_LIST);
	int count=list->GetSelCount();
	if(count==0)
	{
		AfxMessageBox("请选择需要下载的文件!");
		return;
	}

	m_NeedReceiveFiles.RemoveAll();
	CArray<int,int> selarray;
	selarray.SetSize(count);
	list->GetSelItems(count,selarray.GetData());
	CMessg msg;
	msg.m_iCommand=1;
	for(int i=0;i<count;i++)
	{
		CString temp;
		list->GetText(selarray[i],temp);
		m_NeedReceiveFiles.Add(temp);
		msg.m_strText+="!";
		msg.m_strText+=temp;
	}
	m_TransferSocket.SendMessage(&msg);
}

void CTransferDlg::HandleAccept()
{
	if(m_ServerSocket.Accept(m_TransferSocket)==FALSE)
	{
		AfxMessageBox("Accept Error");
		return;
	}
	m_TransferSocket.Init(this);

	m_bOnLine=TRUE;

	CString name,temp;
	UINT port;
	m_TransferSocket.GetPeerName(name,port);
	temp.Format("%d",port);
	temp=name+":"+temp+"到访!";
	SetWindowText(LPCSTR(temp));

	//server先发送自己的共享文件名
	int count=m_LocalShareFiles.GetSize();
	CMessg msg;
	msg.m_iCommand=0;
	for(int i=0;i<count;i++)
	{
		msg.m_strText+="!";
		msg.m_strText+=m_LocalShareFiles[i];
	}
//	AfxMessageBox(msg.m_strText);
	m_TransferSocket.SendMessage(&msg);

}

void CTransferDlg::HandleClose()
{
	m_bOnLine=FALSE;
	m_TransferSocket.CloseSocket();
	m_ServerSocket.Close();
}

void CTransferDlg::OnSetnetwork() 
{
	// TODO: Add your control notification handler code here
	if(m_bClient==FALSE)
	{
		m_ServerSocket.Init(this);
		if(m_ServerSocket.Create(GetDlgItemInt(IDC_PORT))==FALSE)
		{
			AfxMessageBox("Socket Create Error");
			return;
		}
		if(m_ServerSocket.Listen()==FALSE)
		{
			AfxMessageBox("Socket Listen Error");
			return;
		}
		SetWindowText("正在监听连接!");
	}
	else
	{

		BYTE b[4];
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->GetAddress(b[0],b[1],b[2],b[3]);
		m_sIP.Format("%d.%d.%d.%d",b[0],b[1],b[2],b[3]);
		if(m_TransferSocket.Create()==FALSE)
		{
			AfxMessageBox("Socket Create Error");
			return;
		}
		if(m_TransferSocket.Connect(m_sIP,GetDlgItemInt(IDC_PORT))==FALSE)
		{
			AfxMessageBox("Socket Connect Error!!");
			m_TransferSocket.Close();
			return;
		}

		m_bOnLine=TRUE;
		m_TransferSocket.Init(this);
		SetWindowText("成功连接到服务器端!");

		int count=m_LocalShareFiles.GetSize();
		if(count==0)
			return;
		CMessg msg;
		msg.m_iCommand=0;
		for(int i=0;i<count;i++)
		{
			msg.m_strText+="!";
			msg.m_strText+=m_LocalShareFiles[i];
		}
		m_TransferSocket.SendMessage(&msg);
	}
}

void CTransferDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	m_TransferSocket.CloseSocket();
	m_ServerSocket.Close();
	m_bOnLine=FALSE;
}
