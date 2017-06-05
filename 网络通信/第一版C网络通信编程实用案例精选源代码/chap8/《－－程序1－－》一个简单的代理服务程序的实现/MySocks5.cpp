// MySocks5.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "MySocks5.h"
#include "afxsock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
#define HTTP  "http://"
#define FTP   "ftp://"
#define MAXBUFFERSIZE      20*1024      //缓冲区大小

CWinApp theApp;
using namespace std;

//Proxy 端口
UINT pport=1180;
UINT ProxyToServer(LPVOID pParam);
UINT UserToProxy(LPVOID pParam);

struct ProxySockets
{
    SOCKET  user_proxy;			//本地机器到PROXY 服务机的socket
	SOCKET  proxy_server;		//PROXY 服务机到远程主机的socket
	BOOL    IsUser_ProxyClosed; // 本地机器到PROXY 服务机状态
	BOOL    IsProxy_ServerClosed; // PROXY 服务机到远程主机状态
};

struct ProxyParam
{
	char Address[256];    // 远程主机地址
	HANDLE User_SvrOK;    // PROXY 服务机到远程主机的联结状态
	ProxySockets *pPair;    // 维护一组SOCKET的指针
	int     Port;         // 用来联结远程主机的端口
};                   //这个结构用来PROXY SERVER与远程主机的信息交换.
SOCKET    listen_socket;   //用来侦听的SOCKET。

