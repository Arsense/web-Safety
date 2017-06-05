
// QQServer2Dlg.h : 头文件
//

#pragma once
#include<afxmsg_.h>
#define MAX 20
// CQQServer2Dlg 对话框
#include <WINSOCK2.H>

#pragma comment(lib,"ws2_32.lib")
typedef struct _PEOPLEINFOR_ 
{
	CString  strName;      //姓名
	CString  strIp;			//用户IP
}PEOPLEINFOR;


typedef struct _SENDINFOR_
{
	int  iType;				  //0
	char szName[20];	//123
	char szMsg[400];		 //消息类型
}SENDINFOR;


class CQQServer2Dlg : public CDialogEx
{
// 构造
public:
	CQQServer2Dlg(CWnd* pParent = NULL);	// 标准构造函数
	VOID CQQServer2Dlg::InitNotify();
	VOID CQQServer2Dlg::InitSocket();
	VOID CQQServer2Dlg::GetHostInfor();
	VOID CQQServer2Dlg::GetCurrentTime();
// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };
	CStatic	m_strPeople;
	CStatic	m_strHost;
	CString	m_strShow;


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	 NOTIFYICONDATAA m_nid;    //系统托盘
	 	ULONG    m_People;
		SOCKET   m_sListen; 
		SOCKET   m_sClient[MAX];
		PEOPLEINFOR  m_PeopleInfor[MAX];
		//PEOPLEINFOR  m_PeopleInfor[MAX];
		sockaddr_in  m_ServerAddr;
		CString  m_Timer; 

		void CQQServer2Dlg::OnStaticHost();
		void CQQServer2Dlg::OnStaticPeople();
		void CQQServer2Dlg::OnChangeEditShow() ;
		VOID CQQServer2Dlg::OnlineOrOutline(int iIndex,CString YesOrNo);
		VOID CQQServer2Dlg::OnClose(WPARAM wParam);
		
		VOID CQQServer2Dlg::OnAccept();
		VOID CQQServer2Dlg::OnRecive(WPARAM wParam);
		
		//LRESULT CQQServer2Dlg::OnServerAccept(WPARAM wParam,LPARAM lParam);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam); 
	afx_msg LRESULT OnServerAccept(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnDlgExit();
	afx_msg void OnDlgShow();
	afx_msg void OnDlgHide();
	DECLARE_MESSAGE_MAP()
};
