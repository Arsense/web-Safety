#include "GridSpacing.h"
#include "resource.h"

CGridSpacing::CGridSpacing()
{
}

CGridSpacing::~CGridSpacing()
{
}

CGridSpacing::OnInitDialog(HWND hDlg,HWND hwndFocus, LPARAM lParam)
{
	m_hHGrid = GetDlgItem(hDlg,IDC_HGRID);
	m_hVGrid = GetDlgItem(hDlg,IDC_VGRID);
	m_hHGridc = GetDlgItem(hDlg,IDC_HGRIDC);
	m_hVGridc = GetDlgItem(hDlg,IDC_VGRIDC);

	sprintf(m_szText,"%d",m_nGridX);
	Edit_SetText(m_hHGrid,m_szText);

	sprintf(m_szText,"%d",m_nGridY);
	Edit_SetText(m_hVGrid,m_szText);

	Button_SetCheck(m_hHGridc,m_fGridX);
	Button_SetCheck(m_hVGridc,m_fGridY);
	return TRUE;
}

void CGridSpacing::OnCommand(HWND hDlg,int iId,HWND hWndCtl,UINT iCodeNotify)
{
	BOOL	fExit = TRUE;
	if (iId == IDOK)
	{
		Edit_GetText(m_hHGrid,m_szText,10);
		m_iLength = strlen(m_szText);
		if (m_iLength > 0)
			m_nGridX = atoi(m_szText);
		else
			fExit = FALSE;

		Edit_GetText(m_hVGrid,m_szText,10);
		m_iLength = strlen(m_szText);
		if (m_iLength > 0)
			m_nGridY = atoi(m_szText);
		else
			fExit = FALSE;

		if ((m_nGridX == 0) && (m_nGridY == 0))
			fExit = FALSE;
		if ((m_nGridX > 20) || (m_nGridY > 20))
			fExit = FALSE;

		m_fGridX = Button_GetCheck(m_hHGridc);
		m_fGridY = Button_GetCheck(m_hVGridc);
		if (fExit)
			EndDialog(hDlg,TRUE);
	}
	else if (iId == IDDEFAULT)
	{
		sprintf(m_szText,"2");
		Edit_SetText(m_hHGrid,m_szText);

		sprintf(m_szText,"2");
		Edit_SetText(m_hVGrid,m_szText);

		Button_SetCheck(m_hHGridc,FALSE);
		Button_SetCheck(m_hVGridc,FALSE);
	}
	else if (iId == IDCANCEL)
		EndDialog(hDlg,FALSE);
}
