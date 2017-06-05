/*============================================================================

	文件：		TcpIpDog.cpp

	说明:
	---------------------------------------------------
		最小化的 Winsock 2 SPI 传输服务提供者
	---------------------------------------------------

	工程：		MinLSP
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/21
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
//——————————————————————————————————————
// 使用Unicode编码
//
#define UNICODE
#define _UNICODE

#include <afxwin.h>
#include <ws2spi.h>
#include <io.h>				
#include <sporder.h>
#include <TCHAR.H>

//
// 全局变量，用来保存系统服务提供者30个服务函数指针
//
WSPPROC_TABLE		NextProcTable   ;

//
// 全局变量，用来临时保存全部服务提供者的信息
//
LPWSAPROTOCOL_INFOW ProtocolInfo		= NULL;
DWORD				ProtocolInfoSize	= 0;
INT					TotalProtocols		= 0;

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

BOOL GetHookProvider(
	IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
	OUT	TCHAR				*sPathName,
	OUT WSAPROTOCOL_INFOW	*pNextProtocolInfo
)
{
	if(pProtocolInfo->ProtocolChain.ChainLen <= 1)
		return FALSE;
	GetProviders();
	__try
	{
		for(int i = pProtocolInfo->ProtocolChain.ChainLen - 1; i > 0; i--)
		{
			for(int j = 0; j < TotalProtocols; j++)
			{
				if(pProtocolInfo->ProtocolChain.ChainEntries[i]	
					== ProtocolInfo[j].dwCatalogEntryId)
				{
					INT iErrno, iProviderPathLen = MAX_PATH;
					if(WSCGetProviderPath(&ProtocolInfo[j].ProviderId
						, sPathName, &iProviderPathLen, &iErrno) == SOCKET_ERROR)
						return FALSE;
					if (!ExpandEnvironmentStrings(sPathName, sPathName, MAX_PATH))
						return FALSE;
					memcpy(pNextProtocolInfo
						, pProtocolInfo, sizeof(WSAPROTOCOL_INFOW));
					return TRUE;
				}
			}
		}
	}
	__finally
	{
		FreeProviders();
	}

	return FALSE;
}

SOCKET WSPAPI WSPSocket(
	int			af,                               
	int			type,                             
	int			protocol,                         
	LPWSAPROTOCOL_INFOW lpProtocolInfo,   
	GROUP		g,                              
	DWORD		dwFlags,                        
	LPINT		lpErrno
)
{
	OutputDebugString(_T("MinLSP.DLL: WSPSocket ...\n"));

	return NextProcTable.lpWSPSocket(
		af, type, protocol, lpProtocolInfo, g, dwFlags, lpErrno);
}

BOOL WINAPI DllMain(
	HINSTANCE	hModule, 
    DWORD		ul_reason_for_call, 
    LPVOID		lpReserved
)
{
	return TRUE;
}

int WSPAPI WSPStartup(
	WORD				wVersionRequested,
	LPWSPDATA			lpWSPData,
	LPWSAPROTOCOL_INFOW	lpProtocolInfo,
	WSPUPCALLTABLE		upcallTable,
	LPWSPPROC_TABLE		lpProcTable
)
{
	OutputDebugString(_T("MinLSP.DLL: WSPStartup...\n"));

	TCHAR				sLibraryPath[MAX_PATH];
    LPWSPSTARTUP        WSPStartupFunc      = NULL;
	HMODULE				hLibraryHandle		= NULL;
    INT                 ErrorCode           = 0; 
	WSAPROTOCOL_INFOW	NextProtocolInfo;

	if (!GetHookProvider(lpProtocolInfo, sLibraryPath, &NextProtocolInfo)
		|| (hLibraryHandle = LoadLibrary(sLibraryPath)) == NULL
		|| (WSPStartupFunc = (LPWSPSTARTUP)GetProcAddress(
								hLibraryHandle, "WSPStartup")) == NULL
		)
		return WSAEPROVIDERFAILEDINIT;

	if ((ErrorCode = WSPStartupFunc(wVersionRequested, lpWSPData
		, &NextProtocolInfo, upcallTable, lpProcTable)) != ERROR_SUCCESS)
		return ErrorCode;
	
	NextProcTable = *lpProcTable;

	lpProcTable->lpWSPSocket = WSPSocket;

	return 0;
}

