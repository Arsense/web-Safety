// ipmonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ipmon.h"
#include "ipmonDlg.h"
#include "mstcpip.h"
#include "afxsock.h"

#include "Iphlpapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
PROTN2T aOfProto [ PROTO_NUM + 1] = 
{  
	{ IPPROTO_IP   , "IP" },
	{ IPPROTO_ICMP , "ICMP" },  
	{ IPPROTO_IGMP , "IGMP" }, 
	{ IPPROTO_GGP  , "GGP" },  
	{ IPPROTO_TCP  , "TCP" },  
	{ IPPROTO_PUP  , "PUP" },  
	{ IPPROTO_UDP  , "UDP" },  
	{ IPPROTO_IDP  , "IDP" },  
	{ IPPROTO_ND   , "NP"  },  
	{ IPPROTO_RAW  , "RAW" },  
	{ IPPROTO_MAX  , "MAX" },
	{ NULL , "" } 
} ;  
char *get_proto_name( unsigned char proto )
{
	BOOL bFound = FALSE ;
	for( int i = 0 ; i < PROTO_NUM ; i++ )
	{
		if( aOfProto[i].proto == proto )
		{
			bFound = TRUE ;
			break ;
		}	
	}
	if( bFound )
		return aOfProto[i].pprototext ;
	return aOfProto[PROTO_NUM].pprototext ;
}

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
// CIpmonDlg dialog

CIpmonDlg::CIpmonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIpmonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpmonDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_threadID = 0  ;
	m_Multihomed = FALSE ;
	m_Local    = TRUE ;
}

void CIpmonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpmonDlg)
	DDX_Control(pDX, IDC_LIST, m_ctrList);
	DDX_Control(pDX, IDC_LOOKUP, m_start);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIpmonDlg, CDialog)
	//{{AFX_MSG_MAP(CIpmonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOOKUP, OnLookUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpmonDlg message handlers

