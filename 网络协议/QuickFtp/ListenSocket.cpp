// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
	m_pWndServer = NULL;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

//接受等待的请求连接
void CListenSocket::OnAccept(int nErrorCode) 
{
	// 新的连接建立
	CSocket socket;//用于数据传输的套接字!!!!!

	// 用一个临时的CSocket对象接收连接
	Accept(socket);

	// 创建一个新的线程来处理连接
	CConnectThread* pThread = (CConnectThread*)AfxBeginThread(RUNTIME_CLASS(CConnectThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (!pThread)
	{
		socket.Close();
		TRACE("Could not create thread\n");
		return;
	}

	CFTPServer *pWnd = (CFTPServer *)m_pWndServer;

    // 添加这个线程到列表中
	pWnd->m_CriticalSection.Lock();
    pWnd->m_ThreadList.AddTail(pThread);
	pWnd->m_CriticalSection.Unlock();

	// 保存指针
	pThread->m_pWndServer = m_pWndServer;

	// 把套接字句柄传送给这个线程
	pThread->m_hSocket = socket.Detach();//将临时的套接字从CSocket对象中分离出来，赋值到m_hSocket中，在线程类的m_ConnectSocket
	//对象要用到

	// 启动线程
	pThread->ResumeThread();

	CAsyncSocket::OnAccept(nErrorCode);
}

