/*——————————————————————————————————————
	分层服务提供者安装工程
	创建日期：2001年12月21日
	http://www.xfilt.com
	Email:xstudio@xfilt.com
*/

//——————————————————————————————————————
// 使用Unicode编码
//
#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <ws2spi.h>
#include <sporder.h>
#include <TCHAR.H>

//
// 默认安装的服务提供者文件名和路径变量
//
#define		PROVIDER_PATH	L"MinLSP.dll"
TCHAR		m_sProviderPath[MAX_PATH];

//
//服务提供者的唯一编号，可以自定义
//
GUID ProviderGuid = {		//6acd2327-3dea-2123-4123-003792ead212
	0x6acd2327,
	0x3dea,
	0x2123,
	{0x41, 0x23, 0x00, 0x37, 0x92, 0xea, 0xd2, 0x12}
};

GUID ProviderChainGuid = {	//6acd2327-3dea-2123-4123-003792ead213
	0x6acd2327,
	0x3dea,
	0x2123,
	{0x41, 0x23, 0x00, 0x37, 0x92, 0xea, 0xd2, 0x13}
};

//
//函数预声明
//
void GetPathName(OUT TCHAR *sPath);
WCHAR* S2W(char* ansi);
BOOL GetProviders();
void InstallProvider(void);
void RemoveProvider(void);

//
// 全局变量，用来临时保存全部服务提供者的信息
//
LPWSAPROTOCOL_INFOW ProtocolInfo		= NULL;
DWORD				ProtocolInfoSize	= 0;
INT					TotalProtocols		= 0;

void main(int argc, char *argv[])
{
	if (argc > 1)
	{
		if (argc == 2)
			GetPathName(m_sProviderPath);
		else
			_tcscpy(m_sProviderPath, S2W(argv[2]));

		if (!strcmp(argv[1], "-install"))
		{
			InstallProvider();
			return;
		}
		if (!strcmp(argv[1], "-remove"))
		{
			RemoveProvider();
			return;
		}
	}
	printf("Usage: 'Install -install'\n");
	printf("       to install library of current path's MinLSP.dll\n");
	printf("Usage: 'Install -install LibraryPath(eg: d:\\temp\\lsp.dll)'\n");
	printf("       to install custom Library\n");
	printf("Usage: 'Install -remove'\n");
	printf("       to remove the library\n");
}

void GetPathName(OUT TCHAR *sPath) 
{
	TCHAR sFilename[MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	GetModuleFileName(NULL, sFilename, _MAX_PATH);
	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);
	_tcscpy(sPath, sDrive);
	_tcscat(sPath, sDir);
	if(sPath[_tcslen(sPath) - 1] != _T('\\'))
		_tcscat(sPath, _T("\\"));
	_tcscat(sPath, PROVIDER_PATH);
}  

WCHAR* S2W(char* ansi)
{
	int i;
	WCHAR* unicode;
	unicode = (WCHAR*) malloc ((strlen(ansi)+2)*sizeof(WCHAR));
	for (i = 0; i<(signed)strlen(ansi)+1; i++)
		unicode[i] = ansi[i];
	unicode[i] = 0;
	return unicode;
}

BOOL GetProviders()
{
	INT ErrorCode;

	ProtocolInfo		= NULL;
	ProtocolInfoSize	= 0;
	TotalProtocols		= 0;

	if (WSCEnumProtocols(NULL, ProtocolInfo, &ProtocolInfoSize
		, &ErrorCode) == SOCKET_ERROR && ErrorCode != WSAENOBUFS)
	{
		printf("First WSCEnumProtocols failed %d\n", ErrorCode);
		return(FALSE);
	}

	if ((ProtocolInfo = (LPWSAPROTOCOL_INFOW) 
		GlobalAlloc(GPTR, ProtocolInfoSize)) == NULL)
	{
		printf("Cannot enumerate Protocols %d\n", GetLastError());
		return(FALSE);
	}

	if ((TotalProtocols = WSCEnumProtocols(NULL
		, ProtocolInfo, &ProtocolInfoSize, &ErrorCode)) == SOCKET_ERROR)
	{
		printf("Second WSCEnumProtocols failed %d\n", ErrorCode);
		return(FALSE);
	}

	printf("Found %d protocols\n",TotalProtocols); 
	return(TRUE);
}

void FreeProviders(void)
{
	GlobalFree(ProtocolInfo);
}

void SetProtocolInfo(
	OUT	BOOL	&IsSet,
	OUT DWORD	&CatalogId,
	IN	WSAPROTOCOL_INFOW &InProtocolInfo,
	OUT WSAPROTOCOL_INFOW &OutProtocolInfo1,
	OUT WSAPROTOCOL_INFOW *pOutProtocolInfo2
)
{
	IsSet		= TRUE;
	CatalogId	= InProtocolInfo.dwCatalogEntryId;

	memcpy(&OutProtocolInfo1, &InProtocolInfo, sizeof(WSAPROTOCOL_INFOW));
	OutProtocolInfo1.dwServiceFlags1 
		= InProtocolInfo.dwServiceFlags1 & (~XP1_IFS_HANDLES); 
	if(pOutProtocolInfo2 != NULL)
		memcpy(pOutProtocolInfo2, &OutProtocolInfo1, sizeof(WSAPROTOCOL_INFOW));
}

