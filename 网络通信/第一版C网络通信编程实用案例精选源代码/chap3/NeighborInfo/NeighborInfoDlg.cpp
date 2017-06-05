// NeighborInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NeighborInfo.h"
#include "NeighborInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NBTSTATPORT 137		//nbtstat name port
#define OWNERPORT	4321

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//--------------------------------var-------------------------------
BYTE bs[50]=
{0x0,0x00,0x0,0x10,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x43,0x4b,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x0,0x0,0x21,0x0,0x1};
HANDLE wait_handle;
UINT NbtstatThread(LPVOID param);



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
// CNeighborInfoDlg dialog

CNeighborInfoDlg::CNeighborInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNeighborInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNeighborInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNeighborInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNeighborInfoDlg)
	DDX_Control(pDX, IDC_LISTINFOR, m_ListView);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNeighborInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CNeighborInfoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BEGINSCAN, OnBeginScan)
	ON_BN_CLICKED(IDC_STOPSCAN, OnStopScan)
	ON_BN_CLICKED(IDC_MYHELP, OnMyHelp)
	ON_BN_CLICKED(IDC_EMPTY, OnEmpty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeighborInfoDlg message handlers

BOOL CNeighborInfoDlg::OnInitDialog()
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
	wait_handle=CreateEvent(NULL,true,false,"receive data");
	GetDlgItem(IDC_STOPSCAN)->EnableWindow(false);

	DWORD dwStyle=GetWindowLong(m_ListView.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_ListView.GetSafeHwnd(),GWL_STYLE,dwStyle);

	//初始化视图列表
    m_ListView.InsertColumn(0,"MAC地址",LVCFMT_LEFT,120);
	m_ListView.InsertColumn(0,"用户\\其它",LVCFMT_LEFT,100);
	m_ListView.InsertColumn(0,"主机",LVCFMT_LEFT,80);
	m_ListView.InsertColumn(0,"工作组",LVCFMT_LEFT,80);
	m_ListView.InsertColumn(0,"IP地址",LVCFMT_LEFT,100); 
	m_ListView.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_ListView.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
      LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	//初始化端口
	if(AfxSocketInit(NULL)==FALSE)
	{
		AfxMessageBox("Error Init Socket");
	}
	m_socket.Init(this);
	if(m_socket.Create(OWNERPORT,SOCK_DGRAM)==FALSE)
	{
		AfxMessageBox("Socket Create Error");
	}

	m_bStop=FALSE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNeighborInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNeighborInfoDlg::OnPaint() 
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
HCURSOR CNeighborInfoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNeighborInfoDlg::OnBeginScan() 
{
	// TODO: Add your control notification handler code here
	CIPAddressCtrl	*c=(CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESSSTART));
	c->GetAddress(m_bStartAddress[0],m_bStartAddress[1],m_bStartAddress[2],m_bStartAddress[3]);
	c=(CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESSEND));
	c->GetAddress(m_bEndAddress[0],m_bEndAddress[1],m_bEndAddress[2],m_bEndAddress[3]);

	if(m_bEndAddress[0]!=m_bStartAddress[0]||m_bEndAddress[0]!=m_bStartAddress[0])
	{
		AfxMessageBox("不支持A类或B类地址");
		m_bEndAddress[0]=m_bStartAddress[0];
		m_bEndAddress[1]=m_bStartAddress[1];
		if(m_bEndAddress[2]<m_bStartAddress[2])
		{
			m_bEndAddress[2]=m_bStartAddress[2];
			if(m_bEndAddress[3]<m_bStartAddress[3])
			{
				m_bEndAddress[3]=255;
			}
		}
		else	if(m_bEndAddress[2]=m_bStartAddress[2])
		{
			if(m_bEndAddress[3]<m_bStartAddress[3])
			{
				m_bEndAddress[3]=255;
			}
		}
		c->SetAddress(m_bEndAddress[0],m_bEndAddress[1],m_bEndAddress[2],m_bEndAddress[3]);
	}
	GetDlgItem(IDC_IPADDRESSSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESSEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BEGINSCAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOPSCAN)->EnableWindow(TRUE);

	//开始扫描
	AfxBeginThread(NbtstatThread,(LPVOID)this,THREAD_PRIORITY_NORMAL);
}

void CNeighborInfoDlg::OnStopScan() 
{
	// TODO: Add your control notification handler code here
	m_bStop=TRUE;
}

