# Invite message
message( STATUS "-----------------------------------------" )
message( STATUS "- PROJECT GENERATION : GigaSpace Module -" )
message( STATUS "-----------------------------------------" )
message( STATUS "" )

##################################################################################
# Copy 3rd party dependencies
##################################################################################

##################################################################################
# Copy resources
##################################################################################

##################################################################################
# Project
##################################################################################

# Project name
project( SimpleSphereDemo )

# Set parallel build mode
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP" )

# Remove some compilation warnings
add_definitions( -D_CRT_SECURE_NO_WARNINGS ) 

# GLFW
add_definitions( -DGLFW_DLL )

##################################################################################
# Package Management
##################################################################################

# OpenGL
find_package( OpenGL REQUIRED )

# CUDA
find_package( CUDA REQUIRED )
# - add current GPU architecture at runtime
set(CUDA_ARCH_LIST Auto CACHE STRING
    "List of CUDA architectures (e.g. Pascal, Volta, etc) or \
compute capability versions (6.1, 7.0, etc) to generate code for. \
Set to Auto for automatic detection (default)."
)
cuda_select_nvcc_arch_flags( CUDA_ARCH_FLAGS ${CUDA_ARCH_LIST} )

# Idea : we should have our own HELPER file for this (operations on basic types : float2, int 4, */-+, etc...)
if (WIN32)
	set (GS_NVIDIAGPUCOMPUTINGSDK_RELEASE "C:/ProgramData/NVIDIA Corporation/CUDA Samples/v${CUDA_VERSION_STRING}")
	set (GS_NVIDIAGPUCOMPUTINGSDK_INC "${GS_NVIDIAGPUCOMPUTINGSDK_RELEASE}/common/inc")
else ()
endif ()
message (STATUS "GigaSpace requires NVIDIA GPU Computing SDK. Check if include following directory is right.")
message (STATUS "NVIDIA GPU Computing SDK INC = ${GV_NVIDIAGPUCOMPUTINGSDK_INC}")
MESSAGE ( STATUS "" )

##################################################################################
# Include directories
##################################################################################

# GigaSpace SDK
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

# glad
set( glad_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/glad/lib/cmake/glad" )
find_package( glad CONFIG REQUIRED )

# glm
set( glm_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/glm/lib/cmake/glm" )
find_package( glm CONFIG REQUIRED )
include_directories( ${GLM_INCLUDE_DIRS} )

# stb
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/stb" )

# glfw
set( glfw3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/glfw/lib/cmake/glfw3 )
find_package( glfw3 CONFIG REQUIRED )

# cudpp
set( cudpp_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/cudpp/lib )
find_package( cudpp CONFIG REQUIRED )
include_directories( ${cudpp_INCLUDE_DIRS} )

# cuda HELPER
include_directories( ${GS_NVIDIAGPUCOMPUTINGSDK_INC} )

# Loki
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Loki/include" )

# GigaSpace SDK
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../Install/GigaSpaceProject/include/GigaSpace" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../Install/GigaSpaceProject/include" )

##################################################################################
# Program
##################################################################################

# Retrieve source files

file( GLOB incList "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
file( GLOB inlList "${CMAKE_CURRENT_SOURCE_DIR}/*.inl" )
file( GLOB srcList "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
file( GLOB cuList "${CMAKE_CURRENT_SOURCE_DIR}/*.cu" )

# - shaders
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/*.vert" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/*.frag" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/*.geom" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/*.tese" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/*.tesc" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/*.comp" )

# Group files in IDE (Visual Studio)
source_group( "Include" FILES ${incList} )
source_group( "Inline" FILES ${inlList} )
source_group( "Source" FILES ${srcList} )
source_group( "Cuda" FILES ${cuList} )
# - shaders
source_group( "Shaders" FILES ${shaderList} )

# Target program
# - source files
set( resList ${resList} ${incList} )
set( resList ${resList} ${inlList} )
set( resList ${resList} ${shaderList} )

set( srcList ${srcList} ${cuList} )

# target
#add_library( ${PROJECT_NAME} SHARED ${srcList} ${resList} )
# add current GPU architecture at runtime
set( GENCODE ${CUDA_ARCH_FLAGS} )
cuda_add_executable( ${PROJECT_NAME} ${srcList} ${resList} OPTIONS ${GENCODE} )

#####################################################
set( CUSTOM_DEBUG_POSTFIX "d" )
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CUSTOM_DEBUG_POSTFIX} )
#####################################################

##################################################################################
# Linked libraries
##################################################################################

# Graphics
target_link_libraries( ${PROJECT_NAME} ${OPENGL_gl_LIBRARY} )

# GLFW
target_link_libraries( ${PROJECT_NAME} glfw )

# glad
target_link_libraries( ${PROJECT_NAME} glad::glad )

# dlopen/dlclose
target_link_libraries( ${PROJECT_NAME} ${CMAKE_DL_LIBS} )

# cudpp
target_link_libraries( ${PROJECT_NAME} cudpp )

# GigaSpace SDK
target_link_libraries( ${PROJECT_NAME} GigaSpace )
target_link_libraries( ${PROJECT_NAME} GsSpaceCore )
target_link_libraries( ${PROJECT_NAME} GsGraphics )
target_link_libraries( ${PROJECT_NAME} GsCompute )

# ImGui
set( ImGui_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/ImGui" )
#file( GLOB imguiIncList "${ImGui_DIR}/*.h" )
#file( GLOB imguiSrcList "${ImGui_DIR}/*.cpp" )
#file( GLOB addons1imguiIncList "${ImGui_DIR}/addons/imguifilesystem/*.h" )
#file( GLOB addons1imguiSrcList "${ImGui_DIR}/addons/imguifilesystem/*.cpp" )
#file( GLOB addons2imguiIncList "${ImGui_DIR}/addons/imguiyesaddons/imguitinyfiledialogs.h" )
#file( GLOB addons2imguiSrcList "${ImGui_DIR}/addons/imguiyesaddons/imguitinyfiledialogs.cpp" )
#add_library( "ImGui" ${imguiSrcList} ${imguiIncList} ${addons1imguiSrcList} ${addons1imguiIncList} ${addons2imguiSrcList} ${addons2imguiIncList} )
#target_include_directories( "ImGui" PRIVATE "${ImGui_DIR}" )
target_include_directories( ${PROJECT_NAME} PRIVATE "${ImGui_DIR}" )
#target_link_libraries( ${PROJECT_NAME} "ImGui" )
target_link_libraries( ${PROJECT_NAME} debug "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Externals/lib/ImGui${CUSTOM_DEBUG_POSTFIX}.lib" optimized "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Externals/lib/ImGui.lib" )

##################################################################################
# Project Properties
##################################################################################

# Project property
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11 )

##################################################################################
# Custom commands
##################################################################################

# Libraries
install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )

# Copy library
#add_custom_command(TARGET ${PROJECT_NAME}
#           POST_BUILD
#           #COMMAND ${CMAKE_COMMAND} -E echo "Copy DLL"		   
#		   COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/../PtViewer/$<CONFIG>
#)

# Copy library
#add_custom_command(TARGET ${PROJECT_NAME}
 #          POST_BUILD
  #         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/../x64
	#	   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_SOURCE_DIR}/x64/.
		  # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:${PROJECT_NAME}> ${GS_RELEASE_LIB_DIR}/.
		  # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${GS_RELEASE_BIN_DIR}/.
#)
