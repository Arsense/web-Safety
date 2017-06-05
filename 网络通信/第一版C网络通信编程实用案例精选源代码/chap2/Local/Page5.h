#pragma once


// CPage5 对话框

class CPage5 : public CDialog
{
	DECLARE_DYNAMIC(CPage5)

public:
	CPage5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage5();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 计算机安装的协议
	CString m_Protocols;
	// 获取计算机信息
	int GetInfo(void);
	virtual BOOL OnInitDialog();
	// 易读格式打印出WSAPROTOCOL_INFO结构的内容
	void PrintBufEntry(WSAPROTOCOL_INFO *pProtocolBuf);
};