BOOL CIpmonDlg::OnInitDialog()
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


	CHAR		szHostName[128] = {0};
	HOSTENT*	pHost = NULL;
	CHAR*		pszIp = NULL;
	int			iNum = 0;
	if(AfxSocketInit(NULL)==FALSE)
	{
		AfxMessageBox("Sorry, socket load error!");
		return FALSE;
	}

	if(gethostname(szHostName, 128)==0)
	{

		
		pHost = gethostbyname(szHostName);
		if(pHost != NULL)
		{
				pszIp = inet_ntoa(*(in_addr*)pHost->h_addr_list[iNum]);
				m_ipsource = inet_addr(pszIp);
		}
		else AfxMessageBox("pHost = NULL!");
	}
	else AfxMessageBox("can't find host name!");

	//  ListView initialize

	DWORD dwStyle=GetWindowLong(m_ctrList.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_ctrList.GetSafeHwnd(),GWL_STYLE,dwStyle);

	m_ctrList.InsertColumn(0,"数据",LVCFMT_LEFT,525);
	m_ctrList.InsertColumn(0,"大小",LVCFMT_LEFT,80);
    m_ctrList.InsertColumn(0,"端口",LVCFMT_LEFT,40);
	m_ctrList.InsertColumn(0,"目的地址",LVCFMT_LEFT,100);
	m_ctrList.InsertColumn(0,"端口",LVCFMT_LEFT,40);
	m_ctrList.InsertColumn(0,"源地址",LVCFMT_LEFT,100);
	m_ctrList.InsertColumn(0,"协议",LVCFMT_LEFT,50);
    
    ::SendMessage(m_ctrList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


    // Here read all IPs of this host
	DWORD dwSize = 0 ;
    GetIpAddrTable( NULL , &dwSize, FALSE ) ;
    PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE )new BYTE [ dwSize ] ; 
	if( pIpAddrTable )
	{
		if( GetIpAddrTable( (PMIB_IPADDRTABLE)pIpAddrTable,   // // buffer for IP table
							&dwSize,                // size of buffer
							FALSE                  // sort by IP address 
							) == NO_ERROR )
		{
 			if(  pIpAddrTable->dwNumEntries > 2 ) // Second is MS TCP loopback IP ( 127.0.0.1 )
			{
				m_Multihomed = TRUE ;
				char szIP[16];
				for( int i = 0 ; i < (int)pIpAddrTable->dwNumEntries ; i++ )
				{
					in_addr ina ;
					ina.S_un.S_addr = pIpAddrTable->table[i].dwAddr ;
 					char *pIP = inet_ntoa( ina ) ;
					strcpy( szIP , pIP ) ;
					if( stricmp( szIP , "127.0.0.1" ) )
						m_IPArr.Add(pIpAddrTable->table[i].dwAddr) ;
				}
			}
		}
		delete [] pIpAddrTable ;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIpmonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIpmonDlg::OnPaint() 
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
HCURSOR CIpmonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


UINT threadFunc ( LPVOID p )
{
 	CIpmonDlg *pDlg = static_cast<CIpmonDlg *>(p) ;
	char  buf [1000] , *bufwork ;
	MSG   msg ;
	int   iRet ;
	DWORD dwErr ;
    char  *pSource , *pDest ;
	IPHEADER *pIpHeader ;
	in_addr ina ;
	char   szSource [16] , szDest[16] , szErr [ 50 ];
	char *pLastBuf = NULL ;

	int		HdrLen, totallen;
	WORD	sourport, destport;

	struct TCPPacketHead	*pTCPHead;
	struct ICMPPacketHead	*pICMPHead;
	struct UDPPacketHead	*pUDPHead;
	BYTE					*pdata = NULL;

	/*---------------------------------------------------------------------*/

    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE)	; // Force to make the queue
 	pDlg->m_threadID = GetCurrentThreadId() ;
	
	while( TRUE )
	{
        if( PeekMessage( &msg , 0 , WM_CLOSE,WM_CLOSE,PM_NOREMOVE ) )
		{
 	        closesocket( pDlg->m_s ) ;
 	        pDlg->m_threadID = 0 ;
			pDlg->m_start.EnableWindow(TRUE) ; 
			break ;
		}
		memset( buf , 0 , sizeof(buf) ) ;
		iRet = 	recv( pDlg->m_s , buf , sizeof( buf ) , 0 ) ;
		if( iRet == SOCKET_ERROR )
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error recv() = %ld " , dwErr ) ;
			continue ;
		}
		else
			if( *buf )
			{	
				bufwork   = buf ;
        		pIpHeader = (IPHEADER *)bufwork ;
	            WORD iLen = ntohs(pIpHeader->total_len) ;
				while( TRUE )
				{
					if( iLen <= iRet )
					{
 							ina.S_un.S_addr = pIpHeader->sourceIP ;
							pSource = inet_ntoa( ina ) ;
							strcpy( szSource , pSource ) ;
							ina.S_un.S_addr = pIpHeader->destIP ;
							pDest = inet_ntoa( ina ) ;
							strcpy( szDest , pDest ) ;
							CString str, strProto, strSourPort, strDestPort, strData, strSize;


							strProto = get_proto_name( pIpHeader->proto );
							
							/*-------------------zhuwei add(2002.11.9)-----------------------------*/
							HdrLen = pIpHeader->header_len&0xf;
							HdrLen *= 4;
							totallen = ntohs(pIpHeader->total_len);
							totallen-=HdrLen;
							switch(pIpHeader->proto)
							{
							case IPPROTO_ICMP:
								{
									pICMPHead=(struct ICMPPacketHead *)(buf+HdrLen);
									//strL4.Format(" type:%d code:%d\n",pICMPHead->Type,pICMPHead->Code);
									strSourPort = "-";
									strDestPort = "-";
									pdata=((BYTE *)pICMPHead)+ICMP_HEAD_LEN;
									totallen -= ICMP_HEAD_LEN;
									break;
								}
							case IPPROTO_TCP:
								{
									pTCPHead=(struct TCPPacketHead *)(buf+HdrLen);
									sourport = ntohs(pTCPHead->SourPort);
									destport = ntohs(pTCPHead->DestPort);
									//strL4.Format(" sour port:%d,dest port:%d",sourport,destport);
									strSourPort.Format("%d",sourport);
									strDestPort.Format("%d",destport);
									HdrLen = (pTCPHead->HLen)>>4;  //in fact only 4 bits
									HdrLen *= 4;
									pdata=((BYTE *)pTCPHead)+HdrLen;
									totallen -= HdrLen;
									break;
								}
							case IPPROTO_UDP:
								{
									pUDPHead=(struct UDPPacketHead *)(buf+HdrLen);
									sourport = ntohs(pUDPHead->SourPort);
									destport = ntohs(pUDPHead->DestPort);
									//strL4.Format(" sour port:%d,dest port:%d",sourport,destport);
									strSourPort.Format("%d",sourport);
									strDestPort.Format("%d",destport);
									pdata=((BYTE *)pUDPHead)+UDP_HEAD_LEN;
									totallen -= UDP_HEAD_LEN;
									break;
								}
							}

							if(pIpHeader->proto == IPPROTO_ICMP)
								strData.Format("type:%d code:%d data:%s",pICMPHead->Type,pICMPHead->Code,pdata);
							else strData.Format("  %s",pdata);

							strSize.Format("%d",totallen);
							pDlg->AddData(strProto,szSource,strSourPort,szDest,strDestPort,strSize,strData);
						
						if( iLen < iRet )
						{
							iRet -= iLen ;
							bufwork  += iLen ;
        					pIpHeader = (IPHEADER *)bufwork ;
						}
						else
							break ; // pIpHeader->total_len == iRet and go out
					}
					else
					{ // read last part of buf. I wrote it , but always recv() read exactly 
                      // the lenght of the packet
						int iLast = iLen - iRet ;
						pLastBuf = new char [ iLen ] ;
						int iReaden = iRet ;
						memcpy( pLastBuf , bufwork , iReaden ) ;
						iRet = 	recv( pDlg->m_s , pLastBuf + iReaden , iLast , 0 ) ;
						if( iRet == SOCKET_ERROR )
						{
							dwErr = WSAGetLastError() ;
							sprintf( szErr , "Error recv() = %ld " , dwErr ) ;
							break ;
						}
						else
						{
							bufwork = pLastBuf ;	
       						pIpHeader = (IPHEADER *)bufwork ;
							if( iRet == iLast )
								iRet = iLen ;
							else
							{ // read all last data
								iReaden += iRet ;
								iLast -= iRet ;
								while( TRUE )
								{
									iRet = recv( pDlg->m_s , pLastBuf +iReaden , iLast , 0 ) ;
									if( iRet == SOCKET_ERROR )
									{
										dwErr = WSAGetLastError() ;
										sprintf( szErr , "Error recv() = %ld " , dwErr ) ;
										break ;
									}
									else
									{
								        iReaden += iRet ;
								        iLast -= iRet ;
									    if( iLast <= 0 ) 
										    break ;
									}	
								} // while
							}
						}
					}	
				}   // while
				if( pLastBuf )
					delete [ ] pLastBuf ;
			}
			else
			{
				AfxMessageBox( "No data on network" ) ;
				continue ;
			}
	}

	return TRUE ;
}

