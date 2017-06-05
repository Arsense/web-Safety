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

// AfdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Afd.h"
#include "AfdDlg.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include "page4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAfdDlg dialog

extern CAfdApp theApp;

CAfdDlg::CAfdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAfdDlg::IDD, pParent)
{
	//打开对话框的时候载入属性页
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	page1 = new CPage1;
	page2 = new CPage2;
	page3 = new CPage3;
	page4 = new CPage4;
}

void CAfdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAfdDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAfdDlg, CDialog)
	//{{AFX_MSG_MAP(CAfdDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotify)
	ON_COMMAND(IDM_SHOW, OnShow)
	ON_WM_NCPAINT()
	ON_COMMAND(IDM_TRAY_DIAL, OnTrayDial)
	ON_UPDATE_COMMAND_UI(IDM_TRAY_DIAL, OnUpdateTrayDial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAfdDlg message handlers
/*
void Stamp01(CWnd * w, int DotSize, int Shift)
{
	CRect rectcl, rect;
	w->GetClientRect(rectcl);
	w->GetWindowRect(rect);
	int NCHeight = rect.Height() - rectcl.Height();
	w->ScreenToClient(rect);
	CRgn rgn1, rgn2, rgn3;
	rgn1.CreateRectRgn(0, 0, rect.Width(), rect.Height());
	rgn3.CreateRectRgn(0, 0, rect.Width(), rect.Height());
	rgn2.CreateEllipticRgn(0, 0, DotSize, DotSize);
	rgn2.OffsetRgn(- DotSize / 2, rect.Height() - DotSize / 2);
	rgn2.OffsetRgn(Shift, 0);
	for (int x = 0; x < rect.Width() * 3 / 2 / DotSize + 1; x++)
	{
		rgn3.CombineRgn(&rgn1, &rgn2, RGN_DIFF);
		rgn1.CopyRgn(&rgn3);
		rgn2.OffsetRgn(DotSize* 3 / 2, 0);
	}
	CRgn rgn4;
	rgn4.CreateEllipticRgn(0, 0, DotSize, DotSize);
	rgn4.OffsetRgn(- DotSize / 2, NCHeight);
	rgn4.OffsetRgn(0, Shift);
	for (x = 0; x < (rect.Height() - NCHeight) * 3 / 2 / DotSize + 1; x++)
	{
		rgn3.CombineRgn(&rgn1, &rgn4, RGN_DIFF);
		rgn1.CopyRgn(&rgn3);
		rgn4.OffsetRgn(0, DotSize * 3 / 2);
	}
	CRgn rgn5;
	rgn5.CreateEllipticRgn(0, 0, DotSize, DotSize);
	rgn5.OffsetRgn(rect.Width() - DotSize / 2, NCHeight);
	rgn5.OffsetRgn(0, - Shift);
	for (x = 0; x < (rect.Height() - NCHeight) * 3 / 2 / DotSize + 1; x++)
	{
		rgn3.CombineRgn(&rgn1, &rgn5, RGN_DIFF);
		rgn1.CopyRgn(&rgn3);
		rgn5.OffsetRgn(0, DotSize * 3 / 2);
	}
	w->SetWindowRgn(rgn3, TRUE);
}*/

BOOL CAfdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化设置
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//设置图标
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if (!page1) return FALSE;
	if (!page2) return FALSE;
	if (!page3) return FALSE;
	if (!page4) return FALSE;

	//增加属性页到对话框上
	m_dlgPropSheet.AddPage(page1);
	m_dlgPropSheet.AddPage(page2);
	m_dlgPropSheet.AddPage(page3);
	m_dlgPropSheet.AddPage(page4);
	//创建属性页表单，用来容纳属性页
    m_dlgPropSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
    m_dlgPropSheet.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
    m_dlgPropSheet.ModifyStyle(0, WS_TABSTOP);
    m_dlgPropSheet.SetWindowPos(NULL, 5, 5, 0, 0,
    SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	if (!m_dlgPropSheet.SetActivePage(page4)) return FALSE;
	if (!m_dlgPropSheet.SetActivePage(page3)) return FALSE;
	if (!m_dlgPropSheet.SetActivePage(page2)) return FALSE;
	if (!m_dlgPropSheet.SetActivePage(page1)) return FALSE;

	// 初始化属性页
	if (!page3->InitPropertyPage()) return FALSE;
	if (!page1->InitPropertyPage()) return FALSE;
	if (!page2->InitPropertyPage()) return FALSE;
	if (!page4->InitPropertyPage()) return FALSE;
	m_bStartMinimized = page3->m_bInterfaceStartMinimized;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAfdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (IDM_ABOUTBOX == (nID & 0xFFF0))
	{
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAfdDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAfdDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAfdDlg::OnOK() 
{
	return;
//	CDialog::OnOK();
}

void CAfdDlg::OnCancel() 
{
	CDialog::OnOK();
//	CDialog::OnCancel();
}

void CAfdDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	if (page1){page1->DestroyWindow(); delete page1;}
	if (page2){page2->DestroyWindow(); delete page2;}
	if (page3){page3->DestroyWindow(); delete page3;}
	if (page4){page4->DestroyWindow(); delete page4;}
}

void CAfdDlg::OnHide() 
{
	static BOOL bTrayIconCreated = FALSE;
	if (!bTrayIconCreated)
	{
		bTrayIconCreated = TRUE;
		Tray.Create(this, WM_ICON_NOTIFY, _T("alexf dialer"),
		(HICON)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0),
		IDR_TRAY_MENU1);
//		Tray.SetMaxWidth(25);
	}
	else
	Tray.ShowIcon();
	ShowWindow(SW_HIDE);
}

afx_msg LRESULT CAfdDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	m_bStartMinimized = FALSE; // only 1 time
	return Tray.OnTrayNotification(wParam, lParam);
}

void CAfdDlg::OnShow() 
{
	Tray.HideIcon();
	ShowWindow(SW_SHOW);
}

void CAfdDlg::OnNcPaint() 
{
	if (m_bStartMinimized)
	{
		OnHide();
		return;
	}
	CDialog::OnNcPaint();
}

void CAfdDlg::OnTrayDial() 
{
	theApp.PushBigButton();
}

void CAfdDlg::OnUpdateTrayDial(CCmdUI* pCmdUI) 
{
	if (!page1) return;
	pCmdUI->Enable(page1->bPhone);
}

BOOL CAfdDlg::Hide()
{
	OnHide();
	return TRUE;
}
