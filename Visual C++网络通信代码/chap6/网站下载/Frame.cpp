
/*
	Frame.cpp : implementation of the CSnaggerFrame class

	Implements the frame window for SiteSnagger's SDI view.

	Author: Steven E. Sipe
*/

#include "stdafx.h"
#include "SiteSnag.h"

#include "Frame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSnaggerFrame

IMPLEMENT_DYNCREATE(CSnaggerFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CSnaggerFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSnaggerFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Status line contains nothing but the menu item field
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CSnaggerFrame construction/destruction

// Constructor
CSnaggerFrame::CSnaggerFrame()
{
}

// Destructor
CSnaggerFrame::~CSnaggerFrame()
{
}

// Creates the frame window, toolbar and statusbar
int CSnaggerFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the toolbar
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Create the statusbar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

// Called before the window is created to allow overrides (NOT USED)
BOOL CSnaggerFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSnaggerFrame diagnostics

#ifdef _DEBUG
void CSnaggerFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CSnaggerFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSnaggerFrame message handlers
