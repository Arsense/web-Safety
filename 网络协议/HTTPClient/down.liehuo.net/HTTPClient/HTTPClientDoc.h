// HTTPClientDoc.h : interface of the CHTTPClientDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPCLIENTDOC_H__F1EAC74B_B4D6_4142_9127_A4106FCF80B8__INCLUDED_)
#define AFX_HTTPCLIENTDOC_H__F1EAC74B_B4D6_4142_9127_A4106FCF80B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHTTPClientDoc : public CRichEditDoc
{
protected: // create from serialization only
	CHTTPClientDoc();
	DECLARE_DYNCREATE(CHTTPClientDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTTPClientDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CHTTPClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHTTPClientDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPCLIENTDOC_H__F1EAC74B_B4D6_4142_9127_A4106FCF80B8__INCLUDED_)
