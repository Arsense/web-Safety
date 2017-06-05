#include "stdafx.h"
#include "UserManager.h"


IMPLEMENT_SERIAL(CUserAccount, CObject, 1)

CUserAccount::CUserAccount()
{
	//m_bAccountDisabled = FALSE;
}

CUserAccount::~CUserAccount()
{
}

void CUserAccount::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 'store' data
		ar << m_strName;
		ar << m_strPassword;
		ar << m_strDir;
		ar << m_bAllowDownload;
		ar << m_bAllowUpload;
		ar << m_bAllowRename;
		ar << m_bAllowDelete;
		ar << m_bAllowCreateDirectory;
	}
	else
	{
		// 'load' data
		ar >> m_strName;
		ar >> m_strPassword;
		ar >> m_strDir;
		ar >> m_bAllowDownload;
		ar >> m_bAllowUpload;
		ar >> m_bAllowRename;
		ar >> m_bAllowDelete;
		ar >> m_bAllowCreateDirectory;
	}
}


/* Copy-constructor */
CUserAccount::CUserAccount(const CUserAccount &useracount)
{
	m_strName = useracount.m_strName;
	m_strPassword = useracount.m_strPassword;
	m_strDir = useracount.m_strDir;
	m_bAllowDownload = useracount.m_bAllowDownload;
	m_bAllowUpload = useracount.m_bAllowUpload;
	m_bAllowRename = useracount.m_bAllowRename;
	m_bAllowDelete = useracount.m_bAllowDelete;
	m_bAllowCreateDirectory = useracount.m_bAllowCreateDirectory;
}

