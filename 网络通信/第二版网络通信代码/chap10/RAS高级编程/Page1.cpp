///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////

// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "Afd.h"
#include "Page1.h"
#include "PhoneNumberDlg.h"
#include "RunProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAfdApp theApp;

static HRASCONN hRasConn = NULL;

VOID WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	TRACE("RasDialFunc\n");
	if (unMsg != WM_RASDIALEVENT) TRACE("Strange event!\n");
	theApp.PostDialMessage(WM_DIAL_FUNC, rasconnstate, dwError);
	TRACE("RasDialFunc - exit\n");
}

BOOL SelectLVItem(CListCtrl & list, int item)
{
	LV_ITEM lvi;
	ZeroMemory(&lvi, sizeof(lvi));
	lvi.iItem = item;
	lvi.mask = LVIF_STATE;
	if (!list.GetItem(&lvi)) return FALSE;
	lvi.state |= LVIS_FOCUSED | LVIS_SELECTED;
	lvi.stateMask |= LVIS_FOCUSED | LVIS_SELECTED;
	if (!list.SetItem(&lvi)) return FALSE;
	return list.EnsureVisible(item, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CPage1 property page

IMPLEMENT_DYNCREATE(CPage1, CPropertyPage)

CPage1::CPage1() : CPropertyPage(CPage1::IDD)
{
	//{{AFX_DATA_INIT(CPage1)
	m_StatusText = _T("");
	m_LastCallText = _T("");
	//}}AFX_DATA_INIT
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
	DDX_Control(pDX, IDC_LIST_PHONES, m_PhoneList);
	DDX_Control(pDX, IDC_PROGRESS_TIMEOUT, m_ProgressTimeout);
	DDX_Control(pDX, IDC_COMBO_PROVIDER, m_Providers);
	DDX_Text(pDX, IDC_STATUS, m_StatusText);
	DDX_Text(pDX, IDC_LAST_CALL, m_LastCallText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PROV_ADD, OnButtonProvAdd)
	ON_BN_CLICKED(IDC_BUTTON_PROV_EDIT, OnButtonProvEdit)
	ON_BN_CLICKED(IDC_BUTTON_PROV_DEL, OnButtonProvDel)
	ON_BN_CLICKED(IDC_BUTTON_TEL_ADD, OnButtonTelAdd)
	ON_BN_CLICKED(IDC_BUTTON_TEL_EDIT, OnButtonTelEdit)
	ON_BN_CLICKED(IDC_BUTTON_TEL_DEL, OnButtonTelDel)
	ON_BN_CLICKED(IDC_DIAL, OnDial)
	ON_CBN_SELCHANGE(IDC_COMBO_PROVIDER, OnSelchangeComboProvider)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PHONES, OnClickListPhones)
	ON_MESSAGE(WM_DIAL_NEXT_PHONE, OnDialNextPhone)
	ON_MESSAGE(WM_DIAL_FUNC, OnDialFunc)
//	ON_BN_CLICKED(IDC_BUTTON_DIAL, OnButtonDial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers
//初始化属性一
BOOL CPage1::InitPropertyPage()
{
	CString sss;
	sss.LoadString(IDS_LIST1_COL1);
	m_PhoneList.InsertColumn(0, sss, LVCFMT_LEFT, 100);
	sss.LoadString(IDS_LIST1_COL2);
	//向电话号码列表框插入初始化电话号码
	m_PhoneList.InsertColumn(1, sss, LVCFMT_LEFT, 180);
	DWORD dwStyle = m_PhoneList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_PhoneList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_ProgressTimeout.SetRange32(0, 100);
	m_ProgressTimeout.ShowWindow(SW_HIDE);
	//初始化一些变量
	bPhone = FALSE;
	bDialNow = FALSE;
	bInOnTimer = FALSE;
	bConnected = FALSE;
	bDialing = FALSE;
	bAuthenticationBegin = FALSE;
	bSpeedChecked = FALSE;
	//查询连接状态
	QueryConnections();
	//查询目前的拨号情况
	QueryPhones(sCurrentProvider);
	//设定按钮状态
   	SetButtonsState();
	UpdateData(FALSE);
	//设定定时器
	uTimer = SetTimer(1, 1000, NULL);
	CheckExternalConnect();
	return ParseArgs(theApp.m_lpCmdLine);
}

BOOL CPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	return TRUE;
}

void CPage1::OnDestroy() 
{
	KillTimer(uTimer);
	BOOL bTerminateWhenExit = atol(theApp.GetIniString(gchMain, gchEnableTerminateWhenExit, "1"));
	if (bTerminateWhenExit) HangUp(TRUE);
	CPropertyPage::OnDestroy();
}

void CPage1::OnTimer(UINT nIDEvent) 
{
	CPropertyPage::OnTimer(nIDEvent);
	// 如果定时器已经打开，则返回
	if (bInOnTimer) return;
	//否则设定为TRUE
	bInOnTimer = TRUE;
	// 判断是否正在拨号
	if (!bDialing)
	{
		if ((!bDialNow) && (!bConnected)) CheckExternalConnect();
		bInOnTimer = FALSE;
		return;
	}
	// 判断是否已经连接上
	if (bConnected)
	{
		CheckingConnected();
		bInOnTimer = FALSE;
		return;
	}
	if (bDialNow) CheckingDialing();
	else
	// 开始拨号
	StartDial();
	//定时器变量改为FALSE
	bInOnTimer = FALSE;
}

BOOL CPage1::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

//增加新的连接
void CPage1::OnButtonProvAdd() 
{
	RasCreatePhonebookEntry(GetSafeHwnd(), sCurrentProvider);
	QueryConnections();
}

//编辑已有连接
void CPage1::OnButtonProvEdit() 
{
	RasEditPhonebookEntry(GetSafeHwnd(), NULL, sCurrentProvider);
	QueryConnections();
}

//删除连接
void CPage1::OnButtonProvDel() 
{
	int i = AfxMessageBox(IDS_DELETE_CONNECTION, MB_YESNO | MB_ICONQUESTION);
	if (i != IDYES) return;
	//结构体
	typedef DWORD (WINAPI* RDE)
	(
		LPCTSTR lpszPhonebook,  // pointer to full path and filename of 
		LPCTSTR lpszEntry	   // pointer to an entry name to delete
	);
	// 载入动态库
	HINSTANCE hLib = LoadLibrary(_T("RASAPI32.DLL"));
	if (NULL == hLib) return;
	RDE rde = (RDE) GetProcAddress(hLib, "RasDeleteEntryA");
	if (rde != NULL)
	{
		rde(NULL, sCurrentProvider);
		QueryConnections();
		QueryPhones(sCurrentProvider);
	}
	::FreeLibrary(hLib);
}

//增加拨号的号码
void CPage1::OnButtonTelAdd() 
{
	CPhoneNumberDlg dlg;
	int nResponse = dlg.DoModal();
	if (nResponse != IDOK) return;
	UINT pn = atoi(theApp.GetIniString(sCurrentProvider, "Phones", "0"));
	pn++;
	theApp.SetIniString(sCurrentProvider, "Phones", L2S(pn));
	theApp.SetIniString(sCurrentProvider,"Phone" + L2S(pn), dlg.m_sNumber);
	theApp.SetIniString(sCurrentProvider,"Comment" + L2S(pn), dlg.m_sComments);
	theApp.SetIniString(sCurrentProvider,"LastPhone", L2S(pn));
	QueryPhones(sCurrentProvider);
}

void CPage1::OnButtonTelEdit() 
{
	CPhoneNumberDlg dlg;
	dlg.m_sNumber = GetPhoneListSelection();
	UINT lph = atoi(theApp.GetIniString(sCurrentProvider, "LastPhone", "1"));
	dlg.m_sComments = theApp.GetIniString(sCurrentProvider, "Comment" + L2S(lph), "");
	int nResponse = dlg.DoModal();
	if (nResponse != IDOK) return;
	UINT pn = atoi(theApp.GetIniString(sCurrentProvider, "Phones", "0"));
	CString sss;
	for (UINT i = 0; i < pn; i++)
	{
		sss = theApp.GetIniString(sCurrentProvider, "Phone" + L2S(i + 1), "-");
		if (sss != GetPhoneListSelection()) continue;
		sss = dlg.m_sNumber;
		theApp.SetIniString(sCurrentProvider, "Phone" + L2S(i + 1), sss);
		sss = dlg.m_sComments;
		theApp.SetIniString(sCurrentProvider, "Comment" + L2S(i + 1), sss);
		break;
	}
	QueryPhones(sCurrentProvider);
}

void CPage1::OnButtonTelDel() 
{
	int j = AfxMessageBox(IDS_DELETE_PHONE, MB_YESNO | MB_ICONQUESTION);
	if (j != IDYES) return;
	UINT pn = atoi(theApp.GetIniString(sCurrentProvider, "Phones", "0"));
	UINT lph = atoi(theApp.GetIniString(sCurrentProvider, "LastPhone", "1"));
	CString sss;
	for (UINT i = 0; i < pn; i++)
	{
		sss = theApp.GetIniString(sCurrentProvider, "Phone" + L2S(i + 1), "-");
		if (sss != GetPhoneListSelection()) continue;
		sss = theApp.GetIniString(sCurrentProvider, "Phone" + L2S(pn), "-");
		theApp.SetIniString(sCurrentProvider,"Phone" + L2S(i + 1), sss);
		pn--;
		if (lph > pn) lph = pn;
		theApp.SetIniString(sCurrentProvider, "Phones", L2S(pn));
		theApp.SetIniString(sCurrentProvider, "LastPhone", L2S(lph));
		break;
	}
	QueryPhones(sCurrentProvider);
}

//拨号
void CPage1::OnDial() 
{
	if (!bDialing)
	{
		bDialing = TRUE;
		theApp.LOG(CString("Dial:<" + sCurrentProvider) +
		CString("> using phone:<" + GetPhoneListSelection()) + ">.");
	}
	else
	{
		TRACE("OnHangup()\n");
		theApp.LOG("Hang Up button pressed.");
		bDialing = FALSE;
		HangUp(FALSE);
	}
   	SetButtonsState();
}

void CPage1::OnSelchangeComboProvider() 
{
	m_Providers.GetLBText(m_Providers.GetCurSel(), sCurrentProvider);
	m_StatusText = sCurrentProvider;
	theApp.SetIniString(gchMain, "LastEntry", sCurrentProvider);
	QueryPhones(sCurrentProvider);
	UpdateData(FALSE);
	theApp.NotifyProviderChanged();
}

void CPage1::OnClickListPhones(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetPhoneListSelection();
	*pResult = 0;
}

//查询连接
BOOL CPage1::QueryConnections()
{
	DWORD cbBuf;
	DWORD cEntry;
	UINT  ndx;
	char  szMessage[256];
	m_Providers.ResetContent();
	sCurrentProvider = theApp.GetIniString(gchMain, "LastEntry", "");

	DWORD dwRet;
	LPBYTE lpBuffer;

	LPRASENTRYNAME lpRasEntry = new RASENTRYNAME;
	lpRasEntry->dwSize = sizeof(RASENTRYNAME);
	cbBuf = sizeof(RASENTRYNAME);
	//获得所有连接
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntry, &cbBuf, &cEntry);
	delete lpRasEntry;

	lpBuffer = new BYTE[cbBuf];
	lpRasEntry = (LPRASENTRYNAME)lpBuffer;
	lpRasEntry->dwSize = sizeof(RASENTRYNAME);
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntry, &cbBuf, &cEntry);


	if (ERROR_BUFFER_TOO_SMALL == dwRet)
	{
		delete lpRasEntry;
		lpRasEntry = NULL;
		lpRasEntry = (LPRASENTRYNAME) new char[cbBuf];
		if (NULL != lpRasEntry)
		{
			ZeroMemory(lpRasEntry, cbBuf);
			lpRasEntry->dwSize = cbBuf;
			dwRet = RasEnumEntries(NULL, NULL, lpRasEntry, &cbBuf, &cEntry);
		}
		else
			dwRet = ERROR_NOT_ENOUGH_MEMORY;
	}
	if (0 != dwRet) // 其他错误
	{
		if (0 != RasGetErrorString((UINT)dwRet, szMessage, 256))
		{
			CString sss; sss.LoadString(IDS_RASCS_25);
			wsprintf((LPSTR)szMessage, sss);
		}
		m_StatusText = szMessage;
		UpdateData(FALSE);
	}
	if (0 == cEntry)
	{
		delete lpRasEntry ;
		return FALSE;
	}
	BOOL bSet = FALSE;
	if (0 == dwRet)  // 没有错误
	{
		for (ndx = 0; ndx < cEntry; ndx++)
		{
			m_Providers.InsertString(ndx, lpRasEntry[ndx].szEntryName);
			if (lpRasEntry[ndx].szEntryName == sCurrentProvider)
			{
				m_Providers.SetCurSel(ndx);
				bSet = TRUE;
			}
		}
		if (!bSet)
		{
			m_Providers.SetCurSel(0);
			sCurrentProvider = lpRasEntry[0].szEntryName;
		}
	}
	delete lpRasEntry ;
	return TRUE;
}

