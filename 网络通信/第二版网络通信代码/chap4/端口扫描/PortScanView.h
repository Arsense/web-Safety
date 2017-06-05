// PortScanView.h : interface of the CPortScanView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTSCANVIEW_H__4D7B6B4D_FDBC_11D4_B48A_F589D689A532__INCLUDED_)
#define AFX_PORTSCANVIEW_H__4D7B6B4D_FDBC_11D4_B48A_F589D689A532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPortScanDoc;
class CMainFrame;

//This data structure registers the statuses of scanned ports:
typedef struct
{
	int nAttempts;
	TCHAR IPAddress[16];
	TCHAR port[5];
	BOOL bStatus; //1 = open , 0 = close
}DATA;

class CPortScanView : public CFormView
{
public:
	//Maximum attempts to connect a socket
	UINT m_nMaxAttempts;	
protected: 
	// create from serialization only
	CMainFrame* m_parent;
	
	//Shows the headers of member variable m_cResult (See below)
	void ShowHeaders(void);	
	     
	//Adds some new headers to m_cResult.
	void AddHeader(LPTSTR hdr);	

	//Adds a new item to m_cResult
	AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex = -1);
	                                      
	CPortScanView();

	//Adds a new column to m_cResult
	AddColumn(		
	LPCTSTR strItem,int nItem,int nSubItem = -1,
	int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
	int nFmt = LVCFMT_LEFT);     

	DECLARE_DYNCREATE(CPortScanView)

public:
	//{{AFX_DATA(CPortScanView)
	enum { IDD = IDD_PORTSCAN_FORM };
	CProgressCtrl	m_cProgress;

	//Number of attempts to check whether or not a port is open or close
	CEdit	m_cAttempts;	

	CButton	m_cBtnStop;
	CListCtrl	m_cResult;	//The result comes on screen by this object
	CButton	m_cBtnScan;
	CIPAddressCtrl	m_cIP;	//Destination IP address is got from this Control
	CEdit	m_cPortTo;		//Upper range of scanning ports
	CEdit	m_cPortFrom;	//Lower range of scanning ports
	CEdit	m_cSinglePort;	//A single port number to be scanned
	//}}AFX_DATA

// Attributes
public:
	CPortScanDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortScanView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL TestConnection(CString IP, UINT nPort);
	virtual ~CPortScanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bSinglePort;
	UINT m_minPort,m_maxPort;	//Lower bound and upper bound of scanning ports range
	UINT m_nCounter;
	CStringList* m_pColumns;//titles of columns of m_cResult
	COLORREF m_clrBk,
		     m_clrText;

// Generated message map functions
protected:
	//{{AFX_MSG(CPortScanView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRadioSingle();
	afx_msg void OnRadioRange();
	afx_msg void OnButtonScan();
	afx_msg void OnButtonStop();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPtrList* m_pStatusList;//Our link list. The nodes are in type DATA*. See the above structure...
	CBrush* m_pBrush;
};

#ifndef _DEBUG  // debug version in PortScanView.cpp
inline CPortScanDoc* CPortScanView::GetDocument()
   { return (CPortScanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTSCANVIEW_H__4D7B6B4D_FDBC_11D4_B48A_F589D689A532__INCLUDED_)
