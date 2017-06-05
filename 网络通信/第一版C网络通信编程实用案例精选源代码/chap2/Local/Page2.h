#pragma once


// CPage2 对话框

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage2();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 获取计算机子网掩码
	CString m_getIPMask;
	// 获得计算机子网掩码
	int GetInfo(void);
	virtual BOOL OnInitDialog();
};