//获得号码
BOOL CPage1::QueryPhones(CString sEntryName)
{
	m_PhoneList.DeleteAllItems();
	UINT pn = atoi(theApp.GetIniString(sEntryName, "Phones", "0"));
	if (1 == pn)
	{
		bPhone = LoadDefautPhoneNumber(sEntryName);
		pn = 1;
	}
	else
		bPhone = FALSE;
	if (!bPhone) return FALSE;
	CString sss;
	for (UINT i = 0; i < pn; i++)
	{
		sss = theApp.GetIniString(sEntryName, "Phone" + L2S(i + 1), "-");
		if (sss.IsEmpty()) continue;
		m_PhoneList.InsertItem(i, sss);
		sss = theApp.GetIniString(sEntryName, "Comment" + L2S(i + 1), "");
		if (!sss.IsEmpty())
		{
			LV_ITEM lvi;
			lvi.mask = LVIF_TEXT;
			lvi.iItem = i;
			lvi.iSubItem = 1;
			lvi.pszText = sss.GetBuffer(512);
			m_PhoneList.SetItem(&lvi);
			sss.ReleaseBuffer();
		}
	}
	UINT lpn = atoi(theApp.GetIniString(sEntryName, "LastPhone", "1"));
	if (lpn > pn) lpn = pn;
	SelectLVItem(m_PhoneList, lpn - 1);
	SetButtonsState();
	return TRUE;
}


