#if !defined(MAINWND_H)
#define MAINWND_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <time.h>

// 主框架消息处理
class	CMainWnd
{
public:

	CMainWnd();
	~CMainWnd();
	static	BYTE				m_ip[4];

	HINSTANCE			m_hInst;
	HWND				m_hWnd;
	HMENU				m_hMenu;

	HDC					m_hMemDC;
	HBITMAP				m_hDDBitmap;

	int					iVScrollPos,iHScrollPos;
	RECT				m_Rect;

	long				m_iTimerInterval;

	void	SetInstance(HINSTANCE hInst);

	BOOL	OnCreate(HWND hWnd,CREATESTRUCT FAR* lpCreateStruct);
	void	OnDestroy(HWND hWnd);
	void	OnSize(HWND hWnd,UINT state,int cx,int cy);
	void	OnPaint(HWND hWnd);
	void	OnTimer(HWND hWnd,UINT iId);
	void	OnCommand(HWND hWnd,int iId,HWND hWndCtl,UINT iCodeNotify);
	void	OnVScroll(HWND hWnd,HWND hWndCtl,UINT iCode,int iPos);
	void	OnHScroll(HWND hWnd,HWND hWndCtl,UINT iCode,int iPos);
	void	GetScreen();
	void	OnLButtonDblClk(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags);
	void	OnLButtonDown(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags);
	void	OnLButtonUp(HWND hWnd,int x,int y,UINT keyFlags);
	void	OnMButtonDblClk(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags);
	void	OnMButtonDown(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags);
	void	OnMButtonUp(HWND hWnd,int x,int y,UINT keyFlags);
	void	OnRButtonDblClk(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags);
	void	OnRButtonDown(HWND hWnd,BOOL fDoubleClick,int x,int y,UINT keyFlags);
	void	OnRButtonUp(HWND hWnd,int x,int y,UINT keyFlags);
	void	OnMouseMove(HWND hWnd,int x,int y,UINT keyFlags);
	void	OnKeyDown(HWND hWnd,UINT vk,BOOL fDown,int cRepeat,UINT flags);
	void	OnKeyUp(HWND hWnd,UINT vk,BOOL fDown,int cRepeat,UINT flags);
	int		GetLocalHostName(char* sHostName);	//获得本地计算机名称
	int		GetIpAddress(char *sHostName, char *sIpAddress);//获得本地IP
	int		GetIpAddress(char *sHostName, BYTE *f0,BYTE *f1,BYTE *f2,BYTE *f3);//获得本地IP

};

#endif