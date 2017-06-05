#if !defined(AFX_MESSSOCKET_H__9D67A866_D170_4E97_B1FB_6CEFE1EA75FE__INCLUDED_)
#define AFX_MESSSOCKET_H__9D67A866_D170_4E97_B1FB_6CEFE1EA75FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMessSocket command target
class CNeighborInfoDlg;

class CMessSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CMessSocket();
	virtual ~CMessSocket();

// Overrides
public:
	CNeighborInfoDlg  * m_dlg;
	void Init(CNeighborInfoDlg *dlg);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMessSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSSOCKET_H__9D67A866_D170_4E97_B1FB_6CEFE1EA75FE__INCLUDED_)
