// view.h : interface of the CTermView class
//
/////////////////////////////////////////////////////////////////////////////

class CTermView : public CEditView
{
protected: // create from serialization only
	CTermView();
	DECLARE_DYNCREATE(CTermView)

// Attributes
public:
	CTermDoc* GetDocument();
	static CTermView *GetView();
	CTermSocket*	m_pSocket;
	BOOL			m_fConnected;
	BOOL			m_fShowNotifications;

// Operations
public:
	void Display(LPCSTR lpFormat, ...);
	void OnConnect(int nErrorCode);
	void OnSend(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnClose(int nErrorCode);

// Implementation
public:
	virtual ~CTermView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CTermView)
	afx_msg void OnSocketConnect();
	afx_msg void OnUpdateSocketConnect(CCmdUI* pCmdUI);
	afx_msg void OnSocketClose();
	afx_msg void OnUpdateSocketClose(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewSocketNotifications();
	afx_msg void OnUpdateViewSocketNotifications(CCmdUI* pCmdUI);
	afx_msg void OnEditClearBuffer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in view.cpp
inline CTermDoc* CTermView::GetDocument()
   { return (CTermDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
