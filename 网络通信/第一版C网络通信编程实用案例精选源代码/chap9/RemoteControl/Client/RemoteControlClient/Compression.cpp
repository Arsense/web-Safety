#include "Compression.h"
#include "resource.h"

CCompression::CCompression()
{
}
CCompression::~CCompression()
{
}

CCompression::OnInitDialog(HWND hDlg,HWND hwndFocus, LPARAM lParam)
{
	// Set the Compression Level Radio Button
	if (m_iCompressionLevel == 0) // No Compression
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO0),TRUE);
	else if (m_iCompressionLevel == 10) // Huffman Compression
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO10),TRUE);
	else if (m_iCompressionLevel == 11) // Multi Pass Huffman Compression
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO11),TRUE);
	else if (m_iCompressionLevel == 12) // Run Length Encoding
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO12),TRUE);
	else if (m_iCompressionLevel == 13) // Run Length Encoding Plus Huffman
		Button_SetCheck(GetDlgItem(hDlg,IDC_RADIO13),TRUE);

	return TRUE;
}

// Dialog WM_COMMAND Handling
void CCompression::OnCommand(HWND hDlg,int iId,HWND hWndCtl,UINT iCodeNotify)
{
	if (iId == IDOK)
	{
		// Set the New Compression Level
		if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO0))) // No Compression
			m_iCompressionLevel = 0;
		else if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO10))) // Huffman Compression
			m_iCompressionLevel = 10;
		else if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO11))) // Multi Pass Huffman Compression
			m_iCompressionLevel = 11;
		else if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO12))) // Run Length Encoding
			m_iCompressionLevel = 12;
		else if (Button_GetState(GetDlgItem(hDlg,IDC_RADIO13))) // Run Length Encoding Plus Huffman
			m_iCompressionLevel = 13;

		// End the Dialog
		EndDialog(hDlg,TRUE);
	}
	else if (iId == IDCANCEL)
		EndDialog(hDlg,FALSE);
}
