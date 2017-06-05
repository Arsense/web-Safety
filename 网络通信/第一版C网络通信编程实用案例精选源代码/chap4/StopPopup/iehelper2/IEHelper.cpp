// IEHelper.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f IEHelperps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "IEHelper.h"

#include "IEHelper_i.c"
#include "IEHlprObj.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_IEHlprObj, CIEHlprObj)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//只允许iexplorer.exe加载这个DLL
       TCHAR Loader[MAX_PATH];
		GetModuleFileName( NULL, Loader, MAX_PATH);
		for ( int i = lstrlen( Loader); i > 0; i--)
			if ( Loader[i] == _T('\\'))
			{
				lstrcpy( Loader, Loader + i + 1);
				break;
			}
		if ( lstrcmpi(_strlwr(Loader), _T("iexplore.exe")) != 0 && lstrcmpi( _strlwr(Loader), _T("regsvr32.exe")) != 0)
			return FALSE;

		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}


