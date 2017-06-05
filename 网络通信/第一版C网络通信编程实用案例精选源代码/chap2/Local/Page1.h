#pragma once


// CPage1 对话框

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 获取计算机名：GetComputerName
	CString m_GetComputerName;
	virtual BOOL OnInitDialog();
	// 获取计算机名称：gethostname
	CString m_gethostname;
	// 获取IP地址
	CString m_IPAddress;
	// 获取计算机名称和IP地址
	int GetInfo(void);
};
