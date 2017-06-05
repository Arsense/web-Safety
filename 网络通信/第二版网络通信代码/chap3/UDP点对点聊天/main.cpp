
#include "CComm.h"
#include <stdio.h>

int main( int argc, char *argv[] )
{
	char buffer[4096];		// buffer we'll use to store msg read in from stdin
	short listeningport;	 // port to listen on
	short destport;			  // port to send to
	char *desthost;			// address of destination machine

	printf("初始化...\n");

	// 设定命令行参数
	// Make sure data passed on command line is correct
	//	if( argc != 4 || !(listeningport = atoi(argv[1])) 
	//		          || !(destport = atoi(argv[3])))
	//	{
	//		printf("使用方法: SChat [listen port number] [destination machine] [destination port]\n");
	//		return 0;
	//	}

	//设定UDP绑定端口
	listeningport=200;
	//设定发送端口
	destport=100;
	desthost = "192.168.0.3";//设定目标地址
	CComm myComm;		
	if (! myComm.Listen(listeningport))	// 绑定地址
	{
		printf("端口 %s绑定失败\n", listeningport);	
		return 0;
	}

	printf("聊天程序成功建立，端口为200: <Ctrl-C> 退出\n\n");	// 

	while( fgets(buffer, sizeof(buffer), stdin) )	// 获得输入数据
	{
		myComm.SendMsg( buffer, strlen(buffer), desthost, destport );	// 发送数据到目标地址
	}
	return 0;
}