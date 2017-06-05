#if !defined(AFX_NETTRAFFICBUTTONDLG_H__F1CE9C31_5EF8_4B96_8897_29C1FAE99A6E__INCLUDED_)
#define AFX_NETTRAFFICBUTTONDLG_H__F1CE9C31_5EF8_4B96_8897_29C1FAE99A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MFTrafficButton.h"


class CNetTrafficButtonDlg : public CDialog
{

public:
	CNetTrafficButtonDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

	//{{AFX_DATA(CNetTrafficButtonDlg)
	enum { IDD = IDD_NETTRAFFICBUTTON_DIALOG };
	MFTrafficButton	m_cOutgoing;
	MFTrafficButton	m_cIncoming;
	MFTrafficButton	m_cTotalTraffic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CNetTrafficButtonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	//{{AFX_MSG(CNetTrafficButtonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_NETTRAFFICBUTTONDLG_H__F1CE9C31_5EF8_4B96_8897_29C1FAE99A6E__INCLUDED_)
