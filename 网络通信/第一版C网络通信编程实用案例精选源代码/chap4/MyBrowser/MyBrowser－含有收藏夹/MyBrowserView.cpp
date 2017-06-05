// MyBrowserView.cpp : implementation of the CMyBrowserView class
//

#include "stdafx.h"
#include "MyBrowser.h"

#include "MyBrowserDoc.h"
#include "MyBrowserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserView

IMPLEMENT_DYNCREATE(CMyBrowserView, CHtmlView)

BEGIN_MESSAGE_MAP(CMyBrowserView, CHtmlView)
	//{{AFX_MSG_MAP(CMyBrowserView)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_REFRESH, OnGoRefresh)
	ON_COMMAND(ID_GO_SERACH, OnGoSerach)
	ON_COMMAND(ID_GO_STOP, OnGoStop)
	ON_COMMAND(ID_START_PAGE, OnStartPage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserView construction/destruction

CMyBrowserView::CMyBrowserView()
{
	// TODO: add construction code here

}

CMyBrowserView::~CMyBrowserView()
{
}

BOOL CMyBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserView drawing

void CMyBrowserView::OnDraw(CDC* pDC)
{
	CMyBrowserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CMyBrowserView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	// TODO: This code navigates to a popular spot on the web.
	//  change the code to go where you'd like.
//	Navigate2(_T("http://www.microsoft.com/visualc/"),NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserView printing


/////////////////////////////////////////////////////////////////////////////
// CMyBrowserView diagnostics

#ifdef _DEBUG
void CMyBrowserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CMyBrowserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CMyBrowserDoc* CMyBrowserView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyBrowserDoc)));
	return (CMyBrowserDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserView message handlers

void CMyBrowserView::OnGoBack() 
{
	// TODO: Add your command handler code here
	GoBack();
}

void CMyBrowserView::OnGoForward() 
{
	// TODO: Add your command handler code here
	GoForward();
}

void CMyBrowserView::OnGoRefresh() 
{
	// TODO: Add your command handler code here
	Refresh();
}

void CMyBrowserView::OnGoSerach() 
{
	// TODO: Add your command handler code here
	GoSearch();
}

void CMyBrowserView::OnGoStop() 
{
	// TODO: Add your command handler code here
	Stop();
}

void CMyBrowserView::OnStartPage() 
{
	// TODO: Add your command handler code here
	GoHome();	
}
