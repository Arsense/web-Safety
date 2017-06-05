// NeighborDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Neighbor.h"
#include "NeighborDlg.h"
#include "DirectDriver.h"
#include "LocalNameDlg.h"
#include "HostNameDlg.h"
#include  "afxsock.h"
//#include "Ws2tcpip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include <winnetwk.h>

#pragma comment(lib, "mpr.lib")

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
// CNeighborDlg dialog

CNeighborDlg::CNeighborDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNeighborDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNeighborDlg)
	m_NetListBox = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNeighborDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNeighborDlg)
	DDX_LBString(pDX, IDC_LIST, m_NetListBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNeighborDlg, CDialog)
	//{{AFX_MSG_MAP(CNeighborDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LISTBUTTON, OnListbutton)
	ON_BN_CLICKED(IDC_IPBUTTON, OnIpbutton)
	ON_BN_CLICKED(IDC_NAVIGATOR, OnNavigator)
	ON_BN_CLICKED(IDC_REDIRECT, OnRedirect)
	ON_BN_CLICKED(IDC_DISCONNECTDIRECT, OnDisconnectdirect)
	ON_BN_CLICKED(IDC_IPTOHOSTNAME, OnIptohostname)
	ON_BN_CLICKED(IDC_NETSEND, OnNetsend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeighborDlg message handlers

BOOL CNeighborDlg::OnInitDialog()
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
	if(AfxSocketInit(NULL)==FALSE)
	{
		AfxMessageBox("Socket Error");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNeighborDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNeighborDlg::OnPaint() 
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
HCURSOR CNeighborDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//-----------------------------------------------------------------------
//列取网上邻居
BOOL CNeighborDlg::Enumerate(LPNETRESOURCE lpNetRC_p,int depth)
{
	HANDLE hEnum = 0;
	DWORD dwScope = RESOURCE_GLOBALNET ;
	DWORD dwType = RESOURCETYPE_ANY ;

	DWORD dwResult = WNetOpenEnum(
	dwScope,	// scope of enumeration
	dwType,		// resource types to list
	0,			// enumerate all resources
	lpNetRC_p, // pointer to resource structure (NULL at first time)
	&hEnum		// handle to resource
	) ;

	if( dwResult != NO_ERROR )
		return FALSE;


	DWORD dwBuffer = 16384 ;		// 16K is reasonable size
	DWORD dwEntries = -1 ;			//-1, enumerate all possible entries
	LPNETRESOURCE lpnrLocal = 0;

	BOOL bRet = TRUE;

	do
	{
	// first allocate buffer for NETRESOURCE structures ...
		lpnrLocal = (LPNETRESOURCE) GlobalAlloc( GPTR, dwBuffer ) ;

		dwResult = WNetEnumResource(
		hEnum, // resource-handle
		&dwEntries,
		lpnrLocal,
		&dwBuffer
		) ;

		if( dwResult == NO_ERROR )
		{
			for(DWORD i = 0 ; i<dwEntries;i++)
			{
				CString nsname;
				nsname = lpnrLocal[i].lpRemoteName;
				nsname += " ";
				nsname +=lpnrLocal[i].lpLocalName;
				nsname += " ";
				nsname +=lpnrLocal[i].lpComment;
				
				CListBox *listbox=(CListBox*)GetDlgItem(IDC_LIST);
				if(depth==1)	//workgroup
				{
					listbox->AddString(nsname);
				}
				if(depth==2)	//user
				{
					listbox->AddString("   "+nsname);
					m_NeighborList.AddTail(nsname);
				}
				m_List.AddTail(nsname+CString(depth));


				if( RESOURCEUSAGE_CONTAINER == 
					(lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER) )
				if( !Enumerate( &lpnrLocal[i],depth+1) )
				{
					//TRACE0( "CNetwork::Enumerate(): recursiv call failed\n" );
					//throw CNetworkBreak(FALSE);
					//	return FALSE;
				} 

			}
		} 
		else 
			if( dwResult != ERROR_NO_MORE_ITEMS ) 
			{
				return FALSE;
			}
	} while( dwResult != ERROR_NO_MORE_ITEMS );

	if( lpnrLocal )
		GlobalFree((HGLOBAL) lpnrLocal) ;
	WNetCloseEnum(hEnum) ;
	return bRet;
}

//列取网上邻居的按钮消息
void CNeighborDlg::OnListbutton() 
{
	CListBox *listbox=(CListBox*)GetDlgItem(IDC_LIST);
	listbox->ResetContent();
	BeginWaitCursor();
	EnableWindow(FALSE);
	Enumerate(0,0);
	EndWaitCursor();
	EnableWindow(TRUE);
	m_List.RemoveAll();
	m_NeighborList.RemoveAll();
}

BOOL CNeighborDlg::Enumerate2(LPNETRESOURCE lpnr,int depth)
{

  DWORD dwResult, dwResultEnum;
  HANDLE hEnum;
  DWORD cbBuffer = 16384;      // 16K is a good size
  DWORD cEntries = -1;         // enumerate all possible entries
  LPNETRESOURCE lpnrLocal;     // pointer to enumerated structures
  DWORD i;
  //
  // Call the WNetOpenEnum function to begin the enumeration.
  //
  dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network resources
                          RESOURCETYPE_ANY,   // all resources
                          0,        // enumerate all resources
                          lpnr,     // NULL first time the function is called
                          &hEnum);  // handle to the resource

  if (dwResult != NO_ERROR)
  {  
    //
    // Process errors with an application-defined error handler.
    //
    return FALSE;
  }
  //
  // Call the GlobalAlloc function to allocate resources.
  //
  lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
  
  do
  {  
    //
    // Initialize the buffer.
    //
    ZeroMemory(lpnrLocal, cbBuffer);
    //
    // Call the WNetEnumResource function to continue
    //  the enumeration.
    //
    dwResultEnum = WNetEnumResource(hEnum,      // resource handle
                                    &cEntries,  // defined locally as -1
                                    lpnrLocal,  // LPNETRESOURCE
                                    &cbBuffer); // buffer size
    //
    // If the call succeeds, loop through the structures.
    //
    if (dwResultEnum == NO_ERROR)
    {
      for(i = 0; i < cEntries; i++)
      {
        // Call an application-defined function to
        //  display the contents of the NETRESOURCE structures.
        //
				CString nsname;
				nsname = lpnrLocal[i].lpRemoteName;
				nsname += " ";
				nsname +=lpnrLocal[i].lpLocalName;
				nsname += " ";
				nsname +=lpnrLocal[i].lpComment;
				
				CListBox *listbox=(CListBox*)GetDlgItem(IDC_LIST);
				listbox->AddString(nsname);
				m_List.AddTail(nsname+CString(depth));
        // If the NETRESOURCE structure represents a container resource, 
        //  call the EnumerateFunc function recursively.

        if(RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage
                                       & RESOURCEUSAGE_CONTAINER))
          if(!Enumerate2(&lpnrLocal[i],depth+1))
		  {
			 MessageBox("EnumerateFunc returned FALSE.");
		  }
      }
    }
    // Process errors.
    //
    else if (dwResultEnum != ERROR_NO_MORE_ITEMS)
    {
 //     NetErrorHandler(hwnd, dwResultEnum, (LPSTR)"WNetEnumResource");
      break;
    }
  }
  //
  // End do.
  //
  while(dwResultEnum != ERROR_NO_MORE_ITEMS);
  //
  // Call the GlobalFree function to free the memory.
  //
  GlobalFree((HGLOBAL)lpnrLocal);
  //
  // Call WNetCloseEnum to end the enumeration.
  //
  dwResult = WNetCloseEnum(hEnum);
  
  if(dwResult != NO_ERROR)
  { 
    //
    // Process errors.
    //
 //   NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetCloseEnum");
    return FALSE;
  }

  return TRUE;
}

