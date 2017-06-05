#pragma once


// CPage7 对话框

class CPage7 : public CDialog
{
	DECLARE_DYNAMIC(CPage7)

public:
	CPage7(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage7();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 机器所有网络资源
	CString m_EnumResource;
	// 获取计算机信息
	int GetInfo(void);
	virtual BOOL OnInitDialog();
};