/* = operator definition */
CUserAccount& CUserAccount::operator=(const CUserAccount &useracount)
{
	if (&useracount != this)
	{
		m_strName = useracount.m_strName;
		m_strPassword = useracount.m_strPassword;
		m_strDir = useracount.m_strDir;
		m_bAllowDownload = useracount.m_bAllowDownload;
		m_bAllowUpload = useracount.m_bAllowUpload;
		m_bAllowRename = useracount.m_bAllowRename;
		m_bAllowDelete = useracount.m_bAllowDelete;
		m_bAllowCreateDirectory = useracount.m_bAllowCreateDirectory;
	}
	return *this;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

CUserManager::CUserManager()
{
	GetAppDir(m_strFilename);
	m_strFilename += "users.dat";  //用户信息串行化保存到文件名字在这里
}

CUserManager::~CUserManager()  
{
	
}

/********************************************************************/
/*																	*/
/* Function name : Serialize										*/
/* Description   : Call this function to store/load the user data	*/
/*																	*/
/********************************************************************/
BOOL CUserManager::Serialize(BOOL bStoring)
{
	static const TCHAR* lpszSignature = _T("Bai Software Solutions - StoreObject");
	
	CFile file;
	
	if (file.Open(m_strFilename, bStoring ? CFile::modeWrite|CFile::modeCreate : CFile::modeRead))
	{
		TRY
		{
			CString str; 
			CArchive ar(&file, bStoring ? CArchive::store : CArchive::load);
			
			if (bStoring)
			{
				m_nUserCount = m_UserArray.GetSize();
				// save signature
				ar << CString(lpszSignature);
				ar << m_nUserCount;
				
				// Save the changed user details
				for (int i=0; i < m_UserArray.GetSize(); i++)
				{
					m_UserArray[i].Serialize(ar);
				}
				
				ar.Flush();
			}
			else
			{
				// load signature
				ar >> str;
				
				ar >> m_nUserCount;
				// if this the file we are looking for ?
				if (str.Compare(lpszSignature) == 0)
				{
					
					for(int i=0; i < m_nUserCount; i++)
					{
						CUserAccount useraccount;
						
						// get user data
						useraccount.Serialize(ar);
						
						// add user to array
						m_UserArray.Add(useraccount);
					}
				}
			}
			ar.Close();
			file.Close();
		}
		CATCH_ALL(e)
		{
			// catch all exceptions that might happen ...
			return FALSE;
		}
		END_CATCH_ALL
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : GetUserList										*/
/* Description   : Make copy of user array							*/
/*																	*/
/********************************************************************/
void CUserManager::GetUserAccountList(CArray<CUserAccount, CUserAccount&>&array)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_UserArray.GetSize();i++)
	{
		array.Add(m_UserArray[i]);
	}
	m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : UpdateUserList									*/
/* Description   : Update user array								*/
/*																	*/
/********************************************************************/
void CUserManager::UpdateUserAccountList(CArray<CUserAccount, CUserAccount&>&array)
{
	m_CriticalSection.Lock();
	m_UserArray.RemoveAll();
	for (int i=0; i<array.GetSize();i++)
	{
		m_UserArray.Add(array[i]);
	}
	m_CriticalSection.Unlock();
	Serialize(TRUE);
}

/********************************************************************/
/*																	*/
/* Function name : GetDirectoryList									*/
/* Description   : Get directory listing for specfied directory		*/
/*																	*/
/********************************************************************/
int CUserManager::GetDirectoryList(LPCTSTR lpszUser, LPCTSTR lpszDirectory, CString &strResult)
{
	CString strDirectory = lpszDirectory;
	
	// make unix style
	strDirectory.Replace("\\","/");
	while(strDirectory.Replace("//","/"));
	
	// clear list
	strResult = "";
	
	CUserAccount useraccount;
	if (!GetUserAccount(lpszUser, useraccount))
	{
		// user not found -> no permissions
		return 1;
	}
	
	CString strLocalPath;
	if (!ConvertPathToLocal(lpszUser, strDirectory, strLocalPath))
	{
		// unable to convert to local path
		return 2;
	}
	
	// check if user has access right for this directory
	if (!CheckAccessRights(lpszUser, strLocalPath, FTP_DOWNLOAD))
	{
		// user has no permissions, to display this directory
		return 1;
	}
	
	CFileFind find;
	BOOL bFound = FALSE;
	
	// check if it's a directory
	if ((GetFileAttributes(strLocalPath) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		bFound = find.FindFile(strLocalPath + "\\*.*");
	}
	else
	{
		// it's a file
		bFound = find.FindFile(strLocalPath);
	}
	
	while (bFound)
	{
		bFound = find.FindNextFile();
		
		// skip "." and ".." 
		if (find.IsDots())
			continue;
		
		// permissions
		if (find.IsDirectory())
			strResult += "drwx------";
		else
			strResult += "-rwx------";
		
		// groups
		strResult += " 1 user group ";
		// file size
		CString strLength;
		strLength.Format("%d", find.GetLength());
		CString strFiller = "              ";
		strResult += strFiller.Left(strFiller.GetLength() - strLength.GetLength());
		strResult += strLength;
		// file date
		strResult += GetFileDate(find);
		// file name
		strResult += find.GetFileName();
		// end of line
		strResult += "\r\n";
	}
	
	return 0;
}

/********************************************************************/
/*																	*/
/* Function name : ChangeDirectory									*/
/* Description   : Change to specified directory					*/
/*																	*/
/********************************************************************/
int CUserManager::ChangeDirectory(LPCTSTR lpszUser, CString &strCurrentdir, CString &strChangeTo)
{
	// make unix style
	strChangeTo.Replace("\\","/");
	while(strChangeTo.Replace("//","/"));
	strChangeTo.TrimRight("/");
	
	// now looks something like this: 
	// ""				= root
	// "/mydir/apps"	= absolute path
	// "mydir/apps"		= relative path
	
	if (strChangeTo == "")
	{
		// goto root
		strChangeTo = "/";
	}
	else
	{
		// first character '/' ?
		if (strChangeTo.Left(1) != "/")
		{ 
			// client specified a path relative to their current path
			strCurrentdir.TrimRight("/");
			strChangeTo = strCurrentdir + "/" + strChangeTo;
		}
	}
	
	// goto parent directory
	if (strChangeTo.Right(2) == "..")
	{
		strChangeTo.TrimRight("..");
		strChangeTo.TrimRight("/");
		int nPos = strChangeTo.ReverseFind('/');
		if (nPos != -1)
		{
			strChangeTo = strChangeTo.Left(nPos);
		}
		if (strChangeTo == "")
		{
			// goto root
			strChangeTo = "/";
		}
	}
	
	// get local path
	CString strLocalPath;
	if (!ConvertPathToLocal(lpszUser, strChangeTo, strLocalPath))
	{
		// unable to convert to local path
		return 2;
	}
	
	// check if user has access right for this directory
	if (!CheckAccessRights(lpszUser, strLocalPath, FTP_DOWNLOAD))
	{
		// user has no permissions
		return 1;
	}
	
	// everything went successfully
	strCurrentdir = strChangeTo; 
	return 0;
}

/********************************************************************/
/*																	*/
/* Function name : ConvertPathToLocal								*/
/* Description   : Convert relative path to local path				*/
/*																	*/
/********************************************************************/
BOOL CUserManager::ConvertPathToLocal(LPCTSTR lpszUser, CString &strDirectoryIn, CString &strDirectoryOut)
{
	CUserAccount useraccount;
	if (!GetUserAccount(lpszUser, useraccount))
	{
		// user not valid
		return FALSE;
	}
	
	CStringList partList;
	CString strSub;
	int nCount=0;
	
	// split path in parts
	while(AfxExtractSubString(strSub, strDirectoryIn, nCount++, '/'))
	{
		if (!strSub.IsEmpty())
			partList.AddTail(strSub);
	}
	
	
	CString strHomeDir = useraccount.m_strDir;
	while(!partList.IsEmpty())
	{
		CString strPart = partList.GetHead();
		partList.RemoveHead();
		
		CString strCheckDir;
		
		if (strPart == "..")
		{
			// go back one level
			int nPos = strHomeDir.ReverseFind('\\');
			if (nPos != -1)
			{
				strCheckDir = strHomeDir.Left(nPos);
			}
		}
		else
		{
			strCheckDir = strHomeDir + "\\" + strPart;
		}
		
		// does directory exist ?
		if (FileExists(strCheckDir, TRUE))
		{
			strHomeDir = strCheckDir;
		}
		// does file exist ?
		else if (FileExists(strCheckDir, FALSE))
		{
			strHomeDir = strCheckDir;
		}
	}
	
	// successfully converted directory
	strDirectoryOut = strHomeDir;
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : GetUser											*/
/* Description   : Get user object for specified username			*/
/*																	*/
/********************************************************************/
BOOL CUserManager::GetUserAccount(LPCTSTR lpszUser, CUserAccount &useraccount)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_UserArray.GetSize(); i++)
	{
		if (!m_UserArray[i].m_strName.CompareNoCase(lpszUser))
		{
			useraccount = m_UserArray[i];
			m_CriticalSection.Unlock();
			return TRUE;
		}
	}
	m_CriticalSection.Unlock();
	return FALSE;
}

/********************************************************************/
/*																	*/
/* Function name : FileExists										*/
/* Description   : Check if file or directory exists				*/
/*																	*/
/********************************************************************/
BOOL CUserManager::FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck)
{
	// A quick'n'easy way to see if a file exists.
	DWORD dwAttributes = GetFileAttributes(lpszFileName);
    if (dwAttributes == 0xFFFFFFFF)
        return FALSE;

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		if (bIsDirCheck)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (!bIsDirCheck)
			return TRUE;
		else
			return FALSE;
	}
}

