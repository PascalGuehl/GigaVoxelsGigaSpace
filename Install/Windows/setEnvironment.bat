@echo off

rem **************************************************************************
rem Setups GigaVoxels environment variables
rem **************************************************************************

SET GV_RELEASE=%CD%\..\..\Release
SET GV_EXTERNAL=%CD%\..\..\External
SET GV_SOURCES=%CD%\..\..\Development

rem SET GV_PROC_ARCHI=x64
rem SET GV_PROC_ARCHI=x86

rem **************************************************************************
rem Setups Qt environment variables
rem **************************************************************************

SET GV_ASSIMP_RELEASE=%GV_EXTERNAL%\Windows\assimp--2.0.863-sdk
SET GV_ASSIMP_INC=%GV_ASSIMP_RELEASE%\include
SET GV_ASSIMP_LIB=%GV_ASSIMP_RELEASE%\lib
SET GV_ASSIMP_BIN=%GV_ASSIMP_RELEASE%\bin

SET GV_CUDPP_RELEASE=%GV_EXTERNAL%\Windows\cudpp
SET GV_CUDPP_INC=%GV_CUDPP_RELEASE%\include
SET GV_CUDPP_LIB=%GV_CUDPP_RELEASE%\lib

SET GV_FREEGLUT_RELEASE=%GV_EXTERNAL%\Windows\freeglut
SET GV_FREEGLUT_INC=%GV_EXTERNAL%\include
SET GV_FREEGLUT_LIB=%GV_EXTERNAL%\lib
SET GV_FREEGLUT_BIN=%GV_EXTERNAL%\bin

SET GV_GLEW_RELEASE=%GV_EXTERNAL%\Windows\glew-1.7.0
SET GV_GLEW_INC=%GV_GLEW_RELEASE%\include
SET GV_GLEW_LIB=%GV_GLEW_RELEASE%\lib
SET GV_GLEW_BIN=%GV_GLEW_RELEASE%\bin

SET GV_LOKI_RELEASE=%GV_EXTERNAL%\Windows\Loki
SET GV_GLEW_INC=%GV_LOKI_RELEASE%

SET GV_QGLVIEWER_RELEASE=%GV_EXTERNAL%\Windows\QGLViewer
SET GV_QGLVIEWER_INC=%GV_QGLVIEWER_RELEASE%
SET GV_QGLVIEWER_LIB=%GV_QGLVIEWER_RELEASE%\Lib
SET GV_QGLVIEWER_BIN=%GV_QGLVIEWER_RELEASE%\Bin

SET GV_QT_RELEASE=%GV_EXTERNAL%\Windows\Qt
SET GV_QT_INC=%GV_QT_RELEASE%\include
SET GV_QT_LIB=%GV_QT_RELEASE%\lib
SET GV_QT_BIN=%GV_QT_RELEASE%\bin

SET GV_QTFE_RELEASE=%GV_EXTERNAL%\Windows\Qtfe
SET GV_QTFE_INC=%GV_QTFE_RELEASE%\include
SET GV_QTFE_LIB=%GV_QTFE_RELEASE%\lib

rem CUDA is mandatory, no need to include
rem SET GV_CUDATOOLKIT_RELEASE=%GV_EXTERNAL%\Windows\Qtfe
rem SET GV_CUDATOOLKIT_INC=%GV_CUDATOOLKIT_RELEASE%\Windows\Qtfe
rem SET GV_CUDATOOLKIT_LIB=%GV_CUDATOOLKIT_RELEASE%\Windows\Qtfe
rem SET GV_CUDATOOLKIT_BIN=%GV_CUDATOOLKIT_RELEASE%\Windows\Qtfe

SET GV_CUDAGPUCOMPUTINGSDK_RELEASE=%NVSDKCOMPUTE_ROOT%
SET GV_CUDAGPUCOMPUTINGSDK_INC=%GV_CUDAGPUCOMPUTINGSDK_RELEASE%\C\common\inc