void CIpmonDlg::OnLookUp() 
{
	// TODO: Add your control notification handler code here
	char        szErr [ 50 ] , szHostName[MAX_PATH];
	DWORD       dwErr ;
    SOCKADDR_IN sa;

	gethostname(szHostName, sizeof(szHostName)) ;
	m_iphostsource = m_ipsource ;
    

 	m_ipcheckedhost = ntohl(m_iphost) ;


		if( 0 == m_threadID )
		{
			SetDlgItemText(IDC_LOOKUP,"停止查看!" );
		}
		else
		{
			if( m_threadID )
			{
 	            PostThreadMessage(m_threadID,WM_CLOSE,0,0) ;
		        SetDlgItemText(IDC_LOOKUP,"开始查看!");
				m_start.EnableWindow(FALSE) ;
			}
			return ;
		}
    	DWORD dwBufferLen[10] ;
		DWORD dwBufferInLen= 1 ;
		DWORD dwBytesReturned = 0 ;
		m_s = socket( AF_INET , SOCK_RAW , IPPROTO_IP ) ;
		if( INVALID_SOCKET == m_s )
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error socket() = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;

		}
		int rcvtimeo = 5000 ; 
        if( setsockopt( m_s , SOL_SOCKET , SO_RCVTIMEO , (const char *)&rcvtimeo , sizeof(rcvtimeo) ) == SOCKET_ERROR)
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error WSAIoctl = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;
		}
		sa.sin_family = AF_INET;
 		sa.sin_port = htons(7000);
		sa.sin_addr.s_addr= m_iphostsource;
        if (bind(m_s,(PSOCKADDR)&sa, sizeof(sa)) == SOCKET_ERROR)
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error bind() = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;
		} 
        if( SOCKET_ERROR != WSAIoctl( m_s, SIO_RCVALL , &dwBufferInLen, sizeof(dwBufferInLen),             
                                      &dwBufferLen, sizeof(dwBufferLen),
									  &dwBytesReturned , NULL , NULL ) )
            AfxBeginThread( threadFunc , (LPVOID)this );
		else
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error WSAIoctl = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;
		}
}




void CIpmonDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( NULL != m_threadID )
 	    PostThreadMessage(m_threadID,WM_CLOSE,0,0) ;
	if( m_IPArr.GetSize() )
	    m_IPArr.RemoveAll() ;
	CDialog::OnOK();
}

void CIpmonDlg::AddData(CString s0,CString s1, CString s2, CString s3, CString s4, CString s5, CString s6)
{
	int index;

	index = m_ctrList.InsertItem(0,s0);
	m_ctrList.SetItem(index,1,LVIF_TEXT,s1, 0, 0, 0,0);
	m_ctrList.SetItem(index,2,LVIF_TEXT,s2, 0, 0, 0,0);
	m_ctrList.SetItem(index,3,LVIF_TEXT,s3, 0, 0, 0,0);
	m_ctrList.SetItem(index,4,LVIF_TEXT,s4, 0, 0, 0,0);
	m_ctrList.SetItem(index,5,LVIF_TEXT,s5, 0, 0, 0,0);
	m_ctrList.SetItem(index,6,LVIF_TEXT,s6, 0, 0, 0,0);
}




