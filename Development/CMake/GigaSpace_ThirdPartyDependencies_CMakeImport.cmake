#----------------------------------------------------------------
# Import library
#----------------------------------------------------------------
	
MESSAGE (STATUS "IMPORT : GigaSpace third party dependencies")

#----------------------------------------------------------------
# SET common libraries PATH
#----------------------------------------------------------------

# Main GigaSpace paths and settings
INCLUDE (GvSettings_CMakeImport)
	
#----------------------------------------------------------------
# For each 3rd party dependency:
# - add INCLUDE library directories
# - add LINK library directories
# - add LINK libraries
#----------------------------------------------------------------

# Add GigaSpace third party dependencies
# - template and design pattern
INCLUDE (Loki_CMakeImport)
# - graphics
INCLUDE (OpenGL_CMakeImport)
INCLUDE (GLU_CMakeImport)
INCLUDE (glew_CMakeImport)
# - to be removed in the futur... (text rendering)
INCLUDE (freeglut_CMakeImport)
# - to be removed in the futur... (data types, maths and utils)
INCLUDE (GPU_COMPUTING_SDK_CMakeImport)
# - data parallel computing
INCLUDE (CUDPP_CMakeImport)
# - XML library
INCLUDE (TinyXML_CMakeImport)
# - matrix/vector library
INCLUDE (GLM_CMakeImport)

# Linux special features
if (WIN32)
else ()
	INCLUDE (dl_CMakeImport)
	INCLUDE (rt_CMakeImport)
endif()
