// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B416E2D4_A079_4D17_990A_46975324A9A5__INCLUDED_)
#define AFX_STDAFX_H__B416E2D4_A079_4D17_990A_46975324A9A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#include <afxtempl.h>  //CTypedPtrList<CObList, CConnectThread*> m_ThreadList;
#include <afxmt.h>  //CCriticalSection m_CriticalSection;

#define FTPSTAT_DOWNLOADSUCCEEDED	2
#define FTPSTAT_UPLOADSUCCEEDED		3
#define FTPSTAT_DOWNLOADFAILED		4
#define FTPSTAT_UPLOADFAILED		5

#define WM_THREADSTART	WM_USER+200
#define WM_THREADCLOSE	WM_USER+201
#define WM_THREADMSG	WM_USER+202
#define WM_ADDTRACELINE WM_USER+203


BOOL IsNumeric(char *buff);//判断是不是数字字符
BOOL MakeSureDirectoryPathExists(LPCTSTR lpszDirPath);//创建目录函数
CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags);//浏览目录对话框
void GetAppDir(CString& strAppDir);//取得应用程序的目录
extern BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout);//这个等到现场终止的函数不是很理解，以后用WaitForSingleObject()去替换，先这样

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B416E2D4_A079_4D17_990A_46975324A9A5__INCLUDED_)
