#----------------------------------------------------------------
# GigaSpace PROJECT CMake file
# Main user file
#----------------------------------------------------------------

# Check CMAKE version
cmake_minimum_required( VERSION 3.0 )

#----------------------------------------------------------------
# Project
#----------------------------------------------------------------

# Invite message
message( STATUS "------------------------------------------" )
message( STATUS "- PROJECT GENERATION : GigaSpace Library -" )
message( STATUS "------------------------------------------" )
message( STATUS "" )

# Project name
project( GigaSpaceProject )

set( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/Install/GigaSpaceProject" )

# Add PATH variables to be retrieved in source code
set( GS_PROJECT_PATH "${CMAKE_SOURCE_DIR}/.." )
set( GS_DATA_PATH "${GS_PROJECT_PATH}/Data" )
set( GS_IMAGE_PATH "${GS_DATA_PATH}/Images" )
set( GS_SHADER_PATH "${GS_DATA_PATH}/Shaders" )
get_filename_component( GS_PROJECT_PATH "${CMAKE_SOURCE_DIR}/.." ABSOLUTE )
get_filename_component( GS_DATA_PATH "${GS_PROJECT_PATH}/Data" ABSOLUTE )
get_filename_component( GS_IMAGE_PATH "${GS_DATA_PATH}/Images" ABSOLUTE )
get_filename_component( GS_SHADER_PATH "${GS_DATA_PATH}/Shaders" ABSOLUTE )
add_definitions( -DGS_PROJECT_PATH="${GS_PROJECT_PATH}" )
add_definitions( -DGS_DATA_PATH="${GS_DATA_PATH}" )
add_definitions( -DGS_IMAGE_PATH="${GS_IMAGE_PATH}" )
add_definitions( -DGS_SHADER_PATH="${GS_SHADER_PATH}" )

set( GS_BIN_PATH "${GS_PROJECT_PATH}/Dev/Install/GigaSpaceProject/bin" )
get_filename_component( GS_BIN_PATH "${GS_PROJECT_PATH}/Dev/Install/GigaSpaceProject/bin" ABSOLUTE )
add_definitions( -DGS_BIN_PATH="${GS_BIN_PATH}" )

#----------------------------------------------------------------
# Add Library/Tools path
#----------------------------------------------------------------

# SDK
add_subdirectory( "${CMAKE_SOURCE_DIR}/SDK" )

# Demos
#add_subdirectory( "${CMAKE_SOURCE_DIR}/Demos" )

# Tools
#add_subdirectory( "${CMAKE_SOURCE_DIR}/Tools" )

# 3rdParty
# - glad
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/bin
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Install/GigaSpaceProject
    FILES_MATCHING
    PATTERN "*.dll"
)
# - glfw
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/bin
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Install/GigaSpaceProject
    FILES_MATCHING
    PATTERN "*.dll"
)# - assimp
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/assimp/bin
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Install/GigaSpaceProject
    FILES_MATCHING
    PATTERN "*.dll"
)

# Configuration files
#file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Data/ImGui/imgui.ini DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Install/GigaSpaceProject/bin )
