#if !defined(URLDLG_H_)
#define URLDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// URLDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CURLDlg dialog

class CURLDlg : public CDialog
{
protected:
	int m_nMaxPages;
	int m_nMaxLevels;
	BOOL m_bGetMultimedia;
	CString m_strURL;

public:
	void SetMaxPages(int nMaxPages) { m_nMaxPages = nMaxPages; }
	void SetMaxLevels(int nMaxLevels) { m_nMaxLevels = nMaxLevels; }
	void SetGetMultimedia(BOOL bMultimedia) { m_bGetMultimedia = bMultimedia; }
	void SetURL(CString& strURL) { m_strURL = strURL; }
	CString GetURL() { return m_strURL; }

// Construction
public:
	CURLDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CURLDlg)
	enum { IDD = IDD_SNAG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CURLDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHelpbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif 
