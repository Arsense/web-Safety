//  从服务器端接收桌面屏幕
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "command.h"
#include "gdi.h"
#include "HuffCompress.h"
#include "RLE.h"
#include "time.h"

#define DEFAULT_PORT        5150
#define REFRESH_MSG			"REFRESH"
#define SETFLAGS			XP1_GUARANTEED_DELIVERY|XP1_GUARANTEED_ORDER
#define NOTSETFLAGS			XP1_CONNECTIONLESS
#define	LPBMIH				LPBITMAPINFOHEADER

UINT    port=DEFAULT_PORT;
BOOL	bSendOnly = FALSE;
BOOL	fSending = FALSE;
BOOL	fReceiving = FALSE;
DWORD	iLen;
char	*pDIBitmap;
int		iScreenWidth,iScreenHeight;
int		iWidth,iHeight,iWidthX,iHeightY;
int		iWidth1,iWidth2,iHeight1,iHeight2;
int		iCompressionLevel;
BOOL	fDIBitmap = FALSE;
LPBMIH	lpBitmapIH;
struct	sockaddr_in	server;
SOCKET	sClient;
BOOL	fRefreshing = FALSE;
BOOL	fConnected = FALSE;
int		nGridX = 8,nGridY = 4;
BOOL	fGridX = FALSE,fGridY = FALSE;
WORD	bmBitsPixel = 4;

struct	GdiList		GdiStart;
struct	GdiList		*pGdiNode;

int SelectProtocols(DWORD dwSetFlags,DWORD dwNotSetFlags,LPWSAPROTOCOL_INFO lpProtocolBuffer,LPDWORD lpdwBufferLength,WSAPROTOCOL_INFO *Protocol);

void InitGrids()
{
	int		iLoop,jLoop,nGrid;
	struct	GdiDS	Gdi;

	Gdi.fDIBitmap = FALSE;
	GdiStart.pNext = NULL;
	pGdiNode = &GdiStart;

	if ((nGridX > 0) && (nGridY > 0))
	{
		for (iLoop = 0;iLoop < nGridX;iLoop++)
		{
			for (jLoop = 0;jLoop < nGridY;jLoop++)
			{
				Gdi.iGridX = iLoop;
				Gdi.iGridY = jLoop;

				pGdiNode = Add_Gdi(pGdiNode,Gdi);
			}
		}
	}
	else
	{
		nGrid = max(nGridX,nGridY);
		for (iLoop = 0;iLoop < nGrid;iLoop++)
		{
			if (nGridX > 0)
				Gdi.iGridX = iLoop;
			else
				Gdi.iGridX = 0;

			if (nGridY > 0)
				Gdi.iGridY = iLoop;
			else
				Gdi.iGridY = 0;

			pGdiNode = Add_Gdi(pGdiNode,Gdi);
		}
	}

	if (nGridX > 0)
		iWidthX = iWidth/nGridX;
	else
		iWidthX = iWidth;

	if (nGridY > 0)
		iHeightY = iHeight/nGridY;
	else
		iHeightY = iHeight;
}

// 装入Winsock然后连接服务器
int LoadWinsock(HWND hWnd,char *szIP)
{
	WSADATA				wsd;
	DWORD				dwLen;
	int					nRet,nZero;
	LPBYTE				pBuf;
	WSAPROTOCOL_INFO	Protocol;
	char				szString[81];
	char				szMessage[81];

	if (WSAStartup(0x202,&wsd) != 0)
	{
		MessageBox(NULL,"hehe","Client Socket Error",MB_OK);
		return 1;
	}

	dwLen = 0;
	nRet = WSAEnumProtocols(NULL,NULL,&dwLen);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAENOBUFS)
			return 1;
	}

	pBuf = malloc(dwLen);

	nRet = SelectProtocols(SETFLAGS,NOTSETFLAGS,(LPWSAPROTOCOL_INFO)pBuf,&dwLen,&Protocol);

	free(pBuf);

	sClient = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_IP,NULL,0,SOCK_STREAM);
	if (sClient == INVALID_SOCKET)
		return 1;

	nZero = 0;
	setsockopt(sClient,SOL_SOCKET,SO_RCVBUF,(char *)&nZero,sizeof(nZero));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(szIP);

	if (connect(sClient,(struct sockaddr *)&server,sizeof(server)) == SOCKET_ERROR)
	{
		memset(szString,'\0',sizeof(szString));
		sprintf(szString,"Connect() failed: %d",WSAGetLastError());
		MessageBox(NULL,szString,"Client Socket Error",MB_OK);
		return 1;
	}

	GetResolution();
	SetGrids(hWnd);

	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"WM_COMP;%d;0;0;0;\0",iCompressionLevel);
	SendCommand(hWnd,1,szMessage);

	UpdateRegionalScreen(hWnd,sClient,TRUE);

	return 0;
}

