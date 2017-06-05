#include <Winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <malloc.h>



#pragma comment(lib,"Ws2_32")

bool InitSocket();
#define uPort 25

void main()
{
	//初始化套接字库
	
	if (!InitSocket())
	{
		printf("Initilize Socket Error\n");
		
		
		return;
	}

	//创建一个监听的套接字

	SOCKET sListen = socket(AF_INET,SOCK_STREAM,0);
	
	if (sListen==INVALID_SOCKET)
	{
		printf("Create Socket Error\n");
		
		return;
	}


	//初始化我们服务器地址空间

	sockaddr_in LocalAddr;
	
	LocalAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	
	LocalAddr.sin_family = AF_INET;
	
	LocalAddr.sin_port = htons(uPort);


	int Ret = bind(sListen,(sockaddr*)&LocalAddr,sizeof(LocalAddr));
	
	if (Ret==SOCKET_ERROR)
	{
		printf("Bind Error\n");
		
		return ;
	}
	
	
	//监听
	
	listen(sListen,5);


	sockaddr_in Remoteaddr;
	int nLen = sizeof(Remoteaddr);
	
	SOCKET sClient = accept(sListen,(sockaddr*)&Remoteaddr,&nLen);

	if (sClient==SOCKET_ERROR)
	{
		printf("Accept Error\n");

		closesocket(sListen);
		
		return;
	}

	//定义一个缓冲区接受数据包

	char Buffer[1024]={0};

	send(sClient,"Welcome",strlen("Welcome"),0);

	char *MailMessage[] = 
	{
		    "HELO ",
			"MAIL FROM:",
			"RCPT TO:",
			"DATA\r\n",
			"QUIT\r\n",
	};

	char*p = Buffer;

	int iLength = 0;
	int iEnd = 0;

	char szText[1024]={0};

	int Index = 0;
	do 
	{
		recv(sClient,(LPSTR)Buffer+iEnd, sizeof(Buffer)-iEnd,0);

		p = strstr(&Buffer[iEnd],MailMessage[Index]);

		if(p!=NULL)
		{
			iEnd += iLength;

			if(Index==3)
			{
				send(sClient,"354\r\n.\r\n",strlen("354\r\n.\r\n"),0);

			

				int Ret = recv(sClient,szText,1024,0);


				szText[Ret]='\0';
				
				printf("%s",szText);    //在这里最好用动态内存
			}


			else
			{
			    send(sClient,"250",strlen("250"),0);
				
				printf("%s",Buffer);
				
			}

				Index++;
		
		}
				

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