BOOL CPage1::ParseArgs(CString sss)
{
	int i;
	CString aaa;
	BOOL bNeedReload = FALSE;
	aaa = sss;
	aaa.MakeUpper();
	i = aaa.Find("/?");
	if (i != -1)
	{
		AfxMessageBox(IDS_ARG_HELP);
	}
	aaa.MakeUpper();
	i = aaa.Find("/C\"");
	if (i != -1)
	{
		aaa = sss.Mid(i + 3, sss.GetLength());
		i = aaa.Find("\"");
		if (i > 0)
		{
			aaa = aaa.Mid(0, i);
			theApp.SetIniString(gchMain,"LastEntry", aaa);
			bNeedReload = TRUE;
		}
	}
	aaa = sss;
	aaa.MakeUpper();
	i = aaa.Find("/P");
	if (i != -1)
	{
		aaa = sss.Mid(i + 2, sss.GetLength());
		i = atol(aaa);
		if (i != 0)
		{
			theApp.SetIniString(
			sCurrentProvider, "LastPhone", L2S(i));
			bNeedReload = TRUE;
		}
	}
	if (bNeedReload)
	{
		QueryConnections();
		QueryPhones(sCurrentProvider);
	}
	CString s1, s2;
	aaa = sss;
	aaa.MakeUpper();
	i = aaa.Find("/D");
	if (i != -1) OnDial();
	return TRUE;
}

