//服务器端：发送本机桌面到连接的客户端
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <winuser.h>
#include "Server.h"
#include "Command.h"
#include "Gdi.h"
#include "HuffCompress.h"
#include "RLE.h"
//默认端口
#define DEFAULT_PORT        5150
#define SETFLAGS			XP1_GUARANTEED_DELIVERY|XP1_GUARANTEED_ORDER
#define NOTSETFLAGS			XP1_CONNECTIONLESS
#define	LPBMIH				LPBITMAPINFOHEADER

UINT    gPort  = DEFAULT_PORT;
int		nGridX = 8;
int		nGridY = 2;
DWORD	dwLen,dwCompress,dwSendLen;
int		iCompressionLevel = 10;
HWND	hServerWnd;
HDC		hDDC = NULL,hMemDC,hNullDC;
BOOL	fChange = FALSE;
SOCKET	Socket;
SOCKET	Listen;
int		iWidth,iHeight;
WORD	bmBitsPixel = 4;
// 指向全局位图
BOOL	fDIBitmap = FALSE;

// GDI区域结构
struct	GdiList	GdiStart;
struct	GdiList	*pGdiNode;

// 这个结构用来在LPARAM参数中传递信息到客户端线程
struct	myStruct
{
	SOCKET	Socket;
	HWND	hWnd;
};

int SelectProtocols(DWORD dwSetFlags,DWORD dwNotSetFlags,LPWSAPROTOCOL_INFO lpProtocolBuffer,LPDWORD lpdwBufferLength,WSAPROTOCOL_INFO *pProtocol);

//LoadWinsock用来装载和初始化Winsock，绑定本地地址，创建监听socket，等候客户端连接
DWORD WINAPI LoadWinsock(LPVOID lpParam)
{
	// 协议变量
	LPBYTE				pBuf;
	WSAPROTOCOL_INFO	Protocol;
	int					nRet;
	int					nZero = 0;

	int					iAddrSize;
	HANDLE				hThread;
	DWORD				dwThreadId;
	char				szClientIP[81];
	char				szString[255];
	struct	sockaddr_in	local,client;
	// 这个结构用来在LPARAM参数中传递信息到客户端线程
	struct	myStruct	myStructure;

	// 为协议的选择和所有协议的变量决定需要的缓冲区的大小
	dwLen = 0;
	nRet = WSAEnumProtocols(NULL,NULL,&dwLen);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAENOBUFS)
			return 1;
	}
	pBuf = malloc(dwLen);
	// 为WSASocketGet()得到协议
	nRet = SelectProtocols(SETFLAGS,NOTSETFLAGS,(LPWSAPROTOCOL_INFO)pBuf,&dwLen,&Protocol);

	// 创建我们的监听socket
	Listen = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_IP,NULL,0,SOCK_STREAM);
	if (Listen == SOCKET_ERROR)
	{
		sprintf(szString,"socket() failed: %d",WSAGetLastError());
		MessageBox(NULL,szString,"Remote Server",MB_OK);
		return 1;
	}

	// 设置server端信息
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(gPort);

	// 绑定到socket
	if (bind(Listen,(struct sockaddr *)&local,sizeof(local)) == SOCKET_ERROR)
	{
		sprintf(szString,"bind() failed: %d\n", WSAGetLastError());
		MessageBox(NULL,szString,"Remote Server",MB_OK);
		return 1;
	}

	//为了减小CPU的利用率，禁止在socket上将数据发送到缓冲。设置SO_SNDBUF为0,
	//从而使winsock直接发送数据到客户端，而不是将数据缓冲才发送。
	nZero = 0;
	setsockopt(Listen,SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));
	//开始监听
	listen(Listen,SOMAXCONN);

	iAddrSize = sizeof(client);
	while (TRUE)
	{
		// 阻塞方式的接收客户端的连接，但因为这是一个线程函数，所以用户不会感到阻塞
		Socket = accept(Listen,(struct sockaddr *)&client,&iAddrSize);
		if (Socket != INVALID_SOCKET)
		{
			// 设置传到客户端线程的信息的数据结构
			myStructure.Socket = Socket;
			myStructure.hWnd = hServerWnd;
			//找出客户端的IP地址
			memset(szClientIP,'\0',sizeof(szClientIP));
			sprintf(szClientIP,"%s",inet_ntoa(client.sin_addr));
			// 为每一个客户端创建一个线程
			hThread = CreateThread(NULL,0,ClientThread,(LPVOID)&myStructure,0,&dwThreadId);
			if (hThread)
			{
				//关闭线程句柄
				CloseHandle(hThread);
			}
		}
		else
			return 1;
	}

	return 0;
}


