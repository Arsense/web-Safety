// EWDPingDlg.cpp : implementation file
//

//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EWDPing.h"
#include "EWDPingDlg.h"
#include "PingOptionsDlg.h"

//////////////////////////////////////////////////////////////////////////////

#include "RegKey.h"
#include "SysTCPIP.h"

//////////////////////////////////////////////////////////////////////////////

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
// CEWDPingDlg dialog

CEWDPingDlg::CEWDPingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEWDPingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEWDPingDlg)
	m_Host = _T("");
	m_LocalHost = _T("");
	m_NameServer = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEWDPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEWDPingDlg)
	DDX_Control(pDX, IDC_TraceList, m_TraceList);
	DDX_Text(pDX, IDC_DEST, m_Host);
	DDX_Text(pDX, IDC_LocalHost, m_LocalHost);
	DDX_Text(pDX, IDC_NameServer, m_NameServer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEWDPingDlg, CDialog)
	//{{AFX_MSG_MAP(CEWDPingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDQUIT, OnQuit)
	ON_BN_CLICKED(IDC_PingButton, OnPingButton)
	ON_BN_CLICKED(IDC_TraceButton, OnTraceButton)
	ON_BN_CLICKED(IDC_OptionsButton, OnOptionsButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ClearButton, OnClearButton)
	ON_MESSAGE(WSA_PING_ASYNC, OnPINGAsynch)
	ON_BN_CLICKED(IDC_Stop, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEWDPingDlg message handlers

//////////////////////////////////////////////////////////////////////////
//																		//
//						MESSAGE INTERRUPT PROCESSORS					//
//																		//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
//										//
//		Initialize main dialog			//
//										//
//////////////////////////////////////////

//对话框初始化
BOOL CEWDPingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	PingSocket.icmpSocket = INVALID_SOCKET;
	PingSent = FALSE;

	FetchWinsockSettings();			// 获得机器名、域名的属性.

	if (!InitSockets())				// 如果初始化失败则退出
		CDialog::OnCancel();

	InitImageList();				// 设定列表框图标

	pIcmpBuffer = (LPSTR)icmpBuffer;// 设定缓冲区

	return TRUE; 
}

//////////////////////////////////////////////////
//												//
//		Control menu and max/min processor		//
//												//
//////////////////////////////////////////////////
void CEWDPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//////////////////////////////////////////
//										//
//			Repaint window				//
//										//
//////////////////////////////////////////
void CEWDPingDlg::OnPaint() 
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

//////////////////////////////////////////
//										//
//		Called when icon is moving		//
//										//
//////////////////////////////////////////
HCURSOR CEWDPingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////
//													//
//		PING Asynchronous Message Processor			//
//													//
//	Responds to received ICMP messages				//
//		Updates the Trace window with the round		//
//		trip time and sequence,						//
//		Stops the timer,							//
//		If not TRACING, exits.						//
//													//
//		If rcvSockAddr = TraceTarget, displays		//
//			host reached and exits.					//
//		If icmpMaxHops exceeded, displays host not	//
//			reached and exits. (icmpMaxHops is set	//
//			in EWDPing Options Dialog).				//
//		Displays next animated icon picture,		//
//			increments icmpHops and requests next	//
//			PING									//
//													//
//////////////////////////////////////////////////////
LRESULT CEWDPingDlg::OnPINGAsynch(WPARAM wParam, LPARAM lParam)
{

	int WSAEvent = WSAGETSELECTEVENT(lParam);
	int WSAError = WSAGETSELECTERROR(lParam);

	if (WSAError)
	{
		CString ErrorMessage;
		ErrorMessage.Format("WSAAsynch Event # %u, Error: %u", 
							WSAEvent, 
							WSAError);
		MessageBox (ErrorMessage, 
				    "CEWDPingDlg::OnPINGAsynch", 
					MB_OK|MB_SYSTEMMODAL);
	}
	
	StopTimer();

	switch (WSAEvent)
	{
	case FD_READ:

		if (PingSocket.Receive(pIcmpBuffer, sizeof icmpBuffer) == SOCKET_ERROR)
		{
			if (!PingSent)
				return 0;

			PingSocket.DisplayError ("FD_READ", 
									 "CEWDPingDlg::OnWSAAsynch");
		}

		else
		{
			DWORD	PingRoundTrip = PingSocket.icmpPingReceivedAt - PingSocket.icmpPingSentAt;

			if (PingSocket.icmpRoundTripTime > PingRoundTrip)
				PingSocket.icmpRoundTripTime = PingRoundTrip;
			
			if (PingSent)
			{
				UpdateTrace();
			
				if (!icmpTracing)
				{
					m_TraceList.InsertItem (m_TraceList.GetItemCount(), 
											"", 
											Icon_BlueArrow);
					SetTraceFocus (m_TraceList.GetItemCount()-1,
								   0);
					DisplayTrace ("","","Host Reached");

				}
			}
		}

		PingSent = FALSE;
		
		if (icmpTracing)
		{
			PingSocket.icmpHops++;
			if ( (PingSocket.icmpHops >= PingSocket.icmpMaxHops) ||
				 (TraceTarget.s_addr == PingSocket.rcvSockAddr.sin_addr.s_addr ))
			{
				EndTrace();
				m_TraceList.InsertItem (m_TraceList.GetItemCount(), 
										"", 
										Icon_BlueArrow);
				SetTraceFocus (m_TraceList.GetItemCount()-1,
							   0);
				DisplayTrace ("","","HOST Reached");
			}
			else
			{
				ChangeIconState();
				SendPing();
			}
		}

		break;

	case FD_WRITE:
		break;

	default:
		break;

	}

	return 0;
}

