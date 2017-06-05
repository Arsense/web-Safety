// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Example1.h"
#include "Client.h"
#include "Example1Dlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient()
{
	m_hSocket =NULL;
}

CClient::~CClient()
{

}
BOOL CClient::InitAndConnet(HWND hwnd,UINT port,CString strserver)
{
	m_hWnd=hwnd;
	m_uPort=port;
	m_strServer=strserver;
	if(m_hSocket != NULL)
	{
		//如果原来打开这套接字先关闭
		closesocket(m_hSocket);
		m_hSocket = NULL;
	}
	if(m_hSocket == NULL)
	{
		//创建新的流套接字
		m_hSocket = socket(AF_INET, SOCK_STREAM,0);
		ASSERT(m_hSocket != NULL);
		ClientInit();
	}

	//准备服务器的信息，这里需要指定服务器的地址
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = inet_addr(m_strServer.GetBuffer(0));
	m_addr.sin_port = htons(m_uPort);   //改变端口号的数据格式
	
	//这里主动连接服务器，该过程将等待一定时间
	int ret = 0;
	int error = 0;
	ret = connect(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));
	if(ret == SOCKET_ERROR)
	{//连接失败
		if(GetLastError()!=WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("请确认服务器确实已经打开并工作在同样的端口！"));
			return FALSE;
		}
	}
	return TRUE;
}

void CClient::SendString(CString a)
{
	if(send(m_hSocket,a.GetBuffer(0),a.GetLength(),0)==SOCKET_ERROR)
	{
		AfxMessageBox("Client Send data error");
	}
}

void CClient::GetString(CString &str)
{
	recv(m_hSocket,str.GetBuffer(0),1024,MSG_DONTROUTE);
}

void CClient::ClientInit()
{
	if(WSAAsyncSelect(m_hSocket,m_hWnd,CLI_MESSAGE,FD_READ|FD_WRITE|FD_CLOSE|FD_CONNECT)>0)
	{
		AfxMessageBox("Error in select");
	}
}
