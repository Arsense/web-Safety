#include <WINSOCK2.H>
#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32")

//这里的第一个成员是我们第一次与客户端进行连接的套接字
//这里的第三个成员是我们与邮件服务器进行连接的套接字
//这里的第四个成员是我们重新与客户端进行连接的套接字
typedef struct _SERVICESOCKET_ 
{
	SOCKET TcpSocket;
	
	unsigned short ServiceListenPort;
	SOCKET Svctcp_server;
	SOCKET Svctcp_client;
	
}ServiceSocket, *pServiceSocket;


//该数据结构是记录客户信息因为我们是靠这里的信息与客户端进行连接校验的
typedef struct _USERINFOR_
{
	char UserName[20];
	char UserPass[20];
	_USERINFOR_* pNext;

}UserInfor, *pUserInfor;   


pUserInfor pHead = NULL;
pUserInfor pTail = NULL;

//通常我们要登陆代理首先要进行注册我在这里就将我们的客户用链表的形式将其设置死

void CreateList(char* NameBuffer,char* UserPass);



//初始化套接字库
bool InitSocket();

DWORD WINAPI WorkThread(LPVOID lpParam);    //工作线程

bool Identification(SOCKET s);

void ReceiveBuffer(SOCKET s,char* Buffer,int Len); 

void SendBuffer(SOCKET s,char* Buffer,int Len);


//转换端口号

int ConvertPort(char* Buffer);

void GetRequest(ServiceSocket& ServiceSocketTemp);
void ReplyRequest(ServiceSocket& ServiceSocketTemp);

DWORD WINAPI TCPClientToServer(LPVOID lpParam);
DWORD WINAPI TCPServerToClient(LPVOID lpParam);

DWORD WINAPI TCPProxyThread(LPVOID lpParam);

char* g_Name = NULL;

char* g_Password = NULL;




int g_ServiceListenPort = 2000;

void main()
{
	char NameBuffer[20] = "shine";
	char UserPass[20] = "1234";

	CreateList(NameBuffer,UserPass);

	memset(NameBuffer,0,20);
	memset(UserPass,0,20);
	
	strcpy(NameBuffer,"wangxue");
	strcpy(UserPass,"1234");


	CreateList(NameBuffer,UserPass);
	
	
	
	
	if (!InitSocket())
	{
		printf("Init Socket Error\n");
		
		return;
	}

	//创建一个监听套接字
	
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


	HANDLE hThread;
	//循环连接
	while (1)
	{
		sClient = accept(sListen,(sockaddr*)&ClientAddr,&nLen);
		
		if (sClient==INVALID_SOCKET)
		{
			continue;
		}
		
		//创建线程
		
		hThread = CreateThread(NULL,0,WorkThread,(LPVOID)sClient,0,NULL);
		
		
		if (hThread==NULL)
		{
			printf("Create Thread Error\n");
			
			
			break;
		}
		
		
	}
	
	
	
	
	WaitForSingleObject(hThread,INFINITE);
	
	closesocket(sListen);



}


DWORD WINAPI WorkThread(LPVOID lpParam)
{
	
	SOCKET sClient = (SOCKET)lpParam;
	
	
	ServiceSocket ServiceTemp;
	
	ServiceTemp.TcpSocket = sClient;

	
	if (Identification(ServiceTemp.TcpSocket))
	{
		
		GetRequest(ServiceTemp);
		
		ReplyRequest(ServiceTemp);
	}
	
	//当我们的认证通过时我们要进行真正的数据通信
	
	
	
	return 0;
}



