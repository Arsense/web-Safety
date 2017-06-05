#if !defined(__OPTIONS_H__)
#define __OPTIONS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define MAX_LEVELS		20

class CConfigure : public CObject
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

public:									
	void Serialize(CArchive& ar);
};


/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog

class CConfigureDlg : public CDialog
{
protected:
	CConfigure *m_pOptions;

// Construction
public:
	CConfigureDlg(CConfigure *pOptions, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigureDlg)
	enum { IDD = IDD_OPTIONS };
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
	//{{AFX_VIRTUAL(CConfigureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNoLimit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__OPTIONS_H__)