//客户端线程函数，这个函数等候从客户端程序发送过来的消息，
//如果这个消息是"REFRESH",那么它发送当前的桌面图片
//如果这个消息是"DISCONNECT",那么它结束和客户端的连接
//如果这个消息以"WM_"开头，那么它就根据消息类型，在服务器端执行该消息
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	HWND	hWnd;
	SOCKET	MySocket;
	FD_SET	SocketSet;
	struct	timeval	timeout;
	char	szMessage[2049];
	DWORD	iRecv;
	struct	myStruct	*myStructure;
	DWORD	iLength;
	DWORD	iRet;
	int		iUpdates;

	// 分析参数
	myStructure = (struct myStruct *)lpParam;
	MySocket = myStructure->Socket;
	hWnd = myStructure->hWnd;

	// 设置超时值
	timeout.tv_sec = 0;		// 秒
	timeout.tv_usec = 0;	// 微秒

	// 设置Socket集合
	SocketSet.fd_count = 1;
	SocketSet.fd_array[1] = MySocket;

	// 轮询sockets
	while(TRUE)
	{
		// 等候发送过来的数据直到超时
		iRet = select(0,&SocketSet,NULL,NULL,&timeout);
		if (iRet != 0)
		{
 			//初始化缓冲
			memset(szMessage,'\0',sizeof(szMessage));
			// 阻塞方式调用recv()
			iRecv = recv(MySocket,szMessage,2048,0);
			szMessage[iRecv] = '\0';

			CHECK_MSG:
			// 是不是"REFRESH"消息
			if (strncmp(szMessage,"REFRESH",7) == 0)
			{
				// 捕获并且发送桌面的更新的区域
				iUpdates = SendRegionDisplay(hServerWnd,MySocket);
			}
			
			// 检查从客户端发送过来的Windows 命令消息，这是一个核心部分
			else if (strncmp(szMessage,"WM_",3) == 0)
			{
				// 解析从客户端发送过来的消息并发送到本机的消息队列
				DispatchWMMessage(szMessage);

				// 看看是否还有消息
				iLength = strlen(szMessage);
				if (iLength > 0)
					goto CHECK_MSG;
			}
			// 检查是否是查询消息
			else if (strncmp(szMessage,"RESOLUTION",10) == 0)
			{
				SendResolution(MySocket);
			}
			// 检查是否是DISCONNECT消息
			else if (strncmp(szMessage,"DISCONNECT",10) == 0)
			{
				fChange = FALSE;
				fDIBitmap = FALSE;

				pGdiNode = GdiStart.pNext;
				while (pGdiNode)
				{
					free(pGdiNode->Gdi.pDIBitmap);
					free(pGdiNode->Gdi.pDIBChangeStart);
					pGdiNode->Gdi.fDIBitmap = FALSE;
					pGdiNode->Gdi.fChange = FALSE;
					pGdiNode = pGdiNode->pNext;
				}
				// 停止查询，相当于结束该线程
				break;
			}
		}
	}
	closesocket(MySocket);
	return 0;
}

