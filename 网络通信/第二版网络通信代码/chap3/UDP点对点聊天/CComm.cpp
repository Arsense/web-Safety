

#include "CComm.h"


CComm::CComm()
{
	//构造函数
	ListenSocket = INVALID_SOCKET;	// 开始设置为INVALID_SOCKET
	
	#ifdef _WIN32	// 如果是win32系统
		WORD VersionRequested = MAKEWORD(1,1);
		WSADATA wsaData;
		
		WSAStartup(VersionRequested, &wsaData);	// 启动winsock服务
		if ( wsaData.wVersion != VersionRequested )
		{
			printf("Wrong version or WinSock not loaded\n");
			fflush(0);	
		}
	#endif
}

//析构函数
CComm::~CComm()
{
	if ( ListenSocket != INVALID_SOCKET )
		closesocket( ListenSocket );	// 如果已经创建、则关闭

	#ifdef _WIN32	// 调用WSACleanup
		WSACleanup();
	#endif
}

bool CComm::SendMsg( char *Msg, int Len, char *host, short port )
{
	signed int Sent;
	hostent *hostdata;
	if ( atoi(host) )	// 是否IP地址为标准形式
	{
		u_long ip = inet_addr( host );
		hostdata = gethostbyaddr( (char *)&ip, sizeof(ip), PF_INET );
	}
	else	// 否则则可能是机器名
	{
		hostdata = gethostbyname( host );
	}

	if ( !hostdata )
	{
		printf("获得机器名错误\n");
		fflush(0);
		return false;
	}

	sockaddr_in dest;	// 发送目标地址
	dest.sin_family = PF_INET;
	dest.sin_addr = *(in_addr *)(hostdata->h_addr_list[0]);
	dest.sin_port = htons( port );
	printf("信息已经被发送到主机 %s 端口为 %i\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
	//数据发送
	Sent = sendto(ListenSocket, Msg, Len, 0, (sockaddr *)&dest, sizeof(sockaddr_in));
	
	if ( Sent != Len )
	{
		printf("错误发送UDP信息\n");
		fflush(0);
		return false;
	}
	
	return true;
}

void *CComm::ListenThread( void *data )
{
	char buf[4096];
	CComm *Comm = (CComm *)data;
	int len = sizeof(Comm->client);
	while(1)	// 一直循环
	{
		//接收数据
		int result = recvfrom( Comm->ListenSocket, buf, sizeof(buf)-1, 0, (sockaddr *)&Comm->client, (socklen_t *)&len);
		if ( result > 0 )
		{
			buf[result] = 0;
			printf("Message received from host %s port %i\n", inet_ntoa(Comm->client.sin_addr), ntohs(Comm->client.sin_port));
			printf(">> %s", buf);
			fflush(0);
		}	
	}	
}


//地址绑定，注意在UDP协议中，不需要listen，这里函数listen只是绑定一个端口
bool CComm::Listen( int PortNum )
{
	ListenSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if ( ListenSocket == INVALID_SOCKET )
	{
		printf("Error: socket创建失败\n");
		fflush(0);
		return false;
	}

	srv.sin_family = PF_INET;
	srv.sin_addr.s_addr = htonl( INADDR_ANY );	// 任何地址
	srv.sin_port = htons( PortNum );

	if ( bind( ListenSocket, (struct sockaddr *)&srv, sizeof(srv)) != 0 )
	{
		printf("Error: 绑定失败\n");
		fflush(0);
		closesocket( ListenSocket );
		return false;
	}

	int ThreadID;	// 线程id
	
		DWORD thread;
		//调用createthread创建线程
		ThreadID = (int)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(CComm::ListenThread), (void *)this, 0, &thread);
		ThreadID = ThreadID ? 0 : 1;	// 如果成功，则返回为0

	if(ThreadID)	// ThreadID如果不为0，则线程创建失败
	{
		printf("线程创建失败\n");
		return false;
	}
	else
		return true;
}
