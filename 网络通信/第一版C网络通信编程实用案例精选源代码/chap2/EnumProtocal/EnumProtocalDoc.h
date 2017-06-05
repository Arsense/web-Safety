// EnumProtocalDoc.h : interface of the CEnumProtocalDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMPROTOCALDOC_H__D5662FC7_3E56_454A_B747_10BF7B4B81E7__INCLUDED_)
#define AFX_ENUMPROTOCALDOC_H__D5662FC7_3E56_454A_B747_10BF7B4B81E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEnumProtocalDoc : public CDocument
{
protected: // create from serialization only
	CEnumProtocalDoc();
	DECLARE_DYNCREATE(CEnumProtocalDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumProtocalDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnumProtocalDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEnumProtocalDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMPROTOCALDOC_H__D5662FC7_3E56_454A_B747_10BF7B4B81E7__INCLUDED_)
