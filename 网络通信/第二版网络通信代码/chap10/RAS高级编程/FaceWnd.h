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

#if !defined(AFX_FACEWND_H__69893725_9A14_11D3_8419_C34A0E970D48__INCLUDED_)
#define AFX_FACEWND_H__69893725_9A14_11D3_8419_C34A0E970D48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaceWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFaceWnd window

class CFaceWnd : public CStatic
{
// Construction
public:
	CFaceWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceWnd)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL bFace;
	void Draw2Pics(int iP1, int iP2, int p1y);
	void DrawPic(int iPic);
	CBitmap m_bmpFace[2];
	ULONG uTimer;
	virtual ~CFaceWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFaceWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACEWND_H__69893725_9A14_11D3_8419_C34A0E970D48__INCLUDED_)
