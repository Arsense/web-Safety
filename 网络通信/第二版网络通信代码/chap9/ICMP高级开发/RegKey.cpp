// RegKey.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			1998. EarthWalk Designs Software, by Jay Wheeler.					//
//			All inquiries and/or comments to Jay@EarthWalkDesigns.com			//
//			Latest version can be downloaded from:								//
//																				//
//				http://www.earthwalkdesigns.com									//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Designed using Microsoft VisualC++ 4.2 and MFC						//
//			Tested on WindowsNT 4.0 Workstation and Server						//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			This software is released into the public domain as is and			//
//			without warranty.													//
//																				//
//			Use it in good health, and please let me know if it was useful.		//
//			If you make improvements or fixed problems, please drop me an		//
//			e-mail describing your changes.  I will try to incorporate			//
//			them into this package so that others may benefit from your			//
//			improvements.														//
//																				//
//			Enjoy!																//
//																				// 
//////////////////////////////////////////////////////////////////////////////////
//																				//
//			RegKey Class - Derived from CWnd, base class						//
//																				//
//		Provides the following class methods for WindowsNT 4.0 systems			//
//		utilizing the WindowsNT Winsock 2 interface to the Internet.			//
//																				//
//		GetRegistryValue														//
//			If passed an integer value, Get the specified integer value.		//
//			if passed a DWORD value, get the specified DWORD value.				//
//			if passed a xxx_SZ value, get the specified string value.			//
//																				//
//			Get the specified value from the system registry.  Specify			//
//			the Registry table, key and entry for the value.					//
//			A value of TRUE is returned if the value is found, else FALSE.		//
//																				//
//		SetRegistryValue														//
//			if passed an integer value, set the specified DWORD_SZ				//
//			if passed a DWORD value, set the specified DWORD_SZ					//
//			if passed a CString value, set the specified xxx_SZ value			//
//																				//
//			Set the specified value in the system registry.  If the entry		//
//			does not exist, it is created.  Specify	the Registry table, key		//
//			and entry for the value.  A value of TRUE is returned if the		//
//			entry is created, else FALSE.										//
//																				//
//////////////////////////////////////////////////////////////////////////////////
//																				//
//		Useage:																	//
//			To use this class, copy the files RegKey.cpp and RegKey.h to		//
//			the same directory as your project.	Include RegKey.cpp in your		//
//			project.															//
//																				//
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegKey

RegKey::RegKey()
{
}

RegKey::~RegKey()
{
}


