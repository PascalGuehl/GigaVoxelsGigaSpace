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
project( GigaSpace )

# Set parallel build mode
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP" )

# Remove some compilation warnings
add_definitions( -D_CRT_SECURE_NO_WARNINGS ) 

# Shared library
add_definitions( -DGIGASPACE_MAKEDLL )

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
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../SDK/GigaSpace" )

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
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../Install/GigaSpaceProject/include" )

# tinyxml
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/tinyxml/include" )

##################################################################################
# Program
##################################################################################

# Retrieve source files

file( GLOB incList "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
file( GLOB inlList "${CMAKE_CURRENT_SOURCE_DIR}/*.inl" )
file( GLOB srcList "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
file( GLOB cuList "${CMAKE_CURRENT_SOURCE_DIR}/*.cu" )

file( GLOB core_incList "${CMAKE_CURRENT_SOURCE_DIR}/GvCore/*.h" )
file( GLOB core_inlList "${CMAKE_CURRENT_SOURCE_DIR}/GvCore/*.inl" )
file( GLOB core_srcList "${CMAKE_CURRENT_SOURCE_DIR}/GvCore/*.cpp" )
file( GLOB core_cuList "${CMAKE_CURRENT_SOURCE_DIR}/GvCore/*.cu" )

file( GLOB cache_incList "${CMAKE_CURRENT_SOURCE_DIR}/GvCache/*.h" )
file( GLOB cache_inlList "${CMAKE_CURRENT_SOURCE_DIR}/GvCache/*.inl" )
file( GLOB cache_srcList "${CMAKE_CURRENT_SOURCE_DIR}/GvCache/*.cpp" )
file( GLOB cache_cuList "${CMAKE_CURRENT_SOURCE_DIR}/GvCache/*.cu" )

file( GLOB structure_incList "${CMAKE_CURRENT_SOURCE_DIR}/GvStructure/*.h" )
file( GLOB structure_inlList "${CMAKE_CURRENT_SOURCE_DIR}/GvStructure/*.inl" )
file( GLOB structure_srcList "${CMAKE_CURRENT_SOURCE_DIR}/GvStructure/*.cpp" )
file( GLOB structure_cuList "${CMAKE_CURRENT_SOURCE_DIR}/GvStructure/*.cu" )

file( GLOB utils_incList "${CMAKE_CURRENT_SOURCE_DIR}/GvUtils/*.h" )
file( GLOB utils_inlList "${CMAKE_CURRENT_SOURCE_DIR}/GvUtils/*.inl" )
file( GLOB utils_srcList "${CMAKE_CURRENT_SOURCE_DIR}/GvUtils/*.cpp" )
file( GLOB utils_cuList "${CMAKE_CURRENT_SOURCE_DIR}/GvUtils/*.cu" )

file( GLOB rendering_incList "${CMAKE_CURRENT_SOURCE_DIR}/GvRendering/*.h" )
file( GLOB rendering_inlList "${CMAKE_CURRENT_SOURCE_DIR}/GvRendering/*.inl" )
file( GLOB rendering_srcList "${CMAKE_CURRENT_SOURCE_DIR}/GvRendering/*.cpp" )
file( GLOB rendering_cuList "${CMAKE_CURRENT_SOURCE_DIR}/GvRendering/*.cu" )

file( GLOB performance_incList "${CMAKE_CURRENT_SOURCE_DIR}/GvPerfMon/*.h" )
file( GLOB performance_inlList "${CMAKE_CURRENT_SOURCE_DIR}/GvPerfMon/*.inl" )
file( GLOB performance_srcList "${CMAKE_CURRENT_SOURCE_DIR}/GvPerfMon/*.cpp" )
file( GLOB performance_cuList "${CMAKE_CURRENT_SOURCE_DIR}/GvPerfMon/*.cu" )

# - shaders
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.vert" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.frag" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.geom" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.tese" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.tesc" )
file( GLOB shaderList "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.comp" )

# Group files in IDE (Visual Studio)
source_group( "Include" FILES ${incList} ${core_incList} ${cache_incList} ${structure_incList} ${utils_incList} ${rendering_incList} ${performance_incList} )
source_group( "Inline" FILES ${inlList} ${core_inlList} ${cache_inlList} ${structure_inlList} ${utils_inlList} ${rendering_inlList} ${performance_inlList} )
source_group( "Source" FILES ${srcList} ${core_srcList} ${cache_srcList} ${structure_srcList} ${utils_srcList} ${rendering_srcList} ${performance_srcList} )
source_group( "Cuda" FILES ${cuList} ${core_cuList} ${cache_cuList} ${structure_cuList} ${utils_cuList} ${rendering_cuList} ${performance_cuList} )
# - shaders
source_group( "Shaders" FILES ${shaderList} )

# Target program
# - source files
set( resList ${resList} ${incList} )
set( resList ${resList} ${inlList} )
set( resList ${resList} ${shaderList} )

set( core_resList ${core_resList} ${core_incList} )
set( core_resList ${core_resList} ${core_inlList} )
set( core_resList ${core_resList} ${core_shaderList} )

set( cache_resList ${cache_resList} ${cache_incList} )
set( cache_resList ${cache_resList} ${cache_inlList} )
set( cache_resList ${cache_resList} ${cache_shaderList} )

set( structure_resList ${structure_resList} ${structure_incList} )
set( structure_resList ${structure_resList} ${structure_inlList} )
set( structure_resList ${structure_resList} ${structure_shaderList} )

set( utils_resList ${utils_resList} ${utils_incList} )
set( utils_resList ${utils_resList} ${utils_inlList} )
set( utils_resList ${utils_resList} ${utils_shaderList} )

set( rendering_resList ${rendering_resList} ${rendering_incList} )
set( rendering_resList ${rendering_resList} ${rendering_inlList} )
set( rendering_resList ${rendering_resList} ${rendering_shaderList} )

set( performance_resList ${performance_resList} ${performance_incList} )
set( performance_resList ${performance_resList} ${performance_inlList} )
set( performance_resList ${performance_resList} ${performance_shaderList} )

# target
#add_library( ${PROJECT_NAME} SHARED ${srcList} ${resList} )
# add current GPU architecture at runtime
set( GENCODE ${CUDA_ARCH_FLAGS} )
cuda_add_library( ${PROJECT_NAME} SHARED ${srcList} ${core_srcList} ${cache_srcList} ${structure_srcList} ${utils_srcList} ${rendering_srcList} ${performance_srcList} ${cuList} ${core_cuList} ${cache_cuList} ${structure_cuList} ${utils_cuList} ${rendering_cuList} ${performance_cuList} ${resList} ${core_resList} ${cache_resList} ${structure_resList} ${utils_resList} ${rendering_resList} ${performance_resList} OPTIONS ${GENCODE} )

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
target_link_libraries( ${PROJECT_NAME} GsGraphics )
target_link_libraries( ${PROJECT_NAME} GsCompute )
target_link_libraries( ${PROJECT_NAME} GsSpaceCore )

# tinyxml
#target_link_libraries( ${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/tinyxml/lib/tinyxml" )
#find_library( tinyxml_LIBRARY_DEBUG
#    NAMES tinyxml{CUSTOM_DEBUG_POSTFIX}
#    HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/tinyxml/lib"
#)
#find_library( tinyxml_LIBRARY_RELEASE
#    NAMES tinyxml
#    HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/tinyxml/lib"
#)
#target_link_libraries( ${PROJECT_NAME} debug ${tinyxml_LIBRARY_DEBUG} optimized ${tinyxml_LIBRARY_RELEASE} )
target_link_libraries( ${PROJECT_NAME} debug "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/tinyxml/lib/tinyxml${CUSTOM_DEBUG_POSTFIX}.lib" optimized "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/tinyxml/lib/tinyxml.lib" )

# Project property
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11 )

##################################################################################
# Custom commands
##################################################################################

# Libraries
install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )
install( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib )

install( FILES ${resList} DESTINATION include/${PROJECT_NAME} )
install( FILES ${core_resList} DESTINATION include/${PROJECT_NAME}/GvCore )
install( FILES ${cache_resList} DESTINATION include/${PROJECT_NAME}/GvCache )
install( FILES ${structure_resList} DESTINATION include/${PROJECT_NAME}/GvStructure )
install( FILES ${utils_resList} DESTINATION include/${PROJECT_NAME}/GvUtils )
install( FILES ${rendering_resList} DESTINATION include/${PROJECT_NAME}/GvRendering )
install( FILES ${performance_resList} DESTINATION include/${PROJECT_NAME}/GvPerfMon )

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
