# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=EWDPing - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to EWDPing - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "EWDPing - Win32 Release" && "$(CFG)" !=\
 "EWDPing - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "EWDPing.mak" CFG="EWDPing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EWDPing - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EWDPing - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "EWDPing - Win32 Release"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "EWDPing - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\EWDPing.exe"

CLEAN : 
	-@erase "$(INTDIR)\EWDPing.obj"
	-@erase "$(INTDIR)\EWDPing.pch"
	-@erase "$(INTDIR)\EWDPing.res"
	-@erase "$(INTDIR)\EWDPingDlg.obj"
	-@erase "$(INTDIR)\Icmp.obj"
	-@erase "$(INTDIR)\PingOptionsDlg.obj"
	-@erase "$(INTDIR)\RegKey.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysTCPIP.obj"
	-@erase "$(OUTDIR)\EWDPing.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/EWDPing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/EWDPing.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/EWDPing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/EWDPing.pdb" /machine:I386 /out:"$(OUTDIR)/EWDPing.exe" 
LINK32_OBJS= \
	"$(INTDIR)\EWDPing.obj" \
	"$(INTDIR)\EWDPing.res" \
	"$(INTDIR)\EWDPingDlg.obj" \
	"$(INTDIR)\Icmp.obj" \
	"$(INTDIR)\PingOptionsDlg.obj" \
	"$(INTDIR)\RegKey.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysTCPIP.obj"

"$(OUTDIR)\EWDPing.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "EWDPing - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\EWDPing.exe"

CLEAN : 
	-@erase "$(INTDIR)\EWDPing.obj"
	-@erase "$(INTDIR)\EWDPing.pch"
	-@erase "$(INTDIR)\EWDPing.res"
	-@erase "$(INTDIR)\EWDPingDlg.obj"
	-@erase "$(INTDIR)\Icmp.obj"
	-@erase "$(INTDIR)\PingOptionsDlg.obj"
	-@erase "$(INTDIR)\RegKey.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysTCPIP.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\EWDPing.exe"
	-@erase "$(OUTDIR)\EWDPing.ilk"
	-@erase "$(OUTDIR)\EWDPing.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/EWDPing.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/EWDPing.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/EWDPing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/EWDPing.pdb" /debug /machine:I386 /out:"$(OUTDIR)/EWDPing.exe" 
LINK32_OBJS= \
	"$(INTDIR)\EWDPing.obj" \
	"$(INTDIR)\EWDPing.res" \
	"$(INTDIR)\EWDPingDlg.obj" \
	"$(INTDIR)\Icmp.obj" \
	"$(INTDIR)\PingOptionsDlg.obj" \
	"$(INTDIR)\RegKey.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysTCPIP.obj"

"$(OUTDIR)\EWDPing.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "EWDPing - Win32 Release"
# Name "EWDPing - Win32 Debug"

!IF  "$(CFG)" == "EWDPing - Win32 Release"

!ELSEIF  "$(CFG)" == "EWDPing - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "EWDPing - Win32 Release"

!ELSEIF  "$(CFG)" == "EWDPing - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EWDPing.cpp
DEP_CPP_EWDPI=\
	".\EWDPing.h"\
	".\EWDPingDlg.h"\
	".\Icmp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EWDPing.obj" : $(SOURCE) $(DEP_CPP_EWDPI) "$(INTDIR)"\
 "$(INTDIR)\EWDPing.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EWDPingDlg.cpp
DEP_CPP_EWDPIN=\
	".\EWDPing.h"\
	".\EWDPingDlg.h"\
	".\Icmp.h"\
	".\PingOptionsDlg.h"\
	".\RegKey.h"\
	".\StdAfx.h"\
	".\SysTCPIP.h"\
	

"$(INTDIR)\EWDPingDlg.obj" : $(SOURCE) $(DEP_CPP_EWDPIN) "$(INTDIR)"\
 "$(INTDIR)\EWDPing.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "EWDPing - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/EWDPing.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EWDPing.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "EWDPing - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/EWDPing.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EWDPing.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EWDPing.rc
DEP_RSC_EWDPING=\
	".\res\EWDPing.ico"\
	".\res\EWDPing.rc2"\
	".\res\ico00003.ico"\
	".\res\icon1.ico"\
	".\res\Inet1.ICO"\
	".\res\Inet2.ico"\
	".\res\inet3.ico"\
	".\res\inet4.ico"\
	

"$(INTDIR)\EWDPing.res" : $(SOURCE) $(DEP_RSC_EWDPING) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Icmp.cpp
DEP_CPP_ICMP_=\
	".\Icmp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Icmp.obj" : $(SOURCE) $(DEP_CPP_ICMP_) "$(INTDIR)"\
 "$(INTDIR)\EWDPing.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PingOptionsDlg.cpp
DEP_CPP_PINGO=\
	".\EWDPing.h"\
	".\PingOptionsDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PingOptionsDlg.obj" : $(SOURCE) $(DEP_CPP_PINGO) "$(INTDIR)"\
 "$(INTDIR)\EWDPing.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RegKey.cpp
DEP_CPP_REGKE=\
	".\RegKey.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RegKey.obj" : $(SOURCE) $(DEP_CPP_REGKE) "$(INTDIR)"\
 "$(INTDIR)\EWDPing.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysTCPIP.cpp
DEP_CPP_SYSTC=\
	".\RegKey.h"\
	".\StdAfx.h"\
	".\SysTCPIP.h"\
	

"$(INTDIR)\SysTCPIP.obj" : $(SOURCE) $(DEP_CPP_SYSTC) "$(INTDIR)"\
 "$(INTDIR)\EWDPing.pch"


# End Source File
# End Target
# End Project
################################################################################
