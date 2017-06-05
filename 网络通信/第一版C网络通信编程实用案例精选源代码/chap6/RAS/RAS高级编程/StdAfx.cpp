///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////

// stdafx.cpp : source file that includes just the standard includes
//	Afd.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

char gchMain[] = "Main";
char gchRedialWhenHangUp[] = "RedialWhenHangUp";
char gchLogFile[] = "LogFile";
char gchPingTime[] = "PingTime";
char gchMinSpeed[] = "MinSpeed";
char gchTermKB[] = "TermKB";
char gchTermTime[] = "TermTime";
char gchMaxWaitTime[] = "MaxWaitTime";
char gchPulseDialing[] = "PulseDialing";
char gchEnableTerminateWhenExit[] = "EnableTerminateWhenExit";
char gchEnableLogFile[] = "EnableLogFile";
char gchEnablePing[] = "EnablePing";
char gchEnableMinSpeed[] = "EnableMinSpeed";
char gchEnableTerminateLowSpeed[] = "EnableTerminateLowSpeed";
char gchEnableTimeout[] = "EnableTimeout";
char gchInterfaceStartMinimized[] = "InterfaceStartMinimized";
char gchInterfaceMinimizeOnConnect[] = "InterfaceMinimizeOnConnect";
char gchHidePassword[] = "HidePassword";

CString GetRegKey(HKEY hOpen, CString key1,CString key2)
{
	HKEY hkey;
	LONG l = RegOpenKeyEx(hOpen, key1, 0, KEY_READ, &hkey);
	if (l != ERROR_SUCCESS) return "";
	CString sss;
	ULONG LS = _MAX_PATH;
	DWORD lpType = REG_EXPAND_SZ;

	l = RegQueryValueEx(hkey, key2, NULL,
	& lpType, (unsigned char *) sss.GetBuffer(LS), & LS);
	sss.ReleaseBuffer();
	RegCloseKey(hkey);
	if (l != ERROR_SUCCESS) return "";
	return sss;
}

CString L2S(LONG l)
{
	CString sss;
	sprintf(sss.GetBuffer(512), "%ld", l);
	sss.ReleaseBuffer();
	return sss;
}

CString D2S_2(double d)
{
	CString sss;
	sprintf(sss.GetBuffer(512), "%.2f", d);
	sss.ReleaseBuffer();
	return sss;
}

void PrintError()
{
	DWORD dw = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
	FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
	dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	(LPTSTR) &lpMsgBuf,	0, NULL);// Process any inserts in lpMsgBuf.
	// Display the string.
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error",
	MB_OK | MB_ICONINFORMATION);
	// Free the buffer.
	LocalFree(lpMsgBuf);
}


