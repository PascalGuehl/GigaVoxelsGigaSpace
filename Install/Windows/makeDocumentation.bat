@echo off

set CURRENTSCRIPTPATH=%CD%

cd ..
cd ..
cd Development
cd Documents
cd Doxygen

doxygen DoxyfileGigaSpace.cfg

cd %CURRENTSCRIPTPATH%

pause