#if !defined(NETTIMESHEET_H)
#define NETTIMESHEET_H

//=============================================================================================
// CNetTimeSheet

class CNetTimeSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CNetTimeSheet)

// Construction
public:
	CNetTimeSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CNetTimeSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetTimeSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetTimeSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNetTimeSheet)
	afx_msg void OnOK();
	afx_msg BOOL OnCancel();
	afx_msg void OnApply();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//owner add 
public:
	void		InitDlgResource	();
	int			EnableButton	(BOOL bEnable);
	int			Apply			();
};

#endif 
