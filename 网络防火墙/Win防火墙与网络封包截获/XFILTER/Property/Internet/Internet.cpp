//=============================================================================================
/*
	文件：		Internet.cpp

	说明:
	---------------------------------------------------
		网络数据传输操作，用来完成用户注册信息的上载和
	网络命令的下载。
	---------------------------------------------------

	工程：		Xfilter 个人防火墙
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/26
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	---------------------------------------------------
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买本软件源代码的用户被默认授权
	可以在自己的程序中使用本软件的部分代码，但作者不对
	代码产生的任何后果负责。
	
		使用了本软件代码的程序只能以可执行文件形式发布，
	未经特别许可，不能将含有本软件源代码的源程序以任何
	形式发布。
	---------------------------------------------------	
*/
//=============================================================================================

#include "stdafx.h"

#include "..\guires.h"
#include "..\Property.h"		// in the header #include "internet.h"
#include "..\Register.h"

const TCHAR szHeaders[] =
	_T("Accept: text/*\r\nUser-Agent: XFILTER\r\n");

extern CAclFile		m_AclFile;
extern CPropertyApp theApp;

CHttpRequest::CHttpRequest()
{
	m_pUrlRequest[0]		= '\0';
	m_DownloadThread		= NULL;
	m_UploadThread			= NULL;
	m_IsUploaded			= TRUE;
	lCommandId				= NULL;
	m_IsConnecting			= FALSE;
}

CHttpRequest::~CHttpRequest()
{
	Close();
}

int CHttpRequest::ConnectUrl(TCHAR *sUrl, TCHAR *sReturn, long *lVersion, int *Count)
{
	ODS(_T("XFILTER.EXE: GetFromUrl Begin... "));
	
	if(sUrl == NULL)
		return XERR_INVALID_PARAMETER;

	CString			strServerName;
	CString			strObject;
	INTERNET_PORT	nPort;
	DWORD			dwServiceType;

	if (!AfxParseURL(sUrl, dwServiceType, strServerName, strObject, nPort) ||
		dwServiceType != INTERNET_SERVICE_HTTP)
	{
		ODS(_T("XFILTER.EXE: Internet Invalid Url ..."));
		return XERR_INTERNET_URL_ERROR;
	}

	CInternetSession	session(GUI_APP_CLASS_NAME);
	CHttpConnection		*pServer	= NULL;
	CHttpFile			*pFile		= NULL;
	int					iRet		= XERR_SUCCESS;

	m_IsConnecting = TRUE;

	try
	{
		pServer = session.GetHttpConnection(strServerName, nPort);
		pFile	= pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);

		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();

		DWORD	dwRet;
		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet >= 400 && dwRet <= 499)
		{
			ODS(_T("XFILTER.EXE: Internet Request Error ..."));
			iRet = XERR_INTERNET_REQUEST_ERROR;
		}
		else if(dwRet >= 500 && dwRet <= 599)
		{
			ODS(_T("XFILTER.EXE: Internet Server Error ..."));
			iRet = XERR_INTERNET_SERVER_ERROR;
		}
		else if(sReturn != NULL)
		{
			pFile->ReadString(sReturn, MAX_NET_COMMAND_LENTH - 1);
			ODS(sReturn);

			CString tmpStr	= sReturn;
			long lVer		= atol(tmpStr.Left(MAX_NET_COMMAND_VERSION_LENTH));

			if(lVer > *lVersion)
			{
				*lVersion = lVer;
				int		i = 1;

				while (i < MAX_NET_COMMAND
					&& pFile->ReadString((sReturn + MAX_NET_COMMAND_LENTH * i), MAX_NET_COMMAND_LENTH - 1))
				{
					ODS(sReturn + i * MAX_NET_COMMAND_LENTH);
					i ++;
				}
				*Count = i;
			}
		}
		else
		{
			CString sRet;
			pFile->ReadString(sRet);
			if(sRet.GetAt(0) != '1')
				iRet = XERR_INTERNET_REG_ERROR;

			ODS2(_T("XFILTER.EXE: Internet User Register Return Value "),sRet);
		}

		pFile->Close();
		pServer->Close();
	}
	catch(CInternetException* pEx)
	{

#if _DEBUG
		TCHAR sError[1024];
		pEx->GetErrorMessage(sError, 1024);
		ODS(sError);
#endif
		pEx->Delete();
		iRet = XERR_INTERNET_CONNECT_ERROR;
	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();

	m_IsConnecting = FALSE;

	ODS(_T("XFILTER.EXE: GetFromUrl End... "));
	return iRet;
}

