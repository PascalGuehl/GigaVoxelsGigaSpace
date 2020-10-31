@echo off

rem /c : continue if error
rem /y : suppress confirmation for replacing
rem /d : copy if more recent

rem **************************************************************************
rem Setups environment variables
rem **************************************************************************

rem PATH : GigaVoxels RELEASE directory
SET GV_RELEASE=%CD%\..\..\Release
SET GS_DATA=%CD%\..\..\Data

rem PATH : GigaVoxels EXTERNALS directory (third party dependencies)
rem 32 bits mode :
rem SET GV_EXTERNAL=%CD%\..\..\External\Windows\x86
rem 64 bits mode :
SET GV_EXTERNAL=%CD%\..\..\External\Windows\x64

rem Create paths
rem - Release
mkdir "%GV_RELEASE%\Bin"
rem - Settings
mkdir "%GV_RELEASE%\Bin\Settings"

rem **************************************************************************
rem Settings
rem **************************************************************************

xcopy /c /y /d "%GS_DATA%\Settings\GigaSpace.xml" "%GV_RELEASE%\Bin\Settings"
xcopy /c /y /d "%GS_DATA%\Settings\GsViewer.xml" "%GV_RELEASE%\Bin\Settings"

rem **************************************************************************
rem Scripts
rem **************************************************************************

xcopy /y /c /d "Tools\launchViewerDebug.bat" "%GV_RELEASE%\Bin"
xcopy /y /c /d "Tools\launchViewerRelease.bat" "%GV_RELEASE%\Bin"

rem **************************************************************************
rem cudpp
rem **************************************************************************

rem xcopy /y /c /d "%GV_EXTERNAL%\cudpp\lib\cudpp32.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\cudpp\lib\cudpp32d.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\cudpp\lib\cudpp64.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\cudpp\lib\cudpp64d.dll" "%GV_RELEASE%\Bin" 

rem **************************************************************************
rem NvTools
rem **************************************************************************

rem xcopy /y /c /d "%NVTOOLSEXT_PATH%\bin\Win32\nvToolsExt32_1.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%NVTOOLSEXT_PATH%\bin\x64\nvToolsExt64_1.dll" "%GV_RELEASE%\Bin" 

rem **************************************************************************
rem freeglut
rem **************************************************************************

xcopy /y /c /d "%GV_EXTERNAL%\freeglut\bin\freeglut.dll" "%GV_RELEASE%\Bin" 

rem **************************************************************************
rem glew
rem **************************************************************************

xcopy /y /c /d "%GV_EXTERNAL%\glew\bin\glew32.dll" "%GV_RELEASE%\Bin" 

rem **************************************************************************
rem assimp
rem **************************************************************************

rem xcopy /y /c /d "%GV_EXTERNAL%\assimp\bin\Assimp32.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\assimp\bin\Assimp32d.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\assimp\bin\Assimp64.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\assimp\bin\Assimp64d.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\assimp\bin\assimp.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\assimp\bin\assimpd.dll" "%GV_RELEASE%\Bin" 

rem **************************************************************************
rem QGLViewer
rem **************************************************************************

xcopy /y /c /d "%GV_EXTERNAL%\QGLViewer\bin\QGLViewer2.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\QGLViewer\bin\QGLViewerd2.dll" "%GV_RELEASE%\Bin" 

rem **************************************************************************
rem Qt
rem **************************************************************************

xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtCore4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtCored4.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtDesigner4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtDesignerd4.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtDesignerComponents4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtDesignerComponentsd4.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtGui4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtGuid4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtNetwork4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtNetworkd4.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtOpenGL4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtOpenGLd4.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtSvg4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtSvgd4.dll" "%GV_RELEASE%\Bin"
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtXml4.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\QtXmld4.dll" "%GV_RELEASE%\Bin"

rem Qt 5

rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Core.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Cored.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Designer.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Designerd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5DesignerComponents.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5DesignerComponentsd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Gui.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Guid.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Network.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Networkd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5OpenGL.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5OpenGLd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Svg.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Svgd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Xml.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Xmld.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Widgets.dll" "%GV_RELEASE%\Bin" 
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5Widgetsd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5PrintSupport.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\Qt5PrintSupportd.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\icudt51.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\icuin51.dll" "%GV_RELEASE%\Bin"
rem xcopy /y /c /d "%GV_EXTERNAL%\Qt\bin\icuuc51.dll" "%GV_RELEASE%\Bin"

rem **************************************************************************
rem Qwt
rem **************************************************************************

xcopy /y /c /d "%GV_EXTERNAL%\Qwt\lib\qwt.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qwt\lib\qwtd.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qwt\lib\qwtmathml.dll" "%GV_RELEASE%\Bin" 
xcopy /y /c /d "%GV_EXTERNAL%\Qwt\lib\qwtmathmld.dll" "%GV_RELEASE%\Bin"

rem **************************************************************************
rem Data
rem TO DO :
rem -- This is a temporary solution.
rem -- Find a way to store and load data with a GvRessourceManager singleton.
rem **************************************************************************

rem xcopy /y /c /d "%GV_RELEASE%\..\Media\TransferFunction\TransferFunction.xml" "%GV_RELEASE%\Bin"

pause
