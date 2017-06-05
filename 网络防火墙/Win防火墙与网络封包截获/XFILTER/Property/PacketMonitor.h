#if !defined(AFX_PACKETMONITOR_H__4D51B86C_1651_4381_BA05_E300BA08A241__INCLUDED_)
#define AFX_PACKETMONITOR_H__4D51B86C_1651_4381_BA05_E300BA08A241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketMonitor.h : header file
//

//=============================================================================================
// CPacketMonitor dialog

class CPacketMonitor : public CPropertyPage
{
	DECLARE_DYNCREATE(CPacketMonitor)

// Construction
public:
	CPacketMonitor();
	~CPacketMonitor();

// Dialog Data
	//{{AFX_DATA(CPacketMonitor)
	enum { IDD = IDD_PACKET_MONITOR };
	CListCtrl	m_ListPacketMonitor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPacketMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPacketMonitor)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMonitor();
	afx_msg void OnButtonScroll();
	afx_msg void OnButtonClear();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//---------------------------------------------------------------------------------------------
//owner add
public:
	void			InitDlgResource();
	static void		InitList(CListCtrl * mList);

	static int InsertOneList(
		CListCtrl * mList, 
		SESSION *session, 
		BOOL mIsAdd		= TRUE, 
		BOOL mIsScroll	= FALSE
		);

public:
	static BOOL			IsScroll;
	static BOOL			IsMonitor;
	static CListCtrl	*m_pListPacketMonitor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETMONITOR_H__4D51B86C_1651_4381_BA05_E300BA08A241__INCLUDED_)
