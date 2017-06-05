
#include <winsock2.h>
#include <string.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")


DWORD WINAPI ProcessTreadIO(LPVOID lpParam);




typedef struct {
	CHAR   buffRecv[4096];
	CHAR   buffSend[4096];
	WSABUF wsaBuf;
	SOCKET s;
	WSAOVERLAPPED o;
	DWORD dwBytesSend;
	DWORD dwBytesRecv;
	int   nStatus;
} SOCKET_INF, *LPSOCKET_INF;

DWORD g_dwEventTotal = 0;
DWORD g_index;
WSAEVENT g_events[WSA_MAXIMUM_WAIT_EVENTS];
LPSOCKET_INF g_sockets[WSA_MAXIMUM_WAIT_EVENTS];


char  g_szLocalAddr[128]; 
BOOL  g_bLoggedIn;



void main(void)
{
	WSADATA wsaData;
	SOCKET sListen, sAccept;
	SOCKADDR_IN inetAddr;
	DWORD dwFlags;
	DWORD dwThreadId;
	DWORD dwRecvBytes;
	INT   nRet;
	
	
	//初始化套接字库
	if (( nRet = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0 ) {
		printf("WSAStartup failed with error %d\n", nRet);
		return;
	}
	
	
	
	if ((sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) 
	{
		printf("Failed to get a socket\n");
		WSACleanup();
		return;
	}
	
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inetAddr.sin_port = htons(2356);
	
	if (bind(sListen, (PSOCKADDR) &inetAddr, sizeof(inetAddr)) == SOCKET_ERROR)
	{
		printf("bind() failed with error\n");
		return;
	}
	
	if (listen(sListen, SOMAXCONN))
	{
		printf("listen() failed with error\n");
		return;
	}
	
	printf("Mini Ftpserver已经启动 \n");
	printf("Mini Ftpserver开始侦听 \n");
	
	if ((sAccept = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) 
	{
		printf("Failed to get a socket\n");
		return;
	}
	

	// 创建一个线程处理请求
	if (CreateThread(NULL, 0, ProcessTreadIO, NULL, 0, &dwThreadId) == NULL)
	{
		printf("CreateThread failed with error\n");
		return;
	} 
	
	g_dwEventTotal = 0;



	while(TRUE)
	{
		//处理入站连接
		if ((sAccept = accept(sListen, NULL, NULL)) == INVALID_SOCKET)
		{
			printf("accept failed with error\n");
			return;
		}
		
		g_dwEventTotal++;

		
		
		//创建一个新的SOCKET_INF结构处理接受的数据socket.
		if ((g_sockets[g_dwEventTotal] = (LPSOCKET_INF) 
			GlobalAlloc(GPTR,sizeof(SOCKET_INF))) == NULL)
		{
			printf("GlobalAlloc() failed with error\n");
			return;
		}


		char buff[4096]; 
		memset( buff,0,4096 );
		g_sockets[g_dwEventTotal]->wsaBuf.buf = buff;  
		g_sockets[g_dwEventTotal]->wsaBuf.len = 4096;
		g_sockets[g_dwEventTotal]->s = sAccept;
		memset(&(g_sockets[g_dwEventTotal]->o),0, sizeof(OVERLAPPED));
		g_sockets[g_dwEventTotal]->dwBytesSend = 0;
		g_sockets[g_dwEventTotal]->dwBytesRecv = 0;
	    g_sockets[g_dwEventTotal]->nStatus     = 1;    // 接收
		

		
		//创建事件
		if ((g_sockets[g_dwEventTotal]->o.hEvent = g_events[g_dwEventTotal] = 
			WSACreateEvent()) == WSA_INVALID_EVENT)
		{
			printf("WSACreateEvent() failed with error\n");
			return;
		}
		
		//发出接受请求
		dwFlags = 0;
		if (WSARecv(g_sockets[g_dwEventTotal]->s, 
			&(g_sockets[g_dwEventTotal]->wsaBuf), 1, &dwRecvBytes, &dwFlags,
			&(g_sockets[g_dwEventTotal]->o), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error\n");
				return;
			}
		}
	
		
		
	
		
   }
}



DWORD WINAPI ProcessTreadIO(LPVOID lpParam)
{
	DWORD dwFlags;
	LPSOCKET_INF pSI;
	DWORD dwBytesTransferred;
	DWORD i;  
	
	//处理异步的WSASend, WSARecv等请求等
	while(TRUE)
	{
		if ((g_index = WSAWaitForMultipleEvents(g_dwEventTotal, g_events, FALSE,
			WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
		{
			
			
			if (g_index==-1)
			{
				continue;
			}
			
			printf("WSAWaitForMultipleEvents failed\n");
			return 0;
		
		
		
		}


	}

	return 0;
}