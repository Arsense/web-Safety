// HTTPClientView.h : interface of the CHTTPClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPCLIENTVIEW_H__A52D66AD_8FED_4D5A_A4F0_3A11482C5AC0__INCLUDED_)
#define AFX_HTTPCLIENTVIEW_H__A52D66AD_8FED_4D5A_A4F0_3A11482C5AC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClientSocket.h"
#include "request.h"

class CHTTPClientDoc;
class CHTTPClientCntrItem;

class CHTTPClientView : public CRichEditView
{
protected: // create from serialization only
	CHTTPClientView();
	DECLARE_DYNCREATE(CHTTPClientView)

// Attributes
	CHARFORMAT cfm;
public:
	CHTTPClientDoc* GetDocument();
	CClientSocket* m_pSocket;
	CRequest* m_pReq;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTTPClientView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Post(CString filename);
	void Head(CString location);
	void SendData();
	void Get(CString location);
	int m_status;
	void ProcessLine(CString str);
	void Receive();
	void Connect();
	CString GetLine( const CByteArray& bytes, int nBytes, int& ndx );
	BOOL m_bConnected;
	BOOL m_bSend;
	CString m_strSvr;
	CString m_location;
	UINT nPort;
	void TextOut(LPCTSTR lpszMessage, COLORREF clr);
	virtual ~CHTTPClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHTTPClientView)
	afx_msg void OnDestroy();
	afx_msg void OnGet();
	afx_msg void OnHead();
	afx_msg void OnPost();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HTTPClientView.cpp
inline CHTTPClientDoc* CHTTPClientView::GetDocument()
   { return (CHTTPClientDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPCLIENTVIEW_H__A52D66AD_8FED_4D5A_A4F0_3A11482C5AC0__INCLUDED_)
