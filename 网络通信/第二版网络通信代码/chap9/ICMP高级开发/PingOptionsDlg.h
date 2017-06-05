// PingOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPingOptionsDlg dialog

class CPingOptionsDlg : public CDialog
{
// Construction
public:
	CPingOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPingOptionsDlg)
	enum { IDD = IDD_PingOptions };
	UINT	m_BufferSize;
	CString	m_DefaultHost;
	UINT	m_PingTimeout;
	UINT	m_MaxHops;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPingOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPingOptionsDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
