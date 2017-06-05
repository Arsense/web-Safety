//////////////////////////////////////////////////////////
// TCPClient.cpp文件



#include <winsock2.h>
#pragma comment(lib, "WS2_32")

#include <stdio.h>
#include <iostream.h>

DWORD WINAPI RecvProc(LPVOID lpParam);
DWORD WINAPI SendProc(LPVOID lpParam);


int main()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &wsaData) != 0)
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
	



	//这是我们发送的第一个数据包

	/*
	+----+----------+----------+
	|VER | NMETHODS | METHODS  |   
	+----+----------+----------+
	| 1  |    1     | 1 to 255 |
	+----+----------+----------+
    */
	char a[3] ={0x05,0x01,0x02};      //我们也可使用十六进制  a[3]={0x01,0x01,0x02};

	send(s,a,strlen(a),0);



	//接受第一个回复包

	char Buffer[512]={0};


	int Ret = recv(s,Buffer,512,0);

	Buffer[Ret]='\0';


	if (Buffer[0]!=0x05||Buffer[1]!=0x02)
	{
		
		return -1;
	}

	printf("Please Input Your Name And Password\n");

	

	char NameBuffer[20]={0};
	
	scanf("%s",NameBuffer);

	char PassWord[20]={0};
	
	scanf("%s",PassWord);
	//发送我们的第二个数据包就是姓名和密码，这个数据包根据用户的不同要进行不同的定义

	//我在这里没有进行任何安全处理


	//注意我们这里的版本号必须是0x01而不是我们刚刚从代理发来的0x05
	
	/*
	+----+------+----------+------+----------+
	|VER | ULEN |  UNAME   | PLEN |  PASSWD  |
	+----+------+----------+------+----------+
	| 1  |  1   | 1 to 255 |  1   | 1 to 255 |
	+----+------+----------+------+----------+
	*/

	//发送姓名
	Buffer[0]=0x01;
	Buffer[1]=strlen(NameBuffer)+0x30;


	

	send(s,Buffer,2,0);

	send(s,NameBuffer,strlen(NameBuffer),0);





	//发送密码

	memset(Buffer,512,0);

	Buffer[0]=strlen(PassWord)+0x30;
		
	send(s,Buffer,1,0);
	
	send(s,PassWord,strlen(PassWord),0);




	//接受第二次数据包的校验
	Ret = recv(s,Buffer,512,0);
	
	if(Buffer[0]==0x05&&Buffer[1]==0x00)
	{
		printf("Success Welcome!!!\n");
	}

	else
	{
		printf("Error\n");
	}



	//构建请求包

	//请求数据包
	/*
	+----+-----+-------+------+----------+----------+
	|VER | CMD |  RSV  | ATYP | DST.ADDR | DST.gListenSocket |
	+----+-----+-------+------+----------+----------+
	| 1  |  1  | X'00' |  1   | Variable |    2     |
	+----+-----+-------+------+----------+----------+
    */


	memset(Buffer,512,0);

	Buffer[0] = 0x05;

	printf("Input Cmd\n");

	char Cmd[10]={0};

	scanf("%s",Cmd);


	if (stricmp(Cmd,"Connect")==0)
	{
		Buffer[1]=0x01;
	}

	else if (stricmp(Cmd,"Bind")==0)
	{
		Buffer[1]=0x02;
	}

	else if (stricmp(Cmd,"Udp")==0)
	{
		Buffer[1]=0x03;
	}

	else
	{
		Buffer[1]=0xff;

		printf("Cmd Error\n");
	}


	Buffer[2]=0x00;    //这里是必须设置为0x00的


	Buffer[3]=0x01;
	
	
    //目标IP字段的构建
	
	printf("Input Destination IP:\n");

	char IP[20]={0};

	scanf("%s",IP);

	char* p = IP;
	
	char Temp[20]={0};
	
	int i = 0;
	
	int j = 4;
	
	while(p!=NULL)
	{
		p = strstr(IP,".");
		
		if (p!=NULL)
		{
			strcpy(Temp,p+1);
			
			*p='\0';
			
			i = atoi(IP);

			strcpy(IP,Temp);
			
			
			Buffer[j] = char(i);
			
		}
				
		else
		{
			i = atoi(Temp);
			
			Buffer[j] = char(i);
		}
		
		
		j++;
	}

	 //目标Port字段的构建
	
	printf("Input Destination Port:\n");

	char Port[20]={0};
	
	scanf("%s",Port);


	i = 0;

	i = atoi(Port);


	Buffer[8] = (char)(i/256);
	
	Buffer[9] = (char)(i-Buffer[0]*256);

	send(s,Buffer,10,0);



	//接受应答数据包

	memset(Buffer,0,512);


	Ret = recv(s,Buffer,512,0);        



	//收到数据包我们要进行数据包的分析



	sockaddr_in addrin;    //地址信息


	
	
	addrin.sin_addr.s_addr =
		MAKELONG(MAKEWORD((Buffer[4]&0xff),(Buffer[5]&0xff)),
                      MAKEWORD((Buffer[6]&0xff),(Buffer[7]&0xff)));



	
	//测试
	//printf("%s",inet_ntoa(addrin.sin_addr));
	

	int uPort = 0;


	uPort = ((int)Buffer[8])*256+(unsigned char)Buffer[9];


	addrin.sin_port = ntohs(uPort);

	addrin.sin_family = AF_INET;
	//测试
	//printf("%d",uPort);



	closesocket(s);

	//我们得到新的数据信息后我们要建立新的连接与代理服务器

	SOCKET sReal = socket(AF_INET,SOCK_STREAM,0);


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



	return 0;
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

