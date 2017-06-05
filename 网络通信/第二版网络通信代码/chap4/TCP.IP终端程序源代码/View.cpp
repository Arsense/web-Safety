// view.cpp : implementation of the CTermView class
//

#include "stdafx.h"
#include "wsterm.h"

#include "doc.h"
#include "termsock.h"
#include "view.h"
#include "connectd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTermView

IMPLEMENT_DYNCREATE(CTermView, CEditView)

BEGIN_MESSAGE_MAP(CTermView, CEditView)
	//{{AFX_MSG_MAP(CTermView)
	ON_COMMAND(ID_SOCKET_CONNECT, OnSocketConnect)
	ON_UPDATE_COMMAND_UI(ID_SOCKET_CONNECT, OnUpdateSocketConnect)
	ON_COMMAND(ID_SOCKET_CLOSE, OnSocketClose)
	ON_UPDATE_COMMAND_UI(ID_SOCKET_CLOSE, OnUpdateSocketClose)
	ON_WM_CHAR()
	ON_COMMAND(ID_VIEW_SOCKETNOTIFICATIONS, OnViewSocketNotifications)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SOCKETNOTIFICATIONS, OnUpdateViewSocketNotifications)
	ON_COMMAND(ID_EDIT_CLEARBUFFER, OnEditClearBuffer)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermView construction/destruction

CTermView::CTermView()
{
	m_pSocket = NULL;
	m_fConnected = FALSE;
	m_fShowNotifications = FALSE;
}

CTermView::~CTermView()
{
	// If we allocated a socket
	if (m_pSocket != NULL)
	{            
		// If it's still connected
		if (m_fConnected)
			m_pSocket->Close();
		delete m_pSocket;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTermView drawing

void CTermView::OnDraw(CDC* pDC)
{
	CTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTermView printing

BOOL CTermView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CTermView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CTermView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CTermView diagnostics

#ifdef _DEBUG
void CTermView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTermView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CTermDoc* CTermView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTermDoc)));
	return (CTermDoc*)m_pDocument;
}
#endif //_DEBUG

//获得当前视图
CTermView *CTermView::GetView()
{
	CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	CView *pView = pFrame->GetActiveView();
	if (!pView)
		return NULL;
	if (!pView->IsKindOf(RUNTIME_CLASS(CTermView)))
		return NULL;
	return (CTermView *)pView;
}

void CTermView::OnEditClearBuffer()
{
	SetWindowText(NULL);
}


// 显示文字
void CTermView::Display(LPCSTR lpFormat, ...)
{
	// 判断是否整个编辑框是否已经满了
	CEdit& ed = GetEditCtrl();
	if (ed.GetLineCount() > 1000)
	{
		// 清空
		SetWindowText(NULL);
	}
	
	va_list Marker;
	static char szBuf[256];
	
	// 写入字符串
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	ed.SetSel(-1,-1);
	ed.ReplaceSel(szBuf);
}

/////////////////////////////////////////////////////////////////////////////
// CTermView message handlers


// 当用户按回车的时候，发送当前行文字
void CTermView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	// 首先判断是否连接成功
	if (!m_fConnected)
	{
		// 警告用户先连接服务器
		AfxMessageBox("请首先连接服务器\r\n");
		return;
	}   
	// 如果按下回车键
	if (nChar == 13)
	{               
		// 计算目前光标在那一行
		CEdit& ed = GetEditCtrl();
		int iStart, iEnd;
		ed.GetSel(iStart, iEnd);
		int iLine = ed.LineFromChar(iStart);
		if (iLine > -1)
		{
			static char szLine[256];
			memset(szLine, 0, sizeof(szLine));
			// 获得一整行数据
			int iNdx = ed.GetLine(iLine, szLine, sizeof(szLine)-1);
			if (iNdx > 0)
			{                    
				// 发送出数据
				strcat(szLine, "\r\n");
				m_pSocket->Send(szLine, strlen(szLine));
			}
		}
	}
	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CTermView::OnViewSocketNotifications()
{
	if (m_fShowNotifications)
		m_fShowNotifications = FALSE;
	else
		m_fShowNotifications = TRUE;
}