void CPage1::SetButtonsState()
{
	CString sss;
	GetDlgItem(IDC_BUTTON_TEL_ADD)->EnableWindow(!sCurrentProvider.IsEmpty());
	GetDlgItem(IDC_BUTTON_TEL_EDIT)->EnableWindow(bPhone);
	GetDlgItem(IDC_BUTTON_TEL_DEL)->EnableWindow(!bDialing && bPhone);
	GetDlgItem(IDC_BUTTON_PROV_DEL)->EnableWindow(!bDialing);
	UINT pn = atoi(theApp.GetIniString(sCurrentProvider, "Phones", "0"));
	if (pn <= 1) GetDlgItem(IDC_BUTTON_TEL_DEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PROVIDER)->EnableWindow(!bDialing);
	if (!bDialing && bPhone)
	sss.LoadString(IDS_DIAL);
	else
	sss.LoadString(IDS_HANGUP);
	GetDlgItem(IDC_DIAL)->SetWindowText(sss);
	m_ProgressTimeout.ShowWindow(SW_HIDE);
	BOOL m_bMaxWaitTime = atol(theApp.GetIniString(gchMain, gchEnableTimeout, "0"));
	if (m_bMaxWaitTime && bDialing && !bConnected)
	{
		m_ProgressTimeout.ShowWindow(SW_SHOW);
	}
	m_ProgressTimeout.SetPos(0);
}

