# Microsoft Visual C++ generated build script - Do not modify

PROJ = WSTERM
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG
R_RCDEFINES = /dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\CD\CHAPTE~4\WSTERM\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = /YcSTDAFX.H
CUSEPCHFLAG = 
CPPUSEPCHFLAG = /YuSTDAFX.H
FIRSTC =             
FIRSTCPP = STDAFX.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /G2 /W3 /Zi /AL /Od /D "_DEBUG" /FR /GA /Fd"WSTERM.PDB"
CFLAGS_R_WEXE = /nologo /Gs /G2 /W3 /AL /O1 /D "NDEBUG" /FR /GA
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = lafxcwd oldnames libw llibcew winsock commdlg.lib shell.lib 
LIBS_R_WEXE = lafxcw oldnames libw llibcew winsock commdlg.lib shell.lib 
RCFLAGS = /nologo /z
RESFLAGS = /nologo /t
RUNFLAGS = 
DEFFILE = WSTERM.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = STDAFX.SBR \
		WSTERM.SBR \
		MAINFRM.SBR \
		DOC.SBR \
		VIEW.SBR \
		CONNECTD.SBR \
		TERMSOCK.SBR


WSTERM_RCDEP = c:\msvc\include\winsock.h \
	c:\cd\chapte~4\wsterm\res\wsterm.ico \
	c:\cd\chapte~4\wsterm\res\wsterm.rc2


STDAFX_DEP = c:\cd\chapte~4\wsterm\stdafx.h


WSTERM_DEP = c:\cd\chapte~4\wsterm\stdafx.h \
	c:\cd\chapte~4\wsterm\wsterm.h \
	c:\cd\chapte~4\wsterm\mainfrm.h \
	c:\cd\chapte~4\wsterm\doc.h \
	c:\cd\chapte~4\wsterm\termsock.h \
	c:\cd\chapte~4\wsterm\view.h


MAINFRM_DEP = c:\cd\chapte~4\wsterm\stdafx.h \
	c:\cd\chapte~4\wsterm\wsterm.h \
	c:\cd\chapte~4\wsterm\mainfrm.h


DOC_DEP = c:\cd\chapte~4\wsterm\stdafx.h \
	c:\cd\chapte~4\wsterm\wsterm.h \
	c:\cd\chapte~4\wsterm\doc.h


VIEW_DEP = c:\cd\chapte~4\wsterm\stdafx.h \
	c:\cd\chapte~4\wsterm\wsterm.h \
	c:\cd\chapte~4\wsterm\doc.h \
	c:\cd\chapte~4\wsterm\termsock.h \
	c:\cd\chapte~4\wsterm\view.h \
	c:\cd\chapte~4\wsterm\connectd.h


CONNECTD_DEP = c:\cd\chapte~4\wsterm\stdafx.h \
	c:\cd\chapte~4\wsterm\wsterm.h \
	c:\cd\chapte~4\wsterm\connectd.h


TERMSOCK_DEP = c:\cd\chapte~4\wsterm\stdafx.h \
	c:\cd\chapte~4\wsterm\wsterm.h \
	c:\cd\chapte~4\wsterm\doc.h \
	c:\cd\chapte~4\wsterm\termsock.h \
	c:\cd\chapte~4\wsterm\view.h


all:	$(PROJ).EXE $(PROJ).BSC

WSTERM.RES:	WSTERM.RC $(WSTERM_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r WSTERM.RC

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

WSTERM.OBJ:	WSTERM.CPP $(WSTERM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WSTERM.CPP

MAINFRM.OBJ:	MAINFRM.CPP $(MAINFRM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAINFRM.CPP

DOC.OBJ:	DOC.CPP $(DOC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DOC.CPP

VIEW.OBJ:	VIEW.CPP $(VIEW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEW.CPP

CONNECTD.OBJ:	CONNECTD.CPP $(CONNECTD_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CONNECTD.CPP

TERMSOCK.OBJ:	TERMSOCK.CPP $(TERMSOCK_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TERMSOCK.CPP


$(PROJ).EXE::	WSTERM.RES

$(PROJ).EXE::	STDAFX.OBJ WSTERM.OBJ MAINFRM.OBJ DOC.OBJ VIEW.OBJ CONNECTD.OBJ TERMSOCK.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ +
WSTERM.OBJ +
MAINFRM.OBJ +
DOC.OBJ +
VIEW.OBJ +
CONNECTD.OBJ +
TERMSOCK.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
c:\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) WSTERM.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	WSTERM.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) WSTERM.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