//////////////////////////////////////////////////////
//													//
//					End Trace Operation				//
//													//
//		Set ICON to default, and force screen to	//
//		be updated.  Reset icmpTracing so next		//
//		operation will be PING.						//
//													//
//////////////////////////////////////////////////////
void CEWDPingDlg::EndTrace(void)
{
	CStatic *	pDlgWnd;

	m_hIcon = AfxGetApp()->LoadIcon(IDI_CommLink1);		

	pDlgWnd=(CStatic*)GetDlgItem(IDC_CommLink);
	pDlgWnd->SetIcon(m_hIcon);						

	if(IsIconic())
	{
		InvalidateRect(NULL,TRUE);
	}

	icmpTracing = FALSE;

}

//////////////////////////////////////////////////////////////////////////
//																		//
//								TIMER SECTION							//
//																		//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//															//
//				Process Timer Interrupt						//
//															//
//		If time-out period has not elapsed, increment		//
//		icmpIntervalCount, change the animated icon's state	//
//		and exit.											//
//															//
//		Otherwise, stop the timer, reset icon to default,	//
//		set receive address to null, and update the trace	//
//		display to show that the timer has expired.			//
//															//
//////////////////////////////////////////////////////////////

//计时器事件
void CEWDPingDlg::OnTimer(UINT id)
{

	CString		Message;
	CStatic *	pDlgWnd;
	//如果没有超时则不断改变图标状态，并将计时变量增加
	if (icmpIntervalCount < icmpMaxIntervals)
	{
		ChangeIconState ();
		icmpIntervalCount++;
		return;
	}

	StopTimer();
	PingSent = FALSE;

	m_hIcon = AfxGetApp()->LoadIcon(IDI_CommLink1);		

	pDlgWnd=(CStatic*)GetDlgItem(IDC_CommLink);
	pDlgWnd->SetIcon(m_hIcon);						

	if(IsIconic()) 
	{
		InvalidateRect(NULL,TRUE);
	}

	PingSocket.rcvSockAddr.sin_addr.s_addr = NULL;
	
	PingSocket.icmpRoundTripTime = PingSocket.icmpPingTimer * icmpMaxIntervals;
	UpdateTrace ();

	if (icmpTracing)
		SendPing();
}

//////////////////////////////////////////
//										//
//			Start the Timer				//
//										//
//	Start the timer and set TimerActive	//
//										//
//////////////////////////////////////////
void CEWDPingDlg::StartTimer()
{

	TimerActive = FALSE;
	
	icmpIntervalCount = 0;
	icmpMaxIntervals = (PingSocket.icmpPingTimer / MAX_INTERVAL_TIME);

	TimerNumber = SetTimer(IDT_TIMER1, MAX_INTERVAL_TIME, NULL);
	if (TimerNumber != 0)
		TimerActive = TRUE;
	
	return;

}

