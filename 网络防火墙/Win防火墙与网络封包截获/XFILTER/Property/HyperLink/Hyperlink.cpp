//=============================================================================================
/*
	文件：		HyperLink.cpp

	说明:
	---------------------------------------------------
		从 CStatic 继承而来的超级链接类，将静态文本框
	以超级链接形式显示。
	---------------------------------------------------

	工程：		Xfilter 个人防火墙
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/06
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	---------------------------------------------------
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买本软件源代码的用户被默认授权
	可以在自己的程序中使用本软件的部分代码，但作者不对
	代码产生的任何后果负责。
	
		使用了本软件代码的程序只能以可执行文件形式发布，
	未经特别许可，不能将含有本软件源代码的源程序以任何
	形式发布。
	---------------------------------------------------	
*/
//=============================================================================================

#include "stdafx.h"
#include "HyperLink.h"

CHyperLink::CHyperLink()
{
    m_hLinkCursor		= NULL;                
    m_LinkColor			= RGB(0, 0, 255);  
    m_VisitedColor		= RGB(0, 0, 255);  
    m_HoverColor		= RGB(242, 101, 34);
    m_sUrl		        = "";
	m_bIsHover			= FALSE;
	m_bIsVisited		= FALSE;
	m_bUnderline		= TRUE;
}

CHyperLink::~CHyperLink()
{
    m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
    //{{AFX_MSG_MAP(CHyperLink)
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHyperLink::OnClicked()
{
    HINSTANCE result = ShellExecute(NULL
	   , _T("open"), m_sUrl, NULL,NULL, SW_SHOW);

	m_bIsVisited = TRUE;
}

HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    if (m_bIsHover)
        pDC->SetTextColor(m_HoverColor);
    else if (m_bIsVisited)
        pDC->SetTextColor(m_VisitedColor);
    else
        pDC->SetTextColor(m_LinkColor);

    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
    CStatic::OnMouseMove(nFlags, point);

    if (m_bIsHover)        
    {
        CRect rect;
        GetClientRect(rect);

        if (!rect.PtInRect(point))
        {
            m_bIsHover = FALSE;
            ReleaseCapture();
            RedrawWindow();
            return;
        }
    }
    else                      
    {
        m_bIsHover = TRUE;
        RedrawWindow();
        SetCapture();
    }
}

BOOL CHyperLink::OnSetCursor(CWnd*, UINT, UINT) 
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

BOOL CHyperLink::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

void CHyperLink::PreSubclassWindow() 
{
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
    
    if (m_sUrl.IsEmpty())
        GetWindowText(m_sUrl);

    CString strWndText;
    GetWindowText(strWndText);
    if (strWndText.IsEmpty()) 
        SetWindowText(m_sUrl);

    LOGFONT lf;
    GetFont()->GetLogFont(&lf);
    lf.lfUnderline = m_bUnderline;
    m_Font.CreateFontIndirect(&lf);
    SetFont(&m_Font);

    SetCursor();     

    CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_sUrl, rect, 1);

    CStatic::PreSubclassWindow();
}

void CHyperLink::SetCursor()
{
    if (m_hLinkCursor == NULL)                
    {
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hLinkCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
    }
}

//=====================================================================
// public function

void CHyperLink::SetURL(CString sUrl)
{
    m_sUrl = sUrl;

    if (::IsWindow(GetSafeHwnd())) 
        m_ToolTip.UpdateTipText(sUrl, this, 1);
}
