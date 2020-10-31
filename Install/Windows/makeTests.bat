@echo off

rem **************************************************************************
rem USER : Choose your compiler
rem **************************************************************************

rem Call the script selecting the CMAKE compiler (Visual Studio 2010, 2012, etc...)
call cmakeGeneratorSettings.bat

rem **************************************************************************
rem CMAKE GENERATION
rem **************************************************************************

if %GV_COMPILER%==Visual_Studio_9_2008 (
    call :generate_Visual_Studio_9_2008
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_9_2008_Win64 (
    call :generate_Visual_Studio_9_2008_Win64
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_10 (
    call :generate_Visual_Studio_10
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_10_Win64 (
    call :generate_Visual_Studio_10_Win64
	goto :finish
) else if %GV_COMPILER%==Unix_Makefiles (
    call :generate_Unix_Makefiles
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_11 (
    call :generate_Visual_Studio_11
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_11_Win64 (
    call :generate_Visual_Studio_11_Win64
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_12 (
    call :generate_Visual_Studio_12
	goto :finish
) else if %GV_COMPILER%==Visual_Studio_12_Win64 (
    call :generate_Visual_Studio_12_Win64
	goto :finish
)

:generate_Visual_Studio_9_2008
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC9_x86
cd Generated_VC9_x86
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 9 2008" ..\..\Development\Tests
rem exit
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_9_2008_Win64
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC9_x64
cd Generated_VC9_x64
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 9 2008 Win64" ..\..\Development\Tests
rem exit
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_10
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC10_x86
cd Generated_VC10_x86
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 10" ..\..\Development\Tests
rem exit
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_10_Win64
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC10_x64
cd Generated_VC10_x64
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 10 Win64" ..\..\Development\Tests
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Unix_Makefiles
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_Linux
cd Generated_Linux
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Unix Makefiles" ../../Development/Tests
rem exit
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_11
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC11_x86
cd Generated_VC11_x86
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 11" ..\..\Development\Tests
rem exit
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_11_Win64
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC11_x64
cd Generated_VC11_x64
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 11 Win64" ..\..\Development\Tests
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_12
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC12_x86
cd Generated_VC12_x86
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 12" ..\..\Development\Tests
rem exit
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:generate_Visual_Studio_12_Win64
set CURRENTSCRIPTPATH=%CD%
cd ..
cd ..
mkdir Generated_VC12_x64
cd Generated_VC12_x64
mkdir Tests
cd Tests
rem CMake the application
cmake -G "Visual Studio 12 Win64" ..\..\Development\Tests
if NOT ERRORLEVEL 0 pause
cd %CURRENTSCRIPTPATH%
pause
goto :finish

:finish
