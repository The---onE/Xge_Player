# Microsoft Developer Studio Generated NMAKE File, Based on X�����ֲ�����.dsp
!IF $(CFG)" == "
CFG=���ֲ����� - Win32 Release
!MESSAGE No configuration specified. Defaulting to ���ֲ����� - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "���ֲ����� - Win32 Release"
!MESSAGE ָ�������� "$(CFG)" ��Ч.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "X�����ֲ�����.mak" CFG="���ֲ����� - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "���ֲ����� - Win32 Release" (based on "Win32 (x86) Application")
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
OUTDIR=.\Release
INTDIR=.\Release
# ��ʼ�Զ����
OutDir=.\Release
# �����Զ����

ALL : "$(OUTDIR)\X�����ֲ�����.exe"


CLEAN :
	-@erase "$(INTDIR)\Lrc.obj"
	-@erase "$(INTDIR)\MainDlg.obj"
	-@erase "$(INTDIR)\Mini.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\Search.obj"
	-@erase "$(INTDIR)\Set.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\X�����ֲ�����.pch"
	-@erase "$(OUTDIR)\X�����ֲ�����.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\X�����ֲ�����.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /profile /machine:I386 /out:"$(OUTDIR)\X�����ֲ�����.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Lrc.obj" \
	"$(INTDIR)\MainDlg.obj" \
	"$(INTDIR)\Search.obj" \
	"$(INTDIR)\Set.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\resource.res" \
	".\SkinH.lib" \
	"$(INTDIR)\Mini.obj"

"$(OUTDIR)\X�����ֲ�����.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\X�����ֲ�����.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\resource.res" /d "NDEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("X�����ֲ�����.dep")
!INCLUDE "X�����ֲ�����.dep"
!ELSE 
!MESSAGE Warning: cannot find "X�����ֲ�����.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "���ֲ����� - Win32 Release"
SOURCE=.\Lrc.cpp

"$(INTDIR)\Lrc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\X�����ֲ�����.pch"


SOURCE=.\MainDlg.cpp

"$(INTDIR)\MainDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\X�����ֲ�����.pch"


SOURCE=.\Mini.cpp

"$(INTDIR)\Mini.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\X�����ֲ�����.pch"


SOURCE=.\resource.rc

"$(INTDIR)\resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Search.cpp

"$(INTDIR)\Search.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\X�����ֲ�����.pch"


SOURCE=.\Set.cpp

"$(INTDIR)\Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\X�����ֲ�����.pch"


SOURCE=.\StdAfx.cpp
CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\X�����ֲ�����.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\X�����ֲ�����.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<



!ENDIF 

