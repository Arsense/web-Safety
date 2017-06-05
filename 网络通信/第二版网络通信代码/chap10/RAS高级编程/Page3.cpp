
// Page3.cpp : implementation file
//

#include "stdafx.h"
#include "Afd.h"
#include "Page3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAfdApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPage3 property page

IMPLEMENT_DYNCREATE(CPage3, CPropertyPage)

CPage3::CPage3() : CPropertyPage(CPage3::IDD)
{
	//{{AFX_DATA_INIT(CPage3)
	m_sLog = _T("");
	m_sPing = _T("");
	m_sSpeed = _T("");
	m_sTermKB = _T("");
	m_sTermTime = _T("");
	m_sTimeout = _T("");
	m_iTonePulse = -1;
	m_bHangUp = FALSE;
	m_bLog = FALSE;
	m_bPing = FALSE;
	m_bRedial = FALSE;
	m_bSpeed = FALSE;
	m_bTerm = FALSE;
	m_bTimeout = FALSE;
	m_bInterfaceStartMinimized = FALSE;
	m_bInterfaceMinimizeOnConnect = FALSE;
	//}}AFX_DATA_INIT
	dwTraffic = new DWORD[200 * sizeof(DWORD)];
	if (dwTraffic)
	{
		for (int i = 0; i < 200; i++) dwTraffic[i] = 0;
	}
	uTimer = UINT_MAX;
}

CPage3::~CPage3()
{
	if (dwTraffic) delete [] dwTraffic;
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3)
	DDX_Text(pDX, IDC_EDIT_LOG, m_sLog);
	DDX_Text(pDX, IDC_EDIT_PING, m_sPing);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_sSpeed);
	DDX_Text(pDX, IDC_EDIT_TERMINATE, m_sTermKB);
	DDX_Text(pDX, IDC_EDIT_TERMTIME, m_sTermTime);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_sTimeout);
	DDX_Radio(pDX, IDC_RADIO1, m_iTonePulse);
	DDX_Check(pDX, IDC_CHECK_HANGUP, m_bHangUp);
	DDX_Check(pDX, IDC_CHECK_LOG, m_bLog);
	DDX_Check(pDX, IDC_CHECK_PING, m_bPing);
	DDX_Check(pDX, IDC_CHECK_REDIAL, m_bRedial);
	DDX_Check(pDX, IDC_CHECK_SPEED, m_bSpeed);
	DDX_Check(pDX, IDC_CHECK_TERMINATE, m_bTerm);
	DDX_Check(pDX, IDC_CHECK_TIMEOUT, m_bTimeout);
	DDX_Check(pDX, IDC_CHECK_INTERFACE_START_MINIMIZED, m_bInterfaceStartMinimized);
	DDX_Check(pDX, IDC_CHECK_INTERFACE_START_MINIMIZED2, m_bInterfaceMinimizeOnConnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FILE_LOG, OnButtonFileLog)
	ON_BN_CLICKED(IDC_BUTTON_VEW_LOG, OnButtonVewLog)
	ON_BN_CLICKED(IDC_CHECK_HANGUP, OnCheckHangup)
	ON_BN_CLICKED(IDC_CHECK_LOG, OnCheckLog)
	ON_BN_CLICKED(IDC_CHECK_PING, OnCheckPing)
	ON_BN_CLICKED(IDC_CHECK_REDIAL, OnCheckRedial)
	ON_BN_CLICKED(IDC_CHECK_SPEED, OnCheckSpeed)
	ON_BN_CLICKED(IDC_CHECK_TERMINATE, OnCheckTerminate)
	ON_BN_CLICKED(IDC_CHECK_TIMEOUT, OnCheckTimeout)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_EN_KILLFOCUS(IDC_EDIT_LOG, OnKillfocusEditLog)
	ON_EN_KILLFOCUS(IDC_EDIT_PING, OnKillfocusEditPing)
	ON_EN_KILLFOCUS(IDC_EDIT_SPEED, OnKillfocusEditSpeed)
	ON_EN_KILLFOCUS(IDC_EDIT_TERMINATE, OnKillfocusEditTerminate)
	ON_EN_KILLFOCUS(IDC_EDIT_TERMTIME, OnKillfocusEditTermtime)
	ON_EN_KILLFOCUS(IDC_EDIT_TIMEOUT, OnKillfocusEditTimeout)
	ON_BN_CLICKED(IDC_CHECK_INTERFACE_START_MINIMIZED, OnCheckInterfaceStartMinimized)
	ON_BN_CLICKED(IDC_CHECK_INTERFACE_START_MINIMIZED2, OnCheckInterfaceStartMinimized2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3 message handlers
//³õÊ¼»¯ÊôÐÔÒ³
BOOL CPage3::InitPropertyPage()
{
	if (!dwTraffic) return FALSE;
	CString sModFileName;
	//»ñµÃÍêÕûµÄÂ·¾¶ºÍÎÄ¼þÃû
	GetModuleFileName(NULL, sModFileName.GetBuffer(MAX_PATH), MAX_PATH);
	sModFileName.ReleaseBuffer();
	sModFileName.MakeReverse();
	m_sLog = sModFileName.Right(sModFileName.GetLength() - sModFileName.Find('\\'));
	m_sLog.MakeReverse();
	m_sLog += _T("afd.log");
	UpdateData(FALSE);
	LoadAllFromIni();
	dwInitBytesRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
	dwInitBytesSent = theApp.pPerf[PERF_BytesXmit]->GetData();
	TimeBeginSession = CTime::GetCurrentTime();
	LastTrafficCheck = CTime::GetCurrentTime();
	LastPingCheck = CTime::GetCurrentTime();
	if (UINT_MAX == uTimer) uTimer = SetTimer(1, 1000 * 60, NULL);
	theApp.LOG("Program started.");
	return TRUE;
}

BOOL CPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return TRUE;
}

