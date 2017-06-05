// CntrItem.h : interface of the CHTTPClientCntrItem class
//

#if !defined(AFX_CNTRITEM_H__A789B880_4294_46F3_8D4B_F58616846C79__INCLUDED_)
#define AFX_CNTRITEM_H__A789B880_4294_46F3_8D4B_F58616846C79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHTTPClientDoc;
class CHTTPClientView;

class CHTTPClientCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CHTTPClientCntrItem)

// Constructors
public:
	CHTTPClientCntrItem(REOBJECT* preo = NULL, CHTTPClientDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CHTTPClientDoc* GetDocument()
		{ return (CHTTPClientDoc*)CRichEditCntrItem::GetDocument(); }
	CHTTPClientView* GetActiveView()
		{ return (CHTTPClientView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTTPClientCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CHTTPClientCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__A789B880_4294_46F3_8D4B_F58616846C79__INCLUDED_)
