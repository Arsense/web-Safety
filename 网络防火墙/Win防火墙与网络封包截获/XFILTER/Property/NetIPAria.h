#if !defined(AFX_NETIPARIA_H__078ECCD9_4955_42F1_B52C_45BB92DC9CC2__INCLUDED_)
#define AFX_NETIPARIA_H__078ECCD9_4955_42F1_B52C_45BB92DC9CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetIPAria.h : header file
//

//=============================================================================================
// CNetIPAria dialog

class CNetIPAria : public CDialog
{
// Construction
public:
	CNetIPAria(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetIPAria)
	enum { IDD = IDD_NET_IP_ARIA };
	CIPAddressCtrl	m_IPEnd;
	CIPAddressCtrl	m_IPStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetIPAria)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetIPAria)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner declare

public:
	void		InitDlgResource();

public:
	int			iState;			//IN 0:add,1:edit
	DWORD		ulStartIP;
	DWORD		ulEndIP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETIPARIA_H__078ECCD9_4955_42F1_B52C_45BB92DC9CC2__INCLUDED_)
