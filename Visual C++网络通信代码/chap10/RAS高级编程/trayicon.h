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

/////////////////////////////////////////////////////////////////////////////
// TrayIcon.h : header file
//

#ifndef _INCLUDED_TRAYICON_H_
#define _INCLUDED_TRAYICON_H_

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon window

class CTrayIcon : public CObject
{
// Construction/destruction
public:
	CTrayIcon();
	CTrayIcon(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
	virtual ~CTrayIcon();

// Operations
public:
	BOOL Enabled() { return m_bEnabled; }
	BOOL Visible() { return !m_bHidden; }

	//Create the tray icon
	Create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);

	BOOL SetMaxWidth(int iWidth);

	//Change or retrieve the Tooltip text
	BOOL    SetTooltipText(LPCTSTR pszTooltipText);
	BOOL    SetTooltipText(UINT nID);
	CString GetTooltipText();

	//Change or retrieve the icon displayed
	BOOL  SetIcon(HICON hIcon);
	BOOL  SetIcon(LPCTSTR lpIconName);
	BOOL  SetIcon(UINT nIDResource);
	BOOL  SetStandardIcon(LPCTSTR lpIconName);
	BOOL  SetStandardIcon(UINT nIDResource);
	HICON GetIcon();
	void  HideIcon();
	void  ShowIcon();
	void  RemoveIcon();
	void  MoveToRight();

	//Change or retrieve the window to send notification messages to
	BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
	CWnd* GetNotificationWnd();

	//Default handler for tray notification message
	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayIcon)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL			m_bEnabled;	// does O/S support tray icon?
	BOOL			m_bHidden;	// Has the icon been hidden?
	NOTIFYICONDATA	m_tnd;

	DECLARE_DYNAMIC(CTrayIcon)
};


#endif

/////////////////////////////////////////////////////////////////////////////