/********************************************************************/
/*																	*/
/* Function name : CheckAccessRights								*/
/* Description   : Check if user has access to specified directory.	*/
/*																	*/
/********************************************************************/
BOOL CUserManager::CheckAccessRights(LPCTSTR lpszUser, LPCTSTR lpszDirectory, int nOption)
{
	CUserAccount useraccount;
	if (!GetUserAccount(lpszUser, useraccount))
	{
		// user not valid
		return FALSE;
	}

	// start with full path
	CString strCheckDir = lpszDirectory;

	while(!strCheckDir.IsEmpty())
	{
		CString strPath1 = strCheckDir;
		strPath1.TrimRight("\\");
		CString strPath2 = useraccount.m_strDir;
		strPath2.TrimRight("\\");
		
		if (strPath1.CompareNoCase(strPath2) == 0)
		{
			// check file access rights
			if (((!useraccount.m_bAllowDownload) && (nOption == FTP_DOWNLOAD)) ||
				((!useraccount.m_bAllowUpload) && (nOption == FTP_UPLOAD)) ||
				((!useraccount.m_bAllowRename) && (nOption == FTP_RENAME)) ||
				((!useraccount.m_bAllowDelete) && (nOption == FTP_DELETE)) ||
				((!useraccount.m_bAllowCreateDirectory) && (nOption == FTP_CREATE_DIR)))
			{
				return FALSE;
			}
			return TRUE;
		}

		int nPos = strCheckDir.ReverseFind('\\');
		if (nPos != -1)
		{
			// strip subdir 
			strCheckDir = strCheckDir.Left(nPos);
		}
		else
		{
			// we're done
			strCheckDir.Empty();
		}
	}
	// users has no rights to this directory
	return FALSE;
}

