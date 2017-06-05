// FiveChessDoc.cpp : implementation of the CFiveChessDoc class
//

#include "stdafx.h"
#include "FiveChess.h"

#include "FiveChessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFiveChessDoc

IMPLEMENT_DYNCREATE(CFiveChessDoc, CDocument)

BEGIN_MESSAGE_MAP(CFiveChessDoc, CDocument)
	//{{AFX_MSG_MAP(CFiveChessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiveChessDoc construction/destruction

CFiveChessDoc::CFiveChessDoc()
{
	// TODO: add one-time construction code here

}

CFiveChessDoc::~CFiveChessDoc()
{
}

BOOL CFiveChessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFiveChessDoc serialization

void CFiveChessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFiveChessDoc diagnostics

#ifdef _DEBUG
void CFiveChessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFiveChessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFiveChessDoc commands
