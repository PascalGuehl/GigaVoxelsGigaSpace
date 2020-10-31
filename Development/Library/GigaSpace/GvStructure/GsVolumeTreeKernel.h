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

#ifndef _GV_VOLUME_TREE_KERNEL_H_
#define _GV_VOLUME_TREE_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsCUDATexHelpers.h"
#include "GvCore/GsPool.h"
#include "GvCore/GsLocalizationInfo.h"
#include "GvRendering/GsRendererHelpersKernel.h"
#include "GvStructure/GsNode.h"

// CUDA
#include <cuda_surface_types.h>
#include <surface_types.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// Define pools names
#define TEXDATAPOOL 0
#define USE_LINEAR_VOLTREE_TEX 0

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

//----------------------------
// Node pool content declaration (i.e. volume tree children and volume tree data.)
//----------------------------

#if USE_LINEAR_VOLTREE_TEX

/**
 * Volume tree children are placed in a 1D texture
 */
texture< uint, cudaTextureType1D, cudaReadModeElementType > volumeTreeChildTexLinear; // linear texture

/**
 * Volume tree data are placed in a 1D texture
 */
texture< uint, cudaTextureType1D, cudaReadModeElementType > volumeTreeDataTexLinear; // linear texture

#else // USE_LINEAR_VOLTREE_TEX

/**
 * Volume tree children are placed in constant memory
 */
__constant__ GvCore::GsLinearMemoryKernel< uint > k_volumeTreeChildArray;

/**
 * Volume tree data are placed in constant memory
 */
__constant__ GvCore::GsLinearMemoryKernel< uint > k_volumeTreeDataArray;

#ifdef GS_USE_NODE_META_DATA
/**
 * Data structure's per node's meta data are placed in constant memory
 */
__constant__ GvCore::GsLinearMemoryKernel< uint > k_dataStructureMetaDataArray;
#endif

#ifdef GS_USE_MULTI_OBJECTS
/**
 * Current object ID
 */
__constant__ uint k_objectID;
#endif

#endif // USE_LINEAR_VOLTREE_TEX

//----------------------------
// Surfaces declaration.
// Surfaces are used to write into textures.
//----------------------------

//namespace GvStructure
//{
    /**
     * Surfaces declaration used to write data into cache.
	 *
	 * NOTE : there are only 8 surfaces available.
	 *
	 * It is a wrapper to declare surfaces :
	 * - surface< void, cudaSurfaceType3D > surfaceRef_0;
     */
//#if (__CUDA_ARCH__ >= 200)
	// => moved to GvCore/GsPool.h
    //GPUPoolSurfaceReferences( 0 )
    //GPUPoolSurfaceReferences( 1 )
    //GPUPoolSurfaceReferences( 2 )
    //GPUPoolSurfaceReferences( 3 )
    //GPUPoolSurfaceReferences( 4 )
    //GPUPoolSurfaceReferences( 5 )
    //GPUPoolSurfaceReferences( 6 )
    //GPUPoolSurfaceReferences( 7 )
 //#endif
//}

//----------------------------
// 3D textures declaration
//----------------------------

/**
 * Char type.
 */
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, char, cudaReadModeNormalizedFloat );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, char4, cudaReadModeNormalizedFloat );

/**
 * Unsigned char type.
 */
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, uchar, cudaReadModeNormalizedFloat );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, uchar4, cudaReadModeNormalizedFloat );

/**
 * Short type.
 */
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, short, cudaReadModeNormalizedFloat );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, short2, cudaReadModeNormalizedFloat );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, short4, cudaReadModeNormalizedFloat );

/**
 * Unsigned short type.
 */
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, ushort, cudaReadModeNormalizedFloat );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, ushort2, cudaReadModeNormalizedFloat );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, ushort4, cudaReadModeNormalizedFloat );

/**
 * Float type.
 */
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, float, cudaReadModeElementType );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, float2, cudaReadModeElementType );
GPUPoolTextureReferences( TEXDATAPOOL, 4, 3, float4, cudaReadModeElementType );

/**
 * Half type.
 * Note : a redirection is used to float4
 */
