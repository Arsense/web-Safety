#if !defined(AFX_TRANSFERSOCKET_H__B3710126_6EF0_4A33_87F5_680BB011F089__INCLUDED_)
#define AFX_TRANSFERSOCKET_H__B3710126_6EF0_4A33_87F5_680BB011F089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransferSocket.h : header file
//


#include "afxsock.h"
#include "CMessg.h"

class CTransferDlg;

/////////////////////////////////////////////////////////////////////////////
// CTransferSocket command target

class CTransferSocket : public CSocket
{
// Attributes
public:
	CArchive* m_aSessionIn;
	CArchive* m_aSessionOut;
	CSocketFile* m_sfSocketFile;
	CTransferDlg * m_dlg;
	bool m_bInit;
	bool m_bClose;
	CFile m_file;

public:
	void Init(CTransferDlg * dlg);
	BOOL SendMessage(CMessg * msg);
	void CloseSocket();

// Operations
public:
	CTransferSocket();
	virtual ~CTransferSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTransferSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERSOCKET_H__B3710126_6EF0_4A33_87F5_680BB011F089__INCLUDED_)
