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

#ifndef _GV_DATA_PRODUCTION_MANAGER_KERNEL_H_
#define _GV_DATA_PRODUCTION_MANAGER_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Cuda
#include <helper_math.h>

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsOracleRegionInfo.h"
#include "GvCore/GsPool.h"
#include "GvCore/GsLinearMemoryKernel.h"
#include "GvCore/GsVector.h"
#include "GvCore/GsLocalizationInfo.h"
#include "GvCore/GsIPriorityPoliciesManagerKernel.h"
#include "GvCache/GsCacheManagerKernel.h"
#include "GvStructure/GsVolumeTree.h"

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

namespace GvStructure
{

/** 
 * @struct GsDataProductionManagerKernel
 *
 * @brief The GsDataProductionManagerKernel struct provides methods to update buffer
 * of requests on device.
 *
 * Device-side object used to update the buffer of requests emitted by the renderer
 * during the data structure traversal. Requests can be either "node subdivision"
 * or "load brick of voxels".
 */
template< class NodeTileRes, class BrickFullRes, class NodeAddressType, class BrickAddressType, class TPriorityPoliciesManager >
struct GsDataProductionManagerKernel
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Bit mask for subdivision request (30th bit)
	 */
	static const unsigned int VTC_REQUEST_SUBDIV = 0x40000000U;

	/**
	 * Bit mask for load request (31th bit)
	 */
	static const unsigned int VTC_REQUEST_LOAD = 0x80000000U;

	/**
	 * Buffer used to store node addresses updated with subdivision or load requests
	 */
	GvCore::GsLinearMemoryKernel< uint > _updateBufferArray;

	/**
	 * Buffer used to store priority associated with subdivision or load requests
	 */
	GvCore::GsLinearMemoryKernel< int > _priorityBufferArray;

#ifdef GS_USE_MULTI_OBJECTS
	/**
	 * Buffer used to store object IDs
	 */
	GvCore::GsLinearMemoryKernel< uint > _objectIDBuffer;
#endif

	/**
	 * Node cache manager
	 *
	 * Used to update timestamp usage information of nodes
	 */
	GvCache::GsCacheManagerKernel< NodeTileRes, NodeAddressType > _nodeCacheManager;

	/**
	 * Brick cache manager
	 *
	 * Used to update timestamp usage information of bricks
	 */
	GvCache::GsCacheManagerKernel< BrickFullRes, BrickAddressType > _brickCacheManager;

	/******************************** METHODS *********************************/

	/**
	 * Update buffer with a subdivision request for a given node.
	 *
	 * @param pNodeAddressEnc the encoded node address
	 */
	__device__
	__forceinline__ void subDivRequest( uint pNodeAddressEnc );

	/**
	 * Update buffer with a load request for a given node.
	 *
	 * @param pNodeAddressEnc the encoded node address
	 */
	__device__
	__forceinline__ void loadRequest( uint pNodeAddressEnc );

	/**
	 * Update buffer with a subdivision request for a given node.
	 *
	 * @param pNodeAddressEnc the encoded node address
	 * @param pParams the priority parameters
	 */
	__device__
	__forceinline__ void subDivRequest( uint pNodeAddressEnc, const GvCore::GsPriorityParameters& pParams );

	/**
	 * Update buffer with a load request for a given node.
	 *
	 * @param pNodeAddressEnc the encoded node address
	 * @param pParams the priority parameters
	 */
	__device__
	__forceinline__ void loadRequest( uint pNodeAddressEnc, const GvCore::GsPriorityParameters& pParams );

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

} // namespace GvStructure

/******************************************************************************
 ***************************** KERNEL DEFINITION ******************************
 ******************************************************************************/

