// MyBrowserView.h : interface of the CMyBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBROWSERVIEW_H__7DE4B1B8_A24E_4688_9628_5D4686367553__INCLUDED_)
#define AFX_MYBROWSERVIEW_H__7DE4B1B8_A24E_4688_9628_5D4686367553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyBrowserDoc;

class CMyBrowserView : public CHtmlView
{
protected: // create from serialization only
	CMyBrowserView();
	DECLARE_DYNCREATE(CMyBrowserView)

// Attributes
public:
	CMyBrowserDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBrowserView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyBrowserView)
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoRefresh();
	afx_msg void OnGoSerach();
	afx_msg void OnGoStop();
	afx_msg void OnStartPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyBrowserView.cpp
inline CMyBrowserDoc* CMyBrowserView::GetDocument()
   { return (CMyBrowserDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBROWSERVIEW_H__7DE4B1B8_A24E_4688_9628_5D4686367553__INCLUDED_)
