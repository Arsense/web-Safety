// Socks5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <iostream>
#include <map>
#include <string>
using namespace std;

map<string,string> DNS;

#define MAX_QUEUE 10
#define MAX_SIZE 80
#define SERVER_gListenSocket 1130
volatile int servicegListenSocket = 2000;
int gListenSocket=1130;
char g_User[100]="cs";
char g_Password[100]="cs";

enum CMDTYPE 
{
  CONNECT,BIND,UDP
};
struct ServiceStruct
{
    int  tcpsock;
    int  udpsock;
    sockaddr_in clientadd;
    in_addr  clientip;
    unsigned short clientgListenSocket;
    unsigned short servicegListenSocket;//收发都用这个～

    CMDTYPE cmdtype;
    int svctcp_client;
    int svctcp_server;
};
in_addr GetName(const char* name)
{
    hostent* he = gethostbyname(name);
    return *((in_addr*)he->h_addr);
}
void GetHostIP(char* buf)
{
    int ret;
    char name[100];
    ret = gethostname(name,1024);
    if (ret != 0)
	{
		cout<<"gethostname() Error!"<<endl;
        return;
    }
    in_addr hostadd = GetName(name);
    buf[0] = hostadd.S_un.S_un_b.s_b1;
    buf[1] = hostadd.S_un.S_un_b.s_b2;
    buf[2] = hostadd.S_un.S_un_b.s_b3;
    buf[3] = hostadd.S_un.S_un_b.s_b4;
}

bool InitUDPSocket(ServiceStruct& svc)
{
    sockaddr_in serveradd;
    svc.udpsock = socket(AF_INET,SOCK_DGRAM,0);
    if (svc.udpsock == INVALID_SOCKET)
	{
        cout<<"UDP Socket Error!"<<endl;
        return false;
    }
    serveradd.sin_family = AF_INET;
    serveradd.sin_port   = htons(svc.servicegListenSocket);
    serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(svc.udpsock,(sockaddr*)&serveradd,sizeof(serveradd)) < 0)
	{
        cout<<"Bind Error!"<<endl;
        return false;
    }
	return true;
}

bool InitTCPSocket(int& tcpsock)
{
    tcpsock = socket(AF_INET,SOCK_STREAM,0);
    if (tcpsock == INVALID_SOCKET)
	{
        cout<<"TDP Socket Error";
        return false;
    }
    return true;
}
void SendBuffer(int sock,char* buf,int len)
{
    if (!len) return;
    int left = len;
    while (left-=send(sock,buf+(len-left),left,0));
}
void ReceiveBuffer(int sock,char* buf,int len)
{
    if (!len) return;
	int left = len;
    while (left-=recv(sock,buf+(len-left),left,0));
}
void WriteBuffer(FILE* fp,char* buf,int len)
{
    int left = len;
    while (left-=fwrite(buf+(len-left),sizeof(char),left,fp));
}
DWORD WINAPI TCPServerToClient(void* param) //server to client
{
    ServiceStruct* psvc = (ServiceStruct*)param;
    char buf[1024*4];
    int count;
    while (true)
	{
        count = recv(psvc->svctcp_server,buf,1024*4,0);
        SendBuffer(psvc->svctcp_client,buf,count);
    }
}
DWORD WINAPI TCPClientToServer(void* param) //client to server
{
    ServiceStruct* psvc = (ServiceStruct*)param;
    char buf[1024*4];
    int count;
    while (true)
	{
        count = recv(psvc->svctcp_client,buf,1024*4,0);
        SendBuffer(psvc->svctcp_server,buf,count);
    }
}
DWORD WINAPI TCPProxyThread(void* param)
{
    ServiceStruct* psvc = (ServiceStruct*)param;
	void* hThread1 = CreateThread(NULL, 0, TCPClientToServer,param, 0, 0);
	if (hThread1 == NULL)
	{
		cout<<"CreateThread() Error!"<<endl;
		return 1;
	}
	void* hThread2 = CreateThread(NULL, 0, TCPServerToClient, param, 0, 0);
	if (hThread2 == NULL)
	{
		cout<<"CreateThread() Error!"<<endl; 
		return 1;
	}
	SuspendThread(GetCurrentThread());
	return 0;
}

