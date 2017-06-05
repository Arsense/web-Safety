// Server.cpp: implementation of the CServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Example1.h"
#include "Server.h"
#include "Example1Dlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServer::CServer()
{
	m_hSocket=NULL;
}

CServer::~CServer()
{
	WSAAsyncSelect(m_hSocket, m_hWnd, 0, 0);
}

BOOL CServer::InitAndListen(HWND hwnd,UINT port)
{
	m_uPort=port;
	m_hWnd=hwnd;

	if(m_hSocket != NULL)
    {
        //如果已经创建了套接字先关闭原来的
		closesocket(m_hSocket);
		m_hSocket = NULL;
	}
	if(m_hSocket == NULL)
	{
		//创建新的套接字，这里是创建的流类型的套接字
		m_hSocket = socket(AF_INET, SOCK_STREAM,0);
		ASSERT(m_hSocket != NULL);
		ServerInit();
	}
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	m_addr.sin_port = htons(m_uPort);

	int ret = 0;
	int error = 0;
	//绑定一个套接字到本机的地址
	ret = bind(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));

	if(ret == SOCKET_ERROR)
	{	//绑定错误
		AfxMessageBox("Binding Error");
		return FALSE;
	}
	
	//开始一个侦听过程，等待客户的连接
	ret = listen(m_hSocket, 5);    //这里的第二个参数表示最多支持的客户连接数
	if(ret == SOCKET_ERROR)
	{	//listen失败
		AfxMessageBox("Listen Error");
		return FALSE;
	}

	return TRUE;
}
void CServer::ServerInit()
{
	if(WSAAsyncSelect(m_hSocket, m_hWnd, SER_MESSAGE, FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE)>0)
		AfxMessageBox("error select");
}
