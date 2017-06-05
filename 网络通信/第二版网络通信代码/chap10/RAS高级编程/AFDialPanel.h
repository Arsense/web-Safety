

#if !defined(AFX_AFDIALPANEL_H__B8442F65_C480_11D2_9864_D69D2B433156__INCLUDED_)
#define AFX_AFDIALPANEL_H__B8442F65_C480_11D2_9864_D69D2B433156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AFDialPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAFDialPanel window

#include "AlexfPerf.h"

#define STACKSIZ 300

#define PERF_ConnectSpeed	0
#define PERF_BytesRecvd		1
#define PERF_BytesXmit		2
#define PERF_ARRAY			3

class CAFDialPanel : public CStatic
{
// Construction
public:
	CAFDialPanel();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAFDialPanel)
	public:
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CAlexfPerf * pPerf[PERF_ARRAY];
	ULONG uTimer;
	CString text[10];
	int TextLines;
	int LeftWidth;
	DWORD * Recv;
	DWORD * Send;
	DWORD TotalRecv;
	DWORD TotalSend;
	int lStack;
	DWORD MaxSpeed;
	virtual ~CAFDialPanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAFDialPanel)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFDIALPANEL_H__B8442F65_C480_11D2_9864_D69D2B433156__INCLUDED_)
