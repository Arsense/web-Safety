#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define CLASSNAME "ClientWindow"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "MainWnd.h"
#include "WndProc.h"
#include "ServerIP.h"
#include "Compression.h"
#include "GridSpacing.h"
#include "ColorMode.h"
#include "resource.h"

// 全局主窗口对象
CMainWnd	*pWindow = new CMainWnd();

// 全局ServerIP对话框对象
CServerIP	*pServerIP = new CServerIP();

// 全局压缩选项对话框
CCompression	*pCompression = new CCompression();

// 全局网格空间对话框
CGridSpacing	*pGridSpacing = new CGridSpacing();

// 全局颜色模式对话框
CColorMode		*pColorMode = new CColorMode();

// windows 入口
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iShow)
{
	MSG iMsg;

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
	WNDCLASSEX	wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	// 消息处理函数
	wc.lpfnWndProc = WndProc; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL,IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = CLASSNAME;
	wc.hIconSm = NULL;

	// 创建窗口
	if (RegisterClassEx(&wc))
	{
		// 创建主窗口
		return CreateWindow(CLASSNAME,"Client Socket",WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,0,0,640,480,NULL,NULL,hInstance,&iShow);
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
		HANDLE_MSG(hWnd,WM_SIZE,pWindow->OnSize);
		HANDLE_MSG(hWnd,WM_PAINT,pWindow->OnPaint);
		HANDLE_MSG(hWnd,WM_TIMER,pWindow->OnTimer);
		HANDLE_MSG(hWnd,WM_COMMAND,pWindow->OnCommand);
		HANDLE_MSG(hWnd,WM_VSCROLL,pWindow->OnVScroll);
		HANDLE_MSG(hWnd,WM_HSCROLL,pWindow->OnHScroll);
		HANDLE_MSG(hWnd,WM_LBUTTONDBLCLK,pWindow->OnLButtonDblClk);
		HANDLE_MSG(hWnd,WM_LBUTTONDOWN,pWindow->OnLButtonDown);
		HANDLE_MSG(hWnd,WM_LBUTTONUP,pWindow->OnLButtonUp);
		HANDLE_MSG(hWnd,WM_MBUTTONDBLCLK,pWindow->OnMButtonDblClk);
		HANDLE_MSG(hWnd,WM_MBUTTONDOWN,pWindow->OnMButtonDown);
		HANDLE_MSG(hWnd,WM_MBUTTONUP,pWindow->OnMButtonUp);
		HANDLE_MSG(hWnd,WM_RBUTTONDBLCLK,pWindow->OnRButtonDblClk);
		HANDLE_MSG(hWnd,WM_RBUTTONDOWN,pWindow->OnRButtonDown);
		HANDLE_MSG(hWnd,WM_RBUTTONUP,pWindow->OnRButtonUp);
		HANDLE_MSG(hWnd,WM_MOUSEMOVE,pWindow->OnMouseMove);
		HANDLE_MSG(hWnd,WM_KEYDOWN,pWindow->OnKeyDown);
		HANDLE_MSG(hWnd,WM_KEYUP,pWindow->OnKeyUp);

	default:
			
		return DefWindowProc(hWnd,iMsg,wParam,lParam);
	}
}

LRESULT CALLBACK ServerIPDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch (iMsg)
	{
		HANDLE_MSG(hDlg,WM_INITDIALOG,pServerIP->OnInitDialog);
		HANDLE_MSG(hDlg,WM_COMMAND,pServerIP->OnCommand);

	default:
			
		return DefWindowProc(hDlg,iMsg,wParam,lParam);
	}
}

LRESULT CALLBACK CompressionDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch (iMsg)
	{
		HANDLE_MSG(hDlg,WM_INITDIALOG,pCompression->OnInitDialog);
		HANDLE_MSG(hDlg,WM_COMMAND,pCompression->OnCommand);

	default:
			
		return DefWindowProc(hDlg,iMsg,wParam,lParam);
	}
}

LRESULT CALLBACK GridSpacingDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch (iMsg)
	{
		HANDLE_MSG(hDlg,WM_INITDIALOG,pGridSpacing->OnInitDialog);
		HANDLE_MSG(hDlg,WM_COMMAND,pGridSpacing->OnCommand);

	default:
			
		return DefWindowProc(hDlg,iMsg,wParam,lParam);
	}
}

LRESULT CALLBACK ColorModeDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch (iMsg)
	{
		HANDLE_MSG(hDlg,WM_INITDIALOG,pColorMode->OnInitDialog);
		HANDLE_MSG(hDlg,WM_COMMAND,pColorMode->OnCommand);

	default:
			
		return DefWindowProc(hDlg,iMsg,wParam,lParam);
	}
}