//映射网络驱动器
void CNeighborDlg::Redirect(const char *LocalName,const char *RemoteName,const char * UserName,const char *Password)
{
	NETRESOURCE nr;
	DWORD res;

	char szUserName[32],
		szPassword[32],
		szLocalName[32],
		szRemoteName[MAX_PATH];

	strcpy(szUserName,UserName);
	strcpy(szPassword,Password);
	strcpy(szLocalName,LocalName);
	strcpy(szRemoteName,RemoteName);
	
	nr.dwType = RESOURCETYPE_ANY;
	nr.lpLocalName = szLocalName;
	nr.lpRemoteName = szRemoteName;
	nr.lpProvider = NULL;	
	//
	res = WNetAddConnection2(&nr, szPassword, szUserName, FALSE);
	//
	switch(res)
	{
	case NO_ERROR:
		AfxMessageBox("网络驱动器映射成功");
		break;
	case ERROR_BAD_PROFILE:
		AfxMessageBox("ERROR_BAD_PROFILE");
		break;
	case ERROR_CANNOT_OPEN_PROFILE:
		AfxMessageBox("ERROR_CANNOT_OPEN_PROFILE");
		break;
	case ERROR_DEVICE_IN_USE:
		AfxMessageBox("ERROR_DEVICE_IN_USE");
		break;
	case ERROR_EXTENDED_ERROR:
		AfxMessageBox("ERROR_EXTENDED_ERROR");
		break;
	case ERROR_NOT_CONNECTED:
		AfxMessageBox("ERROR_NOT_CONNECTED");
		break;
	case ERROR_OPEN_FILES:
		AfxMessageBox("ERROR_OPEN_FILES");
		break;
	default:
		AfxMessageBox("未知错误,可能需要帐号和密码认证，或者该主机或文件不存在");
		break;
	}
	return;
}

