// Mini FtpServer.cpp : Defines the entry point for the console application.
#include <winsock2.h>
#include <string.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")



#define WSA_RECV         0
#define WSA_SEND         1
#define DATA_BUFSIZE    8192
#define MAX_NAME_LEN    128
#define MAX_PWD_LEN     128
#define MAX_RESP_LEN    1024
#define MAX_REQ_LEN     256
#define MAX_ADDR_LEN    80
#define FTP_PORT        21     // FTP 控制端口   
#define DATA_FTP_PORT   20     // FTP 数据端口
#define USER_OK         331
#define LOGGED_IN       230
#define LOGIN_FAILED    530
#define CMD_OK          200
#define OPENING_AMODE   150
#define TRANS_COMPLETE  226
#define CANNOT_FIND     550
#define FTP_QUIT        221
#define CURR_DIR        257
#define DIR_CHANGED     250
#define OS_TYPE         215
#define REPLY_MARKER    504
#define PASSIVE_MODE    227
#define FTP_USER		"shine"
#define FTP_PASS		"shine"
#define DEFAULT_HOME_DIR    "D:\\TEMP"

#define MAX_FILE_NUM        1024

#define MODE_PORT       0
#define MODE_PASV       1

#define PORT_BIND   1821

typedef struct {
   CHAR   buffRecv[DATA_BUFSIZE];
   CHAR   buffSend[DATA_BUFSIZE];
   WSABUF wsaBuf;
   SOCKET s;
   WSAOVERLAPPED o;
   DWORD dwBytesSend;
   DWORD dwBytesRecv;
   int   nStatus;
} SOCKET_INF, *LPSOCKET_INF;

typedef struct {
	TCHAR    szFileName[MAX_PATH];
	DWORD    dwFileAttributes; 
    FILETIME ftCreationTime; 
    FILETIME ftLastAccessTime; 
    FILETIME ftLastWriteTime; 
    DWORD    nFileSizeHigh; 
    DWORD    nFileSizeLow; 
} FILE_INF, *LPFILE_INF;

DWORD WINAPI ProcessTreadIO( LPVOID lpParam ) ;
BOOL WelcomeInfo( SOCKET s );
int LoginIn( LPSOCKET_INF pSocketInfo  );
int SendRes( LPSOCKET_INF pSI );
int RecvReq( LPSOCKET_INF pSI );
int DealCommand( LPSOCKET_INF pSI );

int GetFileList( LPFILE_INF pFI, UINT nArraySize, const char* szPath  );
char* GetLocalAddress();

DWORD g_dwEventTotal = 0;
DWORD g_index;
WSAEVENT g_events[WSA_MAXIMUM_WAIT_EVENTS];
LPSOCKET_INF g_sockets[WSA_MAXIMUM_WAIT_EVENTS];
 
