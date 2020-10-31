@echo off

rem **************************************************************************
rem Setups environment variables
rem **************************************************************************

rem PATH : GigaVoxels RELEASE directory
SET GV_RELEASE=%CD%\..\..\Release

rem PATH : GigaVoxels DATA directory
SET GV_DATA=%CD%\..\..\Data

rem Create paths
rem - Release
mkdir "%GV_RELEASE%\Bin"
rem - Resources
mkdir "%GV_RELEASE%\Bin\Resources"
mkdir "%GV_RELEASE%\Bin\Resources\Icons"

rem **************************************************************************
rem Icons
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\Icons\*.*" "%GV_RELEASE%\Bin\Resources\Icons"

rem **************************************************************************
rem Shaders
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\Shaders\*.*" "%GV_RELEASE%\Bin\Data\Shaders\"

rem **************************************************************************
rem TransferFunctions
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\TransferFunctions\*.*" "%GV_RELEASE%\Bin\Data\TransferFunctions\"

rem **************************************************************************
rem Voxels
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\Voxels\xyzrgb_dragon512_BR8_B1\*.*" "%GV_RELEASE%\Bin\Data\Voxels\xyzrgb_dragon512_BR8_B1\"

xcopy /y /c /d "%GV_DATA%\Voxels\vd4\*.*" "%GV_RELEASE%\Bin\Data\Voxels\vd4\"

xcopy /y /c /d "%GV_DATA%\Voxels\Raw\aneurism\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Raw\aneurism\"
xcopy /y /c /d "%GV_DATA%\Voxels\Raw\bonsai\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Raw\bonsai\"
xcopy /y /c /d "%GV_DATA%\Voxels\Raw\foot\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Raw\foot\"
xcopy /y /c /d "%GV_DATA%\Voxels\Raw\hydrogenAtom\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Raw\hydrogenAtom\"
xcopy /y /c /d "%GV_DATA%\Voxels\Raw\neghip\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Raw\neghip\"
xcopy /y /c /d "%GV_DATA%\Voxels\Raw\skull\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Raw\skull\"

xcopy /y /c /d "%GV_DATA%\Voxels\Dino\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Dino\"

xcopy /y /c /d "%GV_DATA%\Voxels\Fan\*.*" "%GV_RELEASE%\Bin\Data\Voxels\Fan\"

rem **************************************************************************
rem 3D Models
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\3DModels\*.*" "%GV_RELEASE%\Bin\Data\3DModels\"

rem **************************************************************************
rem Videos
rem **************************************************************************

mkdir "%GV_RELEASE%\Bin\Data\Videos"

rem **************************************************************************
rem Sky Box
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\SkyBox\*.*" "%GV_RELEASE%\Bin\Data\SkyBox\"

rem **************************************************************************
rem Terrain
rem **************************************************************************

xcopy /y /c /d "%GV_DATA%\Terrain\*.*" "%GV_RELEASE%\Bin\Data\Terrain\"

pause
