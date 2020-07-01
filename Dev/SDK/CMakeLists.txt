#----------------------------------------------------------------
# GigaSpace PROJECT CMake file
# Main user file
#----------------------------------------------------------------

#----------------------------------------------------------------
# Project
#----------------------------------------------------------------

# Invite message
message( STATUS "----------------------------------------" )
message( STATUS "- PROJECT GENERATION : GigaSpace Tools -" )
message( STATUS "----------------------------------------" )
message( STATUS "" )

# Project name
#project( GigaSpaceTools )

#----------------------------------------------------------------
# Add Library/Tools path
#----------------------------------------------------------------

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/GsGraphics" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/GsCompute" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/GigaSpace" )