void CHttpRequest::Close()
{
	if(lCommandId != NULL)
	{
		delete lCommandId;
		lCommandId = NULL;
	}

	if(m_UploadThread != NULL)
	{
		TerminateThread(m_UploadThread, 0);
		m_UploadThread = NULL;
	}

	if(m_DownloadThread != NULL)
	{
		TerminateThread(m_DownloadThread, 0);
		m_DownloadThread = NULL;
	}

	ODS(_T("XFILTER.EXE: Internet Close delete[] lCommandId and exit thread..."));
}

BOOL CHttpRequest::IsConnected()
{
	ODS(_T("XFILTER.EXE: Internet Check Connected..."));

    RASCONN			lpRasConn;
	RASCONNSTATUS	rasStatus;     
	DWORD			cbBuf = 0;     
	DWORD			cConn = 0;     
	DWORD			dwRet = 0; 
	cbBuf				= sizeof(RASCONN);
	lpRasConn.dwSize	= sizeof(RASCONN );
	dwRet = RasEnumConnections(&lpRasConn, &cbBuf, &cConn );
	if ( dwRet != 0 )   
		return FALSE;
	else
	{
		rasStatus.dwSize = sizeof(RASCONNSTATUS);
		RasGetConnectStatus(lpRasConn.hrasconn,&rasStatus);
		if (rasStatus.rasconnstate==RASCS_Connected)
			return TRUE;
		else
			return FALSE;
	}
	return TRUE;
}

BOOL CHttpRequest::UserReg()
{
	CRegister dlgReg;

	int iRet = dlgReg.DoModal();

	if(iRet == IDCANCEL)
		return FALSE;
	
	m_UserInfo.iStatus = REG_STATUS_REGISTERING;

	return theApp.m_Install.SaveReg(REG_INFO_ITEM, (BYTE*)&m_UserInfo, sizeof(XUSER_INFO));
}

BOOL CHttpRequest::InitRegister()
{
	if(m_AclFile.mAclHeader.sSignature[0] == '\0' || m_AclFile.mAclHeader.sWebURL[0] == '\0')
		return FALSE;

	theApp.m_Install.ReadReg(REG_INFO_ITEM, (BYTE*)&m_UserInfo, sizeof(XUSER_INFO));

	if(m_UserInfo.iStatus == REG_STATUS_NO_REGISTER)
	{
		theApp.SetAutoStart();

		if(!UserReg())
			return FALSE;

		m_IsUploaded = PreUpload();
	}
	else if(m_UserInfo.iStatus == REG_STATUS_REGISTERING 
		|| m_UserInfo.iStatus == REG_STATUS_INFO_CHANGED)
		m_IsUploaded = PreUpload();

	return TRUE;
}

BOOL CHttpRequest::PreUpload()
{
	DWORD		dwThreadId;
	DWORD		dwFlags;

	if(!InternetGetConnectedState(&dwFlags, 0))
		return FALSE;

	SetRegisterUrl();
	m_UploadThread = ::CreateThread(NULL, 0, UploadUserInfo, 0, 0, &dwThreadId);

	return TRUE;
}

void CHttpRequest::SetRegisterUrl()
{
	CString		tmpStr;

	tmpStr.Format(
		_T("%s?sEmail=%s&sName=%s&bGender=%u&sQQ=%s&sInc=%s&sBirthday=%s&bDegree=%u&bMetier=%u&sDuty=%s&sZip=%s&bSalary=%u&sAddress=%s&iProductId=%u&sPassword=%s")
		, m_AclFile.mAclHeader.sUserRegisterURL
		, m_UserInfo.sEmail
		, m_UserInfo.sName
		, m_UserInfo.bGender 
		, m_UserInfo.sQQ 
		, m_UserInfo.sInc
		, m_UserInfo.tBirthday.Format("%Y-%m-%d")
		, m_UserInfo.bDegree 
		, m_UserInfo.bMetier
		, m_UserInfo.sDuty 
		, m_UserInfo.sZip  
		, m_UserInfo.bSalary 
		, m_UserInfo.sAddress
		, m_AclFile.mAclHeader.uiSerial
		, m_UserInfo.sPassword 
		);

	_tcscpy(m_pUrlRequest, tmpStr);

	ODS(tmpStr);
}

