#include "MainWnd.h"
#include "WndProc.h"
#include "client.h"
#include "ServerIP.h"
#include "Compression.h"
#include "ColorMode.h"
#include "GridSpacing.h"
#include "Gdi.h"
#include "resource.h"

#define	DISCONNECT_MSG		"DISCONNECT"

// 服务器IP的类的一个指针
extern	CServerIP				*pServerIP;

// 压缩类的一个指针
extern	CCompression			*pCompression;

// 网格空间类的一个指针
extern	CGridSpacing			*pGridSpacing;

// 颜色模式类的一个指针
extern	CColorMode				*pColorMode;

// 位图和socket变量
extern	"C" char				*pDIBitmap;
extern	"C" BOOL				fDIBitmap;
extern	"C"	int					iWidth,iHeight;
extern	"C"	int					iScreenWidth,iScreenHeight;
extern	"C"	int					iWidth1,iWidth2,iHeight1,iHeight2;
extern	"C"	int					iCompressionLevel;
extern	"C" LPBITMAPINFOHEADER	lpBitmapIH;
extern	"C" SOCKET				sClient;
extern	"C"	BOOL				fConnected;
extern	"C"	struct	GdiList		GdiStart;
extern	"C"	struct	GdiList		*pGdiNode;
extern	"C"	int					nGridX,nGridY;
extern	"C"	BOOL				fGridX,fGridY;
extern	"C"	WORD				bmBitsPixel;
extern  "C" UINT				port;

// 连接，发送和接收变量
int		iSent,iRecv;
char	szMouseMessage[81];

BYTE CMainWnd::m_ip[4];

CMainWnd::CMainWnd()
{
	// 初始化滚动条
	iHScrollPos = 0;
	iVScrollPos = 0;
	GdiStart.pNext = NULL;

	// 设置初始化时间间隔
	m_iTimerInterval = 398;
}

CMainWnd::~CMainWnd()
{
	delete this;
	// 释放全局内存
	if (fDIBitmap)
		free(pDIBitmap);
	// 释放全局区域的内存
	pGdiNode = GdiStart.pNext;
	while (pGdiNode)
	{
		if (pGdiNode->Gdi.fDIBitmap)
		{
			free(pGdiNode->Gdi.pDIBitmap);
			free(pGdiNode->Gdi.pDIB);
		}
		pGdiNode = pGdiNode->pNext;
	}
}

void CMainWnd::SetInstance(HINSTANCE hInst)
{
	m_hInst = hInst;
}

// 处理WM_CREATE消息
BOOL CMainWnd::OnCreate(HWND hWnd,CREATESTRUCT FAR *lpCreateStruct)
{
	//按照WinMains的nCmdShow参数显示窗口
	ShowWindow(hWnd,*(LPINT)lpCreateStruct->lpCreateParams);
	m_hWnd = hWnd;
	m_hMenu = GetMenu(m_hWnd);
	EnableMenuItem(m_hMenu,IDM_DISCONNECT,MF_BYCOMMAND|MF_GRAYED);
	EnableMenuItem(m_hMenu,IDM_COLORMODE,MF_BYCOMMAND|MF_GRAYED);

	// 设置初始化压缩级数
	iCompressionLevel = 10;
	WSADATA				wsd;
	char				szString[255];
	char				name[20];

	if (WSAStartup(0x202,&wsd) != 0)
	{
		sprintf(szString,"Failed to load Winsock!");
		MessageBox(NULL,szString,"Remote Server",MB_OK);
		return 1;
	}
	GetLocalHostName(name);
	GetIpAddress(name,&m_ip[0],&m_ip[1],&m_ip[2],&m_ip[3]);

	return TRUE;
}

//处理WM_DESTROY消息
void CMainWnd::OnDestroy(HWND hWnd)
{
	if (fDIBitmap)
	{
		KillTimer(m_hWnd,0);
	}

	if (fConnected)
	{
		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		strcpy(szMouseMessage,DISCONNECT_MSG);

		// 发送结束消息到服务器端
		iSent = Send(sClient,szMouseMessage);
		// 关闭socket
		closesocket(sClient);
		WSACleanup();
		// 设置连接状态
		fConnected = FALSE;
	}
	// 发送Windows Quit 消息到窗口
	PostMessage(hWnd,WM_QUIT,(WPARAM)0,0L);
}

