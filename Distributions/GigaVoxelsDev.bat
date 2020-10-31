rem GIGAVOXELS DEVELOPMENT ARCHIVE GENERATION

rem Set the zip directory
set ZIPDIR="C:\Program Files (x86)\7-Zip"

rem Initialize work directory
echo INITALIZE WORK DIRECTORY
mkdir GigaVoxelsSDK
cd GigaVoxelsSDK
mkdir GigaVoxels
cd GigaVoxels

rem Retrieve svn base
echo RETRIEVE SVN BASE
rem svn checkout https://scm.gforge.inria.fr/svn/gigavoxelsdist/trunk .
"C:\Program Files (x86)\Subversion\bin\svn" checkout https://scm.gforge.inria.fr/svn/gigavoxelsdist/trunk .

rem Delete svn file
echo DELETE SVN FILE
cd GigaVoxelsSDK
rd /s /q ".svn"
for /f "Tokens=*" %%G in ('dir /B /s') do (IF EXIST %%G\.svn  rd /s /q "%%G/.svn")

rem Delete Data directory
echo DELETE DATA DIRECTORY
rd /s /q Data

rem installer and devkit
echo DELETE DATA IN DISTRIBUTION DIRECTORY
cd Distributions
cd Windows
rd /s /q Distrib
mkdir Distrib
cd ..
cd Linux
rd /s /q Distrib
mkdir Distrib
cd ..
cd ..

rem Generate doxygen documentation
rem echo GENERATE DOXYGEN DOCUMENTATION
rem set CURRENTSCRIPTPATH=%CD%
rem cd Development/Doc
rem doxygen DoxyfileGigaVoxels.cfg
rem cd %CURRENTSCRIPTPATH%

rem Generate archive
echo GENERATE ARCHIVE
cd ..
%ZIPDIR%\7z.exe a ..\GigaVoxelsSDK.7z

rem Delete work directory
echo DELETE WORK DIRECTORY
cd ..
rd /s /q "GigaVoxelsSDK"

goto exit

:exit 
pause 