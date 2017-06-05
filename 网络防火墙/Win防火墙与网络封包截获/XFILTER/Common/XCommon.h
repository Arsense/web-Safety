#ifndef XCOMMON_H
#define XCOMMON_H

class CXCommon
{
public:
	static	int		GetBit		(BYTE bit, int index, int count = 1);
	static	int		SetBit		(BYTE* bit, int index, BOOL isTrue);
	static	CString	DIPToSIP	(DWORD* pIP);
	static  CString	GetAppPath	(BOOL IsDLL = FALSE, HINSTANCE instance = NULL,  BOOL IsFullPathName = FALSE);
	static	CString GetPath		(TCHAR *sFilename);
	static	CString GetName		(TCHAR *sFilename);
};

#endif