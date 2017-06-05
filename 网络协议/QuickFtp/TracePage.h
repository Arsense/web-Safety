#if !defined(AFX_TRACEPAGE_H__9FB90543_7617_4587_9C6F_AFF4EA856E75__INCLUDED_)
#define AFX_TRACEPAGE_H__9FB90543_7617_4587_9C6F_AFF4EA856E75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracePage.h : header file
//
#include "resource.h"
#include "ColorListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CTracePage dialog

class CTracePage : public CDialog
{
// Construction
public:
	void AddTraceLine(int nLevel,LPCTSTR pstrFormat,...);
	CTracePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTracePage)
	enum { IDD = IDD_PROPPAGE_TRACE };
	CColorListBox	m_TraceList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTracePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTracePage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTraceClear();
	//}}AFX_MSG
	LRESULT OnAddTraceLine(WPARAM, LPARAM);//自定义消息响应!!!!!!!!!!!!!!
	DECLARE_MESSAGE_MAP()

	CObList m_LogQueue;
	CCriticalSection m_QueueLock;

	class CLogMsg : public CObject
	{
	public:
		CLogMsg() {};
		virtual ~CLogMsg() {};

		SYSTEMTIME m_sysTime;
		int        m_nLevel;
		CString    m_strText;
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEPAGE_H__9FB90543_7617_4587_9C6F_AFF4EA856E75__INCLUDED_)
