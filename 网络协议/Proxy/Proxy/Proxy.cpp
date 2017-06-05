#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")


int g_ServiceListenPort = 2000;

DWORD WINAPI TCPServerToClient(LPVOID lpParam);

DWORD WINAPI TCPClientToServer(LPVOID lpParam); 
typedef struct _SERVICESOCKET_ 
{
	SOCKET TcpSocket;
	
	unsigned short ServiceListenPort;
	SOCKET Svctcp_server;
	SOCKET Svctcp_client;
	
}ServiceSocket, *pServiceSocket;

char DesIP[20]={0};

int  uPort = 110;


ServiceSocket ServiceSockTemp;

DWORD WINAPI WorkThread(LPVOID lpParam);    //工作线程


DWORD WINAPI TCPProxyThread(LPVOID lpParam);
bool InitSocket();
void main()
{
	
	if (!InitSocket())
	{
		printf("Initlize Socket Error\n");
		return;
	}
	
	SOCKET sListen;
	
	
	sListen= socket(AF_INET,SOCK_STREAM,0);
	
	if (sListen==INVALID_SOCKET)
	{
		printf("Create Listen Socket Error\n");
		
		return;
	}
	
	//初始化网卡
	sockaddr_in ProxyAddr;
	
	ProxyAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	ProxyAddr.sin_family = AF_INET;
	ProxyAddr.sin_port = htons(2356);


	int Ret = bind(sListen,(sockaddr*)&ProxyAddr,sizeof(ProxyAddr));
	
	if (Ret==SOCKET_ERROR)
	{
		printf("Bind Error\n");
		
		closesocket(sListen);
		
		
		return;
	}
	
	//监听
	
	listen(sListen,5);


	SOCKET sClient;
	
	
	sockaddr_in ClientAddr;
	
	int nLen = sizeof(ClientAddr);


	


	HANDLE hThread = NULL;
	while (1)
	{
		sClient = accept(sListen,(sockaddr*)&ClientAddr,&nLen);


		if (sClient==INVALID_SOCKET)
		{
			continue;
		}

	
		


		hThread = CreateThread(NULL,0,WorkThread,(LPVOID)sClient,0,NULL);
		
		
		if (hThread==NULL)
		{
			printf("Create Thread Error\n");
			
			
			break;
		}
		

		Sleep(INFINITE);
	}
}


DWORD WINAPI WorkThread(LPVOID lpParam)
{
	SOCKET s = (SOCKET)lpParam;

	

	ServiceSockTemp.TcpSocket = s;


	char Buffer[1024]={0};

	char Temp[1024]={0};
	
	int Ret = recv(s,Buffer,1024,0);

	Buffer[Ret]='\0';


	strcpy(Temp,Buffer);

	char* p = strstr(Buffer,"OK");

	*p='\0';

	strcpy(DesIP,Buffer);



	
	sockaddr_in ServerAddr;


	ServiceSockTemp.Svctcp_server = socket(AF_INET,SOCK_STREAM,0);


	ServerAddr.sin_addr.S_un.S_addr = inet_addr(DesIP);
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = ntohs(uPort); 



	if(connect(ServiceSockTemp.Svctcp_server,(sockaddr*)&ServerAddr,sizeof(ServerAddr)) ==SOCKET_ERROR)
	{
		printf("Connect Error\n");
		
		return -1;
	}


	ServiceSockTemp.ServiceListenPort = g_ServiceListenPort;



	sockaddr_in addr;
	
	
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");    //这里的IP地址是代理端的IP地址


	send(s,"127.0.0.1",strlen("127.0.0.1"),0);



	ServiceSockTemp.Svctcp_client= socket(AF_INET,SOCK_STREAM,0);
	
	if (ServiceSockTemp.Svctcp_client==INVALID_SOCKET)
	{
		printf("Create Client Socket Error\n");
		
		return  -1;
	}


	sockaddr_in LocalAddr;
	
	memset(&LocalAddr,0,sizeof(sockaddr_in));
	
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port   = htons(ServiceSockTemp.ServiceListenPort);
	LocalAddr.sin_addr.S_un.S_addr =INADDR_ANY;
	
	if (bind(ServiceSockTemp.Svctcp_client,(sockaddr*)&LocalAddr,sizeof(LocalAddr))==SOCKET_ERROR)
	{
		printf("Bind Error\n");
		return -1;
	}
	if (listen(ServiceSockTemp.Svctcp_client,5)==SOCKET_ERROR)
	{
		printf("Listen Error\n");
		return -1;
    }
	
	//将我们的监听套接字直接转换成通信套接字
	sockaddr_in RemoteAddr;
	int nlen = sizeof(sockaddr_in);
	ServiceSockTemp.Svctcp_client = accept(ServiceSockTemp.Svctcp_client,(sockaddr*)&RemoteAddr,&nlen);




	
	HANDLE hThread = CreateThread(NULL,0,TCPProxyThread,(LPVOID)&ServiceSockTemp, 0, 0);
	
	if (hThread == NULL)
	{
		printf("TCPProxyThread Error\n");
		return -1;
	}
	
	
	
	SuspendThread(GetCurrentThread());







	return 0;
}


DWORD WINAPI TCPProxyThread(LPVOID lpParam)
{
	pServiceSocket pTemp = (pServiceSocket)lpParam;
	
	ServiceSocket Temp;
	
	memcpy(&Temp,pTemp,sizeof(ServiceSocket));
	
	
	HANDLE hThread[2];
	
	
	hThread[0] = CreateThread(NULL,0,TCPClientToServer,(LPVOID)&Temp,0,0);
	
	
	if (hThread[0]==NULL)
	{
		printf("TCPClientToServer Thread Error\n");
		
		return -1;
	}
	
	
	hThread[1] = CreateThread(NULL,0,TCPServerToClient,(LPVOID)&Temp,0,0);
	
	
	if (hThread[1]==NULL)
	{
		printf("TCPServerToClient Thread Error\n");
		
		return -1;
	}
	
	
	WaitForMultipleObjects(2,hThread,false,INFINITE);
	
}


DWORD WINAPI TCPClientToServer(LPVOID lpParam) 
{
    //该线程函数是我们将客户端收到的数据进行转发到服务端
	
	
	pServiceSocket pTemp = (pServiceSocket)lpParam;
	
    char Buffer[4096];
    int count;
    while (1)
	{
        count = recv(pTemp->Svctcp_client,Buffer,4096,0);
		
		if (count==-1)
		{			
			continue;
		}
        

		send(pTemp->Svctcp_server,Buffer,count,0);
    }
}



DWORD WINAPI TCPServerToClient(LPVOID lpParam) 
{
	//该线程函数是我们将服务端收到的数据进行转发到客户端
	
	pServiceSocket pTemp = (pServiceSocket)lpParam;
	
	char Buffer[4096];
    int count;
	
	
	while (1)
	{
		count = recv(pTemp->Svctcp_server,Buffer,4096,0);
		
		if (count==-1)
		{
			continue;
		}
        
	

		send(pTemp->Svctcp_client,Buffer,count,0);
	}
	
	
}



bool InitSocket()
{
	WSADATA wsadata;
	
	if (WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		return false;
	}
	
	return true;
}


