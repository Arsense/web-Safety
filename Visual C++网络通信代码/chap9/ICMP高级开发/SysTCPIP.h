// SysTCPIP.h : header file
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
//			them into this package so others may benefit from your			//
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
//
//
// SysTCPIP window
//

class SysTCPIP : public CWnd
{
// Construction
public:
	BOOL WinsockVersion(WSADATA * lpwsaData);
	BOOL WinsockVersion();
	BOOL GetDomainName (CString * DomainName);
	BOOL GetLocalHostName (CString * HostName);
	BOOL GetNSName(CString * NSName);

	SysTCPIP();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SysTCPIP)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SysTCPIP();

	// Generated message map functions
protected:
	//{{AFX_MSG(SysTCPIP)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