//////////////////////////////////////////////
//											//
//				Stop the Timer				//
//											//
//	Issue KillTimer and reset TimerActive	//
//											//
//////////////////////////////////////////////
void CEWDPingDlg::StopTimer()
{

	if (TimerActive)
		KillTimer (TimerNumber);
	TimerActive = FALSE;
}

//////////////////////////////////////////////////
//												//
//				Change Icon State				//
//												//
//		Select the next icon image to display	//
//		increment the IconState and invoke		//
//		icon display							//
//												//
//////////////////////////////////////////////////

void CEWDPingDlg::ChangeIconState(void)
{
	int			IconId;
	CStatic *	pDlgWnd;

	switch (IconState % 4)
	{
	case 0:
		IconId = IDI_CommLink2;
		break;
	case 1:
		IconId = IDI_CommLink3;
		break;
	case 2:
		IconId = IDI_CommLink4;
		break;
	case 3:
		IconId = IDI_CommLink3;
		break;
	default:
		IconId = IDI_CommLink1;
		break;
	}
	
	IconState++;

	m_hIcon = AfxGetApp()->LoadIcon(IconId);		

	pDlgWnd=(CStatic*)GetDlgItem(IDC_CommLink);
	pDlgWnd->SetIcon(m_hIcon);						

	if(IsIconic())
	{
		InvalidateRect(NULL,TRUE);
	}

}

//////////////////////////////////////////////////////////////////////////
//																		//
//							BUTTON HANDLING SECTION						//
//																		//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////
//											//
//			Issue PING Request				//
//											//
//	Set IconState to default, Set icmpTTL	//
//	to MaxHops, reset icmpTracing, and		//
//	issue IcmpPing request.					//
//											//
//////////////////////////////////////////////
//PING按钮处理程序
void CEWDPingDlg::OnPingButton() 
{
	IconState = 0;
	//列表框插入一条空记录
	DisplayBlankLine ();
	
	icmpTracing = FALSE;
	icmpPingTTL = PingSocket.icmpMaxHops;

	PingSocket.icmpCurSeq = 0;
	//调用SENDPING函数发送PING 命令
	SendPing ( );
}

//////////////////////////////////////////
//										//
//			Trace Route Request			//
//										//
//	Set icmpPingTTL to 0, icmpHops to	//
//	0, set default icon and output		//
//	message about the target, then		//
//	issue a PING with icmpTracing set	//
//	to TRUE to start tracing.			//
//										//
//////////////////////////////////////////

//单击"trace route"按钮处理程序
void CEWDPingDlg::OnTraceButton() 
{
	//列表框插入一个空白项
	DisplayBlankLine ();
	//设定TTL为0
	icmpPingTTL = 0;
	PingSocket.icmpHops = 0;
	//图标状态为0
	IconState = 0;

	PingSocket.icmpCurSeq = 0;
	//设定地址
	TraceTarget.s_addr = HostIPAddress();
	CString TComment;
	TComment.Format("Trace Target = %s",
				    inet_ntoa(TraceTarget));
	//活动TRACE的信息
	TraceComment (TComment);

	icmpTracing = TRUE;
	//调用SendPing函数
	SendPing ( );
}

//////////////////////////////////////////
//										//
//			Stop Current Ping			//
//										//
//////////////////////////////////////////
void CEWDPingDlg::OnStop() 
{

	if (!PingSent)
		return;
	
	PingSent = FALSE;

	StopTimer();
	EndTrace();

	m_TraceList.InsertItem (m_TraceList.GetItemCount(), 
							"", 
							Icon_BlueArrow);
	SetTraceFocus (m_TraceList.GetItemCount()-1,
				   0);
	DisplayTrace ("","","Operation CANCELLED");

}

//////////////////////////////////////////
//										//
//			Clear Display List			//
//										//
//	Delete all items from ListCtrl box	//
//										//
//////////////////////////////////////////
void CEWDPingDlg::OnClearButton() 
{
	if (!PingSent)
 		m_TraceList.DeleteAllItems();
	
}

