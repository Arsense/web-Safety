#include "stdafx.h"

#include <direct.h>
#include <process.h>
#include <io.h>
#include "InternetDownload.h"


// Filenames to create if a link just uses a directory for a reference -- 
// g_szDefPage is for HTML pages, g_szUnknown is for multimedia files (in which
// case something fatal happened)
const char *g_szDefPage = "index.htm";
const char *g_szUnknown = "unknown.fil";

// The default prefix -- we only support HTTP protocols
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

// Types of data that we can handle -- we handle it all!
static const TCHAR szHeaders[] = 
		_T("Accept: */* \r");

// Constructor
CInternetDownload::CInternetDownload()
{
	m_hSession = NULL;
	m_nUniqueCount = 0;
}

// Destructor
CInternetDownload::~CInternetDownload()
{
}

// Initialize a new WinInet session
CInternetDownload::RESULTS CInternetDownload::OpenSession(BOOL bUseProxy, CString& strProxyName)
{
	CInternetDownload::RESULTS ret;

	// Open the new session
	m_hSession = ::InternetOpen("SiteSnagger",
					bUseProxy?INTERNET_OPEN_TYPE_PROXY:INTERNET_OPEN_TYPE_PRECONFIG,
					bUseProxy?(LPCTSTR)strProxyName:(LPCTSTR)NULL,
					NULL,0);

	// Did it work??
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
void CInternetDownload::CloseSession()
{
	::InternetCloseHandle(m_hSession);
}

//////////////////////
// Private operations
//////////////////////

// Gets the specified file from the web server
CInternetDownload::RESULTS CInternetDownload::DoGet(CString& strURL, BYTE **pbyBuffer, int& nLen, 
											BOOL bIsTextPage)
{
	CInternetDownload::RESULTS nRetCode = UNKNOWN_ERROR;
	CString strTempURL = strURL;
	HINTERNET hConnect = NULL;
	HINTERNET hURL = NULL;
   
    try
	{
		// Initalize returned data count
		nLen = 0;
		
		// Make sure that we have the proper protocol prefix
		strTempURL.MakeLower();
		if(strTempURL.Find(szHttp) < 0)
		{
			strTempURL = szHttp+strURL;
			strURL = szHttp+strURL;
		}
		else strTempURL = strURL;

		// Make sure this URL looks valid
		CString strServer,strObject,strUser,strPassword;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		// Bail if it doesn't look okay
		strTempURL.SpanExcluding("#");
		if(!AfxParseURLEx(strTempURL,dwServiceType,strServer,strObject,nPort,
						strUser,strPassword,ICU_NO_ENCODE))
			return(CInternetDownload::BAD_URL);

		// Establish a server connection
		hConnect = ::InternetConnect(m_hSession,strServer,
                                          INTERNET_INVALID_PORT_NUMBER,
                                          "",
                                          "",
                                          INTERNET_SERVICE_HTTP,
                                          0,
                                          0) ;

		// Open an HTTP protocol session
		strObject = strObject.SpanExcluding("#");
		const char *szTempX[2] = {szHeaders,NULL};
		hURL = ::HttpOpenRequest(hConnect,"GET",strObject,
									HTTP_VERSION,NULL,
                                    szTempX,dwHttpRequestFlags,0) ;

		// Send the "get" request
 		::HttpSendRequest(hURL,NULL,0,0,0);

		// If it failed, then get out now
	    if(hURL == NULL) 
			return(CInternetDownload::BAD_URL);

		// Check the return code
		DWORD dwRet, dwLen;
		char szTemp[500];
		dwLen = sizeof(szTemp);
		::HttpQueryInfo(hURL,HTTP_QUERY_STATUS_CODE,szTemp,&dwLen,NULL);
		dwRet = (DWORD) atoi(szTemp);
		dwLen = sizeof(szTemp);
		
		m_dwRet = dwRet;

		// Was this page redirected?
		if(dwRet == HTTP_STATUS_MOVED || dwRet == HTTP_STATUS_REDIRECT ||
			       dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{                  
			// Yep...get the new base location
			CString strNewLocation;
			::HttpQueryInfo(hURL,HTTP_QUERY_LOCATION,NULL,&dwLen,0);
			::HttpQueryInfo(hURL,HTTP_QUERY_LOCATION,
						strNewLocation.GetBufferSetLength(dwLen), 
						&dwLen,0);
			strNewLocation.ReleaseBuffer(dwLen);

			// Make sure that the new address contains a server name too
			CString strNewServer;
			AfxParseURLEx(strNewLocation,dwServiceType,strNewServer,strObject,nPort,
							strUser,strPassword,ICU_NO_ENCODE);

			// Is the server name missing for some reason??
			if(strNewServer.IsEmpty())
			{
				// Yes, add it back in
				CString strTempLocation = strServer;
				if(strNewLocation.Left(1) != "/")
					strTempLocation += "/";
				strTempLocation += strNewLocation;
				strNewLocation = strTempLocation;
			}

			// Save the new location
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
			// Get the data for the file
			BYTE *pbyTempBuffer = *pbyBuffer;
			DWORD dwTotalBytes = 0;
			DWORD dwBytesRead;
			BOOL bOkay = TRUE;

			// Find out how much data we have
			dwLen = sizeof(szTemp);
			::HttpQueryInfo(hURL,HTTP_QUERY_CONTENT_LENGTH,szTemp,&dwLen,NULL);
			dwLen = (DWORD) atol(szTemp);

			// Just make sure we didn't get a stupid length value back
			if(dwLen <= 0) 
				dwLen = 10000;

			// Keep reading data until we've gotten it all
			while(bOkay)
			{
				// Read the data
				bOkay = ::InternetReadFile(hURL,pbyTempBuffer,dwLen,&dwBytesRead);

				// Add to the counts
				if(bOkay && dwBytesRead > 0 && dwTotalBytes < MAX_INET_BUFFER-1)
				{
					dwTotalBytes += dwBytesRead;
					pbyTempBuffer += dwBytesRead;
				}
				else break;
			}

			// NULL terminate the buffer
			pbyTempBuffer[0] = '\0';
			nLen = dwTotalBytes;

			// Close the file
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
	return (UNKNOWN_ERROR);
}

// Saves the page in memory to the specified file
CInternetDownload::RESULTS CInternetDownload::SaveFile(CString& strFileName, LPCTSTR lpszDirectory,
								BYTE *pbyBuffer, int nLen)
{
	CFile fileOut;
	CFileException ex;
	CString strTempName = lpszDirectory+strFileName;
	strTempName = strTempName.SpanExcluding("#");

	// Open the local file for create 
	if(fileOut.Open(strTempName,CFile::modeCreate|CFile::modeWrite,&ex))
	{
		// Write the data
		fileOut.Write(pbyBuffer,nLen);

		// Close the file
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
CInternetDownload::RESULTS CInternetDownload::GetPage(CString& strURL, BYTE **pbyBuffer, int& nLen,
									BOOL bRedirectOkay)
{
	int nRetries = 2;
	CInternetDownload::RESULTS ret;

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
CInternetDownload::RESULTS CInternetDownload::GetFile(CString& strURL, BYTE **pbyBuffer, int& nLen)
{
	int nRetries = 2;
	CInternetDownload::RESULTS ret;

	do 
	{
		// Get the file
		ret = DoGet(strURL,pbyBuffer,nLen,FALSE);
	}
	while(nRetries-- && (ret == LOCATION_MOVED || ret == TIMEOUT));

	return(ret);
}

// Reset the "tie breaker" filename counter used by GenerateUniqueFileName().
void CInternetDownload::ResetUniqueCount()
{
	m_nUniqueCount = 0;
}

// Generates a unique filename for the specified file by searching through
// the specified list of downloaded files and looking for a match.  If one
// is found a unque name is generated by taking the old name and appending
// "__x" to the main part of the filename (where x is m_nUniqueCount).
void CInternetDownload::GenerateUniqueFileName(CString& strName, CString& strFileName, 
						MAP_FILES_ARRAY &arrNameMap, BOOL bIsText)
{
	BOOL bFound = FALSE;
	MAP_FILES entry;
	CString strTempName = strName;
	strTempName.MakeLower();
	CString strTempFileName;

	// Split apart the URL
	CString strServer,strObject,strUser,strPassword;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	AfxParseURLEx(strName,dwServiceType,strServer,strObject,nPort,strUser,
							strPassword,ICU_NO_ENCODE);

	// Get a valid filename
	strFileName = SplitFileName(strObject,CInternetDownload::FNAME|CInternetDownload::EXT);

	// Is there a filename at all??
	if(strFileName.IsEmpty())
	{
		// No, use a default HTML name or a fatal error name
		if(bIsText)
			strFileName = g_szDefPage;
		else strFileName = g_szUnknown;
	}
	else
	{
		// HTML file??
		if(bIsText)
		{
			// Yes, always make sure it ends in .htm, this assures that things
			// such as Active Server Pages (.asp) are loaded correctly.
			CString strExt = SplitFileName(strFileName,CInternetDownload::EXT);
			strExt.MakeLower();

			if(strExt.Find("#") < 0 && strExt != ".htm" && strExt != ".html")
			{
				strFileName += ".htm";
			}
		}
	}

	// Get the new filename
	strTempFileName = strFileName;
	strTempFileName.MakeLower();
	strTempName.MakeLower();

	MAP_FILES *pEntry;
	CString strPage;

	POSITION pos = arrNameMap.GetStartPosition();

	// Name already there???
	for(int nEntryNum = 0; nEntryNum < arrNameMap.GetCount(); nEntryNum++)
	{
		arrNameMap.GetNextAssoc(pos,strPage,(CObject *&) pEntry);
		if(pos == NULL || pEntry == NULL) break;

		// This page is already in the collection, don't create a name for it
		if(strPage == strTempName)
		{
			strName = strPage;
			strFileName = pEntry->strFileName;
			return;
		}

		// Found a duplicate output filename -- generate a unique one now
		if(pEntry->strFileName == strTempFileName)
		{
			// Get out of the loop now so we don't increment the entry number
			bFound = TRUE;
			break;
		}
	}

	// If we found a duplicate, then make sure we create a unique filename using
	// the m_nUniqueCount.
	if(bFound)
	{
		CString strTemp;
		CString strNewFileName;

		// Find the extension
		int nIndex = strFileName.Find(".");
		if(nIndex >= 0)
			strNewFileName = strFileName.Left(nIndex);
		else strNewFileName = strFileName;

		// Append the tie breaker to the main part of the name
		strTemp.Format("__%d",++m_nUniqueCount);
		strNewFileName += strTemp;
		if(nIndex >= 0)
			strNewFileName += strFileName.Mid(nIndex);

		// Save the new name
		strFileName = strNewFileName;
	}
}

// Parses a filename into the specified parts and returns a CString that
// consists of the specified parts.
CString CInternetDownload::SplitFileName(LPCTSTR lpszPath, int nSplit)
{
	CString strResult;

	char szPath[800],szDrive[800],szFileName[800],szExt[800];
	_splitpath(lpszPath,szDrive,szPath,szFileName,szExt);

	if(nSplit & CInternetDownload::DRIVE)
		strResult += szDrive;
	if(nSplit & CInternetDownload::PATH)
		strResult += szPath;
	if(nSplit & CInternetDownload::FNAME)
		strResult += szFileName;
	if(nSplit & CInternetDownload::EXT)
		strResult += szExt;

	return(strResult);
}

CString CInternetDownload::GetErrorText()
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
			strError = "Access denied";
			break;

		case HTTP_STATUS_PROXY_AUTH_REQ:
			strError = "Proxy authentication required";
			break;

		default:
			strError.Format("WinInet error %d",m_dwRet);
			break;
	}

	return(strError);
}
