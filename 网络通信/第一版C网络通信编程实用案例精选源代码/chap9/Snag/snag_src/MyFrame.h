// Frame.h : interface of the CMyFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__3AE25A2D_59EF_11D1_92DD_00600837DA75__INCLUDED_)
#define AFX_FRAME_H__3AE25A2D_59EF_11D1_92DD_00600837DA75__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyFrame : public CFrameWnd
{
protected: // create from serialization only
	CMyFrame();
	DECLARE_DYNCREATE(CMyFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMyFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAME_H__3AE25A2D_59EF_11D1_92DD_00600837DA75__INCLUDED_)
