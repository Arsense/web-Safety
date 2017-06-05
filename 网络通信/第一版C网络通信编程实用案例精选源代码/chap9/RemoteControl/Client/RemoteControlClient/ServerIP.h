#if !defined(SERVERIP_H)
#define SERVERIP_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

class	CServerIP
{
public:

	CServerIP();
	~CServerIP();

	BOOL	OnInitDialog(HWND hDlg,HWND hwndFocus, LPARAM lParam);
	void	OnCommand(HWND hDlg,int iId,HWND hWndCtl,UINT iCodeNotify);

	char	szIP[255];
	UINT	port;
};

#endif