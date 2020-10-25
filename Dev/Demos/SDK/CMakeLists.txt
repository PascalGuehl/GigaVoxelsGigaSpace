#----------------------------------------------------------------
# GigaSpace PROJECT CMake file
# Main user file
#----------------------------------------------------------------

#----------------------------------------------------------------
# Project
#----------------------------------------------------------------

# Invite message
message( STATUS "----------------------------------------" )
message( STATUS "- PROJECT GENERATION : GigaSpace Demos -" )
message( STATUS "----------------------------------------" )
message( STATUS "" )

# Project name
#project( GigaSpaceTools 

#----------------------------------------------------------------
# Add Demos path
#----------------------------------------------------------------

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/GsViewer" )
