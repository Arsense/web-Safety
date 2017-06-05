#pragma once


// CPage9 对话框

class CPage9 : public CDialog
{
	DECLARE_DYNAMIC(CPage9)

public:
	CPage9(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage9();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