//载入默认得电话号码
BOOL CPage1::LoadDefautPhoneNumber(CString sEntryName)
{
//
typedef DWORD (WINAPI* RGEPA)
(LPCTSTR lpszPhonebook,		// pointer to full path and filename of 
							// phone-book file
  LPCTSTR lpszEntry,		// pointer to an entry name
  LPRASENTRY lpRasEntry,	// buffer that receives entry information
  LPDWORD lpdwEntryInfoSize,	// size, in bytes, of the lpRasEntry 
								// buffer
  LPBYTE lpbDeviceInfo,			// buffer that receives device-specific 
								// configuration information
  LPDWORD lpdwDeviceInfoSize	// size, in bytes, of the 
								// lpbDeviceInfo buffer
);
// load library
	HINSTANCE hLib = LoadLibrary(_T("RASAPI32.DLL"));
	if (NULL == hLib) return FALSE;
	RGEPA rgepa = (RGEPA) GetProcAddress(hLib, "RasGetEntryPropertiesA");
	if (NULL == rgepa)
	{
		::FreeLibrary(hLib);
		return FALSE;
	}
	RASENTRY RasEntry;
	DWORD dw = sizeof(RASENTRY);
	ZeroMemory(&RasEntry, dw);
	RasEntry.dwSize = dw;
	DWORD ret = rgepa(NULL, sEntryName, &RasEntry, &dw, NULL, NULL);
	::FreeLibrary(hLib);
	if (ret != 0) return FALSE;
	CString phone;
	if (RasEntry.dwfOptions & RASEO_UseCountryAndAreaCodes)
	{
		phone = "8w";
		phone += RasEntry.szAreaCode;
		phone += RasEntry.szLocalPhoneNumber;
	}
	else
	phone = RasEntry.szLocalPhoneNumber;
	UINT pn = 1;
	theApp.SetIniString(sEntryName, "Phones", L2S(pn));
	theApp.SetIniString(sEntryName,"Phone" + L2S(pn), phone);
	return TRUE;
}

//挂断拨号
BOOL CPage1::HangUp(BOOL bExitApp)
{
	DWORD dwRet;
	RASCONNSTATUS rStatus;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(hRasConn, &rStatus);
	if ((dwRet != ERROR_INVALID_HANDLE) || (bDialNow))
	{
		bDialNow = FALSE;
		theApp.FinishSession();
		//挂断连接
		RasHangUp(hRasConn);
		dwRet = RasGetConnectStatus(hRasConn, &rStatus);
		m_StatusText.LoadString(IDS_TERMINATING_CONN);
		UpdateData(FALSE);
		//获得当前时间
		CTime t1 = CTime::GetCurrentTime();
		CTimeSpan ts;
		MSG msg;
		while (dwRet != ERROR_INVALID_HANDLE)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			dwRet = RasGetConnectStatus(hRasConn, &rStatus);
			ts = CTime::GetCurrentTime() - t1;
			if (!bExitApp)
			if (ts.GetTotalSeconds() > 3) break;
		}
		//设定日志
		theApp.LOG("HangUp.");
		m_StatusText.LoadString(IDS_PAUSE);
		UpdateData(FALSE);
		if (bConnected) theApp.RunProgramsOnEvent(RUN_HANGUP);
	}
	Sleep(500);
	m_StatusText.Empty();
	UpdateData(FALSE);
	bConnected = FALSE;
	return TRUE;
}

