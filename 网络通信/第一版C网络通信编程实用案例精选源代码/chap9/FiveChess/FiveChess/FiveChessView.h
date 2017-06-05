// FiveChessView.h : interface of the CFiveChessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIVECHESSVIEW_H__6E62C787_E804_4966_8738_1664043E1DF4__INCLUDED_)
#define AFX_FIVECHESSVIEW_H__6E62C787_E804_4966_8738_1664043E1DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Match.h"


class CFiveChessDoc;


class CFiveChessView : public CView
{
protected: // create from serialization only
	CFiveChessView();
	DECLARE_DYNCREATE(CFiveChessView)

// Attributes
public:
	CFiveChessDoc* GetDocument();


public:
	CString			m_sMsgList;
	CServerSocket	m_ListenSocket;
	CClientSocket	m_ClientSocket;
	BOOL			m_bIsClient;
	BOOL			m_bIsInit;
	int				m_port;
	CString			m_ip;
	BYTE			m_bIP[4];
	CString			m_sLocalName;
	CEdit			m_inputedit;
	CEdit			m_outputedit;
	int				m_iLineCurrentPos;

public:
	Match			m_match;
	int				m_who;
	int				m_turn;//轮谁走棋
	int				m_POS[2];//落子的位置
	int				m_winpos[5][2];
	BOOL			m_bWin;	//
	BOOL			m_bOver;//结束了
public:
	void ProcessPendingAccept();
	void GetLocalIP();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiveChessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFiveChessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:

	//{{AFX_MSG(CFiveChessView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void INPUTMESSAGE ();
	afx_msg void OnSetclient();
	afx_msg void OnSetserver();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FiveChessView.cpp
inline CFiveChessDoc* CFiveChessView::GetDocument()
   { return (CFiveChessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIVECHESSVIEW_H__6E62C787_E804_4966_8738_1664043E1DF4__INCLUDED_)
