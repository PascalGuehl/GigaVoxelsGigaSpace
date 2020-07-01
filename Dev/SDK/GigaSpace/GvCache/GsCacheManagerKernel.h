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

#ifndef _GV_CACHE_MANAGER_KERNEL_H_
#define _GV_CACHE_MANAGER_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Cuda
#include <vector_types.h>

// Gigavoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsLinearMemoryKernel.h"
#include "GvRendering/GsRendererContext.h"
#include "GvStructure/GsNode.h"

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

namespace GvCache
{

/** 
 * @struct GsCacheManagerKernel
 *
 * @brief The GsCacheManagerKernel class provides mecanisms to update usage information of elements
 *
 * @ingroup GvCore
 * @namespace GvCache
 *
 * GPU side object used to update timestamp usage information of an element (node tile or brick)
 */
template< class ElementRes, class AddressType >
struct GsCacheManagerKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Timestamp buffer.
	 * It holds usage information of elements
	 */
	GvCore::GsLinearMemoryKernel< uint > _timeStampArray;

	/******************************** METHODS *********************************/

	/**
	 * Update timestamp usage information of an element (node tile or brick)
	 * with current time (i.e. current rendering pass)
	 * given its address in its corresponding pool (node or brick).
	 *
	 * @param pElemAddress The address of the element for which we want to update usage information
	 */
	__device__
	__forceinline__ void setElementUsage( uint pElemAddress );

	/**
	 * Update timestamp usage information of an element (node tile or brick)
	 * with current time (i.e. current rendering pass)
	 * given its address in its corresponding pool (node or brick).
	 *
	 * @param pElemAddress The address of the element on which we want to update usage information
	 */
	__device__
	__forceinline__ void setElementUsage( uint3 pElemAddress );

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

} // namespace GvCache

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsCacheManagerKernel.inl"

/******************************************************************************
 ***************************** KERNEL DEFINITION ******************************
 ******************************************************************************/

namespace GvCache
{

/******************************************************************************
 * GsKernel_CacheManager_retrieveElementUsageMasks kernel
 *
 * This kernel creates the usage mask list of used and non used elements (in current rendering pass) in a single pass
 *
 * @param pCacheManager Cache manager
 * @param pNbElements Number of elememts to process
 * @param pTimeStampsElemAddressList Timestamp buffer list
 * @param pUnusedElementMasks Resulting temporary mask list of non-used elements
 * @param pUsedElementMasks Resulting temporary mask list of used elements
 ******************************************************************************/
template< class ElementRes, class AddressType >
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GsKernel_CacheManager_retrieveElementUsageMasks( GsCacheManagerKernel< ElementRes, AddressType > pCacheManager,
								  const uint pNbElements, const uint* __restrict__ pTimeStampsElemAddressList,
								  uint* __restrict__ pUnusedElementMasks, uint* __restrict__ pUsedElementMasks );

