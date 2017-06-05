
// QQClientDlg.h : 头文件
//

#pragma once
#include "Login.h"

// CQQClientDlg 对话框
class CQQClientDlg : public CDialogEx
{
// 构造
public:
	CQQClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	SOCKET  m_ClientSocket;
	SOCKADDR_IN  m_ServerAddr;
	CString  m_Timer;
	//CButton m_ComBoUser;
	DWORD   m_dwFileSize;
	CString m_strFileSize;
	CString  m_strName;
	CString m_strFilePath;
	CString m_strFileName;
	VOID CQQClientDlg::GetCurrentTime();
	VOID CQQClientDlg::OnRecive(WPARAM wParam);
	VOID CQQClientDlg::SendMsg(int iType,CString strName,CString strMsg);
	CString CQQClientDlg::KindChange(DWORD dwFileSize);
	void CQQClientDlg::HideItem();
	void CQQClientDlg::Shaking();
	void CQQClientDlg::AnalyseString(char* szBuffer);
	CString m_strOtherIP;
	CString m_strOtherName;
	CString m_strOtherFileName;
	DWORD   m_dwOtherFileSize;
// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };
	CButton	m_ButtonRecv;
	CButton	m_ButtonCancle;
	CProgressCtrl	m_Progress;
	CComboBox	m_ComBoUser;
	CListBox	m_ListUser;
	CString	m_EditShow;
	CString	m_EditSend;
	BOOL	m_bOnly;
	BOOL	m_bMe;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现

protected:
	HICON m_hIcon;
	afx_msg void OnButtonShake();
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSocketRead(WPARAM wParam,LPARAM lParam);
	afx_msg void OnButtonTrans();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonRecv();
	DECLARE_MESSAGE_MAP()
public:

};
