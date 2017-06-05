// SampleBrowerDoc.cpp : implementation of the CSampleBrowerDoc class
//

#include "stdafx.h"
#include "SampleBrower.h"

#include "SampleBrowerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerDoc

IMPLEMENT_DYNCREATE(CSampleBrowerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSampleBrowerDoc, CDocument)
	//{{AFX_MSG_MAP(CSampleBrowerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerDoc construction/destruction

CSampleBrowerDoc::CSampleBrowerDoc()
{
	// TODO: add one-time construction code here

}

CSampleBrowerDoc::~CSampleBrowerDoc()
{
}

BOOL CSampleBrowerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerDoc serialization

void CSampleBrowerDoc::Serialize(CArchive& ar)
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
// CSampleBrowerDoc diagnostics

#ifdef _DEBUG
void CSampleBrowerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSampleBrowerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerDoc commands
