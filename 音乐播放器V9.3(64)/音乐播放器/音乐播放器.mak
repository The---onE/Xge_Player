# Microsoft Developer Studio Generated NMAKE File, Based on ���ֲ�����.dsp
!IF $(CFG)" == "
CFG=���ֲ����� - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ���ֲ����� - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "���ֲ����� - Win32 Release" && "$(CFG)" != "���ֲ����� - Win32 Debug"
!MESSAGE ָ�������� "$(CFG)" ��Ч.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "���ֲ�����.mak" CFG="���ֲ����� - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "���ֲ����� - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "���ֲ����� - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "���ֲ����� - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# ��ʼ�Զ����
OutDir=.\Release
# �����Զ����

ALL : "$(OUTDIR)\���ֲ�����.exe"


CLEAN :
	-@erase "$(INTDIR)\Main.obj"
	-@erase "$(INTDIR)\MainDlg.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\���ֲ�����.pch"
	-@erase "$(OUTDIR)\���ֲ�����.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\���ֲ�����.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\���ֲ�����.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\���ֲ�����.pdb" /machine:I386 /out:"$(OUTDIR)\���ֲ�����.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Main.obj" \
	"$(INTDIR)\MainDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\resource.res" \
	".\SkinH.lib"

"$(OUTDIR)\���ֲ�����.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "���ֲ����� - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# ��ʼ�Զ����
OutDir=.\Debug
# �����Զ����

ALL : "$(OUTDIR)\���ֲ�����.exe" "$(OUTDIR)\���ֲ�����.bsc"


CLEAN :
	-@erase "$(INTDIR)\Main.obj"
	-@erase "$(INTDIR)\Main.sbr"
	-@erase "$(INTDIR)\MainDlg.obj"
	-@erase "$(INTDIR)\MainDlg.sbr"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\���ֲ�����.pch"
	-@erase "$(OUTDIR)\���ֲ�����.bsc"
	-@erase "$(OUTDIR)\���ֲ�����.exe"
	-@erase "$(OUTDIR)\���ֲ�����.ilk"
	-@erase "$(OUTDIR)\���ֲ�����.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\���ֲ�����.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\���ֲ�����.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Main.sbr" \
	"$(INTDIR)\MainDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\���ֲ�����.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\���ֲ�����.pdb" /debug /machine:I386 /out:"$(OUTDIR)\���ֲ�����.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Main.obj" \
	"$(INTDIR)\MainDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\resource.res" \
	".\SkinH.lib"

"$(OUTDIR)\���ֲ�����.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("���ֲ�����.dep")
!INCLUDE "���ֲ�����.dep"
!ELSE 
!MESSAGE Warning: cannot find "���ֲ�����.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "���ֲ����� - Win32 Release" || "$(CFG)" == "���ֲ����� - Win32 Debug"
SOURCE=.\Main.cpp

!IF  "$(CFG)" == "���ֲ����� - Win32 Release"


"$(INTDIR)\Main.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\���ֲ�����.pch"


!ELSEIF  "$(CFG)" == "���ֲ����� - Win32 Debug"


"$(INTDIR)\Main.obj"	"$(INTDIR)\Main.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\���ֲ�����.pch"


!ENDIF 

SOURCE=.\MainDlg.cpp

!IF  "$(CFG)" == "���ֲ����� - Win32 Release"


"$(INTDIR)\MainDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\���ֲ�����.pch"


!ELSEIF  "$(CFG)" == "���ֲ����� - Win32 Debug"


"$(INTDIR)\MainDlg.obj"	"$(INTDIR)\MainDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\���ֲ�����.pch"


!ENDIF 

SOURCE=.\resource.rc

"$(INTDIR)\resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "���ֲ����� - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\���ֲ�����.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\���ֲ�����.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "���ֲ����� - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\���ֲ�����.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\���ֲ�����.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

