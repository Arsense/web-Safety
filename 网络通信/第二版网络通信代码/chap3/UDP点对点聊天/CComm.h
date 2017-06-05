
#ifndef _CCOMM_H_
#define _CCOMM_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <winsock.h>
#define socklen_t	int
#pragma comment(lib, "wsock32.lib")

class CComm
{
private:
	static void *ListenThread(void *data);
	SOCKET ListenSocket;	// 等待接收数据的socket
	sockaddr_in srv;			// 绑定地址
	sockaddr_in client;			// 发送数据过来的地址

public:
	CComm();
	~CComm();

	bool SendMsg(char *Msg, int Len, char *host, short port);
	bool Listen(int PortNum);

};

#endif	// #define _CCOMM_H_