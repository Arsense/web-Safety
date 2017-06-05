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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#if !defined(AFX_STDAFX_H__298DD349_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
#define AFX_STDAFX_H__298DD349_F00C_11D2_9864_D86B908C7AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>			// MFC core and standard components
#include <afxext.h>			// MFC extensions
#include <afxdisp.h>		// MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WM_DIAL_NEXT_PHONE	(WM_USER + 201)
#define WM_DIAL_FUNC		(WM_USER + 202)
#define WM_START_SESSION	(WM_USER + 203)
#define WM_FINISH_SESSION	(WM_USER + 204)
#define WM_ICON_NOTIFY		(WM_USER + 205)

CString GetRegKey(HKEY hOpen, CString key1,CString key2);
CString L2S(LONG l);
CString D2S_2(double d);
void PrintError();

extern char gchMain[];
extern char gchRedialWhenHangUp[];
extern char gchLogFile[];
extern char gchPingTime[];
extern char gchMinSpeed[];
extern char gchTermKB[];
extern char gchTermTime[];
extern char gchMaxWaitTime[];
extern char gchPulseDialing[];
extern char gchEnableTerminateWhenExit[];
extern char gchEnableLogFile[];
extern char gchEnablePing[];
extern char gchEnableMinSpeed[];
extern char gchEnableTerminateLowSpeed[];
extern char gchEnableTimeout[];
extern char gchInterfaceStartMinimized[];
extern char gchInterfaceMinimizeOnConnect[];
extern char gchHidePassword[];


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__298DD349_F00C_11D2_9864_D86B908C7AFE__INCLUDED_)
