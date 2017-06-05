// SampleBrowerView.cpp : implementation of the CSampleBrowerView class
//

#include "stdafx.h"
#include "SampleBrower.h"

#include "SampleBrowerDoc.h"
#include "SampleBrowerView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerView

IMPLEMENT_DYNCREATE(CSampleBrowerView, CHtmlView)

BEGIN_MESSAGE_MAP(CSampleBrowerView, CHtmlView)
	//{{AFX_MSG_MAP(CSampleBrowerView)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_REFRESH, OnGoRefresh)
	ON_COMMAND(ID_GO_SERACH, OnGoSerach)
	ON_COMMAND(ID_GO_STOP, OnGoStop)
	ON_COMMAND(ID_START_PAGE, OnStartPage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerView construction/destruction

CSampleBrowerView::CSampleBrowerView()
{
	// TODO: add construction code here

}

CSampleBrowerView::~CSampleBrowerView()
{
}

BOOL CSampleBrowerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerView drawing

void CSampleBrowerView::OnDraw(CDC* pDC)
{
	CSampleBrowerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CSampleBrowerView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	// TODO: This code navigates to a popular spot on the web.
	//  change the code to go where you'd like.
	GoHome();
}

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerView printing


/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerView diagnostics

#ifdef _DEBUG
void CSampleBrowerView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CSampleBrowerView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CSampleBrowerDoc* CSampleBrowerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleBrowerDoc)));
	return (CSampleBrowerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleBrowerView message handlers

void CSampleBrowerView::OnGoForward() 
{
	//前进
	GoForward();
}

void CSampleBrowerView::OnGoBack() 
{
	// 后退
	GoBack(); 
}

void CSampleBrowerView::OnGoRefresh() 
{
	// 刷新
	Refresh();
}

void CSampleBrowerView::OnGoSerach() 
{
	// 搜索
	GoSearch();
}

void CSampleBrowerView::OnGoStop() 
{
	// 停止
	Stop();
}

void CSampleBrowerView::OnStartPage() 
{
	//开始页面，主页
	GoHome();
}

void CSampleBrowerView::OnDocumentComplete(LPCTSTR lpszURL) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CMainFrame*)GetParentFrame())->m_wndAddress.SetWindowText(lpszURL);
}
