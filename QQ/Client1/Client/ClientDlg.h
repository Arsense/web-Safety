
// ClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"MyEdit.h"
#include"MyTracker.h"
// CClientDlg 对话框
class CClientDlg : public CDialog
{
// 构造
public:

				int m_xScreen;
				int m_yScreen;

		     CMyTracker			 m_rectTracker;     //像皮筋类
		     HCURSOR				 m_hCursor;              //光标
		     BOOL                      m_bDraw;                   //是否为截取状态
		     BOOL                      m_bFirstDraw;              //是否为首次截取
		     BOOL                      m_bQuit;                   //是否为退出
		     BOOL                      m_bShowMsg;                //显示截取矩形大小信息
		 	CPoint			          	m_startPt;				//截取矩形左上角
			CBitmap *                m_pBitmap;            //背景位图
			CRgn m_rgn;						//背景擦除区域


			void DrawTip();                            //显示操作提示信息
			HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);   //考贝桌面到位图
			void DrawMessage(CRect &inRect,CDC * pDC);       //显示截取矩形信息


	CString m_strOtherIP;
		DWORD   m_dwFileSize;
		CString m_strFileName;
		CString m_strOtherName;
	DWORD   m_dwOtherFileSize;
		CString CClientDlg::KindChange(DWORD dwFileSize);
	CString m_strOtherFileName;
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	SOCKET  m_ClientSocket;
	SOCKADDR_IN  m_ServerAddr;
		void CClientDlg::AnalyseString(char* szBuffer);
	VOID CClientDlg::SendMsg(int iType,CString strName,CString strMsg);
	VOID CClientDlg::OnRecive(WPARAM wParam);
	void CClientDlg::HideItem();
	void CClientDlg::Shaking();
		CString  m_strName;
			CString  m_Timer;
// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };
//	CComboBox	m_ComBoUser;
//	CListBox	m_ListUser;
//	CString		m_EditShow;
	//CString	m_EditSend;
	//BOOL	m_bOnly;
	//BOOL	m_bMe;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSocketRead(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComBoUser;
	afx_msg void OnBnClickedButtonRecv();
	afx_msg void OnButtonShake();
	afx_msg void OnButtonTrans();
	CListBox m_ListUser;
//	CEdit m_EditShow;
	CString m_EditShow;
	afx_msg void OnCbnSelchangeComboUser();
	BOOL m_bOnly;
	BOOL m_bMe;
	afx_msg void OnEnChangeEditSend();
	CString m_EditSend;
	afx_msg void OnButtonSend();
	CButton m_ButtonCancle;
	CProgressCtrl m_Progress;
	CButton m_ButtonRecv;
	afx_msg void OnButtonRecv();
	CMyEdit m_tipEdit;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
