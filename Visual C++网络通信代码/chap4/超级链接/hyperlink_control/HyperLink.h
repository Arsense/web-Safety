// HyperLink.h : header file
//
//
// HyperLink static control. Will open the default browser with the given URL
// when the user clicks on the link.
//
// Copyright Chris Maunder, 1997, 1998
// Feel free to use and distribute. May not be sold for profit. 

#if !defined(AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CHyperLink window

class CHyperLink : public CStatic
{
// Construction/destruction
public:
    CHyperLink();
    virtual ~CHyperLink();

// Attributes
public:

// Operations
public:
	//设定URL
    void SetURL(CString strURL);
    CString GetURL() const;
	//设定颜色
    void SetColours(COLORREF crLinkColour, COLORREF crVisitedColour, 
                    COLORREF crHoverColour = -1);
    //获得连接颜色
	COLORREF GetLinkColour() const;
	//获得被访问后的颜色
    COLORREF GetVisitedColour() const;
	//获得鼠标移动上以后的颜色
    COLORREF GetHoverColour() const;

	//设定是否被访问过
    void SetVisited(BOOL bVisited = TRUE);
	//获得是否被访问过
    BOOL GetVisited() const;
	
	//设定鼠标形状
    void SetLinkCursor(HCURSOR hCursor);
	//获得鼠标形状
    HCURSOR GetLinkCursor() const;
	//设定是否有下划线
    void SetUnderline(BOOL bUnderline = TRUE);
	//获得是否有下划线
    BOOL GetUnderline() const;
	//设定是否是自动改变大小
    void SetAutoSize(BOOL bAutoSize = TRUE);
    BOOL GetAutoSize() const;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHyperLink)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

protected:
	//连接到URL
    HINSTANCE GotoURL(LPCTSTR url, int showcmd);
	//打印错误
    void ReportError(int nError);
	//获得注册表信息
    LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	//调整位置
    void PositionWindow();
	//设定默认的鼠标形状
    void SetDefaultCursor();

// 变量
protected:
    COLORREF m_crLinkColour, m_crVisitedColour;     // 超级连接颜色
    COLORREF m_crHoverColour;                       // 鼠标停留颜色
    BOOL     m_bOverControl;                        // 是否鼠标移到控件上
    BOOL     m_bVisited;                            // 是否被访问
    BOOL     m_bUnderline;                          // 是否有下划线
    BOOL     m_bAdjustToFit;                        // 是否自动调整控件大小
    CString  m_strURL;                              // URL
    CFont    m_Font;                                // 设定字体
    HCURSOR  m_hLinkCursor;                         // 光标
    CToolTipCtrl m_ToolTip;                         // 提示文字

    // Generated message map functions
protected:
    //{{AFX_MSG(CHyperLink)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    afx_msg void OnClicked();
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
