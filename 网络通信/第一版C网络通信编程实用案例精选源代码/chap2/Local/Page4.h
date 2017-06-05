#pragma once


// CPage4 对话框

class CPage4 : public CDialog
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage4();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 网卡地址
	CString m_MACAddress;
	// 获取网卡MAC地址
	int GetInfo(void);
	virtual BOOL OnInitDialog();
	// 网卡MAC地址2
	CString m_MACAddress2;
};
