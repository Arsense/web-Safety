// SampleBrowerDoc.h : interface of the CSampleBrowerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEBROWERDOC_H__248D2433_5B21_4F6F_A5FD_68F40365CD9E__INCLUDED_)
#define AFX_SAMPLEBROWERDOC_H__248D2433_5B21_4F6F_A5FD_68F40365CD9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSampleBrowerDoc : public CDocument
{
protected: // create from serialization only
	CSampleBrowerDoc();
	DECLARE_DYNCREATE(CSampleBrowerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleBrowerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSampleBrowerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSampleBrowerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEBROWERDOC_H__248D2433_5B21_4F6F_A5FD_68F40365CD9E__INCLUDED_)
