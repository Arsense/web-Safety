// ClientSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>
#include "ServerSocket.h"
#include "ClientSocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int SOCK_TCP	= 0;
const int SOCK_UDP  = 1;

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg dialog


CClientSocketDlg::CClientSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientSocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientSocketDlg)
	m_strPort = _T("2000");
	m_nSockType = SOCK_TCP;	// 默认是 TCP
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CClientSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientSocketDlg)
	DDX_Control(pDX, IDC_TXT_MESSAGE, m_ctlMessage);
	DDX_Control(pDX, IDC_SVR_PORTINC, m_ctlPortInc);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_ctlMsgList);
	DDX_Control(pDX, IDC_IPADDRESS, m_ctlIPAddress);
	DDX_Text(pDX, IDC_SVR_PORT, m_strPort);
	DDX_Radio(pDX, IDC_TCP, m_nSockType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CClientSocketDlg)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg message handlers


//消息翻译
BOOL CClientSocketDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_ESCAPE)
			return TRUE;

		if (nVirtKey == VK_RETURN && (GetFocus()->m_hWnd  == m_ctlMessage.m_hWnd))
		{
			if (m_SocketManager.IsOpen())
				OnBtnSend();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CClientSocketDlg::OnInitDialog() 
{
	ASSERT( GetDlgItem(IDC_BTN_SEND) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_START) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_STOP) != NULL );
	ASSERT( GetDlgItem(IDC_TCP) != NULL );
	ASSERT( GetDlgItem(IDC_UDP) != NULL );

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
	m_ctlPortInc.SetRange32( 2000, 4500);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow( FALSE );
	GetDlgItem(IDC_BTN_STOP)->EnableWindow( FALSE );

	CString strLocal;
	m_SocketManager.GetLocalAddress( strLocal.GetBuffer(256), 256);
	strLocal.ReleaseBuffer();
	m_ctlIPAddress.SetWindowText( strLocal );

	// Initialize socket manager
	m_SocketManager.SetMessageWindow( &m_ctlMsgList );
	m_SocketManager.SetServerState( false );	// run as client
	m_SocketManager.SetSmartAddressing( false );	// always send to server
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//连接服务器
void CClientSocketDlg::OnBtnConnect() 
{
	UpdateData();

	CString strServer;
	//获得服务器地址
	m_ctlIPAddress.GetWindowText( strServer );
	bool bSuccess;
	//如果是TCP连接
	if (m_nSockType == SOCK_TCP)
	{
		bSuccess = m_SocketManager.ConnectTo( strServer, m_strPort, AF_INET, SOCK_STREAM); // TCP
	}
	else
	{
		bSuccess = m_SocketManager.ConnectTo( strServer, m_strPort, AF_INET, SOCK_DGRAM); // UDP
	}

	//建立线程
	if (bSuccess && m_SocketManager.WatchComm())
	{

		GetDlgItem(IDC_BTN_SEND)->EnableWindow( TRUE );
		GetDlgItem(IDC_BTN_STOP)->EnableWindow( TRUE );
		NextDlgCtrl();
		GetDlgItem(IDC_BTN_START)->EnableWindow( FALSE );
		GetDlgItem(IDC_TCP)->EnableWindow( FALSE );
		GetDlgItem(IDC_UDP)->EnableWindow( FALSE );
		CString strMsg;
	
		//如果是TCP协议
		if (m_nSockType == SOCK_TCP)
		{
			strMsg = _T("连接建立在服务器: ") + strServer;
			strMsg += _T("上，端口为 ") + m_strPort + CString("\r\n");
		}
		else
		{
			SockAddrIn addrin;
			m_SocketManager.GetSockName( addrin );	// 获得目前的地址
			LONG  uAddr = addrin.GetIPAddr();
			BYTE* sAddr = (BYTE*) &uAddr;
			short nPort = ntohs( addrin.GetPort() );
			CString strAddr;
			// IP地址保存成网络形式
			strAddr.Format(_T("IP: %u.%u.%u.%u, Port: %d"),
						(UINT)(sAddr[0]), (UINT)(sAddr[1]),
						(UINT)(sAddr[2]), (UINT)(sAddr[3]), nPort);
			strMsg = _T("Socket created: ") + strAddr + CString("\r\n");
		}

		m_SocketManager.GetPeerName( m_SockPeer );
		m_ctlMsgList.SetWindowText( strMsg );
	}
	
}

void CClientSocketDlg::OnBtnDisconnect() 
{
	//断开连接
	m_SocketManager.StopComm();
	if (!m_SocketManager.IsOpen())
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow( TRUE );
		PrevDlgCtrl();
		GetDlgItem(IDC_BTN_STOP)->EnableWindow( FALSE );
		GetDlgItem(IDC_TCP)->EnableWindow( TRUE );
		GetDlgItem(IDC_UDP)->EnableWindow( TRUE );
	}
}

//客户端发送信息
void CClientSocketDlg::OnBtnSend() 
{
	BYTE byBuffer[256] = { 0 };
	CString strText;

	// 更新数据
	UpdateData();
	//获得要发送的信息
	m_ctlMessage.GetWindowText( strText );
	int nLen = strText.GetLength();
	
	if (nLen > 0)
	{
		//加上回车符号
		strText += _T("\r\n");
		nLen = strText.GetLength() + 1;
		USES_CONVERSION;
		strcpy((LPSTR)byBuffer, T2CA(strText));
		//发送出去
		m_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
	}
}


void CClientSocketDlg::OnDestroy() 
{
	m_SocketManager.StopComm();

	CDialog::OnDestroy();

}

