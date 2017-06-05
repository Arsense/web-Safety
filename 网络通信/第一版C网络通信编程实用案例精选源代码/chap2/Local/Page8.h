#pragma once
#include "afxcmn.h"


// CPage8 对话框

class CPage8 : public CDialog
{
	DECLARE_DYNAMIC(CPage8)

public:
	CPage8(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage8();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 获取计算机信息
	int GetInfo(void);
	virtual BOOL OnInitDialog();
	// IP地址
	CIPAddressCtrl m_IPAddress;
	// 网卡MAC地址
	CString m_MACAddress;
	// 网卡名称
	CString m_InterfaceName;
	afx_msg void OnBnClickedButton1();
	// 网卡号
	DWORD m_dwIndex;
	// 网卡类型
	DWORD m_dwType;
	// 网卡MAC地址长度
	DWORD m_dwAddressLength;
};
