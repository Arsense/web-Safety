#if !defined(AFX_CONNECTTHREAD_H__D2C93F59_C8E1_4BBD_8FB4_0E457BAB7980__INCLUDED_)
#define AFX_CONNECTTHREAD_H__D2C93F59_C8E1_4BBD_8FB4_0E457BAB7980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CConnectThread thread
#include "ConnectSocket.h"

class CConnectThread : public CWinThread
{
	DECLARE_DYNCREATE(CConnectThread)
protected:
	CConnectThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CConnectSocket m_ConnectSocket;

	CString m_strRemoteHost;
	SOCKET m_hSocket;
	CWnd* m_pWndServer;

private:
	int m_nReceivedBytes;
	int m_nSentBytes;
// Operations
public:
	void UpdateStatistic(int nType);
	void IncSentBytes(int nBytes);
	void IncReceivedBytes(int nBytes);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConnectThread();

	// Generated message map functions
	//{{AFX_MSG(CConnectThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
    LRESULT OnThreadMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTTHREAD_H__D2C93F59_C8E1_4BBD_8FB4_0E457BAB7980__INCLUDED_)
