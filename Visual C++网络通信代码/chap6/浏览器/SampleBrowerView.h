// SampleBrowerView.h : interface of the CSampleBrowerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEBROWERVIEW_H__8B48F780_F538_4881_A3D7_A45E7C44EF8E__INCLUDED_)
#define AFX_SAMPLEBROWERVIEW_H__8B48F780_F538_4881_A3D7_A45E7C44EF8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleBrowerDoc;
class CSampleBrowerView : public CHtmlView
{
protected: // create from serialization only
	CSampleBrowerView();
	DECLARE_DYNCREATE(CSampleBrowerView)

// Attributes
public:
	CSampleBrowerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleBrowerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSampleBrowerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSampleBrowerView)
	afx_msg void OnGoForward();
	afx_msg void OnGoBack();
	afx_msg void OnGoRefresh();
	afx_msg void OnGoSerach();
	afx_msg void OnGoStop();
	afx_msg void OnStartPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SampleBrowerView.cpp
inline CSampleBrowerDoc* CSampleBrowerView::GetDocument()
   { return (CSampleBrowerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEBROWERVIEW_H__8B48F780_F538_4881_A3D7_A45E7C44EF8E__INCLUDED_)
