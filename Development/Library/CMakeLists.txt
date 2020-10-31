#----------------------------------------------------------------
# GigaSpace LIBRARY CMake file
# Main user file
#----------------------------------------------------------------

# Check CMAKE version
cmake_minimum_required (VERSION 2.8)

#----------------------------------------------------------------
# Project
#----------------------------------------------------------------

# Invite message
MESSAGE ( STATUS "-----------------------------------------------------" )
MESSAGE ( STATUS "- PROJECT GENERATION : GigaVoxels/GigaSpace Library -" )
MESSAGE ( STATUS "-----------------------------------------------------" )

# Project name
project (Library)

# Tell CMake to first search in directories listed in CMAKE_MODULE_PATH when 
# using FIND_PACKAGE() or INCLUDE()
SET (GV_ROOT "${CMAKE_SOURCE_DIR}/../..")
SET (CMAKE_MODULE_PATH "${GV_ROOT}/Development/CMake")

# Include common settings
INCLUDE (GvSettings_CMakeImport)

#----------------------------------------------------------------
# Initialize some environment variables
#----------------------------------------------------------------
SET (RELEASE_BIN_DIR ${GV_RELEASE}/Bin)
SET (RELEASE_LIB_DIR ${GV_RELEASE}/Lib)
SET (RELEASE_INC_DIR ${GV_RELEASE}/Inc)

#----------------------------------------------------------------
# Add GigaSpace Library path
#----------------------------------------------------------------
add_subdirectory ("${CMAKE_SOURCE_DIR}/GigaSpace")
add_subdirectory ("${CMAKE_SOURCE_DIR}/GsGraphics")
add_subdirectory ("${CMAKE_SOURCE_DIR}/GsCompute")