// 解析从客户端发送过来的消息并发送到本机的消息队列
void DispatchWMMessage(char *szString)
{
	//鼠标消息
	struct {char *szWMMouseMsg;} 
	WMMouseMsg[] = {"WM_MM","WM_LBD","WM_LBU","WM_LBK",
					"WM_MBD","WM_MBU","WM_MBK",
					"WM_RBD","WM_RBU","WM_RBK"};

	// 键盘消息
	struct {char *szWMKeyBdMsg;}
	WMKeyBdMsg[] = {"WM_KD","WM_KU"};

	// 通用消息：色彩模式，网格数和压缩消息
	struct {char *szMsg;}
	Msg[] = {"WM_COMP","WM_GRID","WM_CMOD"};

	int		nWMMouseMsg;
	int		nWMKeyBdMsg;
	int		nMsg;

	struct	CommandList	CommandStart;
	struct	CommandList	*pCommandNode;
	struct	CommandDS	Command;
	char	*pDest;
	int		iLoc,nChar;
	int		iLoop,iParms;
	char	szString2[2049];

	// 分别得到鼠标，键盘，通用消息的数目
	nWMMouseMsg = (int)(sizeof(WMMouseMsg)/sizeof(WMMouseMsg[0]));
	nWMKeyBdMsg = (int)(sizeof(WMKeyBdMsg)/sizeof(WMKeyBdMsg[0]));
	nMsg = (int)(sizeof(Msg)/sizeof(Msg[0]));

	// 初始化command链表
	CommandStart.pNext = NULL;
	pCommandNode = &CommandStart;

	// 分析command命令，截获命令的参数
	iParms = 0;
	while (pDest = strchr(szString,';'))
	{
		iLoc = pDest - szString;
		nChar = iLoc;
		memset(Command.szElement,'\0',sizeof(Command.szElement));
		strncpy(Command.szElement,szString,nChar);
		// 发送到命令栈中
		pCommandNode = Add_Command(pCommandNode,Command);
		memset(szString2,'\0',sizeof(szString2));
		strcpy(szString2,&szString[iLoc + 1]);
		strcpy(szString,szString2);

		iParms++;
		if (iParms == 5) // 每条命令5个参数
			break;
	}

	// 处理命令
	pCommandNode = CommandStart.pNext;
	if (pCommandNode)
	{
		// 鼠标消息
		UINT	keyFlags;
		int		iMessage;
		int		fWMMouseMsg;
		double	iScaleX,iScaleY,iX,iY;
		DWORD	dwX,dwY;

		// 键盘消息
		int		fWMKeyBdMsg;
		UINT	vk;
		int		fDown;
		int		cRepeat;
		UINT	flags;

		// 判断是否有鼠标消息
		fWMMouseMsg = FALSE;
		for (iLoop = 0;iLoop < nWMMouseMsg;iLoop++)
		{
			if (strcmp(pCommandNode->Command.szElement,WMMouseMsg[iLoop].szWMMouseMsg) == 0)
			{
				// 设置鼠标消息的标志
				fWMMouseMsg = TRUE;
				// 具体的鼠标消息
				if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_MM\0") == 0)
					iMessage = 1;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_LBD\0") == 0)
					iMessage = 2;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_LBU\0") == 0)
					iMessage = 3;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_LBK\0") == 0)
					iMessage = 4;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_MBD\0") == 0)
					iMessage = 5;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_MBU\0") == 0)
					iMessage = 6;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_MBK\0") == 0)
					iMessage = 7;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_RBD\0") == 0)
					iMessage = 8;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_RBU\0") == 0)
					iMessage = 9;
				else if (strcmp(WMMouseMsg[iLoop].szWMMouseMsg,"WM_RBK\0") == 0)
					iMessage = 10;

				// 移动到参数栈的下一个节点，x坐标
				pCommandNode = pCommandNode->pNext;
				iX = atof(pCommandNode->Command.szElement);

				// 移动到参数栈的下一个节点，y坐标
				pCommandNode = pCommandNode->pNext;
				iY = atof(pCommandNode->Command.szElement);

				// 移动到参数栈的下一个节点，辅助键
				pCommandNode = pCommandNode->pNext;
				keyFlags = atoi(pCommandNode->Command.szElement);

				// 退出循环
				break;
			}
		}

		// 如果有鼠标消息则对鼠标消息进行处理
		if (fWMMouseMsg)
		{
			// 得到坐标的范围因子
			iScaleX = 65535.0 / (iWidth - 1);
			iScaleY = 65535.0 / (iHeight - 1);
			// 对坐标进行比例缩放
			iX *= iScaleX;
			iY *= iScaleY;
			// 转换成DWORDS
			dwX = (DWORD)iX;
			dwY = (DWORD)iY;

			// 处理鼠标消息
			if (iMessage == 1) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE,dwX,dwY,0,0);
			}
			else if (iMessage == 2) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,dwX,dwY,0,0);
			}
			else if (iMessage == 3) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,dwX,dwY,0,0);
			}
			else if (iMessage == 4) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,dwX,dwY,0,0);
			}
			else if (iMessage == 5) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MIDDLEDOWN,dwX,dwY,0,0);
			}
			else if (iMessage == 6) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MIDDLEUP,dwX,dwY,0,0);
			}
			else if (iMessage == 7) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MIDDLEDOWN,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MIDDLEUP,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MIDDLEDOWN,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MIDDLEUP,dwX,dwY,0,0);
			}
			else if (iMessage == 8) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN,dwX,dwY,0,0);
			}
			else if (iMessage == 9) 
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,dwX,dwY,0,0);
			}
			else if (iMessage == 10)
			{
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN,dwX,dwY,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,dwX,dwY,0,0);
			}
		}
		else
		{
			// 没有鼠标消息则判断是否有键盘消息
			fWMKeyBdMsg = FALSE;
			for (iLoop = 0;iLoop < nWMKeyBdMsg;iLoop++)
			{
				if (strcmp(pCommandNode->Command.szElement,WMKeyBdMsg[iLoop].szWMKeyBdMsg) == 0)
				{
					// 设置键盘消息标志
					fWMKeyBdMsg = TRUE;
					if (strcmp(WMKeyBdMsg[iLoop].szWMKeyBdMsg,"WM_KD\0") == 0)
						iMessage = 1;//按下键
					else if (strcmp(WMKeyBdMsg[iLoop].szWMKeyBdMsg,"WM_KU\0") == 0)
						iMessage = 2;//松开键

					// 移动到参数链表的下一个节点，Virtural 键码
					pCommandNode = pCommandNode->pNext;
					vk = atoi(pCommandNode->Command.szElement);

					//移动到参数链表的下一个节点，按下键标志
					pCommandNode = pCommandNode->pNext;
					fDown = atoi(pCommandNode->Command.szElement);

					// 移动到参数链表的下一个节点，按键重复数
					pCommandNode = pCommandNode->pNext;
					cRepeat = atoi(pCommandNode->Command.szElement);

					// 移动到参数链表的下一个节点，标志位
					pCommandNode = pCommandNode->pNext;
					flags = atoi(pCommandNode->Command.szElement);

					break;
				}
			}

			// 如果有键盘消息，则处理键盘消息
			if (fWMKeyBdMsg)
			{
				if (iMessage == 1) //模拟按键消息
				{
					keybd_event((BYTE)vk,(BYTE)vk,0,0);
				}
				else if (iMessage == 2) //模拟松开键的消息
				{
					keybd_event((BYTE)vk,(BYTE)vk,KEYEVENTF_KEYUP,0);
				}
			}
			else // 通用消息
			{
				for (iLoop = 0;iLoop < nMsg;iLoop++)
				{
					if (strcmp(pCommandNode->Command.szElement,Msg[iLoop].szMsg) == 0)
					{
						if (strcmp(Msg[iLoop].szMsg,"WM_COMP\0") == 0)
						{
							// 移动到参数链表的下一个节点，压缩级数
							pCommandNode = pCommandNode->pNext;
							iCompressionLevel = atoi(pCommandNode->Command.szElement);
						}
						else if (strcmp(Msg[iLoop].szMsg,"WM_GRID\0") == 0)
						{
							// 移动到参数链表的下一个节点，x网格数
							pCommandNode = pCommandNode->pNext;
							nGridX = atoi(pCommandNode->Command.szElement);

							// 移动到参数链表的下一个节点，y网格数
							pCommandNode = pCommandNode->pNext;
							nGridY = atoi(pCommandNode->Command.szElement);

							// 清除当前的显示设置
							ClearDisplay(hServerWnd);
							// 初始化新的显示设置
							InitDisplay(hServerWnd);
						}
						else if (strcmp(Msg[iLoop].szMsg,"WM_CMOD\0") == 0)
						{
							// 移动到参数链表的下一个节点，每个屏幕点的字节数
							pCommandNode = pCommandNode->pNext;
							bmBitsPixel = atoi(pCommandNode->Command.szElement);

							ClearDisplay(hServerWnd);
							InitDisplay(hServerWnd);
						}
					}
				}
			}
		}
	}
	// 清除命令队列
	Clear_Command(&CommandStart);
}

