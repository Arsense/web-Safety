//////////////////////////////////////////////////////////
// TCPClient.cpp文件


//#include "../common/InitSock.h"
#include <winsock2.h>
#pragma comment(lib, "WS2_32")

#include <stdio.h>
#include <iostream.h>


int main()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
	}
	// 创建套节字
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}
	
	// 也可以在这里调用bind函数绑定一个本地地址
	// 否则系统将会自动安排
	
	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = ntohs(2356);
	// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
	// 如果你的计算机没有联网，直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	if(connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf(" Failed connect() \n");
		return 0;
	}
	
	// 接收数据





	char *p = "GET http://127.0.0.1/ HTTP1.1";   //  == > GET / HTTP1.1;


	char Buffer[512]={0};

	strcpy(Buffer,p);


	send(s,Buffer,strlen(Buffer),0);

		
		
	

    int Ret = recv(s,Buffer,1000,0);


	Buffer[Ret]='\0';

	printf("%s",Buffer);
	

//	send(s,Buffer,strlen(Buffer),0);



	Sleep(INFINITE);


	return 0;
}
