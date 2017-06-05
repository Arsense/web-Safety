#if !defined(AFX_DATASOCKET_H__D5F5CAB0_5E3D_455E_BDF6_976F2B5ACB69__INCLUDED_)
#define AFX_DATASOCKET_H__D5F5CAB0_5E3D_455E_BDF6_976F2B5ACB69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSocket.h : header file
//

#define XFERMODE_IDLE	0
#define XFERMODE_LIST	1
#define XFERMODE_SEND	2
#define XFERMODE_RECEIVE 3
#define XFERMODE_ERROR	4

/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target

class CDataSocket : public CAsyncSocket
{
// Attributes
public:
	CFile m_File;

// Operations
public:
	CDataSocket(CConnectSocket *pSocket, int nTransferType);
	virtual ~CDataSocket();
	int GetStatus();

// Overrides
public:
	void SetData(LPCTSTR lpszData);
	void SetTransferType(int nType, BOOL bWaitForAccept = FALSE);
	void SetRestartOffset(DWORD dwOffset);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSocket)
	public:
	virtual void OnSend(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDataSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	int Receive();
	BOOL PrepareSendFile(LPCTSTR lpszFilename);
	BOOL PrepareReceiveFile(LPCTSTR lpszFilename);
	CString m_strData;
	DWORD m_dwRestartOffset;
	DWORD m_nTotalBytesTransfered;
	DWORD m_nTotalBytesReceive;
	DWORD m_nTotalBytesSend;
	int m_nTransferType;
	BOOL m_bConnected;
	BOOL m_bInitialized;
	int m_nStatus;
	CConnectSocket *m_pConnectSocket;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASOCKET_H__D5F5CAB0_5E3D_455E_BDF6_976F2B5ACB69__INCLUDED_)
