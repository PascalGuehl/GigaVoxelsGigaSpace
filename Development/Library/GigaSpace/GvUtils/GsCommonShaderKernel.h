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

#ifndef _GV_COMMON_SHADER_KERNEL_H_
#define _GV_COMMON_SHADER_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvRendering/GsRendererContext.h"

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

namespace GvUtils
{

/** 
 * @struct GsCommonShaderKernel
 *
 * @brief The GsCommonShaderKernel struct provides the way to shade the data structure.
 *
 * It is used in conjonction with the base class GsIRenderShader to implement the shader functions.
 */
class GsCommonShaderKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * This method is called just before the cast of a ray. Use it to initialize any data
	 *  you may need. You may also want to modify the initial distance along the ray (tTree).
	 *
	 * @param pRayStartTree the starting position of the ray in octree's space.
	 * @param pRayDirTree the direction of the ray in octree's space.
	 * @param pTTree the distance along the ray's direction we start from.
	 */
	__device__
	__forceinline__ void preShadeImpl( const float3& pRayStartTree, const float3& pRayDirTree, float& pTTree );

	/**
	 * This method is called after the ray stopped or left the bounding
	 * volume. You may want to do some post-treatment of the color.
	 */
	__device__
	__forceinline__ void postShadeImpl();

	/**
	 * This method returns the cone aperture for a given distance.
	 *
	 * @param pTTree the current distance along the ray's direction.
	 *
	 * @return the cone aperture
	 */
	__device__
	__forceinline__ float getConeApertureImpl( const float pTTree ) const;

	/**
	 * This method returns the final rgba color that will be written to the color buffer.
	 *
	 * @return the final rgba color.
	 */
	__device__
	__forceinline__ float4 getColorImpl() const;

	/**
	 * This method is called before each sampling to check whether or not the ray should stop.
	 *
	 * @param pRayPosInWorld the current ray's position in world space.
	 *
	 * @return true if you want to continue the ray. false otherwise.
	 */
	__device__
	__forceinline__ bool stopCriterionImpl( const float3& pRayPosInWorld ) const;

	/**
	 * This method is called to know if we should stop at the current octree's level.
	 *
	 * @param pVoxelSize the voxel's size in the current octree level.
	 *
	 * @return false if you want to stop at the current octree's level. true otherwise.
	 */
	__device__
	__forceinline__ bool descentCriterionImpl( const float pVoxelSize ) const;

	/**
	 * This method is called for each sample. For example, shading or secondary rays
	 * should be done here.
	 *
	 * @param pBrickSampler brick sampler
	 * @param pSamplePosScene position of the sample in the scene
	 * @param pRayDir ray direction
	 * @param pRayStep ray step
	 * @param pConeAperture cone aperture
	 */
	template< typename TSamplerType >
	__device__
	__forceinline__ void runImpl( const TSamplerType& pBrickSampler, const float3 pSamplePosScene,
						const float3 pRayDir, float& pRayStep, const float pConeAperture );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Accumulated color during ray casting
	 */
	float4 _accColor;

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsCommonShaderKernel.inl"

#endif // !_GV_COMMON_SHADER_KERNEL_H_
