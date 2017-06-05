// PortScanDoc.cpp : implementation of the CPortScanDoc class
//

#include "stdafx.h"
#include "PortScan.h"

#include "PortScanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortScanDoc

IMPLEMENT_DYNCREATE(CPortScanDoc, CDocument)

BEGIN_MESSAGE_MAP(CPortScanDoc, CDocument)
	//{{AFX_MSG_MAP(CPortScanDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortScanDoc construction/destruction

CPortScanDoc::CPortScanDoc()
{
	// TODO: add one-time construction code here

}

CPortScanDoc::~CPortScanDoc()
{
}

BOOL CPortScanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPortScanDoc serialization

void CPortScanDoc::Serialize(CArchive& ar)
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
// CPortScanDoc diagnostics

#ifdef _DEBUG
void CPortScanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPortScanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPortScanDoc commands