//断开网络驱动器映射
void CNeighborDlg::DisConnectDirect(const char * localname)
{
//	CHAR  szDeviceName[80]; 
//	DWORD cchBuff = sizeof(szDeviceName); 
	/*
	dwResult = WNetGetConnection(localname, 
		(LPSTR) szDeviceName, 
		&cchBuff); 
	*/

	DWORD dwResult;
	
	dwResult = WNetCancelConnection2(localname, 
    CONNECT_UPDATE_PROFILE, // remove connection from profile 
    FALSE);                 // fail if open files or jobs 
	switch(dwResult)
	{
	case NO_ERROR:
		AfxMessageBox("成功断开网络驱动器映射");
		break;
	case ERROR_BAD_PROFILE:
		AfxMessageBox("ERROR_BAD_PROFILE");
		break;
	case ERROR_CANNOT_OPEN_PROFILE:
		AfxMessageBox("ERROR_CANNOT_OPEN_PROFILE");
		break;
	case ERROR_DEVICE_IN_USE:
		AfxMessageBox("ERROR_DEVICE_IN_USE");
		break;
	case ERROR_EXTENDED_ERROR:
		AfxMessageBox("ERROR_EXTENDED_ERROR");
		break;
	case ERROR_NOT_CONNECTED:
		AfxMessageBox("ERROR_NOT_CONNECTED");
		break;
	case ERROR_OPEN_FILES:
		AfxMessageBox("ERROR_OPEN_FILES");
		break;
	default:
		AfxMessageBox("未知错误");
		break;
	}
}



//获得本地计算机名称
int CNeighborDlg::GetLocalHostName(CString &sHostName)	
{
	char szHostName[256];
	int nRetCode;
	nRetCode=gethostname(szHostName,sizeof(szHostName));
	if(nRetCode!=0)
	{
		//产生错误
		sHostName=_T("没有取得");
		return GetLastError();
	}
	sHostName=szHostName;
	return 0;
}

int CNeighborDlg::GetIpAddress(const CString &sHostName, CStringArray &sIpAddress)//获得本地IP
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	sIpAddress.RemoveAll();
	if(lpHostEnt==NULL)
	{
		//产生错误
		return GetLastError();
	}
	//获取IP
	int i=0;
	LPSTR lpAddr=lpHostEnt->h_addr_list[i];
	CString temp;
	while(lpAddr)
	{
		i++;
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//转换为标准格式
		temp=inet_ntoa(inAddr);
		if(temp.IsEmpty())
		{
			break;
		}
		sIpAddress.Add(temp);
		lpAddr=lpHostEnt->h_addr_list[i];
	}
	return 0;
}

int CNeighborDlg::GetIpAddress(const CString &sHostName, BYTE f0[],BYTE f1[],BYTE f2[],BYTE f3[],int &count)//获得本地IP
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		f0[0]=f1[0]=f2[0]=f3[0]=0;
		return GetLastError();
	}
	//获取IP
	count=0;
	LPSTR lpAddr=lpHostEnt->h_addr_list[count];
	while(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		f0[count]=inAddr.S_un.S_un_b.s_b1;
		f1[count]=inAddr.S_un.S_un_b.s_b2;
		f2[count]=inAddr.S_un.S_un_b.s_b3;
		f3[count]=inAddr.S_un.S_un_b.s_b4;
		count++;
		lpAddr=lpHostEnt->h_addr_list[count];
	}
	return 0;
}


//由主机名称得到IP地址
void CNeighborDlg::OnIpbutton() 
{
	// TODO: Add your control notification handler code here
	CStringArray ip;
	CHostNameDlg dlg;
	dlg.DoModal();
	GetIpAddress(dlg.m_sHostName,ip);//获得本地IP
	CString temp;
	int i=0;
	while(i<ip.GetSize())
	{
		
		temp+=ip[i];
		temp+=";\n";
		i++;
	}
	AfxMessageBox(dlg.m_sHostName+"的IP是\n"+temp);
}

