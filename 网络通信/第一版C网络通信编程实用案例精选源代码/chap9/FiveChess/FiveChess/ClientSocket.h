#if !defined(AFX_CLIENTSOCKET_H__AC98B01D_DD4E_4558_84AF_AE1543A74CD5__INCLUDED_)
#define AFX_CLIENTSOCKET_H__AC98B01D_DD4E_4558_84AF_AE1543A74CD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target
#include "afxsock.h"
#include "CMessg.h"


class CFiveChessView;

class CClientSocket : public CSocket
{
// Attributes
public:
	CArchive* m_aSessionIn;
	CArchive* m_aSessionOut;
	CSocketFile* m_sfSocketFile;
	CFiveChessView * m_view;
	bool m_bInit;
	bool m_bClose;
public:
	void Init(CFiveChessView * view);
	BOOL SendMessage(CMessg * msg);
	void CloseSocket();
	
public:
	static int GetLocalHostName(CString &sHostName);	//获得本地计算机名称
	static int GetIpAddress(const CString &sHostName, CString &sIpAddress);//获得本地IP
	static int GetIpAddress(const CString &sHostName, BYTE &f0,BYTE &f1,BYTE &f2,BYTE &f3);//获得本地IP
	static CString ErrorReason(int tag);				//

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__AC98B01D_DD4E_4558_84AF_AE1543A74CD5__INCLUDED_)
