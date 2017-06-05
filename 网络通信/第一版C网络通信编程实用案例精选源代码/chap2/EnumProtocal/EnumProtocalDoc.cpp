// EnumProtocalDoc.cpp : implementation of the CEnumProtocalDoc class
//

#include "stdafx.h"
#include "EnumProtocal.h"

#include "EnumProtocalDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnumProtocalDoc

IMPLEMENT_DYNCREATE(CEnumProtocalDoc, CDocument)

BEGIN_MESSAGE_MAP(CEnumProtocalDoc, CDocument)
	//{{AFX_MSG_MAP(CEnumProtocalDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumProtocalDoc construction/destruction

CEnumProtocalDoc::CEnumProtocalDoc()
{
	// TODO: add one-time construction code here

}

CEnumProtocalDoc::~CEnumProtocalDoc()
{
}

BOOL CEnumProtocalDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEnumProtocalDoc serialization

void CEnumProtocalDoc::Serialize(CArchive& ar)
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
// CEnumProtocalDoc diagnostics

#ifdef _DEBUG
void CEnumProtocalDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEnumProtocalDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEnumProtocalDoc commands