void CPage3::OnDestroy()
{
	KillTimer(uTimer);
	theApp.LOG("Program finished.");
	theApp.LOG("-----------------");
	CPropertyPage::OnDestroy();
}

//¶¨Ê±Æ÷ÊÂ¼þ
void CPage3::OnTimer(UINT nIDEvent)
{
	for (int i = 199; i > 0; i--) dwTraffic[i] = dwTraffic[i - 1];
	DWORD dwBytesSent = theApp.pPerf[PERF_BytesXmit]->GetData();
	dwBytesSent -= dwInitBytesSent;
	DWORD dwBytesRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
	dwBytesRecv -= dwInitBytesRecv;
	dwTraffic[0] = dwBytesSent + dwBytesRecv;
	BOOL bConnected = theApp.IsConnected();
	// 
	if (m_bPing && bConnected)
	{
		CTime ttt = CTime::GetCurrentTime();
		CTimeSpan ts = ttt - LastPingCheck;
		if (ts.GetTotalSeconds() >= atol(m_sPing) * 60)  // 
		{
			LastPingCheck = CTime::GetCurrentTime();
			PingISPServer();
		}
	}
	// 
	if (m_bTerm && bConnected)
	{
		CTime ttt = CTime::GetCurrentTime();
		CTimeSpan ts = ttt - LastTrafficCheck;
		if (ts.GetTotalSeconds() >= atol(m_sTermTime) * 60)
		{
			LastTrafficCheck = CTime::GetCurrentTime();
			DWORD dw = 0;
			int iTrafficTime = atol(m_sTermTime);
			if (iTrafficTime > 180) iTrafficTime = 180;
			for (i = 0; i < iTrafficTime; i++)
			dw += dwTraffic[i];
			DWORD dwTrafficKB = (DWORD) atol(m_sTermKB) * 1024;
			if (dw <= dwTrafficKB)
			{
				CString sss; sss.LoadString(IDS_DISCONNECT_LOW_TRAFFIC);
				theApp.SetLastCallText(sss);
				theApp.LOG("Disconnected due to low traffic.");
				TRACE("Disconnected due to low traffic.\n");
				theApp.PushBigButton();

			}
		}
	}
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CPage3::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

void CPage3::OnButtonFileLog()
{
	UpdateData();
	CString sss = m_sLog;
	if (sss.IsEmpty()) sss = "*.*";
	CFileDialog dlg(TRUE, NULL, sss,
	OFN_EXPLORER | OFN_OVERWRITEPROMPT, "*.*", NULL);
	int i = dlg.DoModal();
	if (i != IDOK) return;
	m_sLog = dlg.GetPathName();
	UpdateData(FALSE);
	SaveAllToIni();
}

void CPage3::OnButtonVewLog()
{
	UpdateData();
	if (!m_sLog.IsEmpty())
	ShellExecute(0,"open", m_sLog, NULL, NULL, SW_SHOW);
}

void CPage3::OnCheckHangup()
{
	SaveAllToIni();
}

void CPage3::OnCheckLog()
{
	SaveAllToIni();
}

void CPage3::OnCheckPing()
{
	SaveAllToIni();
//	ïåócòàíîâèòü íà÷àëüíûé ïåðèîä äëÿ ping'a
	LastPingCheck = CTime::GetCurrentTime();
}

void CPage3::OnCheckRedial()
{
	SaveAllToIni();
}

void CPage3::OnCheckSpeed()
{
	SaveAllToIni();
}

void CPage3::OnCheckTerminate()
{
	SaveAllToIni();
//	ïåócòàíîâèòü íà÷àëüíûé ïåðèîä äëÿ ïîäñ÷åòà òðàôôèêà
	LastTrafficCheck = CTime::GetCurrentTime();
}

void CPage3::OnCheckTimeout()
{
	SaveAllToIni();
}

void CPage3::OnCheckInterfaceStartMinimized() 
{
	SaveAllToIni();
}

void CPage3::OnRadio1()
{
	SaveAllToIni();
}

void CPage3::OnRadio2()
{
	SaveAllToIni();
}

void CPage3::OnKillfocusEditLog()
{
	SaveAllToIni();
}

void CPage3::OnKillfocusEditPing()
{
	UpdateData();
	long l = atol(m_sPing);
	if (l <= 0) m_sPing= "0";
	UpdateData(FALSE);
	SaveAllToIni();
}

void CPage3::OnKillfocusEditSpeed()
{
	UpdateData();
	long l = atol(m_sSpeed);
	if (l <= 0) m_sSpeed= "0";
	UpdateData(FALSE);
	SaveAllToIni();
}

void CPage3::OnKillfocusEditTerminate()
{
	UpdateData();
	long l = atol(m_sTermKB);
	if (l <= 0) m_sTermKB= "0";
	UpdateData(FALSE);
	SaveAllToIni();
}

void CPage3::OnKillfocusEditTermtime()
{
	UpdateData();
	long l = atol(m_sTermTime);
	if (l <= 1) m_sTermTime= "1";
	if (l > 180) m_sTermTime= "180";
	UpdateData(FALSE);
	SaveAllToIni();
}

void CPage3::OnKillfocusEditTimeout()
{
	UpdateData();
	long l = atol(m_sTimeout);
	if (l <= 1) m_sTimeout= "1";
	if (l > 300) m_sTimeout= "300";
	UpdateData(FALSE);
	SaveAllToIni();
}

BOOL CPage3::LoadAllFromIni()
{
	UpdateData();
	m_sLog		= theApp.GetIniString(gchMain, gchLogFile, m_sLog);
	m_sPing		= theApp.GetIniString(gchMain, gchPingTime, "60");
	m_sSpeed	= theApp.GetIniString(gchMain, gchMinSpeed, "2400");
	m_sTermKB	= theApp.GetIniString(gchMain, gchTermKB, "0");
	m_sTermTime	= theApp.GetIniString(gchMain, gchTermTime, "60");
	m_sTimeout	= theApp.GetIniString(gchMain, gchMaxWaitTime, "60");
	m_iTonePulse= atoi(theApp.GetIniString(gchMain, gchPulseDialing, "1"));
	m_bHangUp	= atol(theApp.GetIniString(gchMain, gchEnableTerminateWhenExit, "1"));
	m_bLog		= atoi(theApp.GetIniString(gchMain, gchEnableLogFile, "1"));
	m_bPing		= atol(theApp.GetIniString(gchMain, gchEnablePing, "0"));
	m_bRedial	= atol(theApp.GetIniString(gchMain, gchRedialWhenHangUp, "1"));
	m_bSpeed	= atol(theApp.GetIniString(gchMain, gchEnableMinSpeed, "0"));
	m_bTerm		= atol(theApp.GetIniString(gchMain, gchEnableTerminateLowSpeed, "0"));
	m_bTimeout	= atol(theApp.GetIniString(gchMain, gchEnableTimeout, "0"));
	m_bInterfaceStartMinimized = atol(theApp.GetIniString(gchMain, gchInterfaceStartMinimized, "0"));
	m_bInterfaceMinimizeOnConnect = atol(theApp.GetIniString(gchMain, gchInterfaceMinimizeOnConnect, "0"));
	
	UpdateData(FALSE);
	return TRUE;
}

BOOL CPage3::SaveAllToIni()
{
	UpdateData();
	theApp.SetIniString(gchMain, gchLogFile,				m_sLog);
	theApp.SetIniString(gchMain, gchPingTime,				m_sPing);
	theApp.SetIniString(gchMain, gchMinSpeed,				m_sSpeed);
	theApp.SetIniString(gchMain, gchTermKB,					m_sTermKB);
	theApp.SetIniString(gchMain, gchTermTime,				m_sTermTime);
	theApp.SetIniString(gchMain, gchMaxWaitTime,			m_sTimeout);
	theApp.SetIniString(gchMain, gchPulseDialing,			L2S(m_iTonePulse));
	theApp.SetIniString(gchMain, gchEnableTerminateWhenExit,L2S(m_bHangUp));
	theApp.SetIniString(gchMain, gchEnableLogFile,			L2S(m_bLog));
	theApp.SetIniString(gchMain, gchEnablePing,				L2S(m_bPing));
	theApp.SetIniString(gchMain, gchRedialWhenHangUp,		L2S(m_bRedial));
	theApp.SetIniString(gchMain, gchEnableMinSpeed,			L2S(m_bSpeed));
	theApp.SetIniString(gchMain, gchEnableTerminateLowSpeed,L2S(m_bTerm));
	theApp.SetIniString(gchMain, gchEnableTimeout,			L2S(m_bTimeout));
	theApp.SetIniString(gchMain, gchInterfaceStartMinimized,L2S(m_bInterfaceStartMinimized));
	theApp.SetIniString(gchMain, gchInterfaceMinimizeOnConnect, L2S(m_bInterfaceMinimizeOnConnect));
	UpdateData(FALSE);
	return TRUE;
}

BOOL CPage3::StartSession()
{
	dwInitBytesRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
	dwInitBytesSent = theApp.pPerf[PERF_BytesXmit]->GetData();
	TimeBeginSession = CTime::GetCurrentTime();
	LastTrafficCheck = CTime::GetCurrentTime();
	LastPingCheck = CTime::GetCurrentTime();
	// reset timer
	if (uTimer != UINT_MAX) KillTimer(uTimer);
	uTimer = SetTimer(1, 1000 * 60, NULL);
	// minimize
	if (m_bInterfaceMinimizeOnConnect)
	{
		theApp.pDlg->Hide();
	}
	return TRUE;
}

BOOL CPage3::FinishSession()
{
	return FALSE;
}

BOOL CPage3::PingISPServer()
{
	CString sss = theApp.GetCurrentIPAddress();
	if (sss.IsEmpty()) return FALSE;
	theApp.LOG("Ping ISP server " + sss);
	return PingIP(sss);
}

BOOL CPage3::PingIP(CString sss)
{
	CString aaa = sss;
	ULONG ul = 0;
	int m;
	int i = aaa.Find(".");
	if (i <= 0) return 0;
	m = atol(aaa.Mid(0, i));
	ul = m;
	aaa = aaa.Mid(i + 1, aaa.GetLength());
	i = aaa.Find(".");
	if (i <= 0) return 0;
	m = atol(aaa.Mid(0, i));
	ul = ul << 8;
	ul += m;
	aaa = aaa.Mid(i + 1, aaa.GetLength());
	i = aaa.Find(".");
	if (i <= 0) return 0;
	m = atol(aaa.Mid(0, i));
	ul = ul << 8;
	ul += m;
	aaa = aaa.Mid(i + 1, aaa.GetLength());
	m = atol(aaa.Mid(0, i));
	ul = ul << 8;
	ul += m;
	return PingIP(ul);
}

BOOL CPage3::PingIP(ULONG ulAddr)
{
//
typedef unsigned long   IPAddr;     // An IP address.
struct ip_option_information {
    unsigned char      Ttl;             // Time To Live
    unsigned char      Tos;             // Type Of Service
    unsigned char      Flags;           // IP header flags
    unsigned char      OptionsSize;     // Size in bytes of options data
    unsigned char FAR *OptionsData;     // Pointer to options data
}; /* ip_option_information */
struct icmp_echo_reply {
    IPAddr                         Address;         // Replying address
    unsigned long                  Status;          // Reply IP_STATUS
    unsigned long                  RoundTripTime;   // RTT in milliseconds
    unsigned short                 DataSize;        // Reply data size in byte
    unsigned short                 Reserved;        // Reserved for system use
    void FAR                      *Data;            // Pointer to the reply da
    struct ip_option_information   Options;         // Reply options
}; /* icmp_echo_reply */
typedef struct ip_option_information IP_OPTION_INFORMATION,
                                     FAR *PIP_OPTION_INFORMATION;
//
typedef DWORD (WINAPI* IcmpSendEchoXX)
(
HANDLE                   IcmpHandle,
IPAddr                   DestinationAddress,
LPVOID                   RequestData,
WORD                     RequestSize,
PIP_OPTION_INFORMATION   RequestOptions,
LPVOID                   ReplyBuffer,
DWORD                    ReplySize,
DWORD                    Timeout
);
typedef HANDLE (WINAPI* IcmpCreateFileXX)();
typedef BOOL (WINAPI* IcmpCloseHandleXX)(HANDLE IcmpHandle); 

	int iPingTimeout = 1000;

// load library
    HINSTANCE hLib = LoadLibrary(_T("ICMP.DLL"));
    if (NULL == hLib) return FALSE;
	IcmpSendEchoXX IcmpSendEchoFF = (IcmpSendEchoXX) GetProcAddress(hLib, "IcmpSendEcho");
	if (NULL == IcmpSendEchoFF) {::FreeLibrary(hLib);return FALSE;}
	IcmpCreateFileXX IcmpCreateFileFF = (IcmpCreateFileXX) GetProcAddress(hLib, "IcmpCreateFile");
	if (NULL == IcmpCreateFileFF) {::FreeLibrary(hLib);return FALSE;}
	IcmpCloseHandleXX IcmpCloseHandleFF = (IcmpCloseHandleXX) GetProcAddress(hLib, "IcmpCloseHandle");
	if (NULL == IcmpCloseHandleFF) {::FreeLibrary(hLib);return FALSE;}
	HANDLE icmphandle = IcmpCreateFileFF();
//
	char reply[sizeof(icmp_echo_reply)+8];
	icmp_echo_reply* iep=(icmp_echo_reply*)&reply;
	iep->RoundTripTime = 0xffffffff;
	DWORD dw = IcmpSendEchoFF(icmphandle, ulAddr, 0, 0, NULL, reply,
	sizeof(icmp_echo_reply)+8, iPingTimeout);
//	if (dw == 0) PrintError();
	IcmpCloseHandleFF(icmphandle);
//	return iep->RoundTripTime;
	::FreeLibrary(hLib);
	return TRUE;
}

void CPage3::OnCheckInterfaceStartMinimized2() 
{
	SaveAllToIni();
}
