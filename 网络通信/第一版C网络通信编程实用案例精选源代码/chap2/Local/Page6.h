#pragma once


// CPage6 对话框

class CPage6 : public CDialog
{
	DECLARE_DYNAMIC(CPage6)

public:
	CPage6(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage6();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 计算机提供的服务名
	CString m_ServiceName;
	virtual BOOL OnInitDialog();
	// 获取计算机信息
	int GetInfo(void);
	// 提供的服务2
	CString m_ServiceName2;
};
