#if !defined(__Inet_h__)
#define __Inet_h__


class MAP_FILES : public CObject
{
public:
	CString strFileName;
	int nMaxLevel;
};

typedef CMapStringToOb MAP_FILES_ARRAY;

#define MAX_INET_BUFFER	700000L

class CInternetDownload
{
private:
	HINTERNET m_hSession;
	int m_nUniqueCount;
	DWORD m_dwRet;

private:
	void DoConnect(LPCTSTR lpszURL, BOOL bFileDownload);

public:
	CInternetDownload();
	~CInternetDownload();

	typedef enum
	{
		SUCCESS,
		BAD_URL,
		BAD_FILENAME,
		LOCATION_MOVED,
		UNKNOWN_ERROR,
		ALREADY_EXISTS,
		TIMEOUT,
	} RESULTS;

	typedef enum
	{
		DRIVE = 0x0001,
		PATH = 0x0002,
		FNAME = 0x0004,
		EXT = 0x0008,
	} SPLIT;

public:
	CString GetErrorText();

	void ResetUniqueCount();
	RESULTS DoGet(CString& strURL, BYTE **pbyBuffer, int& nLen, 
									BOOL bIsTextPage);
	RESULTS GetPage(CString& strURL, BYTE **pbyBuffer, int& nLen,
									BOOL bRedirectOkay);
	RESULTS GetFile(CString& strURL, BYTE **pbyBuffer, int& nLen);
	RESULTS SaveFile(CString& strFileName, LPCTSTR lpszDirectory, 
									BYTE *pbyBuffer, int nLen);
	RESULTS OpenSession(BOOL bUseProxy, CString& strProxyName);

	void CloseSession();

	void GenerateUniqueFileName(CString& strName, CString& strFileName, 
							MAP_FILES_ARRAY &arrNameMap,
							BOOL bIsText);

	static CString SplitFileName(LPCTSTR lpszPath, int nSplit);

};

#endif
