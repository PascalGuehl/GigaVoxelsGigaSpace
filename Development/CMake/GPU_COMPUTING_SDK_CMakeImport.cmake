#----------------------------------------------------------------
# Import library
#----------------------------------------------------------------
	
MESSAGE (STATUS "IMPORT : GPU Computing SDK library")

#----------------------------------------------------------------
# SET library PATH
#----------------------------------------------------------------

INCLUDE (GvSettings_CMakeImport)
	
#----------------------------------------------------------------
# Add INCLUDE library directories
#----------------------------------------------------------------

INCLUDE_DIRECTORIES (${GV_NVIDIAGPUCOMPUTINGSDK_INC})

#----------------------------------------------------------------
# Add LINK library directories
#----------------------------------------------------------------
	
#----------------------------------------------------------------
# Set LINK libraries if not defined by user
#----------------------------------------------------------------

#----------------------------------------------------------------
# Add LINK libraries
#----------------------------------------------------------------
