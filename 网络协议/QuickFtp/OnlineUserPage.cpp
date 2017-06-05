// OnlineUserPage.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "OnlineUserPage.h"
#include "UserAccountsDlg.h"
#include "FTPServerApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFTPServer theServer;
/////////////////////////////////////////////////////////////////////////////
// COnlineUserPage dialog


COnlineUserPage::COnlineUserPage(CWnd* pParent /*=NULL*/)
	: CDialog(COnlineUserPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(COnlineUserPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COnlineUserPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnlineUserPage)
	DDX_Control(pDX, IDC_ONLINE_USERS, m_OnlineUsers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COnlineUserPage, CDialog)
	//{{AFX_MSG_MAP(COnlineUserPage)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_KICK_USER, OnKickUser)
	ON_COMMAND(IDC_EDIT_USER, OnEditUser)
	ON_COMMAND(IDC_BLOCK_IP, OnBlockIp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnlineUserPage message handlers

//Called by the framework in response to the WM_INITDIALOG message.
BOOL COnlineUserPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_OnlineUsers.InsertColumn(0, "线程ID");		
	m_OnlineUsers.InsertColumn(1, "用户名");	
	m_OnlineUsers.InsertColumn(2, "IP地址");	
	m_OnlineUsers.InsertColumn(3, "登录时间");
	
	DWORD dwStyle = m_OnlineUsers.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
    m_OnlineUsers.SetExtendedStyle(dwStyle);
	
	return TRUE; 
}

//Handle WM_SIZE message
void COnlineUserPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_ONLINE_USERS)))
	{
		CRect rect;
		GetClientRect(rect);
		m_OnlineUsers.MoveWindow(rect);
		m_OnlineUsers.SetColumnWidth(0, 0);
		m_OnlineUsers.SetColumnWidth(1, rect.Width()/3-2);
		m_OnlineUsers.SetColumnWidth(2, rect.Width()/3-2);
		m_OnlineUsers.SetColumnWidth(3, rect.Width()/3-2);
	}
}

//Add new connected user to list	
void COnlineUserPage::AddUser(DWORD nThreadID, LPCTSTR lpszName, LPCTSTR lpszAddress)
{
	CString strThreadID;
	strThreadID.Format("%d",nThreadID);

	LVFINDINFO info;

	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = (LPCTSTR)strThreadID;

	int nIndex = m_OnlineUsers.FindItem(&info);
	if(nIndex == -1)
	{
		nIndex = m_OnlineUsers.InsertItem(0,strThreadID);
	}

	m_OnlineUsers.SetItemText(nIndex,1,lpszName);
	m_OnlineUsers.SetItemText(nIndex,2,lpszAddress);
	m_OnlineUsers.SetItemText(nIndex,3,CTime::GetCurrentTime().Format("%H:%M:%S"));
}

//Remove disconnected user from list
void COnlineUserPage::RemoveUser(DWORD nThreadID)
{
	CString strThreadID;
	strThreadID.Format("%d",nThreadID);

	LVFINDINFO info;

	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = (LPCTSTR)strThreadID;

	int nIndex = m_OnlineUsers.FindItem(&info);
	if(nIndex != -1)//之前一直删除不掉的原因是这里写成的"=="
	{
		m_OnlineUsers.DeleteItem(nIndex);
	}
}

//Show context menu
void COnlineUserPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// get selected user
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nIndex == -1)
        return;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_ONLINE_MENU));
	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);
}

//Close connection for this user(s)
void COnlineUserPage::OnKickUser() 
{
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	while (nIndex != -1)
	{
		CString strThreadID = m_OnlineUsers.GetItemText(nIndex, 0);
		PostThreadMessage(atoi(strThreadID), WM_QUIT, 0 ,0);
		nIndex = m_OnlineUsers.GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED);
	}
}

//Change user rights of selected user	
void COnlineUserPage::OnEditUser() 
{
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if (nIndex != -1)	
	{
		CUserAccountsDlg dlg;

		theServer.m_UserManager.GetUserAccountList(dlg.m_UsersArray);
		dlg.m_strUserName = m_OnlineUsers.GetItemText(nIndex, 1);
		if (dlg.DoModal() == IDOK)
		{
			theServer.m_UserManager.UpdateUserAccountList(dlg.m_UsersArray);
		}	
	}
}

// Change user rights of selected user	
void COnlineUserPage::OnBlockIp() 
{
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if (nIndex == -1)
		return;

	while (nIndex != -1)
	{
		CString strIP = m_OnlineUsers.GetItemText(nIndex, 2);
		//((CApplicationDlg *)GetParent())->m_SecurityPage.AddIPToBlockList(strIP);
		nIndex = m_OnlineUsers.GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED);
	}
}
