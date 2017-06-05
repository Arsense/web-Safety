////////////////////////////////////////////////////////////////
// 1998 Microsoft Systems Journal
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef __MODULEVER_H
#define __MODULEVER_H

// tell linker to link with version.lib for VerQueryValue, etc.
#pragma comment(linker, "/defaultlib:version.lib")

#ifndef DLLVERSIONINFO
// following is from shlwapi.h, in November 1997 release of the Windows SDK

typedef struct _DllVersionInfo
{
    DWORD cbSize;
    DWORD dwMajorVersion;                   // Major version
    DWORD dwMinorVersion;                   // Minor version
    DWORD dwBuildNumber;                    // Build number
    DWORD dwPlatformID;                     // DLLVER_PLATFORM_*
} DLLVERSIONINFO;

// Platform IDs for DLLVERSIONINFO
#define DLLVER_PLATFORM_WINDOWS      0x00000001      // Windows 95
#define DLLVER_PLATFORM_NT           0x00000002      // Windows NT

#endif // DLLVERSIONINFO

//////////////////
// CModuleVersion version info about a module.
// To use:
//
// CModuleVersion ver
// if (ver.GetFileVersionInfo("_T("mymodule))) {
//    // info is in ver, you can call GetValue to get variable info like
//    CString s = ver.GetValue(_T("CompanyName"));
// }
//
// You can also call the static fn DllGetVersion to get DLLVERSIONINFO.
//
class CModuleVersion : public VS_FIXEDFILEINFO {
protected:
   BYTE* m_pVersionInfo;   // all version info

   struct TRANSLATION {
      WORD langID;         // language ID
      WORD charset;        // character set (code page)
   } m_translation;

public:
   CModuleVersion();
   virtual ~CModuleVersion();

   BOOL     GetFileVersionInfo(LPCTSTR modulename);
   BOOL     GetDLLVersionInfo(LPTSTR filename);
   CString  GetValue(LPCTSTR lpKeyName);
   static BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);
};

#endif
