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

// Page2.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include "Afd.h"
#include "Page2.h"
#include "RunProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAfdApp theApp;

BOOL strxor(char * key, char * str, size_t len)
{
	int k = 0;
	int l = strlen(key);
	for (size_t i = 0; i < len; i++)
	{
		str[i] = str[i] ^ key[k];
		k ++;
		if (k >= l) k = 0;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPage2 property page

IMPLEMENT_DYNCREATE(CPage2, CPropertyPage)

CPage2* CPage2::ziz = NULL;
CRITICAL_SECTION CPage2::sCriticalSection;

CPage2::CPage2() : CPropertyPage(CPage2::IDD)
{
	//{{AFX_DATA_INIT(CPage2)
	m_sUserName = _T("");
	m_sPassword = _T("");
	m_sSoundLogin = _T("");
	m_sSoundLogout = _T("");
	m_sConnectionName = _T("");
	m_bHidePassword = FALSE;
	//}}AFX_DATA_INIT
	ziz = this;
	bThreadExitImmediately = FALSE;
	iThreadWorkingNow = 0;
	InitializeCriticalSection(&sCriticalSection);
	bWindowActivated = FALSE;
}

CPage2::~CPage2()
{
	DeleteCriticalSection(&sCriticalSection);
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2)
//	DDX_Control(pDX, IDC_LIST_PROGS, m_listProgs);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_sUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_sPassword);
//	DDX_Text(pDX, IDC_EDIT_SOUND_LOGIN, m_sSoundLogin);
//	DDX_Text(pDX, IDC_EDIT_SOUND_LOGOUT, m_sSoundLogout);
	DDX_Text(pDX, IDC_STATIC_NAME, m_sConnectionName);
	DDX_Check(pDX, IDC_HIDEPWD, m_bHidePassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2)
	ON_WM_DESTROY()
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_LOAD_LOGIN, OnLoadLogin)
//	ON_BN_CLICKED(IDC_LOAD_LOGOUT, OnLoadLogout)
//	ON_BN_CLICKED(IDC_TEST_LOGIN, OnTestLogin)
//	ON_BN_CLICKED(IDC_TEST_LOGOUT, OnTestLogout)
	ON_EN_KILLFOCUS(IDC_EDIT_PASSWORD, OnKillfocusEditPassword)
//	ON_EN_KILLFOCUS(IDC_EDIT_SOUND_LOGIN, OnKillfocusEditSoundLogin)
//	ON_EN_KILLFOCUS(IDC_EDIT_SOUND_LOGOUT, OnKillfocusEditSoundLogout)
	ON_EN_KILLFOCUS(IDC_EDIT_USER_NAME, OnKillfocusEditUserName)
//	ON_BN_CLICKED(IDC_BUTTON_PROG_ADD, OnButtonProgAdd)
//	ON_BN_CLICKED(IDC_BUTTON_PROG_EDIT, OnButtonProgEdit)
//	ON_BN_CLICKED(IDC_BUTTON_PROG_DEL, OnButtonProgDel)
	ON_BN_CLICKED(IDC_HIDEPWD, OnHidepwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2 message handlers

BOOL CPage2::InitPropertyPage()
{
	CString sss; sss.LoadString(IDS_LIST2_COL1);
	sss.LoadString(IDS_LIST2_COL2);

	m_bHidePassword = atol(theApp.GetIniString(gchMain, gchHidePassword, "0"));
	if (m_bHidePassword)
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->SetPasswordChar('*');
	else
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->SetPasswordChar(0);
	
	Reload();
	bWindowActivated = TRUE;
	return TRUE;
}

BOOL CPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	return TRUE;
}

void CPage2::OnDestroy() 
{
//	bThreadExitImmediately = TRUE;
	EnterCriticalSection(&sCriticalSection);
	LeaveCriticalSection(&sCriticalSection);
	CPropertyPage::OnDestroy();
}

void CPage2::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CPage2::OnSetActive() 
{
	if (!bWindowActivated) return CPropertyPage::OnSetActive();
	Reload();
	return CPropertyPage::OnSetActive();
}




/*void CPage2::OnTestLogin() 
{
	UpdateData();
	if (!m_sSoundLogin.IsEmpty()) sndPlaySound(m_sSoundLogin, SND_ASYNC);
}

void CPage2::OnTestLogout() 
{
	UpdateData();
	if (!m_sSoundLogout.IsEmpty()) sndPlaySound(m_sSoundLogout, SND_ASYNC);
}
*/
void CPage2::OnKillfocusEditPassword() 
{
	SaveCurrentUNP();
}
/*
void CPage2::OnKillfocusEditSoundLogin() 
{
	SaveCurrentUNP();
}

void CPage2::OnKillfocusEditSoundLogout() 
{
	SaveCurrentUNP();
}
*/
void CPage2::OnKillfocusEditUserName() 
{
	SaveCurrentUNP();
}

//保存当前状态
BOOL CPage2::SaveCurrentUNP()
{
	if (!bWindowActivated) return TRUE;
	CString Provider = theApp.GetCurrentProviderName();
	UpdateData();
	theApp.SetIniString(Provider, "UserName", m_sUserName);
	theApp.SetIniString(Provider, "Password", "");
	theApp.SetIniString(Provider, "Pwd", "");
	if (!m_sPassword.IsEmpty())
	{
		char aaa[1024]; ZeroMemory(&aaa, 1024);
		char bbb[1024]; ZeroMemory(&bbb, 1024);
		char ccc[1024]; ZeroMemory(&ccc, 1024);
		if (!m_sUserName.IsEmpty()) strcpy(aaa, m_sUserName);
		else strcpy(aaa, "z");
		if (!m_sPassword.IsEmpty()) strcpy(bbb, m_sPassword);
		size_t len = strlen(bbb);
		strxor(aaa, bbb, len);
		aaa[0] = 0;
		for (UINT i = 0; i < len; i++)
		{
			sprintf(ccc,"%s%03u", aaa, (unsigned char) bbb[i]);
			strcpy(aaa, ccc);
		}
		theApp.SetIniString(Provider, "Pwd", aaa);
	}

	theApp.SetIniString(Provider, "Sound01", m_sSoundLogin);
	theApp.SetIniString(Provider, "Sound02", m_sSoundLogout);
	return TRUE;
}

//载入大当前状态
BOOL CPage2::LoadCurrentUNP()
{
	BOOL bNeedRemoveOldPwd = FALSE;
	CString Provider = theApp.GetCurrentProviderName();
	m_sUserName		= theApp.GetIniString(Provider, "UserName", "");
	m_sPassword		= theApp.GetIniString(Provider, "Password", "");
	if (!m_sPassword.IsEmpty()) bNeedRemoveOldPwd = TRUE;
	else
	{
		m_sPassword = theApp.GetIniString(Provider, "Pwd", "");
		if (!m_sPassword.IsEmpty())
		{
			char aaa[1024]; ZeroMemory(&aaa, 1024);
			char bbb[1024]; ZeroMemory(&bbb, 1024);
			char ccc[1024]; ZeroMemory(&ccc, 1024);
			int len = 0;
			for (int i = 0; i < m_sPassword.GetLength(); i+=3)
			{
				bbb[len] = (char) atol(m_sPassword.Mid(i, 3));
				len++;
			}
			if (!m_sUserName.IsEmpty()) strcpy(aaa, m_sUserName);
			else strcpy(aaa, "z");
			strxor(aaa, bbb, len);
			m_sPassword = bbb;
		}
	}
	m_sSoundLogin	= theApp.GetIniString(Provider, "Sound01", "");
	m_sSoundLogout	= theApp.GetIniString(Provider, "Sound02", "");
	LoadPrograms(Provider);
	UpdateData(FALSE);
	if (bNeedRemoveOldPwd)
	{
		BOOL bb = bWindowActivated;
		bWindowActivated = TRUE;
		SaveCurrentUNP();
		bWindowActivated = bb;
	}
	return TRUE;
}

//读入相关参数
BOOL CPage2::Reload()
{
	LoadCurrentUNP();
	m_sConnectionName = theApp.GetCurrentProviderName();
	return TRUE;
}


BOOL CPage2::LoadPrograms(CString Provider)
{
	CString sss, s1, s2;
	UINT i = 0;
	while(true)
	{
		sprintf(sss.GetBuffer(1024), "Program%02ld_Condition", i + 1);
		sss.ReleaseBuffer();
		s1 = theApp.GetIniString(Provider, sss, "online");
		if (s1.IsEmpty()) break;
		sprintf(sss.GetBuffer(1024), "Program%02ld", i + 1);
		sss.ReleaseBuffer();
		s2 = theApp.GetIniString(Provider, sss, "");
		if (s2.IsEmpty()) break;
//		m_listProgs.InsertItem(i, s1);
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 1;
		lvi.pszText = s2.GetBuffer(1024);
//		m_listProgs.SetItem(&lvi);
		s2.ReleaseBuffer();
		i++;
	}
	return TRUE;
}
/*
void CPage2::OnButtonProgAdd() 
{
	CRunProgDlg dlg;
	int nResponse = dlg.DoModal();
	if (nResponse != IDOK) return;
	CString s1, s2, sss;
	s1 = dlg.m_sName;
	if (s1.IsEmpty()) return;
	int i = m_listProgs.GetItemCount();
	s2 = RunConditionI2S(dlg.m_iRadio);
	sprintf(sss.GetBuffer(1024), "Program%02ld_Condition", i + 1);
	sss.ReleaseBuffer();
	CString Provider = theApp.GetCurrentProviderName();
	theApp.SetIniString(Provider, sss, s2);
	sprintf(sss.GetBuffer(1024), "Program%02ld", i + 1);
	sss.ReleaseBuffer();
	theApp.SetIniString(Provider, sss, s1);
	LoadPrograms(Provider);
}

void CPage2::OnButtonProgEdit() 
{
	CString s1, s2, sss;
	for (int i = 0; i < m_listProgs.GetItemCount(); i++)
	{
		if (LVIS_SELECTED == m_listProgs.GetItemState(i, LVIS_SELECTED))
		{
				s1 = m_listProgs.GetItemText(i, 1);
				s2 = m_listProgs.GetItemText(i, 0);
				break;
		}
	}
	if (s2.IsEmpty()) return;
	CRunProgDlg dlg;
	dlg.m_sName = s1;
	dlg.m_iRadio = RunConditionS2I(s2);
	int nResponse = dlg.DoModal();
	if (nResponse != IDOK) return;
	s1 = dlg.m_sName;
	if (s1.IsEmpty()) return;
	s2 = RunConditionI2S(dlg.m_iRadio);
	sprintf(sss.GetBuffer(1024), "Program%02ld_Condition", i + 1);
	sss.ReleaseBuffer();
	CString Provider = theApp.GetCurrentProviderName();
	theApp.SetIniString(Provider, sss, s2);
	sprintf(sss.GetBuffer(1024), "Program%02ld", i + 1);
	sss.ReleaseBuffer();
	theApp.SetIniString(Provider, sss, s1);
	LoadPrograms(Provider);
}

void CPage2::OnButtonProgDel() 
{
	CString s1, s2;
	for (int i = 0; i < m_listProgs.GetItemCount(); i++)
	{
		if (LVIS_SELECTED == m_listProgs.GetItemState(i, LVIS_SELECTED))
		{
				s1 = m_listProgs.GetItemText(i, 1);
				s2 = m_listProgs.GetItemText(i, 0);
				break;
		}
	}
	if (s2.IsEmpty()) return;
	int j = AfxMessageBox(IDS_DELETE_PROGRAM, MB_YESNO | MB_ICONQUESTION);
	if (j != IDYES) return;
	CString p1, p2;
	CString Provider = theApp.GetCurrentProviderName();
	for (j = i + 1; j < m_listProgs.GetItemCount() + 1; j++)
	{
		sprintf(p1.GetBuffer(1024), "Program%02ld_Condition", j + 1); p1.ReleaseBuffer();
		sprintf(p2.GetBuffer(1024), "Program%02ld", j + 1); p2.ReleaseBuffer();
		s1 = theApp.GetIniString(Provider, p1, "");
		s2 = theApp.GetIniString(Provider, p2, "");
		theApp.SetIniString(Provider, p1, "");
		theApp.SetIniString(Provider, p2, "");
		sprintf(p1.GetBuffer(1024), "Program%02ld_Condition", j); p1.ReleaseBuffer();
		sprintf(p2.GetBuffer(1024), "Program%02ld", j); p2.ReleaseBuffer();
		theApp.SetIniString(Provider, p1, s1);
		theApp.SetIniString(Provider, p2, s2);
	}
	LoadPrograms(Provider);
}
*/

CString CPage2::RunConditionI2S(int i)
{
	switch (i)
	{
		case RUN_HANGUP:
			return "hangup";
		case RUN_CONNECT:
			return "connect";
		case RUN_ONLINE:
		default:
			return "online";
	}
	return "";
}

int CPage2::RunConditionS2I(CString s)
{
	if (s == "online")  return RUN_ONLINE;
	if (s == "hangup")  return RUN_HANGUP;
	if (s == "connect") return RUN_CONNECT;
	return -1;
}

UINT CPage2::RunPrograms(LPVOID lpThreadParameter)
{
	EnterCriticalSection(&sCriticalSection);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	if (!ziz)
	{
		LeaveCriticalSection(&sCriticalSection);
		return 0;
	}
	BOOL bRun = (BOOL) lpThreadParameter;
	ziz->iThreadWorkingNow++;
	int i = 0;
	CString s1, s2, sss;
	CString Provider = theApp.GetCurrentProviderName();
	while (!ziz->bThreadExitImmediately)
	{
		sprintf(sss.GetBuffer(1024), "Program%02ld_Condition", i + 1);
		sss.ReleaseBuffer();
		s1 = theApp.GetIniString(Provider, sss, "");
		if (s1.IsEmpty()) break;
		sprintf(sss.GetBuffer(1024), "Program%02ld", i + 1);
		sss.ReleaseBuffer();
		s2 = theApp.GetIniString(Provider, sss, "");
		if (s2.IsEmpty()) break;
		if (bRun == ziz->RunConditionS2I(s1))
		{
			ShellExecute(0,"open", s2, NULL, NULL, SW_SHOW);
		}
		i++;
	}
	ziz->iThreadWorkingNow--;
	LeaveCriticalSection(&sCriticalSection);
	return 0;
}

BOOL CPage2::DestroyWindow() 
{
	EnterCriticalSection(&sCriticalSection);
	LeaveCriticalSection(&sCriticalSection);
	return CPropertyPage::DestroyWindow();
}

//隐藏密码
void CPage2::OnHidepwd() 
{
	UpdateData();
	UpdateData(FALSE);
	if (m_bHidePassword)
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->SetPasswordChar('*');
	else
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->SetPasswordChar(0);
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->RedrawWindow();
	theApp.SetIniString(gchMain, gchHidePassword, L2S(m_bHidePassword));
}


//获得密码
CString CPage2::GetPassword(BOOL b)
{
	if (b) return m_sPassword;
	if (! m_bHidePassword) return m_sPassword;
	return "*****";
}
