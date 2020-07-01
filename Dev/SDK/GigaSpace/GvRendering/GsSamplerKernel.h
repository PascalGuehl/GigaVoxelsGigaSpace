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

#ifndef _GV_SAMPLER_KERNEL_H_
#define _GV_SAMPLER_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

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
 * @struct GsSamplerKernel
 *
 * @brief The GsSamplerKernel struct provides features
 * to sample data in a data stucture.
 *
 * The rendering stage is done brick by brick along a ray.
 * The sampler is used to store useful current parameters needed to fecth data from data pool.
 *
 * @param VolumeTreeKernelType the data structure to sample data into.
 */
template< typename VolumeTreeKernelType >
struct GsSamplerKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Data structure.
	 * It is used to sample data into (the data pool is store inside the data structure)
	 */
	VolumeTreeKernelType* _volumeTree;

	/**
	 * Position of the brick in data pool (i.e. in 3D texture space)
	 */
	float3 _brickChildPosInPool;

	/**
	 * Position of the parent brick in data pool (i.e. in 3D texture space)
	 */
	float3 _brickParentPosInPool;
		
	/**
	 * Sample offset in the node
	 */
	float3 _sampleOffsetInNodeTree;
		
	/**
	 * Node/brick size
	 */
	float _nodeSizeTree;

	/**
	 * Ray length in node starting from sampleFirstOffsetInNodeTree
	 */
	float _rayLengthInNodeTree;	// not used anymore ?

	/**
	 * Flag telling wheter or not mipmapping is activated to render/visit the current brick
	 */
	bool _mipMapOn;

	/**
	 * If mipmapping is activated, this represents the coefficient to blend between child and parent brick
	 *
	 * note : 0.0f -> child, 1.0f -> parent
	 */
	float _mipMapInterpCoef;

	/**
	 * Coefficient used to transform/scale tree space to brick pool space
	 */
	float _scaleTree2BrickPool;

	/******************************** METHODS *********************************/

	/**
	 * Sample data at given cone aperture
	 *
	 * @param coneAperture the cone aperture
	 *
	 * @return the sampled value
	 */
	template< int channel >
	__device__
	__forceinline__ float4 getValue( const float coneAperture ) const;

	/**
	 * Sample data at given cone aperture and offset in tree
	 *
	 * @param coneAperture the cone aperture
	 * @param offsetTree the offset in the tree
	 *
	 * @return the sampled value
	 */
	template< int channel >
	__device__
	__forceinline__ float4 getValue( const float coneAperture, const float3 offsetTree ) const;

	/**
	 * Move sample offset in node tree
	 *
	 * @param offsetTree offset in tree
	 */
	__device__
	__forceinline__ void moveSampleOffsetInNodeTree( const float3 offsetTree );

	/**
	 * Update MipMap parameters given cone aperture
	 *
	 * @param coneAperture the cone aperture
	 *
	 * @return It returns false if coneAperture > voxelSize in parent brick
	 */
	__device__
	__forceinline__ bool updateMipMapParameters( const float coneAperture );

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

}

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsSamplerKernel.inl"

#endif // !_GV_SAMPLER_KERNEL_H_
