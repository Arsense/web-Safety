#pragma once





#define CRIT_RECTTRACKER    5

void AFXAPI AfxUnlockGlobals(int nLockType);



class CMyTracker
{
public:
	//当前矩形颜色
	COLORREF m_rectColor;
	//设置调整光标
	void SetResizeCursor(UINT nID_N_S,UINT nID_W_E,UINT nID_NW_SE,
		UINT nID_NE_SW,UINT nIDMiddle);
		CRect m_rectLast;
		void GetTrueRect(LPRECT lpTrueRect) ;
		//创建军画刷,内部调用,啥意思
		void CMyTracker::CreatePen();
		//设置矩形颜色
		void SetRectColor(COLORREF rectColor);
		virtual int GetHandleSize(LPCRECT lpRect = NULL) ;
		void CMyTracker::Draw(CDC* pDC) ;

	// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32,
		resizeMiddle =80 //设置中间
	};
	// Attributes
	UINT m_nStyle;      // current state
	CRect m_rect;       // current position (always in pixels)
	int m_nHandleSize;  // size of resize handles (default from WIN.INI)
	


	//**************************************************************

	CMyTracker(void);
	~CMyTracker(void);
};