void SetGrids(HWND hWnd)
{
	char	szMessage[81];
	Clear_Gdi(&GdiStart);
	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"WM_GRID;%d;%d;0;0;\0",nGridX,nGridY);
	SendCommand(hWnd,1,szMessage);
	InitGrids();
}

void GetResolution()
{
	int		iSend,iRecv;
	char	szMessage[81];
	char	szRecv[81];

	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"RESOLUTION\0");
	iSend = Send(sClient,szMessage);

	iRecv = Recv(sClient,szRecv,80);
	iWidth = atoi(szRecv);

	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"OK\0");
	iSend = Send(sClient,szMessage);

	iRecv = Recv(sClient,szRecv,80);
	iHeight = atoi(szRecv);

	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"OK\0");
	iSend = Send(sClient,szMessage);
}

int SendCommand(HWND hWnd,int iParm,char *szWMMessage)
{
	int		iSent;

	if (iParm == 0)
	{
		UpdateRegionalScreen(hWnd,sClient,FALSE);
	}
	else if (iParm == 1)
	{
		iSent = Send(sClient,szWMMessage);
	}
	return 0;
}

int Send(SOCKET Socket,char *szSend)
{
	int		iSent;
	fSending = TRUE;
	iSent = send(Socket,szSend,strlen(szSend),0);
	fSending = FALSE;
	return iSent;
}

int Recv(SOCKET Socket,char *szRecv,int iRecvLen)
{
	int		iRecv;
	fReceiving = TRUE;
	iRecv = recv(Socket,szRecv,iRecvLen,0);
	if (iRecv == SOCKET_ERROR || iRecv == 0)
		MessageBox(NULL,"NO DATA or SOCKET ERROR","Client",MB_OK);
	szRecv[iRecv] = '\0';
	fReceiving = FALSE;
	return iRecv;
}

