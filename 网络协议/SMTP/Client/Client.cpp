#include <winsock2.h>
#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>


#pragma comment(lib,"Ws2_32.lib")


#define uPort 25


bool InitSocket();



void main()
{
	//初始化套接字库
	
	if (!InitSocket())
	{
		printf("Initilize Socket Error\n");


		return;
	}


	//定义我们所需要的缓冲区也就是我们所认为的SMTP的基本工作指令

	char Buffer[30]={0};

	char RemoteIP[30]={0};

	memset(RemoteIP,20,0);
	memset(Buffer,20,0);

	printf("Server IP:\n");
	scanf("%s",RemoteIP);

	char HELO[30] = "HELO ";                  //服务器地址的缓冲区也叫做域名

	strcat(HELO,RemoteIP);

	strcat(HELO,"\r\n");

	char MAIL[30] = "MAIL FROM:<";			//发送者的地址

	memset(Buffer,0,20);

	printf("Send From:\n");
	
	scanf("%s",Buffer);
	
	strcat(MAIL,Buffer);

	strcat(MAIL,">\r\n");


	memset(Buffer,0,20);           
	
	char RCPT[30] = "RCPT TO:<";             //收件者的地址

	printf("Recive To:\n");
	
	scanf("%s",Buffer);

	strcat(RCPT,Buffer);

	strcat(RCPT,">\r\n");



	char szText[1024] = {0};                //信件的内容


	printf("Message:\n");

	scanf("%s",szText);


	strcat(szText,"\r\n\r\n.\r\n");         //想想这里千万不要错了


	//在这里定义了一个指针数组每个指针指向一个缓冲区
	char *MailMessage[] = 
	{
		HELO,
		MAIL,
		RCPT,
		"DATA\r\n",
		szText,
		"QUIT\r\n",
		NULL
	};



	//创建一个监听的套接字

	SOCKET sClient = socket(AF_INET,SOCK_STREAM,0);

	if (sClient==INVALID_SOCKET)
	{
		printf("Create Socket Error\n");

		return;
	}

	//初始化我们想要发送的远程地址空间
	sockaddr_in RemoteAddr;

	RemoteAddr.sin_addr.S_un.S_addr = inet_addr(RemoteIP);

	RemoteAddr.sin_family = AF_INET;

	RemoteAddr.sin_port = ntohs(uPort);



	//我们在这里主动连接

	if (connect(sClient,(sockaddr*)&RemoteAddr,sizeof(RemoteAddr))==SOCKET_ERROR)
	{
		closesocket(sClient);

		printf("Connect Error\n");


		return;
	}


	int iLength = 0;
	int iEnd = 0;

	char sBuff[255] ={0};
	int Index = 0;
	//发送请求

	do 
	{
		//接受到响应信息

		iLength = recv(sClient,(LPSTR)sBuff+iEnd, sizeof(sBuff)-iEnd,0);
		iEnd += iLength;
		sBuff[iEnd] = '\0';
		send(sClient,(LPSTR)MailMessage[Index],strlen(MailMessage[Index]),0);
		Index++;


	} while (MailMessage[Index]);

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