// 初始化显示变量
void InitDisplay(HWND hWnd)
{
	struct	GdiDS	Gdi;
	int		iWidthX,iHeightY,nGrid;
	int		iXGrid,iYGrid,iLoop;

	GdiStart.pNext = NULL;
	pGdiNode = &GdiStart;
	hDDC = CreateDC("DISPLAY",NULL,NULL,NULL);

	// 得到屏幕的宽度和高度
	iWidth = GetDeviceCaps(hDDC,HORZRES);
	iHeight = GetDeviceCaps(hDDC,VERTRES);

	// 分割屏幕成X网格区域
	if (nGridX > 0)
		iWidthX = iWidth/nGridX;
	else
		iWidthX = iWidth;

	// 分割屏幕成Y网格区域
	if (nGridY > 0)
		iHeightY = iHeight/nGridY;
	else
		iHeightY = iHeight;

	// 为网格创建区域的位图和Memory DC
	if ((nGridX > 0) && (nGridY > 0))
	{
		for (iXGrid = 0;iXGrid < nGridX;iXGrid++)
		{
			for (iYGrid = 0;iYGrid < nGridY;iYGrid++)
			{
				Gdi.fChange = FALSE;
				Gdi.fDIBitmap = FALSE;

				Gdi.iGridX = iXGrid;
				Gdi.iGridY = iYGrid;

				Gdi.iWidth1 = iXGrid * iWidthX;
				Gdi.iWidth2 = iXGrid * iWidthX + iWidthX;
				Gdi.iHeight1 = iYGrid * iHeightY;
				Gdi.iHeight2 = iYGrid * iHeightY + iHeightY;

				Gdi.hMemDC = CreateCompatibleDC(hDDC);
				Gdi.hDIBitmap = CreateCompatibleBitmap(hDDC,iWidthX,iHeightY);
				SelectObject(Gdi.hMemDC,Gdi.hDIBitmap);

				// 往队列中添加GDI信息
				pGdiNode = Add_Gdi(pGdiNode,Gdi);
			}
		}
	}
	else 
	{
		nGrid = max(nGridX,nGridY);
		for (iLoop = 0;iLoop < nGrid;iLoop++)
		{
			Gdi.fChange = FALSE;
			Gdi.fDIBitmap = FALSE;

			if (nGridX > 0)
			{
				Gdi.iGridX = iLoop;
				Gdi.iWidth1 = iLoop * iWidthX;
				Gdi.iWidth2 = iLoop * iWidthX + iWidthX;
			}
			else
			{
				Gdi.iGridX = 0;
				Gdi.iWidth1 = 0;
				Gdi.iWidth2 = iWidthX;
			}

			if (nGridY > 0)
			{
				Gdi.iGridY = iLoop;
				Gdi.iHeight1 = iLoop * iHeightY;
				Gdi.iHeight2 = iLoop * iHeightY + iHeightY;
			}
			else
			{
				Gdi.iGridY = 0;
				Gdi.iHeight1 = 0;
				Gdi.iHeight2 = iHeightY;
			}

			Gdi.hMemDC = CreateCompatibleDC(hDDC);
			Gdi.hDIBitmap = CreateCompatibleBitmap(hDDC,iWidthX,iHeightY);
			SelectObject(Gdi.hMemDC,Gdi.hDIBitmap);
			pGdiNode = Add_Gdi(pGdiNode,Gdi);
		}
	}
	hNullDC = GetDC(NULL);
}

// 清除显示变量
void ClearDisplay(HWND hWnd)
{
	DeleteDC(hDDC);
	DeleteDC(hNullDC);
	Clear_Gdi(&GdiStart);
}