// 处理WM_SIZE消息
void CMainWnd::OnSize(HWND hWnd,UINT state,int cx,int cy)
{
	if (fDIBitmap)
	{
		GetClientRect(m_hWnd,&m_Rect);

		// 按照新的屏幕尺寸设置滚动条
		SetScrollRange(m_hWnd,SB_HORZ,0,iScreenWidth - m_Rect.right,FALSE);
		SetScrollPos(m_hWnd,SB_HORZ,iHScrollPos,TRUE);
		SetScrollRange(m_hWnd,SB_VERT,0,iScreenHeight - m_Rect.bottom,FALSE);
		SetScrollPos(m_hWnd,SB_VERT,iVScrollPos,TRUE);

		// 生成 WM_PAINT 消息
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

// 处理WM_PAINT消息
void CMainWnd::OnPaint(HWND hWnd)
{
	PAINTSTRUCT			ps;
	HDC					hDC;
	struct	GdiList		*pNode;
	int					iX1,iX2,iY1,iY2;

	hDC = BeginPaint(hWnd,&ps);

	if (fDIBitmap)
	{
		GetClientRect(m_hWnd,&m_Rect);
		if (!fConnected)
		{
			m_hMemDC = CreateCompatibleDC(hDC);
			m_hDDBitmap = CreateCompatibleBitmap(hDC,iScreenWidth,iScreenHeight);
			SelectObject(m_hMemDC,m_hDDBitmap);
		}
		pNode = GdiStart.pNext;
		while (pNode)
		{
			if (pNode->Gdi.fDIBitmap)
			{
				StretchDIBits(m_hMemDC,
					pNode->Gdi.iWidth1,pNode->Gdi.iHeight1,
					pNode->Gdi.lpBitmapIH->biWidth,pNode->Gdi.lpBitmapIH->biHeight,
					0,0,
					pNode->Gdi.lpBitmapIH->biWidth,pNode->Gdi.lpBitmapIH->biHeight,
					(LPBYTE)pNode->Gdi.lpBitmapIH + (pNode->Gdi.lpBitmapIH->biSize + (1 << pNode->Gdi.lpBitmapIH->biBitCount) * sizeof(RGBQUAD)),
					(LPBITMAPINFO)pNode->Gdi.lpBitmapIH,DIB_RGB_COLORS,SRCCOPY);
			}

			if (fGridX)
			{
				iX1 = pNode->Gdi.iWidth1;
				iY1 = pNode->Gdi.iHeight1;
				iY2 = pNode->Gdi.iHeight2;

				MoveToEx(m_hMemDC,iX1,iY1,NULL);
				LineTo(m_hMemDC,iX1,iY2);
			}

			if (fGridY)
			{
				iX1 = pNode->Gdi.iWidth1;
				iX2 = pNode->Gdi.iWidth2;
				iY1 = pNode->Gdi.iHeight1;
				MoveToEx(m_hMemDC,iX1,iY1,NULL);
				LineTo(m_hMemDC,iX2,iY1);
			}

			pNode = pNode->pNext;
		}

		BitBlt(hDC,0,0,iScreenWidth,iScreenHeight,m_hMemDC,iHScrollPos,-iVScrollPos,SRCCOPY);
		if (!fConnected)
		{
			DeleteObject(m_hDDBitmap);
			DeleteDC(m_hMemDC);
		}
	}
	EndPaint(hWnd,&ps);
}

//处理WM_TIMER消息
void CMainWnd::OnTimer(HWND hWnd,UINT iId)
{
	// 如果我们连上了服务器
	if (fConnected)
	{
		// 得到一个新的屏幕
		GetScreen();
	}
}

// 处理WM_COMMAND消息
void CMainWnd::OnCommand(HWND hWnd,int iId,HWND hWndCtl,UINT iCodeNotify)
{
	// 选择菜单的退出
	if (iId == IDM_EXIT)
	{
		// 是否连上了服务器?
		if (fConnected)
		{
			// 发送结束消息到服务器
			memset(szMouseMessage,'\0',sizeof(szMouseMessage));
			strcpy(szMouseMessage,DISCONNECT_MSG);

			iSent = Send(sClient,szMouseMessage);

			// 关闭socket
			closesocket(sClient);
			// 清除socket
			WSACleanup();
			// 设置链接状态
			fConnected = FALSE;
			// 删去背景的位图
			DeleteObject(m_hDDBitmap);
			// 删去背景的Mem DC
			DeleteDC(m_hMemDC);
		}
		// 结束应用程序
		PostQuitMessage(0);
	}
	// 选择菜单的连接
	else if (iId == IDM_CONNECT)
	{
		// 初始化IP地址
		sprintf(pServerIP->szIP,"%d.%d.%d.%d",CMainWnd::m_ip[0],CMainWnd::m_ip[1],CMainWnd::m_ip[2],CMainWnd::m_ip[3]);
		pServerIP->port=::port;
		// 创建ip和port的对话框
		if (DialogBox(m_hInst,MAKEINTRESOURCE(IDD_SERVERIP),m_hWnd,(DLGPROC)ServerIPDlgProc))
		{
			if (LoadWinsock(m_hWnd,pServerIP->szIP) == 0)
				fConnected = TRUE;
			else
				fConnected = FALSE;

			if (fConnected)
			{
				// 使断开连接菜单项有效
				EnableMenuItem(m_hMenu,IDM_DISCONNECT,MF_BYCOMMAND|MF_ENABLED);
				// 使连接菜单项有效
				EnableMenuItem(m_hMenu,IDM_REFRESH,MF_BYCOMMAND|MF_GRAYED);
				// 使色彩模式菜单有效
				EnableMenuItem(m_hMenu,IDM_COLORMODE,MF_BYCOMMAND|MF_ENABLED);

				if (fDIBitmap)
				{
					// 在Mem DC建立完整的位图
					m_hMemDC = CreateCompatibleDC(GetDC(NULL));
					// 为Mem DC创建兼容句柄
					m_hDDBitmap = CreateCompatibleBitmap(GetDC(NULL),iScreenWidth,iScreenHeight);
					SelectObject(m_hMemDC,m_hDDBitmap);

					// 得到客户区域
					GetClientRect(m_hWnd,&m_Rect);

					// 设置滚动条
					SetScrollRange(m_hWnd,SB_HORZ,0,iScreenWidth - m_Rect.right,FALSE);
					SetScrollPos(m_hWnd,SB_HORZ,iHScrollPos,TRUE);
					SetScrollRange(m_hWnd,SB_VERT,0,iScreenHeight - m_Rect.bottom,FALSE);
					SetScrollPos(m_hWnd,SB_VERT,iVScrollPos,TRUE);

					// 设置刷新时间
					SetTimer(m_hWnd,0,m_iTimerInterval,NULL);
				}
			}
		}
	}
	// 选择断开连接的菜单
	else if (iId == IDM_DISCONNECT)
	{
		if (fConnected)
		{
			KillTimer(m_hWnd,0);
			EnableMenuItem(m_hMenu,IDM_DISCONNECT,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(m_hMenu,IDM_COLORMODE,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(m_hMenu,IDM_REFRESH,MF_BYCOMMAND|MF_ENABLED);
			//发送消息到服务器端结束连接
			memset(szMouseMessage,'\0',sizeof(szMouseMessage));
			strcpy(szMouseMessage,DISCONNECT_MSG);
			
			// 发送消息到服务器
			iSent = Send(sClient,szMouseMessage);

			// 关闭socket
			closesocket(sClient);
			WSACleanup();
			fConnected = FALSE;
			DeleteObject(m_hDDBitmap);
			DeleteDC(m_hMemDC);
		}
	}
	// 选择压缩菜单选项
	else if (iId == IDM_COMPRESSION)
	{
		// 选择压缩级数和压缩方式
		pCompression->m_iCompressionLevel = iCompressionLevel;
		if (DialogBox(m_hInst,MAKEINTRESOURCE(IDD_COMPRESSION),m_hWnd,(DLGPROC)CompressionDlgProc))
		{
			iCompressionLevel = pCompression->m_iCompressionLevel;
			if (fConnected)
			{
				KillTimer(m_hWnd,0);
				// 建立压缩消息
				memset(szMouseMessage,'\0',sizeof(szMouseMessage));
				sprintf(szMouseMessage,"WM_COMP;%d;0;0;0;\0",iCompressionLevel);
				// 设置压缩级数
				SendCommand(hWnd,1,szMouseMessage);
				// 设置网格
				SetGrids(m_hWnd);
				UpdateRegionalScreen(m_hWnd,sClient,TRUE);
				SetTimer(m_hWnd,0,m_iTimerInterval,NULL);
			}
		}
	}
	// 选择网格空间菜单
	else if (iId == IDM_GRIDSPACING)
	{
		// 设置当前的网格空间
		pGridSpacing->m_nGridX = nGridX;
		pGridSpacing->m_nGridY = nGridY;
		pGridSpacing->m_fGridX = fGridX;
		pGridSpacing->m_fGridY = fGridY;
		if (DialogBox(m_hInst,MAKEINTRESOURCE(IDD_GRIDSPACING),m_hWnd,(DLGPROC)GridSpacingDlgProc))
		{
			BOOL	fChange = FALSE;
			if (nGridX != pGridSpacing->m_nGridX)
				fChange = TRUE;
			if (nGridY != pGridSpacing->m_nGridY)
				fChange = TRUE;

			// 得到网格空间值
			nGridX = pGridSpacing->m_nGridX;
			nGridY = pGridSpacing->m_nGridY;
			fGridX = pGridSpacing->m_fGridX;
			fGridY = pGridSpacing->m_fGridY;
			if (fConnected && fChange)
			{
				KillTimer(m_hWnd,0);
				SetGrids(m_hWnd);
				UpdateRegionalScreen(m_hWnd,sClient,TRUE);
				SetTimer(m_hWnd,0,m_iTimerInterval,NULL);
			}
		}
	}
	// 选择颜色模式的菜单
	else if (iId == IDM_COLORMODE)
	{
		// 设置当前的压缩级数和模式
		pColorMode->m_bmBitsPixel = bmBitsPixel;
		if (DialogBox(m_hInst,MAKEINTRESOURCE(IDD_COLORMODE),m_hWnd,(DLGPROC)ColorModeDlgProc))
		{
			bmBitsPixel = pColorMode->m_bmBitsPixel;
			if (fConnected)
			{
				KillTimer(m_hWnd,0);
				memset(szMouseMessage,'\0',sizeof(szMouseMessage));
				sprintf(szMouseMessage,"WM_CMOD;%d;0;0;0;\0",bmBitsPixel);
				SendCommand(hWnd,1,szMouseMessage);
				SetGrids(m_hWnd);
				UpdateRegionalScreen(m_hWnd,sClient,TRUE);
				SetTimer(m_hWnd,0,m_iTimerInterval,NULL);
			}
		}
	}
	else if(iId==IDC_ABOUT)
	{
		MessageBox(hWnd,"2003-7-2创建\r\n一个远程控制的客户端程序","远程控制客户端",MB_OK);
	}

}
// 处理WM_VSCROLL消息
void CMainWnd::OnVScroll(HWND hWnd,HWND hWndCtl,UINT iCode,int iPos)
{
	if (fDIBitmap)
	{
		if (iCode == SB_LINEUP)
			iVScrollPos = iVScrollPos + 1;
		else if (iCode == SB_LINEDOWN)
			iVScrollPos = iVScrollPos - 1;
		else if (iCode == SB_PAGEUP)
			iVScrollPos = iVScrollPos + 64;
		else if (iCode == SB_PAGEDOWN)
			iVScrollPos = iVScrollPos - 64;
		else if (iCode == SB_THUMBTRACK)
			iVScrollPos = -iPos;
		else if (iCode == SB_THUMBPOSITION)
			iVScrollPos = -iPos;

		if (-iVScrollPos < 0)
			iVScrollPos = 0;
		if (-iVScrollPos > (iScreenHeight - m_Rect.bottom))
			iVScrollPos = -(iScreenHeight - m_Rect.bottom);

		SetScrollPos(hWnd,SB_VERT,-iVScrollPos,TRUE);

		InvalidateRect(hWnd,NULL,FALSE);
	}
}

//处理WM_HSCROLL消息
void CMainWnd::OnHScroll(HWND hWnd,HWND hWndCtl,UINT iCode,int iPos)
{
	if (fDIBitmap)
	{
		if (iCode == SB_LINEUP)
			iHScrollPos = iHScrollPos - 1;
		else if (iCode == SB_LINEDOWN)
			iHScrollPos = iHScrollPos + 1;
		else if (iCode == SB_PAGEUP)
			iHScrollPos = iHScrollPos - 48;
		else if (iCode == SB_PAGEDOWN)
			iHScrollPos = iHScrollPos + 48;
		else if (iCode == SB_THUMBTRACK)
			iHScrollPos = iPos;
		else if (iCode == SB_THUMBPOSITION)
			iHScrollPos = iPos;

		if (iHScrollPos < 0)
			iHScrollPos = 0;
		if (iHScrollPos > (iScreenWidth - m_Rect.right))
			iHScrollPos = iScreenWidth - m_Rect.right;

		SetScrollPos(hWnd,SB_HORZ,iHScrollPos,TRUE);

		InvalidateRect(hWnd,NULL,FALSE);
	}
}

// 处理左键双击的消息
void CMainWnd::OnLButtonDblClk(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_LBK;%d;%d;%d;0;\0",x,y,keyFlags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理左键按下的消息
void CMainWnd::OnLButtonDown(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);
		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_LBD;%d;%d;%d;0;\0",x,y,keyFlags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理左键弹起的消息
void CMainWnd::OnLButtonUp(HWND hWnd,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_LBU;%d;%d;%d;0;\0",x,y,keyFlags);

		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理中键按下的消息
void CMainWnd::OnMButtonDblClk(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_MBK;%d;%d;%d;0;\0",x,y,keyFlags);

		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理中键按下的消息
void CMainWnd::OnMButtonDown(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_MBD;%d;%d;%d;0;\0",x,y,keyFlags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理中键弹起的消息
void CMainWnd::OnMButtonUp(HWND hWnd,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_MBU;%d;%d;%d;0;\0",x,y,keyFlags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理右键双击的消息
void CMainWnd::OnRButtonDblClk(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);
		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_RBK;%d;%d;%d;0;\0",x,y,keyFlags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理右键按下的消息
void CMainWnd::OnRButtonDown(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_RBD;%d;%d;%d;0;\0",x,y,keyFlags);

		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理右键弹起的消息
void CMainWnd::OnRButtonUp(HWND hWnd,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_RBU;%d;%d;%d;0;\0",x,y,keyFlags);

		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理鼠标移动的消息
void CMainWnd::OnMouseMove(HWND hWnd,int x,int y,UINT keyFlags)
{
	if (fConnected)
	{
		x += iHScrollPos;
		y += (-1 * iVScrollPos);

		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_MM;%d;%d;%d;0;\0",x,y,keyFlags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理键按下的消息
void CMainWnd::OnKeyDown(HWND hWnd,UINT vk,BOOL fDown,int cRepeat,UINT flags)
{
	if (fConnected)
	{
		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_KD;%d;%d;%d;%d;\0",vk,fDown,cRepeat,flags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 处理键弹起的消息
void CMainWnd::OnKeyUp(HWND hWnd,UINT vk,BOOL fDown,int cRepeat,UINT flags)
{
	if (fConnected)
	{
		memset(szMouseMessage,'\0',sizeof(szMouseMessage));
		sprintf(szMouseMessage,"WM_KU;%d;%d;%d;%d;\0",vk,fDown,cRepeat,flags);
		SendCommand(m_hWnd,1,szMouseMessage);
	}
}

// 得到新的屏幕
void CMainWnd::GetScreen()
{
	SendCommand(m_hWnd,0,NULL);
}
//获得本地计算机名称
int CMainWnd::GetLocalHostName(char* sHostName)	
{
	char szHostName[256];
	int nRetCode;
	nRetCode=gethostname(szHostName,sizeof(szHostName));
	if(nRetCode!=0)
	{
		//产生错误
		strcpy(sHostName,"没有取得");
		return GetLastError();
	}
	strcpy(sHostName,szHostName);
	return 0;
}
//获得本地IP
int CMainWnd::GetIpAddress(char *sHostName, char *sIpAddress)
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		sIpAddress=NULL;
		return GetLastError();
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//转换为标准格式
		strcpy(sIpAddress,inet_ntoa(inAddr));
	}
	return 0;
}
//获得本地IP
int CMainWnd::GetIpAddress(char *sHostName, BYTE *f0,BYTE *f1,BYTE *f2,BYTE *f3)
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		f0=f1=f2=f3=0;
		return GetLastError();
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		*f0=inAddr.S_un.S_un_b.s_b1;
		*f1=inAddr.S_un.S_un_b.s_b2;
		*f2=inAddr.S_un.S_un_b.s_b3;
		*f3=inAddr.S_un.S_un_b.s_b4;
	}
	return 0;
}
