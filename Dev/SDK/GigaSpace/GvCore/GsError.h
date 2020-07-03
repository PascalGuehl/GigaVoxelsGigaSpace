/*
 * GigaVoxels - GigaSpace
 *
 * Website: http://gigavoxels.inrialpes.fr/
 *
 * Contributors: GigaVoxels Team
 *
 * BSD 3-Clause License:
 *
 * Copyright (C) 2007-2015 INRIA - LJK (CNRS - Grenoble University), All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the organization nor the names  of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * @version 1.0
 */

#ifndef _GV_ERROR_H_
#define _GV_ERROR_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// System 
#include <cstdlib>
#include <cstdio>

// CUDA
#include <cuda_runtime.h>

// OpenGL
#include <glad/glad.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
 
// Windows
#ifdef WIN32
# include <windows.h>
#endif

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/**
 * Check for OpenGL errors
 *
 * @param pFile File in which errors are checked
 * @param pLine Line of file on which errors are checked
 *
 * @return Flag to say wheter or not there has been an error
 */
inline bool checkGLError( const char* pFile, const int pLine )
{
	// Check for error
	GLenum error = glGetError();
	if ( error != GL_NO_ERROR )
	{

// Windows specific stuff
#ifdef _WIN32
		char buf[ 512 ];
		//sprintf( buf, "\n%s(%i) : GL Error : %s\n\n", pFile, pLine, gluErrorString( error ) );
		OutputDebugStringA( buf );
#endif

		fprintf( stderr, "GL Error in file '%s' in line %d :\n", pFile, pLine );
		//fprintf( stderr, "%s\n", gluErrorString( error ) );

		return false;
	}

	return true;
}

} // namespace GvCore

/******************************************************************************
 ****************************** MACRO DEFINITION ******************************
 ******************************************************************************/

/**
 * MACRO
 * 
 * Call a Cuda method in a safe way (by checking error)
 */
#define GS_CUDA_SAFE_CALL( call )													\
{																					\
    cudaError_t error = call;														\
    if ( cudaSuccess != error )														\
	{																				\
		/* Write error info */														\
		fprintf( stderr, "\nCuda error :\n\t- file : '%s' \n\t- line %i : %s",		\
				__FILE__, __LINE__, cudaGetErrorString( error ) );					\
																					\
		/* Exit program */															\
        exit( EXIT_FAILURE );														\
    }																				\
}

// TO DO : add a flag to Release mode to optimize code => no check
/**
 * MACRO
 * 
 * Check for CUDA error
 */
#ifdef _DEBUG

// Debug mode version
#define GV_CHECK_CUDA_ERROR( pText )												\
{																					\
	/* Check for error */															\
	cudaError_t error = cudaGetLastError();											\
	if ( cudaSuccess != error )														\
	{																				\
		/* Write error info */														\
		fprintf( stderr, "\nCuda error : %s \n\t- file : '%s' \n\t- line %i : %s",	\
				pText, __FILE__, __LINE__, cudaGetErrorString( error ) );			\
																					\
		/* Exit program */															\
		exit( EXIT_FAILURE );														\
	}																				\
																					\
	/* Blocks until the device has completed all preceding requested tasks */		\
	error = cudaDeviceSynchronize();												\
	if ( cudaSuccess != error )														\
	{																				\
		fprintf( stderr, "Cuda error : %s in file '%s' in line %i : %s.\n",			\
				pText, __FILE__, __LINE__, cudaGetErrorString( error ) );			\
																					\
		/* Exit program */															\
		exit( EXIT_FAILURE );														\
	}																				\
}

#else

// TO DO : optimize code for Release/Distribution => don't call cudaGetLastError()

// Release mode version
#define GV_CHECK_CUDA_ERROR( pText )												\
{																					\
	/* Check for error */															\
	cudaError_t error = cudaGetLastError();											\
	if ( cudaSuccess != error )														\
	{																				\
		/* Write error info */														\
		fprintf( stderr, "\nCuda error : %s \n\t- file : '%s' \n\t- line %i : %s",	\
				pText, __FILE__, __LINE__, cudaGetErrorString( error ) );			\
																					\
		/* Exit program */															\
		exit( EXIT_FAILURE );														\
	}																				\
}

#endif

/******************************************************************************
 ****************************** MACRO DEFINITION ******************************
 ******************************************************************************/

/**
 * MACRO
 *
 * Check for OpenGL errors
 */
#ifdef NDEBUG

	// Release mode version
	#define GV_CHECK_GL_ERROR()

#else

	// Debug mode version
	#define GV_CHECK_GL_ERROR()														\
	if ( ! GvCore::checkGLError( __FILE__, __LINE__ ) )								\
	{																				\
		/* Exit program */															\
		exit( EXIT_FAILURE );														\
	}

#endif

#endif // !_GV_ERROR_H_