	/******************************************************************************
	 * InitElemAddressList kernel
	 *
	 * @param addressList
	 * @param numElems
	 * @param elemsCacheRes
	 ******************************************************************************/
	template< typename AddressType >
	__global__
	// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
	void InitElemAddressList( uint* addressList, uint numElems, uint3 elemsCacheRes )
	{
		uint lineSize = __uimul( blockDim.x, gridDim.x );
		uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

		if ( elem < numElems )
		{
			uint3 pos;
			pos.x = elem % elemsCacheRes.x;
			pos.y = ( elem / elemsCacheRes.x ) % elemsCacheRes.y;
			pos.z = ( elem / ( elemsCacheRes.x * elemsCacheRes.y ) );

			addressList[ elem - 1 ] = AddressType::packAddress( pos );
		}
	}

/******************************************************************************
 * CacheManagerFlagInvalidations KERNEL
 *
 * Reset the time stamp info of given elements to 1.
 *
 * @param pCacheManager cache manager
 * @param pNbElements number of elements to process
 * @param pSortedElemAddressList input list of elements to process (sorted list with unused elements before used ones)
 *
 * TODO : pass the "timestamps" array directly instead of pCacheManager wrapper, in order to be able to add __restrict__ keyword 
 ******************************************************************************/
template< class ElementRes, class AddressType >
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void CacheManagerFlagInvalidations( GsCacheManagerKernel< ElementRes, AddressType > pCacheManager,
								   const uint pNbElements, const uint* __restrict__ pSortedElemAddressList )
{
	// Retrieve global index
	const uint lineSize = __uimul( blockDim.x, gridDim.x );
	const uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Check bounds
	if ( elem < pNbElements )
	{
		// Retrieve element address processed by current thread
		const uint elemAddressEnc = pSortedElemAddressList[ elem ];
		const uint3 elemAddress = AddressType::unpackAddress( elemAddressEnc );

		// Update cache manager element (1 is set to reset timestamp)
		pCacheManager._timeStampArray.set( elemAddress, 1 );
	}
}

/******************************************************************************
 * CacheManagerInvalidatePointers KERNEL
 *
 * Reset all node addresses in the cache to NULL (i.e 0).
 * Only the first 30 bits of address are set to 0, not the 2 first flags.
 *
 * @param pCacheManager cache manager
 * @param pNbElements number of elements to process
 * @param pPageTable page table associated to the cache manager from which elements will be processed
 ******************************************************************************/
template< class ElementRes, class AddressType, class PageTableKernelArrayType >
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void CacheManagerInvalidatePointers( GsCacheManagerKernel< ElementRes, AddressType > pCacheManager,
									const uint pNbElements, PageTableKernelArrayType pPageTable )
{
	// Retrieve global data index
	const uint lineSize = __uimul( blockDim.x, gridDim.x );
	const uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Check bounds
	if ( elem < pNbElements )
	{
		const uint elementPointer = pPageTable.get( elem );

		// TODO: allow constant values !

		if ( ! AddressType::isNull( elementPointer ) )
		{
			const uint3 elemAddress = AddressType::unpackAddress( elementPointer );
			const uint3 elemCacheSlot = ( elemAddress / ElementRes::get() );

			if ( pCacheManager._timeStampArray.get( elemCacheSlot ) == 1 )
			{
				// Reset the 30 first bits of address to 0, and let the 2 first the same
				pPageTable.set( elem, elementPointer & ~(AddressType::packedMask) );
			}
		}
	}
}

/******************************************************************************
 * CacheManagerCreateUpdateMask kernel
 *
 * Given a flag indicating a type of request (subdivision or load), and the updated global buffer of requests,
 * it fills a resulting mask buffer.
 * In fact, this mask indicates, for each element of the usage list, if it was used in the current rendering pass
 *
 * @param pNbElements Number of elements to process
 * @param pUpdateList Buffer of node addresses updated with subdivision or load requests.
 * @param pResMask List of resulting usage mask
 * @param pFlag Request flag : either node subdivision or data load/produce
 ******************************************************************************/
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void CacheManagerCreateUpdateMask( const uint pNbElements, const uint* __restrict__ pUpdateList, uint* __restrict__ pResMask, const uint pFlag )
{
	// Retrieve global data index
	const uint lineSize = __uimul( blockDim.x, gridDim.x );
	const uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Out of bound check
	if ( elem < pNbElements )
	{
		// Retrieve
		const uint elemVal = pUpdateList[ elem ];

		// Compare element value with flag and write mask value
		if ( elemVal & pFlag )
		{
			pResMask[ elem ] = 1;
		}
		else
		{
			pResMask[ elem ] = 0;
		}
	}
}

/******************************************************************************
 * CacheManagerCreateUpdateMask kernel
 *
 * Given a flag indicating a type of request (subdivision or load), and the updated global buffer of requests,
 * it fills a resulting mask buffer.
 * In fact, this mask indicates, for each element of the usage list, if it was used in the current rendering pass
 *
 * @param pNbElements Number of elements to process
 * @param pUpdateList Buffer of node addresses updated with subdivision or load requests.
 * @param pResMask List of resulting usage mask
 * @param pFlag Request flag : either node subdivision or data load/produce
 ******************************************************************************/
//#ifdef GS_USE_MULTI_OBJECTS
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void CacheManagerCreateUpdateMask( const uint pNbElements, const uint* __restrict__ pRequests, uint* __restrict__ pMasks, const uint pFlag, const uint* __restrict__ pObjectIDs, const uint pObjectID )
{
	// Retrieve global data index
	const uint lineSize = __uimul( blockDim.x, gridDim.x );
	const uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Out of bound check
	if ( elem < pNbElements )
	{
		// Retrieve
		const uint elemVal = pRequests[ elem ];

		// Compare element value with flag one and write mask value
		if ( ( elemVal & pFlag ) && ( pObjectIDs[ elem ] == pObjectID ) )
		{
			pMasks[ elem ] = 1;
		}
		else
		{
			pMasks[ elem ] = 0;
		}
	}
}
//#endif