//////////////////////////////////////////
//										//
//			Set operating options		//
//										//
//	Copy current option settings to		//
//	interchange variables, invoke		//
//  options dialog.						//
//										//
//	Exit dialog if option dialog was	//
//	cancelled.							//
//										//
//	Update current option variables and //
//	save in the Registry.				//
//										//
//////////////////////////////////////////
void CEWDPingDlg::OnOptionsButton() 
{

	CPingOptionsDlg OptionsDlg;

	OptionsDlg.m_BufferSize = icmpDataLen;
	OptionsDlg.m_DefaultHost = DefHost;
	OptionsDlg.m_PingTimeout = PingSocket.icmpPingTimer;
	OptionsDlg.m_MaxHops = PingSocket.icmpMaxHops;

	int Result = OptionsDlg.DoModal();

	if (Result != IDOK)
		return;

	icmpDataLen = OptionsDlg.m_BufferSize;
	DefHost = OptionsDlg.m_DefaultHost;
	PingSocket.icmpPingTimer = OptionsDlg.m_PingTimeout;
	PingSocket.icmpMaxHops = OptionsDlg.m_MaxHops;

	SaveRegValues();
	
}

//////////////////////////////////////////
//										//
//		Shut down pinger and exit		//
//										//
//////////////////////////////////////////
void CEWDPingDlg::OnQuit() 
{

	StopTimer ();
	PingSocket.CloseIcmpSocket();
	CDialog::OnOK();

}

//////////////////////////////////////////////////////////////////////////
//																		//
//							SOCKET SUPPORT SECTION						//
//																		//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
//												//
//			Send a PING Message					//
//												//
//		Sends a PING message to the currently	//
//		selected host.							//
//												//
//		If icmpTracing is TRUE,					//
//			increment and set icmpPingTTL to	//
//			reach the next node in the Internet //
//			tree. Note: must timeout for a small//
//			period of time to allow TTL to		//
//			complete.							//
//												//
//		Start the timer and issue the Icmp Ping	//
//												//
//////////////////////////////////////////////////

//发送PING消息包到目的主机
void CEWDPingDlg::SendPing(void)
{

	PingSent = TRUE;
	PingSocket.icmpCurSeq++;
	PingSocket.icmpCurId = (USHORT)GetCurrentProcessId();

	PingSocket.icmpHostAddress = HostIPAddress();
	
	//如果目标地址为空，则返回
	if (PingSocket.icmpHostAddress == NULL)
		return;
	//如果icmpTracing为真，则必须将
	if (icmpTracing)
	{
		icmpPingTTL++;
	
    }
	//设定TTL
	if (PingSocket.SetTTL (icmpPingTTL) == SOCKET_ERROR)
	{
		PingSocket.DisplayError ("setsocket(TTL)",
								 "CEWDPingDlg::SendPing");
		return;
	}
	//要设定一定的时间让TTL设定完成
	Sleep (100);
	
	//显示序列号，同时用蓝色尖头表示出来
	SetTraceSequence (PingSocket.icmpCurSeq, 
					  m_TraceList.GetItemCount(), 
					  Icon_BlueArrow);

	//设定地址
	PingSocket.icmpSockAddr.sin_family = PF_INET;
	PingSocket.icmpSockAddr.sin_addr.s_addr = PingSocket.icmpHostAddress;
	PingSocket.icmpSockAddr.sin_port = 0;
	//开始计时
	StartTimer();
	//调用PING函数发出PING命令
	if (PingSocket.Ping (pIcmpBuffer, icmpDataLen) == SOCKET_ERROR)
		PingSocket.DisplayError("Ping", "CEWDPingDlg::SendPing");

}

