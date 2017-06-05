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

// AFDialPanel.cpp : implementation file
//

#include "stdafx.h"
#include "AFDialPanel.h"
#include "afd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CAfdApp;
extern CAfdApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAFDialPanel

CAFDialPanel::CAFDialPanel()
{
	Recv = new DWORD[STACKSIZ * sizeof(DWORD)];
	if (!Recv) return;
	Send = new DWORD[STACKSIZ * sizeof(DWORD)];
	if (!Send)
	{
		delete [] Recv;
		return;
	}
	for (int i = 0; i < STACKSIZ; i++)
	{
		Recv[i] = 0;
		Send[i] = 0;
	}
	TextLines = 7;
	LeftWidth = 50;
	text[0] = _T("接收");
	text[2] = _T("速度 ");
	text[4] = _T("Kb/sec");
	text[6] = _T("发送");
}

CAFDialPanel::~CAFDialPanel()
{
	if (Recv) delete [] Recv;
	if (Send) delete [] Send;
}


BEGIN_MESSAGE_MAP(CAFDialPanel, CStatic)
	//{{AFX_MSG_MAP(CAFDialPanel)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAFDialPanel message handlers

//动态显示流量数据
void CAFDialPanel::OnPaint() 
{
	if (!Recv) return;
	if (!Send) return;
	CPaintDC dc(this); // device context for painting
	CRect cr;
	GetClientRect(&cr);
	CRect cr1(cr);
// fill text
	text[1] = L2S(Recv[lStack]);
	text[3] = D2S_2((double)MaxSpeed / 1000.0);
	text[5] = L2S(Send[lStack]);
// text
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetSysColor(COLOR_BTNTEXT));
	int hh = cr.Height() / TextLines;
	cr1.right = cr1.left + LeftWidth;
	dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_BTNFACE)));
	cr1.bottom = cr1.top + hh;
	CFont* pOldFont = dc.SelectObject(GetFont());
	for (int i = 0; i < TextLines; i++)
	{
		dc.DrawText(text[i], (LPRECT)&cr1,
		DT_WORDBREAK | DT_CENTER | DT_VCENTER);
		cr1.OffsetRect(0, hh);
	}
	dc.SelectObject(pOldFont);
// axis
	cr1 = CRect(cr);
	cr1.left = cr.left + LeftWidth;
	cr1.right = cr1.left + 1;
	dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_APPWORKSPACE)));
	cr1.right = cr.right;
	cr1.top = cr.Height() / 2;
	cr1.bottom = cr1.top + 1;
	dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_APPWORKSPACE)));

	if (0 == MaxSpeed) return;
// graph
	i = lStack;
	int h = cr.Height() / 2 - 1;
	cr1.left = cr.left + LeftWidth + 1;
	cr1.right = cr1.left + 2;
//	int ccc = 0;
	while (TRUE)
	{
		int h1 = 10 * h * Recv[i] / MaxSpeed;
		if (h1 < h)
		{
			cr1.top = cr.top;
			cr1.bottom = cr.top + h - h1;
			// clear
			dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_BTNFACE)));
		}
		if (h1 > h)
		{
			h1 = h - 2;
			cr1.top = cr.top;
			cr1.bottom = cr.top + 2;
			// chache hit
			dc.FillRect(& cr1, & CBrush(RGB(255, 0, 0)));
		}
		if (0 == h1 && 0 != Recv[i]) h1 = 1;
		cr1.bottom = cr.Height() / 2 - 1;
		cr1.top = cr1.bottom - h1;
		dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_BTNHIGHLIGHT)));// ccc)));
//		ccc++;
		h1 = 10 * h * Send[i] / MaxSpeed;
		if (h1 < h)
		{
			cr1.bottom = cr.bottom;
			cr1.top = cr.bottom - h + h1;
			// clear
			dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_BTNFACE)));
		}
		if (h1 > h)
		{
			h1 = h - 2;
			cr1.bottom = cr.bottom;
			cr1.top = cr.top;
			cr1.top = cr.bottom - 2;
			// chache hit
			dc.FillRect(& cr1, & CBrush(RGB(255, 0, 0)));
		}
		if (0 == h1 && 0 != Send[i]) h1 = 1;
		cr1.top = cr.Height() / 2 + 1;
		cr1.bottom = cr1.top + h1;
		dc.FillRect(& cr1, & CBrush(GetSysColor(COLOR_HIGHLIGHT)));
		cr1.OffsetRect(3, 0);
		if (0 == i) i = STACKSIZ;
		i--;
		if (cr1.left > cr.right - 2) break;
	}
}

void CAFDialPanel::OnTimer(UINT nIDEvent) 
{
	if (!Recv) return;
	if (!Send) return;
	lStack++;
	if (lStack >= STACKSIZ) lStack = 0;
	MaxSpeed = theApp.pPerf[PERF_ConnectSpeed]->GetData();
	DWORD dRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
	DWORD dSend = theApp.pPerf[PERF_BytesXmit]->GetData();
	Recv[lStack] = dRecv - TotalRecv;
	Send[lStack] = dSend - TotalSend;
	TotalRecv = dRecv;
	TotalSend = dSend;
	RedrawWindow();
	CStatic::OnTimer(nIDEvent);
}

void CAFDialPanel::PreSubclassWindow() 
{
	TotalRecv = 0;
	TotalSend = 0;
	lStack = 0;
	MaxSpeed = 0;
	pPerf[PERF_ConnectSpeed] = new CAlexfPerf(ALEXF_PERF_DIALUP_CONNECTSPEED);
	pPerf[PERF_BytesRecvd]   = new CAlexfPerf(ALEXF_PERF_DIALUP_BYTESRECVD);
	pPerf[PERF_BytesXmit]    = new CAlexfPerf(ALEXF_PERF_DIALUP_BYTESXMIT);
	for (int i = 0; i < PERF_ARRAY; i++) if (NULL == pPerf[i]) VERIFY(0);
	TotalRecv = theApp.pPerf[PERF_BytesRecvd]->GetData();
	TotalSend = theApp.pPerf[PERF_BytesXmit]->GetData();
	uTimer = SetTimer(1, 1000, NULL);
	CStatic::PreSubclassWindow();
}

void CAFDialPanel::OnDestroy() 
{
	for (int i = 0; i < PERF_ARRAY; i++) delete pPerf[i];
	KillTimer(uTimer);
	CStatic::OnDestroy();
}