void CNeighborInfoDlg::ReceiveMessage()
{
	BYTE Buf[500];
	//m_UDPSocket.Receive(Buf,500,0);
	static CString strOldIP;
	CString str,strIP,strHost,strHex,strMac,Host,Group,User;
	UINT dport;
	m_socket.ReceiveFrom(Buf,500,strIP,dport,0);

	if(strIP==(char)NULL||strIP==strOldIP)return;
	strOldIP=strIP;

	//ip
	int index=m_ListView.InsertItem(0,strIP);

	strHost="";
	strHex="";
	User="?";
	Host="\\";
	
	int tem=0,num=0;
	bool bAdd=true;

	for(int i=0;i<500;i++)
	{
		if(i%50==0)
			TRACE("\n");
		TRACE("%c",Buf[i]);
	}

	//nbtstat前面的57个字节是
	for(i=57;i<500;i++) //57-72
	{
		//end
		if(Buf[i]==0xcc)
			break; 
		
		if(Buf[i]==0x20)
			bAdd=false;
		if(bAdd)
		{
			str.Format("%c",Buf[i]);
			if(Buf[i]>=' ')strHost+=str;

			str.Format("%02x.",Buf[i]);
			strHex+=str;
		}

		if((++tem)%18==0)
		{
            bAdd=true; 
			//m_ListBox.AddString(strHost);//
			strHost.TrimRight((char)NULL);
			if(strHost=="")
			{
   				strMac.Delete(17,strMac.GetLength()-17);
				m_ListView.SetItem(index,4,LVIF_TEXT,strMac, 0, 0, 0,0);
				break;
			}

			if(num==0&&strHost!="")
			{
				m_ListView.SetItem(index,2,LVIF_TEXT,strHost, 0, 0, 0,0);
				Host=strHost;
				num++;
			}
			else
			{
				if(Host!=strHost&&num==1&&strHost!="")
				{
					m_ListView.SetItem(index,1,LVIF_TEXT,strHost, 0, 0, 0,0);
					Group=strHost;
				    num++;
				}
				else 
				{
					if(strHost!=Host&&strHost!=Group&&num==2&&strHost!="")
					{
						User=strHost;
						if(User!="__MSBROWSE__")
						{
							m_ListView.SetItem(index,3,LVIF_TEXT,User, 0, 0, 0,0);
							num++;
						}
					}
				}

			}
			
			strMac=strHex;
			strHost="";
			strHex="";
			
		}
		
	}

	SetEvent(wait_handle);
}

void CNeighborInfoDlg::OnMyHelp() 
{
	// TODO: Add your control notification handler code here
	CDialog dlg(IDD_ABOUTBOX);
	dlg.DoModal();
	
}

//--------------------------nbtstat线程----------------------
UINT NbtstatThread(LPVOID param)
{
	CNeighborInfoDlg * dlg=(CNeighborInfoDlg *)param;
	BYTE begin[4],end[4];
	memcpy(begin,dlg->m_bStartAddress,sizeof(dlg->m_bStartAddress));
	memcpy(end,dlg->m_bEndAddress,sizeof(dlg->m_bEndAddress));
	CString ip;
	do
	{
		if(dlg->m_bStop)
		{
			AfxMessageBox("终止扫描!");
			dlg->GetDlgItem(IDC_STOPSCAN)->EnableWindow(false);
	        dlg->GetDlgItem(IDC_BEGINSCAN)->EnableWindow(true);
			dlg->GetDlgItem(IDC_IPADDRESSSTART)->EnableWindow(true);
			dlg->GetDlgItem(IDC_IPADDRESSEND)->EnableWindow(true);
			dlg->m_bStop=FALSE;
			return 1;
		}

		ip.Format("%d.%d.%d.%d",begin[0],begin[1],begin[2],begin[3]);
		if(begin[3]!=0&&begin[2]!=0)
			dlg->m_socket.SendTo((void*)bs,50,NBTSTATPORT,ip,0);
		
 		WaitForSingleObject(wait_handle,100);
		ResetEvent(wait_handle);
		
		//=============================================
		if(begin[2]<=end[2])
		{
		   if(begin[3]<end[3])
			   begin[3]++;
		   else if(begin[2]<end[2]&&begin[3]<255)
			   begin[3]++;
		   else if(begin[2]<end[2]&&begin[3]==255)
		   {
			   begin[3]=0;
			   begin[2]++;
		   }
		}
		else 
			break;
		if(begin[3]>=end[3]&&begin[2]>=end[2])
			break;
	}while(begin[2]<=255&&begin[3]<=255);

	dlg->GetDlgItem(IDC_STOPSCAN)->EnableWindow(false);
	dlg->GetDlgItem(IDC_BEGINSCAN)->EnableWindow(true);
	dlg->GetDlgItem(IDC_IPADDRESSSTART)->EnableWindow(true);
	dlg->GetDlgItem(IDC_IPADDRESSEND)->EnableWindow(true);
	return 0;
}
//-----------------------------------------------------------

void CNeighborInfoDlg::OnEmpty() 
{
	// TODO: Add your control notification handler code here
	m_ListView.DeleteAllItems();
}
