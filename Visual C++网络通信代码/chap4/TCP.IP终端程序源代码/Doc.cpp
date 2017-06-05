// doc.cpp : implementation of the CTermDoc class
//

#include "stdafx.h"
#include "wsterm.h"

#include "doc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTermDoc

IMPLEMENT_DYNCREATE(CTermDoc, CDocument)

BEGIN_MESSAGE_MAP(CTermDoc, CDocument)
	//{{AFX_MSG_MAP(CTermDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermDoc construction/destruction

CTermDoc::CTermDoc()
{
	// TODO: add one-time construction code here
}

CTermDoc::~CTermDoc()
{
}

BOOL CTermDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // Empty the edit control
    ((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTermDoc serialization

void CTermDoc::Serialize(CArchive& ar)
{
	// Let the edit control handle serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}


/////////////////////////////////////////////////////////////////////////////
// CTermDoc diagnostics

#ifdef _DEBUG
void CTermDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTermDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTermDoc commands
