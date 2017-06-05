#include <WINSOCK2.H>
#include <windows.h>
#include <stdio.h>
#include <malloc.h>



//程序需要一个好的数据结构，没有构造好该程序思想清楚

#pragma comment(lib,"Ws2_32")


//定义一个端口号

#define uPort  23
#define ConnectMax 2





//一个结构体包含着用户的信息

typedef struct _USER_ 
{
	char szName[20];     //用户的姓名
	char szInput[256];   //用户输入的消息
	SOCKET sClient;      //与用户通信的套接字
	sockaddr_in UserAddr; //用户的地址
	int    nLen;          //地址的长度
}User,*pUser;


//一个消息的结构体

typedef struct _MESSAGE_
{
	int nUser;
	char cMsg[256];

}MESSAGE,*pMESSAGE;

char CONNECT_MSG[30]={0};
char SERVER_FULL_MSG[30]={0};



bool InitSocket();

void AcceptConnects(SOCKET& s,pUser& pTemp);

void SendUser(int Index,pUser& pTemp,char* Buffer);

void CloseClientSocket(int Index,pUser& pTemp);

void AcceptMessages(pUser& pTemp,pMESSAGE& mMsg);

MESSAGE GetMessage(pMESSAGE& mMsg);

void CloseEmptySockets(pUser& pTemp);

void main()
{

	char Buffer[4096]={0};
	//初始化套接字库
	
	if (!InitSocket())
	{
		printf("Initilize Socket Error\n");

		return;
	}


	//创建一个监听套接字

	SOCKET sListen = socket(AF_INET,SOCK_STREAM,0);

	if (sListen==INVALID_SOCKET)
	{
		printf("Create Listen Error\n");

		return;
	}

	//初始化本地网卡

	sockaddr_in LocalAddr;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(uPort);
	LocalAddr.sin_addr.S_un.S_addr = INADDR_ANY;


	//绑定网卡
	int Ret = bind(sListen,(sockaddr*)&LocalAddr,sizeof(LocalAddr));

	if (Ret==SOCKET_ERROR)
	{
		closesocket(sListen);

		printf("Bind Error\n");
		
		return;
	}

	//监听

	listen(sListen,ConnectMax);


	//申请内存我在这里用了连续的内存没有用链表

	pUser pTemp = (pUser)malloc(sizeof(User)*6);

	if (pTemp==NULL)
	{
		closesocket(sListen);

		printf("Apply Memory Error\n");
		
		return;
	}


	//初始化内存中的信息

	int i = 0;
	for (i=0;i<6;i++)
	{
		pTemp[i].sClient = INVALID_SOCKET;
		memset(pTemp[i].szName,0,20);
		memset(pTemp[i].szInput,0,256);
		pTemp[i].nLen = sizeof(sockaddr_in);
	}




	//我们在这里构建两个数据包一个是通知用户你可以登陆，一个数据包是通知用户登陆以满

	CONNECT_MSG[0] = 0;

	strcat(CONNECT_MSG,"Welcome!");

	char Temp[5]={0};
	Temp[0] = (char)13;
	Temp[1] = (char)10;   //这两个是回车换行
	Temp[2] = 0;

	strcat(CONNECT_MSG,Temp);

	


	SERVER_FULL_MSG[0] = 0;
	
	strcat(SERVER_FULL_MSG,"Full Member Wait...!");
	
	strcat(SERVER_FULL_MSG,Temp);


	//定义一个消息队列的数组并初始化消息队列
	pMESSAGE mMeg = (pMESSAGE)malloc(sizeof(MESSAGE)*10);

	for (i=0;i<10;i++)
	{
		mMeg[i].cMsg[0]=0;
		mMeg[i].nUser = -1;
	}


	MESSAGE mTemp = {0};

	while (1)
	{
		AcceptConnects(sListen,pTemp);   //接受连接


		AcceptMessages(pTemp,mMeg);                //接受传递过来的消息并送到消息队列中



		mTemp = GetMessage(mMeg);       //方法用的啰嗦迷惑陈曦


		if (mTemp.cMsg[0]!=0)
		{
			//在消息的末尾添加
			strcat(mTemp.cMsg,Temp);


			printf("%s",mTemp.cMsg);
		}


		CloseEmptySockets(pTemp);   //释放已经断开的客户端
	}



}

// 关闭空连接
void CloseEmptySockets(pUser& pTemp)
{
	for (int i = 0; i< (ConnectMax + 1); i++)
	{
		SendUser(i,pTemp, "");
	}
}