BOOL CPage1::PublicOnDial()
{
	OnDial();
	return TRUE;
}


LRESULT CPage1::OnDialFunc(WPARAM w, LPARAM l)
{

	char szMessage[256];

	m_StatusText = GetRasConnState((RASCONNSTATE) w);
	UpdateData(FALSE);
	if (l)  // 错误产生
	{
		if (RasGetErrorString((UINT) l, szMessage, 256) != 0)
		{
			CString sss; sss.LoadString(IDS_RASCS_25);
			wsprintf((LPSTR)szMessage, sss);
		}
		m_LastCallText = szMessage;
		TRACE(CString("External error ") + CString(szMessage) + "\n");
		HangUp(FALSE);
		theApp.LOG(m_LastCallText);
		OnDialNextPhone();
	}
	else if (RASCS_DONE & w)
	{
		bConnected = TRUE;
		m_StatusText.LoadString(IDS_ONLINE);
		theApp.LOG("On line.");
		theApp.StartSession();
		theApp.RunProgramsOnEvent(RUN_ONLINE);
		SetButtonsState();
	}
	UpdateData(FALSE);
	return 0;
}

//拨下一个号码
void CPage1::OnDialNextPhone()
{
	int nItems = m_PhoneList.GetItemCount();
	for (int i = 0; i < nItems; i++)
	{
		UINT state = m_PhoneList.GetItemState(i, LVIS_SELECTED);
		if (LVIS_SELECTED == state)
		{
				m_PhoneList.SetItemState(i, 0, LVIS_SELECTED);
				if (i + 1 < nItems) i++;
				else i = 0;
				SelectLVItem(m_PhoneList, i);
				GetPhoneListSelection();
				return;
		}
	}
	SelectLVItem(m_PhoneList, 0);
	GetPhoneListSelection();
}

//设置拨号名称
CString CPage1::GetPhoneListSelection()
{
	CString sss;
	for (int i = 0; i < m_PhoneList.GetItemCount(); i++)
	{
		if (LVIS_SELECTED == m_PhoneList.GetItemState(i, LVIS_SELECTED))
		{
				sss = m_PhoneList.GetItemText(i, 0);
				theApp.SetIniString(sCurrentProvider, "LastPhone", L2S(i + 1));
				return sss;
		}
	}
	SelectLVItem(m_PhoneList, 0);
	theApp.SetIniString(sCurrentProvider, "LastPhone", "1");
	return sss;
}

BOOL CPage1::GetActiveConnectionParam(CString & Conn, CString & Phon)
{
	RASCONN rcArray[100];
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	DWORD dw1 = sizeof(rcArray);
	DWORD dw2;
	ZeroMemory(rcArray, dw1);
	rcArray[0].dwSize = sizeof(RASCONN);
	dwRet = RasEnumConnections(rcArray, & dw1, & dw2);
//	ERROR_BUFFER_TOO_SMALL 
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	for (dw1 = 0; dw1 < dw2; dw1++)
	{
		dwRet = RasGetConnectStatus(rcArray[dw1].hrasconn, &rStatus);
		if (RASCS_Connected == rStatus.rasconnstate)
		{
			Conn = rcArray[dw1].szEntryName;
			hRasConn = rcArray[dw1].hrasconn;
			return TRUE;
		}
	}
	return FALSE;
}


