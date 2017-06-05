

#pragma once


// CLogin 对话框

class CLogin : public CDialog
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	CString m_strIP;


// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

	CIPAddressCtrl	m_ControlIP;
	CString	m_strName;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

