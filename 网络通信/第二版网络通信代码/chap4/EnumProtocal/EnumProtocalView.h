// EnumProtocalView.h : interface of the CEnumProtocalView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMPROTOCALVIEW_H__8DAD25AD_2FF4_44C1_914F_5F11E07250E9__INCLUDED_)
#define AFX_ENUMPROTOCALVIEW_H__8DAD25AD_2FF4_44C1_914F_5F11E07250E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include <wsipx.h>
#include <ws2dnet.h>
#include <ws2atm.h>

// #include <ws2rsvp.h> -- Not provided
// RSVP protocols according to WSANX203.DOC
#define IPPROTO_RSVP 		0x2e 

// #include <ws2osi.h>	-- Not provided
// ISO protocols according to WSANX203.DOC
#define ISOPROTO_TP_CONS   25
#define ISOPROTO_TP4_CLNS  29
#define ISOPROTO_CLTP_CLNS 30
#define ISOPROTO_X25       32

class CEnumProtocalView : public CTreeView
{
protected: // create from serialization only
	CEnumProtocalView();
	DECLARE_DYNCREATE(CEnumProtocalView)

// Attributes
public:
	CEnumProtocalDoc* GetDocument();
    HTREEITEM AddTreeItem(HWND hWndTree, 
					  HTREEITEM hParent, 
					  LPSTR lpszFormat, 
					  ...);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumProtocalView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	LPCTSTR GetProtoDesc(int iProtocol);
	LPCTSTR GetDescription(int nAddressFamily);
	LPCTSTR GUIDtoString(GUID *pGUID);
	void AddAllProtocals();
	virtual ~CEnumProtocalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEnumProtocalView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EnumProtocalView.cpp
inline CEnumProtocalDoc* CEnumProtocalView::GetDocument()
   { return (CEnumProtocalDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMPROTOCALVIEW_H__8DAD25AD_2FF4_44C1_914F_5F11E07250E9__INCLUDED_)
