
/*
	Inet.cpp : implementation of the CInet class

	Implements the internet communication class responsible for downloading files
	from the web server.  It is basically a wrapper class for the WinInet calls.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include <direct.h>
#include <process.h>
#include <io.h>

#include "inet.h"

// Filenames to create if a link just uses a directory for a reference -- 
// g_szDefPage is for HTML pages, g_szUnknown is for multimedia files (in which
// case something fatal happened)
// 
const char *g_szDefPage = "index.htm";
const char *g_szUnknown = "unknown.fil";

// 定义协议类型即HTTP类型
static const char *szHttp = "http://";

// Request flags for session initialization:
//	Use existing INET setup, don't redirect to other servers automatically,
//	don't save pages in the browser's cache, always reload a page from the 
//	server (don't use the browser's cached copy)

static DWORD dwHttpRequestFlags =
	   INTERNET_FLAG_EXISTING_CONNECT|
	   INTERNET_FLAG_NO_AUTO_REDIRECT|
       INTERNET_FLAG_DONT_CACHE|
       INTERNET_FLAG_RELOAD; 

// 能够处理的数据类型，下面的定义表示能够处理所有的类型
static const TCHAR szHeaders[] = 
//		_T("Accept: */*\r\n");
		_T("Accept: */* \r");

// 构造函数
CInet::CInet()
{
	m_hSession = NULL;
	m_nUniqueCount = 0;
}

// 析构函数
CInet::~CInet()
{
}

// 初始化一个新的WinInet 会话
CInet::RESULTS CInet::OpenSession(BOOL bUseProxy, CString& strProxyName)
{
	CInet::RESULTS ret;

	// 打开
	m_hSession = ::InternetOpen("SiteSnagger",
					bUseProxy?INTERNET_OPEN_TYPE_PROXY:INTERNET_OPEN_TYPE_PRECONFIG,
					bUseProxy?(LPCTSTR)strProxyName:(LPCTSTR)NULL,
					NULL,0);

	// 是否成功
	if(m_hSession != NULL)
	{
		DWORD dwTimeout = 4000;
		::InternetSetOption(m_hSession,INTERNET_OPTION_CONNECT_TIMEOUT,
									&dwTimeout,sizeof(dwTimeout));
						
		ret = SUCCESS;
	}
	else ret = BAD_URL;

	return(ret);
}

// Close the WinInet session
void CInet::CloseSession()
{
	::InternetCloseHandle(m_hSession);
}

//////////////////////
// Private operations
//////////////////////

