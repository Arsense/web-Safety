#pragma once


// CPage3 对话框

class CPage3 : public CDialog
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 计算机DNS设置
	CString m_DNS;
	// 获取计算机信息
	int GetInfo(void);
	virtual BOOL OnInitDialog();
};
