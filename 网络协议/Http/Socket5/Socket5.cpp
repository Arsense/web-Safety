#include <WINSOCK2.H>
#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32")


enum CMDTYPE 
{
	CONNECT,BIND,UDP
};


typedef struct _SERVICESOCKET_ 
{
	SOCKET TcpSocket;
	CMDTYPE CmdType;


    unsigned short ServiceListenPort;

	SOCKET Svctcp_server;
	SOCKET Svctcp_client;

}ServiceSocket, *pServiceSocket;




bool InitSocket();

bool Identification(SOCKET s);



void ReceiveBuffer(SOCKET s,char* Buffer,int Len);
void SendBuffer(SOCKET s,char* Buffer,int Len);


//请求函数
void GetRequest(ServiceSocket& ServiceSocketTemp);

//应答函数
void ReplyRequest(ServiceSocket& ServiceSocketTemp);

bool CreateSocket(SOCKET& s);


void GetHostIP(char* Buffer);

in_addr GetName(char* Name);
DWORD WINAPI WorkThread(LPVOID lpParam);
DWORD WINAPI TCPProxyThread(LPVOID lpParam);

DWORD WINAPI TCPClientToServer(LPVOID lpParam);
DWORD WINAPI TCPServerToClient(LPVOID lpParam) ;

char* g_Name= NULL;
char* g_Password = NULL;

int g_ServiceListenPort = 2000;

//这是两个进行校验的密码和姓名
char User[100]="shine";          
char Password[100]="2356"; 

void main()
{
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

		    //printf("%d\n",Buffer[i]-0x30);
			
			break;
		}
	}

	//当我们在这里接受到一个身份认证的一个数据包时我们必须给客户一个回复包

	//这是第一个回复包

	
	/*
		+----+----------+
		|VER | NMETHODS |  
		+----+----------+
		| 1  |    1     | 
		+----+----------+
	*/


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


	
	//这里很重要好好体会一下
	ReceiveBuffer(s,Reply,2);         //我们要将网卡缓冲区中的内存清空
    //这里很重要好好体会一下




	memset(Reply,0,2);
	memset(Buffer,0,1024);
	//我们要在这里接受第二个数据包的并要判断前两个字节
	ReceiveBuffer(s,Buffer,2); 

	if (Buffer[0]!=0x01)
	{
		return false;
	}

	int ulen = Buffer[1]-0x30;     //这里包存的数据就是我们用户姓名的长度我们进行动态申请内存


	g_Name = (char*)malloc(sizeof(ulen));

	//接受姓名
	ReceiveBuffer(s,g_Name,ulen);


	//测试
	g_Name[ulen]='\0';
	printf("User Come:%s\n",g_Name);


	//接受Password
	memset(Buffer,0,1024);
	
	ReceiveBuffer(s,Buffer,1);
	
	ulen = Buffer[0]-0x30;


	g_Password = (char*)malloc(sizeof(ulen));
	
	//接受姓名
	ReceiveBuffer(s,g_Password,ulen);
	
	
	//测试
	g_Password[ulen]='\0';
	printf("User Password:%s\n",g_Password);



	//当我们接受到姓名和密码后进行第二次的数据包的回复


	/*
	+----+--------+
	|VER | STATUS |
	+----+--------+
	| 1  |   1    |
	+----+--------+
	*/

	memset(Reply,0,2);
	Reply[0] = 0x05;

	//校验
	if (!strcmp(g_Name,User) && !strcmp(g_Password,Password))
	{
	

		//状态为零为验证成功
		
		Reply[1]=0x00;    
		SendBuffer(s,Reply,2);


		//我没有处理
		return true;

	}

	else
	{
		//错误的话我们进行数据反馈
		Reply[1]=0x01;

		SendBuffer(s,Reply,2);
	}


	return false;
}




