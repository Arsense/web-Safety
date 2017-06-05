# Microsoft Developer Studio Project File - Name="Property" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Property - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Property.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Property.mak" CFG="Property - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Property - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Property - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Property - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x417 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 rasapi32.lib ws2_32.lib /nologo /subsystem:windows /machine:I386 /out:"../release/Xfilter.exe"

!ELSEIF  "$(CFG)" == "Property - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 rasapi32.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../release/Xfilter.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Property - Win32 Release"
# Name "Property - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\Acl.cpp
# End Source File
# Begin Source File

SOURCE=.\AclSet.cpp
# End Source File
# Begin Source File

SOURCE=.\LogQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\NetIPAria.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\SetNet.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemSet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\About.h
# End Source File
# Begin Source File

SOURCE=.\Acl.h
# End Source File
# Begin Source File

SOURCE=.\AclSet.h
# End Source File
# Begin Source File

SOURCE=.\LogQuery.h
# End Source File
# Begin Source File

SOURCE=.\NetIPAria.h
# End Source File
# Begin Source File

SOURCE=.\PacketMonitor.h
# End Source File
# Begin Source File

SOURCE=.\Register.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetNet.h
# End Source File
# Begin Source File

SOURCE=.\SetTime.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemSet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ACLSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Alert.ico
# End Source File
# Begin Source File

SOURCE=.\res\ALERTSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\APPSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\BASESET.ICO
# End Source File
# Begin Source File

SOURCE=..\Resource\box.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMMONSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\CTRPANEL.ICO
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Deny.ico
# End Source File
# Begin Source File

SOURCE=.\res\Deny1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Deny16_15.ico
# End Source File
# Begin Source File

SOURCE=.\res\DenyEx.ico
# End Source File
# Begin Source File

SOURCE=.\res\DenyEx1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Email.ico
# End Source File
# Begin Source File

SOURCE=.\res\IPSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\MEMO.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Message.ico
# End Source File
# Begin Source File

SOURCE=.\res\Monitor.ico
# End Source File
# Begin Source File

SOURCE=.\res\NETHOOD.ICO
# End Source File
# Begin Source File

SOURCE=.\res\NETSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\NULL.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pass.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pass1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pass16_15.ico
# End Source File
# Begin Source File

SOURCE=.\res\PassEx.ico
# End Source File
# Begin Source File

SOURCE=.\res\PassEx1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Property.ico
# End Source File
# Begin Source File

SOURCE=.\Property.rc
# End Source File
# Begin Source File

SOURCE=.\res\Property.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Query.ico
# End Source File
# Begin Source File

SOURCE=.\res\Query1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Query16_15.ico
# End Source File
# Begin Source File

SOURCE=.\res\QueryEx.ico
# End Source File
# Begin Source File

SOURCE=.\res\QueryEx1.ico
# End Source File
# Begin Source File

SOURCE=.\res\QueryResult.ico
# End Source File
# Begin Source File

SOURCE=.\res\QuerySet.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Remove.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SUPERSET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\test.ico
# End Source File
# Begin Source File

SOURCE=.\res\TIMESET.ICO
# End Source File
# Begin Source File

SOURCE=.\res\UserInfo.ico
# End Source File
# Begin Source File

SOURCE=.\res\Xfilter.ico
# End Source File
# Begin Source File

SOURCE=.\Res\xfilter16_4.ico
# End Source File
# Begin Source File

SOURCE=.\Res\xfilter32_1.ico
# End Source File
# Begin Source File

SOURCE=.\res\XfilterStop.ico
# End Source File
# Begin Source File

SOURCE=.\res\XfilterWorking.ico
# End Source File
# End Group
# Begin Group "GUI Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GuiRes.h
# End Source File
# End Group
# Begin Group "HyperLink"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HyperLink\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink\HyperLink.h
# End Source File
# End Group
# Begin Group "Common Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\Debug.h
# End Source File
# Begin Source File

SOURCE=..\Common\TypeStruct.h
# End Source File
# Begin Source File

SOURCE=..\Common\XFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\XFile.h
# End Source File
# Begin Source File

SOURCE=..\Common\XFileRes.h
# End Source File
# Begin Source File

SOURCE=..\Common\XInstall.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\XInstall.h
# End Source File
# Begin Source File

SOURCE=..\Common\XLogFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\XLogFile.h
# End Source File
# End Group
# Begin Group "Main Frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrame\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\MainSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\MainSheet.h
# End Source File
# Begin Source File

SOURCE=.\NetTimeSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\NetTimeSheet.h
# End Source File
# Begin Source File

SOURCE=.\Property.cpp
# End Source File
# Begin Source File

SOURCE=.\Property.h
# End Source File
# End Group
# Begin Group "System Tray"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SystemTray\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemTray\SystemTray.h
# End Source File
# End Group
# Begin Group "Internet Transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Internet\Internet.cpp
# End Source File
# Begin Source File

SOURCE=.\Internet\Internet.h
# End Source File
# End Group
# Begin Group "HtmlHelp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Lib\htmlhelp.h
# End Source File
# Begin Source File

SOURCE=..\Lib\htmlhelp.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Property : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:103
# 	2:21:SplashScreenInsertKey:4.0
# End Section
