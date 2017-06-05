// CntrItem.cpp : implementation of the CHTTPClientCntrItem class
//

#include "stdafx.h"
#include "HTTPClient.h"

#include "HTTPClientDoc.h"
#include "HTTPClientView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientCntrItem implementation

IMPLEMENT_SERIAL(CHTTPClientCntrItem, CRichEditCntrItem, 0)

CHTTPClientCntrItem::CHTTPClientCntrItem(REOBJECT* preo, CHTTPClientDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CHTTPClientCntrItem::~CHTTPClientCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientCntrItem diagnostics

#ifdef _DEBUG
void CHTTPClientCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CHTTPClientCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
