///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////

// FaceWnd.cpp : implementation file
//

#include "stdafx.h"
#include "afd.h"
#include "FaceWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MORDA1 0
#define MORDA2 1

/////////////////////////////////////////////////////////////////////////////
// CFaceWnd

CFaceWnd::CFaceWnd()
{
}

CFaceWnd::~CFaceWnd()
{
}


BEGIN_MESSAGE_MAP(CFaceWnd, CStatic)
	//{{AFX_MSG_MAP(CFaceWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaceWnd message handlers

void CFaceWnd::PreSubclassWindow() 
{
	bFace = TRUE;
	m_bmpFace[MORDA1].LoadBitmap(IDB_MORDA);
	m_bmpFace[MORDA2].LoadBitmap(IDB_PHILOSOPHICAL_GNU);
	BITMAP BitInfo;
	m_bmpFace[MORDA1].GetObject(sizeof(BitInfo), &BitInfo);
	uTimer = SetTimer(1, 2000, NULL);
	CStatic::PreSubclassWindow();
}

void CFaceWnd::OnTimer(UINT nIDEvent) 
{
	CStatic::OnTimer(nIDEvent);
	bFace = !bFace;
	RedrawWindow();
}

void CFaceWnd::OnPaint() 
{
	if (bFace)
		DrawPic(MORDA1);
	else
		DrawPic(MORDA2);
}

void CFaceWnd::OnDestroy() 
{
	KillTimer(uTimer);
	CStatic::OnDestroy();
}

void CFaceWnd::DrawPic(int iPic)
{
	CPaintDC dc(this); // device context for painting
	BITMAP BitInfo;
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap* oldBitmap = MemDC.SelectObject(&m_bmpFace[iPic]);
	m_bmpFace[iPic].GetObject(sizeof(BitInfo), &BitInfo);
	CRect R;
	GetClientRect(&R);
	dc.BitBlt(R.left, R.top, BitInfo.bmWidth, BitInfo.bmHeight,
		&MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(oldBitmap);
}

void CFaceWnd::Draw2Pics(int iP1, int iP2, int p1y)
{
	CPaintDC dc(this); // device context for painting
	BITMAP BitInfo;
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap* oldBitmap = MemDC.SelectObject(&m_bmpFace[iP1]);
	m_bmpFace[iP1].GetObject(sizeof(BitInfo), &BitInfo);
	CRect R;
	GetClientRect(&R);
	dc.BitBlt(R.left, p1y, BitInfo.bmWidth, BitInfo.bmHeight,
		&MemDC, 0, 0, SRCCOPY);
	oldBitmap = MemDC.SelectObject(&m_bmpFace[iP2]);
	dc.BitBlt(R.left, R.top - p1y, BitInfo.bmWidth, BitInfo.bmHeight,
		&MemDC, 0, R.top - p1y, SRCCOPY);
	MemDC.SelectObject(oldBitmap);
}
