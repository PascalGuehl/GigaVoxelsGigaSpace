@echo off

rem **************************************************************************
rem CMake projects generation
rem **************************************************************************

rem Call each script of CMake generation (library, dems, tools, viewerPlugins)
call makeLibrary.bat
call makeDemoTutorials.bat
call makeTools.bat
call makeViewerPluginTutorials.bat

pause
