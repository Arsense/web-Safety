#if !defined(AFX_HYPERLINKER_H__A4601792_11A7_49E1_ABF7_9F2B3CD91EBF__INCLUDED_)
#define AFX_HYPERLINKER_H__A4601792_11A7_49E1_ABF7_9F2B3CD91EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperLinker.h : header file
//


class CHyperLinker : public CStatic
{
public:
	CHyperLinker();
	virtual ~CHyperLinker();

public:
    COLORREF m_InitColor;							//文本初始颜色
	COLORREF m_VisitedColor;						//文本被访问过后的颜色
    COLORREF m_CoverColor;							//鼠标悬浮在文本上方时的颜色
    
	BOOL     m_bAboveControl;                        // 是否鼠标移到控件上
    BOOL     m_bVisited;                            // 是否被访问
    BOOL     m_bUnderLine;                          // 是否有下划线
    BOOL     m_bAdjustToFit;                        // 是否自动调整控件大小
    
	CString  m_sURL;								// URL或者应用程序的位置
    CFont    m_Font;                                // 设定字体
    HCURSOR  m_hLinkCursor;                         // 光标

public:
	void SetAttribute(CString url="www.xici.net",COLORREF InitColor=RGB(0,0,255), COLORREF VisitedColor=RGB(255,0,0), 
                    COLORREF CoverColor=RGB(125,125,0),BOOL bUnderLine=TRUE);
	void HighLight(BOOL state);
	BOOL OpenUsingShellExecute();



	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLinker)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CHyperLinker)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINKER_H__A4601792_11A7_49E1_ABF7_9F2B3CD91EBF__INCLUDED_)