//获得连接状态，根据不同情况，设定不同的状态
CString CPage1::GetRasConnState(RASCONNSTATE rasconn)
{
	CString sss;
	switch(rasconn)
	{
		case RASCS_OpenPort:
			sss.LoadString(IDS_RASCS_1); break;
		case RASCS_PortOpened:
			sss.LoadString(IDS_RASCS_2); break;
		case RASCS_ConnectDevice:
			sss.LoadString(IDS_RASCS_3); break;
		case RASCS_DeviceConnected:
			sss.LoadString(IDS_RASCS_4); break;
		case RASCS_AllDevicesConnected:
			sss.LoadString(IDS_RASCS_5); break;
		case RASCS_Authenticate:
			sss.LoadString(IDS_RASCS_6); break;
		case RASCS_AuthNotify:
			sss.LoadString(IDS_RASCS_7); break;
		case RASCS_AuthRetry:
			sss.LoadString(IDS_RASCS_8); break;
		case RASCS_AuthCallback:
			sss.LoadString(IDS_RASCS_9); break;
		case RASCS_AuthChangePassword:
			sss.LoadString(IDS_RASCS_10); break;
		case RASCS_AuthProject:
			sss.LoadString(IDS_RASCS_11); break;
		case RASCS_AuthLinkSpeed:
			sss.LoadString(IDS_RASCS_12); break;
		case RASCS_AuthAck:
			sss.LoadString(IDS_RASCS_13); break;
		case RASCS_ReAuthenticate:
			sss.LoadString(IDS_RASCS_14); break;
		case RASCS_Authenticated:
			sss.LoadString(IDS_RASCS_15); break;
		case RASCS_PrepareForCallback:
			sss.LoadString(IDS_RASCS_16); break;
		case RASCS_WaitForModemReset:
			sss.LoadString(IDS_RASCS_17); break;
		case RASCS_WaitForCallback:
			sss.LoadString(IDS_RASCS_18); break;
		case RASCS_Interactive:
			sss.LoadString(IDS_RASCS_19); break;
		case RASCS_RetryAuthentication:
			sss.LoadString(IDS_RASCS_20); break;
		case RASCS_CallbackSetByCaller:
			sss.LoadString(IDS_RASCS_21); break;
		case RASCS_PasswordExpired:
			sss.LoadString(IDS_RASCS_22); break;
		case RASCS_Connected:
			sss.LoadString(IDS_RASCS_23); break;
		case RASCS_Disconnected:
			sss.LoadString(IDS_RASCS_24); break;
		default:
			sss.LoadString(IDS_RASCS_25); break;
	}
	return sss;
}

CString CPage1::GetIPAddress()
{
	CString sss;
	if (!bConnected) return sss;
	RASPPPIP pppip;
	DWORD dw = sizeof(pppip);
	ZeroMemory(& pppip, dw);
	pppip.dwSize = dw;
	//通过RasGetProjectionInfo获得IP地址
	DWORD ret =	RasGetProjectionInfo(hRasConn, RASP_PppIp, & pppip, & dw);
	if (ERROR_SUCCESS == ret) sss = pppip.szServerIpAddress;
	return sss;
}

//检查扩展连接
BOOL CPage1::CheckExternalConnect()
{
	CString s1, s2;
	if (!GetActiveConnectionParam(s1, s2)) return FALSE;
	if (s1.IsEmpty())  return FALSE;
	sCurrentProvider = s1;
	theApp.SetIniString(gchMain,"LastEntry", sCurrentProvider);
	bDialing = TRUE;
	bDialNow = TRUE;
	bConnected = TRUE;
	QueryConnections();
	QueryPhones(sCurrentProvider);
	SetButtonsState();
	m_StatusText.LoadString(IDS_EXTERN_ONLINE);
	theApp.LOG("External On line.");
	UpdateData(FALSE);
	theApp.StartSession();
	theApp.RunProgramsOnEvent(RUN_ONLINE);
	return TRUE;
}

