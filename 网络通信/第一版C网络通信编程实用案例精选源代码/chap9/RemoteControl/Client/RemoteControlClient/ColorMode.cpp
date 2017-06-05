#include "ColorMode.h"
#include "resource.h"

CColorMode::CColorMode()
{
}

CColorMode::~CColorMode()
{
}

//对话框初始化
CColorMode::OnInitDialog(HWND hDlg,HWND hwndFocus, LPARAM lParam)
{
	// 设置颜色模式
	if (m_bmBitsPixel == 4)
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO0),TRUE);
	else
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO1),TRUE);

	return TRUE;
}

//对话框WM_COMMAND消息处理
void CColorMode::OnCommand(HWND hDlg,int iId,HWND hWndCtl,UINT iCodeNotify)
{
	if (iId == IDOK)
	{
		//设置新的颜色模式
		if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO0)))
			m_bmBitsPixel = 4;
		else if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO1)))
			m_bmBitsPixel = 8;
		EndDialog(hDlg,TRUE);
	}
	else if (iId == IDCANCEL)
		EndDialog(hDlg,FALSE);
}
