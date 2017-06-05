// Frame.h : interface of the CSnaggerFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__3AE25A2D_59EF_11D1_92DD_00600837DA75__INCLUDED_)
#define AFX_FRAME_H__3AE25A2D_59EF_11D1_92DD_00600837DA75__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSnaggerFrame : public CFrameWnd
{
protected: // create from serialization only
	CSnaggerFrame();
	DECLARE_DYNCREATE(CSnaggerFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSnaggerFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSnaggerFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CSnaggerFrame)
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