bool Authentication(int sock)
{
	cout << "Authentication begin...." << endl;
    /*
   +----+----------+----------+
   |VER | NMETHODS | METHODS  |
   +----+----------+----------+
   | 1  |    1     | 1 to 255 |
   +----+----------+----------+
    */
	int i = 0;
    char buf[1024];
    ReceiveBuffer(sock,buf,2);
    int number_of_method = buf[1];
    ReceiveBuffer(sock,buf,number_of_method);
    cout << "number of method to choose : "<< number_of_method << endl;
    for (i = 0;i < number_of_method;++i)
        if (buf[i] == 0x02) //user & pass method
            break;
  /*
   +----+-----------------+
   |VER | METHOD CHOSSED  |
   +----+-----------------+
   | 1  |    1 to 255     |
   +----+-----------------+
    */
    char reply[2];
    reply[0] = 0x05;
    reply[1] = (i == number_of_method)?0xff:0x02;
    SendBuffer(sock,reply,2);
    if (i == number_of_method) 
		return false;
	/*
	+----+------+----------+------+----------+
	|VER | ULEN |  UNAME   | PLEN |  PASSWD  |
	+----+------+----------+------+----------+
	| 1  |  1   | 1 to 255 |  1   | 1 to 255 |
	+----+------+----------+------+----------+
	*/
	ReceiveBuffer(sock,buf,2);
	int ulen = buf[1];
	//cout << ulen << endl;
	ReceiveBuffer(sock,buf,ulen);
	buf[ulen] = 0;
	char username[256];
	strcpy(username,buf);
	ReceiveBuffer(sock,buf,1);
	int plen = buf[0];
	ReceiveBuffer(sock,buf,plen);
	buf[plen] = 0;
	char password[256];
	strcpy(password,buf);
	cout << "username = " << username << endl;
	cout << "password = " << password << endl;
	/*
	+----+--------+
	|VER | STATUS |
	+----+--------+
	| 1  |   1    |
	+----+--------+
	*/
	reply[0] = 0x05;
	if (!strcmp(username,g_User) && !strcmp(password,g_Password))
	reply[1] = 0x00;
	else
	reply[1] = 0x01;
	SendBuffer(sock,reply,2);
	return reply[1] == 0x00;
}

