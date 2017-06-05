#if !defined(AFX_HOSTNAMEDLG_H__B6FB8DFD_A821_4A63_B225_D915395ED571__INCLUDED_)
#define AFX_HOSTNAMEDLG_H__B6FB8DFD_A821_4A63_B225_D915395ED571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HostNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHostNameDlg dialog

class CHostNameDlg : public CDialog
{
// Construction
public:
	CHostNameDlg(BOOL hosttag=TRUE,CWnd* pParent = NULL);   // standard constructor
	BOOL  m_bHost;	//判断这个对话框是输入ip还是输入主机名称
// Dialog Data
	//{{AFX_DATA(CHostNameDlg)
	enum { IDD = IDD_HOSTNAME };
	CString	m_sHostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHostNameDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTNAMEDLG_H__B6FB8DFD_A821_4A63_B225_D915395ED571__INCLUDED_)
