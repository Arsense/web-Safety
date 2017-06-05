//一些回调函数
HWND				WinInit(HINSTANCE hInst,int iShow);
LRESULT	CALLBACK	WndProc (HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);
LRESULT	CALLBACK	ServerIPDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK	CompressionDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK	GridSpacingDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK	ColorModeDlgProc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam);
