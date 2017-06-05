#include <winsock2.h>
#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>


#pragma comment(lib,"Ws2_32")

#define MAXBUFFERSIZE 1000


typedef struct _PROXYSOCKET_ 
{
    SOCKET  user_proxy;			             //本地机器到PROXY 服务机的socket
	SOCKET  proxy_server;		             //PROXY 服务机到远程主机的socket
	BOOL    IsUser_ProxyClosed;              // 本地机器到PROXY 服务机状态
	BOOL    IsProxy_ServerClosed;            // PROXY 服务机到远程主机状态

}ProxySocket,*pProxySocket;




//我在这里定义了一个结构体关于远程服务器的信息

typedef struct _REMOTEADDR_
{
	char Address[256];        //远程服务的地址
	int  uPort ;              //代理与远程服务的连接端口
	HANDLE User_SvrOK;        //这是个事件如果代理和远程服务连接上就为不受信否则受信
	pProxySocket pPros;       // 维护一组SOCKET的指针

}RemoteAddr,*pRemoteAddr;




bool InitSocket();
bool ResolveInformation(char* Buffer,char* RemoteServerAddr,int* Port);


DWORD WINAPI UseProxy(LPVOID lpParma);

DWORD WINAPI ProxyToServer(LPVOID lpParam);


void main()
{
	if (!InitSocket())
	{
		printf("Initilize Socket Error\n");

		return;
	}

	//创建一个监听套接字

	SOCKET sListen;

	sListen = socket(AF_INET,SOCK_STREAM,0);

	if (sListen==INVALID_SOCKET)
	{
		printf("Create Listen Error\n");

		return;
	}

	//初始化本地网卡

	sockaddr_in LocalAddr;
	LocalAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(2356);          //这里我们可以提供给用户让用户来设定


	//绑定监听套接字

	int Ret = bind(sListen,(sockaddr*)&LocalAddr,sizeof(LocalAddr));

	if (Ret==SOCKET_ERROR)
	{
		printf("Bind Error\n");

		closesocket(sListen);
		return;
	}


	//监听

	listen(sListen,5);


	//创建一个通信套接字这是与客户端进行连接的套接字
	
	SOCKET sClient;

	sockaddr_in RemoteClientAddr;
	int nLen = sizeof(RemoteClientAddr);
	
	while (1)
	{
		sClient = accept(sListen,(sockaddr*)&RemoteClientAddr,&nLen);


		//当我接收到客户端的连接我们要创建一个工作线
		CreateThread(NULL,0,UseProxy,(LPVOID)sClient,0,NULL);

	
	}
}




DWORD WINAPI UseProxy(LPVOID lpParma)
{
	ProxySocket ProSockTemp;

	
	ProSockTemp.user_proxy = (SOCKET)lpParma;

	RemoteAddr RemoteAddrTemp;


	char Buffer[MAXBUFFERSIZE];


	if (ProSockTemp.user_proxy==INVALID_SOCKET)
	{
		return -1;
	}

	//接受从客户端发送来的第一个数据包

	ProSockTemp.IsUser_ProxyClosed=FALSE;
	ProSockTemp.IsProxy_ServerClosed=TRUE;


	int Ret = recv(ProSockTemp.user_proxy,Buffer,MAXBUFFERSIZE,0);

	if (Ret==SOCKET_ERROR)
	{
		printf("Recv Error\n");

		closesocket(ProSockTemp.user_proxy);


		return -1;
	}


	//当我们接收到数据时我们要对数据包进行解析

	Buffer[Ret]='\0';


	//测试
	printf("%s",Buffer);


	RemoteAddrTemp.pPros = &ProSockTemp;

	RemoteAddrTemp.User_SvrOK = CreateEvent(NULL,TRUE,FALSE,NULL);
	
	if (!ResolveInformation(Buffer,RemoteAddrTemp.Address,&RemoteAddrTemp.uPort))
	{

		printf("Resolve Error\n");
		
		closesocket(ProSockTemp.user_proxy);
		return -1;
	}


	//创建一个线程该线程是代理服务与远程服务进行通信的线程

	HANDLE hThread = CreateThread(NULL,0,ProxyToServer,(LPVOID)&RemoteAddrTemp,0,NULL);

	WaitForSingleObject(RemoteAddrTemp.User_SvrOK ,INFINITE);    //我们在这里等待事件的触发


	while (ProSockTemp.IsProxy_ServerClosed==FALSE&&ProSockTemp.IsUser_ProxyClosed==FALSE)
	{
		Ret = send(ProSockTemp.proxy_server,Buffer,sizeof(Buffer),0);


		if (Ret==SOCKET_ERROR)
		{
			printf("SendError\n");
			
			if(ProSockTemp.IsProxy_ServerClosed==FALSE)
			{
				closesocket(ProSockTemp.proxy_server);
				
				ProSockTemp.IsProxy_ServerClosed=TRUE;
			}
			continue;
		}


		Ret=recv(ProSockTemp.user_proxy,Buffer,sizeof(Buffer),0);
		
		if(Ret==SOCKET_ERROR)
		{
			printf("Recv Error\n"); 
			if(ProSockTemp.IsUser_ProxyClosed==FALSE)
			{
				closesocket(ProSockTemp.user_proxy);
				ProSockTemp.IsUser_ProxyClosed=TRUE;
			}
			continue;
		}

		//测试
		printf("%s",Buffer);
		
	}




	WaitForSingleObject(hThread,INFINITE);
	return 0;
}