bool Identification(SOCKET s)
{
	
	//我们要将在这里接受到第一次数据包的接受并且进行校验
	
	char Buffer[1024]={0};    //这是接受数据存放的缓冲区
	
	ReceiveBuffer(s,Buffer,2);   //我们先取前两个字节
	
	int NumMethod = Buffer[1]; 
	
	memset(Buffer,1024,0);
	
	ReceiveBuffer(s,Buffer,NumMethod);    //我们取最后的一个字节来判断方法
	
	//测试
	//printf("%d\n",NumMethod);
	
	
	int i = 0;
	for (i = 0;i<NumMethod;i++)
	{
		if (Buffer[i]==0x02)
		{
			//测试
			
			//printf("OK");
			
			break;
		}
	}


	//定义一个缓冲区用于发送
	
	char Reply[2]={0};
	
	Reply[0] = 0x05;
	
	
	if (i==NumMethod)
	{
		Reply[1] = 0xff;
		
		return false;
	}
	
	else
	{
		Reply[1] = 0x02;
	}
	
	SendBuffer(s,Reply,2);    //我们可以在客户端进行判断



	
	memset(Buffer,0,1024);
	//我们要在这里接受第二个数据包的并要判断前两个字节
	ReceiveBuffer(s,Buffer,2); 
	
	if (Buffer[0]!=0x01)
	{
		return false;
	}
	
	int ulen = (int)Buffer[1];     //这里包存的数据就是我们用户姓名的长度我们进行动态申请内存
	
	
	g_Name = (char*)malloc(sizeof(ulen));



	ReceiveBuffer(s,g_Name,ulen);
	
	
	//测试
	g_Name[ulen]='\0';
	printf("User Come:%s\n",g_Name);
	
	
	//接受Password
	memset(Buffer,0,1024);
	
	ReceiveBuffer(s,Buffer,1);
	
	ulen = (int)Buffer[0];
	
	
	g_Password = (char*)malloc(sizeof(ulen));
	
	ReceiveBuffer(s,g_Password,ulen);
	
	
	//测试
	g_Password[ulen]='\0';
	printf("User Password:%s\n",g_Password);


	//以上的读取数据和以前Socket5有些不一样要注意代理程序的修改



	memset(Reply,0,2);
	Reply[0] = 0x05;
	
	//校验

	pUserInfor pTemp = pHead;
	while (pTemp!=NULL)
	{
		if (!strcmp(g_Name,pTemp->UserName) && !strcmp(g_Password,pTemp->UserPass))
		{
			
			
			//状态为零为验证成功
			
			Reply[1]=0x00;    
			SendBuffer(s,Reply,2);
			
			
			//我没有处理
			return true;
			
		}

		else
		{
			pTemp = pTemp->pNext;
		}
	}

	

		//错误的话我们进行数据反馈
		Reply[1]=0x01;
		
		SendBuffer(s,Reply,2);

	
	
	return false;

}




void GetRequest(ServiceSocket& ServiceSocketTemp)
{
	ServiceSocketTemp.Svctcp_server = socket(AF_INET,SOCK_STREAM,0);
	
	if (ServiceSocketTemp.Svctcp_server==INVALID_SOCKET)
	{
		printf("Create Server Socket Error\n");
		
		return;
	}

	char Buffer[1024]={0};   //接受数据并且保存到缓冲区中 
	
	char Reply[2]={0};
	
	ReceiveBuffer(ServiceSocketTemp.TcpSocket,Buffer,10);

	sockaddr_in ServerAddr;
	

	
	ServerAddr.sin_addr.s_addr =
		MAKELONG(MAKEWORD((Buffer[4]&0xff),(Buffer[5]&0xff)),
		MAKEWORD((Buffer[6]&0xff),(Buffer[7]&0xff)));
	
	
	int uPort = ConvertPort(Buffer);
	
	//测试
	printf("%s\n",inet_ntoa(ServerAddr.sin_addr));
	
	printf("%d",uPort);


	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = ntohs(uPort); 

	//在这里我们最好将得到的邮件服务器的IP地址和端口进行记录，然后连接如果连接成功就记录下来，如果连接不通我们就其从内存中删除并发送
	//一个错误指令这里我就不判断了


	if(connect(ServiceSocketTemp.Svctcp_server,(sockaddr*)&ServerAddr,sizeof(ServerAddr)) ==SOCKET_ERROR)
	{
		printf("Connect Error\n");
		return;
	}

	else
	{
		Reply[0] = 0x05;
		Reply[1] = 0x00;
		
	//  测试	
	//	send(ServiceSocketTemp.Svctcp_server,"Hello",strlen("Hello"),0);

		SendBuffer(ServiceSocketTemp.TcpSocket,Reply,2);
	}



	

}