// 从远程服务器获得指定的文件
CInet::RESULTS CInet::DoGet(CString& strURL, BYTE **pbyBuffer, int& nLen, 
											BOOL bIsTextPage)
{
	CInet::RESULTS nRetCode = UNKNOWN_ERROR;
	CString strTempURL = strURL;
	HINTERNET hConnect = NULL;
	HINTERNET hURL = NULL;
   
    try
	{
		// 初始化返回的数据大笑
		nLen = 0;
		
		// 确保文件路径正确
		strTempURL.MakeLower();
		if(strTempURL.Find(szHttp) < 0)
		{
			strTempURL = szHttp+strURL;
			strURL = szHttp+strURL;
		}
		else strTempURL = strURL;

		// 确保文件路径合法
		CString strServer,strObject,strUser,strPassword;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		// 文件路径修正
		strTempURL.SpanExcluding("#");
		if(!AfxParseURLEx(strTempURL,dwServiceType,strServer,strObject,nPort,
						strUser,strPassword,ICU_NO_ENCODE))
			return(CInet::BAD_URL);

		// 建立服务器连接
		hConnect = ::InternetConnect(m_hSession,strServer,
                                          INTERNET_INVALID_PORT_NUMBER,
                                          strUser,
                                          strPassword,
                                          INTERNET_SERVICE_HTTP,
                                          0,
                                          0) ;

		// 打开HTTP会话
		strObject = strObject.SpanExcluding("#");
		const char *szTempX[2] = {szHeaders,NULL};
		hURL = ::HttpOpenRequest(hConnect,"GET",strObject,
									HTTP_VERSION,NULL,
                                    szTempX,dwHttpRequestFlags,0) ;

		// 发送请求
 		::HttpSendRequest(hURL,NULL,0,0,0);

		// 如果失败，则返回
	    if(hURL == NULL) 
			return(CInet::BAD_URL);

		// 检查返回码
		DWORD dwRet, dwLen;
		char szTemp[500];
		dwLen = sizeof(szTemp);
		::HttpQueryInfo(hURL,HTTP_QUERY_STATUS_CODE,szTemp,&dwLen,NULL);
		dwRet = (DWORD) atoi(szTemp);
		dwLen = sizeof(szTemp);
		
		m_dwRet = dwRet;

		// 需要重定向该页面
		if(dwRet == HTTP_STATUS_MOVED || dwRet == HTTP_STATUS_REDIRECT ||
			       dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{                  
			// 获得新的地址
			CString strNewLocation;
			::HttpQueryInfo(hURL,HTTP_QUERY_LOCATION,NULL,&dwLen,0);
			::HttpQueryInfo(hURL,HTTP_QUERY_LOCATION,
						strNewLocation.GetBufferSetLength(dwLen), 
						&dwLen,0);
			strNewLocation.ReleaseBuffer(dwLen);

			// 确信新地址包含服务器名
			CString strNewServer;
			AfxParseURLEx(strNewLocation,dwServiceType,strNewServer,strObject,nPort,
							strUser,strPassword,ICU_NO_ENCODE);

			// 如果不包含服务器名
			if(strNewServer.IsEmpty())
			{
				// 增加服务器名
				CString strTempLocation = strServer;
				if(strNewLocation.Left(1) != "/")
					strTempLocation += "/";
				strTempLocation += strNewLocation;
				strNewLocation = strTempLocation;
			}

			//保存新的地址
			strURL = strNewLocation;

			::InternetCloseHandle(hURL);
			::InternetCloseHandle(hConnect);
			return(LOCATION_MOVED);
		}
		else if(dwRet == HTTP_STATUS_REQUEST_TIMEOUT || 
					dwRet == HTTP_STATUS_GATEWAY_TIMEOUT ||
					dwRet == HTTP_STATUS_NOT_FOUND)
		{
			Sleep(500L);
			::InternetCloseHandle(hURL);
			::InternetCloseHandle(hConnect);
			return(TIMEOUT);
		}
		else if(dwRet == HTTP_STATUS_OK)
		{
			// 获取文件
			BYTE *pbyTempBuffer = *pbyBuffer;
			DWORD dwTotalBytes = 0;
			DWORD dwBytesRead;
			BOOL bOkay = TRUE;

			// 获得大小
			dwLen = sizeof(szTemp);
			::HttpQueryInfo(hURL,HTTP_QUERY_CONTENT_LENGTH,szTemp,&dwLen,NULL);
			dwLen = (DWORD) atol(szTemp);

			// 确保没有获得错误的文件长度
			if(dwLen <= 0) 
				dwLen = 10000;

			// 循环读取数据
			while(bOkay)
			{
				// 读取数据
				bOkay = ::InternetReadFile(hURL,pbyTempBuffer,dwLen,&dwBytesRead);

				// 增加总下载数据量
				if(bOkay && dwBytesRead > 0 && dwTotalBytes < MAX_INET_BUFFER-1)
				{
					dwTotalBytes += dwBytesRead;
					pbyTempBuffer += dwBytesRead;
				}
				else break;
			}

			// 以NULL结束缓冲区
			pbyTempBuffer[0] = '\0';
			nLen = dwTotalBytes;

			// 关闭文件
			::InternetCloseHandle(hURL);
			::InternetCloseHandle(hConnect);

			return(SUCCESS);
		}
	}
	catch(...)
	{
		// Fatal error, just get out
		if(hURL) 
			::InternetCloseHandle(hURL);

		if(hConnect)
			::InternetCloseHandle(hConnect);

		return(UNKNOWN_ERROR);
	}
}

// 保存数据
CInet::RESULTS CInet::SaveFile(CString& strFileName, LPCTSTR lpszDirectory,
								BYTE *pbyBuffer, int nLen)
{
	CFile fileOut;
	CFileException ex;
	CString strTempName = lpszDirectory+strFileName;
	strTempName = strTempName.SpanExcluding("#");

	// 创建当地文件
	if(fileOut.Open(strTempName,CFile::modeCreate|CFile::modeWrite,&ex))
	{
		// 写入数据
		fileOut.Write(pbyBuffer,nLen);

		// 关闭文件
		fileOut.Close();
	}
	else return(BAD_FILENAME);

	return(SUCCESS);
}

/////////////////////
// Public operations
/////////////////////

// Retrieves a page from the web server by calling DoGet().  Also handles
// redirection to other servers as well as retries.
CInet::RESULTS CInet::GetPage(CString& strURL, BYTE **pbyBuffer, int& nLen,
									BOOL bRedirectOkay)
{
	int nRetries = 2;
	CInet::RESULTS ret;

	do 
	{
		// Get the page
		ret = DoGet(strURL,pbyBuffer,nLen,TRUE);
	}
	while(nRetries-- && ((ret == LOCATION_MOVED && bRedirectOkay) || 
							ret == TIMEOUT));

	return(ret);
}

// Retrieves a file from the web server by calling DoGet().  Handles
// redirection to other web servers as well as retries.
CInet::RESULTS CInet::GetFile(CString& strURL, BYTE **pbyBuffer, int& nLen)
{
	int nRetries = 2;
	CInet::RESULTS ret;

	do 
	{
		// Get the file
		ret = DoGet(strURL,pbyBuffer,nLen,FALSE);
	}
	while(nRetries-- && (ret == LOCATION_MOVED || ret == TIMEOUT));

	return(ret);
}

// Reset the "tie breaker" filename counter used by GenerateUniqueFileName().
void CInet::ResetUniqueCount()
{
	m_nUniqueCount = 0;
}

// 产生一个唯一的文件名，如果有相同的文件名，则在后面增加_x来表示新的文件名
// x表示是数量，即是第几个同名的
void CInet::GenerateUniqueFileName(CString& strName, CString& strFileName, 
						MAP_FILES_ARRAY &arrNameMap, BOOL bIsText)
{
	BOOL bFound = FALSE;
	MAP_FILES entry;
	CString strTempName = strName;
	strTempName.MakeLower();
	CString strTempFileName;

	// 分离出URL
	CString strServer,strObject,strUser,strPassword;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	AfxParseURLEx(strName,dwServiceType,strServer,strObject,nPort,strUser,
							strPassword,ICU_NO_ENCODE);

	// 获得合法的文件名
	strFileName = SplitFileName(strObject,CInet::FNAME|CInet::EXT);

	// 是否是一个文件名
	if(strFileName.IsEmpty())
	{
		// 如果不是，使用默认的文件名或者错误
		if(bIsText)
			strFileName = g_szDefPage;
		else strFileName = g_szUnknown;
	}
	else
	{
		// 如果是HTML文件
		if(bIsText)
		{
			// 确保文件名以html文件结尾，这样，即使是.asp文件也能够执行
			CString strExt = SplitFileName(strFileName,CInet::EXT);
			strExt.MakeLower();

			if(strExt.Find("#") < 0 && strExt != ".htm" && strExt != ".html")
			{
				strFileName += ".htm";
			}
		}
	}

	// 获得新的文件名
	strTempFileName = strFileName;
	strTempFileName.MakeLower();
	strTempName.MakeLower();

	MAP_FILES *pEntry;
	CString strPage;

	POSITION pos = arrNameMap.GetStartPosition();

	// 判断是否存在
	for(int nEntryNum = 0; nEntryNum < arrNameMap.GetCount(); nEntryNum++)
	{
		arrNameMap.GetNextAssoc(pos,strPage,(CObject *&) pEntry);
		if(pos == NULL || pEntry == NULL) break;

		// 如果该页已经存在
		if(strPage == strTempName)
		{
			strName = strPage;
			strFileName = pEntry->strFileName;
			return;
		}

		// 如果找到一个重名的输出文件
		//则产生一个唯一的
		if(pEntry->strFileName == strTempFileName)
		{
			// 退出循环
			bFound = TRUE;
			break;
		}
	}

	// 如果找到重名的，则创建一个新的
	if(bFound)
	{
		CString strTemp;
		CString strNewFileName;

		// 寻找后缀名
		int nIndex = strFileName.Find(".");
		if(nIndex >= 0)
			strNewFileName = strFileName.Left(nIndex);
		else strNewFileName = strFileName;

		// 加入到文件名中
		strTemp.Format("__%d",++m_nUniqueCount);
		strNewFileName += strTemp;
		if(nIndex >= 0)
			strNewFileName += strFileName.Mid(nIndex);

		// 保存新文件名
		strFileName = strNewFileName;
	}
}

// 分割文件名
CString CInet::SplitFileName(LPCTSTR lpszPath, int nSplit)
{
	CString strResult;

	char szPath[800],szDrive[800],szFileName[800],szExt[800];
	_splitpath(lpszPath,szDrive,szPath,szFileName,szExt);

	if(nSplit & CInet::DRIVE)
		strResult += szDrive;
	if(nSplit & CInet::PATH)
		strResult += szPath;
	if(nSplit & CInet::FNAME)
		strResult += szFileName;
	if(nSplit & CInet::EXT)
		strResult += szExt;

	return(strResult);
}

CString CInet::GetErrorText()
{
	CString strError;

	switch(m_dwRet)
	{
		case HTTP_STATUS_MOVED:
		case HTTP_STATUS_REDIRECT:
		case HTTP_STATUS_REDIRECT_METHOD:
			strError = "Redirected, but not found";
			break;

		case HTTP_STATUS_REQUEST_TIMEOUT:
		case HTTP_STATUS_GATEWAY_TIMEOUT:
			strError = "Timed out on request";
			break;

		case HTTP_STATUS_NOT_FOUND:
		case HTTP_STATUS_BAD_REQUEST:
		case HTTP_STATUS_GONE:
			strError = "Page or file not found";
			break;
	
		case HTTP_STATUS_DENIED:
		case HTTP_STATUS_PAYMENT_REQ:
		case HTTP_STATUS_FORBIDDEN:
		//case HTTP_STATUS_AUTH_REFUSED:
		//	strError = "Access denied";
		//	break;

		case HTTP_STATUS_PROXY_AUTH_REQ:
			strError = "Proxy authentication required";
			break;

		default:
			strError.Format("WinInet error %d",m_dwRet);
			break;
	}

	return(strError);
}
