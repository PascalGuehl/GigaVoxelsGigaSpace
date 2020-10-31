@echo off

rem **************************************************************************
rem USER : Choose your compiler
rem **************************************************************************

rem Select requested CMAKE compiler (Visual Studio 2010, 2012, etc...)
rem set GV_COMPILER=Visual_Studio_9_2008
rem set GV_COMPILER=Visual_Studio_9_2008_Win64
rem set GV_COMPILER=Visual_Studio_10
set GV_COMPILER=Visual_Studio_10_Win64
rem set GV_COMPILER=Visual_Studio_11
rem set GV_COMPILER=Visual_Studio_11_Win64
rem set GV_COMPILER=Visual_Studio_12
rem set GV_COMPILER=Visual_Studio_12_Win64

rem **************************************************************************
rem 7z path : tools to uncompress archives
rem **************************************************************************

set GV_7ZIP_PATH=C:\Program Files (x86)\7-Zip
rem set GV_7ZIP_PATH=C:\Program Files\7-Zip