//////////////////////////////////////////////////////
//													//
//					HostIPAddress					//
//													//
//		Return the IP Address for the curently		//
//		selected host in Internet byte order.		//
//													//
//		GetIPAddress leaves a copy of the address	//
//		in icmpSockAddr and sets HostName to its	//
//		DNS official name.							//
//													//
//////////////////////////////////////////////////////
unsigned long CEWDPingDlg::HostIPAddress()
{

	unsigned long		iHostAddress;

	GetDlgItemText (IDC_DEST, HostName, MAXHOSTNAME);

	iHostAddress = PingSocket.GetIPAddress ((LPSTR)&HostName);

	if ((iHostAddress == INADDR_ANY) || (iHostAddress == NULL))
	{
		PingSocket.DisplayError ("gethostbyname", 
								 "CEWDPingDlg::HostIPAddress");
		iHostAddress = NULL;
	}
	
	else
		
		SetDlgItemText (IDC_DEST, HostName);

	return iHostAddress;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//					CListCtrl and Image List Sections					//
//																		//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
//										//
//		Initialize Image Lists			//
//										//
//	The small image list is used, the	//
//	large is not, but is initialized	//
//	anyhow since they exist.			//
//										//
//////////////////////////////////////////
void CEWDPingDlg::InitImageList()
{

	//
	//		Create the image lists and add the icons
	//

	smallImageList.Create(16,16,FALSE,1,1);
	largeImageList.Create(32,32,FALSE,1,1);

	HICON TIcon;
	
	TIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_Blank));
	smallImageList.Add (TIcon);
	largeImageList.Add (TIcon);

	TIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_BlueArrow));
	smallImageList.Add (TIcon);
	largeImageList.Add (TIcon);

	//
	//		Assign the image lists to the list view
	//
	m_TraceList.SetImageList (&largeImageList, LVSIL_NORMAL);
	m_TraceList.SetImageList (&smallImageList, LVSIL_SMALL);
	
	//
	//		Initialize the columns and headings
	//
	AddListColumn (0, 
				   LVCFMT_RIGHT, 
				   m_TraceList.GetStringWidth("SEQXX")+5, 
				   "SEQ",
				   0);
	AddListColumn (1, 
				   LVCFMT_RIGHT, 
				   m_TraceList.GetStringWidth("TIMEXX")+5, 
				   "时间",
				   1);
	AddListColumn (2, 
				   LVCFMT_LEFT, 
				   m_TraceList.GetStringWidth("255.255.255.255.1")+10, 
				   "IP地址",
				   2);
	AddListColumn (3, 
				   LVCFMT_LEFT, 
				   m_TraceList.GetStringWidth("XXXXXXXXEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"), 
				   "信息", 
				   3);

}

//////////////////////////////////////////////
//											//
//		Create Column Headings/Widths		//
//											//
//////////////////////////////////////////////
int CEWDPingDlg::AddListColumn(int lcolumn, int lfmt, int lmaxwidth, LPSTR ltext, int lsubitem)
{

	LV_COLUMN		lColumn;

	lColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lColumn.fmt = lfmt;
	lColumn.cx = lmaxwidth;
	lColumn.pszText = ltext;
	lColumn.cchTextMax = sizeof (ltext);
	lColumn.iSubItem = lsubitem;

	int colno = m_TraceList.InsertColumn (lcolumn, &lColumn);

	return colno;

}

//////////////////////////////////////////
//										//
//		Update Trace List Display		//
//										//
//////////////////////////////////////////
void CEWDPingDlg::UpdateTrace()
{

	CString				IPAddressMessage;
	CString				TripTimeMessage;
	CString				SeqMessage;
	CString				HostMessage;

	if (PingSocket.rcvSockAddr.sin_addr.s_addr == NULL)
	{
		IPAddressMessage = "***********";
		HostMessage.Format("**** No response (TTL = %ld)", 
							PingSocket.icmpCurSeq);
		TripTimeMessage = "*";
	}
	else
	{

		TripTimeMessage.Format("%ld", 
							   PingSocket.icmpRoundTripTime);
		IPAddressMessage.Format("%s", 
							    inet_ntoa(PingSocket.rcvSockAddr.sin_addr));
		HostMessage.Format("%s", 
						   inet_ntoa(PingSocket.rcvSockAddr.sin_addr));
	}

	SeqMessage.Format ("%ld", 
					   PingSocket.icmpCurSeq);
	DisplayTrace (TripTimeMessage,
				  IPAddressMessage, 
				  HostMessage);
}

//////////////////////////////////////////
//										//
//			Display Trace				//
//										//
//////////////////////////////////////////
void CEWDPingDlg::DisplayTrace(LPCSTR TripTimeMessage, LPCSTR IPAddressMessage, LPCSTR HostMessage)
{
	int				ItemNumber;

	ItemNumber = m_TraceList.GetItemCount();
	if (ItemNumber > 0)
		ItemNumber--;

	SetDisplayImage (ItemNumber, Icon_Blank);
	m_TraceList.SetItemText(ItemNumber, 1, TripTimeMessage);
	m_TraceList.SetItemText(ItemNumber, 2, IPAddressMessage);
	m_TraceList.SetItemText(ItemNumber, 3, HostMessage);

}

