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

#ifndef _GV_RENDERER_CONTEXT_H_
#define _GV_RENDERER_CONTEXT_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvRendering/GsGraphicsInteroperabiltyHandler.h"
#include "GvRendering/GsGraphicsResource.h"

// Cuda
#include <host_defines.h>

// Cutil
//#include <helper_math.h>

// GigaVoxels
#include "GvCore/GsLinearMemoryKernel.h"
#include "GvCore/GsVectorTypesExt.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * Opacity step
 */
#define cOpacityStep 0.99f

/**
 * Alpha threshold for depth output
 * - used to choose when to write depth in output buffer
 *
 * TODO : pass by RayCaster template parameter
 */
#define cOpacityDepthThreshold 0.1f

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
 * @struct GsRendererContext
 *
 * @brief The GsRendererContext struct provides access to useful variables
 * from rendering context (view matrix, model matrix, etc...)
 *
 * TO DO : analyse memory alignement of data in this structure (ex : float3).
 *
 * @ingroup GsRenderer
 */
struct GsRendererContext
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/******************************* ATTRIBUTES *******************************/

	// TO DO
	// [ 1 ]
	// - Maybe store all constants that could changed in a frame in this monolythic structure to avoid multiple copy of constant at each frame
	// - cause actually, many graphics cards have only one copy engine, so it will be serialized.
	// [ 2 ]
	// - check the data alignment : maybe using float3 or single 32-bits, could misaligned memory access pattern, I forgot ?
	
	/**
	 * View matrix
	 */
	float4x4 viewMatrix;

	/**
	 * Inverted view matrix
	 */
	float4x4 invViewMatrix;

	/**
	 * Model matrix
	 */
	float4x4 modelMatrix;

	/**
	 * Inverted model matrix
	 */
	float4x4 invModelMatrix;

	/**
	 * Distance to the near depth clipping plane
	 */
	float frustumNear;

	/**
	 * Distance to the near depth clipping plane
	 */
	float frustumNearINV;

	/**
	 * Distance to the far depth clipping plane
	 */
	float frustumFar;
	
	/**
	 * Specify the coordinate for the right vertical clipping plane 
	 */
	float frustumRight;

	/**
	 * Specify the coordinate for the top horizontal clipping plane
	 */
	float frustumTop;

	float frustumC; //cf: http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml
	float frustumD;

	/**
	 * Pixel size
	 */
	float2 pixelSize;

	/**
	 * Frame size (viewport dimension)
	 */
	uint2 frameSize;

	/**
	 * Camera position (in world coordinate system)
	 */
	float3 viewCenterWP;
	// TEST
	float3 viewCenterTP;

	/**
	 * Camera's vector from eye to [left, bottom, -near] clip plane position
	 * (in world coordinate system)
	 *
	 * This vector is used during ray casting as base direction
	 * from which camera to pixel ray directions are computed.
	 */
	float3 viewPlaneDirWP;
	// TEST
	float3 viewPlaneDirTP;

	/**
	 * Camera's X axis (in world coordinate system)
	 */
	float3 viewPlaneXAxisWP;
	// TEST
	float3 viewPlaneXAxisTP;

	/**
	 * Camera's Y axis (in world coordinate system)
	 */
	float3 viewPlaneYAxisWP;
	// TEST
	float3 viewPlaneYAxisTP;
	
	/**
	 * Projected 2D Bounding Box of the GigaVoxels 3D BBox.
	 * It holds its bottom left corner and its size.
	 * ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
	 */
	uint4 _projectedBBox;
	
	/**
	 * Color and depth graphics resources
	 */
	void* _graphicsResources[ GsGraphicsInteroperabiltyHandler::eNbGraphicsResourceDeviceSlots ];
	GsGraphicsResource::MappedAddressType _graphicsResourceAccess[ GsGraphicsInteroperabiltyHandler::eNbGraphicsResourceDeviceSlots ];
	unsigned int _inputColorTextureOffset;
	unsigned int _inputDepthTextureOffset;

	// TO DO : à deplacer en dehors du context ?
	/**
	 * Specify clear values for the color buffers
	 */
	uchar4 _clearColor;

	// TO DO : à deplacer en dehors du context ?
	/**
	 * Specify the clear value for the depth buffer
	 */
	float _clearDepth;

	/******************************** METHODS *********************************/

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GvRendering

/**************************************************************************
 **************************** CONSTANTS SECTION ***************************
 **************************************************************************/

/**
 * Render view context
 *
 * It provides access to useful variables from rendering context (view matrix, model matrix, etc...)
 * As a CUDA constant, all values will be available in KERNEL and DEVICE code.
 */
__constant__ GvRendering::GsRendererContext k_renderViewContext;

/**
 * Max volume tree depth
 */
__constant__ uint k_maxVolTreeDepth;

/**
 * Current time
 */
__constant__ uint k_currentTime;

namespace GvRendering
{

	/**
	 * Voxel size multiplier
	 */
	__constant__ float k_voxelSizeMultiplier;

}

#endif
