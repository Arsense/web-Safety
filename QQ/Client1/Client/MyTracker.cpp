#include "StdAfx.h"
#include "MyTracker.h"
#include"Resource.h"
#include"ClientDlg.h"



// various GDI objects we need to draw
AFX_STATIC_DATA HCURSOR _afxCursors[10] = { 0,};
AFX_STATIC_DATA HPEN _afxBlackDottedPen = 0;
AFX_STATIC_DATA HPEN _afxBlackSolidPen = 0;

CMyTracker::CMyTracker(void)
{
}


CMyTracker::~CMyTracker(void)
{
}

void CMyTracker::SetResizeCursor(UINT nID_N_S,UINT nID_W_E,UINT nID_NW_SE,UINT nID_NE_SW,UINT nIDMiddle)
{
	//////////////////////////////////////////////////////////////////////////////////
	//                             N            
	//               NW -----------|------------NE
	//                  |                      |
	//                  |					   |
	//				W	|                      |    E
	//					|        Middle        |
	//					|                      |
	//					|                      |
	//				  SW-----------|------------SE
	//                             S
	////////////////////////////////////////////////////////////////////////////////////

	_afxCursors[0] = AfxGetApp()->LoadCursor(nID_NW_SE);  //nw
	_afxCursors[1] = AfxGetApp()->LoadCursor(nID_NE_SW);  //ne
	_afxCursors[2] = _afxCursors[0];						//se	
	_afxCursors[3] = _afxCursors[1];						//sw
	_afxCursors[4] = AfxGetApp()->LoadCursor(nID_N_S);  //n
	_afxCursors[5] = AfxGetApp()->LoadCursor(nID_W_E);  //w
	_afxCursors[6] = _afxCursors[4];						//s
	_afxCursors[7] = _afxCursors[5];                        //e
	_afxCursors[8] = AfxGetApp()->LoadCursor(nIDMiddle);  //m
	//	_afxCursors[9] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_MOVE4WAY));
}

////////////////////增加的函数/////////////////////////////////////////////////////////////

void CMyTracker::SetRectColor(COLORREF rectColor)
{
	m_rectColor=rectColor;
	CreatePen();

}
void CMyTracker::CreatePen()
{


	//if (_afxBlackDottedPen == NULL)
	{
		// create black dotted pen
		_afxBlackDottedPen = ::CreatePen(PS_DOT, 0, m_rectColor);
		if (_afxBlackDottedPen == NULL)
		{
			AfxUnlockGlobals(CRIT_RECTTRACKER);
			AfxThrowResourceException();
		}
	}
	//if (_afxBlackSolidPen == NULL)
	{
		// create black dotted pen
		_afxBlackSolidPen = ::CreatePen(PS_SOLID, 0, m_rectColor);
		if (_afxBlackSolidPen == NULL)
		{
			AfxUnlockGlobals(CRIT_RECTTRACKER);
			AfxThrowResourceException();
		}
	}
}
void CMyTracker::GetTrueRect(LPRECT lpTrueRect) 
{
	ASSERT(AfxIsValidAddress(lpTrueRect, sizeof(RECT)));

	CRect rect = m_rect;
	rect.NormalizeRect();
	int nInflateBy = 0;
	if ((m_nStyle & (resizeOutside|hatchedBorder)) != 0)
		nInflateBy += GetHandleSize() - 1;
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		++nInflateBy;
	rect.InflateRect(nInflateBy, nInflateBy);
	*lpTrueRect = rect;
}
int CMyTracker::GetHandleSize(LPCRECT lpRect) 
{
	if (lpRect == NULL)
		lpRect = &m_rect;

	int size = m_nHandleSize;
	if (!(m_nStyle & resizeOutside))
	{
		// make sure size is small enough for the size of the rect
		int sizeMax = min(abs(lpRect->right - lpRect->left),
			abs(lpRect->bottom - lpRect->top));
		if (size * 2 > sizeMax)
			size = sizeMax / 2;
	}

	return size;
}
void CMyTracker::Draw(CDC* pDC) 
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
		{	
			//改变当前矩形颜色
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxBlackDottedPen));
		}
		else
		{
			//改变当前矩形颜色
			//pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN); //BLACK_PEN
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxBlackSolidPen));
		}

		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);
		rect.InflateRect(+1, +1);   // borders are one pixel outside
		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
		pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(OPAQUE);
		CRect rectTrue;
		GetTrueRect(&rectTrue);
		pDC->PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), 0x000F0001 /* Pn */);
		pDC->PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), 0x000F0001 /* Pn */);
	}

	// draw resize handles
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				//改变当前调整手柄矩形颜色,也就是那八个点
				pDC->FillSolidRect(rect, m_rectColor);
			}
		}
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}