void GetRequest(ServiceStruct& svc)
{
    /*
     +----+-----+-------+------+----------+----------+
   |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.gListenSocket |
   +----+-----+-------+------+----------+----------+
   | 1  |  1  | X'00' |  1   | Variable |    2     |
   +----+-----+-------+------+----------+----------+
    */
    char buf[1024];
    ReceiveBuffer(svc.tcpsock,buf,10);
    cout << "THE CMD : ";
    switch (buf[1])
	{
        case 0x01:
            cout << "CONNECT" << endl;
            svc.cmdtype = CONNECT;
            break;
        case 0x02:
            cout << "BIND" << endl;
            svc.cmdtype = BIND;
            break;
        case 0x03:
            cout << "UDP ASSOCIATE" << endl;
            svc.cmdtype = UDP;
            break;
    }
    //I assumed that the client send its ip
    if (buf[3] == 0x01)
	{
        if (svc.cmdtype == UDP)
		{
            cout << "THE Client IP : ";
            cout << (buf[4]&0xff) << "." << (buf[5]&0xff) << "."<< (buf[6]&0xff) << "." << (buf[7]&0xff) << endl;
            svc.clientip.s_addr =MAKELONG(MAKEWORD((buf[4]&0xff),(buf[5]&0xff)),
					MAKEWORD((buf[6]&0xff),(buf[7]&0xff)));
            cout << "THE CLIENT gListenSocket :";
            cout << ((int)buf[8])*256 + (unsigned char)buf[9] << endl;
            svc.clientgListenSocket = ((int)buf[8])*256 + (unsigned char)buf[9];
            memset(&svc.clientadd,0x00,sizeof(sockaddr_in));
            svc.clientadd.sin_family = AF_INET;
            svc.clientadd.sin_port   = htons(svc.clientgListenSocket);
            svc.clientadd.sin_addr.s_addr = inet_addr("10.111.171.105");
        }
        else if (svc.cmdtype == CONNECT)
		{
            cout << "THE DESTINATION IP : ";
            cout << (buf[4]&0xff) << "." << (buf[5]&0xff) << "."
                     << (buf[6]&0xff) << "." << (buf[7]&0xff) << endl;
            cout << "THE DESTINATION gListenSocket : ";
            cout << ((int)buf[8])*256 + (unsigned char)buf[9] << endl;
            InitTCPSocket(svc.svctcp_server);
            sockaddr_in serveraddr;
            memset(&serveraddr,0x00,sizeof(sockaddr_in));
            serveraddr.sin_family = AF_INET;
            serveraddr.sin_port = ((int)buf[8])*256 + (unsigned char)buf[9];
	        serveraddr.sin_addr.s_addr =
				MAKELONG(MAKEWORD((buf[4]&0xff),(buf[5]&0xff)),
                      MAKEWORD((buf[6]&0xff),(buf[7]&0xff))) ;
          if(connect(svc.svctcp_server,(sockaddr*)&serveraddr,sizeof(serveraddr)) ==SOCKET_ERROR)
		  {
            cerr << "connect remote host failed!" << endl;
            return;
          }
      }
      else if (svc.cmdtype == BIND){
        //not implemented yet;-(
      }
    }
}
void ReplyRequest(ServiceStruct& svc)
{
    /*
      +----+-----+-------+------+----------+----------+
    |VER | REP |  RSV  | ATYP | BND.ADDR | BND.gListenSocket |
    +----+-----+-------+------+----------+----------+
    | 1  |  1  | X'00' |  1   | Variable |    2     |
    +----+-----+-------+------+----------+----------+
    */
    char reply[1024];
    reply[0] = 0x05;
    reply[1] = 0x00;
    reply[2] = 0x00;
    reply[3] = 0x01;
    GetHostIP(reply+4);
    servicegListenSocket++;
    svc.servicegListenSocket = servicegListenSocket;
    reply[8] = svc.servicegListenSocket/256;
    reply[9] = svc.servicegListenSocket%256;
    cout << "SERVER gListenSocket:" << svc.servicegListenSocket << endl;
    SendBuffer(svc.tcpsock,reply,10);

    if (svc.cmdtype == CONNECT)
	{
        InitTCPSocket(svc.svctcp_client);
        sockaddr_in serveraddr;
        memset(&serveraddr,0x00,sizeof(sockaddr_in));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port   = htons(svc.servicegListenSocket);
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(svc.svctcp_client,(sockaddr*)&serveraddr,sizeof(serveraddr)) <0)
		{
			cout<<"Bind Error!"<<endl;
            return;
        }
        if (listen(svc.svctcp_client,MAX_QUEUE) < 0)
		{
			cout<<"Listen Error!"<<endl;
            return;
        }
        void* hThread = CreateThread(NULL, 0, TCPProxyThread,(void*)&svc, 0, 0);
		if (hThread == NULL)
		{
			cout<<"CreateThread() Error!"<<endl;
			return;
		}
    }
}
void UDPRequest(ServiceStruct& svc)
{
    /*
      +----+------+------+----------+----------+----------+
    |RSV | FRAG | ATYP | DST.ADDR | DST.gListenSocket |   DATA   |
    +----+------+------+----------+----------+----------+
    | 2  |  1   |  1   | Variable |    2     | Variable |
    +----+------+------+----------+----------+----------+
    */
    int i,j;
    sockaddr_in desireadd,sourceadd;
    desireadd.sin_family = AF_INET;
    //sourceadd.sin_family = AF_INET;
    int socklen = sizeof(sourceadd);
    int data_start_pos = 0;
    char buf[1024*4];
    int count=recvfrom(svc.udpsock,buf,1024*4,0,(sockaddr*)&sourceadd,&socklen);
    //int count = recvfrom(svc.udpsock,buf,1024*4,0,0,0);
    if (count == SOCKET_ERROR)
	{
        cout << "Recvfrom() Error!" << endl;
        return;
    }
    buf[count] = 0;
	//通过端口判断来源
	if (sourceadd.sin_port == svc.clientadd.sin_port)
	{
		int atyp = buf[3];
		cout << "THE ADDRESS TYPE = " << atyp << endl;
		if (atyp == 0x01)
		{
            cout << "THE DESIRE IP : ";
            cout << (buf[4]&0xff) << "." << (buf[5]&0xff) << "."
                     << (buf[6]&0xff) << "." << (buf[7]&0xff) << endl;
            desireadd.sin_addr.s_addr =MAKELONG(MAKEWORD((buf[4]&0xff),(buf[5]&0xff)),
					MAKEWORD((buf[6]&0xff),(buf[7]&0xff)));;
            cout << "THE DISIRE gListenSocket : ";
            cout << (buf[8]&0xff)*256 + (unsigned char)buf[9] << endl;
            desireadd.sin_port  = htons((buf[8]&0xff)*256 + (unsigned char)buf[9]);
            data_start_pos = 10;
        }
        else if (atyp == 0x03)
		{
            int domainname_length = buf[4]&0xff;
            char domainname[100];
            cout << "THE DESIRE DOMAINNAME :";
            for (i = 0;i < domainname_length;++i)
                 domainname[i] = buf[i+5];
            domainname[i] = 0;
            cout << domainname << endl;
            desireadd.sin_addr = GetName(domainname);
            DNS[string(inet_ntoa(desireadd.sin_addr))] = string(domainname);
            i += 5;
            cout << "THE DISIRE gListenSocket :";
            cout << (buf[i]&0xff)*256 + (unsigned char)buf[i+1] << endl;
            desireadd.sin_port = htons((buf[i]&0xff)*256 + (unsigned char)buf[i+1]);
            data_start_pos = i + 2;
      }
	else if (atyp == 0x04){
        //ipv6 not implement:)
        }
		count -= data_start_pos;
		sendto(svc.udpsock,buf+data_start_pos,count,0,(sockaddr*)&desireadd,sizeof(desireadd));
    }
    else
	{
		//封装这个消息
		cout << "GOT MESSAGE FROM : ";
		cout << inet_ntoa(sourceadd.sin_addr);
		cout << ":" << ntohs(sourceadd.sin_port) << endl;
		char reply[1024*4];
		if (DNS.find(string(inet_ntoa(sourceadd.sin_addr))) == DNS.end())
		{
			reply[0] = reply[1] = reply[2] = 0;
			reply[3] = 0x01;
			reply[4] = sourceadd.sin_addr.S_un.S_un_b.s_b1;
			reply[5] = sourceadd.sin_addr.S_un.S_un_b.s_b2;
			reply[6] = sourceadd.sin_addr.S_un.S_un_b.s_b3;
			reply[7] = sourceadd.sin_addr.S_un.S_un_b.s_b4;
			reply[8] = ntohs(sourceadd.sin_port)/256;
			reply[9] = ntohs(sourceadd.sin_port)%256;
			for (i = 0;i < count;++i)
				reply[10+i] = buf[i];
			sendto(svc.udpsock,reply,10+count,0,(sockaddr*)&svc.clientadd,sizeof(sockaddr));
		}
		else
		{
			reply[0] = reply[1] = reply[2] = 0;
			reply[3] = 0x03;
			string domainname = DNS[string(inet_ntoa(sourceadd.sin_addr))];
			cout << "THE DOMAINNAME = " << domainname << endl;
			reply[4] = domainname.size();
			for (i = 0;i < domainname.size();++i)
				reply[5+i] = domainname[i];
			sendto(svc.udpsock,reply,5+domainname.size(),0,(sockaddr*)&svc.clientadd,sizeof(svc.clientadd));
			count =	sendto(svc.udpsock,buf,count,0,(sockaddr*)&svc.clientadd,sizeof(svc.clientadd));
			cout << "actually reply = " << count << endl;
	    }
	}
}