char  g_szLocalAddr[MAX_ADDR_LEN]; 
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
  /* if ((sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 
      WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) 
*/
   if ((sListen = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
   {
      printf("Failed to get a socket\n");
	  WSACleanup();
      return;
   }

   inetAddr.sin_family = AF_INET;
   inetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   inetAddr.sin_port = htons(FTP_PORT);
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
   //创建第一个手动重置对象 
   if ((g_events[0] = WSACreateEvent()) == WSA_INVALID_EVENT)
   {
      printf("WSACreateEvent failed with error\n");
      return;
   }

   // 创建一个线程处理请求
   if (CreateThread(NULL, 0, ProcessTreadIO, NULL, 0, &dwThreadId) == NULL)
   {
      printf("CreateThread failed with error\n");
      return;
   } 
   g_dwEventTotal = 1;
   while(TRUE)
   {
       //处理入站连接
      if ((sAccept = accept(sListen, NULL, NULL)) == INVALID_SOCKET)
      {
          printf("accept failed with error\n");
          return;
      }

      //回传欢迎消息让waitforsingle受信
	  if( !WelcomeInfo(sAccept)) 
	  {
		  break;
	  }
      //设置ftp根目录
	  if(!SetCurrentDirectory(DEFAULT_HOME_DIR))
	  {
		  break;
	  }


	  //创建一个新的SOCKET_INF结构处理接受的数据socket.
      if ((g_sockets[g_dwEventTotal] = (LPSOCKET_INF) 
		  GlobalAlloc(GPTR,sizeof(SOCKET_INF))) == NULL)
      {
		  printf("GlobalAlloc() failed with error\n");
		  return;
      }
	  

      //初始化新的SOCKET_INF结构
	  char buff[DATA_BUFSIZE]; 
	  memset( buff,0,DATA_BUFSIZE );
	  g_sockets[g_dwEventTotal]->wsaBuf.buf = buff;  
	  g_sockets[g_dwEventTotal]->wsaBuf.len = DATA_BUFSIZE;
      g_sockets[g_dwEventTotal]->s = sAccept;
      memset(&(g_sockets[g_dwEventTotal]->o),0, sizeof(OVERLAPPED));
      g_sockets[g_dwEventTotal]->dwBytesSend = 0;
      g_sockets[g_dwEventTotal]->dwBytesRecv = 0;
	  g_sockets[g_dwEventTotal]->nStatus     = WSA_RECV;    // 接收
   
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



		  printf("%d\r\n",WSAGetLastError());
         if (WSAGetLastError() != ERROR_IO_PENDING)
         {
            printf("WSARecv() failed with error\n");
            return;
         }
      }
      g_dwEventTotal++;



	  //使第一个事件有信号。使工作者线程处理其他的事件
	  if (WSASetEvent(g_events[0]) == FALSE)
      {
		  printf("WSASetEvent failed with error\n");
		  return;
      }
   }
}

