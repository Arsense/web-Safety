// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__439295AD_B682_4F1F_9A8F_D34BCB2207C5__INCLUDED_)
#define AFX_STDAFX_H__439295AD_B682_4F1F_9A8F_D34BCB2207C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//Setting->Link->output->entrys->wWinMainCRTStartup
//#define UNICODE
//#define _UNICODE

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <ras.h>
#include <afxinet.h>
#include <ws2spi.h>
#include <io.h>				// to use access function
#include "..\lib\htmlhelp.h"
#include "..\common\xfile.h"
#include "guires.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__439295AD_B682_4F1F_9A8F_D34BCB2207C5__INCLUDED_)
