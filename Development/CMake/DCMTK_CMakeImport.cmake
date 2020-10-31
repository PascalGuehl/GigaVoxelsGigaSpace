#----------------------------------------------------------------
# Import library
#----------------------------------------------------------------

MESSAGE (STATUS "IMPORT : DCMTK library")

#----------------------------------------------------------------
# SET library PATH
#----------------------------------------------------------------

INCLUDE (GvSettings_CMakeImport)
	
#----------------------------------------------------------------
# Add INCLUDE library directories
#----------------------------------------------------------------

INCLUDE_DIRECTORIES (${GV_DCMTK_INC})

#----------------------------------------------------------------
# Add LINK library directories
#----------------------------------------------------------------

LINK_DIRECTORIES (${GV_DCMTK_LIB})

#----------------------------------------------------------------
# Set LINK libraries if not defined by user
#----------------------------------------------------------------

IF ( "${dcmtkLib}" STREQUAL "" )
	IF (WIN32)
		IF ( ${GV_DESTINATION_ARCH} STREQUAL "x86" )
#			SET (dcmtkLib "charls" "dcmdata" "dcmdsig" "dcmimage" "dcmimgle" "dcmjpeg" "dcmjpls" "dcmnet" "dcmpstat" "dcmqrdb" "dcmsr" "dcmtls" "dcmwlm" "ijg8" "ijg12" "ijg16" "libi2d" "oflog" "ofstd")
			#SET (dcmtkLib "dcmimage")
			#SET (dcmtkLib "ofstd" "oflog" "dcmdata" "dcmimgle" "dcmimage" "dcmjpeg")
			SET (dcmtkLib "ofstd" "dcmdata" "dcmimgle" "dcmimage" "ijg8" "ijg12" "ijg16" "libi2d" "dcmjpeg" "dcmnet" "dcmdsig" "dcmsr" "dcmpstat" "dcmtls" "dcmwlm" "dcmjpls" "dcmqrdb" "oflog" "charls")
		ELSE ()
#			SET (dcmtkLib "charls" "dcmdata" "dcmdsig" "dcmimage" "dcmimgle" "dcmjpeg" "dcmjpls" "dcmnet" "dcmpstat" "dcmqrdb" "dcmsr" "dcmtls" "dcmwlm" "ijg8" "ijg12" "ijg16" "libi2d" "oflog" "ofstd")
			#SET (dcmtkLib "dcmimage")
			#SET (dcmtkLib "ofstd" "oflog" "dcmdata" "dcmimgle" "dcmimage" "dcmjpeg")
			SET (dcmtkLib "ofstd" "dcmdata" "dcmimgle" "dcmimage" "ijg8" "ijg12" "ijg16" "libi2d" "dcmjpeg" "dcmnet" "dcmdsig" "dcmsr" "dcmpstat" "dcmtls" "dcmwlm" "dcmjpls" "dcmqrdb" "oflog" "charls")
		ENDIF ()
	ELSE ()
		IF ( ${GV_DESTINATION_ARCH} STREQUAL "x86" )
			#			SET (dcmtkLib "charls" "dcmdata" "dcmdsig" "dcmimage" "dcmimgle" "dcmjpeg" "dcmjpls" "dcmnet" "dcmpstat" "dcmqrdb" "dcmsr" "dcmtls" "dcmwlm" "ijg8" "ijg12" "ijg16" "libi2d" "oflog" "ofstd")
			#SET (dcmtkLib "dcmimage")
			#SET (dcmtkLib "ofstd" "oflog" "dcmdata" "dcmimgle" "dcmimage" "dcmjpeg")
			SET (dcmtkLib "ofstd" "dcmdata" "dcmimgle" "dcmimage" "ijg8" "ijg12" "ijg16" "libi2d" "dcmjpeg" "dcmnet" "dcmdsig" "dcmsr" "dcmpstat" "dcmtls" "dcmwlm" "dcmjpls" "dcmqrdb" "oflog" "charls")
		ELSE ()
			#			SET (dcmtkLib "charls" "dcmdata" "dcmdsig" "dcmimage" "dcmimgle" "dcmjpeg" "dcmjpls" "dcmnet" "dcmpstat" "dcmqrdb" "dcmsr" "dcmtls" "dcmwlm" "ijg8" "ijg12" "ijg16" "libi2d" "oflog" "ofstd")
			#SET (dcmtkLib "dcmimage")
			#SET (dcmtkLib "ofstd" "oflog" "dcmdata" "dcmimgle" "dcmimage" "dcmjpeg")
			SET (dcmtkLib "ofstd" "dcmdata" "dcmimgle" "dcmimage" "ijg8" "ijg12" "ijg16" "dcmjpeg" "dcmnet" "dcmdsig" "dcmsr" "dcmpstat" "dcmtls" "dcmwlm" "dcmjpls" "dcmqrdb" "oflog")
		ENDIF ()
	ENDIF ()
ENDIF ()

#----------------------------------------------------------------
# Add LINK libraries
#----------------------------------------------------------------

FOREACH (it ${dcmtkLib})
	IF (WIN32)
		LINK_LIBRARIES (optimized ${it} debug ${it}.d)
	ELSE ()
		LINK_LIBRARIES (optimized ${it} debug ${it})
	ENDIF ()
ENDFOREACH (it)
