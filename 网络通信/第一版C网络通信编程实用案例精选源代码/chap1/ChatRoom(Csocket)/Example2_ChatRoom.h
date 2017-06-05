// Example2_ChatRoom.h : main header file for the EXAMPLE2_CHATROOM application
//

#if !defined(AFX_EXAMPLE2_CHATROOM_H__AFC6005D_BE35_4ED1_9DDC_5F54BE23EE76__INCLUDED_)
#define AFX_EXAMPLE2_CHATROOM_H__AFC6005D_BE35_4ED1_9DDC_5F54BE23EE76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExample2_ChatRoomApp:
// See Example2_ChatRoom.cpp for the implementation of this class
//

class CExample2_ChatRoomApp : public CWinApp
{
public:
	CExample2_ChatRoomApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExample2_ChatRoomApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExample2_ChatRoomApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLE2_CHATROOM_H__AFC6005D_BE35_4ED1_9DDC_5F54BE23EE76__INCLUDED_)