//网上邻居的浏览：这是利用CFileDlg的情况
void CNeighborDlg::OnNavigator() 
{
	// TODO: Add your control notification handler code here
	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,"All Files (*.*)|*.*||",this);
	TRACE("%d",filedlg.m_ofn.nMaxFile);
	TRACE("%d",sizeof(filedlg.m_ofn.lpstrFile));
	//扩大多选文件名的缓冲区,这是为OFN_ALLOWMULTISELECT服务的
	filedlg.m_ofn.lpstrTitle="浏览网上邻居";
	filedlg.m_ofn.nMaxFile=1024;			
	filedlg.m_ofn.lpstrFile=new TCHAR[1024];
	filedlg.m_ofn.lpstrFile[0]=NULL;
	char path[MAX_PATH];
	(SHGetSpecialFolderPath(this->GetSafeHwnd(),path,CSIDL_NETWORK,FALSE));
	filedlg.m_ofn.lpstrInitialDir=path;
	
	//如何让这个窗口定位在网络邻居上？？？
	filedlg.DoModal();

	POSITION pos=filedlg.GetStartPosition();
	CListBox *listbox=(CListBox*)GetDlgItem(IDC_LIST);
	listbox->ResetContent();
	CString pathname;
	while(pos!=NULL)
	{
		pathname=filedlg.GetNextPathName(pos);
		listbox->AddString(pathname);
	}
	//得到了所选的文件后,可以调用其它API函数进行拷贝等操作
	delete filedlg.m_ofn.lpstrFile;
}



void CNeighborDlg::OnRedirect() 
{
	// TODO: Add your control notification handler code here
	CDirectDriver direct;
	if(direct.DoModal()==IDOK)
	{
		Redirect(LPCSTR(direct.m_localname),LPCSTR(direct.m_remotename),LPCSTR(direct.m_username),LPCSTR(direct.m_password));
	}
}

void CNeighborDlg::OnDisconnectdirect() 
{
	// TODO: Add your control notification handler code here
	CLocalNameDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		DisConnectDirect(LPCSTR(dlg.m_sLocalName));
	}
}

void CNeighborDlg::OnIptohostname() 
{
	// TODO: Add your control notification handler code here
	CString ip,name;
	CHostNameDlg dlg(FALSE);
	dlg.DoModal();
	unsigned long a;
	ip=dlg.m_sHostName;
	a=inet_addr(ip);
	struct hostent FAR *host=gethostbyaddr((char *)(&a),4,AF_INET);
	if(host==NULL)
	{
		AfxMessageBox("错误的IP地址");
		return;
	}
	int b=GetLastError();
	ip=host->h_name;
	AfxMessageBox(dlg.m_sHostName+"的主机名是"+ip);
}

void CNeighborDlg::OnNetsend() 
{
	CString strComputer,strMessage;

	//先检测传送内容
	GetDlgItemText(IDC_MESSAGE,strMessage);
	if(strMessage.IsEmpty())
	{
		AfxMessageBox("传送的内容不能为空!");
		return;
	}

	//收集发送的目的地
	GetDlgItemText(IDC_SEND_ADDRESS,strComputer);
	if(strComputer.IsEmpty())
	{
		CListBox *list=(CListBox*)GetDlgItem(IDC_LIST);
		if(list->GetCurSel()<0)
		{
			AfxMessageBox("请在发送地址框中输入发送目的地!\r\n或者请从左边的列表中选择相应的机器，如果该列表为空，则点击\"列取网上邻居\"按钮");
			return;
		}
		list->GetText(list->GetCurSel(),strComputer);
		strComputer.TrimLeft(" ");
		if(strComputer.IsEmpty())
		{
			AfxMessageBox("请在发送地址框中输入发送目的地!\r\n或者请从左边的列表中选择相应的机器，如果该列表为空，则点击\"列取网上邻居\"按钮");
			return;
		}
	}
	strComputer.TrimLeft(" ");
	strComputer.TrimLeft("\\");
	strComputer.TrimRight(" ");

	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
    ZeroMemory( &pi, sizeof(pi) );

	CString strCommand=
		CString("net send ")+strComputer+" "+strMessage;

	BeginWaitCursor();
    // Start the child process. 
	if( !CreateProcess( NULL, // No module name (use command line). 
        (LPSTR)LPCSTR(strCommand), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
    {
        AfxMessageBox( "创建发送进程失败." );
    }
	
    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );
	EndWaitCursor();
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}
