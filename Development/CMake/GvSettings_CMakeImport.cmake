#----------------------------------------------------------------
# Global GigaSpace Library Settings
#----------------------------------------------------------------

# Include guard
if (GvSettings_Included)
	return()
endif ()
set (GvSettings_Included true)

message (STATUS "IMPORT: GigaSpace Library Settings")

# Use colored output
set (CMAKE_COLOR_MAKEFILE ON)

#set(CMAKE_SKIP_RPATH TRUE)

#----------------------------------------------------------------
# Check architecture
#
# Note :
# CMAKE_SIZEOF_VOID_P is undefined if used before
# a call to the "project" command.
#----------------------------------------------------------------

IF ( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    MESSAGE ( STATUS "ARCHITECTURE : 32 bits" )
    SET (GV_DESTINATION_ARCH "x86")
ENDIF()
IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
    MESSAGE ( STATUS "ARCHITECTURE : 64 bits" )
    SET ( GV_DESTINATION_ARCH "x64" )
ENDIF()
MESSAGE ( STATUS "" )

#----------------------------------------------------------------
# GigaSpace Settings
#----------------------------------------------------------------

# SYSTEM_NAME should be Windows or Linux
SET (GV_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")

# Check the Linux distribution
if (NOT WIN32)
	file(READ "/etc/issue" ETC_ISSUE)
	string(REGEX MATCH "Debian|Ubuntu" DIST ${ETC_ISSUE})

	if(DIST STREQUAL "Debian")
		message(STATUS ">>>> Found Debian <<<<")
	elseif(DIST STREQUAL "Ubuntu")
		message(STATUS ">>>> Found Ubuntu <<<<")
	else()
		message(STATUS ">>>> Found unknown distribution <<<<")
	endif()
endif()

# Set Third Party Dependencies path
MESSAGE ( STATUS "THIRD PARTY DEPENDENCIES" )
IF ( ${GV_DESTINATION_ARCH} STREQUAL "x86" )
	SET (GV_EXTERNAL ${GV_ROOT}/External/${CMAKE_SYSTEM_NAME}/x86)
ENDIF()
IF ( ${GV_DESTINATION_ARCH} STREQUAL "x64" )
	SET (GV_EXTERNAL ${GV_ROOT}/External/${CMAKE_SYSTEM_NAME}/x64)
ENDIF()
MESSAGE ( STATUS "path : ${GV_EXTERNAL}" )
MESSAGE ( STATUS "" )

# Set Main GigaSpace RELEASE directory
# It will contain all generated executables, demos, tools, etc...
SET (GV_RELEASE ${GV_ROOT}/Release)

#----------------------------------------------------------------
# Defines
#----------------------------------------------------------------

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-DNOMINMAX)
endif ()

#----------------------------------------------------------------
# Third Library Dependencies
#----------------------------------------------------------------

# USER OPTION
OPTION (GV_USE_GIGASPACE_EXTERNALS "By default, use the 3rd library dependencies provided by GigaSpace. If not, user has to set PATH to its libraries." ON)

IF (GV_USE_GIGASPACE_EXTERNALS)

ELSE ()

ENDIF ()

#----------------------------------------------------------------
# Required packages
#----------------------------------------------------------------

# Search for CUDA
MESSAGE ( STATUS "REQUIRED PACKAGE : CUDA 5.0" )
find_package (CUDA 5.0 REQUIRED) # TO DO : utiliser FindCUDA à la place
if (CUDA_FOUND)
	message (STATUS "system has CUDA")
	message (STATUS "CUDA version : ${CUDA_VERSION_STRING}")
	message (STATUS "CUDA_TOOLKIT_ROOT_DIR = ${CUDA_TOOLKIT_ROOT_DIR}")
	message (STATUS "CUDA_INCLUDE_DIRS = ${CUDA_INCLUDE_DIRS}")
	message (STATUS "CUDA_LIBRARIES = ${CUDA_LIBRARIES}")
endif()
MESSAGE ( STATUS "" )

# Search for OpenGL
MESSAGE ( STATUS "REQUIRED PACKAGE : OpenGL" )
find_package (OpenGL REQUIRED) # TO DO : utiliser FindOpenGL à la place
if (OPENGL_FOUND)
	message (STATUS "system has OpenGL")
endif()
if (OPENGL_GLU_FOUND)
	message (STATUS "system has GLU")
endif()
if (OPENGL_XMESA_FOUND)
	message (STATUS "system has XMESA")
endif()
message (STATUS "OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
message (STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")
message (STATUS "OPENGL_gl_LIBRARY = ${OPENGL_gl_LIBRARY}")
message (STATUS "OPENGL_glu_LIBRARY = ${OPENGL_glu_LIBRARY}")
MESSAGE ( STATUS "" )

# Search for ImageMagick
MESSAGE ( STATUS "REQUIRED PACKAGE : ImageMagick" )
find_package (ImageMagick COMPONENTS Magick++)
MESSAGE ( STATUS "" )

#----------------------------------------------------------------
# ASSIMP library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_ASSIMP_RELEASE "${GV_EXTERNAL}/assimp")
	set (GV_ASSIMP_INC "${GV_ASSIMP_RELEASE}/include")
	set (GV_ASSIMP_LIB "${GV_ASSIMP_RELEASE}/lib")
else ()
	set (GV_ASSIMP_RELEASE "/usr")
	set (GV_ASSIMP_INC "${GV_ASSIMP_RELEASE}/include/assimp")
	set (GV_ASSIMP_LIB "${GV_ASSIMP_RELEASE}/lib")
endif ()

#----------------------------------------------------------------
# CIMG library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_CIMG_RELEASE "${GV_EXTERNAL}/CImg")
	set (GV_CIMG_INC "${GV_CIMG_RELEASE}")
else ()
	set (GV_CIMG_RELEASE "/usr")
	set (GV_CIMG_INC "${GV_CIMG_RELEASE}/include")
endif ()

#----------------------------------------------------------------
# CUDPP library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_CUDPP_RELEASE "${GV_EXTERNAL}/cudpp")
	set (GV_CUDPP_INC "${GV_CUDPP_RELEASE}/include")
	set (GV_CUDPP_LIB "${GV_CUDPP_RELEASE}/lib")
else ()
	#set (GV_CUDPP_RELEASE "/usr/local")
	set (GV_CUDPP_RELEASE "${GV_EXTERNAL}/cudpp")
	set (GV_CUDPP_INC "${GV_CUDPP_RELEASE}/include")
	set (GV_CUDPP_LIB "${GV_CUDPP_RELEASE}/lib")
endif ()

#----------------------------------------------------------------
# FREEGLUT library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_FREEGLUT_RELEASE "${GV_EXTERNAL}/freeglut")
	set (GV_FREEGLUT_INC "${GV_FREEGLUT_RELEASE}/include")
	set (GV_FREEGLUT_LIB "${GV_FREEGLUT_RELEASE}/lib")
else ()
	set (GV_FREEGLUT_RELEASE "/usr")
	set (GV_FREEGLUT_INC "${GV_FREEGLUT_RELEASE}/include")
	set (GV_FREEGLUT_LIB "${GV_FREEGLUT_RELEASE}/lib/x86_64-linux-gnu")
endif ()

#----------------------------------------------------------------
# GLEW library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_GLEW_RELEASE "${GV_EXTERNAL}/glew")
	set (GV_GLEW_INC "${GV_GLEW_RELEASE}/include")
	set (GV_GLEW_LIB "${GV_GLEW_RELEASE}/lib")
#	set (GV_GLEW_BIN "${GV_GLEW_RELEASE}/bin")
else ()
# TO DO : modify the following paths...
	#set (GV_GLEW_RELEASE "/usr")
	#set (GV_GLEW_INC "${GV_GLEW_RELEASE}/include")
	#set (GV_GLEW_LIB "${GV_GLEW_RELEASE}/lib64")
	#set (GV_GLEW_BIN "${GV_GLEW_RELEASE}/bin")
	set (GV_GLEW_RELEASE "${GV_EXTERNAL}/glew")
	set (GV_GLEW_INC "${GV_GLEW_RELEASE}/include")
	set (GV_GLEW_LIB "${GV_GLEW_RELEASE}/lib")
endif ()

#----------------------------------------------------------------
# CUDA SDK library settings
#----------------------------------------------------------------

# Idea : we should have our own HELPER file for this (operations on basic types : float2, int 4, */-+, etc...)
if (WIN32)
	set (GV_NVIDIAGPUCOMPUTINGSDK_RELEASE "C:/ProgramData/NVIDIA Corporation/CUDA Samples/v${CUDA_VERSION_STRING}")
	set (GV_NVIDIAGPUCOMPUTINGSDK_INC "${GV_NVIDIAGPUCOMPUTINGSDK_RELEASE}/common/inc")
else ()
	#set (GV_NVIDIAGPUCOMPUTINGSDK_RELEASE "/usr/local/cuda-${CUDA_VERSION_STRING}/samples")
	#set (GV_NVIDIAGPUCOMPUTINGSDK_RELEASE "${CUDA_TOOLKIT_ROOT_DIR}/samples")
	#set (GV_NVIDIAGPUCOMPUTINGSDK_INC "${GV_NVIDIAGPUCOMPUTINGSDK_RELEASE}/common/inc")
	# For the moment, we store the file helper_math.h in our Library
	set (GV_NVIDIAGPUCOMPUTINGSDK_INC "${GV_ROOT}/External/CommonLibraries/nvidia_helper/")
endif ()
message (STATUS "GigaSpace requires NVIDIA GPU Computing SDK. Check if include following directory is right.")
message (STATUS "NVIDIA GPU Computing SDK INC = ${GV_NVIDIAGPUCOMPUTINGSDK_INC}")
MESSAGE ( STATUS "" )

#----------------------------------------------------------------
# NV TOOLS library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_NVTOOLS_RELEASE "${CUDA_TOOLKIT_ROOT_DIR}/../../../NVIDIA Corporation/NvToolsExt")
	set (GV_NVTOOLS_INC "${GV_NVTOOLS_RELEASE}/include")
	set (GV_NVTOOLS_LIB "${GV_NVTOOLS_RELEASE}/lib")
	set (GV_NVTOOLS_BIN "${GV_NVTOOLS_RELEASE}/bin")
endif ()

#----------------------------------------------------------------
# IMAGEMAGICK library settings
#----------------------------------------------------------------

# The CMake FindImageMagick module define the following INCLUDE path
# ImageMagick_Magick++_INCLUDE_DIR

# The CMake FindImageMagick module define the ImageMagick_Magick++_LIBRARY filepath
# to the library, but not the path to the directory, that's why we use
# the IMAGEMAGICK_BINARY_PATH path on Windows.

if (WIN32)
	set (GV_IMAGEMAGICK_INC "${ImageMagick_Magick++_INCLUDE_DIR}")
	set (GV_IMAGEMAGICK_LIB "${ImageMagick_EXECUTABLE_DIR}/lib")
else ()
	set (GV_IMAGEMAGICK_INC "${ImageMagick_Magick++_INCLUDE_DIR}")
	set (GV_IMAGEMAGICK_LIB "/usr/lib")
endif ()

#----------------------------------------------------------------
# LOKI library settings
#
# NOTE
# GigaVoxels uses a modified version of the library in order to
# be able to use it in device code (i.e. on GPU).
#----------------------------------------------------------------

set (GV_LOKI_RELEASE "${GV_EXTERNAL}/Loki")
set (GV_LOKI_INC "${GV_LOKI_RELEASE}/include")

#----------------------------------------------------------------
# QGLVIEWER library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_QGLVIEWER_RELEASE "${GV_EXTERNAL}/QGLViewer")
	set (GV_QGLVIEWER_INC "${GV_QGLVIEWER_RELEASE}/include")
	set (GV_QGLVIEWER_LIB "${GV_QGLVIEWER_RELEASE}/lib")
#	set (GV_QGLVIEWER_BIN "${GV_QGLVIEWER_RELEASE}/bin")
else ()
	set (GV_QGLVIEWER_RELEASE "/usr")
	set (GV_QGLVIEWER_INC "${GV_QGLVIEWER_RELEASE}/include")
	set (GV_QGLVIEWER_LIB "${GV_QGLVIEWER_RELEASE}/lib/x86_64-linux-gnu")
#	set (GV_QGLVIEWER_BIN "${GV_QGLVIEWER_RELEASE}/bin")
endif ()

#----------------------------------------------------------------
# QT library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_QT_RELEASE "${GV_EXTERNAL}/Qt")
	#set (GV_QT_RELEASE "C:/Qt/Qt5.2.1/5.2.1/msvc2012_64_opengl")
	set (GV_QT_INC "${GV_QT_RELEASE}/include")
	set (GV_QT_LIB "${GV_QT_RELEASE}/lib")
	set (GV_QT_BIN "${GV_QT_RELEASE}/bin")
else ()
	set (GV_QT_RELEASE "/usr")
	set (GV_QT_INC "${GV_QT_RELEASE}/include/qt4")
	set (GV_QT_LIB "${GV_QT_RELEASE}/lib/x86_64-linux-gnu")
	set (GV_QT_BIN "${GV_QT_RELEASE}/bin")
endif ()

# Where to find the uic, moc and rcc tools

if (WIN32)
	set (GV_QT_UIC_EXECUTABLE ${GV_QT_BIN}/uic)
	set (GV_QT_MOC_EXECUTABLE ${GV_QT_BIN}/moc)
	set (GV_QT_RCC_EXECUTABLE ${GV_QT_BIN}/rcc)
else ()
	#set (GV_QT_UIC_EXECUTABLE ${GV_QT_BIN}/uic)
	#set (GV_QT_MOC_EXECUTABLE ${GV_QT_BIN}/moc)
	#set (GV_QT_RCC_EXECUTABLE ${GV_QT_BIN}/rcc)
	set (GV_QT_UIC_EXECUTABLE ${GV_QT_LIB}/qt4/bin/uic)
	set (GV_QT_MOC_EXECUTABLE ${GV_QT_LIB}/qt4/bin/moc)
	set (GV_QT_RCC_EXECUTABLE ${GV_QT_LIB}/qt4/bin/rcc)
endif ()

message (STATUS "Qt UIC : ${GV_QT_UIC_EXECUTABLE}")
message (STATUS "Qt MOC : ${GV_QT_MOC_EXECUTABLE}")
message (STATUS "Qt RCC : ${GV_QT_RCC_EXECUTABLE}")

#----------------------------------------------------------------
# QTFE library settings
#----------------------------------------------------------------

set (GV_QTFE_RELEASE "${GV_EXTERNAL}/Qtfe")
set (GV_QTFE_INC "${GV_QTFE_RELEASE}/include")
set (GV_QTFE_LIB "${GV_QTFE_RELEASE}/lib")
#set (GV_QTFE_BIN "${GV_QTFE_RELEASE}/bin")

#----------------------------------------------------------------
# DCMTK library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_DCMTK_RELEASE "${GV_EXTERNAL}/DCMTK")
	set (GV_DCMTK_INC "${GV_DCMTK_RELEASE}/include")
	set (GV_DCMTK_LIB "${GV_DCMTK_RELEASE}/lib")
else ()
	# Search for DCMTK
	find_package( DCMTK )
	IF ( DCMTK_FOUND )
		set (GV_DCMTK_INC ${DCMTK_INCLUDE_DIRS})
		set (GV_DCMTK_LIB ${DCMTK_LIBRARIES})
	ELSE ()
		MESSAGE ( STATUS "" )
		MESSAGE ( STATUS "WARNING : Unable to find DCMTK on the system" )
	ENDIF ()
endif ()

#----------------------------------------------------------------
# QWT library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_QWT_RELEASE "${GV_EXTERNAL}/Qwt")
	set (GV_QWT_INC "${GV_QWT_RELEASE}/include")
	set (GV_QWT_LIB "${GV_QWT_RELEASE}/lib")
else ()
	set (GV_QWT_RELEASE "/usr")
	set (GV_QWT_INC "${GV_QWT_RELEASE}/include/qwt")
	set (GV_QWT_LIB "${GV_QWT_RELEASE}/lib")
endif ()

#----------------------------------------------------------------
# GLM (OpenGL Mathematics) library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_GLM_RELEASE "${GV_EXTERNAL}/glm")
	set (GV_GLM_INC "${GV_GLM_RELEASE}")
else ()
	set (GV_GLM_RELEASE "${GV_EXTERNAL}/glm")
	set (GV_GLM_INC "${GV_GLM_RELEASE}")
endif ()

#----------------------------------------------------------------
# OGRE3D library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_OGRE3D_RELEASE "${GV_EXTERNAL}/Ogre3D")
	set (GV_OGRE3D_INC "${GV_OGRE3D_RELEASE}/include")
	set (GV_OGRE3D_LIB "${GV_OGRE3D_RELEASE}/lib")
#	set (GV_OGRE3D_BIN "${GV_OGRE3D_RELEASE}/bin")
else ()
	set (GV_OGRE3D_RELEASE "/usr/local")
	set (GV_OGRE3D_INC "${GV_OGRE3D_RELEASE}/include")
	set (GV_OGRE3D_LIB "${GV_OGRE3D_RELEASE}/lib")
#	set (GV_OGRE3D_BIN "${GV_OGRE3D_RELEASE}/bin")
endif ()

#----------------------------------------------------------------
# TinyXML library settings
#----------------------------------------------------------------

if (WIN32)
	set (GV_TINYXML_RELEASE "${GV_EXTERNAL}/tinyxml")
	set (GV_TINYXML_INC "${GV_TINYXML_RELEASE}/include")
	set (GV_TINYXML_LIB "${GV_TINYXML_RELEASE}/lib")
#	set (GV_TINYXML_BIN "${GV_TINYXML_RELEASE}/bin")
else ()
	set (GV_TINYXML_RELEASE "/usr/local")
	set (GV_TINYXML_INC "${GV_TINYXML_RELEASE}/include")
	set (GV_TINYXML_LIB "${GV_TINYXML_RELEASE}/lib")
#	set (GV_TINYXML_BIN "${GV_TINYXML_RELEASE}/bin")
endif ()

#----------------------------------------------------------------
# Options
#----------------------------------------------------------------

option (GV_ENABLE_DEBUGABLE_DEVICE_CODE "Enable/disable generation of debug-able device code" OFF)
option (GV_USE_CUDPP_LIBRARY "Enable/disable use of CUDPP library to speed up some device kernels. If OFF, it uses Thrust library." ON)

option ( GV_USE_64_BITS "Enable/disable compilation in 64 bits." ON )
if ( GV_USE_64_BITS )
    set ( GV_SYSTEM_PROCESSOR "x64" )
else()
	set ( GV_SYSTEM_PROCESSOR "x86" )
endif()

option (GV_ENABLE_DEBUGABLE_DEVICE_CODE "Enable/disable generation of debug-able device code" OFF)

#----------------------------------------------------------------
# CUDA : additional NVCC command line arguments
# NOTE: multiple arguments must be semi-colon delimited (e.g. --compiler-options;-Wall)
#----------------------------------------------------------------

#list(APPEND CUDA_NVCC_FLAGS --keep-dir;"Debug")
#list(APPEND CUDA_NVCC_FLAGS --compile)
#list(APPEND CUDA_NVCC_FLAGS -maxrregcount=0)

# Set your compute capability version
#
# GigaVoxels requires 2.0 at least
#
# NOTE : choose if you want virtual mode or not with "gencode" and "arch" keywords.
# - you can also choose to embed several architectures.
#
#list(APPEND CUDA_NVCC_FLAGS -gencode=arch=compute_20,code=\"sm_20,compute_20\")
#
#list(APPEND CUDA_NVCC_FLAGS "-gencode=arch=compute_20,code=sm_20 -gencode=arch=compute_20,code=compute_20")
#list(APPEND CUDA_NVCC_FLAGS "-gencode=arch=compute_30,code=sm_30 -gencode=arch=compute_30,code=compute_30")
#list(APPEND CUDA_NVCC_FLAGS "-gencode=arch=compute_50,code=sm_50 -gencode=arch=compute_50,code=compute_50")
#
list(APPEND CUDA_NVCC_FLAGS "-arch=sm_20")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_21")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_30")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_32")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_35")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_37")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_50")
#list(APPEND CUDA_NVCC_FLAGS "-arch=sm_52")

# NVCC verbose mode : set this flag to see detailed NVCC statistics (registers usage, memory, etc...)
#list(APPEND CUDA_NVCC_FLAGS -Xptxas;-v)

# Generate line-number information for device code
#list(APPEND CUDA_NVCC_FLAGS -lineinfo)

# Max nb registers
#list(APPEND CUDA_NVCC_FLAGS -maxrregcount=32)

# Use fast math
list(APPEND CUDA_NVCC_FLAGS -use_fast_math)

# NSight Debugging
# - debug on CPU
#list(APPEND CUDA_NVCC_FLAGS -g)
# - debug on GPU
#list(APPEND CUDA_NVCC_FLAGS -G)

# Set this flag to see detailed NVCC command lines
#set (CUDA_VERBOSE_BUILD "ON")

message (STATUS "CUDA NVCC info (command line) = ${CUDA_NVCC_FLAGS}")
MESSAGE ( STATUS "" )
