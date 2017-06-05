#if !defined(__WAIT_H__)
#define __WAIT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Wait.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSnagWait dialog

class CSnagWait : public CDialog
{
// Construction
public:
	CSnagWait(LPCTSTR lpszMessage, CWnd* pParent = NULL);   // standard constructor
	void SetWaitMessage(LPCTSTR lpszMessage) { m_Message = lpszMessage; }

// Dialog Data
	//{{AFX_DATA(CSnagWait)
	enum { IDD = IDD_WAIT };
	CString	m_Message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSnagWait)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSnagWait)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__WAIT__H__)
