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

#ifndef _GV_PROXY_GEOMETRY_HANDLER_H_
#define _GV_PROXY_GEOMETRY_HANDLER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// OpenGL
#include <GL/glew.h>

// CUDA
#include <driver_types.h>

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

namespace GvUtils
{

/** 
 * @class GsProxyGeometryHandler
 *
 * @brief The GsProxyGeometryHandler class provides interface to proxy geometry
 *
 * Proxy geometries are used to provided an approximation or a lower resolution
 * of a model.
 */
class GIGASPACE_EXPORT GsProxyGeometryHandler
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * CUDA graphics resource
	 */
	struct cudaGraphicsResource* _d_vertices;	// TEST : à remettre en protected

	/**
	 *
	 */
	unsigned int _nbPoints;

	/**
	 * Vertex array
	 */
	GLuint _vertexBuffer;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsProxyGeometryHandler();

	/**
	 * Destructor
	 */
	 virtual ~GsProxyGeometryHandler();

	 /**
	  * Initialize
	  *
	  * @return a flag to tell wheter or not it succeeds.
	  */
	 bool initialize();

	  /**
	  * Finalize
	  *
	  * @return a flag to tell wheter or not it succeeds.
	  */
	 bool finalize();

	 /**
	  * Render
	  */
	 void render();

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	///**
	// * Vertex array
	// */
	//GLuint _vertexBuffer;

	/**
	 * Index array
	 */
	GLuint _indexBuffer;

	///**
	// * CUDA graphics resource
	// */
	//struct cudaGraphicsResource* _d_vertices;

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

//#include "GsProxyGeometryHandler.inl"

#endif