GPUPoolTextureRedirection( TEXDATAPOOL, 4, 3, half4, cudaReadModeElementType, float4 );

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvStructure
{

/** 
 * @struct VolumeTreeKernel
 *
 * @brief The VolumeTreeKernel struct provides the interface to a GigaVoxels
 * data structure on device (GPU).
 *
 * @ingroup GvStructure
 *
 * This is the device-side associated object to a GigaVoxels data structure.
 *
 * @todo: Rename VolumeTreeKernel as GsVolumeTreeKernel.
 */
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
struct VolumeTreeKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/**
	 * Enumeration to define the brick border size
	 */
	enum
	{
		brickBorderSize = BorderSize
	};

	/**
	 * Type definition of the node resolution
	 */
	typedef NodeTileRes NodeResolution;

	/**
	 * Type definition of the brick resolution
	 */
	typedef BrickRes BrickResolution;

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Root node address
	 */
	uint _rootAddress;

	/**
	 * Size of a voxel in the cache (i.e. pool of bricks of voxels implemented as a 3D texture)
	 */
	float3 brickCacheResINV;

	/**
	 * Size of a brick of voxels in the cache (i.e. pool of bricks of voxels implemented as a 3D texture)
	 */
	float3 brickSizeInCacheNormalized;

	/******************************** METHODS *********************************/

	/** @name Sampling data
	 *
	 *  Methods to sample user data attributes in the data structure (i.e. color, normal, density, etc...)
	 */
	///@{

	/**
	 * Sample data in specified channel at a given position.
	 * 3D texture are used with hardware tri-linear interpolation.
	 *
	 * @param pBrickPos Brick position in the pool of bricks
	 * @param pPosInBrick Position in brick
	 *
	 * @return the sampled value
	 */
	template< int TChannel >
	__device__
	__forceinline__ float4 getSampleValueTriLinear( float3 pBrickPos, float3 pPosInBrick ) const;

	/**
	 * Sample data in specified channel at a given position.
	 * 3D texture are used with hardware tri-linear interpolation.
	 *
	 * @param mipMapInterpCoef mipmap interpolation coefficient
	 * @param brickChildPosInPool brick child position in pool
	 * @param brickParentPosInPool brick parent position in pool
	 * @param posInBrick position in brick
	 * @param coneAperture cone aperture
	 *
	 * @return the sampled value
	 */
	// QUESTION : le paramètre "coneAperture" ne semble pas utilisé ? A quoi sert-il (servait ou servira) ?
	template< int TChannel >
	__device__
	__forceinline__ float4 getSampleValueQuadriLinear( float mipMapInterpCoef, float3 brickChildPosInPool,
											  float3 brickParentPosInPool, float3 posInBrick, float coneAperture ) const;

	/**
	 * Sample data in specified channel at a given position.
	 * 3D texture are used with hardware tri-linear interpolation.
	 *
	 * @param mipMapInterpCoef mipmap interpolation coefficient
	 * @param brickChildPosInPool brick child position in pool
	 * @param brickParentPosInPool brick parent position in pool
	 * @param posInBrick position in brick
	 * @param coneAperture cone aperture
	 *
	 * @return the sampled value
	 */
	template< int TChannel >
	__device__
	__forceinline__ float4 getSampleValue( float3 brickChildPosInPool, float3 brickParentPosInPool,
								  float3 sampleOffsetInBrick, float coneAperture, bool mipMapOn, float mipMapInterpCoef ) const;

	///@}

	/**
	 * ...
	 *
	 * @param nodeTileAddress ...
	 * @param nodeOffset ...
	 *
	 * @return ...
	 */
	__device__
	__forceinline__ uint computenodeAddress( uint3 nodeTileAddress, uint3 nodeOffset ) const;

	/**
	 * ...
	 *
	 * @param nodeTileAddress ...
	 * @param nodeOffset ...
	 *
	 * @return ...
	 */
	__device__
	__forceinline__ uint3 computeNodeAddress( uint3 nodeTileAddress, uint3 nodeOffset ) const;

	/** @name Reading nodes information
	 *
	 *  Methods to read nodes information from the data structure (nodes address and its flags)
	 */
	///@{

	/**
	 * Retrieve node information (address + flags) from data structure
	 *
	 * @param resnode ...
	 * @param nodeTileAddress ...
	 * @param nodeOffset ...
	 */
	__device__
	__forceinline__ void fetchNode( GsNode& resnode, uint3 nodeTileAddress, uint3 nodeOffset ) const;

	/**
	 * Retrieve node information (address + flags) from data structure
	 *
	 * @param resnode ...
	 * @param nodeTileAddress ...
	 * @param nodeOffset ...
	 */
	__device__
	__forceinline__ void fetchNode( GsNode& resnode, uint nodeTileAddress, uint nodeOffset ) const;

	/**
	 * Retrieve node information (address + flags) from data structure
	 *
	 * @param resnode ...
	 * @param nodeAddress ...
	 */
	__device__
	__forceinline__ void fetchNode( GsNode& resnode, uint nodeAddress ) const;

	///**
	// * ...
	// *
	// * @param resnode ...
	// * @param nodeTileAddress ...
	// * @param nodeOffset ...
	// */
	//__device__
	//__forceinline__ void fetchNodeChild( GsNode& resnode, uint nodeTileAddress, uint nodeOffset );

	///**
	// * ...
	// *
	// * @param resnode ...
	// * @param nodeTileAddress ...
	// * @param nodeOffset ...
	// */
	//__device__
	//__forceinline__ void fetchNodeData( GsNode& resnode, uint nodeTileAddress, uint nodeOffset );

	///@}

	/** @name Writing nodes information
	 *
	 *  Methods to write nodes information into the data structure (nodes address and its flags)
	 */
	///@{

	/**
	 * Write node information (address + flags) in data structure
	 *
	 * @param node ...
	 * @param nodeTileAddress ...
	 * @param nodeOffset ...
	 */
	__device__
	__forceinline__ void setNode( GsNode node, uint3 nodeTileAddress, uint3 nodeOffset );

	/**
	 * Write node information (address + flags) in data structure
	 *
	 * @param node ...
	 * @param nodeTileAddress ...
	 * @param nodeOffset ...
	 */
	__device__
	__forceinline__ void setNode( GsNode node, uint nodeTileAddress, uint nodeOffset );

	/**
	 * Write node information (address + flags) in data structure
	 *
	 * @param node ...
	 * @param nodeAddress ...
	 */
	__device__
	__forceinline__ void setNode( GsNode node, uint nodeAddress );

	///@}

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

} //namespace GvStructure

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsVolumeTreeKernel.inl"

#endif // !_GV_VOLUME_TREE_KERNEL_H_