void SetProtocolChain(
	IN		TCHAR *sName,
	IN OUT	WSAPROTOCOL_INFOW &ProtocolInfo,
	IN		DWORD LayeredCatalogId,
	IN		DWORD NextCatalogId,
	OUT		WSAPROTOCOL_INFOW &OutProtocolInfo
)
{
	WCHAR	ChainName[WSAPROTOCOL_LEN+1];
	swprintf(ChainName, L"%s [%s]", sName, ProtocolInfo.szProtocol);
	wcscpy(ProtocolInfo.szProtocol, ChainName);

	ProtocolInfo.ProtocolChain.ChainEntries[0]	= LayeredCatalogId;
	ProtocolInfo.ProtocolChain.ChainEntries[ProtocolInfo.ProtocolChain.ChainLen] 
												= NextCatalogId;
	ProtocolInfo.ProtocolChain.ChainLen++;

	memcpy(&OutProtocolInfo, &ProtocolInfo, sizeof(WSAPROTOCOL_INFOW));
}

void InstallProvider(void)
{
	INT					ErrorCode;
	LPDWORD				CatalogEntries;
	INT					i;
	INT					CatIndex;
	DWORD				LayeredCatalogId, RawOrigCatalogId;
	DWORD				TcpOrigCatalogId, UdpOrigCatalogId;
	WSAPROTOCOL_INFOW	TCPChainInfo, UDPChainInfo;
	WSAPROTOCOL_INFOW	RAWChainInfo, IPLayeredInfo, ChainArray[3];
	BOOL				RawIP	= FALSE;
	BOOL				UdpIP	= FALSE;
	BOOL				TcpIP	= FALSE;
	INT					ProvCnt = 0;

	GetProviders();

	for (i = 0; i < TotalProtocols; i++)
	{
		if(ProtocolInfo[i].iAddressFamily == AF_INET)
		{
			if (!RawIP && ProtocolInfo[i].iProtocol == IPPROTO_IP)
				SetProtocolInfo(RawIP, RawOrigCatalogId
				, ProtocolInfo[i], IPLayeredInfo, &RAWChainInfo);

			if (!TcpIP && ProtocolInfo[i].iProtocol == IPPROTO_TCP)
				SetProtocolInfo(TcpIP, TcpOrigCatalogId
				, ProtocolInfo[i], TCPChainInfo, NULL);

			if (!UdpIP && ProtocolInfo[i].iProtocol == IPPROTO_UDP)
				SetProtocolInfo(UdpIP, UdpOrigCatalogId
				, ProtocolInfo[i], UDPChainInfo, NULL);
		}
	}

	wcscpy(IPLayeredInfo.szProtocol, L"Layered IP");
	IPLayeredInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;
	if (WSCInstallProvider(&ProviderGuid
		, m_sProviderPath, &IPLayeredInfo, 1, &ErrorCode) == SOCKET_ERROR)
	{
		printf("WSCInstallProvider failed %d\n", ErrorCode);
		return;
	}

	FreeProviders();
	GetProviders();

	for (i = 0; i < TotalProtocols; i++)
	{
		if (memcmp (&ProtocolInfo[i].ProviderId, &ProviderGuid, sizeof (GUID))==0)
		{
			LayeredCatalogId = ProtocolInfo[i].dwCatalogEntryId;
			break;
		}
	}

	if (TcpIP)
		SetProtocolChain(L"Layered TCP/IP over", TCPChainInfo
			, LayeredCatalogId, TcpOrigCatalogId, ChainArray[ProvCnt++]);
	if (UdpIP)
		SetProtocolChain(L"Layered UDP/IP over", UDPChainInfo
			, LayeredCatalogId, UdpOrigCatalogId, ChainArray[ProvCnt++]);
	if (RawIP)
		SetProtocolChain(L"Layered RAW/IP over", RAWChainInfo
			, LayeredCatalogId, RawOrigCatalogId, ChainArray[ProvCnt++]);

	if (WSCInstallProvider(&ProviderChainGuid
		, m_sProviderPath, ChainArray, ProvCnt, &ErrorCode) == SOCKET_ERROR)
	{
		printf("WSCInstallProvider for protocol chain failed %d\n", ErrorCode);
		return;
	}

	FreeProviders();
	GetProviders();

	if ((CatalogEntries = (LPDWORD) GlobalAlloc(
		GPTR, TotalProtocols * sizeof(DWORD))) == NULL)
	{
		printf("GlobalAlloc failed %d\n", GetLastError());
		return;
	}
	CatIndex = 0;
	for (i = 0; i < TotalProtocols; i++)
	{
		if (memcmp (&ProtocolInfo[i].ProviderId
				, &ProviderGuid, sizeof (GUID))==0 
			|| memcmp (&ProtocolInfo[i].ProviderId
				, &ProviderChainGuid, sizeof (GUID))==0)
		{
			CatalogEntries[CatIndex++] = ProtocolInfo[i].dwCatalogEntryId;
		}
	}
	for (i = 0; i < TotalProtocols; i++)
	{
		if (memcmp (&ProtocolInfo[i].ProviderId
				, &ProviderGuid, sizeof (GUID))!=0 
			&& memcmp (&ProtocolInfo[i].ProviderId
				, &ProviderChainGuid, sizeof (GUID))!=0)
		{
			CatalogEntries[CatIndex++] = ProtocolInfo[i].dwCatalogEntryId;
		}
	}
	if ((ErrorCode = WSCWriteProviderOrder(CatalogEntries, TotalProtocols)) 
		!= ERROR_SUCCESS)
	{
		printf("WSCWriteProviderOrder failed %d\n", ErrorCode);
		return;
	}

	FreeProviders();
}

void RemoveProvider(void)
{
	INT ErrorCode;
	if (WSCDeinstallProvider(&ProviderGuid, &ErrorCode) == SOCKET_ERROR)
		printf("WSCDeistallProvider for Layer failed %d\n", ErrorCode);
	if (WSCDeinstallProvider(&ProviderChainGuid, &ErrorCode) == SOCKET_ERROR)
		printf("WSCDeistallProvider for Chain failed %d\n", ErrorCode);
}