void UpdateRegionalScreen(HWND hWnd,SOCKET sClient,BOOL fFirstTime)
{
	LPBMIH	lpBMIH;
	char	*pDIB;
	char	*pDIBCompress;
	char	*pStartDIB;
	DWORD	iSent,iRecv;
	DWORD	iCompLen,iNewLen,iRLELen,iStartPos;
	int		iGridX,iGridY;
	int		iUpdates,iLoop,iCompressions,jLoop;
	char	szMessage[81];

	struct	CommandList	CommandStart;
	struct	CommandList	*pCommandNode;
	struct	CommandDS	Command;
	char				*pDest;
	int					iLoc1,iLoc2,nChar;

	char				*pTempDIB;

	if (!fRefreshing)
	{
		CommandStart.pNext = NULL;
		fRefreshing = TRUE;
		strcpy(szMessage,REFRESH_MSG);
		iSent = Send(sClient,szMessage);
		memset(szMessage,'\0',sizeof(szMessage));
		iRecv = Recv(sClient,szMessage,80);
		if (iRecv == SOCKET_ERROR || iRecv == 0)
			MessageBox(NULL,"NO DATA or SOCKET ERROR","Client",MB_OK);
		iUpdates = atoi(szMessage);
		strcpy(szMessage,"OK\0");
		iSent = Send(sClient,szMessage);
		if (iUpdates > 0)
		{
			pGdiNode = GdiStart.pNext;
			for (iLoop = 0;iLoop < iUpdates;iLoop++)
			{
				pCommandNode = &CommandStart;
				memset(szMessage,'\0',sizeof(szMessage));
				iRecv = Recv(sClient,szMessage,80);
				if (iRecv == SOCKET_ERROR || iRecv == 0)
					MessageBox(NULL,"NO DATA or SOCKET ERROR","Client",MB_OK);
				iLoc1 = 0;
				while (pDest = strchr(szMessage,';'))
				{
					iLoc2 = pDest - szMessage;
					nChar = iLoc2 - iLoc1;
					memset(Command.szElement,'\0',sizeof(Command.szElement));
					strncpy(Command.szElement,&szMessage[iLoc1],nChar);
					pCommandNode = Add_Command(pCommandNode,Command);
					memset(&szMessage[iLoc2],':',1);
					iLoc1 = iLoc2 + 1;
				}

				pCommandNode = CommandStart.pNext;
				if (pCommandNode)
				{
					iCompressions = atoi(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iGridX = atoi(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;
					iGridY = atoi(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iStartPos = atol(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iLen = atol(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iCompLen = atol(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;
				}

				if (iLen > iCompLen)
					pDIBCompress = (char *)malloc(iLen);
				else
					pDIBCompress = (char *)malloc(iCompLen);

				while (pGdiNode)
				{
					if ((pGdiNode->Gdi.iGridX == iGridX) &&
						(pGdiNode->Gdi.iGridY == iGridY))
					{
						if (!pGdiNode->Gdi.fDIBitmap)
						{
							pGdiNode->Gdi.pDIBitmap = malloc(iLen);
							memset(pGdiNode->Gdi.pDIBitmap,0,iLen);

							pGdiNode->Gdi.lpBitmapIH = (LPBMIH)pGdiNode->Gdi.pDIBitmap;
							pGdiNode->Gdi.iWidth1 = iGridX * iWidthX;
							pGdiNode->Gdi.iWidth2 = iGridX * iWidthX + iWidthX;
							pGdiNode->Gdi.iHeight1 = iGridY * iHeightY;
							pGdiNode->Gdi.iHeight2 = iGridY * iHeightY + iHeightY;

							iScreenWidth = iWidth;
							iScreenHeight = iHeight;

							pGdiNode->Gdi.pDIB = (char *)malloc(iLen);
							pGdiNode->Gdi.pStartDIB = pGdiNode->Gdi.pDIB;
						}

						strcpy(szMessage,"OK\0");
						iSent = Send(sClient,szMessage);
						pStartDIB = pDIBCompress;
						Receive(sClient,pDIBCompress,iCompLen);
						pDIBCompress = pStartDIB;
						pStartDIB = pGdiNode->Gdi.pStartDIB;

						if (iCompressionLevel == 0)
						{
							memblast(pGdiNode->Gdi.pDIB,pDIBCompress,iLen);
							iNewLen = iLen;
						}
						else if (iCompressionLevel == 10)
						{
							iNewLen = HuffmanUnCompress((BYTE *)pDIBCompress,(BYTE *)pGdiNode->Gdi.pDIB);
						}
						else if (iCompressionLevel == 11)
						{
							for (jLoop = 0;jLoop < iCompressions;jLoop++)
							{
								iNewLen = HuffmanUnCompress((BYTE *)pDIBCompress,(BYTE *)pGdiNode->Gdi.pDIB);
								if ((jLoop + 1) < iCompressions)
									memblast(pDIBCompress,pGdiNode->Gdi.pDIB,iNewLen);
							}
						}
						else if (iCompressionLevel == 12) 
						{
							iNewLen = RunLengthDecode((BYTE *)pDIBCompress,(BYTE *)pGdiNode->Gdi.pDIB);
						}
						else if (iCompressionLevel == 13)
						{
							char	*pTempDIB;
							iLen = HuffmanGetSize((BYTE *)pDIBCompress);
							pTempDIB = (char *)malloc(iLen);
							iRLELen = HuffmanUnCompress((BYTE *)pDIBCompress,(BYTE *)pTempDIB);
							iNewLen = RunLengthDecode((BYTE *)pTempDIB,(BYTE *)pGdiNode->Gdi.pDIB);

							free(pTempDIB);
						}

						pGdiNode->Gdi.pDIB = pGdiNode->Gdi.pStartDIB;
						lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;

						if (fFirstTime)
						{
							pTempDIB = pGdiNode->Gdi.pDIBitmap;
							memblast(&pGdiNode->Gdi.pDIBitmap[iStartPos],pGdiNode->Gdi.pDIB,iNewLen);

							pGdiNode->Gdi.fDIBitmap = TRUE;
						}
						else
						{
							pTempDIB = pGdiNode->Gdi.pDIBitmap;
							pDIB = pGdiNode->Gdi.pDIB;

							__asm
							{
								MOV		ECX,iNewLen 
								SHR		ECX,2 
								MOV		EDI,pTempDIB
								ADD		EDI,iStartPos
								MOV		ESI,pDIB
								Addition:
								LODSD
								ADD		[EDI],EAX
								ADD		EDI,4
								DEC		ECX
								JNZ		Addition
							}
						}

						Clear_Command(&CommandStart);
						strcpy(szMessage,"OK\0");
						iSent = Send(sClient,szMessage);
						break;
					}
					pGdiNode = pGdiNode->pNext;
				}

				free(pDIBCompress);
			}

			if (fFirstTime)
				fDIBitmap = TRUE;
			InvalidateRect(hWnd,NULL,FALSE);
		}
		fRefreshing = FALSE;
	}
}

DWORD Receive(SOCKET Socket,char *pData,DWORD iLength)
{
	WSAOVERLAPPED	olRecv;
	WSAEVENT	gheventOlSock;
	WSAEVENT	eventArray[2];

	WSABUF		buffRecv;
	DWORD		dwRet,dwNumBytes,dwFlags;
	int			nWSAError;
	DWORD		iNumBytes = 0;

	char		szError[81];

	gheventOlSock = WSACreateEvent();
	eventArray[0] = gheventOlSock;

	ZeroMemory(&olRecv,sizeof(WSAOVERLAPPED));
	olRecv.hEvent= gheventOlSock;
	buffRecv.len = iLength;
	buffRecv.buf = pData;

	fReceiving = TRUE;

	while (TRUE)
	{
		dwFlags = 0;
		dwNumBytes = 0;

		if ((dwRet = WSARecv(Socket,&buffRecv,1,&dwNumBytes,&dwFlags,&olRecv,NULL)) == SOCKET_ERROR) 
		{
			nWSAError = WSAGetLastError();
			if (nWSAError != ERROR_IO_PENDING)
			{
				sprintf(szError,"WSARecv failed with error %d\n",nWSAError);
				MessageBox(NULL,szError,"Client",MB_OK);
				return SOCKET_ERROR;
			}
		}

		if (WSAWaitForMultipleEvents(1,eventArray,FALSE,WSA_INFINITE,FALSE) == WSA_WAIT_FAILED) 
		{
			sprintf(szError,"WSAWaitForMultipleEvents failed %d\n", WSAGetLastError());
			MessageBox(NULL,szError,"Client",MB_OK);
		} 
		WSAResetEvent(eventArray[0]);
		if (WSAGetOverlappedResult(Socket,&olRecv,&dwNumBytes,FALSE,&dwFlags) == TRUE)
		{
			if (dwNumBytes == 0)
				break;
		} 
		else 
		{
			sprintf(szError,"WSAGetOverlappedResult failed with error %d\n", WSAGetLastError());
			MessageBox(NULL,szError,"Client",MB_OK);
			return SOCKET_ERROR;
		}
		buffRecv.len -= dwNumBytes;
		iNumBytes = iNumBytes + dwNumBytes;
		if (buffRecv.len == 0) 
			break;
		else 
			buffRecv.buf += dwNumBytes;
	}
	fReceiving = FALSE;
	return iNumBytes;
}

int SelectProtocols(DWORD dwSetFlags,DWORD dwNotSetFlags,LPWSAPROTOCOL_INFO lpProtocolBuffer,LPDWORD lpdwBufferLength,WSAPROTOCOL_INFO *pProtocol)
{
	LPBYTE				pBuf;
	LPWSAPROTOCOL_INFO	pInfo;
	DWORD				dwNeededLen;
	LPWSAPROTOCOL_INFO	pRetInfo;
	DWORD				dwRetLen;
	int					nCount;
	int					nMatchCount;
	int					nRet;

	dwNeededLen = 0;
	nRet = WSAEnumProtocols(NULL, NULL, &dwNeededLen);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAENOBUFS)
			return SOCKET_ERROR;
	}

	pBuf = malloc(dwNeededLen);
	if (pBuf == NULL)
	{
		WSASetLastError(WSAENOBUFS);
		return SOCKET_ERROR;
	}

	nRet = WSAEnumProtocols(NULL,(LPWSAPROTOCOL_INFO)pBuf,&dwNeededLen);
	if (nRet == SOCKET_ERROR)
	{
		free(pBuf);
		return SOCKET_ERROR;
	}
	#define REJECTSET(f) \
	    ((dwSetFlags & f) && !(pInfo->dwServiceFlags1 & f))
	#define REJECTNOTSET(f) \
	    ((dwNotSetFlags &f) && (pInfo->dwServiceFlags1 & f))
	#define REJECTEDBY(f) (REJECTSET(f) || REJECTNOTSET(f))

	pInfo = (LPWSAPROTOCOL_INFO)pBuf;	
	pRetInfo = lpProtocolBuffer;
	dwRetLen = 0;
	nMatchCount = 0;
	for(nCount = 0; nCount < nRet; nCount++)
	{
		while(1)
		{
			if (REJECTEDBY(XP1_CONNECTIONLESS))
				break;
			if (REJECTEDBY(XP1_GUARANTEED_DELIVERY))
				break;
			if (REJECTEDBY(XP1_GUARANTEED_ORDER))
				break;
			if (REJECTEDBY(XP1_MESSAGE_ORIENTED))
				break;
			if (REJECTEDBY(XP1_PSEUDO_STREAM))
				break;
			if (REJECTEDBY(XP1_GRACEFUL_CLOSE))
				break;
			if (REJECTEDBY(XP1_EXPEDITED_DATA))
				break;
			if (REJECTEDBY(XP1_CONNECT_DATA))
				break;
			if (REJECTEDBY(XP1_DISCONNECT_DATA))
				break;
			if (REJECTEDBY(XP1_SUPPORT_BROADCAST)) 
				break;
			if (REJECTEDBY(XP1_SUPPORT_MULTIPOINT))
				break;
			if (REJECTEDBY(XP1_MULTIPOINT_DATA_PLANE))
				break;
			if (REJECTEDBY(XP1_QOS_SUPPORTED))
				break;
			if (REJECTEDBY(XP1_UNI_SEND))
				break;
			if (REJECTEDBY(XP1_UNI_RECV))
				break;
			if (REJECTEDBY(XP1_IFS_HANDLES))
				break;
			if (REJECTEDBY(XP1_PARTIAL_MESSAGE))
				break;

			dwRetLen += sizeof(WSAPROTOCOL_INFO);
			if (dwRetLen > *lpdwBufferLength)
			{
				WSASetLastError(WSAENOBUFS);
				*lpdwBufferLength = dwNeededLen;
				free(pBuf);
				return SOCKET_ERROR;
			}
			nMatchCount++;
			memblast(pRetInfo,pInfo,sizeof(WSAPROTOCOL_INFO));
			if (strcmp(pInfo->szProtocol,"MSAFD Tcpip [TCP/IP]") == 0)
				memblast(pProtocol,pInfo, sizeof(WSAPROTOCOL_INFO));

			pRetInfo++;
			break;
		}
		pInfo++;
	}
	free(pBuf);
	*lpdwBufferLength = dwRetLen;
	return(nMatchCount);
}

void memblast(void* dest,void* src,DWORD count)
{
	DWORD	iCount;

	__asm
	{
		MOV		ECX,count
		SHR		ECX,2
		SHL		ECX,2
		MOV		iCount,ECX
		MOV		ESI,src	
		MOV		EDI,dest
		MOV		ECX,iCount
		SHR		ECX,2	
		REP		MOVSD	

		MOV		ECX,count
		MOV		EAX,iCount
		SUB		ECX,EAX

		JZ		Exit
		MOV		ESI,src	
		ADD		ESI,EAX	
		MOV		EDI,dest
		ADD		EDI,EAX
		REP		MOVSB	
		Exit:
	}
}
