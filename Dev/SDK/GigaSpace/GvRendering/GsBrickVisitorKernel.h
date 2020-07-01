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

#ifndef _GV_BRICK_VISITOR_KERNEL_H_
#define _GV_BRICK_VISITOR_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvRendering/GsSamplerKernel.h"

// Cuda
#include <host_defines.h>
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
 * @class GsBrickVisitorKernel
 *
 * @brief The GsBrickVisitorKernel class provides ...
 *
 * @ingroup GvRendering
 *
 * ...
 */
class GsBrickVisitorKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * DEVICE function
	 *
	 * This is the function where shading is done (ray marching along ray and data sampling).
	 * Shading is done with cone-tracing (LOD is selected by comparing cone aperture versus voxel size).
	 *
	 * @param pVolumeTree data structure
	 * @param pSampleShader shader 
	 * @param pGpuCache cache (not used for the moment...)
	 * @param pRayStartTree camera position in Tree coordinate system
	 * @param pRayDirTree ray direction in Tree coordinate system
	 * @param pTTree the distance from the eye to current position along the ray
	 * @param pRayLengthInNodeTree the distance along the ray from start to end of the brick, according to ray direction
	 * @param pBrickSampler The object in charge of sampling data (i.e. texture fetches)
	 * @param pModifInfoWriten (not used for the moment...)
	 *
	 * @return the distance where ray-marching has stopped
	 */
	template< bool TFastUpdateMode, bool TPriorityOnBrick, class TVolumeTreeKernelType, class TSampleShaderType, class TGPUCacheType >
	__device__
	static float visit( TVolumeTreeKernelType& pVolumeTree, TSampleShaderType& pSampleShader,
						TGPUCacheType& pGpuCache, const float3 pRayStartTree, const float3 pRayDirTree, const float pTTree,
						const float pRayLengthInNodeTree, GsSamplerKernel< TVolumeTreeKernelType >& pBrickSampler, bool& pModifInfoWriten );

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

} // namespace GvRendering

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsBrickVisitorKernel.inl"

#endif // !_GV_BRICK_VISITOR_KERNEL_H_
