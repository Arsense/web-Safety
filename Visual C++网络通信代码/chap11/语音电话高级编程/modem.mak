# Microsoft Developer Studio Generated NMAKE File, Based on modem.dsp
!IF "$(CFG)" == ""
CFG=modem - Win32 Debug
!MESSAGE No configuration specified. Defaulting to modem - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "modem - Win32 Release" && "$(CFG)" != "modem - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "modem.mak" CFG="modem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "modem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "modem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "modem - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\modem.exe"

!ELSE 

ALL : "$(OUTDIR)\modem.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\modem.obj"
	-@erase "$(INTDIR)\modem.pch"
	-@erase "$(INTDIR)\modem.res"
	-@erase "$(INTDIR)\modemDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\wave.obj"
	-@erase "$(OUTDIR)\modem.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\modem.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\modem.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\modem.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\modem.pdb" /machine:I386 /out:"$(OUTDIR)\modem.exe" 
LINK32_OBJS= \
	"$(INTDIR)\modem.obj" \
	"$(INTDIR)\modem.res" \
	"$(INTDIR)\modemDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\wave.obj"

"$(OUTDIR)\modem.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "modem - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\modem.exe"

!ELSE 

ALL : "$(OUTDIR)\modem.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\modem.obj"
	-@erase "$(INTDIR)\modem.pch"
	-@erase "$(INTDIR)\modem.res"
	-@erase "$(INTDIR)\modemDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\wave.obj"
	-@erase "$(OUTDIR)\modem.exe"
	-@erase "$(OUTDIR)\modem.ilk"
	-@erase "$(OUTDIR)\modem.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\modem.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\modem.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\modem.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\modem.pdb" /debug /machine:I386 /out:"$(OUTDIR)\modem.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\modem.obj" \
	"$(INTDIR)\modem.res" \
	"$(INTDIR)\modemDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\wave.obj"

"$(OUTDIR)\modem.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "modem - Win32 Release" || "$(CFG)" == "modem - Win32 Debug"
SOURCE=.\modem.cpp
DEP_CPP_MODEM=\
	".\modem.h"\
	".\modemDlg.h"\
	

"$(INTDIR)\modem.obj" : $(SOURCE) $(DEP_CPP_MODEM) "$(INTDIR)"\
 "$(INTDIR)\modem.pch"


SOURCE=.\modem.rc
DEP_RSC_MODEM_=\
	".\res\modem.ico"\
	".\res\modem.rc2"\
	

"$(INTDIR)\modem.res" : $(SOURCE) $(DEP_RSC_MODEM_) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\modemDlg.cpp
DEP_CPP_MODEMD=\
	".\modem.h"\
	".\modemDlg.h"\
	

"$(INTDIR)\modemDlg.obj" : $(SOURCE) $(DEP_CPP_MODEMD) "$(INTDIR)"\
 "$(INTDIR)\modem.pch"


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "modem - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\modem.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c\
 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\modem.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "modem - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\modem.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\modem.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\wave.cpp

!IF  "$(CFG)" == "modem - Win32 Release"

DEP_CPP_WAVE_=\
	".\wave.h"\
	

"$(INTDIR)\wave.obj" : $(SOURCE) $(DEP_CPP_WAVE_) "$(INTDIR)"\
 "$(INTDIR)\modem.pch"


!ELSEIF  "$(CFG)" == "modem - Win32 Debug"

DEP_CPP_WAVE_=\
	".\StdAfx.h"\
	".\wave.h"\
	

"$(INTDIR)\wave.obj" : $(SOURCE) $(DEP_CPP_WAVE_) "$(INTDIR)"\
 "$(INTDIR)\modem.pch"


!ENDIF 


!ENDIF 

