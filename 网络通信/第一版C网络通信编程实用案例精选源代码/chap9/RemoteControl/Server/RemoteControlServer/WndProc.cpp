#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define CLASSNAME "ServerSocket"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "MainWnd.h"
#include "WndProc.h"
#include "resource.h"

#define DEFAULT_PORT        5150
#define DEFAULT_BUFFER      4096

// 全局Window对象
CMainWnd		*pWindow = new CMainWnd();

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iShow)
{
	MSG		iMsg;

	if (!hPrevInstance && !WinInit(hInstance,iShow))
		return FALSE;
	pWindow->SetInstance(hInstance);
    for(;;)
	{
		if(PeekMessage(&iMsg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&iMsg,NULL,0,0))
				return iMsg.wParam;

			TranslateMessage(&iMsg);
			DispatchMessage(&iMsg);
        }
		else
			WaitMessage();
	}
}

HWND WinInit(HINSTANCE hInstance, int iShow)
{
	WNDCLASS	wc;
	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WndProc; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL,IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = CLASSNAME;

	if (RegisterClass(&wc))
	{
		return CreateWindow(CLASSNAME,"Server Socket",WS_OVERLAPPEDWINDOW,0,0,200,70,NULL,NULL,hInstance,&iShow);
	}
	else
		return NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch (iMsg)
	{
		HANDLE_MSG(hWnd,WM_CREATE,pWindow->OnCreate);
		HANDLE_MSG(hWnd,WM_DESTROY,pWindow->OnDestroy);
//		HANDLE_MSG(hWnd,WM_GETMINMAXINFO,pWindow->OnGetMinMaxInfo);
		HANDLE_MSG(hWnd,WM_KEYDOWN,pWindow->OnKeyDown);
		HANDLE_MSG(hWnd,WM_COMMAND,pWindow->OnCommand);

	default:
			
		return DefWindowProc(hWnd,iMsg,wParam,lParam);
	}
}
