#pragma once
#include "afxwin.h"


// CChooseDlg 对话框

class CChooseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseDlg)

public:
	CChooseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChooseDlg();
	void CChooseDlg::AddToList(CString s);

// 对话框数据
	enum { IDD = IDD_MSGCHOOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_List;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
