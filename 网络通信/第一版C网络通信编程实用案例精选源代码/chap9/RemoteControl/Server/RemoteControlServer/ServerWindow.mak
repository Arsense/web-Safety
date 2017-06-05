# Microsoft Developer Studio Generated NMAKE File, Based on ServerWindow.dsp
!IF "$(CFG)" == ""
CFG=ServerWindow - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ServerWindow - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ServerWindow - Win32 Release" && "$(CFG)" !=\
 "ServerWindow - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServerWindow.mak" CFG="ServerWindow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerWindow - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ServerWindow - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ServerWindow - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ServerWindow.exe"

!ELSE 

ALL : "$(OUTDIR)\ServerWindow.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Command.obj"
	-@erase "$(INTDIR)\MainWnd.obj"
	-@erase "$(INTDIR)\Script1.res"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\ServerWindow.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ServerWindow.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\ServerWindow.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Script1.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ServerWindow.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=c:\devstudio\cppprojects\zip\release\zip.lib ws2_32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ServerWindow.pdb"\
 /machine:I386 /out:"$(OUTDIR)\ServerWindow.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Command.obj" \
	"$(INTDIR)\MainWnd.obj" \
	"$(INTDIR)\Script1.res" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\ServerWindow.obj"

"$(OUTDIR)\ServerWindow.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ServerWindow - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ServerWindow.exe"

!ELSE 

ALL : "$(OUTDIR)\ServerWindow.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Command.obj"
	-@erase "$(INTDIR)\MainWnd.obj"
	-@erase "$(INTDIR)\Script1.res"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\ServerWindow.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ServerWindow.exe"
	-@erase "$(OUTDIR)\ServerWindow.ilk"
	-@erase "$(OUTDIR)\ServerWindow.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\ServerWindow.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Script1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ServerWindow.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=c:\devstudio\cppprojects\zip\debug\zipdbg.lib ws2_32.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ServerWindow.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\ServerWindow.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Command.obj" \
	"$(INTDIR)\MainWnd.obj" \
	"$(INTDIR)\Script1.res" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\ServerWindow.obj"

"$(OUTDIR)\ServerWindow.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "ServerWindow - Win32 Release" || "$(CFG)" ==\
 "ServerWindow - Win32 Debug"
SOURCE=.\Command.c

!IF  "$(CFG)" == "ServerWindow - Win32 Release"

DEP_CPP_COMMA=\
	".\Command.h"\
	".\Server.h"\
	

"$(INTDIR)\Command.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ServerWindow - Win32 Debug"

DEP_CPP_COMMA=\
	".\Command.h"\
	".\Server.h"\
	

"$(INTDIR)\Command.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainWnd.cpp

!IF  "$(CFG)" == "ServerWindow - Win32 Release"

DEP_CPP_MAINW=\
	"..\..\..\program files\devstudio\vc\include\windowsx.h"\
	".\MainWnd.h"\
	".\Server.h"\
	".\ServerWindow.h"\
	

"$(INTDIR)\MainWnd.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ServerWindow - Win32 Debug"

DEP_CPP_MAINW=\
	"..\..\..\program files\devstudio\vc\include\windowsx.h"\
	".\MainWnd.h"\
	".\Server.h"\
	".\ServerWindow.h"\
	

"$(INTDIR)\MainWnd.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"


!ENDIF 

SOURCE=.\Script1.rc
DEP_RSC_SCRIP=\
	".\yinyang.ico"\
	

"$(INTDIR)\Script1.res" : $(SOURCE) $(DEP_RSC_SCRIP) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Server.c

!IF  "$(CFG)" == "ServerWindow - Win32 Release"

DEP_CPP_SERVE=\
	".\Command.h"\
	".\Server.h"\
	".\zconf.h"\
	".\zlib.h"\
	

"$(INTDIR)\Server.obj" : $(SOURCE) $(DEP_CPP_SERVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ServerWindow - Win32 Debug"

DEP_CPP_SERVE=\
	".\Command.h"\
	".\Server.h"\
	".\zconf.h"\
	".\zlib.h"\
	

"$(INTDIR)\Server.obj" : $(SOURCE) $(DEP_CPP_SERVE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ServerWindow.cpp

!IF  "$(CFG)" == "ServerWindow - Win32 Release"

DEP_CPP_SERVER=\
	"..\..\..\program files\devstudio\vc\include\windowsx.h"\
	".\MainWnd.h"\
	".\ServerWindow.h"\
	

"$(INTDIR)\ServerWindow.obj" : $(SOURCE) $(DEP_CPP_SERVER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ServerWindow - Win32 Debug"

DEP_CPP_SERVER=\
	"..\..\..\program files\devstudio\vc\include\windowsx.h"\
	".\MainWnd.h"\
	".\ServerWindow.h"\
	

"$(INTDIR)\ServerWindow.obj" : $(SOURCE) $(DEP_CPP_SERVER) "$(INTDIR)"


!ENDIF 


!ENDIF 

