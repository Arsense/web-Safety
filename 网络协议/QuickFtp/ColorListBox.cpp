// ColorListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "ColorListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListBox

CColorListBox::CColorListBox()
{
}

CColorListBox::~CColorListBox()
{
}


BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	//{{AFX_MSG_MAP(CColorListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorListBox message handlers

/********************************************************************/
/*																	*/
/* Function name : DrawItem											*/		
/* Description   : Called by the framework when a visual aspect of	*/
/*				   an owner-draw list box changes.					*/
/*																	*/
/********************************************************************/
void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// Losing focus ?
    if (lpDrawItemStruct->itemID == -1) 
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem);
		return;
    }

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	COLORREF clrOld;
	CString sText;
	
	// get color info from item data
	COLORREF clrNew = (COLORREF)(lpDrawItemStruct->itemData);

	// item selected ?
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brush(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}

	// item deselected ?
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&	
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		CBrush brush(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}	 	

	// item has focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
	}

	// lost focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
	}

	// set the background mode to TRANSPARENT
	int nBkMode = pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		clrOld = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else 
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
		clrOld = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		clrOld = pDC->SetTextColor(clrNew);

	// get item text
	GetText(lpDrawItemStruct->itemID, sText);
	CRect rect = lpDrawItemStruct->rcItem;

	// text format
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;
	
	// draw the text
	pDC->DrawText(sText, -1, &rect, nFormat);

	// restore old values
	pDC->SetTextColor(clrOld); 
	pDC->SetBkMode(nBkMode);
}

void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENUCHECK);
}

/********************************************************************/
/*																	*/
/* Function name : AddString										*/		
/* Description   : Add string to the listbox and save color info.	*/
/*																	*/
/********************************************************************/
int CColorListBox::AddString(LPCTSTR lpszItem, COLORREF itemColor)
{
	// Add the string to the list box
	int nIndex = CListBox::AddString(lpszItem);
   
	// save color data
	if (nIndex >= 0)
		SetItemData(nIndex, itemColor);

	return nIndex;
}
