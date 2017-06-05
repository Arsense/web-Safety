//=============================================================================================
/*
	文件：		SystemTray.cpp

	说明:
	---------------------------------------------------
		在系统任务栏上显示图标。
	---------------------------------------------------

	工程：		Xfilter 个人防火墙
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/12
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	---------------------------------------------------
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买本软件源代码的用户被默认授权
	可以在自己的程序中使用本软件的部分代码，但作者不对
	代码产生的任何后果负责。
	
		使用了本软件代码的程序只能以可执行文件形式发布，
	未经特别许可，不能将含有本软件源代码的源程序以任何
	形式发布。
	---------------------------------------------------	
*/
//=============================================================================================

#include "stdafx.h"
#include "SystemTray.h"

CSystemTray::CSystemTray()
{
    Initialise();
}

CSystemTray::CSystemTray(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, 
                         HICON icon, UINT uID)
{
    Initialise();
    Create(pParent, uCallbackMessage, szToolTip, icon, uID);
}

void CSystemTray::Initialise()
{
    memset(&m_tnd, 0, sizeof(m_tnd));
    m_bEnabled				= FALSE;
    m_bHidden				= FALSE;
    m_DefaultMenuItemID		= 0;
    m_DefaultMenuItemByPos	= TRUE;
}

BOOL CSystemTray::Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, 
                         HICON icon, UINT uID)
{
    m_bEnabled = (GetVersion() & 0xff) >= 4;
    if (!m_bEnabled) 
    {
        ASSERT(FALSE);
        return FALSE;
    }

    ASSERT(uCallbackMessage >= WM_USER);

    ASSERT(_tcslen(szToolTip) <= 64);

    m_tnd.cbSize = sizeof(NOTIFYICONDATA);
    m_tnd.hWnd   = pParent->GetSafeHwnd();
    m_tnd.uID    = uID;
    m_tnd.hIcon  = icon;
    m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_tnd.uCallbackMessage = uCallbackMessage;
    _tcscpy(m_tnd.szTip, szToolTip);

    m_bEnabled = Shell_NotifyIcon(NIM_ADD, &m_tnd);
    ASSERT(m_bEnabled);
    return m_bEnabled;
}

CSystemTray::~CSystemTray()
{
    RemoveIcon();
}

void CSystemTray::RemoveIcon()
{
    if (!m_bEnabled) return;

    m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
    m_bEnabled = FALSE;
}

void CSystemTray::HideIcon()
{
    if (m_bEnabled && !m_bHidden) {
        m_tnd.uFlags = NIF_ICON;
        Shell_NotifyIcon (NIM_DELETE, &m_tnd);
        m_bHidden = TRUE;
    }
}

BOOL CSystemTray::SetIcon(UINT nIDResource)
{
    HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

    if (!m_bEnabled) return FALSE;

    m_tnd.uFlags = NIF_ICON;
    m_tnd.hIcon = hIcon;

    return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CSystemTray::SetMenuDefaultItem(UINT uItem, BOOL bByPos)
{
    if ((m_DefaultMenuItemID == uItem) && (m_DefaultMenuItemByPos == bByPos)) 
        return TRUE;

    m_DefaultMenuItemID = uItem;
    m_DefaultMenuItemByPos = bByPos;   

    CMenu menu, *pSubMenu;

    if (!menu.LoadMenu(m_tnd.uID))
        return FALSE;

    pSubMenu = menu.GetSubMenu(0);
    if (!pSubMenu)
        return FALSE;

    ::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

    return TRUE;
}

LRESULT CSystemTray::OnTrayNotification(UINT wParam, LONG lParam) 
{
    if (wParam != m_tnd.uID)
        return 0;

    CMenu menu, *pSubMenu;
    CWnd* pTarget			= AfxGetMainWnd();
	static BOOL IsShowed	= FALSE;

    if (LOWORD(lParam) == WM_RBUTTONUP)
    {    
        if (!menu.LoadMenu(m_tnd.uID))
            return 0;
        
        pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return 0;

        ::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

        CPoint pos;
        GetCursorPos(&pos);

        pTarget->SetForegroundWindow();  
		IsShowed = TRUE;
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, 
                         pTarget->GetSafeHwnd(), NULL);
 		IsShowed = FALSE;
        pTarget->PostMessage(WM_NULL, 0, 0);

        menu.DestroyMenu();
    } 
    else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
    {
		if(IsShowed)
			return 0;

        pTarget->SetForegroundWindow();  

        UINT uItem;
        if (m_DefaultMenuItemByPos)
        {
           if (!menu.LoadMenu(m_tnd.uID))
                return 0;
            
            pSubMenu = menu.GetSubMenu(0);
            if (!pSubMenu)
                return 0;
            
            uItem = pSubMenu->GetMenuItemID(m_DefaultMenuItemID);
        }
        else
            uItem = m_DefaultMenuItemID;
        
        pTarget->SendMessage(WM_COMMAND, uItem, 0);

        menu.DestroyMenu();
    }

    return 1;
}
