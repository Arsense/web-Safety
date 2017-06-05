#if !defined(AFX_SERVERSOCKET_H__7AC10AC2_CEAA_4A38_A4F7_24D4CF24EA65__INCLUDED_)
#define AFX_SERVERSOCKET_H__7AC10AC2_CEAA_4A38_A4F7_24D4CF24EA65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//

#include "afxsock.h"
class CTransferDlg;



/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

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
	CTransferDlg* m_dlg;
	void Init(CTransferDlg * dlg);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
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

#endif // !defined(AFX_SERVERSOCKET_H__7AC10AC2_CEAA_4A38_A4F7_24D4CF24EA65__INCLUDED_)