void ReplyRequest(ServiceSocket& ServiceSocketTemp)
{
	char Reply[1024] ={0};
    Reply[0] = 0x05;
    Reply[1] = 0x00;
    Reply[2] = 0x00;
    Reply[3] = 0x01;
	
	
	g_ServiceListenPort++;
	
	ServiceSocketTemp.ServiceListenPort = g_ServiceListenPort;    //这里的端口是重新与用户连接的端口这里非常重要

	sockaddr_in addr;
	
	
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");    //这里的IP地址是代理端的IP地址
	
	Reply[4] = addr.sin_addr.S_un.S_un_b.s_b1;
	Reply[5] = addr.sin_addr.S_un.S_un_b.s_b2;
	Reply[6] = addr.sin_addr.S_un.S_un_b.s_b3;
	Reply[7] = addr.sin_addr.S_un.S_un_b.s_b4;
	
	
	Reply[8] = ServiceSocketTemp.ServiceListenPort/256;
    Reply[9] = ServiceSocketTemp.ServiceListenPort%256;
	 
	
	
	//发送一个数据包来迎合请求数据包
	
	SendBuffer(ServiceSocketTemp.TcpSocket,Reply,10);      //发送给客户让客户重新更新连接


	ServiceSocketTemp.Svctcp_client = socket(AF_INET,SOCK_STREAM,0);
	
	if (ServiceSocketTemp.Svctcp_client==INVALID_SOCKET)
	{
		printf("Create Client Socket Error\n");
		
		return;
	}


	sockaddr_in ServerAddr;
	
	memset(&ServerAddr,0,sizeof(sockaddr_in));
	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port   = htons(ServiceSocketTemp.ServiceListenPort);
	ServerAddr.sin_addr.S_un.S_addr =INADDR_ANY;
	
	if (bind(ServiceSocketTemp.Svctcp_client,(sockaddr*)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)
	{
		printf("Bind Error\n");
		return;
	}
	if (listen(ServiceSocketTemp.Svctcp_client,5)==SOCKET_ERROR)
	{
		printf("Listen Error\n");
		return;
    }

	//将我们的监听套接字直接转换成通信套接字
	sockaddr_in RemoteAddr;
	int nlen = sizeof(sockaddr_in);
	ServiceSocketTemp.Svctcp_client = accept(ServiceSocketTemp.Svctcp_client,(sockaddr*)&RemoteAddr,&nlen);
	
	HANDLE hThread = CreateThread(NULL,0,TCPProxyThread,(LPVOID)&ServiceSocketTemp, 0, 0);
	
	if (hThread == NULL)
	{
		printf("TCPProxyThread Error\n");
		return;
	}
	
	
	
	SuspendThread(GetCurrentThread());



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
        
		SendBuffer(pTemp->Svctcp_server,Buffer,count);
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
        
		SendBuffer(pTemp->Svctcp_client,Buffer,count);
	}
	
	
}



//辅助函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//这里的第三个参数是接受的字节长度
void ReceiveBuffer(SOCKET s,char* Buffer,int Len)
{
	if (!Len)
	{
		return;   //如果我们接受的长度为0时我们就退出函数
	}
	
	int Temp = Len;
	
	int Ret = 0;
	while (1)
	{
		
		Temp = Temp-recv(s,Buffer+(Len-Temp),Len,0);
		
		if (Temp==0)
		{
			break;
		}
	}
}


void SendBuffer(SOCKET s,char* Buffer,int Len)
{
	if (!Len)
	{
		return;
	}
	
	int Temp = Len;
	
	while (1)
	{
		Temp = Temp-send(s,Buffer+(Len-Temp),Len,0);
		
		if (Temp==0)
		{
			break;
		}
	}
}


int ConvertPort(char* Buffer)
{
	char a1 = Buffer[8];
	
	char a2 = Buffer[9];
	
	int b1 = a1;
	
	int b2 = a2;
	
	if (b1==0)
	{
		return b2;
	}
	
	
	else
	{
		b2<<=8;
		
		b1+=b2;
		
		return b1;
	}
	
}

void CreateList(char* NameBuffer,char* UserPass)
{
	pUserInfor pTemp;

	pTemp = (pUserInfor)malloc(sizeof(UserInfor));

	if (pTemp==NULL)
	{
		printf("No Memory\n");
	}

	else
	{
		strcpy(pTemp->UserName,NameBuffer);

		strcpy(pTemp->UserPass,UserPass);

		pTemp->pNext = NULL;
		
		if (pHead==NULL)
		{
			pHead = pTail = pTemp;
		}

		else
		{
			pTail->pNext = pTemp;

			pTemp = pTail->pNext;
		}
	}
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