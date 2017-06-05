#if !defined(AFX_SERVERSOCKET_H__124E26CF_1EC6_491D_BB7F_7663663D1D4F__INCLUDED_)
#define AFX_SERVERSOCKET_H__124E26CF_1EC6_491D_BB7F_7663663D1D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target
#include "afxsock.h"

class CFiveChessView;

class CServerSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CServerSocket();
	virtual ~CServerSocket();

// Overrides
public:
	CFiveChessView * m_view;
	UINT m_uPort;
	BOOL Init(UINT port, CFiveChessView* dlg);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__124E26CF_1EC6_491D_BB7F_7663663D1D4F__INCLUDED_)