BEGIN_MESSAGE_MAP(RegKey, CWnd)
	//{{AFX_MSG_MAP(RegKey)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// RegKey message handlers

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Get the specified value from the system registry.  Specify			//
//			the Registry table, key and entry for the value.					//
//			A value of TRUE is returned if the value is found, else FALSE.		//
//																				//
//////////////////////////////////////////////////////////////////////////////////
BOOL RegKey::GetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, LPINT RegValue, LPDWORD RegType)
{
	HKEY hKey;
	
	if(RegOpenKeyEx(RootKey,
				    (LPCSTR)RegKey,
					0,
					KEY_ALL_ACCESS,
					&hKey) != ERROR_SUCCESS)
		return FALSE;

	unsigned long	BufferSize=80L;
	char			ValueBuffer[80];
	CString			regname=RegEntry;
	DWORD			dwType = NULL;
	
	if(RegQueryValueEx(hKey,
					   regname,
					   NULL,
					   (LPDWORD)dwType,
					   (LPBYTE)ValueBuffer,
					   &BufferSize) == ERROR_SUCCESS)
	{
		DWORD		FetchVal;

		FetchVal = *((long *)ValueBuffer);
		*RegValue = (int)FetchVal;
		*RegType = dwType;
		RegCloseKey (hKey);

		return TRUE;
	}

	RegCloseKey (hKey);
	
	return FALSE;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Set the specified value in the system registry.  If the entry		//
//			does not exist, it is created.  Specify	the Registry table, key		//
//			and entry for the value.  A value of TRUE is returned if the		//
//			entry is created, else FALSE.										//
//																				//
//////////////////////////////////////////////////////////////////////////////////
BOOL RegKey::SetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, int RegValue, DWORD RegType)
{
	HKEY			hKey;
	DWORD			result;
	
	if(RegCreateKeyEx (RootKey,
					   (LPCSTR)RegKey,
					   0,
					   NULL,
					   REG_OPTION_NON_VOLATILE,
					   KEY_ALL_ACCESS,
					   NULL,
					   &hKey,
					   &result) != ERROR_SUCCESS)
			return FALSE;

	DWORD		regval = (long)RegValue;
	CString		regname = RegEntry;

	if (RegSetValueEx (hKey,
					   RegEntry,
					   0,
					   RegType,
					   (unsigned char *)&regval,
					   sizeof(DWORD)) != ERROR_SUCCESS)
	{
		RegCloseKey (hKey);
		return FALSE;
	}

	RegCloseKey (hKey);

	return TRUE;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Get the specified value from the system registry.  Specify			//
//			the Registry table, key and entry for the value.					//
//			A value of TRUE is returned if the value is found, else FALSE.		//
//																				//
//////////////////////////////////////////////////////////////////////////////////
BOOL RegKey::GetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, LPDWORD RegValue, LPDWORD RegType)
{
	HKEY hKey;
	
	if(RegOpenKeyEx(RootKey,
				    (LPCSTR)RegKey,
					0,
					KEY_ALL_ACCESS,
					&hKey) != ERROR_SUCCESS)
		return FALSE;

	unsigned long	BufferSize=80L;
	char			ValueBuffer[80];
	CString			regname=RegEntry;
	DWORD			dwType = NULL;
	
	if(RegQueryValueEx(hKey,
					   regname,
					   NULL,
					   (LPDWORD)dwType,
					   (LPBYTE)ValueBuffer,
					   &BufferSize) == ERROR_SUCCESS)
	{
		*RegValue = *((DWORD *)ValueBuffer);
		*RegType = dwType;
		RegCloseKey (hKey);

		return TRUE;
	}

	RegCloseKey (hKey);
	
	return FALSE;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Set the specified value in the system registry.  If the entry		//
//			does not exist, it is created.  Specify	the Registry table, key		//
//			and entry for the value.  A value of TRUE is returned if the		//
//			entry is created, else FALSE.										//
//																				//
//////////////////////////////////////////////////////////////////////////////////
BOOL RegKey::SetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, DWORD RegValue, DWORD RegType)
{
	HKEY			hKey;
	DWORD			result;
	
	if(RegCreateKeyEx (RootKey,
					   (LPCSTR)RegKey,
					   0,
					   NULL,
					   REG_OPTION_NON_VOLATILE,
					   KEY_ALL_ACCESS,
					   NULL,
					   &hKey,
					   &result) != ERROR_SUCCESS)
			return FALSE;

	CString		regname = RegEntry;

	if (RegSetValueEx (hKey,
					   RegEntry,
					   0,
					   RegType,
					   (unsigned char *)&RegValue,
					   sizeof(DWORD)) != ERROR_SUCCESS)
	{
		RegCloseKey (hKey);
		return FALSE;
	}

	RegCloseKey (hKey);

	return TRUE;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Get the specified string value from the system registry.  Specify	//
//			the Registry table, key and entry for the string.					//
//			A value of TRUE is returned if the string is found, else FALSE.		//
//																				//
//////////////////////////////////////////////////////////////////////////////////
BOOL RegKey::GetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, CString *RegValue, LPDWORD RegType)
{
	HKEY				hKey;
	
	if (RegOpenKeyEx (RootKey,
					  (LPCSTR)RegKey,
					  0,
					  KEY_ALL_ACCESS,
					  &hKey) != ERROR_SUCCESS)
		return FALSE;

	unsigned long		BufferSize = 80L;
	char				ValueBuffer[80];
	CString				regname = RegEntry;
	DWORD				dwType = NULL;

	if (RegQueryValueEx (hKey,
						 regname,
						 NULL,
						 (LPDWORD)dwType,
						 (LPBYTE)ValueBuffer,
						 &BufferSize) == ERROR_SUCCESS)
	{
		*RegValue = ValueBuffer;
		*RegType = dwType;

		RegCloseKey (hKey);
		
		return TRUE;
	}

	RegCloseKey (hKey);
	return FALSE;

}

//////////////////////////////////////////////////////////////////////////////////
//																				//
//			Set the specified string value in the system registry.  If the		//
//			entry does not exist, it is created.  Specify the Registry table,	//
//			key and entry for the value.  A value of TRUE is returned if the	//
//			entry is created, else FALSE.										//
//																				//
//////////////////////////////////////////////////////////////////////////////////
BOOL RegKey::SetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, CString RegValue, DWORD RegType)
{
	HKEY				hKey;
	DWORD				result;
	
	if (RegCreateKeyEx (RootKey,
					    (LPCSTR)RegKey,
					    0,
					    NULL,
					    REG_OPTION_NON_VOLATILE,
					    KEY_ALL_ACCESS,
					    NULL,
					    &hKey,
					    &result) != ERROR_SUCCESS)
			return FALSE;

	char				ValueBuffer[80];
	
	strcpy (ValueBuffer, (LPCSTR)RegValue);

	CString				regname = RegEntry;

	if (RegSetValueEx (hKey,
				       regname,
					   0,
					   RegType,
					   (unsigned char*)ValueBuffer,
					   strlen(ValueBuffer) + 1) != ERROR_SUCCESS)
	{
		RegCloseKey (hKey);
		return FALSE;
	}

	RegCloseKey (hKey);

	return TRUE;

}