//得到区域的显示位图
int GetRegionDisplay(HWND hWnd)
{
	int		iWidth1,iWidth2,iHeight1,iHeight2;
	BOOL	bGotBits;
	DWORD	iLength;
	char	*pStartDIB;

	DWORD	dwLen1;
	DWORD	dwBitMapHeader1;
	char	*pDIBitmap1;
	char	*pDIB1;
	int		fChange1;
	char	*pDIBChange1;

	// 得到区域的矩形坐标
	iWidth1 = pGdiNode->Gdi.iWidth1;
	iWidth2 = pGdiNode->Gdi.iWidth2;
	iHeight1 = pGdiNode->Gdi.iHeight1;
	iHeight2 = pGdiNode->Gdi.iHeight2;

	BitBlt(pGdiNode->Gdi.hMemDC,0,0,iWidth2,iHeight2,hDDC,iWidth1,iHeight1,SRCCOPY);

	//将DDB转化成DIB
	// 得到区域位图信息
	GetObject(pGdiNode->Gdi.hDIBitmap,sizeof(BITMAP),&(pGdiNode->Gdi.DIBitmap));
	// 设置颜色模式
	pGdiNode->Gdi.DIBitmap.bmBitsPixel = bmBitsPixel;
	// 初始化位图信息头
	pGdiNode->Gdi.BMIH.biSize = sizeof(BITMAPINFOHEADER);
	pGdiNode->Gdi.BMIH.biWidth = pGdiNode->Gdi.DIBitmap.bmWidth;
	pGdiNode->Gdi.BMIH.biHeight = pGdiNode->Gdi.DIBitmap.bmHeight;
	pGdiNode->Gdi.BMIH.biPlanes = 1;
	pGdiNode->Gdi.BMIH.biBitCount = (WORD)pGdiNode->Gdi.DIBitmap.bmPlanes * (WORD)pGdiNode->Gdi.DIBitmap.bmBitsPixel;
	pGdiNode->Gdi.BMIH.biCompression = BI_RGB;
	pGdiNode->Gdi.BMIH.biSizeImage = 0;
	pGdiNode->Gdi.BMIH.biXPelsPerMeter = 0;
	pGdiNode->Gdi.BMIH.biYPelsPerMeter = 0;
	pGdiNode->Gdi.BMIH.biClrUsed = 0;
	pGdiNode->Gdi.BMIH.biClrImportant = 0;

	//设置区域的颜色数
	pGdiNode->Gdi.nColors = 1 << pGdiNode->Gdi.BMIH.biBitCount;
	if (pGdiNode->Gdi.nColors > 256)
		pGdiNode->Gdi.nColors = 0; 
	// 计算为位图信息头和彩色表分配内存所需的尺寸
	pGdiNode->Gdi.dwLen = (DWORD)(sizeof(BITMAPINFOHEADER) + pGdiNode->Gdi.nColors * sizeof(RGBQUAD));

	if (!pGdiNode->Gdi.fDIBitmap)
	{
		pGdiNode->Gdi.pDIB = (char *)malloc(pGdiNode->Gdi.dwLen);
		pStartDIB = pGdiNode->Gdi.pDIB;
	}

	pGdiNode->Gdi.lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;
	*(pGdiNode->Gdi.lpBMIH) = pGdiNode->Gdi.BMIH;
	GetDIBits(hNullDC,pGdiNode->Gdi.hDIBitmap,0L,(DWORD)pGdiNode->Gdi.BMIH.biHeight,(LPBYTE)NULL,(LPBITMAPINFO)pGdiNode->Gdi.lpBMIH,DIB_RGB_COLORS);

	// 以计算的尺寸设置区域位图信息头
	pGdiNode->Gdi.BMIH = *(pGdiNode->Gdi.lpBMIH);
	// 保存位图信息头的大小
	pGdiNode->Gdi.dwBitMapHeader = pGdiNode->Gdi.dwLen;
	pGdiNode->Gdi.dwLen += (DWORD)(pGdiNode->Gdi.BMIH.biSizeImage);
	if (!pGdiNode->Gdi.fDIBitmap)
	{
		pGdiNode->Gdi.pDIB = pStartDIB;
		pGdiNode->Gdi.pDIB = (char *)realloc(pGdiNode->Gdi.pDIB,pGdiNode->Gdi.dwLen);
	}

	pGdiNode->Gdi.lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;
	bGotBits = GetDIBits(hNullDC,pGdiNode->Gdi.hDIBitmap,0L,(DWORD)pGdiNode->Gdi.BMIH.biHeight,(LPBYTE)pGdiNode->Gdi.lpBMIH + (pGdiNode->Gdi.BMIH.biSize + pGdiNode->Gdi.nColors * sizeof(RGBQUAD)),(LPBITMAPINFO)pGdiNode->Gdi.lpBMIH,DIB_RGB_COLORS);

	// 比较区域DIBS之前和之后的变化
	if (pGdiNode->Gdi.fDIBitmap)
	{
		dwLen1 = pGdiNode->Gdi.dwLen;
		dwBitMapHeader1 = pGdiNode->Gdi.dwBitMapHeader;
		pDIBitmap1 = pGdiNode->Gdi.pDIBitmap;
		pDIB1 = pGdiNode->Gdi.pDIB;
		fChange1 = pGdiNode->Gdi.fChange;

		// 每次4个字节的比较两副位图,注意要跳过位图的头部
		__asm
		{
			MOV		ECX,dwLen1
			SUB		ECX,dwBitMapHeader1
			SHR		ECX,2 
			MOV		EDX,dwBitMapHeader1
			MOV		ESI,pDIBitmap1
			ADD		ESI,EDX
			MOV		EDI,pDIB1
			ADD		EDI,EDX
			REP		CMPSD
			JNZ		SetFlagRegion1
			MOV		fChange1,FALSE
			JMP		ExitRegion1
			SetFlagRegion1:
			MOV		fChange1,TRUE
			ExitRegion1:
		}

		//设置变化状态
		pGdiNode->Gdi.fChange = fChange1;
		// 为两副区域DIB的差别建立一个新的DIB
		if (pGdiNode->Gdi.fChange)
		{
			DWORD		iZeros = 0;
			iLength = (pGdiNode->Gdi.dwLen - pGdiNode->Gdi.dwBitMapHeader);
			pGdiNode->Gdi.pDIBChange = pGdiNode->Gdi.pDIBChangeStart;
			pDIBChange1 = pGdiNode->Gdi.pDIBChange;

			__asm
			{
				MOV		ECX,iLength 
				SHR		ECX,2 
				MOV		EDI,pDIBChange1
				MOV		ESI,pDIB1
				ADD		ESI,dwBitMapHeader1
				MOV		EDX,pDIBitmap1
				ADD		EDX,dwBitMapHeader1
				SubtractRegion:
				LODSD
				SUB		EAX,[EDX]
				ADD		EDX,4
				STOSD
				DEC		ECX
				JNZ		SubtractRegion
			}

			//拷贝到全局区域的DIB
			memblast(pGdiNode->Gdi.pDIBitmap,pGdiNode->Gdi.pDIB,pGdiNode->Gdi.dwLen);
			// 设置压缩长度
			pGdiNode->Gdi.dwCompress = pGdiNode->Gdi.dwLen - pGdiNode->Gdi.dwBitMapHeader;
			// 设置数据的起始位
			pGdiNode->Gdi.iStartPos = pGdiNode->Gdi.dwBitMapHeader;
		}
	}
	else
	{
		iLength = (pGdiNode->Gdi.dwLen);
		pGdiNode->Gdi.pDIBitmap = (char *)malloc(iLength);
		pGdiNode->Gdi.pDIBChange = (char *)malloc(iLength);
		pGdiNode->Gdi.pDIBChangeStart = pGdiNode->Gdi.pDIBChange;
		memblast(pGdiNode->Gdi.pDIBitmap,pGdiNode->Gdi.pDIB,pGdiNode->Gdi.dwLen);
		memblast(pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.pDIB,pGdiNode->Gdi.dwLen);

		// 不需要再分配内存
		pGdiNode->Gdi.fDIBitmap = TRUE;
		pGdiNode->Gdi.fChange = TRUE;

		pGdiNode->Gdi.dwCompress = pGdiNode->Gdi.dwLen;
		pGdiNode->Gdi.iStartPos = 0;
	}

	return pGdiNode->Gdi.fChange;
}

