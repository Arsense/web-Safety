#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#pragma comment(lib,"Ws2_32.lib")




//该程序因为了和我们所看的的图形相符我们采取了代理的方法并且使用了Socket5


//全局变量
int ServerPort =110;  //POP3邮件服务器的端口号


SOCKET sReal;



//初始化socket库
bool InitSocket();


//用户登陆函数
bool Login(char* ServerName,char* UserName,int UserPassWord,SOCKET& s,char* ProxyServerName,char* ProxyUserName,
		   int ProxyUserPassWord,int ProxyPort);

//创建用户目录函数
void CreateUserDirectory(char* UserName);


//地址转换函数
unsigned long ConvertServerNameToLongServerAddr(char* ServerName);


//与代理服务器的连接
bool Socket5Start(char* ProxyServerName,int ProxyPort,char* ProxyUserName,int ProxyPassWord,unsigned long ServerAddr,int sPort,
				  SOCKET& s);




//检查文件个数
void MailSTAT(SOCKET& s);  //该函数是通过代理发送标记STAT来获取该用户的信件信息

//查询文件信息操作指令查看文档

void MailLIST(SOCKET& s,char a);

void MailDownLoad(SOCKET& s, char* srt); //文件下载




//工作线程
DWORD WINAPI WorkThread(LPVOID lpParam);

