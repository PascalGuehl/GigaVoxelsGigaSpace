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

#ifndef _GV_RENDERER_CUDA_H_
#define _GV_RENDERER_CUDA_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Cuda
#include <driver_types.h>

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsPool.h"
#include "GvCore/GsRendererTypes.h"
#include "GvCore/GsVector.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvStructure/GsVolumeTree.h"
#include "GvStructure/GsDataProductionManager.h"
#include "GvRendering/GsRenderer.h"
//#include "GvRendering/GsQualityRendererCUDAKernel.h"
#include "GvRendering/GsGraphicsInteroperabiltyHandler.h"
#include "GvRendering/GsGraphicsInteroperabiltyHandlerKernel.h"
#include "GvRendering/GsRendererContext.h"

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
 * @class GsQualityRendererCUDA
 *
 * @brief The GsQualityRendererCUDA class provides an implementation of a renderer
 * specialized for CUDA.
 *
 * That is the commun renderer that users may use.
 * It implements the renderImpl() method from GsRenderer::GsIRenderer base class
 * and has access to the data structure, cache and producer through the inherited
 * GsRenderer base class.
 */
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
class GsQualityRendererCUDA : public GsRenderer< TVolumeTreeType, TVolumeTreeCacheType >
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * CUDA block dimension used during rendering (kernel launch).
	 * Screen is split in 2D blocks of 8 per 8 pixels.
	 */
	typedef GvCore::GsVec3D< 8, 8, 1 > RenderBlockResolution;
	// TO DO : Profiling / Optimization
	// - try to play with differerent sizes of 2D kernel
	//typedef GvCore::GsVec3D< 16, 8, 1 > RenderBlockResolution;

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param pVolumeTree data structure to render
	 * @param pVolumeTreeCache cache
	 */
	GsQualityRendererCUDA( TVolumeTreeType* pVolumeTree, TVolumeTreeCacheType* pVolumeTreeCache );

	/**
	 * Destructor
	 */
	virtual ~GsQualityRendererCUDA();

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
	
	/**
	 * This function is the specific implementation method called
	 * by the parent GsIRenderer::render() method during rendering.
	 *
	 * @param pModelMatrix the current model matrix
	 * @param pViewMatrix the current view matrix
	 * @param pProjectionMatrix the current projection matrix
	 * @param pViewport the viewport configuration
	 */
	virtual void preRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );
	
	/**
	 * This function is the specific implementation method called
	 * by the parent GsIRenderer::render() method during rendering.
	 *
	 * @param pModelMatrix the current model matrix
	 * @param pViewMatrix the current view matrix
	 * @param pProjectionMatrix the current projection matrix
	 * @param pViewport the viewport configuration
	 */
	virtual void postRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );
	
	/**
	 * Attach an OpenGL buffer object (i.e. a PBO, a VBO, etc...) to an internal graphics resource 
	 * that will be mapped to a color or depth slot used during rendering.
	 *
	 * @param pGraphicsResourceSlot the associated internal graphics resource (color or depth) and its type of access (read, write or read/write)
	 * @param pBuffer the OpenGL buffer
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool connect( GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot pGraphicsResourceSlot, GLuint pBuffer );

	/**
	 * Attach an OpenGL texture or renderbuffer object to an internal graphics resource 
	 * that will be mapped to a color or depth slot used during rendering.
	 *
	 * @param pGraphicsResourceSlot the associated internal graphics resource (color or depth) and its type of access (read, write or read/write)
	 * @param pImage the OpenGL texture or renderbuffer object
	 * @param pTarget the target of the OpenGL texture or renderbuffer object
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool connect( GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot pGraphicsResourceSlot, GLuint pImage, GLenum pTarget );

	/**
	 * Dettach an OpenGL buffer object (i.e. a PBO, a VBO, etc...), texture or renderbuffer object
	 * to its associated internal graphics resource mapped to a color or depth slot used during rendering.
	 *
	 * @param pGraphicsResourceSlot the internal graphics resource slot (color or depth)
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool disconnect( GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot pGraphicsResourceSlot );

	/**
	 * Disconnect all registered graphics resources
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool resetGraphicsResources();

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Frame size
	 */
	uint2 _frameSize;

	/**
	 * Fast build mode flag
	 */
	bool _fastBuildMode;

	/**
	 * ...
	 */
	uint _frameNumAfterUpdate;

	/**
	 * ...
	 */
	uint _numUpdateFrames;	// >= 1

	/**
	 * Graphics interoperability handler
	 */
	GsGraphicsInteroperabiltyHandler* _graphicsInteroperabiltyHandler;

#ifdef _GS_RENDERER_USE_STREAM_
	/**
	 * Dedicated stream for Renderer
	 */
	cudaStream_t _rendererStream;
#endif

	/**
	 * Renderer context
	 * - to access to useful variables during rendering (view matrix, model matrix, etc...)
	 */
	GsRendererContext _viewContext;

	/**
	 * Current depth buffer
	 */
	GvCore::GsLinearMemory< float >* _d_rayDepthBuffer;

	/**
	 * Max depth buffer
	 */
	GvCore::GsLinearMemory< float >* _d_rayMaxDepthBuffer;

	/**
	 * Accumulated color along ray
	 */
	GvCore::GsLinearMemory< float4 >* _d_rayAccumulatedColorBuffer;

	/******************************** METHODS *********************************/

	/**
	 * Get the graphics interoperability handler
	 *
	 * @return the graphics interoperability handler
	 */
	GsGraphicsInteroperabiltyHandler* getGraphicsInteroperabiltyHandler();

	/**
	 * Initialize Cuda objects
	 */
	void initializeCuda();

	/**
	 * Finalize Cuda objects
	 */
	void finalizeCuda();

	/**
	 * Initialize internal buffers used during rendering
	 * (i.e. input/ouput color and depth buffers, ray buffers, etc...).
	 * Buffers size are dependent of the frame size.
	 *
	 * @param pFrameSize the frame size
	 */
	void initFrameObjects( const uint2& pFrameSize );

	/**
	 * Destroy internal buffers used during rendering
	 * (i.e. input/ouput color and depth buffers, ray buffers, etc...)
	 * Buffers size are dependent of the frame size.
	 */
	void deleteFrameObjects();

	/**
	 * Start the rendering process.
	 *
	 * @param pModelMatrix the current model matrix
	 * @param pViewMatrix the current view matrix
	 * @param pProjectionMatrix the current projection matrix
	 */
	virtual void doRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );

	/**
	 * Bind all graphics resources used by the GL interop handler during rendering.
	 *
	 * Internally, it binds textures and surfaces to arrays associated to mapped graphics reources.
	 *
	 * NOTE : this method should be in the GsGraphicsInteroperabiltyHandler but it seems that
	 * there are conflicts with textures ans surfaces symbols. The binding succeeds but not the
	 * read/write operations.
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool bindGraphicsResources();

	/**
	 * Unbind all graphics resources used by the GL interop handler during rendering.
	 *
	 * Internally, it unbinds textures and surfaces to arrays associated to mapped graphics reources.
	 *
	 * NOTE : this method should be in the GsGraphicsInteroperabiltyHandler but it seems that
	 * there are conflicts with textures ans surfaces symbols. The binding succeeds but not the
	 * read/write operations.
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool unbindGraphicsResources();

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsQualityRendererCUDA( const GsQualityRendererCUDA& );

	/**
	 * Copy operator forbidden.
	 */
	GsQualityRendererCUDA& operator=( const GsQualityRendererCUDA& );

};

} // namespace GvRendering

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsQualityRendererCUDA.inl"

#endif // _GV_RENDERER_CUDA_H_
