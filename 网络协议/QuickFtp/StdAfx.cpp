// stdafx.cpp : source file that includes just the standard includes
//	FTPServer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

/********************************************************************/
/*																	*/
/* Function name : BrowseForFolder									*/		
/* Description   : Browse for folder using SHBrowseForFolder.		*/
/*																	*/
/********************************************************************/
CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags)
{
	// We're going to use the shell to display a 
	// "Choose Directory" dialog box for the user.
	CString strResult = "";
  
	LPMALLOC lpMalloc;
	
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
	{
		// failed to get allocator  
		return strResult; 
	}

	char szBuffer[_MAX_PATH];
	char szDisplayName[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hWnd;
	// set root at Desktop
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = lpszTitle;
	browseInfo.ulFlags = nFlags;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0;
	
	LPITEMIDLIST lpItemIDList;

	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path the user chose.
			if (szBuffer[0] == '\0')
			{
				// SHGetPathFromIDList failed, or SHBrowseForFolder failed.
				AfxMessageBox("Failed to get directory", MB_ICONSTOP|MB_OK);
				return strResult;
			}
     
			// We have a path in szBuffer!
			strResult = szBuffer;
			return strResult;
		}
		else
		{
			// The thing referred to by lpItemIDList 
			// might not have been a file system object.
			// For whatever reason, SHGetPathFromIDList didn't work!
			AfxMessageBox("Failed to get directory", MB_ICONSTOP|MB_OK);
			return strResult; // strResult is empty 
		}
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();      
	}
	return strResult;
}


/********************************************************************/
/*																	*/
/* Function name : IsNumeric										*/
/* Description   : Check if specified data is numeric				*/
/*	检查指定的字符是否是数字										*/
/********************************************************************/
BOOL IsNumeric(char *buff)
{
	//validate data 验证数据
	char *ptr = buff;
	while(*ptr!='\0')
	{
		if(isdigit(*ptr))
		{
			ptr++;
		}
		else
			return FALSE;
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : MakeSureDirectoryPathExists						*/
/* Description   : This function creates all the directories in		*/
/*				   the specified DirPath, beginning with the root.	*/
/*				   This is a clone a Microsoft function with the	*/
/*			       same name.										*/
/*创建目录															*/
/********************************************************************/
BOOL MakeSureDirectoryPathExists(LPCTSTR lpszDirPath)
{
	CString strDirPath = lpszDirPath;
	
	int nPos = 0;
   
	while((nPos = strDirPath.Find('\\', nPos+1)) != -1) 
	{
		CreateDirectory(strDirPath.Left(nPos), NULL);
	}
	return CreateDirectory(strDirPath, NULL);
}

/********************************************************************/
/*																	*/
/* Function name : GetAppDir										*/		
/* Description   : Get application directory.						*/
/*																	*/
/********************************************************************/
void GetAppDir(CString& strAppDir)
{
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	// Get application's full path.
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// Break full path into seperate components.
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	// Store application's drive and path
	strAppDir.Format(_T("%s%s"), szDrive, szDir);
}

// Pump messages while waiting for event
BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout)
{   
	DWORD dwRet;

	while (1)
	{
		// wait for event or message, if it's a message, process it and return to waiting state
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, nTimeout, QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
		{
			TRACE0("WaitWithMessageLoop() event triggered.\n");
			return TRUE;      
		}   
		else
		if (dwRet == WAIT_OBJECT_0 + 1)
		{
			// process window messages
			AfxGetApp()->PumpMessage();
		}  
		else
		if (dwRet == WAIT_TIMEOUT)
		{
			// timed out !
			return FALSE;
		}
		else
		{
			// WAIT_ABANDONED_0 ...
			return TRUE;
		}
	}
}