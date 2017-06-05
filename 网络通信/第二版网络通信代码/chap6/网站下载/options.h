
// Options.h : header file
//

#if !defined(__OPTIONS_H__)
#define __OPTIONS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define MAX_LEVELS		20

class CSnagOptions : public CObject
{
public:
	BOOL bMultimedia;
	int	nMaxDepth;
	int nMaxPages;
	BOOL bFixupLinks;
	BOOL bContents;
	BOOL bOffsiteLinks;
	BOOL bUseProxy;
	CString strProxyName;
	long m_lFileVersion;

public:									
	void Serialize(CArchive& ar);
	void SetVersion(long lVersion) { m_lFileVersion = lVersion; }
};


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
protected:
	CSnagOptions *m_pOptions;

// Construction
public:
	COptionsDlg(CSnagOptions *pOptions, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CSpinButtonCtrl	m_PageSpin;
	CSpinButtonCtrl	m_LevelSpin;
	BOOL	m_FixupHTML;
	BOOL	m_GenerateTOC;
	BOOL	m_GetMedia;
	BOOL	m_Log;
	int		m_MaxLevels;
	int		m_MaxPages;
	BOOL	m_OffsiteLinks;
	BOOL	m_NoLimit;
	BOOL	m_UseProxy;
	CString	m_ProxyServer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNoLimit();
	afx_msg void OnHelpbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__OPTIONS_H__)