BOOL CHttpRequest::InitNetCommand()
{
	if(m_AclFile.mAclHeader.sSignature[0] == '\0' 
		|| m_AclFile.mAclHeader.sCommandURL[0] == '\0')
		return FALSE;

	DWORD		dwThreadId;
	theApp.m_Install.ReadReg(REG_NET_COMMAND_HEADER_ITEM, (BYTE*)&m_CommandHeader, sizeof(XNET_COMMAND_HEADER));
	
	if(m_AclFile.mAclHeader.bUpdateInterval > 100)
		return TRUE;

	if(m_AclFile.mAclHeader.bUpdateInterval == 0 
		|| m_CommandHeader.tCheckTime == 0
		|| (CTime::GetCurrentTime() - m_CommandHeader.tCheckTime) 
		>= ACL_MAX_TIME * m_AclFile.mAclHeader.bUpdateInterval) 
	{
		m_DownloadThread = ::CreateThread(NULL, 0, DownloadCommandFile, 0, 0, &dwThreadId);
	}

	return TRUE;
}

DWORD WINAPI DownloadCommandFile(LPVOID pVoid)
{
	if(m_AclFile.mAclHeader.sCommandURL[0] == '\0')
		return 0;
	
	BOOL			IsSuccess	= FALSE;
	BOOL			IsAclChange = FALSE;
	int				iCount		= 0;
	int				tmpCount	= 0;
	TCHAR			sCommand[MAX_NET_COMMAND][MAX_NET_COMMAND_LENTH];

	if(theApp.m_DownloadCommand.m_CommandHeader.lCount > 0)
	{
		theApp.m_DownloadCommand.lCommandId = new long[theApp.m_DownloadCommand.m_CommandHeader.lCount];
		theApp.m_Install.ReadReg(REG_NET_COMMAND_ITEM
			, (BYTE*)theApp.m_DownloadCommand.lCommandId, sizeof(long) * theApp.m_DownloadCommand.m_CommandHeader.lCount);
	}

	while(!IsSuccess)
	{
		if(++iCount > 3)
			break;

		if(theApp.m_DownloadCommand.ConnectUrl(m_AclFile.mAclHeader.sCommandURL
			, (TCHAR*)sCommand, &theApp.m_DownloadCommand.m_CommandHeader.lVersion,&tmpCount) != XERR_SUCCESS)
		{
			Sleep(180000);
			continue;
		}

		if(tmpCount <= 1)
			break;

		CString		tmpStr;
		long		tmpLong;

		for(int i = 1; i < tmpCount; i++)
		{
			tmpStr	= sCommand[i];
			tmpLong = atol(tmpStr.Left(MAX_NET_COMMAND_VERSION_LENTH));

			for(int j = 0; j < theApp.m_DownloadCommand.m_CommandHeader.lCount; j++)
			{
				if(tmpLong == theApp.m_DownloadCommand.lCommandId [j])
					break;
			}

			if(j < theApp.m_DownloadCommand.m_CommandHeader.lCount)
				continue;

			long *pLong = NULL;

			if(theApp.m_DownloadCommand.m_CommandHeader.lCount > 0)
			{
				pLong = new long[theApp.m_DownloadCommand.m_CommandHeader.lCount];
				memcpy(pLong, theApp.m_DownloadCommand.lCommandId, sizeof(long) * theApp.m_DownloadCommand.m_CommandHeader.lCount);
				delete[](theApp.m_DownloadCommand.lCommandId);
			}

			theApp.m_DownloadCommand.lCommandId = new long[theApp.m_DownloadCommand.m_CommandHeader.lCount + 1];

			if(theApp.m_DownloadCommand.m_CommandHeader.lCount > 0)
			{
				memcpy(theApp.m_DownloadCommand.lCommandId, pLong, sizeof(long) * theApp.m_DownloadCommand.m_CommandHeader.lCount);
				delete[](pLong);
			}

			theApp.m_DownloadCommand.lCommandId[theApp.m_DownloadCommand.m_CommandHeader.lCount] = tmpLong;
			theApp.m_DownloadCommand.m_CommandHeader.lCount ++;

			int iCommand	= atoi(tmpStr.Mid(MAX_NET_COMMAND_VERSION_LENTH + 1
								, MAX_NET_COMMAND_COMMAND_LENTH));
			CString sMessage= tmpStr.Mid(MAX_NET_COMMAND_VERSION_LENTH 
								+ MAX_NET_COMMAND_COMMAND_LENTH + 2);

			sMessage.Replace(13, '\0');
			sMessage.Replace(10, '\0');

			if(sMessage.GetAt(0) == '\0')
				break;
			
			BYTE bInterval;

			switch(iCommand)
			{
			case NET_COMMAND_CHANGE_WEB_STATION_URL:
				if(sMessage.CompareNoCase(m_AclFile.mAclHeader.sWebURL) == 0)
					break;
				_tcscpy(m_AclFile.mAclHeader.sWebURL, sMessage);
				IsAclChange = TRUE;
				break;

			case NET_COMMAND_CHANGE_NET_COMMAND_URL:
				if(sMessage.CompareNoCase(m_AclFile.mAclHeader.sCommandURL) == 0)
					break;
				_tcscpy(m_AclFile.mAclHeader.sCommandURL, sMessage);
				IsAclChange = TRUE;
				break;

			case NET_COMMAND_CHANGE_USER_REGISTER_URL:
				if(sMessage.CompareNoCase(m_AclFile.mAclHeader.sUserRegisterURL) == 0)
					break;
				_tcscpy(m_AclFile.mAclHeader.sUserRegisterURL, sMessage);
				IsAclChange = TRUE;
				break;

			case NET_COMMAND_CHANGE_EMAIL_ADDRESS:	
				if(sMessage.CompareNoCase(m_AclFile.mAclHeader.sEmail) == 0)
					break;
				_tcscpy(m_AclFile.mAclHeader.sEmail, sMessage);
				IsAclChange = TRUE;
				break;

			case NET_COMMAND_CHANGE_UPDATE_INTERVAL_DAYS:
				bInterval	= BYTE(atoi(sMessage));
				if(bInterval== m_AclFile.mAclHeader.bUpdateInterval)
					break;
				m_AclFile.mAclHeader.bUpdateInterval = bInterval;
				IsAclChange	= TRUE;
				break;

			case NET_COMMAND_CHANGE_POST_MESSAGE:
				_tcscpy(theApp.m_sMessage[i], sMessage);
				::SendMessage(theApp.m_pMainWnd->m_hWnd, WM_NET_MESSAGE, i, NULL);
				break;

			default:
				break;
			}
		}

		theApp.m_DownloadCommand.m_CommandHeader.tCheckTime = CTime::GetCurrentTime();

		theApp.m_Install.SaveReg(REG_NET_COMMAND_HEADER_ITEM, 
			(BYTE*)&theApp.m_DownloadCommand.m_CommandHeader, sizeof(XNET_COMMAND_HEADER));

		theApp.m_Install.SaveReg(REG_NET_COMMAND_ITEM, (BYTE*)theApp.m_DownloadCommand.lCommandId
			, sizeof(long) * theApp.m_DownloadCommand.m_CommandHeader.lCount);

		if(IsAclChange)
			m_AclFile.SaveAcl();

		IsSuccess = TRUE;

		break;
	}

	if(theApp.m_DownloadCommand.lCommandId != NULL)
	{
		delete theApp.m_DownloadCommand.lCommandId;
		theApp.m_DownloadCommand.lCommandId = NULL;
	}

	return 0;
}

DWORD WINAPI UploadUserInfo(LPVOID pVoid)
{
	if(theApp.m_RegisterRequest.m_pUrlRequest[0] == '\0')
		return 0;

	BOOL	IsUploaded	= FALSE;
	BOOL	IsSaved		= FALSE;
	int		iCount		= 0;

	while(!IsUploaded || !IsSaved)
	{
		if(++iCount > 3)
			break;

		if(!IsUploaded && theApp.m_RegisterRequest.ConnectUrl(
			theApp.m_RegisterRequest.m_pUrlRequest
			) == XERR_SUCCESS)
		{
			theApp.m_RegisterRequest.m_UserInfo.iStatus = REG_STATUS_REGISTERED;
			IsUploaded = TRUE;
		}

		if(theApp.m_Install.SaveReg(REG_INFO_ITEM, (BYTE*)&theApp.m_RegisterRequest.m_UserInfo, sizeof(XUSER_INFO)))
		{
			IsSaved = TRUE;
			break;
		}
		
		Sleep(180000);
	}

	return 0;
}