DWORD WINAPI RecvProc(LPVOID lpParam);
DWORD WINAPI SendProc(LPVOID lpParam);
void main()
{
	if (!InitSocket())
	{
		printf("Initlize Socket Error\n");
		return;
	}

	//邮件服务器信息
	char ServerName[20]={0};




	printf("Input Server Name\n");

	scanf("%s",ServerName);

	printf("Input User Name\n");

	char UserName[20] = {0};

	scanf("%s",UserName);
	
	printf("Input User PassWord\n");

	int UserPassWord = 0;

	scanf("%d",&UserPassWord);


	//邮件发送代理服务器

	printf("Input ProxyServer Name\n");
	
	char ProxyServerName[20] = {0};

	scanf("%s",ProxyServerName);

	int ProxyPort = 0;

	printf("Input ProxyServer Port\n");
	
	scanf("%d",&ProxyPort);

	char ProxyUserName[20] = {0};

	printf("Input ProxyUserServer Name\n");

	scanf("%s",ProxyUserName);

	int ProxyUserPassWord = 0;

	printf("Input Proxy Server PassUserWord\n");

	scanf("%d",&ProxyUserPassWord);


	//创建一个套接字

	SOCKET s;


	if (!Login(ServerName,UserName,UserPassWord,s,ProxyServerName,ProxyUserName,ProxyUserPassWord,ProxyPort))
	{
		//测试

		printf("Login() Error");

		return;
	}

	

	//在这里我们要创建一个线程循环的以消息的形式进行与邮件服务器进行命令通信
	
	HANDLE hThread;

	hThread = CreateThread(NULL,0,WorkThread,(LPVOID)sReal,0,NULL);

	




	WaitForSingleObject(hThread,INFINITE);








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


//这里的参数s最好使用引用固定内存
bool Login(char* ServerName,char* UserName,int UserPassWord,SOCKET& s,char* ProxyServerName,char* ProxyUserName,
		   int ProxyUserPassWord,int ProxyPort)
{
	//创建一个用户目录到当前程序下

	CreateUserDirectory(UserName);

	char Buffer[512]={0};

	if (ProxyServerName!=NULL)    //在这里的判断我们也可以更详细些
	{
		unsigned long ServerAddr = ConvertServerNameToLongServerAddr(ServerName);

		if (!Socket5Start(ProxyServerName,ProxyPort,ProxyUserName,ProxyUserPassWord,ServerAddr,ServerPort,s))
		{
			
			//测试
			printf("Socket5Start() Error");
			
			return  false;
		}


		//如果与代理端成功连接上并且将我们要连接的邮件服务器的信息成功发送到代理端并的成功的返回

        

		//这里非常重要就是我们要与代理重新建立连接以便实现转发这里是个重点


		recv(s,Buffer,10,0);
		
		sockaddr_in addrin;
		
		
		
		addrin.sin_addr.s_addr =
			MAKELONG(MAKEWORD((Buffer[4]&0xff),(Buffer[5]&0xff)),
			MAKEWORD((Buffer[6]&0xff),(Buffer[7]&0xff)));
		
		
		int uPort = 0;

		uPort = ((int)Buffer[8])*256+(unsigned char)Buffer[9];
		
		//测试
		//printf("%s\n",inet_ntoa(ServerAddr.sin_addr));
		
		//printf("%d",uPort);
		
		
		addrin.sin_family = AF_INET;
		addrin.sin_port = ntohs(uPort); 
		
		//我们得到新的数据信息后我们要建立新的连接与代理服务器
		
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

	

		//我们在这里设置的两个线程主要就是为了我们做连接的测试


	//	HANDLE hThread[2];
		
		
	//	hThread[0] = CreateThread(NULL,0,RecvProc,(LPVOID)sReal,0,NULL);
		
	//	hThread[1] = CreateThread(NULL,0,SendProc,(LPVOID)sReal,0,NULL);
		
		
		
	//	WaitForMultipleObjects(2,hThread,false,INFINITE);

		

		//将我们刚设置的ServerName和密码通过代理服务器发送到邮件服务器进行验证

		memset(Buffer,0,512);

		


	    sprintf(Buffer,"USER %s\r\n",UserName);

		//int n = strlen(Buffer); 测试
		send(sReal,Buffer,strlen(Buffer),0);


		memset(Buffer,0,512);
		sprintf(Buffer,"PASS %d\r\n",UserPassWord);
		send(sReal,Buffer,strlen(Buffer),0);



		//这里最好进行错误的处理使用Recv函数我没有处理使用while循环让用户有重新登陆的机会

		memset(Buffer,0,512);


		int nRet =recv(sReal,Buffer,512,0);

		Buffer[nRet]='\0';

		printf("%s\n",Buffer);
		

		return true;
		
	}

	return false;

}

void CreateUserDirectory(char* UserName)
{
	//在这里我们可以使用文件查找的几个相关函数
	
	WIN32_FIND_DATA FileData;   
	
	HANDLE hSearch;

	char Temp[20] = {0};

	strcpy(Temp,".\\");

	strcat(Temp,UserName);

	hSearch = FindFirstFile(Temp,&FileData);

	if (hSearch==INVALID_HANDLE_VALUE||FileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY)
	{
		//如果目录中没有该用户的目录我们进行创建这和QQ中创建文件目录的方法一样是一个经常用到的知识点

		//在这里我们最好进行密码的验证要用到数据库或是数据结构保存密码和用户名。。。(当第二次进行操作时)

		CreateDirectory(Temp,NULL);
	}

	//最后要关闭文件句柄

	FindClose(hSearch);
}

unsigned long ConvertServerNameToLongServerAddr(char* ServerName)
{


	//在这里要学习DEMO 在这里我是将我们的计算机改名了最好使用DEMO的用法
	HOSTENT* host = gethostbyname(ServerName);


	if (host==NULL)
	{
		printf("Can Not Resolve Host Name\n");

		return -1;
	}

	return ntohl((*(struct in_addr*)host->h_addr).S_un.S_addr);
}


DWORD WINAPI WorkThread(LPVOID lpParam)
{
	SOCKET s = (SOCKET)lpParam;


	char ComBuffer[20]={0};



	int nRet = 0;

	printf("\n");
	printf("Please Input STAT First\n");
	while (1)
	{
		
		memset(ComBuffer,0,20);
		
		printf("Input Command\n");
	//	scanf("%s",ComBuffer);
		cin.getline(ComBuffer, 20);

		if (strcmp(ComBuffer,"STAT")==0)
		{
			MailSTAT(s);              
		}

		if (strstr(ComBuffer,"LIST"))
		{
			char* pTemp = ComBuffer;

			pTemp = pTemp + strlen("LIST-");


			
			MailLIST(s,*pTemp);
		} 	    

		
		if (strstr(ComBuffer, "RETR "))
		{
			MailDownLoad(s, ComBuffer);
			
		}
		
	}
}

void MailSTAT(SOCKET& s)
{
	char Buffer[4096]={0};

	int NumOfMails = 0;
	
	sprintf(Buffer,"STAT\r\n");
	
	send(s,Buffer,strlen(Buffer),0);
	
	memset(Buffer,0,4096);
	
	int nRet = recv(s,Buffer,4096,0);
	
	Buffer[nRet]='\0';
	
	//收到正确的信息后将我们信件信息格式化到我们参数NumOfMails中所以我们最好使用引用
	
	sscanf(Buffer+3,"%d",&NumOfMails);


	printf("Mails Is:%d\n",NumOfMails);


	char* pTemp = Buffer+6; 

	while (*pTemp!='\0')
	{
		while (*pTemp!='\r'&&*(pTemp+1)!='\n')
		{
			printf("%c",*pTemp);             //在这里我没有用字符串操作，用的是字符操作因此文件起名时不要用中文的因为中文的ASC码不止一个字节
			pTemp++;
		}
		pTemp+=2;

		printf("\n");
	}

}



void MailLIST(SOCKET& s,char a)
{
	char Buffer[4096]={0};

	int NumOfMail = 0;

	int CountOfMail = 0;
	
	sprintf(Buffer,"LIST %c",a);
	
	send(s,Buffer,strlen(Buffer),0);
	
	memset(Buffer,0,4096);


	int nRet = recv(s,Buffer,4096,0);

	Buffer[nRet] = '\0';

//	char* pTemp = Buffer+6; 

	sscanf(Buffer+3,"%d",&NumOfMail);

	sscanf(Buffer+6,"%d",&CountOfMail);

	printf("%dFile:  %dBytes\n",NumOfMail,CountOfMail);

}

bool Socket5Start(char* ProxyServerName,int ProxyPort,char* ProxyUserName,int ProxyPassWord,unsigned long ServerAddr,int sPort,
				  SOCKET& s)
{
	//初始化套接字
	s = socket(AF_INET,SOCK_STREAM,0);


	//初始化连接对象也就是我们想要连接的代理服务器

	sockaddr_in ProxyAddr;

	ProxyAddr.sin_addr.S_un.S_addr = htonl(ConvertServerNameToLongServerAddr(ProxyServerName));  //这里也就是我们通过计算机名得到的IP地址

	ProxyAddr.sin_family = AF_INET;

	ProxyAddr.sin_port = htons(ProxyPort);   //这里第端口一定要和我们的代理的服务器的端口一致

	//测试：

	//printf("Proxy Server IP: %s ",inet_ntoa(ProxyAddr.sin_addr));


	//与我们初始化好的代理服务器进行连接

	if(connect(s,(sockaddr*)&ProxyAddr,sizeof(sockaddr))==SOCKET_ERROR)
	{
		printf("Cannot connect to Proxy\n");

		closesocket(s);  
		
		return false;
	}


	//发送确认数据包这里要查看我们Socket5的学习

	char Buffer[512] = {0};

	BYTE Version=5;
	BYTE authorWay1=0;	//不需要认证
	BYTE authorWay2=2;	//认证
	int rsv=0;


	//在这里我们要构建我们的第一个数据包

	sprintf(Buffer,"%c%c%c%c",Version,2,authorWay1,authorWay2);     //这里数据包的构建一定要和我们代理服务器的代码中数据包解析一致


	send(s,Buffer,4,0);   //发送完后我们要接受数据包

	int nRet = recv(s,Buffer,512,0);


	Buffer[nRet]='\0';

	if (Buffer[1]==authorWay2)
	{
		//这里要将我们的用户的姓名和密码发送到代理端

		sprintf(Buffer,"%c%c%s%c%d",char(0x01),char(strlen(ProxyUserName)),ProxyUserName,char(sizeof(int)),ProxyPassWord);

		nRet = 7+strlen(ProxyUserName);   //这里好好的体会

		send(s,Buffer,nRet,0);


		nRet=recv(s,Buffer,512,0);


		Buffer[nRet]='\0';
	
		if(Buffer[1]!=0)
		{
			return false;
		}
	
		
	}

	
	//发送我们想要访问的邮件服务器的IP地址和端口号
	
	int uPort = htons(sPort);
	
	
	char* p = Buffer;
	
	
	//格式化邮件服务器中的信息
	
	memset(Buffer,0,sizeof(Buffer));
	
	sprintf(Buffer,"%c%c%c%c%c%c%c%c%c%c",Version,char(0x01),rsv,char(0x01),((char*)&ServerAddr)[3],
		((char*)&ServerAddr)[2],((char*)&ServerAddr)[1],((char*)&ServerAddr)[0],
		((char *)&uPort)[0],((char *)&uPort)[1]);   //这里的端口号是110
	
	
	send(s,Buffer,10,0);    //将信息发送到代理服务器端特别是最后两个字节要解析的参照DEMO



	//等待校验信息

	memset(Buffer,0,512);
	
	nRet = recv(s,Buffer,512,0);
	
	Buffer[nRet] = '\0';

	if (Buffer[1]!=0)
	{
		printf("Can Not Connect Destination\n");

		return false;
	}

	return true;
}





void MailDownLoad(SOCKET& s, char* srt)
{
	send(s, srt, strlen(srt), 0);

	char* Buffer = (char*)malloc(sizeof(char)*100);

	int Ret = recv(s, Buffer, 512, 0);

	char SaveDic[256];
	char CurrentDic[256];
	char Temp[12];
	DWORD dwWrite;

	GetCurrentDirectory(256, CurrentDic);

	strcat(CurrentDic, "\\wangxue");

	strcpy(SaveDic, CurrentDic);

	scanf(" %s", Temp);
	
	strcat(SaveDic, "\\");

	strcat(SaveDic, Temp);

	HANDLE hFile = CreateFile("1.txt", GENERIC_WRITE, 0, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	BOOL bRet = WriteFile(hFile, (LPVOID)Buffer, Ret, &dwWrite, NULL);

	if (bRet == TRUE)
	{
		MessageBox(NULL, "Receive File OK", "OK", MB_OK);
	}
}









/*
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
}*/


