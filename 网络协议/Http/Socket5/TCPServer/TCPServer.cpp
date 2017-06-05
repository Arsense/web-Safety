#include <winsock2.h>
#pragma comment(lib, "WS2_32")

#include <stdio.h>
#include <iostream.h>


DWORD WINAPI RecvProc(LPVOID lpParam);
DWORD WINAPI SendProc(LPVOID lpParam);

void main()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
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
	ProxyAddr.sin_port = htons(2000);
	
	//绑定
	int Ret = bind(sListen,(sockaddr*)&ProxyAddr,sizeof(ProxyAddr));
	
	if (Ret==SOCKET_ERROR)
	{
		printf("Bind Error\n");
		
		closesocket(sListen);
		
		
		return;
	}
	
	//监听
	
	listen(sListen,5);
	
	//创建一个与客户通信的套接字
	
	SOCKET sClient;
	
	
	sockaddr_in ClientAddr;
	
	int nLen = sizeof(ClientAddr);

	sClient = accept(sListen,(sockaddr*)&ClientAddr,&nLen);





	HANDLE hThread[2];


	hThread[0] = CreateThread(NULL,0,RecvProc,(LPVOID)sClient,0,NULL);

	hThread[1] = CreateThread(NULL,0,SendProc,(LPVOID)sClient,0,NULL);



	WaitForMultipleObjects(2,hThread,false,INFINITE);

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

