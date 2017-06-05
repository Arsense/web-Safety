
// QQServerDlg.h : 头文件
//

#pragma once
#include "resource.h"
#include<afxwin.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")


#define MAX 20
// CQQServerDlg 对话框
class CQQServerDlg : public CDialogEx
{
// 构造
public:
	CQQServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	VOID CQQServerDlg::InitSocket();
	VOID CQQServerDlg::GetHostInfor();
	VOID CQQServerDlg::InitNotify();
// 对话框数据
	enum { IDD = IDD_QQSERVER_DIALOG };
	CStatic	m_strPeople;
	CStatic	m_strHost;
	CString	m_strShow;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
		SOCKET   m_sListen;
		SOCKET   m_sClient[MAX];
		sockaddr_in  m_ServerAddr;
		ULONG    m_People;


// 实现
protected:
	HICON m_hIcon;

	 NOTIFYICONDATAA m_nid;    //系统托盘

	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
