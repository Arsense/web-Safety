//=============================================================================================
/*
	Internet.h
	Internet Transfer operator.

	Project	: XFILTER 1.0
	Author	: Tony Zhu
	Create Date	: 2001/08/26
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/
//=============================================================================================
#ifndef INTERNET_H
#define INTERNET_H

class CHttpRequest
{
public:
	CHttpRequest();
	virtual ~CHttpRequest();

private:

public:
	BOOL IsConnected();
	int	 ConnectUrl(TCHAR *sUrl, TCHAR *sReturn = NULL, long *lVersion = 0, int *Count = NULL);
	void Close();

	BOOL UserReg();
	BOOL InitRegister();
	void SetRegisterUrl();
	BOOL InitNetCommand();
	BOOL PreUpload();

public:
	XUSER_INFO			m_UserInfo;
	TCHAR				m_pUrlRequest[1024];
	XNET_COMMAND_HEADER	m_CommandHeader;
	long				*lCommandId;

	HANDLE				m_DownloadThread;
	HANDLE				m_UploadThread;
	BOOL				m_IsUploaded;
	BOOL				m_IsConnecting;
};

DWORD WINAPI UploadUserInfo(LPVOID pVoid);
DWORD WINAPI DownloadCommandFile(LPVOID pVoid);

#endif