//工作者线程处理函数
DWORD WINAPI ProcessTreadIO(LPVOID lpParameter)
{
   DWORD dwFlags;
   LPSOCKET_INF pSI;   DWORD dwBytesTransferred;
   DWORD i;  

   //处理异步的WSASend, WSARecv等请求等
   while(TRUE)
   {
      if ((g_index = WSAWaitForMultipleEvents(g_dwEventTotal, g_events, FALSE,
                                      WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
      {
         printf("WSAWaitForMultipleEvents failed\n");
         return 0;
      }
      
      if ((g_index - WSA_WAIT_EVENT_0) == 0)
      {
         WSAResetEvent(g_events[0]);
         continue;
      }

      pSI = g_sockets[g_index - WSA_WAIT_EVENT_0];
      WSAResetEvent(g_events[g_index - WSA_WAIT_EVENT_0]);

      if (WSAGetOverlappedResult(pSI->s, &(pSI->o), &dwBytesTransferred,
                    FALSE, &dwFlags) == FALSE || dwBytesTransferred == 0)
      {
         printf("Closing socket %d\n", pSI->s);

         if (closesocket(pSI->s) == SOCKET_ERROR)
         {
            printf("closesocket() failed with error\n");
         }

         GlobalFree(pSI);
         WSACloseEvent(g_events[g_index - WSA_WAIT_EVENT_0]);

        
          if ((g_index - WSA_WAIT_EVENT_0) + 1 != g_dwEventTotal)
            for (i = g_index - WSA_WAIT_EVENT_0; i < g_dwEventTotal; i++) 
			{
               g_events[i] = g_events[i + 1];
			   g_sockets[i] = g_sockets[i + 1];
            }

         g_dwEventTotal--;

         continue;
      }

	  // 已经有数据传递
	  if( pSI->nStatus == WSA_RECV )
	  {
		  memcpy( &pSI->buffRecv[pSI->dwBytesRecv],pSI->wsaBuf.buf,dwBytesTransferred);
		  pSI->dwBytesRecv += dwBytesTransferred;
		  printf( "接受:%s\n",pSI->buffRecv);
		  if( pSI->buffRecv[pSI->dwBytesRecv-2] == '\r'      // 要保证最后是\r\n
				&& pSI->buffRecv[pSI->dwBytesRecv-1] == '\n' 
				&& pSI->dwBytesRecv > 2 )  
		  {                 
			 if( !g_bLoggedIn )
			 {
				if( LoginIn(pSI) == LOGGED_IN )
					g_bLoggedIn = TRUE;
			 } 
			 else 
			 {
				  if(DealCommand( pSI )==FTP_QUIT)
				  continue;
			 }
			 // 缓冲区清除
			 memset( pSI->buffRecv,0,sizeof(pSI->buffRecv) );
			 pSI->dwBytesRecv = 0;
		  }
	  } 
	  else
	  {
		  pSI->dwBytesSend += dwBytesTransferred;
	  }
	  
 	  // 继续接收以后到来的数据
	  if( RecvReq( pSI ) == -1 ) 
		  return -1; 
   }
   return 0;
}

// 由于只是简单的出现一个登录信息，直接用send就可以了
int SendRes( LPSOCKET_INF pSI )
{
	static DWORD dwSendBytes = 0;
	pSI->nStatus = WSA_SEND;
    memset(&(pSI->o), 0,sizeof(WSAOVERLAPPED));
    pSI->o.hEvent = g_events[g_index - WSA_WAIT_EVENT_0];
    pSI->wsaBuf.buf = pSI->buffSend + pSI->dwBytesSend;
    pSI->wsaBuf.len = strlen( pSI->buffSend ) - pSI->dwBytesSend;
    if (WSASend(pSI->s, &(pSI->wsaBuf), 1,&dwSendBytes,
		        0,&(pSI->o), NULL) == SOCKET_ERROR) 
	{
        if (WSAGetLastError() != ERROR_IO_PENDING) 
		{
			printf("WSASend() failed with error\n");
			return -1;
        }
    }
	return 0;
}
//接受数据
int RecvReq( LPSOCKET_INF pSI )
{
	static DWORD dwRecvBytes = 0;	
	pSI->nStatus = WSA_RECV;	

	DWORD dwFlags = 0;
	memset(&(pSI->o), 0,sizeof(WSAOVERLAPPED));
	pSI->o.hEvent = g_events[g_index - WSA_WAIT_EVENT_0];
	pSI->wsaBuf.len = DATA_BUFSIZE;

	if (WSARecv(pSI->s, &(pSI->wsaBuf), 1, &dwRecvBytes,
                &dwFlags,&(pSI->o), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
		   printf("WSARecv() failed with error\n");
		   return -1;
		}
	}
	return 0;
}

//显示欢迎消息
BOOL WelcomeInfo(SOCKET s)
{
	char* szWelcomeInfo = "220 Hello FtpServer...\r\n";
	if(send(s,szWelcomeInfo,strlen(szWelcomeInfo),0 ) == SOCKET_ERROR ) 
	{
		printf("Ftp client error:%d\n", WSAGetLastError() );
		return FALSE;
	}
	// 刚进来，还没连接，故设置初始状态为false
	g_bLoggedIn = FALSE;
	return TRUE;
}
//登录函数
int LoginIn( LPSOCKET_INF pSocketInfo  )
{
	const char* szUserOK = "331 User name okay, need password.\r\n"; 
	const char* szLoggedIn = "230 User logged in, proceed.\r\n";

	int  nRetVal = 0;
    static char szUser[MAX_NAME_LEN];
	static char szPwd[MAX_PWD_LEN];
	LPSOCKET_INF pSI = pSocketInfo;
	// 取得登录用户名
	if(strstr(strupr(pSI->buffRecv),"USER")) 
	{		
		sprintf(szUser,"%s",pSI->buffRecv+strlen("USER")+1);   //USER Hello\r\n
		strtok( szUser,"\r\n");		
		// 响应信息
		sprintf(pSI->buffSend,"%s",szUserOK );
		if( SendRes(pSI) == -1 ) 
		{
			return -1;
		}
		return USER_OK;
	}
	if(strstr(strupr(pSI->buffRecv),"PASS") || strstr(pSI->buffRecv,"pass")) 
	{
		sprintf(szPwd,"%s",pSI->buffRecv+strlen("PASS")+1 );
		
		strtok( szPwd,"\r\n");
		// 判断用户名跟口令正确性
		if( stricmp( szPwd,FTP_PASS) || stricmp(szUser,FTP_USER) ) 
		{
			sprintf(pSI->buffSend,"530 User %s cannot log in.\r\n",szUser );
			printf("User %s cannot log in\n",szUser );
			nRetVal = LOGIN_FAILED;
		} 
		else 
		{
			sprintf(pSI->buffSend,"%s",szLoggedIn);
			printf("User %s logged in\n",szUser );
			nRetVal = LOGGED_IN;
		}
		if( SendRes( pSI ) == -1 ) 
			return -1;
	}
	return nRetVal;
}

char* ConvertCommaAddress( char* szAddress, WORD wPort )
{
	char szPort[10];
	sprintf( szPort,"%d,%d",wPort/256,wPort%256 );
	char szIpAddr[20];
	sprintf( szIpAddr,"%s,",szAddress );
	int idx = 0;
	while( szIpAddr[idx] ) 
	{
		if( szIpAddr[idx] == '.' )
		{
			szIpAddr[idx] = ',';
		}
			
		idx ++;
	}
	sprintf( szAddress,"%s%s",szIpAddr,szPort );
	
	return szAddress;
}

int ConvertDotAddress( char* szAddress, LPDWORD pdwIpAddr, LPWORD pwPort ) 
{
	int  idx = 0,i = 0, iCount = 0;
	char szIpAddr[MAX_ADDR_LEN]; 
	memset( szIpAddr,0,sizeof(szIpAddr));
	
	char szPort[MAX_ADDR_LEN];   
	memset( szPort,0,  sizeof(szPort));

	*pdwIpAddr = 0; *pwPort = 0;

	//192.168.1.106,78787
	while( szAddress[idx]  )
	{
		if( szAddress[idx] == ',')
		{
			iCount ++;
			szAddress[idx] ='.';
		}
		if( iCount < 4 )
			szIpAddr[idx] = szAddress[idx];
		else
			szPort[i++] =   szAddress[idx];
		idx++;
	}

	if( iCount != 5 ) 
	{
		return -1;
	}
	*pdwIpAddr = inet_addr( szIpAddr );

	if( *pdwIpAddr  == INADDR_NONE ) 
	{
		return -1;
	}
	char *pToken = strtok( szPort+1,"." );
	if( pToken == NULL )
	{
		return -1;
	}
	*pwPort = (WORD)(atoi(pToken) * 256);
	pToken = strtok(NULL,".");
	
	if( pToken == NULL ) 
	{
		return -1;
	}
	*pwPort += (WORD)atoi(pToken);


		
	return 0;
}

DWORD ReadFileToBuffer( const char* szFile, char *buff, DWORD nFileSize )
{
	DWORD  idx = 0;
	DWORD  dwBytesLeft = nFileSize;
	DWORD  dwNumOfBytesRead = 0;
	char lpFileName[MAX_PATH];
	GetCurrentDirectory( MAX_PATH,lpFileName );
	
	strcat( lpFileName,"\\" );
	strcat(lpFileName,szFile );
	
	HANDLE hFile = CreateFile( lpFileName,
							   GENERIC_READ,
							   FILE_SHARE_READ,
							   NULL,
							   OPEN_EXISTING,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		while( dwBytesLeft > 0 ) 
		{
			if( !ReadFile( hFile,&buff[idx],dwBytesLeft,&dwNumOfBytesRead,NULL ) )
			{
				printf("读文件出错.\n");
				CloseHandle( hFile );
				return 0;
			}
			idx += dwNumOfBytesRead;
			dwBytesLeft -= dwNumOfBytesRead;
		}
		CloseHandle( hFile );
	}
	return idx;
}

DWORD WriteToFile( SOCKET s , const char* szFile )
{
	DWORD  idx = 0;
	DWORD  dwNumOfBytesWritten = 0;
	DWORD  nBytesLeft = DATA_BUFSIZE;
	char   buf[DATA_BUFSIZE];
	char   lpFileName[MAX_PATH];
	GetCurrentDirectory( MAX_PATH,lpFileName );
	strcat( lpFileName,"\\" );
	strcat(lpFileName,szFile );
	HANDLE hFile = CreateFile( lpFileName,
							   GENERIC_WRITE,
							   FILE_SHARE_WRITE,
							   NULL,
							   OPEN_ALWAYS,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL );
	if( hFile == INVALID_HANDLE_VALUE ) 
	{ 
		printf("打开文件出错.\n");
		return 0;
	}
	
	while( TRUE )
	{
		int nBytesRecv = 0;
		idx = 0; nBytesLeft = DATA_BUFSIZE;
		while( nBytesLeft > 0 )
		{
			nBytesRecv = recv( s,&buf[idx],nBytesLeft,0 );
			if( nBytesRecv == SOCKET_ERROR ) 
			{
				printf("Failed to send buffer to socket %d\n",WSAGetLastError() );
				return -1;
			}
			if( nBytesRecv == 0 )
			{
				 break;
			}
		
			idx += nBytesRecv;
			nBytesLeft -= nBytesRecv;
		}
		nBytesLeft = idx;   // 要写入文件中的字节数
		idx = 0;			// 索引清0,指向开始位置
		while( nBytesLeft > 0 ) 
		{
			// 移动文件指针到文件末尾
			if( !SetEndOfFile(hFile) ) return 0;
			if( !WriteFile( hFile,&buf[idx],nBytesLeft,&dwNumOfBytesWritten,NULL ) ) 
			{
				printf("写文件出错.\n");
				CloseHandle( hFile );
				return 0;
			}
			idx += dwNumOfBytesWritten;
			nBytesLeft -= dwNumOfBytesWritten;
		}
		// 如果没有数据可接收，退出循环
		if( nBytesRecv == 0 )
		{
			 break;
		}
	}

	CloseHandle( hFile );
	return idx;
}
int CombindFileNameSize( const char* szFileName,char* szFileNS )
{
	// 假定文件的大小不超过4GB,只处理低位
	int nFileSize = -1;
	FILE_INF fi[1];
	int nFiles = GetFileList( fi,1,szFileName );
	if( nFiles != 1 ) 
	{
		return -1;
	}
	
	sprintf( szFileNS, "%s<%d bytes>",szFileName,fi[0].nFileSizeLow );
	nFileSize = fi[0].nFileSizeLow;
	return nFileSize;
}

int	DataConn( SOCKET& s, DWORD dwIp, WORD wPort, int nMode ) 
{
	// 创建一个socket
	s = socket( AF_INET,SOCK_STREAM,0 );
	if( s == INVALID_SOCKET )
	{
		 printf("Failed to get a socket %d\n", WSAGetLastError());
		 return -1;
	}

	struct sockaddr_in inetAddr;
	inetAddr.sin_family = AF_INET;
	if( nMode == MODE_PASV )
	{
		 inetAddr.sin_port = htons( wPort );
		 inetAddr.sin_addr.s_addr = dwIp;
	}
	else
	{ 
		inetAddr.sin_port = htons( DATA_FTP_PORT );
		inetAddr.sin_addr.s_addr = inet_addr(GetLocalAddress());
	}

	BOOL optval = TRUE;
	if( setsockopt(s,SOL_SOCKET,SO_REUSEADDR,
				(char*)&optval,sizeof(optval) ) == SOCKET_ERROR ) 
	{
		printf("Failed to setsockopt %d.\n",WSAGetLastError() );
		closesocket(s);
		return -1;
	}

	if( bind( s,(struct sockaddr*)&inetAddr,sizeof(inetAddr)) == SOCKET_ERROR )
	{
		printf("Failed to bind a socket %d.\n",WSAGetLastError() );
		closesocket(s);
		return -1;
	}

	if( MODE_PASV == nMode )
	{
		if( listen( s,SOMAXCONN ) == SOCKET_ERROR ) 
		{
			printf("Failed to listen a socket %d.\n",WSAGetLastError() );
			closesocket(s);
			return -1;
		}
	} 
	else if( MODE_PORT == nMode ) 
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port   = htons( wPort );
		addr.sin_addr.s_addr   = dwIp;
		if( connect( s, (const sockaddr*)&addr,sizeof(addr) ) == SOCKET_ERROR ) 
		{
			printf("Failed to connect a socket %d\n",WSAGetLastError() );
			closesocket( s );
			return -1;
		}
	}
	return 0;
}

int DataSend( SOCKET s, char* buff,int nBufSize )
{
	int nBytesLeft = nBufSize;
	int idx = 0, nBytes = 0;
	while( nBytesLeft > 0 ) {
		nBytes = send( s,&buff[idx],nBytesLeft,0);
		if( nBytes == SOCKET_ERROR )
		{
			printf("Failed to send buffer to socket %d\n",WSAGetLastError() );
			closesocket( s );
			return -1;
		}
		nBytesLeft -= nBytes;
		idx += nBytes;
	}
	return idx;
}
int DataRecv( SOCKET s, const char* szFileName )
{
	return WriteToFile( s, szFileName );	
}

SOCKET DataAccept( SOCKET& s )
{
	SOCKET sAccept = accept( s ,NULL,NULL );
	if( sAccept != INVALID_SOCKET ) 
	{
		closesocket( s );
	}
	return sAccept;
}
int DealCommand( LPSOCKET_INF pSI )
{
	int nRetVal = 0;
	static SOCKET sAccept = INVALID_SOCKET;
	static SOCKET s       = INVALID_SOCKET;
	static BOOL   bPasv   = FALSE;
		
	char  szCmd[MAX_REQ_LEN]; 

	strcpy( szCmd, pSI->buffRecv );
	if( strtok( szCmd," \r\n") == NULL ) 
	{
		return -1;
	}
	strupr( szCmd );

	const char*  szOpeningAMode = "150 Opening ASCII mode data connection for ";
	static DWORD  dwIpAddr = 0;
	static WORD   wPort    = 0;
	// ?PORT n1,n2,n3,n4,n5,n6
	if( strstr(szCmd,"PORT") )
	{
		if( ConvertDotAddress( pSI->buffRecv+strlen("PORT")+1,&dwIpAddr,&wPort) == -1 )
		{
				return -1;
		}
		
		const char*  szPortCmdOK    = "200 PORT Command successful.\r\n";
		sprintf(pSI->buffSend,"%s",szPortCmdOK );
		if( SendRes( pSI ) == -1 )
		{
			return -1;
		}
		bPasv = FALSE;
		
		return CMD_OK;
	}
	if( strstr( szCmd,"PASV") ) 
	{
		if( DataConn( s, htonl(INADDR_ANY), PORT_BIND, MODE_PASV ) == -1 )
		{
			return -1;
		}
		char *szCommaAddress = ConvertCommaAddress( GetLocalAddress(),PORT_BIND );
		
		sprintf( pSI->buffSend,"227 Entering Passive Mode (%s).\r\n",szCommaAddress );
		
		if( SendRes( pSI ) == -1 ) 
		{
				return -1;
		}
		
    	
		bPasv = TRUE;
		return PASSIVE_MODE;		
	}

	if(strstr( szCmd, "RETR") ) 
	{
		if( bPasv ) sAccept = DataAccept(s);	
		char szFileNS[MAX_PATH];
		char *szFile = strtok( NULL," \r\n" );
		int nFileSize = CombindFileNameSize( szFile,szFileNS );
		if( nFileSize == -1  ) 
		{
			sprintf( pSI->buffSend,"550 %s: 系统找不到指定的文件.\r\n",szFile);
			if( SendRes( pSI ) == -1 )
				return -1;
			if( !bPasv ) closesocket( sAccept );
			else closesocket( s );
			
			return CANNOT_FIND; 
		}
		else 
			sprintf(pSI->buffSend,"%s%s.\r\n",szOpeningAMode,szFileNS);

		if( SendRes( pSI ) == -1 )
		{
			return -1;
		}
			
	
		char* buff =(char*)malloc(sizeof(char)*nFileSize);
		if( NULL ==buff ) 
		{
			printf("Not enough memory error!\n");
			return -1;
		}
		if( ReadFileToBuffer( szFile,buff, nFileSize ) == (DWORD)nFileSize ) 
		{
			// 处理Data FTP连接
			Sleep( 10 );
			if( bPasv ) 
			{
				DataSend( sAccept,buff,nFileSize );
				closesocket( sAccept );
			} 
			else 
			{
				if( DataConn( s,dwIpAddr,wPort,MODE_PORT ) == -1 )
				{
					return -1;
				}
				
				DataSend( s, buff, nFileSize );
				closesocket( s );
			}
		}
		if( buff != NULL )
		{
			free(buff);
		}

		sprintf( pSI->buffSend,"%s","226 Transfer complete.\r\n" );
		if( SendRes( pSI ) == -1 )
		{
			return -1;
		}
			
					
		return TRANS_COMPLETE;
	}
	if( strstr( szCmd, "STOR") ) 
	{
		if( bPasv ) sAccept = DataAccept(s);		
		char *szFile = strtok( NULL," \r\n" );
		if( NULL == szFile ) return -1;
		sprintf(pSI->buffSend,"%s%s.\r\n",szOpeningAMode,szFile);
		if( SendRes( pSI ) == -1 )
		{
			return -1;
		}
			

		// 处理Data FTP连接
		if( bPasv )
		{
			DataRecv( sAccept,szFile );
		}
			
		else 
		{
			if( DataConn( s,dwIpAddr,wPort,MODE_PORT ) == -1 )
			{
					return -1;
			}
			
			DataRecv( s, szFile );
		}
		
		sprintf( pSI->buffSend,"%s","226 Transfer complete.\r\n" );
		if( SendRes( pSI ) == -1 )
		{
			return -1;
		}
		
				
		return TRANS_COMPLETE;
	}
	if( strstr( szCmd,"QUIT" ) )
	{
		sprintf( pSI->buffSend,"%s","221 Good bye,欢迎下次再来.\r\n" );
		if( SendRes( pSI ) == -1 )
		{
				return -1;
		}
		
		return FTP_QUIT; 
	}

	//其余都是无效的命令
	sprintf(pSI->buffSend,"500 '%s' command not understand.\r\n",szCmd );
	if( SendRes( pSI ) == -1 )
	{
		 return -1;
	}
	return nRetVal;
}


//其他函数
char* GetLocalAddress()
{
    struct in_addr *pinAddr;
    LPHOSTENT	lpHostEnt;
	int			nRet;
	int			nLen;
	char        szLocalAddr[80];
	memset( szLocalAddr,0,sizeof(szLocalAddr) );
	// Get our local name
    nRet = gethostname(szLocalAddr,sizeof(szLocalAddr) );
	if (nRet == SOCKET_ERROR)
	{
		return NULL;
	}
	// "Lookup" the local name
	lpHostEnt = gethostbyname(szLocalAddr);
    if (NULL == lpHostEnt)	
	{
		return NULL;
	}
    // Format first address in the list
	pinAddr = ((LPIN_ADDR)lpHostEnt->h_addr);
	nLen = strlen(inet_ntoa(*pinAddr));
	if ((DWORD)nLen > sizeof(szLocalAddr))
	{
		WSASetLastError(WSAEINVAL);
		return NULL;
	}
	return inet_ntoa(*pinAddr);
}

int GetFileList( LPFILE_INF pFI, UINT nArraySize, const char* szPath  )
{
	WIN32_FIND_DATA  wfd;
	int idx = 0;
	CHAR lpFileName[MAX_PATH];
	GetCurrentDirectory( MAX_PATH,lpFileName );
	strcat( lpFileName,"\\" );
	strcat( lpFileName, szPath );
	HANDLE hFile = FindFirstFile( lpFileName, &wfd );
	if ( hFile != INVALID_HANDLE_VALUE ) 
	{
		pFI[idx].dwFileAttributes = wfd.dwFileAttributes;
		lstrcpy( pFI[idx].szFileName, wfd.cFileName );
		pFI[idx].ftCreationTime = wfd.ftCreationTime;
		pFI[idx].ftLastAccessTime = wfd.ftLastAccessTime;
		pFI[idx].ftLastWriteTime  = wfd.ftLastWriteTime;
		pFI[idx].nFileSizeHigh    = wfd.nFileSizeHigh;
		pFI[idx++].nFileSizeLow   = wfd.nFileSizeLow;
	
		while( FindNextFile( hFile,&wfd ) && idx < (int)nArraySize ) 
		{
			pFI[idx].dwFileAttributes = wfd.dwFileAttributes;
			lstrcpy( pFI[idx].szFileName, wfd.cFileName );
			pFI[idx].ftCreationTime = wfd.ftCreationTime;
			pFI[idx].ftLastAccessTime = wfd.ftLastAccessTime;
			pFI[idx].ftLastWriteTime  = wfd.ftLastWriteTime;
			pFI[idx].nFileSizeHigh    = wfd.nFileSizeHigh;
			pFI[idx++].nFileSizeLow   = wfd.nFileSizeLow;
		}
		FindClose( hFile );
	}
	return idx;
}


