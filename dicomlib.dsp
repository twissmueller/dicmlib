# Microsoft Developer Studio Project File - Name="dicomlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=dicomlib - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "dicomlib.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "dicomlib.mak" CFG="dicomlib - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "dicomlib - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "dicomlib - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dicomlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "dicomlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(USRINCLUDE)\dcmtk351" /I "$(QTDIR)\include" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Parallel" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Hybrid" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Patented" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Rendering" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\IO" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Imaging" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Graphics" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Filtering" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Common" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Common\Testing\Cxx" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Utilities\zlib" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Utilities\jpeg" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Utilities\png" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Utilities\tiff" /I "C:\Documents and Settings\development\My Documents\Projects\external\VTK-4.2.2\Utilities\expat" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "dicomlib - Win32 Release"
# Name "dicomlib - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DcmtkBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\DcmtkConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\DcmtkHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\DcmtkQtWidgetFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomFacade.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomToVtk.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DcmtkBuilder.h
# End Source File
# Begin Source File

SOURCE=.\DcmtkConvert.h
# End Source File
# Begin Source File

SOURCE=.\DcmtkHelper.h
# End Source File
# Begin Source File

SOURCE=.\DcmtkQtWidgetFactory.h
# End Source File
# Begin Source File

SOURCE=.\DicomFacade.h
# End Source File
# Begin Source File

SOURCE=.\DicomLibException.h
# End Source File
# Begin Source File

SOURCE=.\DicomToVtk.h
# End Source File
# End Group
# Begin Group "Widgets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DicomDirQtListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomDirQtListView.h
# End Source File
# Begin Source File

SOURCE=.\DicomDirQtWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomDirQtWidget.h

!IF  "$(CFG)" == "dicomlib - Win32 Release"

!ELSEIF  "$(CFG)" == "dicomlib - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName)...
ProjDir=.
InputPath=.\DicomDirQtWidget.h
InputName=DicomDirQtWidget

"$(Projdir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(Projdir)\$(InputName).h -o $(Projdir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DicomDumpQtListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomDumpQtListView.h
# End Source File
# End Group
# Begin Group "Generierte Dateien"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\moc_DicomDirQtWidget.cpp
# End Source File
# End Group
# End Target
# End Project
