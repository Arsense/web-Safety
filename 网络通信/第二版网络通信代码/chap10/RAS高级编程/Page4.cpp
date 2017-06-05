// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "Afd.h"
#include "Page4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAfdApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPage4 property page

IMPLEMENT_DYNCREATE(CPage4, CPropertyPage)

CPage4::CPage4() : CPropertyPage(CPage4::IDD)
{
	//{{AFX_DATA_INIT(CPage4)
	m_sRecv = _T("");
	m_sSent = _T("");
	m_sSpeed = _T("");
	m_sTimeCur = _T("");
	m_sTimeTot = _T("");
	m_sConnectionName = _T("");
	//}}AFX_DATA_INIT
	bWindowActivated = FALSE;
}

CPage4::~CPage4()
{
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage4)
	DDX_Control(pDX, IDC_STATIC_PANEL, m_PaneModem);
	DDX_Text(pDX, IDC_EDIT_RECV, m_sRecv);
	DDX_Text(pDX, IDC_EDIT_SENT, m_sSent);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_sSpeed);
	DDX_Text(pDX, IDC_EDIT_TIME_CUR, m_sTimeCur);
	DDX_Text(pDX, IDC_EDIT_TIME_TOT, m_sTimeTot);
	DDX_Text(pDX, IDC_STATIC_NAME, m_sConnectionName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CPage4)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CLEAN, OnClean)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage4 message handlers

//属性页初始化
BOOL CPage4::InitPropertyPage()
{
	//载入时间INI文件
	LoadTimeINI();
	//接收到的数据
	dwInitBytesRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
	//发送出去的数据
	dwInitBytesSent = theApp.pPerf[PERF_BytesXmit]->GetData();
    m_sRecv = _T("0");
    m_sSent = _T("0");
    m_sSpeed = _T("0");
	UpdateData(FALSE);
	//设定定时器
	uTimer = SetTimer(1, 1000, NULL);
	bWindowActivated = TRUE;
	return TRUE;
}

BOOL CPage4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	return TRUE;
}

void CPage4::OnDestroy() 
{
	KillTimer(uTimer);
	CPropertyPage::OnDestroy();
}

//定时器事件
void CPage4::OnTimer(UINT nIDEvent) 
{
	//获得数据
	DWORD sss = theApp.pPerf[PERF_BytesXmit]->GetData();
	//获得当前时刻发送的数据，用于计算当前的发送速度
	sss -= dwInitBytesSent;
	DWORD rrr = theApp.pPerf[PERF_BytesRecvd]->GetData();
	//获得当前时刻接收的数据，用于计算发送速度
	rrr -= dwInitBytesRecv;
    m_sRecv = L2S(rrr);
    m_sSent = L2S(sss);
    m_sSpeed = _T("0");
	//如果仍然连接
	if (theApp.IsConnected())
	{
		DWORD dw = theApp.pPerf[PERF_ConnectSpeed]->GetData();
		m_sSpeed = L2S(dw);
		CTimeSpan ts = CTime::GetCurrentTime() - TimeBeginSession;
		m_sTimeTot = ConvercTStoS(ParseSavedTime(sTimeTotalLast) + ts);
		m_sTimeCur = ConvercTStoS(ts);
		CString sss ="接收 " + m_sRecv + " / 发送 " + m_sSent + " / 速度 " + m_sSpeed;
		if (sss.GetLength() > 64) sss = sss.Left(64);
		theApp.pDlg->Tray.SetTooltipText(sss);
	}
	UpdateData(FALSE);
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CPage4::OnSetActive() 
{
	if (!bWindowActivated) return CPropertyPage::OnSetActive();
	if (!theApp.IsConnected())
	{
		LoadTimeINI();
		dwInitBytesRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
		dwInitBytesSent = theApp.pPerf[PERF_BytesXmit]->GetData();
	    m_sRecv = _T("0");
	    m_sSent = _T("0");
	    m_sSpeed = _T("0");
		m_sConnectionName = theApp.GetCurrentProviderName();
		UpdateData(FALSE);
	}
	return CPropertyPage::OnSetActive();
}

void CPage4::OnClean() 
{
	CTimeSpan ts(0);
	m_sTimeTot = ConvercTStoS(ts);
	m_sTimeCur = ConvercTStoS(ts);
	UpdateData(FALSE);
	SaveTimeINI();
}

CTimeSpan CPage4::ParseSavedTime(CString sss)
{
	CString aaa = sss;
	int i = aaa.Find(":");
	if (i <= 0) return CTimeSpan(0);
	int dd = atol(aaa.Mid(0, i));
	if (dd < 0 || dd > 25000) return CTimeSpan(0);
	aaa = aaa.Mid(i + 1, aaa.GetLength());
	i = aaa.Find(":");
	if (i <= 0) return CTimeSpan(0);
	int hh = atol(aaa.Mid(0, i));
	if (hh < 0 || hh > 23) return CTimeSpan(0);
	aaa = aaa.Mid(i + 1, aaa.GetLength());
	i = aaa.Find(":");
	if (i <= 0) return CTimeSpan(0);
	int mm = atol(aaa.Mid(0, i));
	if (mm < 0 || mm > 59) return CTimeSpan(0);
	aaa = aaa.Mid(i + 1, aaa.GetLength());
	int ss = atol(aaa);
	if (ss < 0 || ss > 59) return CTimeSpan(0);
	return CTimeSpan(dd, hh, mm, ss);
}

BOOL CPage4::LoadTimeINI()
{
	CString Provider = theApp.GetCurrentProviderName();
	CString sss = theApp.GetIniString(Provider, "TimeSession", "");
	m_sTimeCur = sss;
	sss = theApp.GetIniString(Provider, "TimeTotal", "");
	m_sTimeTot = sss;
	sTimeTotalLast = sss;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CPage4::SaveTimeINI()
{
	if (!bWindowActivated) return TRUE;
	UpdateData();
	CString Provider = theApp.GetCurrentProviderName();
    theApp.SetIniString(Provider,"TimeSession", m_sTimeCur);
    theApp.SetIniString(Provider,"TimeTotal", m_sTimeTot);
	return TRUE;
}

CString CPage4::ConvercTStoS(CTimeSpan ts)
{
	CString sss;
	sss.Format("%ld:%02ld:%02ld:%02ld",
	ts.GetDays(), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
	return sss;
}

BOOL CPage4::StartSession()
{
	TimeBeginSession = CTime::GetCurrentTime();
	DWORD dw = theApp.pPerf[PERF_ConnectSpeed]->GetData();
	m_sSpeed = L2S(dw);
	theApp.LOG("Connect " + m_sSpeed);
	return TRUE;
}

BOOL CPage4::FinishSession()
{
	CString sss = "Bytes sent:<" + m_sSent;
	sss += ">. Bytes received:<" + m_sRecv;
	CTimeSpan ts = CTime::GetCurrentTime() - TimeBeginSession;
	sss += ">. Session time:<" + ConvercTStoS(ts);
	sss += ">.";
	theApp.LOG(sss);
	SaveTimeINI();
	return TRUE;
}

BOOL CPage4::SetStatisticData()
{
	OnSetActive();
	return TRUE;
}