	// Optim
	/******************************************************************************
	 * UpdateBrickUsageFromNodes kernel
	 *
	 * @param numElem
	 * @param nodeTilesAddressList
	 * @param volumeTree
	 * @param gpuCache
	 ******************************************************************************/
	template< class VolTreeKernel, class GPUCacheType >
	__global__
	// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
	void UpdateBrickUsageFromNodes( uint numElem, uint *nodeTilesAddressList, VolTreeKernel volumeTree, GPUCacheType gpuCache )
	{
		uint lineSize = __uimul( blockDim.x, gridDim.x );
		uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

		if ( elem < numElem )
		{
			uint nodeTileAddressEnc = nodeTilesAddressList[ elem ];
			uint nodeTileAddressNode = nodeTileAddressEnc * VolTreeKernel::NodeResolution::getNumElements();

			for ( uint i = 0; i < VolTreeKernel::NodeResolution::getNumElements(); ++i )
			{
				GvStructure::GsNode node;
				volumeTree.fetchNode( node, nodeTileAddressNode, i );

				if ( node.hasBrick() )
				{
					gpuCache._brickCacheManager.setElementUsage( node.getBrickAddress() );
					//setBrickUsage<VolTreeKernel>(node.getBrickAddress());
				}
			}
		}
	}

	// FIXME: Move this to another place!
	/******************************************************************************
	 * UpdateBrickUsageFromNodes kernel
	 *
	 * @param syntheticBuffer
	 * @param numElems
	 * @param lruElemAddressList
	 * @param elemsCacheSize
	 ******************************************************************************/
	template< typename ElementRes, typename AddressType >
	__global__
	// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
	void SyntheticInfo_Update_DataWrite( uchar4* syntheticBuffer, uint numElems, uint* lruElemAddressList, uint3 elemsCacheSize )
	{
		uint lineSize = __uimul( blockDim.x, gridDim.x );
		uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

		if ( elem < numElems )
		{
			uint pageIdxEnc = lruElemAddressList[ elem ];
			uint3 pageIdx = AddressType::unpackAddress( pageIdxEnc );
			uint syntheticIdx = pageIdx.x + pageIdx.y * elemsCacheSize.x + pageIdx.z * elemsCacheSize.x * elemsCacheSize.y;
			syntheticBuffer[ syntheticIdx ].w = 1;
		}
	}

	/******************************************************************************
	 * UpdateBrickUsageFromNodes kernel
	 *
	 * @param syntheticBuffer
	 * @param numPageUsed
	 * @param usedPageList
	 * @param elemsCacheSize
	 ******************************************************************************/
	template< typename ElementRes, typename AddressType >
	__global__
	// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
	void SyntheticInfo_Update_PageUsed( uchar4* syntheticBuffer, uint numPageUsed, uint* usedPageList, uint3 elemsCacheSize )
	{
		uint lineSize = __uimul( blockDim.x, gridDim.x );
		uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

		if ( elem < numPageUsed )
		{
			uint pageIdxEnc = usedPageList[ elem ];
			uint3 pageIdx = AddressType::unpackAddress( pageIdxEnc );
			uint syntheticIdx = pageIdx.x + pageIdx.y * elemsCacheSize.x + pageIdx.z * elemsCacheSize.x * elemsCacheSize.y;
			syntheticBuffer[ syntheticIdx ].x = 1;
		}
	}

} // namespace GvCache

#endif // !_GV_CACHE_MANAGER_KERNEL_H_