//////////////////////////////////////////
//										//
//			Set Display Image			//
//										//
//		Set the selected icon in the	//
//		trace list box and set the		//
//		focus to it.					//
//										//
//////////////////////////////////////////
void CEWDPingDlg::SetDisplayImage(int FocusItem, ImageType FocusImage)
{

	LV_ITEM			TraceItem;

	TraceItem.iItem = FocusItem;
	TraceItem.iSubItem = 0;
	TraceItem.mask = LVIF_IMAGE;
	TraceItem.iImage = FocusImage;
	m_TraceList.SetItem (&TraceItem);

}

//////////////////////////////////////////
//										//
//			Set Trace Focus				//
//										//
//		Set the focus to the TraceList	//
//		item and exit.					//
//										//
//////////////////////////////////////////
void CEWDPingDlg::SetTraceFocus(int FocusItem, int FocusSubItem)
{

	LV_ITEM			TraceItem;

	TraceItem.iItem = FocusItem;
	TraceItem.iSubItem = FocusSubItem;
	TraceItem.mask = LVIF_STATE;
	TraceItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	m_TraceList.SetItem (&TraceItem);

	m_TraceList.EnsureVisible (FocusItem, FALSE);

}

//////////////////////////////////////////
//										//
//			Set Trace Sequence			//
//										//
//	 Output the current sequence number	//
//	set focus, and exit.				//
//										//
//////////////////////////////////////////
void CEWDPingDlg::SetTraceSequence(int Seq, int FocusItem, ImageType FocusImage)
{

	CString				SeqMessage;

	SeqMessage.Format ("%ld", Seq);
	m_TraceList.InsertItem (FocusItem, SeqMessage, FocusImage);
	SetTraceFocus (FocusItem, 0);

}

//////////////////////////////////////////
//										//
//			Display Blank Line			//
//										//
//		Display a blank line and exit.	//
//										//
//////////////////////////////////////////
void CEWDPingDlg::DisplayBlankLine(void)
{
	int			FocusItem;

	FocusItem = m_TraceList.GetItemCount();
	if (FocusItem != 0)
	{
		m_TraceList.InsertItem (FocusItem, " ", Icon_Blank);
		SetTraceFocus (FocusItem, 0);
	}
		
}

//////////////////////////////////////////
//										//
//				Trace Comment			//
//										//
//	Output the comment at the current	//
//	display line, set focus and exit.	//
//										//
//////////////////////////////////////////

//插入TRACE信息
void CEWDPingDlg::TraceComment(CString Comment)
{

	m_TraceList.InsertItem (m_TraceList.GetItemCount(), 
							"", 
							Icon_BlueArrow);
	SetTraceFocus (m_TraceList.GetItemCount()-1,
				   0);
	DisplayTrace ("", "", Comment);

}
//////////////////////////////////////////////////////////////////////////
//																		//
//						REGISTRY LOAD AND SAVE SECTION					//
//																		//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//		Get a raw socket and set message processor		//
//			Return TRUE if Icmp object allocated		//
//														//
//////////////////////////////////////////////////////////