//开始拨号
BOOL CPage1::StartDial()
{
	bDialNow = TRUE;
	bAuthenticationBegin = FALSE;
	bSpeedChecked = FALSE;
	hRasConn = NULL;
	tBeginDial = CTime::GetCurrentTime();
	RASDIALPARAMS rdParams;
	char  szBuf[256] = "";
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, sCurrentProvider);
	int pd = atoi(theApp.GetIniString(gchMain, gchPulseDialing, "1"));
	if (1 == pd) lstrcpy(rdParams.szPhoneNumber, CString("P") + GetPhoneListSelection());
	else lstrcpy(rdParams.szPhoneNumber, GetPhoneListSelection());
	CString un, pw;
	un = theApp.GetIniString(sCurrentProvider, "UserName", "");
	pw = theApp.GetPassword(TRUE);
	lstrcpy(rdParams.szUserName, un);
	lstrcpy(rdParams.szPassword, pw);
	rdParams.szCallbackNumber[0] = '*';
	rdParams.szDomain[0] = '*';
	m_StatusText = CString("Dial:<" + sCurrentProvider) +
	CString("> Phone:<") + CString(rdParams.szPhoneNumber) +
	CString("> User Name:<" + un) + CString("> Password:<" + theApp.GetPassword()) + ">";
	theApp.LOG(m_StatusText);
	UpdateData(FALSE);
	Sleep(400);
	DWORD dwRet;
	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (LPVOID)RasDialFunc, &hRasConn);
	if (dwRet)
	{
		if (RasGetErrorString((UINT)dwRet, (LPSTR)szBuf, 256) != 0)
		m_LastCallText = szBuf;
		theApp.LOG(m_LastCallText);
		UpdateData(FALSE);
		HangUp(FALSE);
		OnDialNextPhone();
	}
	return TRUE;
}

//检查拨号
BOOL CPage1::CheckingDialing()
{
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(hRasConn, &rStatus);
	RASCONNSTATE rcs = rStatus.rasconnstate;
	if (!bConnected)
	{
		BOOL m_bMaxWaitTime = atol(theApp.GetIniString(gchMain, gchEnableTimeout, "0"));
		if (m_bMaxWaitTime)
		{
			CString sss = theApp.GetIniString(gchMain, gchMaxWaitTime, "60");
			CTimeSpan tst = CTime::GetCurrentTime() - tBeginDial;
			m_ProgressTimeout.SetPos(100 * tst.GetTotalSeconds() / atol(sss));
			if (tst.GetTotalSeconds() > atol(sss))
			{
				m_LastCallText.LoadString(IDS_TIME_OUT);
				theApp.LOG("Time out.");
				UpdateData(FALSE);
				HangUp(FALSE);
			}
		}
		if ((RASCS_Authenticate == rcs) && (FALSE == bAuthenticationBegin))
		{
			bAuthenticationBegin = TRUE;
			theApp.RunProgramsOnEvent(RUN_CONNECT);
		}
	}
	return TRUE;
}

BOOL CPage1::CheckingConnected()
{
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(hRasConn, &rStatus);
	RASCONNSTATE rcs = rStatus.rasconnstate;
	if (ERROR_INVALID_HANDLE == dwRet)
	{
		HangUp(FALSE);
		m_LastCallText.LoadString(IDS_LOST_OF_CARRIER);
		theApp.LOG("Lost of carrier.");
		UpdateData(FALSE);
		BOOL bbb = atol(theApp.GetIniString(gchMain, gchRedialWhenHangUp, "1"));
		if (!bbb) OnDial();
		TRACE("RedialWhenHangUp = FALSE\n");
		return TRUE;
	}
	if (!bSpeedChecked)
	{
		bSpeedChecked = TRUE;
		BOOL m_bMinSpeed = atol(theApp.GetIniString(gchMain, gchEnableMinSpeed, "0"));
		if (m_bMinSpeed)
		{
			DWORD dwBPS = atol(theApp.GetIniString(gchMain, gchMinSpeed, "2400"));
			DWORD dwMaxSpeed;
			dwMaxSpeed = theApp.pPerf[PERF_ConnectSpeed]->GetData();
			if (dwBPS > dwMaxSpeed)
			{
				CString sss = "Low speed disconnect. Min Speed <";
				sss += L2S(dwBPS) + "> Current Speed <";
				sss += L2S(dwMaxSpeed) + ">.";
				m_LastCallText = sss;
				theApp.LOG(sss);
				UpdateData(FALSE);
				HangUp(FALSE);
			}
		}
	}
	return TRUE;
}

void CPage1::OnOK() 
{
	return;	
	CPropertyPage::OnOK();
}

//void CPage1::OnButtonDial() 
//{
	// TODO: Add your control notification handler code here
//	StartDial();
	
//}
