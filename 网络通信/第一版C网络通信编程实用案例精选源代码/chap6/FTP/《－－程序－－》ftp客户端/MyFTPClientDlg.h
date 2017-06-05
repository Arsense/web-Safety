// FTPSOCKETDlg.h : header file
//

#if !defined(AFX_FTPSOCKETDLG_H__E0B6E988_9D2B_4461_8694_DDAB5027F497__INCLUDED_)
#define AFX_FTPSOCKETDLG_H__E0B6E988_9D2B_4461_8694_DDAB5027F497__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FTPCommandProcessor.h"
#include "ListItem.h"



/////////////////////////////////////////////////////////////////////////////
// CMyFTPClientDlg dialog
#define WM_SETFILE     WM_USER +100
#define WM_SETREDRAWFLAG     WM_USER +101
#define WM_SETDIRECTORYDISPLAY     WM_USER +102
#define WM_REDISPLAYFILE     WM_USER +103
#define WM_SETDIR     WM_USER +104





enum
	{
		FILEICON,
		DIRECTORYICON,
		LOCFILE,
		SERVFILE
	};


class CMyFTPClientDlg : public CDialog
{
// Construction
public:
	CMyFTPClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyFTPClientDlg)
	enum { IDD = IDD_FTPSOCKET_DIALOG };
	CListCtrl	m_ctrlServerFile;
	CListCtrl	m_ctrlLocalFile;
	CComboBoxEx	m_ctrlServer;
	CComboBoxEx	m_ctrlLocal;
	CString	m_PWD;
	CString	m_Server;
	CString	m_UID;
	CString	m_Message;
	int		m_Port;
	BOOL	m_PASV;
	//}}AFX_DATA
	BOOL m_bHaveDotFlag;
	CFTPCommandProcessor ftp;

	CImageList* m_pDragImageList;
	CImageList m_ctImageList;
	int m_iDirCount;


	typedef struct
	{
		char szFileName[256];
		char szFileDate[20];
		char szFileSize[20];
		int nType;
	}FILE_FTP_INFO;




	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFTPClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyFTPClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	afx_msg void OnDblclkListLocalfile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboboxexLocal();
	afx_msg void OnDblclkListServerfile(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

protected:
/*
	afx_msg void OnLocFileOpen();
*/
	void OnInsertFile(CListCtrl* pListCtrl,LPVOID pIn,LPARAM lParam);
	void GetFileIcon(CString& fileName,int* iIcon,int* iIconSel=NULL);
	BOOL SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo=NULL);
	void SetFileColumns(CListCtrl* pListCtrl);
	void GetLocDir();
	int InsertLocDir(CString &str, int iIcon, int iIconSel);
	void GetLocCurrentDirectory();
	BOOL BeginFindLocFile();
	void FindLocFile();
	void SetServerDir(CString& str);
	UINT FindServFile();
	//void Server
	void AddLine(CString strLine);
	BOOL AddItem(CListItem *pItem);
	void SetServerFileColumns(CListCtrl *pListCtrl);
	void CD(CString dir);
	void Back();
	void UpLoadFile(CString LocalFilePath,CString LocalFileName,bool pasv,bool get);
	void DownLoadFile(CString ServerFilePath,CString ServerFileName,bool pasv,bool get);





	afx_msg LRESULT OnReDisplayLocFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetLocDirectoryDisplay(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnInsertLocFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetLocRedrawFlag(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetLocDir(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSOCKETDLG_H__E0B6E988_9D2B_4461_8694_DDAB5027F497__INCLUDED_)