int StartProxyServer()  //启动服务
{
     WSADATA wsaData;
     sockaddr_in local;
     SOCKET listen_socket;
     if(::WSAStartup(0x202,&wsaData)!=0)
     {
           printf("\nError in Startup session.\n");WSACleanup();
           return -1;
     }
     local.sin_family=AF_INET;
     local.sin_addr.s_addr=INADDR_ANY;
     local.sin_port=htons(pport);
     listen_socket=socket(AF_INET,SOCK_STREAM,0);
     if(listen_socket==INVALID_SOCKET)
     {
		printf("\nError in New a Socket.");WSACleanup();
		return -2;
     }
     if(::bind(listen_socket,(sockaddr *)&local,sizeof(local))!=0)
     {
		printf("\n Error in Binding socket.");
		WSACleanup();
		return -3;	
     }
	if(::listen(listen_socket,5)!=0)
	{
		printf("\n Error in Listen.");
		WSACleanup(); 
		return -4;
	}
	::listen_socket=listen_socket; 
	AfxBeginThread(UserToProxy,NULL);   //启动侦听
	return 1;
}
int CloseServer()  //关闭服务
{
	closesocket(listen_socket);
	WSACleanup();
	return 1;
}
//分析接收到的字符，得到远程主机地址
int ResolveInformation( char * str, char *address, int * port)
{
	char buf[MAXBUFFERSIZE], command[512], proto[128], *p;
	int j;
	sscanf(str,"%s%s%s",command,buf,proto);
	p=strstr(buf,HTTP);
	//HTTP
	if(p)
	{
		p+=strlen(HTTP);
		for(int i=0;i< strlen(p);i++)
			if( *(p+i)=='/') break;
		*(p+i)=0;
		strcpy(address,p);
		p=strstr(str,HTTP);
		//去掉远程主机名: 
		//GET http://www.njust.edu.cn/ HTTP1.1  == > GET / HTTP1.1
		for(int j=0;j< i+strlen(HTTP);j++)
			*(p+j)=' ';
		//缺省的 http 端口 
		*port=80;      
	}
	else
	{
		//FTP, 不支持, 下面的代码可以省略.
		p=strstr(buf,FTP);
		if(!p) return 0;
		p+=strlen(FTP);
		for(int i=0;i< strlen(p);i++)
			if( *(p+i)=='/') break;      //Get The Remote Host
		*(p+i)=0;
		for(j=0;j< strlen(p);j++)
			if(*(p+j)==':') 
			{
				*port=atoi(p+j+1);    //Get The Port
				*(p+j)=0;
			}
			else *port=21;		 			              
		strcpy(address,p);
		p=strstr(str,FTP);
		for(j=0;j< i+strlen(FTP);j++)
			*(p+j)=' ';		
	}
	return 1; 
}
// 取到本地的数据，发往远程主机
UINT UserToProxy(void *pParam)
{
	char Buffer[MAXBUFFERSIZE];
	int  Len;
	sockaddr_in from;
	SOCKET msg_socket;
	int fromlen,retval;
	ProxySockets SPair;
	ProxyParam ProxyP;
	CWinThread *pChildThread;
	fromlen=sizeof(from);
	msg_socket=accept(listen_socket,(struct sockaddr*)&from,&fromlen);
	AfxBeginThread(UserToProxy,pParam); //启动另一侦听.
	if( msg_socket==INVALID_SOCKET)
	{ 
		printf( "\nError  in accept "); 
		return -5;
	}
	 
	//读客户的第一行数据
	SPair.IsUser_ProxyClosed=FALSE;
	SPair.IsProxy_ServerClosed=TRUE;
	SPair.user_proxy=msg_socket;
	retval=recv(SPair.user_proxy,Buffer,sizeof(Buffer),0);
	 
	if(retval==SOCKET_ERROR)
	{ 
		printf("\nError Recv"); 
		if(SPair.IsUser_ProxyClosed==FALSE)
		{
			closesocket(SPair.user_proxy);
			SPair.IsUser_ProxyClosed=TRUE;
		}
	}
	if(retval==0)
	{
		printf("Client Close connection\n");
		if(SPair.IsUser_ProxyClosed==FALSE)
		{	
			closesocket(SPair.user_proxy);
			SPair.IsUser_ProxyClosed=TRUE;
		}
	}
	Len=retval;
	#ifdef _DEBUG
		Buffer[Len]=0;
		printf("\n Received %d bytes,data[%s]from client\n",retval,Buffer);
	#endif
	//
	SPair.IsUser_ProxyClosed=FALSE;
	SPair.IsProxy_ServerClosed=TRUE;
	SPair.user_proxy=msg_socket;
	ProxyP.pPair=&SPair;
	ProxyP.User_SvrOK=CreateEvent(NULL,TRUE,FALSE,NULL);
	ResolveInformation( Buffer,ProxyP.Address,&ProxyP.Port);
	pChildThread=AfxBeginThread(ProxyToServer,(LPVOID)&ProxyP);
	::WaitForSingleObject(ProxyP.User_SvrOK,60000);  //等待联结
	::CloseHandle(ProxyP.User_SvrOK);
	while(SPair.IsProxy_ServerClosed ==FALSE && SPair.IsUser_ProxyClosed==FALSE)
	{	
		retval=send(SPair.proxy_server,Buffer,Len,0);
		if(retval==SOCKET_ERROR)
		{ 
			printf("\n send() failed:error%d\n",WSAGetLastError());
			if(SPair.IsProxy_ServerClosed==FALSE)
			{
				closesocket(SPair.proxy_server);
				SPair.IsProxy_ServerClosed=TRUE;
			}
			continue;
		}
	    retval=recv(SPair.user_proxy,Buffer,sizeof(Buffer),0);
	 
		if(retval==SOCKET_ERROR)
		{
			printf("\nError Recv"); 
			if(SPair.IsUser_ProxyClosed==FALSE)
			{
				closesocket(SPair.user_proxy);
				SPair.IsUser_ProxyClosed=TRUE;
			}
			continue;
		}
		if(retval==0)
		{
			printf("Client Close connection\n");
		    if(SPair.IsUser_ProxyClosed==FALSE)
			{
				closesocket(SPair.user_proxy);
				SPair.IsUser_ProxyClosed=TRUE;
			}
			break;
		}
		Len=retval;
		#ifdef _DEBUG
			Buffer[Len]=0;
			printf("\n Received %d bytes,data[%s]from client\n",retval,Buffer);
		#endif
	} //End While
	if(SPair.IsProxy_ServerClosed==FALSE)
	{
		closesocket(SPair.proxy_server);
		SPair.IsProxy_ServerClosed=TRUE;
	}
	if(SPair.IsUser_ProxyClosed==FALSE)
	{
		closesocket(SPair.user_proxy);
		SPair.IsUser_ProxyClosed=TRUE;
	}
	::WaitForSingleObject(pChildThread->m_hThread,20000);  //Should check the return value
	return 0;
}
// 读取远程主机数据，并发往本地客户机
UINT ProxyToServer(LPVOID pParam)
{
    ProxyParam * pPar=(ProxyParam*)pParam;
	char Buffer[MAXBUFFERSIZE];
	char *server_name= "localhost";
	unsigned short port ;
	int retval,Len;
	unsigned int addr;
	int socket_type ;
	struct sockaddr_in server;
	struct hostent *hp;
	SOCKET  conn_socket;
	socket_type = SOCK_STREAM;
	server_name = pPar->Address;
	port = pPar->Port;
	if (isalpha(server_name[0])) 
	{
		/* server address is a name */
		hp = gethostbyname(server_name);
	}
	else  
	{
		/* Convert nnn.nnn address to a usable one */
		addr = inet_addr(server_name);
		hp = gethostbyaddr((char *)&addr,4,AF_INET);
	}
	if (hp == NULL ) 
	{
		fprintf(stderr,"Client: Cannot resolve address [%s]: Error %d\n",server_name,WSAGetLastError());
		::SetEvent(pPar->User_SvrOK);	
		return 0;
	}
	//
	// Copy the resolved information into the sockaddr_in structure
	//
	memset(&server,0,sizeof(server));
	memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
	server.sin_family = hp->h_addrtype;
	server.sin_port = htons(port);
	conn_socket = socket(AF_INET,socket_type,0); /* 打开一个 socket */
	if (conn_socket < 0 ) 
	{
		fprintf(stderr,"Client: Error Opening socket: Error %d\n",WSAGetLastError());
		pPar->pPair->IsProxy_ServerClosed=TRUE;
		::SetEvent(pPar->User_SvrOK);	
		return -1;
	}
	#ifdef _DEBUG
		printf("Client connecting to: %s\n",hp->h_name);
	#endif
	if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR) 
	{
		fprintf(stderr,"connect() failed: %d\n",WSAGetLastError());
		pPar->pPair->IsProxy_ServerClosed=TRUE;	
		::SetEvent(pPar->User_SvrOK);	
		return -1;
	}
	pPar->pPair->proxy_server=conn_socket;
	pPar->pPair->IsProxy_ServerClosed=FALSE;
    ::SetEvent(pPar->User_SvrOK);
	// cook up a string to send
	while(!pPar->pPair->IsProxy_ServerClosed &&!pPar->pPair->IsUser_ProxyClosed)
	{
		retval = recv(conn_socket,Buffer,sizeof (Buffer),0 );
		if (retval == SOCKET_ERROR ) 
		{
			fprintf(stderr,"recv() failed: error %d\n",WSAGetLastError());
			closesocket(conn_socket);
			pPar->pPair->IsProxy_ServerClosed=TRUE;
			break;
		}
		Len=retval;
		if (retval == 0) 
		{
			printf("Server closed connection\n");
			closesocket(conn_socket);
			pPar->pPair->IsProxy_ServerClosed=TRUE;
			break;
		}
		retval = send(pPar->pPair->user_proxy,Buffer,Len,0);
		if (retval == SOCKET_ERROR) 
		{
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
			closesocket(pPar->pPair->user_proxy);
			pPar->pPair->IsUser_ProxyClosed=TRUE;
			break;						
		}
		#ifdef _DEBUG	
			Buffer[Len]=0;		
			printf("Received %d bytes, data [%s] from server\n",retval,Buffer);
		#endif
	}
	if(pPar->pPair->IsProxy_ServerClosed==FALSE)
	{
		closesocket(pPar->pPair->proxy_server);
		pPar->pPair->IsProxy_ServerClosed=TRUE;
	}
	if(pPar->pPair->IsUser_ProxyClosed==FALSE)
	{
		closesocket(pPar->pPair->user_proxy);
		pPar->pPair->IsUser_ProxyClosed=TRUE;
	}
	return 1;
}
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	// 初始化SOCKET
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// 错误处理
		cerr<<  _T("Fatal Error: MFC initialization failed") <<  endl;
		nRetCode = 1;
		return 1;
	}
	// 主程序开始.
	cout<<"请设置代理服务器端口号!"<<endl<<"Port:";
	cin>>pport;
	if(pport<1024)
	{
		cout<<"端口号小于1024，有可能会引起端口冲突!"<<endl;
	}
	cout<<"正在监听连接!如果想结束程序，请按\"e\"键"<<endl;
	StartProxyServer();	
	while(1)	
		if(getchar()=='e') break;
	CloseServer();
	return nRetCode;
}
