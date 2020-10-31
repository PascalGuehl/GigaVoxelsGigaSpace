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

#ifndef _GV_RENDERER_GLSL_H_
#define _GV_RENDERER_GLSL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvRendering/GsRenderer.h"

// OpenGL
#include <GL/glew.h>

// Cuda
#include <vector_types.h>

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

namespace GvRendering
{

/** 
 * @class GsRendererGLSL
 *
 * @brief The GsRendererGLSL class provides an implementation of a renderer
 * specialized for GLSL.
 *
 * It implements the renderImpl() method from GsRenderer::GsIRenderer base class
 * and has access to the data structure, cache and producer through the inherited
 * VolumeTreeRenderer base class.
 */
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
class GsRendererGLSL : public GvRendering::GsRenderer< TVolumeTreeType, TVolumeTreeCacheType >
{

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
	GsRendererGLSL();

	/**
	 * Destructor
	 */
	virtual ~GsRendererGLSL();

	/**
	 * Initialize
	 *
	 * @param pDataStructure data structure
	 * @param pDataProductionManager data production manager
	 */
	virtual void initialize( TDataStructure* pDataStructure, TDataProductionManager* pDataProductionManager );

	/**
	 * Finalize
	 */
	virtual void finalize();

	/**
	 * This function is the specific implementation method called
	 * by the parent GsIRenderer::render() method during rendering.
	 *
	 * @param pModelMatrix the current model matrix
	 * @param pViewMatrix the current view matrix
	 * @param pProjectionMatrix the current projection matrix
	 * @param pViewport the viewport configuration
	 */
	virtual void render( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );
	//void renderGL( const float4x4& pModelViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport);

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

	/**
	 * GLGL shader program
	 */
	GLuint _rayCastProg;

	/**
	 * Requests buffers
	 */
	GLuint _updateBufferTBO;

	/**
	 * Nodes timetamp buffer
	 */
	GLuint _nodeTimeStampTBO;

	/**
	 * Bricks timetamp buffer
	 */
	GLuint _brickTimeStampTBO;

	/**
	 * Nodes address buffer
	 */
	GLuint _childArrayTBO;

	/**
	 * Data address buffer
	 */
	GLuint _dataArrayTBO;

	/**
	 * ...
	 */
	GLuint _textBuffer;

	/**
	 * ...
	 */
	GLuint _textBufferTBO;

	/******************************** METHODS *********************************/

	/**
	 * Start the rendering process.
	 *
	 * @param pModelMatrix the current model matrix
	 * @param pViewMatrix the current view matrix
	 * @param pProjectionMatrix the current projection matrix
	 * @param pViewport the viewport configuration
	 */
	virtual void doRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );
	//void render( const uint2& fs, const float4x4& modelViewMatrix, const float4x4& projectionMatrix );

	/**
	 * Copy constructor forbidden.
	 */
	GsRendererGLSL( const GsRendererGLSL& );

	/**
	 * Copy operator forbidden.
	 */
	GsRendererGLSL& operator=( const GsRendererGLSL& );

};

} // namespace GvRendering

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsRendererGLSL.inl"

#endif // !_GV_RENDERER_GLSL_H_
