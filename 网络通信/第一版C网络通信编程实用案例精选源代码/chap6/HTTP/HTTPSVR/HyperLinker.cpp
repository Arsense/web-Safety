// HyperLinker.cpp : implementation file
//

#include "stdafx.h"
#include "HyperLinker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLinker

CHyperLinker::CHyperLinker()
{
	m_bVisited=FALSE;
	m_bAboveControl=FALSE;
}

CHyperLinker::~CHyperLinker()
{
 int i;
 i=10;
}


BEGIN_MESSAGE_MAP(CHyperLinker, CStatic)
	//{{AFX_MSG_MAP(CHyperLinker)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHyperLinker::SetAttribute(CString url,COLORREF InitColor, COLORREF VisitedColor, COLORREF CoverColor,BOOL bUnderLine)
{
	m_sURL=url;
	m_InitColor=InitColor;
	m_VisitedColor=VisitedColor;
	m_CoverColor=CoverColor;
	m_bUnderLine=bUnderLine;
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLinker message handlers
void CHyperLinker::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	static BOOL bIsIn=FALSE;	//判断是否前一此鼠标就已经在static控件区域类
	if (rect.PtInRect(point))
	{
		m_bAboveControl=TRUE;
		if(bIsIn==FALSE)
		{
			SetCapture();
			bIsIn=TRUE;
			Invalidate();
		} 
	}
	else
	{
		m_bAboveControl=FALSE;
		if (bIsIn==TRUE)
		{
			ReleaseCapture();
			bIsIn=FALSE;
			Invalidate();
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}


void CHyperLinker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OpenUsingShellExecute();
	m_bVisited=TRUE;
//	UpdateWindow();
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}


void CHyperLinker::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	// 获得鼠标事件
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

BOOL CHyperLinker::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR  LinkCursor = ::AfxGetApp()->LoadCursor(IDC_CURSOR1);
	::SetCursor(LinkCursor);
	return TRUE;			//必须包括这句话
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CHyperLinker::OpenUsingShellExecute()
{
	HINSTANCE hRun = ShellExecute(GetParent()->GetSafeHwnd(), _T("open"), m_sURL, NULL, NULL, SW_SHOW);
	if ((int) hRun <= 32)
	{
		AfxMessageBox(_T("提供的超级链接或者制定的文件无法执行"));
		return FALSE;
	}
	return TRUE;
}



HBRUSH CHyperLinker::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	ASSERT(nCtlColor == CTLCOLOR_STATIC);

	DWORD dwStyle = GetStyle();
	if (!(dwStyle & SS_NOTIFY)) 
	{
		// Turn on notify flag to get mouse messages and STN_CLICKED.
		// Otherwise, I'll never get any mouse clicks!
		::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);
	}

	HBRUSH hbr = NULL;
	if ((dwStyle & 0xFF) <= SS_RIGHT) 
	{
		// Modify the font to be underline
		if (!((HFONT) m_Font))
		{
			LOGFONT lf;
			GetFont()->GetObject(sizeof(lf), &lf);

			lf.lfUnderline = m_bUnderLine;
			m_Font.CreateFontIndirect(&lf);
		}
		pDC->SelectObject(&m_Font);

		//set the text colors
		if(m_bVisited==TRUE)
		{
			pDC->SetTextColor(m_VisitedColor);
//			AfxMessageBox("Click");
		}
		else
		{
			if(m_bAboveControl==TRUE)
			{
				pDC->SetTextColor(m_CoverColor);
//				AfxMessageBox("Above");
			}
			else
			{
				pDC->SetTextColor(this->m_InitColor);
//				AfxMessageBox("init");
			}
		}
		pDC->SetBkMode(TRANSPARENT);
		// return hollow brush to preserve parent background color
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	return hbr;
}
