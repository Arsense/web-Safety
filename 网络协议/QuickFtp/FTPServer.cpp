// FTPServer.cpp : implementation file
//

#include "stdafx.h"
#include "FTPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFTPServer

CFTPServer::CFTPServer()
{
	m_nPort = 21;
	m_nMaxUsers = 10;
	m_strWelcomeMessage = "Welcome to Bai's FTP Server";
	m_strGoodbyeMessage = "Bye";
	m_nTimeout = 5;
	m_bRunning = FALSE;
	m_hWnd = NULL;
	m_nConnectionCount = 0;


	m_nStatisticsInterval = 0;

	// load users
	m_UserManager.Serialize(FALSE);
}

CFTPServer::~CFTPServer()
{
}


BEGIN_MESSAGE_MAP(CFTPServer, CWnd)
	//{{AFX_MSG_MAP(CFTPServer)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADSTART, OnThreadStart)
	ON_MESSAGE(WM_THREADCLOSE, OnThreadClose)
	ON_MESSAGE(WM_THREADMSG, OnThreadMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFTPServer message handlers

BOOL CFTPServer::Start()
{
	if(m_bRunning)
		return FALSE;
	if(!CWnd::CreateEx(0,AfxRegisterWndClass(0),"FTP Server Notification Sink",WS_POPUP,0,0,0,0,NULL,0))
	{
		AddTraceLine(0,"Failed to create notification window.");
		//AfxMessageBox(_T("Failed to create notification window."));
		return FALSE;
	}
	
	if(m_ListenSocket.Create(m_nPort))
	{
		if(m_ListenSocket.Listen())
		{
			m_ListenSocket.AsyncSelect();//没有这行代码，程序还是可以很好的运行
			m_ListenSocket.m_pWndServer = this;
			m_bRunning = TRUE;
			
            //设置时间，多长时间更新一次统计页面
			SetTimer(1,m_nStatisticsInterval,NULL);  //这个可以不需要，在主对话框调用SetStatisticsInterval()函数时会进行一个时钟的设置
			AddTraceLine(0,"FTP Server started on port %d.",m_nPort);
			//AfxMessageBox(_T("FTP Server started on port 21!"));
			return TRUE;
		}
	}
	
	AddTraceLine(0,"FTP Server failed to listen on port %d.",m_nPort);
	//AfxMessageBox(_T("FTP Server failed to listen on port 21!"));
	//调试了好久，就是不知为何客户端来连接时没有自动触发m_ListenSocket对象的OnAccept()函数。
	//原因在于这句弹出对话框后，我点击了确定，等于说这个函数执行完了，m_ListenSocket被销毁了，没有监听的SOCKET了。
	//主要原因还是我在开启服务器按钮的响应函数中添加了下面的代码，下面的代码都是临时对象，执行完会被销毁
	//CFTPServer ftpserver;
	//ftpserver.Start();
	//如果把CFTPServer ftpserver;定义在开启服务器按钮的响应函数之外，即全局变量，程序执行完才会被销毁。
	//在开启服务器按钮的响应函数中调用这句theServer.Start();
	
	if(IsWindow(m_hWnd))
		DestroyWindow();
	m_hWnd = NULL;
	return FALSE;
}

//停止FTP服务器
void CFTPServer::Stop()
{
	if (!m_bRunning)
		return;

	// 停止统计计时器
	KillTimer(1);

	m_bRunning = FALSE;	
	m_ListenSocket.Close();

	CConnectThread* pThread = NULL;

	//关闭所有线程
	do
	{
		m_CriticalSection.Lock();

		POSITION pos = m_ThreadList.GetHeadPosition();
		if (pos != NULL)
		{
			pThread = (CConnectThread *)m_ThreadList.GetAt(pos);
		
			m_CriticalSection.Unlock();

			// 保存线程
			int nThreadID = pThread->m_nThreadID;
			HANDLE hThread = pThread->m_hThread;

			AddTraceLine(0, "[%d] Shutting down thread...", nThreadID);

			// 通知线程停止
			pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
			pThread->PostThreadMessage(WM_QUIT,0,0);

			//等待线程终止
			if (WaitWithMessageLoop(hThread, 5000) == FALSE)
			{
				// 线程不能终止
				AddTraceLine(0, "[%d] Problem while killing thread.", nThreadID);
				//清除线程
				m_CriticalSection.Lock();
				POSITION rmPos = m_ThreadList.Find(pThread);
				if (rmPos != NULL)
					m_ThreadList.RemoveAt(rmPos);
				m_CriticalSection.Unlock();
			}
			else
			{
				AddTraceLine(0, "[%d] Thread successfully stopped.", nThreadID);
			}
		}
		else
		{
			m_CriticalSection.Unlock();	
			pThread = NULL;
		}
	}
	while (pThread != NULL);
    //更新服务器状态
	AddTraceLine(0, "FTP Server stopped.");

	if (IsWindow(m_hWnd))
		DestroyWindow();

	m_hWnd = NULL;
}

//FTP status change.	
void CFTPServer::AddTraceLine(int nType, LPCTSTR pstrFormat, ...)
{
	CString str;

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	m_pEventSink->OnFTPStatusChange(nType, str);
}

/********************************************************************/
/*																	*/
/* Function name : Initialize										*/
/* Description   : Initialize event sink.							*/
/*																	*/
/********************************************************************/
void CFTPServer::Initialize(CFTPEventSink *pEventSink)
{
	m_pEventSink = pEventSink;
}

//Called when a new thread has started.
//自定义消息WM_THREADSTART的响应函数
LRESULT CFTPServer::OnThreadStart(WPARAM wParam, LPARAM)
{
	m_nConnectionCount++;
	m_nTotalConnections++;
	CConnectThread *pThread = (CConnectThread *)wParam;

	UINT port;

	pThread->m_ConnectSocket.GetPeerName(pThread->m_strRemoteHost, port);
	AddTraceLine(0, "[%d] Client connected from %s.", pThread->m_nThreadID, pThread->m_strRemoteHost);

	return TRUE;
}

//Called when a thread is about to stop.
//自定义消息WM_THREADCLOSE的响应函数
LRESULT CFTPServer::OnThreadClose(WPARAM wParam, LPARAM lParam)
{
	m_nConnectionCount--;
	CConnectThread *pThread = (CConnectThread *)wParam;

	AddTraceLine(0, "[%d] Client disconnected from %s.", pThread->m_nThreadID, pThread->m_strRemoteHost);
	
	m_pEventSink->OnFTPUserDisconnected(pThread->m_nThreadID, pThread->m_ConnectSocket.m_strUserName);
	return TRUE;
}

//Message sent from connect connection.
////自定义消息WM_THREADMSG的响应函数
LRESULT CFTPServer::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 0:
			m_dwTotalSentBytes += (int)lParam;
			break;
		case 1:
			m_dwTotalReceivedBytes += (int)lParam;
			break;
		case 2:
			switch(lParam)
			{
				case FTPSTAT_DOWNLOADSUCCEEDED:
					m_nFilesDownloaded++;
					break;
				case FTPSTAT_UPLOADSUCCEEDED:
					m_nFilesUploaded++;
					break;
				case FTPSTAT_DOWNLOADFAILED:
					m_nFailedDownloads++;
					break;
				case FTPSTAT_UPLOADFAILED:
					m_nFailedUploads++;
					break;
			}
			break;
		default:
			
			break;
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : SetWelcomeMessage								*/
/* Description   : Set welcome message								*/
/*																	*/
/********************************************************************/
void CFTPServer::SetWelcomeMessage(LPCTSTR lpszText)
{
	m_strWelcomeMessage = lpszText;
}


/********************************************************************/
/*																	*/
/* Function name : SetGoodbyeMessage								*/
/* Description   : Set goodbye message								*/
/*																	*/
/********************************************************************/
void CFTPServer::SetGoodbyeMessage(LPCTSTR lpszText)
{
	m_strGoodbyeMessage = lpszText;
}

/********************************************************************/
/*																	*/
/* Function name : SetMaxUsers										*/
/* Description   : Set maximum number of users						*/
/*																	*/
/********************************************************************/
void CFTPServer::SetMaxUsers(int nValue)
{
	m_nMaxUsers = nValue;
}


/********************************************************************/
/*																	*/
/* Function name : SetPort											*/
/* Description   : Set listening port for new connections			*/
/*																	*/
/********************************************************************/
void CFTPServer::SetPort(int nValue)
{
	m_nPort = nValue;
}


/********************************************************************/
/*																	*/
/* Function name : SetTimeout										*/
/* Description   : Set connection timeout							*/
/*																	*/
/********************************************************************/
void CFTPServer::SetTimeout(int nValue)
{
	m_nTimeout = nValue;
}

/********************************************************************/
/*																	*/
/* Function name : OnTimer											*/
/* Description   : Update statictics.								*/
/*																	*/
/********************************************************************/
void CFTPServer::OnTimer(UINT nIDEvent) 
{
	// update statictics 
	if (nIDEvent == 1)
	{
		m_pEventSink->OnFTPSentBytesChange(m_dwTotalSentBytes);
		m_pEventSink->OnFTPReceivedBytesChange(m_dwTotalReceivedBytes);
		m_pEventSink->OnFTPStatisticChange(0, m_nTotalConnections);
		m_pEventSink->OnFTPStatisticChange(1, m_nConnectionCount);
		m_pEventSink->OnFTPStatisticChange(2, m_nFilesDownloaded);
		m_pEventSink->OnFTPStatisticChange(3, m_nFilesUploaded);
		m_pEventSink->OnFTPStatisticChange(4, m_nFailedDownloads);
		m_pEventSink->OnFTPStatisticChange(5, m_nFailedUploads);
	}
	CWnd::OnTimer(nIDEvent);
}

/********************************************************************/
/*																	*/
/* Function name : SetTimeout										*/
/* Description   : Set connection timeout							*/
/*																	*/
/********************************************************************/
void CFTPServer::SetStatisticsInterval(int nValue)
{
	m_nStatisticsInterval = nValue;
	//下面的代码走到KillTimer()时总是报错！
	/*if (m_nStatisticsInterval != 0)
	{
		KillTimer(1);
		SetTimer(1, nValue, NULL);
	}
	else
	{
		KillTimer(1);
	}*/
}

/********************************************************************/
/*																	*/
/* Function name : CheckMaxUsers									*/
/* Description   : Reached maximum number of connections?			*/
/*																	*/
/********************************************************************/
BOOL CFTPServer::CheckMaxUsers()
{
	if (m_nConnectionCount > m_nMaxUsers)
		return TRUE;
	else
		return FALSE;
}