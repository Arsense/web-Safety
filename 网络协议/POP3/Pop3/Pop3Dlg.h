
// Pop3Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Pop3Manage.h"
#include "resource.h"


// CPop3Dlg 对话框

	
class CPop3Dlg : public CDialogEx
{
// 构造
public:
	CPop3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_POP3_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	CString m_Password;
	CString m_UserName;
	CString m_ServerAddress;
	BOOL m_bDelete;
	CString m_InFormation;
	CRichEditCtrl m_RichEditCtrlInformation;
	CPop3Manage Pop3;
	void CPop3Dlg::Dispatch(LONG param);
	
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedView();
};
