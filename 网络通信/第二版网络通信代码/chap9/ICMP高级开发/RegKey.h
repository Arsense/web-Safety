// RegKey.h : header file
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
//
// RegKey window

class RegKey : public CWnd
{
// Construction
public:
	RegKey();

	BOOL GetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, LPINT RegValue, LPDWORD RegType);
	BOOL GetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, LPDWORD RegValue, LPDWORD RegType);
	BOOL GetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, CString *RegValue, LPDWORD RegType);

	BOOL SetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, int RegValue, DWORD RegType);
	BOOL SetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, DWORD RegValue, DWORD RegType);
	BOOL SetRegistryValue(HKEY RootKey, CString RegKey, CString RegEntry, CString RegValue, DWORD RegType);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RegKey)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~RegKey();

	// Generated message map functions
protected:
	//{{AFX_MSG(RegKey)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
