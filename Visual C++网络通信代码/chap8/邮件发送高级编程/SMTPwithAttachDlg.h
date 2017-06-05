// SMTPwithAttachDlg.h : header file
//

#if !defined(AFX_SMTPWITHATTACHDLG_H__17FC5C0F_4496_4C15_B8B0_9DA2BD3528DA__INCLUDED_)
#define AFX_SMTPWITHATTACHDLG_H__17FC5C0F_4496_4C15_B8B0_9DA2BD3528DA__INCLUDED_
#include "smtp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "smtp.h"

/////////////////////////////////////////////////////////////////////////////
// CSMTPwithAttachDlg dialog

class CSMTPwithAttachDlg : public CDialog
{
// Construction
public:
	CSMTPwithAttachDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSMTPwithAttachDlg)
	enum { IDD = IDD_SMTPWITHATTACH_DIALOG };
	CComboBox	m_ctrlAuthenticate;
	CString	m_Address;
	CString	m_BCC;
	CString	m_CC;
	CString	m_Letter;
	CString	m_Name;
	CString	m_PWD;
	CString	m_Receiver;
	CString	m_Server;
	CString	m_Title;
	CString	m_UID;
	CString	m_IP;
	int		m_Port;
	CString	m_Attachment;
	DWORD	m_test;
	//}}AFX_DATA
    CString m_sEncodingFriendly;
    CString m_sEncodingCharset;
    BOOL    m_bMime;
    BOOL    m_bHTML;


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTPwithAttachDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	//设置初始化参数
	void SetPara();
	//创建信息
	CSMTPMessage* CreateMessage();
	//定义变量来表示服务器验证方式
	CSMTPConnection::LoginMethod m_Authenticate;
	DWORD m_Authenticate_1;

	int CBAddStringAndData(CWnd* pDlg, int nIDC, LPCTSTR pszString, DWORD dwItemData);


	// Generated message map functions
	//{{AFX_MSG(CSMTPwithAttachDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBrowerFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
	void DDX_CBData(CDataExchange* pDX, int nIDC, DWORD& dwItemData);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPWITHATTACHDLG_H__17FC5C0F_4496_4C15_B8B0_9DA2BD3528DA__INCLUDED_)