namespace GvStructure
{

/******************************************************************************
 * KERNEL GsKernel_ClearVolTreeRoot
 *
 * This clears node pool child and brick 1st nodetile after root node.
 *
 * @param pDataStructure data structure
 * @param pRootAddress root node address from which to clear data
 ******************************************************************************/
template< typename VolTreeKernelType >
__global__
void GsKernel_ClearVolTreeRoot( VolTreeKernelType volumeTree, const uint rootAddress );

// Updates
/******************************************************************************
 * KERNEL UpdateBrickUsage
 *
 * @param volumeTree ...
 * @param rootAddress ...
 ******************************************************************************/
template< typename ElementRes, typename GPUCacheType >
__global__
void UpdateBrickUsage( uint numElems, uint* lruElemAddressList, GPUCacheType gpuCache );

/******************************************************************************
 * KERNEL GvKernel_PreProcessRequests
 *
 * This kernel is used as first pass a stream compaction algorithm
 * in order to create the masks of valid requests
 * (i.e. the ones that have been requested during the N3-Tree traversal).
 *
 * @param pRequests Array of requests (i.e. subdivide nodes or load/produce bricks)
 * @param pIsValidMask Resulting array of isValid masks
 * @param pNbElements Number of elememts to process
 ******************************************************************************/
__global__
void GvKernel_PreProcessRequests( const uint* __restrict__ pRequests, unsigned int* __restrict__ pIsValidMasks, const uint pNbElements );

///******************************************************************************
// * ...
// ******************************************************************************/
//template< typename TDataStructure, typename TPageTable >
//__global__ void GVKernel_TrackLeafNodes( TDataStructure pDataStructure, TPageTable pPageTable, const unsigned int pNbNodes, const unsigned int pMaxDepth, unsigned int* pLeafNodes )
//{
//	// Retrieve global index
//	const uint index = threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.x * gridDim.x;
//
//	// Check bounds
//	if ( index < pNbNodes )
//	{
//		// Try to retrieve node from the node pool given its address
//		GvStructure::GsNode node;
//		pDataStructure.fetchNode( node, index );
//
//		// Check if node has been initialized
//		// - maybe its safer to check only for childAddress
//		//if ( node.isInitializated() )
//		if ( node.childAddress != 0 )
//		{
//			// Retrieve node depth
//			const GvCore::GsLocalizationInfo localizationInfo = pPageTable.getLocalizationInfo( index );
//			
//			// Check node depth
//			if ( nodeDepth < pMaxDepth )
//			{
//				// Check is node is a leaf
//				if ( ! node.hasSubNodes() )
//				{
//					// Check is node is empty
//					if ( ! node.hasBrick() )
//					{
//						// Empty node
//						pLeafNodes[ index ] = 1;
//					}
//					else
//					{
//						// Node has data
//						pLeafNodes[ index ] = 0;
//					}
//				}
//				else
//				{
//					// Node has children
//					pLeafNodes[ index ] = 0;
//				}
//			}
//			else if ( nodeDepth == pMaxDepth )
//			{
//				// Check is node is empty
//				if ( ! node.hasBrick() )
//				{
//					// Empty node
//					pLeafNodes[ index ] = 1;
//				}
//				else
//				{
//					// Node has data
//					pLeafNodes[ index ] = 0;
//				}
//			}
//			else // ( localizationInfo.locDepth > pMaxDepth )
//			{
//				pLeafNodes[ index ] = 0;
//			}
//		}
//		else
//		{
//			// Uninitialized node
//			pLeafNodes[ index ] = 0;
//		}
//	}
//}

/******************************************************************************
 * Retrieve number of leaf nodes in tree based on a given max depth
 ******************************************************************************/
template< typename TDataStructure, typename TPageTable >
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GVKernel_TrackLeafNodes( TDataStructure pDataStructure, TPageTable pPageTable, const unsigned int pNbNodes, const unsigned int pMaxDepth, unsigned int* pLeafNodes, float* pEmptyVolume )
{
	// Retrieve global index
	const uint index = threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.x * gridDim.x;

	// Check bounds
	if ( index < pNbNodes )
	{
		// Try to retrieve node from the node pool given its address
		GvStructure::GsNode node;
		pDataStructure.fetchNode( node, index );

		//---------------------------
		if ( node.childAddress == 0 )
		{
			// Node has data
			pLeafNodes[ index ] = 0;

			// Update volume
			pEmptyVolume[ index ] = 0.f;

			return;
		}
		//---------------------------

		// Retrieve its "node tile" address
		//const uint nodeTileAddress = index / 8/*NodeTileRes::getNumElements()*/;

		// Retrieve node depth
		const GvCore::GsLocalizationInfo localizationInfo = pPageTable.getLocalizationInfo( index );
		const unsigned int nodeDepth = localizationInfo.locDepth.get()/* + 1*/;
		
		// Check node depth
		if ( localizationInfo.locDepth.get() < pMaxDepth )
		{
			if ( node.childAddress != 0 )
			{
				// Check if node is a leaf
				if ( ! node.hasSubNodes() )
				{
					// Check if node is empty
					if ( ! node.hasBrick() )
					{
						// Empty node
						pLeafNodes[ index ] = 1;

						// Update volume
						//printf( "\n%u", nodeDepth );
						const float nodeSize = 1.f / static_cast< float >( 1 << nodeDepth );
						pEmptyVolume[ index ] = nodeSize * nodeSize * nodeSize;
					}
					else
					{
						// Node has data
						pLeafNodes[ index ] = 0;

						// Update volume
						pEmptyVolume[ index ] = 0.f;
					}
				}
				else
				{
					// Node has children
					pLeafNodes[ index ] = 0;

					// Update volume
					pEmptyVolume[ index ] = 0.f;
				}
			}
			else
			{
				// ...
				pLeafNodes[ index ] = 0;

				// Update volume
				pEmptyVolume[ index ] = 0.f;
			}
		}
		else if ( localizationInfo.locDepth.get() == pMaxDepth )
		{
			if ( node.childAddress != 0 )
			{
				// Check if node is empty
				if ( ! node.hasBrick() )
				{
					// Empty node
					pLeafNodes[ index ] = 1;

					// Update volume
					const float nodeSize = 1.f / static_cast< float >( 1 << nodeDepth );
					pEmptyVolume[ index ] = nodeSize * nodeSize * nodeSize;
				}
				else
				{
					// Node has data
					pLeafNodes[ index ] = 0;

					// Update volume
					pEmptyVolume[ index ] = 0.f;
				}
			}
			else
			{
				// ...
				pLeafNodes[ index ] = 0;

				// Update volume
				pEmptyVolume[ index ] = 0.f;
			}
		}
		else // ( localizationInfo.locDepth > pMaxDepth )
		{
			// Don't take node into account
			pLeafNodes[ index ] = 0;

			// Update volume
			pEmptyVolume[ index ] = 0.f;
		}
	}
}

///******************************************************************************
// * ...
// ******************************************************************************/
//template< typename TDataStructure, typename TPageTable >
//__global__ void GVKernel_TrackNodes( TDataStructure pDataStructure, TPageTable pPageTable, const unsigned int pNbNodes, const unsigned int pMaxDepth, unsigned int* pLeafNodes )
//{
//	// Retrieve global index
//	const uint index = threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.x * gridDim.x;
//
//	// Check bounds
//	if ( index < pNbNodes )
//	{
//		// Try to retrieve node from the node pool given its address
//		GvStructure::GsNode node;
//		pDataStructure.fetchNode( node, index );
//
//		// Check if node has been initialized
//		// - maybe its safer to check only for childAddress
//		//if ( node.isInitializated() )
//		if ( node.childAddress != 0 )
//		{
//			// Retrieve node depth
//			GvCore::GsLocalizationInfo localizationInfo = pPageTable.getLocalizationInfo( index );
//			
//			// Check if node is a leaf
//			if ( nodeDepth < pMaxDepth )
//			{
//				if ( ! node.hasSubNodes() )
//				{
//					pLeafNodes[ index ] = 1;
//				}
//				else
//				{
//					pLeafNodes[ index ] = 0;
//				}
//			}
//			else if ( nodeDepth == pMaxDepth )
//			{
//				pLeafNodes[ index ] = 1;
//			}
//			else // ( localizationInfo.locDepth > pMaxDepth )
//			{
//				pLeafNodes[ index ] = 0;
//			}
//		}
//		else
//		{
//			// Uninitialized node
//			pLeafNodes[ index ] = 0;
//		}
//	}
//}

/******************************************************************************
 * ...
 ******************************************************************************/
template< typename TDataStructure, typename TPageTable >
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GVKernel_TrackNodes( TDataStructure pDataStructure, TPageTable pPageTable, const unsigned int pNbNodes, const unsigned int pMaxDepth, unsigned int* pLeafNodes, float* pEmptyVolume )
{
	//const float pi = 3.141592f;

	// Retrieve global index
	const uint index = threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.x * gridDim.x;

	// Check bounds
	if ( index < pNbNodes )
	{
		// Try to retrieve node from the node pool given its address
		GvStructure::GsNode node;
		pDataStructure.fetchNode( node, index );

		// Retrieve its "node tile" address
		const uint nodeTileAddress = index / 8/*NodeTileRes::getNumElements()*/;

		// Retrieve node depth
		const GvCore::GsLocalizationInfo localizationInfo = pPageTable.getLocalizationInfo( nodeTileAddress );
		const unsigned int nodeDepth = localizationInfo.locDepth.get() + 1;

		// Check node depth
		if ( nodeDepth < pMaxDepth )
		{
			if ( node.childAddress != 0 )
			{
				// Check if node is a leaf
				if ( ! node.hasSubNodes() )
				{
					// Leaf node
					pLeafNodes[ index ] = 1;

					// Update volume
					const float nodeSize = 1.f / static_cast< float >( 1 << nodeDepth );
					pEmptyVolume[ index ] = nodeSize * nodeSize * nodeSize;
				}
				else
				{
					// Node has children
					pLeafNodes[ index ] = 0;

					// Update volume
					pEmptyVolume[ index ] = 0.f;
				}
			}
			else
			{
				// ...
				pLeafNodes[ index ] = 0;

				// Update volume
				pEmptyVolume[ index ] = 0.f;
			}
		}
		else if ( nodeDepth == pMaxDepth )
		{
			if ( node.childAddress != 0 )
			{
				// Leaf node
				pLeafNodes[ index ] = 1;

				// Update volume
				const float nodeSize = 1.f / static_cast< float >( 1 << nodeDepth );
				pEmptyVolume[ index ] = nodeSize * nodeSize * nodeSize;
			}
			else
			{
				// ...
				pLeafNodes[ index ] = 0;

				// Update volume
				pEmptyVolume[ index ] = 0.f;
			}
		}
		else // ( localizationInfo.locDepth > pMaxDepth )
		{
			// Don't take node into account
			pLeafNodes[ index ] = 0;

			// Update volume
			pEmptyVolume[ index ] = 0.f;
		}
	}
}
//---------------------------------------------------------------------------------------------------

} // namespace GvStructure

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsDataProductionManagerKernel.inl"

#endif // !_GV_DATA_PRODUCTION_MANAGER_KERNEL_H_

