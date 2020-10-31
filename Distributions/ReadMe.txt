---- Distribution procedure on Windows ----

This deals with the distribution of a GigaVoxels/GigaSpace Windows installer and devkit

The tool NSIS is a scriptable tool used to generate installers on Windows.

[ 0 ]
You need to install NSIS tool.

[ 1 ]
First, you need to compile GigaVoxels and copy all data in Release\Bin
=> check the "HowTo.txt" file at root directory

[ 2 ]
Then, after installing NSIS, right click on the file "GigaVoxelsSDK.nsi". A menu appears and ask to "Compile script". Click on it.
A window open and create the installer with an output console to see the process and check for error.
At the end, you get an installer (based on the name of the output in the GigaVoxelsSDK.nsi" script file.

-----------------------------------------------------------------------------------

NOTE : the GigaVoxelsSDK.nsi file is very easy to understand. You just have to:
- set the name of the output
- for each file (or group of files), set the output path and copy files in destination directory.
You also have options to add sections in the installer.