DWORD WINAPI WorkingThread(void* param)
{
    ServiceStruct svc;
    int sock = (int)param;
    svc.tcpsock = sock;
    if (Authentication(svc.tcpsock))
	{
        GetRequest(svc);
        ReplyRequest(svc);
    }
    int udpsock;
    InitUDPSocket(svc);
    while (true)
        UDPRequest(svc);
    closesocket(sock);
    return 0;
}

int main(int argc, char* argv[])
{
    //init module
	WSADATA wsaData;
	int wVersionRequested = MAKEWORD( 2, 2 );
	int err = WSAStartup( wVersionRequested, &wsaData );

	int sock,clientsock;
	sockaddr_in serveradd,clientadd;
	sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0)
	{
		cout<<"Server Socket Error!"<<endl;
		return 1;
    }
	memset(&serveradd,0x00,sizeof(sockaddr_in));
	serveradd.sin_family = AF_INET;
	cout<<"请输入代理服务端口号:";
	cin>>gListenSocket;
	cout<<endl;
	serveradd.sin_port = htons(gListenSocket);
	serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock,(sockaddr*)&serveradd,sizeof(serveradd))<0)
	{
		cout<<"Bind error!"<<endl;
		return 1;
    }
	if(listen(sock,MAX_QUEUE) < 0)
	{
		cout<<"Listen Error!"<<endl;
		return 1;
    }
    while (true)
	{
        int adrlen;
		cout<<"Accepting ......!"<<endl;
		while((clientsock = accept(sock,(sockaddr*)&clientadd,&adrlen))<0);
        //为客户端创建一个工作线程
		void* hThread = CreateThread(NULL, 0, WorkingThread,(void*)clientsock, 0, 0);
		if (hThread == NULL)
		{
			cout<<"CreateThread() Error!"<<endl;
			break;
		}
	}
	closesocket(sock);
	WSACleanup();
}
