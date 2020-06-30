#----------------------------------------------------------------
# PROJECT CMake file
# Main user file
#----------------------------------------------------------------

# Check CMAKE version
cmake_minimum_required( VERSION 3.0 )

#----------------------------------------------------------------
# Project
#----------------------------------------------------------------

# Invite message
message( STATUS "----------------------------------------" )
message( STATUS "- PROJECT GENERATION : ASSIMP 3rdParty -" )
message( STATUS "----------------------------------------" )
message( STATUS "" )

# Project name
project( assimpProject )

# Automatic cross-plateform compilation of assimp

include( ExternalProject )

SET(
	assimp_CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_DEBUG_POSTFIX=d
	-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
	-DASSIMP_BUILD_ASSIMP_VIEW=OFF
	-DASSIMP_BUILD_SAMPLES=OFF
    -DBUILD_TESTING=OFF
	-DASSIMP_BUILD_TESTS=OFF
	-DASSIMP_INSTALL_PDB=ON
	#-DCMAKE_CONFIGURATION_TYPES=Debug;Release
	#-DCMAKE_CONFIGURATION_TYPES:STRING=${BUILD_TYPE} 
	#-DASSIMP_LIBRARY_SUFFIX=
	#-DLIBRARY_SUFFIX=
)

ExternalProject_Add(
	assimp
	INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../assimp"
	BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/assimp-build"
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v4.1.0
	GIT_SHALLOW ON
	CMAKE_ARGS ${CMAKE_ARGS} ${assimp_CMAKE_ARGS}
)