//初始化SOCKET
BOOL CEWDPingDlg::InitSockets()
{

	if (!PingSocket.OpenNewSocket(GetSafeHwnd(),
					              WSA_PING_ASYNC, 
					              FD_READ | FD_WRITE,
					              AF_INET,
					              SOCK_RAW,
					              IPPROTO_ICMP))
	{
		PingSocket.DisplayError ("WSA_PING_ASYNC",
								 "CEWDPingDlg::InitSockets");
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////
//														//
//		Get Windows Winsock settings from Registry		//
//														//
//////////////////////////////////////////////////////////

BOOL CEWDPingDlg::FetchWinsockSettings()
{
	SysTCPIP	STcpIp;

	if (!STcpIp.WinsockVersion(&wsaData))
	{
		MessageBox ("No valid winsock.dll detected",
					"CEWDPingDlg::OnInitDialog",
					MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

	if (!STcpIp.GetLocalHostName (&m_LocalHost))
	{
		gethostname(CurrentHostName, MAXHOSTNAME);
	}
	else
	{
		memcpy (CurrentHostName, m_LocalHost, m_LocalHost.GetLength());
		CurrentHostName[m_LocalHost.GetLength()] = 0;
	}
	m_LocalHost = CurrentHostName;

	if (!STcpIp.GetDomainName(&LocalDomainName))
		LocalDomainName = "";

	m_LocalHost += "." + LocalDomainName;
	memcpy (CurrentHostName, m_LocalHost, m_LocalHost.GetLength());
	CurrentHostName[m_LocalHost.GetLength()] = 0;

	if (!STcpIp.GetNSName (&LocalNameServer))
		LocalNameServer = "";

	m_NameServer = "Name Server: " + LocalNameServer;

	SetDlgItemText (IDC_LocalHost, m_LocalHost);
	SetDlgItemText (IDC_NameServer, m_NameServer);

	LoadRegValues();

	SetDlgItemText (IDC_DEST, HostName);

	return TRUE;

}

//////////////////////////////////////////
//										//
//		Load operating options			//
//										//
//////////////////////////////////////////
BOOL CEWDPingDlg::LoadRegValues(void)
{

	RegKey		hKey;
	DWORD		dwType;

	icmpDataLen = 64;
	memcpy (HostName, CurrentHostName, MAXHOSTNAME);
	PingSocket.icmpPingTimer = 3000;
	PingSocket.icmpMaxHops = 30;

	if (!hKey.GetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "BufferSize",
							   &icmpDataLen,
							   &dwType))
	{
		if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "BufferSize",
							   icmpDataLen,
							   REG_DWORD))
		{
			MessageBox ("Unable to access registry entry for ""BufferSize""", 
						"Registry Access Error", 
						MB_OK|MB_SYSTEMMODAL);
			return FALSE;
		}
	}

	if (!hKey.GetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "PingTimeout",
							   &PingSocket.icmpPingTimer,
							   &dwType))
	{
		if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "PingTimeout",
							   PingSocket.icmpPingTimer,
							   REG_DWORD))
		{
			MessageBox ("Unable to access registry entry for ""PingTimeout""", 
						"Registry Access Error", 
						MB_OK|MB_SYSTEMMODAL);
			return FALSE;
		}
	}

	if (!hKey.GetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "MaxHops",
							   &PingSocket.icmpMaxHops,
							   &dwType))
	{
		if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "MaxHops",
							   PingSocket.icmpMaxHops,
							   REG_DWORD))
		{
			MessageBox ("Unable to access registry entry for ""MaxHops""", 
						"Registry Access Error", 
						MB_OK|MB_SYSTEMMODAL);
			return FALSE;
		}
	}

	if (!hKey.GetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "DefaultHost",
							   &DefHost,
							   &dwType))
	{
		DefHost = HostName;
		if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "DefaultHost",
							   HostName,
							   REG_SZ))
		{
			MessageBox ("Unable to access registry entry for ""DefaultHost""", 
					    "Registry Access Error", 
						MB_OK|MB_SYSTEMMODAL);
			return FALSE;
		}
	}
	memcpy (HostName, DefHost, DefHost.GetLength());
	HostName[DefHost.GetLength()] = 0;

	return TRUE;

} // End LoadRegValues

//////////////////////////////////////////////
//											//
//		Save updated operating options		//
//											//
//////////////////////////////////////////////
void CEWDPingDlg::SaveRegValues()
{

	RegKey		hKey;

	if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "BufferSize",
							   icmpDataLen,
							   REG_DWORD))
	{
		MessageBox ("Unable to access registry entry for ""BufferSize""", 
					"Registry Access Error", 
					MB_OK|MB_SYSTEMMODAL);
	}

	if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "PingTimeout",
							   PingSocket.icmpPingTimer,
							   REG_DWORD))
	{
		MessageBox ("Unable to access registry entry for ""PingTimeout""", 
					"Registry Access Error", 
					MB_OK|MB_SYSTEMMODAL);
	}

	if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "MaxHops",
							   PingSocket.icmpMaxHops,
							   REG_DWORD))
	{
		MessageBox ("Unable to access registry entry for ""MaxHops""", 
					"Registry Access Error", 
					MB_OK|MB_SYSTEMMODAL);
	}

	if (!hKey.SetRegistryValue(HKEY_CURRENT_USER,
							   "SOFTWARE\\EarthWalk Designs\\EWDPing",
							   "DefaultHost",
							   DefHost,
							   REG_SZ))
	{
		MessageBox ("Unable to access registry entry for ""DefaultHost""", 
				    "Registry Access Error", 
					MB_OK|MB_SYSTEMMODAL);
	}

} // End SaveRegValues

