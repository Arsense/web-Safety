#if !defined(COLORMODE_H)
#define COLORMODE_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

//颜色模式对话框的处理
class	CColorMode
{
public:

	CColorMode();
	~CColorMode();

	// 消息处理函数
	BOOL	OnInitDialog(HWND hDlg,HWND hwndFocus, LPARAM lParam);
	void	OnCommand(HWND hDlg,int iId,HWND hWndCtl,UINT iCodeNotify);

	int		m_bmBitsPixel;
};

#endif