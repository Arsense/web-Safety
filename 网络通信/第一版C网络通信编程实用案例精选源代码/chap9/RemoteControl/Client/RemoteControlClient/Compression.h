#if !defined(COMPRESSION_H)
#define COMPRESSION_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

//压缩选项对话框的处理
class	CCompression
{
public:

	CCompression();
	~CCompression();

	BOOL	OnInitDialog(HWND hDlg,HWND hwndFocus, LPARAM lParam);
	void	OnCommand(HWND hDlg,int iId,HWND hWndCtl,UINT iCodeNotify);

	int		m_iCompressionLevel;
};

#endif