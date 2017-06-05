#pragma once


// CMsgViewDlg 对话框

class CMsgViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgViewDlg)

public:
	CMsgViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgViewDlg();

// 对话框数据
	enum { IDD = IDD_MSGVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString text;
	afx_msg void OnBnClickedSave();
};
