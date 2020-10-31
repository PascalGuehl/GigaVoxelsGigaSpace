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

#ifndef _GS_GRAPHICS_CORE_H_
#define _GS_GRAPHICS_CORE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GsGraphics/GsGraphicsCoreConfig.h"

// OpenGL
#include <GL/glew.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

//#define glDispatchComputeEXT GLEW_GET_FUN(__glewMemoryBarrierEXT)
//
//typedef void (GLAPIENTRY *PFNGLDISPATCHCOMPUTEEXTPROC)(GLuint num_groups_x,  GLuint num_groups_y,  GLuint num_groups_z);
//#define glDispatchComputeEXT GLEW_GET_FUN(__glewMemoryBarrierEXT)
//GLEW_FUN_EXPORT PFNGLMEMORYBARRIEREXTPROC __glewMemoryBarrierEXT;

/**
 * Compute shader
 */
//typedef void (GLAPIENTRY *PFNGLDISPATCHCOMPUTEEXTPROC)( GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z );
//PFNGLDISPATCHCOMPUTEEXTPROC glDispatchComputeEXT;

/**
 * Compute shader
 */
#ifndef GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#endif
#ifndef GL_MAX_COMPUTE_WORK_GROUP_COUNT
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#endif
#ifndef GL_MAX_COMPUTE_WORK_GROUP_SIZE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF
#endif

/**
 * OpenGL
 */
//typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
//PFNGLTEXSTORAGE2DPROC glTexStorage2D;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GsGraphics
{
	
/** 
 * @class GsGraphicsCore
 *
 * @brief The GsGraphicsCore class provides an interface for accessing OpenGL properties.
 *
 * @ingroup GsGraphics
 *
 * It holds OpenGL properties.
 */
class GSGRAPHICS_EXPORT GsGraphicsCore
{

	/**************************************************************************
	 ***************************** FRIEND SECTION *****************************
	 **************************************************************************/

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/
		
	/**
	 * Constructor
	 */
	GsGraphicsCore();

	/**
	 * Destructor
	 */
	~GsGraphicsCore();

	/**
	 * Print information about the device
	 */
	static void printInfo();
		
	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/
		
	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GsGraphics

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#endif // !_GS_GRAPHICS_CORE_H_

