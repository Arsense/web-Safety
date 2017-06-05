// PortScanDoc.h : interface of the CPortScanDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTSCANDOC_H__4D7B6B4B_FDBC_11D4_B48A_F589D689A532__INCLUDED_)
#define AFX_PORTSCANDOC_H__4D7B6B4B_FDBC_11D4_B48A_F589D689A532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPortScanDoc : public CDocument
{
protected: // create from serialization only
	CPortScanDoc();
	DECLARE_DYNCREATE(CPortScanDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortScanDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPortScanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPortScanDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTSCANDOC_H__4D7B6B4B_FDBC_11D4_B48A_F589D689A532__INCLUDED_)
