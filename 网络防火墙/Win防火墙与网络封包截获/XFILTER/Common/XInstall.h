//=============================================================================================
/*
	XInstall.h
	Install XFILTER Winscok 2 base service provider hook

	Project	: XFILTER 1.0 Personal Firewall
	Author	: Tony Zhu
	Create Date	: 2001/08/28
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/
//=============================================================================================
#ifndef XINSTALL_H
#define XINSTALL_H

class CXInstall
{
private:
	int EnumHookKey(BOOL IsRemove = FALSE);
	int SaveHookKey(HKEY hkey, LPCTSTR sSubKey, BOOL IsRemove = FALSE);

public:
	BOOL	IsWinsock2();
	BOOL	IsInstalled(TCHAR *sPathName = NULL);
	int		InstallProvider(TCHAR *sPathName);
	int		RemoveProvider();

public:
	BOOL ReadReg(
		TCHAR	*sKey,
		BYTE	*pBuffer,
		DWORD	dwBufSize, 
		HKEY	hkey	= HKEY_LOCAL_MACHINE, 
		TCHAR	*sSubKey = REG_INSTALL_KEY,
		DWORD	ulType	= REG_BINARY
		);

	BOOL SaveReg(
		TCHAR	*sKey, 
		BYTE	*pBuffer, 
		DWORD	dwBufSize, 
		HKEY	hkey	= HKEY_LOCAL_MACHINE, 
		TCHAR	*sSubKey = REG_INSTALL_KEY,	
		DWORD	ulType	= REG_BINARY
		);

	BOOL DeleteReg(
		HKEY	hkey	= HKEY_LOCAL_MACHINE,
		TCHAR	*sSubKey = REG_INSTALL_KEY, 
		TCHAR	*sItem	= NULL
		);

public:
	TCHAR m_sPathName[MAX_PATH];
};

#endif