/********************************************************************/
/*																	*/
/* Function name : GetFileDate										*/
/* Description   : return file date in unix style					*/
/*																	*/
/********************************************************************/
CString CUserManager::GetFileDate(CFileFind &find)
{
	CString strResult;

	CTime time = CTime::GetCurrentTime();

	find.GetLastWriteTime(time);

	CTimeSpan timeSpan = CTime::GetCurrentTime() - time;

	if (timeSpan.GetDays() > 356)
	{
		strResult = time.Format(" %b %d %Y ");
	}
	else
	{
		strResult.Format(" %s %02d:%02d ", time.Format("%b %d"), time.GetHour(), time.GetMinute());
	}

	return strResult;
}

/********************************************************************/
/*																	*/
/* Function name : CheckDirectory									*/
/* Description   : Check if directory exists and if user has		*/
/*				   appropriate permissions.							*/
/*																	*/
/********************************************************************/
int CUserManager::CheckDirectory(LPCTSTR lpszUser, CString strDirectory, CString strCurrentdir, int nOption, CString &strResult)
{
	// make unix compatible
	strDirectory.Replace("\\","/");
	while(strDirectory.Replace("//","/"));
	strDirectory.TrimRight("/");
	
	if (strDirectory == "")
	{
		// no directory
		return 2;
	}
	else
	{
		// first character '/' ?
		if (strDirectory.Left(1) != "/")
		{ 
			// client specified a path relative to their current path
			strCurrentdir.TrimRight("/");
			strDirectory = strCurrentdir + "/" + strDirectory;
		}
	}

	// split part into 2 parts
	int nPos = strDirectory.ReverseFind('/');
	if (nPos == -1)
		return 2;

	// get left part of directory
	CString strNode = strDirectory.Left(nPos);
	// root ?
	if (strNode == "")
		strNode = "/";
	// get right part of directory
	strDirectory = strDirectory.Mid(nPos+1);

	CString strLocalPath;

	do
	{
		// does parent directory exist ?
		if ((!ConvertPathToLocal(lpszUser, strNode, strLocalPath)) && (nOption == FTP_CREATE_DIR))
		{ 
			// directory could not be found, maybe one level higher
			int nPos = strNode.ReverseFind('/');
			// no more levels
			if (nPos == -1) 
				return 2;

			strDirectory = strNode.Mid(nPos+1) + "/" + strDirectory;
			strNode = strNode.Left(nPos);
			continue;
		}

		// check directory access rights
		if (!CheckAccessRights(lpszUser, strLocalPath, nOption))
		{
			// user has no permissions, to execute specified action
			return 1;
		}
		
		strNode = strLocalPath;
		break;
	} 
	while (strNode != "/");
	
	strDirectory.Replace("/","\\");
	strResult = strNode + "\\" + strDirectory;
		
	// check if directory exists
	if (!FileExists(strResult))
		return 2;

	// function successfull
	return 0;
}

/********************************************************************/
/*																	*/
/* Function name : CheckFileName									*/
/* Description   : Check if filename is valid and if user has		*/
/*				   access rights.									*/
/*																	*/
/********************************************************************/
int CUserManager::CheckFileName(LPCTSTR lpszUser, CString strFilename, CString strCurrentdir, int nOption, CString &strResult)
{
	// make unix style
	strFilename.Replace("\\", "/");
	while(strFilename.Replace("//", "/"));
	strFilename.TrimRight("/");
	
	if (strFilename == "")
	{
		// no file name
		return 2;
	}

	// append filename to directory
	CString strDirectory = strCurrentdir;

	// client has specified complete path 
	int nPos = strFilename.ReverseFind('/');
	if (nPos != -1)
	{
		strDirectory = strFilename.Left(nPos);
		if (strDirectory == "")
			strDirectory = "/";
		strFilename = strFilename.Mid(nPos+1);
	}

	// get local path
	CString strLocalPath;
	if (!ConvertPathToLocal(lpszUser, strDirectory, strLocalPath))
	{
		// directory does not exist
		return 2;
	}

	// create the complete path
	strResult = strLocalPath + "\\" + strFilename;

	if ((nOption != FTP_UPLOAD) && !FileExists(strResult, FALSE))
	{
		// file does not exist
		return 2;
	}

	// return relative path
	if (nOption == FTP_LIST)
	{
		strResult = strCurrentdir;
		strResult.TrimRight('/');
		strResult += "/" + strFilename;
		return 0;
	}

	// check file access rights
	if (!CheckAccessRights(lpszUser, strLocalPath, nOption))
	{
		// user has no permissions, to execute specified action
		return 1;
	}
	// everything is ok
	return 0;
}