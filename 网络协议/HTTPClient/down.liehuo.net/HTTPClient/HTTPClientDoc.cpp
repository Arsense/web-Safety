// HTTPClientDoc.cpp : implementation of the CHTTPClientDoc class
// Download by http://www.codefans.net

#include "stdafx.h"
#include "HTTPClient.h"

#include "HTTPClientDoc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientDoc

IMPLEMENT_DYNCREATE(CHTTPClientDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CHTTPClientDoc, CRichEditDoc)
	//{{AFX_MSG_MAP(CHTTPClientDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientDoc construction/destruction

CHTTPClientDoc::CHTTPClientDoc()
{
	// Use OLE compound files
	// EnableCompoundFile();

	// TODO: add one-time construction code here

}

CHTTPClientDoc::~CHTTPClientDoc()
{
}

BOOL CHTTPClientDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CHTTPClientDoc::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CHTTPClientCntrItem(preo, (CHTTPClientDoc*) this);
}



/////////////////////////////////////////////////////////////////////////////
// CHTTPClientDoc serialization

void CHTTPClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientDoc diagnostics

#ifdef _DEBUG
void CHTTPClientDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CHTTPClientDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientDoc commands