// 发送Resolution到客户端
void SendResolution(SOCKET MySocket)
{
	char	szMessage[81];
	DWORD	iSent,iRecv;

	// 建立屏幕宽度
	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"%d",iWidth);
	iSent = Transmit(MySocket,szMessage,strlen(szMessage));

	// 接收确认
	memset(szMessage,'\0',sizeof(szMessage));
	iRecv = recv(MySocket,szMessage,81,0);
	szMessage[iRecv] = '\0';

	// 建立屏幕的高度
	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"%d",iHeight);
	iSent = Transmit(MySocket,szMessage,strlen(szMessage));

	// 接收确认
	memset(szMessage,'\0',sizeof(szMessage));
	iRecv = recv(MySocket,szMessage,81,0);
	szMessage[iRecv] = '\0';
}

// 通过socket发送区域显示位图
int SendRegionDisplay(HWND hWnd,SOCKET MySocket)
{
	char	szMessage[81];
	DWORD	iSent,iRecv;
	int		fSend = FALSE;
	int		iUpdates;
	WORD	wTreeSize;
	DWORD	dwByteTree[768];
	DWORD	dwCodes[514];
	DWORD	dwCompLen,dwLastCompLen;
	BOOL	fTransmit;
	char	*pTempDIB;
	DWORD	dwMinCompress;

	// 指向GDI链表的起始位
	iUpdates = 0;
	pGdiNode = GdiStart.pNext;
	while (pGdiNode)
	{
		//为每个网格得到区域显示
		fSend = GetRegionDisplay(hWnd);
		// 计算需要发送的变化的显示区域的数目
		if (fSend)
			iUpdates++;
		// 移动到下一个节点
		pGdiNode = pGdiNode->pNext;
	}

	// 发送到客户端需要更新的显示区域的数目
	memset(szMessage,'\0',sizeof(szMessage));
	sprintf(szMessage,"%d",iUpdates);
	iSent = Transmit(MySocket,szMessage,strlen(szMessage));
	// 接收确认
	memset(szMessage,'\0',sizeof(szMessage));
	iRecv = recv(MySocket,szMessage,81,0);
	szMessage[iRecv] = '\0';

	if (iUpdates > 0)
	{
		// 指向GDI链表的起始位
		pGdiNode = GdiStart.pNext;
		while (pGdiNode)
		{
			// 如果桌面发生了变化，则发送DIB
			if (pGdiNode->Gdi.fChange)
			{
				int		iCompressions = 1;
				//无压缩
				if (iCompressionLevel == 0) 
				{
					pGdiNode->Gdi.pDIBCompress = (char *)malloc(pGdiNode->Gdi.dwCompress);
					memblast(pGdiNode->Gdi.pDIBCompress,pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.dwCompress);
					dwSendLen = pGdiNode->Gdi.dwCompress;
				}
				if (iCompressionLevel == 10) // 单遍霍夫曼编码压缩
				{
					pGdiNode->Gdi.pDIBCompress = (char *)malloc(pGdiNode->Gdi.dwCompress + 1536);
					// 生成霍夫曼字节树字典
					wTreeSize = HuffmanDictionary(pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.dwCompress,&dwByteTree[0],&dwCodes[0]);
					// 使用霍夫曼压缩方法压缩图片
					dwSendLen = HuffmanCompress(pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.dwCompress,wTreeSize,&dwByteTree[0],&dwCodes[0],pGdiNode->Gdi.pDIBCompress);
				}
				else if (iCompressionLevel == 11) // 多遍霍夫曼压缩编码
				{
					dwMinCompress = pGdiNode->Gdi.dwCompress + 1536;
					pGdiNode->Gdi.pDIBCompress = (char *)malloc(dwMinCompress);
					pTempDIB = (char *)malloc(pGdiNode->Gdi.dwCompress);
					memblast(pTempDIB,pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.dwCompress);
					iCompressions = 0;
					dwCompLen = pGdiNode->Gdi.dwCompress;
					dwLastCompLen = dwCompLen;
					for (;;)
					{
						// 生成霍夫曼字节树字典
						wTreeSize = HuffmanDictionary(pTempDIB,pGdiNode->Gdi.dwCompress,&dwByteTree[0],&dwCodes[0]);
						// 计算压缩长度
						dwCompLen = HuffmanCountCompress(pTempDIB,dwCompLen,wTreeSize,&dwCodes[0]);
						if (dwCompLen < dwMinCompress)
						{
							dwSendLen = HuffmanCompress(pTempDIB,dwLastCompLen,wTreeSize,&dwByteTree[0],&dwCodes[0],pGdiNode->Gdi.pDIBCompress);
							memblast(pTempDIB,pGdiNode->Gdi.pDIBCompress,dwSendLen);
							dwMinCompress = dwSendLen;
							dwLastCompLen = dwCompLen;
							iCompressions++;
						}
						else
							break;
					}
					free(pTempDIB);
				}
				else if (iCompressionLevel == 12) // Run Length编码
				{
					//为最坏的情况分配压缩空间
					pGdiNode->Gdi.pDIBCompress = (char *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					// Run Length编码图象
					dwSendLen = RunLengthEncode(pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.dwCompress,pGdiNode->Gdi.pDIBCompress);
				}
				else if (iCompressionLevel == 13) // Run Length&Huffman编码
				{
					pTempDIB = (char *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					pGdiNode->Gdi.pDIBCompress = (char *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					// Run Length 编码图象
					dwCompLen = RunLengthEncode(pGdiNode->Gdi.pDIBChange,pGdiNode->Gdi.dwCompress,pTempDIB);
					// 生成霍夫曼字节树的字典
					wTreeSize = HuffmanDictionary(pTempDIB,dwCompLen,&dwByteTree[0],&dwCodes[0]);
					// 使用霍夫曼压缩Run Lenght编码的图象
					dwSendLen = HuffmanCompress(pTempDIB,dwCompLen,wTreeSize,&dwByteTree[0],&dwCodes[0],pGdiNode->Gdi.pDIBCompress);
					// 释放临时的DIB
					free(pTempDIB);
				}

				// 建立位图控制消息
				memset(szMessage,'\0',sizeof(szMessage));
				sprintf(szMessage,"%d;%d;%d;%ld;%ld;%ld;",
					iCompressions,
					pGdiNode->Gdi.iGridX,pGdiNode->Gdi.iGridY,
					pGdiNode->Gdi.iStartPos,
					pGdiNode->Gdi.dwCompress,dwSendLen);
				// 发送控制消息
				iSent = Transmit(MySocket,szMessage,strlen(szMessage));

				// 接收确认
				memset(szMessage,'\0',sizeof(szMessage));
				iRecv = recv(MySocket,szMessage,81,0);
				szMessage[iRecv] = '\0';

				// 发送压缩的DIB
				fTransmit = Transmit(MySocket,pGdiNode->Gdi.pDIBCompress,dwSendLen);
				// 释放压缩的DIB
				free(pGdiNode->Gdi.pDIBCompress);
				// 接收确认
				memset(szMessage,'\0',sizeof(szMessage));
				iRecv = recv(MySocket,szMessage,81,0);
				szMessage[iRecv] = '\0';
			}
			pGdiNode = pGdiNode->pNext;
		}
	}
	return iUpdates;
}

//传输数据到客户端
BOOL Transmit(SOCKET MySocket,char *pData,DWORD dwLength)
{
	WSAOVERLAPPED	olSend;
	WSAEVENT	gheventOlSock;
	WSAEVENT	eventArray[2];
	WSABUF		buffSend;
	DWORD		dwRet,dwNumBytes,dwFlags;
	int			nWSAError;
	char		szError[81];

	// 为发送完成创建一个信号事件
	gheventOlSock = WSACreateEvent();
	eventArray[0] = gheventOlSock;

	// 初始化重叠发送的结构
	ZeroMemory(&olSend, sizeof(WSAOVERLAPPED));
	// 为发送重叠结构创建一个信号时间
	olSend.hEvent= gheventOlSock;
	buffSend.len = dwLength;
	buffSend.buf = pData;
	// 持续发送，直到dwSendLen个字节被发送完成
	while (TRUE)
	{
		if ((dwRet = WSASend(MySocket,&buffSend,1,&dwNumBytes,0,&olSend,NULL)) == SOCKET_ERROR)
		{
			nWSAError= WSAGetLastError();
			if (nWSAError != ERROR_IO_PENDING)
			{
				sprintf(szError,"WSASend failed with error %d\n",nWSAError);
				MessageBox(NULL,szError,"Server",MB_OK);
			}
		}
	
		if (WSAWaitForMultipleEvents(1,eventArray,FALSE,WSA_INFINITE,FALSE) == WSA_WAIT_FAILED)
		{
			sprintf(szError,"WSAWaitForMultipleEvents failed %d\n", WSAGetLastError());
			MessageBox(NULL,szError,"Server",MB_OK);
		} 
		// 重置gheventOlSock
		WSAResetEvent(eventArray[0]);
		if (WSAGetOverlappedResult(MySocket,&olSend,&dwNumBytes,FALSE,&dwFlags) == FALSE)
		{
			sprintf(szError,"WSAGetOverlappedResult failed with error %d\n", WSAGetLastError());
			MessageBox(NULL,szError,"Server",MB_OK);
		}
		buffSend.len -= dwNumBytes;
		if (buffSend.len == 0) 
			break;
		else 
			buffSend.buf += dwNumBytes;
	}

	// 关闭信号事件
	WSACloseEvent(gheventOlSock);
	return TRUE;
}

//返回本机安装的所有的有XP1_GUARANTEED_DELIVERY and XP1_GUARANTEED_ORDER标志的协议和协议链
int SelectProtocols(DWORD dwSetFlags,DWORD dwNotSetFlags,LPWSAPROTOCOL_INFO lpProtocolBuffer,LPDWORD lpdwBufferLength,WSAPROTOCOL_INFO *pProtocol)
{
	LPBYTE				pBuf;
	LPWSAPROTOCOL_INFO	pInfo;
	DWORD				dwNeededLen;
	LPWSAPROTOCOL_INFO	pRetInfo;
	DWORD				dwRetLen;
	int					nCount;
	int					nMatchCount;
	int					nRet;

	// 决定需要的缓冲区大小
	dwNeededLen = 0;
	nRet = WSAEnumProtocols(NULL, NULL, &dwNeededLen);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAENOBUFS)
			return SOCKET_ERROR;
	}
	// 分配大小
	pBuf = malloc(dwNeededLen);
	if (pBuf == NULL)
	{
		WSASetLastError(WSAENOBUFS);
		return SOCKET_ERROR;
	}

	nRet = WSAEnumProtocols(NULL,(LPWSAPROTOCOL_INFO)pBuf,&dwNeededLen);
	if (nRet == SOCKET_ERROR)
	{
		free(pBuf);
		return SOCKET_ERROR;
	}

	#define REJECTSET(f) \
	    ((dwSetFlags & f) && !(pInfo->dwServiceFlags1 & f))
	#define REJECTNOTSET(f) \
	    ((dwNotSetFlags &f) && (pInfo->dwServiceFlags1 & f))
	#define REJECTEDBY(f) (REJECTSET(f) || REJECTNOTSET(f))

	pInfo = (LPWSAPROTOCOL_INFO)pBuf;	
	pRetInfo = lpProtocolBuffer;
	dwRetLen = 0;
	nMatchCount = 0;
	for(nCount = 0; nCount < nRet; nCount++)
	{
		while(1)
		{
			if (REJECTEDBY(XP1_CONNECTIONLESS))
				break;
			if (REJECTEDBY(XP1_GUARANTEED_DELIVERY))
				break;
			if (REJECTEDBY(XP1_GUARANTEED_ORDER))
				break;
			if (REJECTEDBY(XP1_MESSAGE_ORIENTED))
				break;
			if (REJECTEDBY(XP1_PSEUDO_STREAM))
				break;
			if (REJECTEDBY(XP1_GRACEFUL_CLOSE))
				break;
			if (REJECTEDBY(XP1_EXPEDITED_DATA))
				break;
			if (REJECTEDBY(XP1_CONNECT_DATA))
				break;
			if (REJECTEDBY(XP1_DISCONNECT_DATA))
				break;
			if (REJECTEDBY(XP1_SUPPORT_BROADCAST)) 
				break;
			if (REJECTEDBY(XP1_SUPPORT_MULTIPOINT))
				break;
			if (REJECTEDBY(XP1_MULTIPOINT_DATA_PLANE))
				break;
			if (REJECTEDBY(XP1_QOS_SUPPORTED))
				break;
			if (REJECTEDBY(XP1_UNI_SEND))
				break;
			if (REJECTEDBY(XP1_UNI_RECV))
				break;
			if (REJECTEDBY(XP1_IFS_HANDLES))
				break;
			if (REJECTEDBY(XP1_PARTIAL_MESSAGE))
				break;

			dwRetLen += sizeof(WSAPROTOCOL_INFO);

			if (dwRetLen > *lpdwBufferLength)
			{
				WSASetLastError(WSAENOBUFS);
				*lpdwBufferLength = dwNeededLen;
				free(pBuf);
				return SOCKET_ERROR;
			}
			nMatchCount++;
			// 拷贝协议到调用者的buffer里
			memblast(pRetInfo,pInfo,sizeof(WSAPROTOCOL_INFO));
			if (strcmp(pInfo->szProtocol,"MSAFD Tcpip [TCP/IP]") == 0)
				memblast(pProtocol,pInfo,sizeof(WSAPROTOCOL_INFO));
			pRetInfo++;
			break;
		}
		pInfo++;
	}
	free(pBuf);
	*lpdwBufferLength = dwRetLen;
	return(nMatchCount);
}

void memblast(void* dest,void* src,DWORD count)
{
	DWORD	iCount;

	__asm
	{
		MOV		ECX,count
		SHR		ECX,2
		SHL		ECX,2
		MOV		iCount,ECX

		MOV		ESI,src
		MOV		EDI,dest
		MOV		ECX,iCount
		SHR		ECX,2	
		REP		MOVSD	

		MOV		ECX,count
		MOV		EAX,iCount
		SUB		ECX,EAX

		JZ		Exit

		MOV		ESI,src	
		ADD		ESI,EAX	
		MOV		EDI,dest	
		ADD		EDI,EAX		
		REP		MOVSB		
		Exit:
	}
}
/*
int GetLocalHostName(char* sHostName)	//获得本地计算机名称
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
int GetIpAddress(char *sHostName, BYTE *f0,BYTE *f1,BYTE *f2,BYTE *f3)//获得本地IP
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		f0=f1=f2=f3=0;
		return GetLastError();
	}
	//获取IP
	if(lpHostEnt->h_addr_list[0])
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpHostEnt->h_addr_list[0],4);
		*f0=inAddr.S_un.S_un_b.s_b1;
		*f1=inAddr.S_un.S_un_b.s_b2;
		*f2=inAddr.S_un.S_un_b.s_b3;
		*f3=inAddr.S_un.S_un_b.s_b4;
	}
	return 0;
}
*/