//=============================================================================================
/*
	SystemTray.h

	Project	: XFILTER 1.0
	Author	: Tony Zhu
	Create Date	: 2001/08/12
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/
//=============================================================================================

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

class CSystemTray 
{
public:
    CSystemTray();
    CSystemTray(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
    virtual ~CSystemTray();

public:
    Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
    BOOL  SetIcon(UINT nIDResource);
    void  HideIcon();
    void  RemoveIcon();
    BOOL SetMenuDefaultItem(UINT uItem, BOOL bByPos);
    virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

protected:
	void			Initialise();
    BOOL            m_bEnabled;   
    BOOL            m_bHidden;    
    NOTIFYICONDATA  m_tnd;
    UINT			m_DefaultMenuItemID;
    BOOL			m_DefaultMenuItemByPos;
};

#endif
