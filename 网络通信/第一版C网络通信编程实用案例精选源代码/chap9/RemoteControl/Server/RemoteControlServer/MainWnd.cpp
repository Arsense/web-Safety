#include "MainWnd.h"
#include "WndProc.h"
#include "Server.h"
#include "Gdi.h"
#include "resource.h"

//引用到的外部全局变量
extern	"C"	SOCKET	Listen;
extern	"C"	HWND	hServerWnd;
extern	"C"	BOOL	fDIBitmap;
extern	"C"	struct	GdiList	GdiStart;
extern	"C"	struct	GdiList	*pGdiNode;
extern  "C" UINT    gPort;
// 线程变量
HANDLE	hThread;
DWORD	dwThreadId;

//因为IP地址是一个全局信息，所以我们把它设置为CMainWnd的静态变量
BYTE	CMainWnd::m_ip[4];

CMainWnd::CMainWnd()
{

}

CMainWnd::~CMainWnd()
{
	// 删除自窗口自身的指针
	delete this;

	// 释放全局区域的DIB
	ClearDisplay(m_hWnd);
}

// 设置程序Instance
void CMainWnd::SetInstance(HINSTANCE hInst)
{
	m_hInst = hInst;
}

// 处理WM_CREATE消息
BOOL CMainWnd::OnCreate(HWND hWnd,CREATESTRUCT FAR *lpCreateStruct)
{
	// 显示主窗口
	lpCreateStruct->cx=400;
	lpCreateStruct->cy=600;
	lpCreateStruct->x=100;
	lpCreateStruct->y=100;

	ShowWindow(hWnd,*(LPINT)lpCreateStruct->lpCreateParams);
	
	m_hWnd = hWnd;

	m_hMenu = GetMenu(m_hWnd);
	EnableMenuItem(m_hMenu,IDM_STARTSERVER,MF_BYCOMMAND|MF_ENABLED);
	EnableMenuItem(m_hMenu,IDM_STOPSERVER,MF_BYCOMMAND|MF_GRAYED);
	
	// Winsock初始化的地方
	WSADATA				wsd;
	char				szString[255];
	char				name[20];

	// 装入Winsock2.2版本
	if (WSAStartup(0x202,&wsd) != 0)
	{
		sprintf(szString,"Failed to load Winsock!");
		MessageBox(NULL,szString,"Remote Server",MB_OK);
		return 1;
	}
	//得到本机的IP地址
	GetLocalHostName(name);
	GetIpAddress(name,&m_ip[0],&m_ip[1],&m_ip[2],&m_ip[3]);
	return TRUE;
}

// 处理WM_DESTORY消息
void CMainWnd::OnDestroy(HWND hWnd)
{
	// 关闭监听Socket
	closesocket(Listen);
	// 清除socket
	WSACleanup();
	// 发送结束消息到窗口
	PostMessage(hWnd,WM_QUIT,(WPARAM)0,0L);
}

// 处理WM_GETMINMAXINFO消息
void CMainWnd::OnGetMinMaxInfo(HWND hWnd,MINMAXINFO FAR* lpMinMaxInfo)
{
	RECT size;
	size.bottom=1000;
	size.top=100;
	size.left=100;
	size.right=1000;
	lpMinMaxInfo->ptMaxTrackSize.x = size.right-size.left;
	lpMinMaxInfo->ptMaxTrackSize.y = size.bottom-size.top;

	lpMinMaxInfo->ptMaxSize.x = size.right-size.left;
	lpMinMaxInfo->ptMaxSize.y = size.bottom-size.top;
}

// 处理WM_KEYDOWN消息
void CMainWnd::OnKeyDown(HWND hWnd,UINT vk,BOOL fDown,int cRepeat,UINT flags)
{
	if (vk == VK_ESCAPE || vk == VK_F12)
		PostMessage(hWnd,WM_CLOSE,0,0);
}
//对话框处理的回调函数
BOOL CALLBACK SettingItemProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	BOOL b;
	char name[15];
    switch (message) 
    { 
	  case WM_INITDIALOG:
		  sprintf(name,"%d.%d.%d.%d",CMainWnd::m_ip[0],CMainWnd::m_ip[1],CMainWnd::m_ip[2],CMainWnd::m_ip[3]);
   	     ::SetDlgItemInt(hwndDlg,IDC_EDIT1,gPort,false);
		 ::SetDlgItemText(hwndDlg,IDC_IP,name);
			return TRUE;
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                  gPort=::GetDlgItemInt(hwndDlg, IDC_EDIT1,&b, false);                     
                case IDCANCEL: 
                    EndDialog(hwndDlg, wParam); 
                    return TRUE; 
            } 
    } 
    return FALSE; 
} 
//处理WM_COMMAND消息
void CMainWnd::OnCommand(HWND hWnd,int iId,HWND hWndCtl,UINT iCodeNotify)
{
	if (iId == IDM_EXIT)
	{
		//终结线程
		if (hThread)
		{
			//关闭监听窗口
			closesocket(Listen);
			// 清除socket
			WSACleanup();
		}
		// 结束程序
		PostQuitMessage(0);
	}
	else if (iId == IDM_STARTSERVER)
	{
       if (DialogBox(this->m_hInst, 
                     MAKEINTRESOURCE(IDD_DIALOG), 
                     hWnd, (DLGPROC)SettingItemProc)==IDOK)
	   {
		ShowWindow(hWnd,SW_MINIMIZE);

		RedrawWindow(hWnd,NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

		// 创建一个新的线程，LoadWinsock是在Server.c中定义的线程函数
		hThread = CreateThread(NULL,0,LoadWinsock,(LPVOID)NULL,0,&dwThreadId);
		if (hThread == NULL)
			MessageBox(hWnd,"Socket线程创建失败","远程控制服务器端",MB_ICONEXCLAMATION|MB_OK);
		else
		{
			InitDisplay(hWnd);
			hServerWnd = hWnd;
			EnableMenuItem(m_hMenu,IDM_STARTSERVER,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(m_hMenu,IDM_STOPSERVER,MF_BYCOMMAND|MF_ENABLED);
		}
	   }
	}
	else if (iId == IDM_STOPSERVER)
	{
		if(MessageBox(hWnd,"你真的想关闭远程控制服务?","远程控制服务器端",MB_YESNO)==IDYES)
		{
			EnableMenuItem(m_hMenu,IDM_STARTSERVER,MF_BYCOMMAND|MF_ENABLED);
			EnableMenuItem(m_hMenu,IDM_STOPSERVER,MF_BYCOMMAND|MF_GRAYED);

			ClearDisplay(hWnd);

			// 终结线程
			if (hThread)
			{
				// 清除监听窗口
				closesocket(Listen);
			}
		}
	}
	else if(iId==IDC_About)
	{
		MessageBox(hWnd,"2003-7-2创建\r\n一个远程控制的程序","远程控制服务器端",MB_OK);
	}
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