void GetRequest(ServiceSocket& ServiceSocketTemp)
{
	//请求数据包
/*
+----+-----+-------+------+----------+----------+
|VER | CMD |  RSV  | ATYP | DST.ADDR | DST.gListenSocket |
+----+-----+-------+------+----------+----------+
| 1  |  1  | X'00' |  1   | Variable |    2     |
+----+-----+-------+------+----------+----------+
    */


	char Buffer[1024]={0};


	ReceiveBuffer(ServiceSocketTemp.TcpSocket,Buffer,10);


	//在这里最好进行版本的校验我就在这里不处理了

	
	printf("The COMMAND:\n");

    switch (Buffer[1])
	{
	case 0x01:
		{
			printf("Connect\n");
			ServiceSocketTemp.CmdType = CONNECT;
			break;
		}
			
	case 0x02:
		{
			printf("Bind\n");
			ServiceSocketTemp.CmdType = BIND;
			break;
		}
			
	case 0x03:
		{
			printf("UDP ASSOCIATE\n");
			ServiceSocketTemp.CmdType = UDP;
			break;
		}
	
	default:
		{
			break;
		}

    }


	if (Buffer[3]==0x01)              //我在这里没有过多的处理
	{
		if(ServiceSocketTemp.CmdType == UDP)
		{
			
        }
        else if(ServiceSocketTemp.CmdType == CONNECT)
		{
			
			//解析数据包中存在的目的地址
			//测试
			printf("The Destination IP :  ");

			printf("%d.%d.%d.%d    Port :",Buffer[4]&0xff,Buffer[5]&0xff,Buffer[6]&0xff,Buffer[7]&0xff);


			int uPort = ((int)Buffer[8])*256+(unsigned char)Buffer[9];

			printf("%d",uPort);



			if (!CreateSocket(ServiceSocketTemp.Svctcp_server))
			{
				return;
			}

			//设置远程服务端的网卡信息
			sockaddr_in RemoteServerAddr;

			RemoteServerAddr.sin_family = AF_INET;
			RemoteServerAddr.sin_port = ntohs(uPort);


			RemoteServerAddr.sin_addr.s_addr =
				MAKELONG(MAKEWORD((Buffer[4]&0xff),(Buffer[5]&0xff)),
                      MAKEWORD((Buffer[6]&0xff),(Buffer[7]&0xff)));



			if(connect(ServiceSocketTemp.Svctcp_server,(sockaddr*)&RemoteServerAddr,sizeof(RemoteServerAddr)) ==SOCKET_ERROR)
			{
				printf("Connect Error\n");
				return;
			}
			

			//测试

		//	send(ServiceSocketTemp.Svctcp_server,"Hello",strlen("Hello"),0);

		}
		else if (ServiceSocketTemp.CmdType == BIND)
		{
			
			//没有做处理
		}
	}
}


//应答请求
void ReplyRequest(ServiceSocket& ServiceSocketTemp)
{
/*
+----+-----+-------+------+----------+----------+
|VER | REP |  RSV  | ATYP | BND.ADDR | BND.gListenSocket |
+----+-----+-------+------+----------+----------+
| 1  |  1  | X'00' |  1   | Variable |    2     |
+----+-----+-------+------+----------+----------+
    */


	char Reply[1024] ={0};
    Reply[0] = 0x05;
    Reply[1] = 0x00;
    Reply[2] = 0x00;
    Reply[3] = 0x01;


	g_ServiceListenPort++;

	ServiceSocketTemp.ServiceListenPort = g_ServiceListenPort;

	//GetHostIP(Reply+4);   调用该函数是最好的做法


	sockaddr_in addr;

	
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	Reply[4] = addr.sin_addr.S_un.S_un_b.s_b1;
	Reply[5] = addr.sin_addr.S_un.S_un_b.s_b2;
	Reply[6] = addr.sin_addr.S_un.S_un_b.s_b3;
	Reply[7] = addr.sin_addr.S_un.S_un_b.s_b4;


	Reply[8] = ServiceSocketTemp.ServiceListenPort/256;
    Reply[9] = ServiceSocketTemp.ServiceListenPort%256;


	//发送一个数据包来迎合请求数据包

	SendBuffer(ServiceSocketTemp.TcpSocket,Reply,10);


	if (ServiceSocketTemp.CmdType == CONNECT)
	{
        CreateSocket(ServiceSocketTemp.Svctcp_client);
       
		sockaddr_in ServerAddr;
        
		memset(&ServerAddr,0x00,sizeof(sockaddr_in));
        
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

bool CreateSocket(SOCKET& s)
{
	s = socket(AF_INET,SOCK_STREAM,0);

	if (s==INVALID_SOCKET)
	{
		printf("Create Server Socket Error\n");

		return false;
	}


	return true;


}

void GetHostIP(char* Buffer)
{
	int Ret = 0;
    char HostName[100];
    Ret = gethostname(HostName,1024);
    
	if (Ret != 0)
	{
		printf("%s",HostName);
        
		return;
    }

	in_addr HostAdd = GetName(HostName);
    Buffer[0] = HostAdd.S_un.S_un_b.s_b1;
    Buffer[1] = HostAdd.S_un.S_un_b.s_b2;
    Buffer[2] = HostAdd.S_un.S_un_b.s_b3;
    Buffer[3] = HostAdd.S_un.S_un_b.s_b4;
}


in_addr GetName(char* Name)
{
    hostent* he = gethostbyname(Name);
    
	return *((in_addr*)he->h_addr);
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
