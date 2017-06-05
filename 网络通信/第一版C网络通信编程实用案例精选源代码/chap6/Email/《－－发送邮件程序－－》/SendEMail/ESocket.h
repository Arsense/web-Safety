#if !defined(AFX_ESOCKET_H__BF0EE2CE_9102_4FC8_82F0_8991AE0A3A23__INCLUDED_)
#define AFX_ESOCKET_H__BF0EE2CE_9102_4FC8_82F0_8991AE0A3A23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ESocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CESocket command target
class CSendEMailDlg;

class CESocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CESocket();
	virtual ~CESocket();

// Overrides
public:
	int m_iCount;
	CSendEMailDlg * m_dlg;
	void Init(CSendEMailDlg *dlg);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CESocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CESocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESOCKET_H__BF0EE2CE_9102_4FC8_82F0_8991AE0A3A23__INCLUDED_)