DWORD WINAPI ProxyToServer(LPVOID lpParam)
{
	//该线程中的参数就保存了关于远程服务器的信息

	pRemoteAddr pTemp = (pRemoteAddr)lpParam;

	int uPort = pTemp->uPort;

	int Ret = 0;

	char Buffer[MAXBUFFERSIZE] ={0};


	//定义一个远程连接的网卡信息

	sockaddr_in Remoteaddr_in ={0};

                     
	char RemoteIP[512]={0};
	strcpy(RemoteIP,pTemp->Address);
	Remoteaddr_in.sin_addr.S_un.S_addr = inet_addr(RemoteIP);
    Remoteaddr_in.sin_family = AF_INET;
	Remoteaddr_in.sin_port = htons(uPort); 
	
	
	
	
	
	//创建一个连接的套接字

	SOCKET sConnect = socket(AF_INET,SOCK_STREAM,0);


	if (sConnect==INVALID_SOCKET)
	{
		printf("Create Connect Socket Error\n");


		return -1;
	}

	if (connect(sConnect,(sockaddr*)&Remoteaddr_in,sizeof(Remoteaddr_in))== SOCKET_ERROR) 
	{
		printf("Connect RemoteServer Error\n");
		
		pTemp->pPros->IsProxy_ServerClosed = TRUE;
		
		SetEvent(pTemp->User_SvrOK);    //通知UserProxy线程触发	
		
		return -1;
	}

	//连接成功时

	pTemp->pPros->proxy_server = sConnect;
	pTemp->pPros->IsProxy_ServerClosed = FALSE;
	SetEvent(pTemp->User_SvrOK); 

	while (!pTemp->pPros->IsProxy_ServerClosed &&!pTemp->pPros->IsUser_ProxyClosed)
	{
		Ret = recv(sConnect,Buffer,sizeof(Buffer),0);

		if (Ret==SOCKET_ERROR)
		{
			closesocket(sConnect);
			pTemp->pPros->IsProxy_ServerClosed = TRUE;

			break;
		}


		Ret = send(pTemp->pPros->user_proxy,Buffer,sizeof(Buffer),0);

		if (Ret == SOCKET_ERROR) 
		{
			
			closesocket(pTemp->pPros->user_proxy);
			pTemp->pPros->IsUser_ProxyClosed=TRUE;
			break;						
		}
	}





	return 0;
}



//解析数据包函数

bool ResolveInformation(char* Buffer,char* RemoteServerAddr,int* Port)
{
	char Command[512] = {0};
	char Protocol[128]={0};
	char Temp[MAXBUFFERSIZE]={0};

	sscanf(Buffer,"%s%s%s",Command,Temp,Protocol);   //调试一下看看就知道了



	// "GET http://127.0.0.1/ HTTP1.1"; 

	//在缓冲区Temp中查找Http

	char* p = NULL;
	p = strstr(Temp,"http://");

	//下移指针
	if (p)
	{
		p = p+strlen("http://");

		//我们要解析出远程服务器的地址

		for (int i=0;i<strlen(p);i++)
		{
			if (*(p+i)=='/')
			{
				break;
			}
		}                                 
		
		//找到字符/我们要将其去掉

		*(p+i) = 0;

		strcpy(RemoteServerAddr,p);   //127.0.0.1           // 1  accept client 127.0.0.1
                                                            // 2  RemoteSever.sin.sun.sin = inet_addr(127.0.0.1) //connect 
															//    RemoteServer.port = htons(80);
																				//AF_INET
		//去掉远程的主机的名字

		p = strstr(Buffer,"http://");


		for (int j=0;j<i+strlen("http://");j++)
		{
			*(p+j) = ' '; 
		}


		
		p = Buffer;

		j = 0;

	
		for (i=0;i<strlen(Buffer);i++)
		{
			if (*(p+i)!=' ')
			{
					
			   Buffer[j] = *(p+i);
			   j++;
		
			}
		}

		Buffer[j]='\0';


		p = strstr(Buffer,"/");

		//定义一个临时缓冲区

		char Temp[512]={0};

		strcpy(Temp,p);

		int n = p-Buffer;

		Buffer[n] = ' ';

		Buffer[n+1]='\0';

		strcat(Buffer,Temp);

		strcat(Buffer,"\r\n\r\n");
		//设置远程的端口

		*Port = 80;
		
	}

	else
	{
		return false;
	}


	return true;

}


bool InitSocket()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2,2),&wsadata)==0)
	{
		return true;
	}

	return false;
}
