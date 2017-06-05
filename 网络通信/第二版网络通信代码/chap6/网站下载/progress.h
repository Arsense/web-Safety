#if !defined(__PROGRESS_H__)
#define __PROGRESS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// progress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgress dialog

class CProgress : public CDialog
{
protected:
	BOOL m_bAborted;
	int m_nMaxPages;
	int m_nMaxLevels;

// Construction
public:
	CProgress(CWnd* pParent = NULL);   // standard constructor

public:
	void SetActionTitle(LPCTSTR lpszText);
	void SetQueuedFiles(int nNumber);
	void SetDownloadedPages(int nNumber);
	void SetDownloadedFiles(int nNumber);
	void SetKBDownloaded(int nNumber);
	void SetLevel(int nLevel);
	BOOL IsAborted() { return m_bAborted; }
	void SetAborted(BOOL bAborted) { m_bAborted = bAborted; }

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CStatic	m_Level;
	CStatic	m_Action;
	CStatic	m_QueuedFiles;
	CStatic	m_FilesDownloaded;
	CStatic	m_PagesDownloaded;
	CStatic	m_KBDownloaded;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__PROGRESS_H__)
