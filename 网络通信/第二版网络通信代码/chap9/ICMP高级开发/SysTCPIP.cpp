// SysTCPIP.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////////////
//																			//
//			1998. EarthWalk Designs Software, by Jay Wheeler.				//
//			All inquiries and/or comments to Jay@EarthWalkDesigns.com		//
//			Latest version can be downloaded from:							//
//																			//
//				http://www.earthwalkdesigns.com								//
//																			//
//////////////////////////////////////////////////////////////////////////////
//																			//
//			Designed using Microsoft VisualC++ 4.2 and MFC					//
//			Tested on WindowsNT 4.0 Workstation and Server					//
//																			//
//////////////////////////////////////////////////////////////////////////////
//																			//
//			This software is released into the public domain as is and		//
//			without warranty.												//
//																			//
//			Use it in good health, and please let me know if it was useful. //
//			If you make improvements or fixed problems, please drop me an	//
//			e-mail describing your changes.  I will try to incorporate		//
//			them into this package so that others may benefit from your		//
//			improvements.													//
//																			//
//			Enjoy!															//
//																			// 
//////////////////////////////////////////////////////////////////////////////
//																			//
//			SysTCPIP Class - Derived from CWnd, base class					//
//																			//
//		Provides the following class methods for WindowsNT 4.0 systems		//
//		utilizing the WindowsNT Winsock 2 interface to the Internet.		//
//																			//
//		GetNSName - 	    Fetch the current name server name from			//
//						    system TCPIP parameters in REGISTRY				//
//																			//
//		GetLocalHostName - 	Fetch the Local Host Name from					//
//							system TCPIP parameters in REGISTRY				//
//																			//
//		GetDomainName -	 	Fetch the system Domain Name from				//
//							system TCPIP parameters in REGISTRY				//
//																			//
//		WinsockVersion -	If called with no parameters, returns			//
//							TRUE if a Winsock is present, else FALSE		//
//																			//
//		WinsockVersion -	If called with a pointer to a WSADATA structure,//
//							Returns TRUE if a Winsock is present, else FALSE//
//							If a winsock is present, fills in the WSADATA	//
//							structure pointed to.							//
//																			//
//////////////////////////////////////////////////////////////////////////////
//																			//
//		Uses MFC class RegKey to access the WindowsNT registry.				//
//																			//
//		This class is also available from EarthWalk Designs Software		//
//		at http://www.earthwalkdesigns.com									//
//																			//
//////////////////////////////////////////////////////////////////////////////
//																			//
//		Useage:																//
//			To use this class, copy the files SysTCPIP.cpp, SysTCPIP.h,		//
//			RegKey.cpp and RegKey.h to the same directory as your project.	//
//			Include both SysTCPIP.cpp and RegKey.cpp in your project.		//
//																			//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysTCPIP.h"
#include "RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SysTCPIP

SysTCPIP::SysTCPIP()
{
}

SysTCPIP::~SysTCPIP()
{
}


BEGIN_MESSAGE_MAP(SysTCPIP, CWnd)
	//{{AFX_MSG_MAP(SysTCPIP)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SysTCPIP message handlers

//////////////////////////////////////////////////////////
//														//
//		Fetch the current name server name from			//
//			system TCPIP parameters in REGISTRY			//
//														//
//////////////////////////////////////////////////////////

//获得命名服务器
BOOL SysTCPIP::GetNSName(CString * NSName)
{
	RegKey			hKey;
	DWORD			RegType = NULL;

	if (!hKey.GetRegistryValue(HKEY_LOCAL_MACHINE,
							   "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
							   "NameServer",
							   NSName,
							   &RegType))
	{
			return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////
//														//
//			Fetch the Local Host Name from				//
//			system TCPIP parameters in REGISTRY			//
//														//
//////////////////////////////////////////////////////////

//获得计算机名
BOOL SysTCPIP::GetLocalHostName (CString * HostName)
{
	RegKey			hKey;
	DWORD			RegType = NULL;

	if (!hKey.GetRegistryValue(HKEY_LOCAL_MACHINE,
							   "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
							   "HostName",
							   HostName,
							   &RegType))
	{
			return FALSE;
	}
	return TRUE;

}

//////////////////////////////////////////////////////////
//														//
//			Fetch the system Domain Name from			//
//			system TCPIP parameters in REGISTRY			//
//														//
//////////////////////////////////////////////////////////

//获得域名
BOOL SysTCPIP::GetDomainName (CString * DomainName)
{
	RegKey			hKey;
	DWORD			RegType = NULL;

	if (!hKey.GetRegistryValue(HKEY_LOCAL_MACHINE,
							   "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
							   "Domain",
							   DomainName,
							   &RegType))
	{
			return FALSE;
	}
	return TRUE;

}

//////////////////////////////////////////////////////////////
//															//
//		Returns TRUE if a Winsock is present, else FALSE	//
//															//
//////////////////////////////////////////////////////////////
//获得Winsock版本
BOOL SysTCPIP::WinsockVersion()
{

	WSADATA wsaData;

	return WinsockVersion(&wsaData);
}

//////////////////////////////////////////////////////////////
//															//
//		Returns TRUE if a Winsock is present, else FALSE	//
//		If a winsock is present, fills in the WSADATA		//
//		structure pointed to.							    //
//															//
//////////////////////////////////////////////////////////////
BOOL SysTCPIP::WinsockVersion(WSADATA * lpwsaData)
{

	WORD wVersionRequested;

	wVersionRequested = MAKEWORD(1,1);
	if (WSAStartup(wVersionRequested, lpwsaData))
	{
		return FALSE;
	}
	return TRUE;
}
