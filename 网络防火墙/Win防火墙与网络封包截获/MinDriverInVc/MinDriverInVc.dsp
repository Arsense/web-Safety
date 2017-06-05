# Microsoft Developer Studio Project File - Name="MinDriverInVc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=MinDriverInVc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MinDriverInVc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MinDriverInVc.mak" CFG="MinDriverInVc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MinDriverInVc - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "MinDriverInVc - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "MinDriverInVc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f MinDriverInVc.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "MinDriverInVc.exe"
# PROP BASE Bsc_Name "MinDriverInVc.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "build.bat %DDKROOT% free d: D:\tony\Xfilter_Disk\DdkSource\MinDriverInVc\MinDriver"
# PROP Rebuild_Opt "/a"
# PROP Target_File "MinDriverInVc.exe"
# PROP Bsc_Name "MinDriverInVc.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "MinDriverInVc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f MinDriverInVc.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "MinDriverInVc.exe"
# PROP BASE Bsc_Name "MinDriverInVc.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "build.bat %DDKROOT% checked d: D:\tony\Xfilter_Disk\Disk_No_Explain\MinDriverInVc\Mindriver"
# PROP Rebuild_Opt ""
# PROP Target_File "MinDriver.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "MinDriverInVc - Win32 Release"
# Name "MinDriverInVc - Win32 Debug"

!IF  "$(CFG)" == "MinDriverInVc - Win32 Release"

!ELSEIF  "$(CFG)" == "MinDriverInVc - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MinDriver\MinDriver.c
# End Source File
# Begin Source File

SOURCE=.\MinDriver\MinDriver.h
# End Source File
# End Group
# Begin Group "Make Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MinDriver\MAKEFILE
# End Source File
# Begin Source File

SOURCE=.\MinDriver\Sources
# End Source File
# End Group
# Begin Group "Other Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\MinDriver\MinDriver.reg
# End Source File
# End Group
# Begin Source File

SOURCE=.\MinDriver\Readme.txt
# End Source File
# End Target
# End Project