void AcceptConnects(SOCKET& s,pUser& pTemp)
{
	fd_set fds;

	FD_ZERO(&fds);
	
	FD_SET(s,&fds);

	int iConnect = 0;

	timeval tTime;

	tTime.tv_sec = 0;
	tTime.tv_usec = 0;

	iConnect = select(0,&fds,NULL,NULL,&tTime);

	int i = ConnectMax + 1;     //这里的i是控制如果超过我们的连接数量时第六个人到达时我们也要进行连接并给他发送一条信息

	while ((iConnect)&&(i))
	{
		int j = 0;
		while ((pTemp[j].sClient!=INVALID_SOCKET)&&(j<(ConnectMax + 1)))
		{
			j++;
		}

		if (j<ConnectMax)
		{
			pTemp[j].sClient = accept(s,(sockaddr*)&pTemp[j].UserAddr,(int*)&pTemp[j].nLen);

			if (pTemp[j].sClient!=INVALID_SOCKET)
			{
				SendUser(j,pTemp,&CONNECT_MSG[0]);
			}
		}


		else
		{
			pTemp[j].sClient = accept(s,(sockaddr*)&pTemp[j].UserAddr,(int*)&pTemp[j].nLen);
			
			if (pTemp[j].sClient!=INVALID_SOCKET)
			{
				SendUser(j,pTemp,&SERVER_FULL_MSG[0]);

				CloseClientSocket(j,pTemp);
			}
		}


		iConnect = select(0,&fds,NULL,NULL,&tTime);

		i--;

	}


}



void AcceptMessages(pUser& pTemp,pMESSAGE& mMsg)
{

	int i = 0;
	int j = 0;
	int k = 0;
	int Ret = 0;
	u_long lMsg = 0;

	int nAtoI = 0;

	char* cInput = NULL;

	char cMsg[255]={0};

	char cTemp[2] = {0};
	for (i=0;i<ConnectMax;i++)
	{

		Ret = ioctlsocket(pTemp[i].sClient,FIONREAD,&lMsg);    //在MSDN中看看该函数

		if ((Ret==0)&&(lMsg>0))
		{
			//如果有消息到来我们进行接收

			Ret = recv(pTemp[i].sClient, cMsg, 255, 0);

			if ((Ret != SOCKET_ERROR) && (Ret != 0))
			{
				//解析数据包
				for (j=0;j<(int)strlen(cMsg);j++)
				{
					nAtoI= (int)cMsg[j];

					//这个if中的条件非常关键在MSDN当中查ASCII码表
					if ((cMsg[0] != 0) && (strlen(pTemp[i].szInput) < 255) && (nAtoI >31) 
						&& (nAtoI < 127))
					{
						cInput = &pTemp[i].szInput[0];
						cTemp[0] = cMsg[j];
						cTemp[1] = 0;
						
						strcat(cInput, &cTemp[0]);            //数据包重组
					}


					if (nAtoI == 13)                          //设置消息队列将我们的信息送到消息队列中   
					{
						k = 0;
						while (mMsg[k].cMsg[0] != 0) 
						{ 
							k++; 
						}
						
						mMsg[k].nUser = i;
					
						strcat(&mMsg[k].cMsg[0], &pTemp[i].szInput[0]);
						
						pTemp[i].szInput[0] = 0;                      
					}
				}
				 

			}
		}


	}
}

MESSAGE GetMessage(pMESSAGE& mMsg)
{
	MESSAGE mMessage;
	
	mMessage.nUser = mMsg[0].nUser;
	mMessage.cMsg[0] = 0;
	if (mMsg[0].cMsg[0] != 0)
	{
		strcpy(&mMessage.cMsg[0], &mMsg[0].cMsg[0]);
	}
	
	for (int i = 0; i< 10; i++)
	{
		mMsg[i] = mMsg[i + 1];
	}
	
	return mMessage;
}

void SendUser(int Index,pUser& pTemp,char* Buffer)
{
	int Ret;

	Ret = send(pTemp[Index].sClient,Buffer,strlen(Buffer)+1,0);

	if (Ret==SOCKET_ERROR)
	{
		CloseClientSocket(Index,pTemp);
	}
}


void CloseClientSocket(int Index,pUser& pTemp)
{
	
	shutdown(pTemp[Index].sClient, 0x02);
	closesocket(pTemp[Index].sClient);

	memset(pTemp[Index].szName,0,20);

	memset((void*)&pTemp[Index].UserAddr,0,sizeof(sockaddr_in));


	pTemp[Index].sClient = INVALID_SOCKET;
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



