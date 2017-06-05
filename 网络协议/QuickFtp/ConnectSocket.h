#if !defined(AFX_CONNECTSOCKET_H__876F27B1_B566_4F12_A329_A7B98ED26AEF__INCLUDED_)
#define AFX_CONNECTSOCKET_H__876F27B1_B566_4F12_A329_A7B98ED26AEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CConnectSocket command target
class CDataSocket;

class CConnectSocket : public CSocket
{
	enum // Token ID's  26÷÷√¸¡Ó
	{
		TOK_ABOR, TOK_BYE, TOK_CDUP, TOK_CWD,
		TOK_DELE, TOK_DIR, TOK_HELP, TOK_LIST,
		TOK_MKD, TOK_NOOP, TOK_PASS, TOK_PASV, 
		TOK_PORT, TOK_PWD, TOK_QUIT, TOK_REST,
		TOK_RETR, TOK_RMD, TOK_RNFR, TOK_RNTO, 
		TOK_SIZE, TOK_STOR, TOK_SYST, TOK_TYPE, 
		TOK_USER, TOK_ERROR,
	};

// Attributes
public:
	struct CFTPCommand
	{
		int m_nTokenID;
		char *m_pszName;
		BOOL m_bHasArguments;
		char *m_pszDescription;
	};

	CDataSocket *m_pDataSocket;
	int m_nRemotePort;
	CString m_strRemoteHost;

// Operations
public:
	CConnectSocket();
	virtual ~CConnectSocket();

// Overrides
public:
	void FireStatusMessage(LPCTSTR lpszStatus, int nType);
	BOOL SendResponse(LPCTSTR pstrFormat, ...);
	void DestroyDataConnection();
	BOOL m_bPassiveMode;
	BOOL CreateDataConnection(int nTransferType,LPCTSTR lpszData);
	CString m_strUserName;
	BOOL m_bLoggedon;
	void ParseCommand();
	BOOL GetRxCommand(CString &strCommand,CString &strArgument);
	CWinThread* m_pThread;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CConnectSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	BOOL m_bRenameFile;
	CString m_strRenameFile;
	CStringList m_strCommands;
	void GetRxLine();
	CString m_RxBuffer;
	DWORD m_dwRestartOffset;
	CString m_strCurrentDir;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTSOCKET_H__876F27B1_B566_4F12_A329_A7B98ED26AEF__INCLUDED_)
