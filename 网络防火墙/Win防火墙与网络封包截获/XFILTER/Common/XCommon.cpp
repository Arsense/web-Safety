#include "stdafx.h"
#include "xcommon.h"

CString CXCommon::DIPToSIP(DWORD* pIP)
{
	if(pIP == NULL)
		return _T("");

	CString	s;
	BYTE	*b = (BYTE*)pIP;
	s.Format(_T("%d.%d.%d.%d"),b[3],b[2],b[1],b[0]);

	return s;
}

/*---------------------------------------------------------------------------------------------
	index from 0 start, for example:
	index:				0 1 2 3 4 5 6 7
	Binary value:		0 0 0 0 0 0 0 0
*/
int	CXCommon::GetBit(BYTE bit, int index, int count)
{
	bit <<= index;
	bit >>= (8 - count);

	return bit;
}

int CXCommon::SetBit(BYTE* bit, int index, BOOL isTrue)
{
	BYTE bOr = 0xFF,bAnd = 0x00;

	bOr <<= index;
	bOr >>= 7;
	bOr <<= (7 - index);
	bAnd = ~bOr;

	if(isTrue)
		*bit = *bit | bOr;
	else
		*bit = *bit & bAnd;

	return 0;
}

//============================================================================================
//static function

CString CXCommon::GetAppPath(BOOL IsDLL, HINSTANCE instance, BOOL IsFullPathName) 
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	if(IsDLL)
		GetModuleFileName(instance, sFilename, _MAX_PATH);
	else
		GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);

	if(IsFullPathName)
		return sFilename;

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal(CString(sDrive) + CString(sDir));
	int nLen = rVal.GetLength();

	if (rVal.GetAt(nLen-1) != _T('\\'))
		rVal += _T("\\");

	return rVal;
}

CString CXCommon::GetPath(TCHAR *sFilename) 
{
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal(CString(sDrive) + CString(sDir));
	int nLen = rVal.GetLength();

	if (rVal.GetAt(nLen-1) != _T('\\'))
		rVal += _T("\\");

	return rVal;
}  

CString CXCommon::GetName(TCHAR *sFilename) 
{
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal;
	rVal.Format(_T("%s%s"), sFname, sExt);

	return rVal;
}