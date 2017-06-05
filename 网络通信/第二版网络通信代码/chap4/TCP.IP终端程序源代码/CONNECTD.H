// connectd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectDialog dialog

class CConnectDialog : public CDialog
{
// Construction
public:
	CConnectDialog(CWnd* pParent = NULL);	// standard constructor
	
// Attributes
	UINT	m_nPort;

// Dialog Data
	//{{AFX_DATA(CConnectDialog)
	enum { IDD = IDD_CONNECT };
	CString	m_strHostName;
	CString	m_strPort;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CConnectDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
