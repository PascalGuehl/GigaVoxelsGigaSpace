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

#ifndef _GV_CACHE_HELPER_KERNEL_H_
#define _GV_CACHE_HELPER_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GvCore/GsCoreConfig.h"
#include "GvStructure/GsVolumeTreeAddressType.h"
#include "GvCore/GsVectorTypesExt.h"

/******************************************************************************
 ***************************** KERNEL DEFINITION ******************************
 ******************************************************************************/

namespace GvCache
{

/******************************************************************************
 * KERNEL GvKernel_genericWriteIntoCache
 *
 * This method is a helper for writing into the cache.
 *
 * @param pNumElems The number of elements we need to produce and write.
 * @param pNodesAddressList buffer of element addresses that producer(s) has to process (subdivide or produce/load)
 * @param pElemAddressList buffer of available element addresses in cache where producer(s) can write
 * @param pGpuPool The pool where we will write the produced elements.
 * @param pGpuProvider The provider called for the production.
 * @param pPageTable page table used to retrieve localization information from element addresses
 ******************************************************************************/
template< typename TElementRes, typename TGPUPoolType, typename TGPUProviderType, typename TPageTableType >
__global__
// TO DO : Prolifing / Optimization
// - use "Launch Bounds" feature to profile / optimize code
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GvKernel_genericWriteIntoCache( const uint pNumElems, uint* pNodesAddressList, uint* pElemAddressList,
						    TGPUPoolType pGpuPool, TGPUProviderType pGpuProvider, TPageTableType pPageTable )
{
	// Retrieve global indexes
	const uint elemNum = blockIdx.x;
	const uint processID = threadIdx.x + __uimul( threadIdx.y, blockDim.x ) + __uimul( threadIdx.z, __uimul( blockDim.x, blockDim.y ) );

	// Check bound
	if ( elemNum < pNumElems )
	{
		// Clean the syntax a bit
		typedef typename TPageTableType::ElemAddressType ElemAddressType;

		// Shared Memory declaration
		__shared__ uint nodeAddress;
		__shared__ ElemAddressType elemAddress;
		__shared__ GvCore::GsLocalizationInfo parentLocInfo;

		// Retrieve node address and its localisation info along with new element address
		//
		// Done by only one thread of the kernel
		if ( processID == 0 )
		{
			// Compute node address
			const uint nodeAddressEnc = pNodesAddressList[ elemNum ];
			nodeAddress = GvStructure::VolTreeNodeAddress::unpackAddress( nodeAddressEnc ).x;

			// Compute element address
			const uint elemIndexEnc = pElemAddressList[ elemNum ];
			const ElemAddressType elemIndex = TPageTableType::ElemType::unpackAddress( elemIndexEnc );
			elemAddress = elemIndex * TElementRes::get(); // convert into node address             ===> NOTE : for bricks, the resolution holds the border !!!

			// Get the localization of the current element
			//parentLocInfo = pPageTable.getLocalizationInfo( elemNum );
			parentLocInfo = pPageTable.getLocalizationInfo( nodeAddress );
		}

		// Thread Synchronization
		__syncthreads();

		// Produce data
#ifndef GV_USE_PRODUCTION_OPTIMIZATION_INTERNAL
		// Shared Memory declaration
		__shared__ uint producerFeedback;	// Shared Memory declaration
		producerFeedback = pGpuProvider.produceData( pGpuPool, elemNum, processID, elemAddress, parentLocInfo ); // TODO <= This can't work
		// Thread Synchronization
		__syncthreads();
#else
		// Optimization
		// - remove this synchonization for brick production
		// - let user-defined synchronization barriers in the producer directly
		uint producerFeedback = pGpuProvider.produceData( pGpuPool, elemNum, processID, elemAddress, parentLocInfo );
#endif

		// Note : for "nodes", producerFeedback is un-un-used for the moment

		// Update page table (who manages localisation information)
		//
		// Done by only one thread of the kernel
		if ( processID == 0 )
		{
			pPageTable.setPointer( nodeAddress, elemAddress, producerFeedback );
		}
	}
}

/******************************************************************************
 * KERNEL GvKernel_genericWriteIntoCache_NoSynchronization
 *
 * This method is a helper for writing into the cache.
 *
 * @param pNumElems The number of elements we need to produce and write.
 * @param pNodesAddressList buffer of element addresses that producer(s) has to process (subdivide or produce/load)
 * @param pElemAddressList buffer of available element addresses in cache where producer(s) can write
 * @param pGpuPool The pool where we will write the produced elements.
 * @param pGpuProvider The provider called for the production.
 * @param pPageTable page table used to retrieve localization information from element addresses
 ******************************************************************************/
template< typename TElementRes, typename TGPUPoolType, typename TGPUProviderType, typename TPageTableType >
__global__
// TO DO : Prolifing / Optimization
// - use "Launch Bounds" feature to profile / optimize code
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GvKernel_genericWriteIntoCache_NoSynchronization( const uint pNumElems, uint* pNodesAddressList, uint* pElemAddressList,
						    TGPUPoolType pGpuPool, TGPUProviderType pGpuProvider, TPageTableType pPageTable )
{
	// Retrieve global indexes
	const uint elemNum = blockIdx.x;
	const uint processID = threadIdx.x + __uimul( threadIdx.y, blockDim.x ) + __uimul( threadIdx.z, __uimul( blockDim.x, blockDim.y ) );

	// Check bound
	if ( elemNum < pNumElems )
	{
		// Clean the syntax a bit
		typedef typename TPageTableType::ElemAddressType ElemAddressType;

		// Shared Memory declaration
		/*__shared__*/ uint nodeAddress;
		__shared__ ElemAddressType elemAddress;
		__shared__ GvCore::GsLocalizationInfo parentLocInfo;

		// Retrieve node address and its localisation info along with new element address
		//
		// Done by only one thread of the kernel
		if ( processID == 0 )
		{
			// Compute node address
			const uint nodeAddressEnc = pNodesAddressList[ elemNum ];
			nodeAddress = GvStructure::VolTreeNodeAddress::unpackAddress( nodeAddressEnc ).x;

			// Compute element address
			const uint elemIndexEnc = pElemAddressList[ elemNum ];
			const ElemAddressType elemIndex = TPageTableType::ElemType::unpackAddress( elemIndexEnc );
			elemAddress = elemIndex * TElementRes::get(); // convert into node address             ===> NOTE : for bricks, the resolution holds the border !!!

			// Get the localization of the current element
			//parentLocInfo = pPageTable.getLocalizationInfo( elemNum );
			parentLocInfo = pPageTable.getLocalizationInfo( nodeAddress );
		}

		// Produce data
		uint producerFeedback = pGpuProvider.produceData( pGpuPool, elemNum, processID, elemAddress, parentLocInfo );

		// Update page table (who manages localisation information)
		//
		// Done by only one thread of the kernel
		if ( processID == 0 )
		{
			pPageTable.setPointer( nodeAddress, elemAddress, producerFeedback );
		}
	}
}

} // namespace GvCache

#endif