void CTermView::OnUpdateViewSocketNotifications(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_fShowNotifications);
}


// 连接菜单处理
void CTermView::OnSocketConnect()
{
	// 显示连接对话框
	CConnectDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;
	Display("Connect to port %d on %s...\r\n", 
			dlg.m_nPort,
			dlg.m_strHostName);

	// 如果原来socket已经存在，则删除
	if (m_pSocket != NULL)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}
			
	// 创建新的socket
	m_pSocket = new CTermSocket();
	
	// 调用create方法创建
	if (!m_pSocket->Create())
	{
		AfxMessageBox("Socket 创建失败");
		return;
	} 
	
	// 连接到远程服务器
	if (!m_pSocket->Connect(dlg.m_strHostName, dlg.m_nPort))
	{   
		if (m_pSocket->GetLastError() != WSAEWOULDBLOCK)
		{                                
			CString strError;
			strError.LoadString(m_pSocket->GetLastError());
			Display("Connect() failed: %s\r\n",
				strError);
			m_fConnected = FALSE;
			delete m_pSocket;
			m_pSocket = NULL;
		}
	}
	// 给变量赋值表示连接成功
	m_fConnected = TRUE;
}

void CTermView::OnUpdateSocketConnect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_fConnected);
}

void CTermView::OnSocketClose()
{                      
	//关闭
	m_pSocket->ShutDown(CAsyncSocket::sends);
	
	// 收到所有数据
	int nRet;
	char szBuf[256];
	while(1)
	{
    	nRet = m_pSocket->Receive(szBuf, sizeof(szBuf));
    	if (nRet == 0 || nRet == SOCKET_ERROR)
    		break;
	}
	// 关闭接收和接收 
	m_pSocket->ShutDown(CAsyncSocket::both);
	
	// 关闭socket	
	m_pSocket->Close();
	// 删除socket
	delete m_pSocket;
	m_pSocket = NULL;
	m_fConnected = FALSE;
}

void CTermView::OnUpdateSocketClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_fConnected);
}

//连接服务器
void CTermView::OnConnect(int nErrorCode)
{
	if (m_fShowNotifications)
		Display("\tOnConnect(%d)\r\n", nErrorCode);
		
	if (nErrorCode)
	{
		m_fConnected = FALSE;
		Display("\tError OnConnect(): %d\r\n",
					nErrorCode);
	}
	else
	{
		//显示连接成功
		m_fConnected = TRUE;
		Display("\tSocket connected\r\n");
	}
}

//发送数据事件
void CTermView::OnSend(int nErrorCode)
{
	if (m_fShowNotifications)
		Display("\tOnSend(%d)\r\n", nErrorCode);
}

//接收数据事件
void CTermView::OnReceive(int nErrorCode)
{
	
	if (m_fShowNotifications)
		Display("\tOnReceive(%d)\r\n", nErrorCode);
		
	static char szBuf[256];
	// 清空缓冲区，并确保缓冲区以结束
	memset(szBuf, 0, sizeof(szBuf));
	// 接收数据
	int nBytes = m_pSocket->Receive(szBuf, 255, 0);
	if (nBytes == 0)
	{
		Display("Receive() indicates that socket is closed\r\n");
		return;
	}
	if (nBytes == SOCKET_ERROR)
	{                            
		CString strError;
		strError.LoadString(m_pSocket->GetLastError());
		Display("Receive error %s\r\n", strError);
		return;
	}
	//显示接收到的数据
	Display(szBuf);
}

//socket关闭
void CTermView::OnClose(int nErrorCode)
{                       
	if (m_fShowNotifications)
		Display("\tOnClose(%d)\r\n", nErrorCode);

	AfxMessageBox("Socket 已经被关闭");
	m_fConnected = FALSE;
}



