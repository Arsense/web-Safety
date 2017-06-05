#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")

DWORD WINAPI SendProc(LPVOID lpParam);

DWORD WINAPI RecvProc(LPVOID lpParam);

SOCKET sReal;

int uPort = 2000;

bool InitSocket();

void main()
{
	if (!InitSocket())
	{
		printf("Initlize Socket Error\n");
		return;
	}

	SOCKET s;

	s = socket(AF_INET,SOCK_STREAM,0);


	sockaddr_in ProxyAddr;
	
	ProxyAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //这里也就是我们通过计算机名得到的IP地址
	
	ProxyAddr.sin_family = AF_INET;
	
	ProxyAddr.sin_port = htons(2356);   //这里第端口一定要和我们的代理的服务器的端口一致



	if(connect(s,(sockaddr*)&ProxyAddr,sizeof(sockaddr))==SOCKET_ERROR)
	{
		printf("Cannot connect to Proxy\n");
		
		closesocket(s);  
		
		return;
	}



	char BufferIP[50]={0};

	scanf("%s",BufferIP);

	char BufferPort[20]={0};

	scanf("%s",BufferPort);


	strcat(BufferIP,"OK");
	strcat(BufferIP,BufferPort);
	send(s,BufferIP,strlen(BufferIP),0);



	memset(BufferIP,0,50);
	
	
	int nRet = recv(s,BufferIP,50,0);

	BufferIP[nRet] = '\0';


	sockaddr_in addrin;

	addrin.sin_addr.S_un.S_addr = inet_addr(BufferIP);
	addrin.sin_family = AF_INET;
	addrin.sin_port = ntohs(uPort); 



	sReal = socket(AF_INET,SOCK_STREAM,0);
	
	while (1)
	{
		if(connect(sReal,(sockaddr*)&addrin,sizeof(addrin))==SOCKET_ERROR)
		{
			printf("Connect Error\n");
			
			continue;
		}
		
		break;
		
	}


	HANDLE hThread[2];
	
	
		hThread[0] = CreateThread(NULL,0,RecvProc,(LPVOID)sReal,0,NULL);
	
		hThread[1] = CreateThread(NULL,0,SendProc,(LPVOID)sReal,0,NULL);
	
	
	
		WaitForMultipleObjects(2,hThread,false,INFINITE);

	
	
	
	


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




DWORD WINAPI RecvProc(LPVOID lpParam)
{
	SOCKET s = (SOCKET)lpParam;
	
	char Buffer[512]={0};
	
	while (1)
	{
		int Ret = recv(s,Buffer,512,0);
		
		Buffer[Ret]='\0';
		
		printf("%s\n",Buffer);
		
		
		memset(Buffer,0,512);
	}
	
	
	return 0;
}


DWORD WINAPI SendProc(LPVOID lpParam)
{
	SOCKET s = (SOCKET)lpParam;
	
	char Buffer[512]={0};
	
	while (1)
	{
		scanf("%s",Buffer);
		
		int Ret = send(s,Buffer,strlen(Buffer),0);	
		
		memset(Buffer,0,512);
	}
	
	
	return 0;
}