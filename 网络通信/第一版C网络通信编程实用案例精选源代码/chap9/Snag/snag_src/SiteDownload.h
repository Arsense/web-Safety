// SiteSnag.h : main header file for the SITESNAG application
//

#if !defined(AFX_SITESNAG_H__3AE25A29_59EF_11D1_92DD_00600837DA75__INCLUDED_)
#define AFX_SITESNAG_H__3AE25A29_59EF_11D1_92DD_00600837DA75__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "ConfigureSetting.h"

/////////////////////////////////////////////////////////////////////////////
// CSiteDownloadApp:
// See SiteSnag.cpp for the implementation of this class
//

class CSiteDownloadApp : public CWinApp
{
protected:
	BOOL HasNewShell();
	BOOL AlreadyRunning();
	BOOL ProcessCommandLine();
	
public:
	CSiteDownloadApp();

	void UpdateMRU(CCmdUI *pCmdUI);
	CRecentFileList *GetMRUList() { return m_pRecentFileList; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSiteDownloadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSiteDownloadApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SITESNAG_H__3AE25A29_59EF_11D1_92DD_00600837DA75__INCLUDED_)
