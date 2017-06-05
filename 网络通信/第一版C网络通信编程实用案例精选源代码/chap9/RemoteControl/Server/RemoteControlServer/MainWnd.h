#if !defined(MAINWND_H)
#define MAINWND_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

//消息处理的主窗口
class	CMainWnd
{
public:

	CMainWnd();
	~CMainWnd();

	HINSTANCE			m_hInst;
	HWND				m_hWnd;
	HMENU				m_hMenu;
	static	BYTE				m_ip[4];

	// 存储Instance
	void	SetInstance(HINSTANCE hInst);
	//获得本地计算机名称
	int GetLocalHostName(char* sHostName);	
	//获得本地IP
	int GetIpAddress(char *sHostName, char *sIpAddress);
	//获得本地IP
	int GetIpAddress(char *sHostName, BYTE *f0,BYTE *f1,BYTE *f2,BYTE *f3);


	// 消息处理函数
	BOOL	OnCreate(HWND hWnd,CREATESTRUCT FAR* lpCreateStruct);
	void	OnDestroy(HWND hWnd);
	void	OnGetMinMaxInfo(HWND hWnd,MINMAXINFO FAR *lpMinMaxInfo);
	void	OnKeyDown(HWND hWnd,UINT vk,BOOL fDown,int cRepeat,UINT flags);
	void	OnCommand(HWND hWnd,int iId,HWND hWndCtl,UINT iCodeNotify);
};

#endif