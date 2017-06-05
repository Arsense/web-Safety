#if !defined(AFX_MEMDC_H__CA1D3541_7235_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_MEMDC_H__CA1D3541_7235_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////
// CMemDC 实现一个内存DC

class CMemDC : public CDC
{
private:
    CBitmap  m_bitmap;      // 实际的bitmap
    CBitmap* m_pOldBitmap;  // 原先在CMemDC中的位图
    CDC*     m_pDC;         // 保存从构造函数中传来的CDC
    CRect    m_rect;        // 画的矩形区域
    BOOL     m_bMemDC;      // 如果CDC是一个内存DC，则为TRUE

public:

    CMemDC(CDC* pDC) : CDC()
    {
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();//判断是否是一个内存DC
              
        if (m_bMemDC)    // 创建一个内存DC
        {
            pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else  // 这不是一个内存DC，我们只拷贝相关的DC部分以备打印
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
   
    ~CMemDC()
    {
        if (m_bMemDC) 
        {    
            // 拷贝实际的位图到屏幕上
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);
            //装入原始的位图
            SelectObject(m_pOldBitmap);
        } 
		else 
		{
            m_hDC = m_hAttribDC = NULL;
        }
    }

    // 允许作为指针
    CMemDC* operator->() {return this;}
    operator CMemDC*() {return this;}

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMDC_H__CA1D3541_7235_11D1_ABBA_00A0243D1382__INCLUDED_)
