#if !defined(AFX_USERMANAGER_H__80FCDEAE_C56D_4E25_998D_8BE0511F7C10__INCLUDED_)
#define AFX_USERMANAGER_H__80FCDEAE_C56D_4E25_998D_8BE0511F7C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FTP_DOWNLOAD	1
#define FTP_UPLOAD		2
#define FTP_RENAME		3
#define FTP_DELETE		4
#define FTP_CREATE_DIR	5
#define FTP_LIST		6

//用户账户类
class CUserAccount : public CObject
{
	DECLARE_SERIAL(CUserAccount)

	CUserAccount();
	virtual ~CUserAccount();

public:
	virtual void Serialize(CArchive&);
	CUserAccount (const CUserAccount &user);				// copy-constructor
	CUserAccount &operator=(const CUserAccount &user);	// =-operator

	CString m_strName;
	CString m_strPassword;
	CString m_strDir;
	BOOL m_bAllowDownload;
	BOOL m_bAllowUpload;
	BOOL m_bAllowRename;
	BOOL m_bAllowDelete;
	BOOL m_bAllowCreateDirectory;
};

//用户管理类
class CUserManager  
{
public:
	CUserManager();
	virtual ~CUserManager();

	void GetUserAccountList(CArray<CUserAccount, CUserAccount&>&array);
	void UpdateUserAccountList(CArray<CUserAccount, CUserAccount&>&array);
	BOOL GetUserAccount(LPCTSTR lpszUser, CUserAccount &useraccount);
	BOOL ConvertPathToLocal(LPCTSTR lpszUser, CString &strDirectoryIn, CString &strDirectoryOut);
	BOOL CheckAccessRights(LPCTSTR lpszUser, LPCTSTR lpszDirectory, int nOption);
	BOOL FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck = TRUE);
	int  GetDirectoryList(LPCTSTR lpszUser, LPCTSTR lpszDirectory, CString &strResult);
	int  ChangeDirectory(LPCTSTR lpszUser, CString &strCurrentdir, CString &strChangeTo);
	CString GetFileDate(CFileFind &find);
	int CheckDirectory(LPCTSTR lpszUser, CString strDirectory, CString strCurrentdir, int nOption, CString &strResult);
	int CheckFileName(LPCTSTR lpszUser, CString strFilename, CString strCurrentdir, int nOption, CString &strResult);
	
	BOOL Serialize(BOOL bStoring);

	CArray<CUserAccount, CUserAccount&> m_UserArray;

protected:
	CString m_strFilename;
	int m_nUserCount;//记录在文件中保存了几条用户信息
	CCriticalSection m_CriticalSection;
};

#endif