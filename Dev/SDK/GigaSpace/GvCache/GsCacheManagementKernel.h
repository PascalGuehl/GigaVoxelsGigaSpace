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

#ifndef _GV_CACHE_MANAGEMENT_KERNEL_H_
#define _GV_CACHE_MANAGEMENT_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvStructure/GsVolumeTreeAddressType.h"
#include "GvRendering/GsRendererContext.h"

// Cuda
#include <host_defines.h>

/******************************************************************************
 ***************************** KERNEL DEFINITION ******************************
 ******************************************************************************/

// NOTE :
// can't use a GsCacheManagementKernel.cu file, it does not work cause "constant" is not the same in different compilation units...

namespace GvCache
{

/******************************************************************************
 * GvKernel_NodeCacheManagerFlagTimeStamps kernel
 *
 * This kernel creates the usage mask list of used and non used elements (in current rendering pass) in a single pass
 *
 * @param pCacheManager Cache manager
 * @param pNumElem Number of elememts to process
 * @param pTimeStampsElemAddressList Timestamp buffer list
 * @param pTempMaskList Resulting temporary mask list of non-used elements
 * @param pTempMaskList2 Resulting temporary mask list of used elements
 ******************************************************************************/
__global__
GIGASPACE_EXPORT void GvKernel_NodeCacheManagerFlagTimeStamps( const unsigned int pNbElements
											, const unsigned int* __restrict__ pSortedElements, const unsigned int* __restrict__ pTimestamps
											, unsigned int* __restrict__ pUnusedElementMasks, unsigned int* __restrict__ pUsedElementMasks );
///**
// * ...
// */
//__global__
///*GIGASPACE_EXPORT*/ void GvKernel_DataCacheManagerFlagTimeStamps( const unsigned int pNbElements
//											, const unsigned int* /*__restrict__*/ pSortedElements, const unsigned int* /*__restrict__*/ pTimestamps
//											, unsigned int* /*__restrict__*/ pUnusedElementMasks, unsigned int* /*__restrict__*/ pUsedElementMasks
//											, const unsigned int pResolution, const unsigned int pPitch );

} // namespace GvCache

namespace GvCache
{

/******************************************************************************
 * GvKernel_NodeCacheManagerFlagTimeStamps kernel
 *
 * This kernel creates the usage mask list of used and non used elements (in current rendering pass) in a single pass
 *
 * @param pCacheManager Cache manager
 * @param pNumElem Number of elememts to process
 * @param pTimeStampsElemAddressList Timestamp buffer list
 * @param pTempMaskList Resulting temporary mask list of non-used elements
 * @param pTempMaskList2 Resulting temporary mask list of used elements
 ******************************************************************************/
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GvKernel_NodeCacheManagerFlagTimeStamps( const unsigned int pNbElements
											, const unsigned int* __restrict__ pSortedElements, const unsigned int* __restrict__ pTimestamps
											, unsigned int* __restrict__ pUnusedElementMasks, unsigned int* __restrict__ pUsedElementMasks )
{
	// Retrieve global data index
	const unsigned int lineSize = __uimul( blockDim.x, gridDim.x );
	const unsigned int index = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Check bounds
	if ( index < pNbElements )
	{
		// Retrieve element processed by current thread
		const unsigned int elementIndex = pSortedElements[ index ];

		// Check element's timestamp and set associated masks accordingly
		if ( pTimestamps[ elementIndex ] == k_currentTime )
		{
			pUnusedElementMasks[ index ] = 0;
			pUsedElementMasks[ index ] = 1;
		}
		else
		{
			pUnusedElementMasks[ index ] = 1;
			pUsedElementMasks[ index ] = 0;
		}
	}
}

///**
// * ...
// */
//__global__
//void GvKernel_DataCacheManagerFlagTimeStamps( const unsigned int pNbElements
//											, const unsigned int* /*__restrict__*/ pSortedElements, const unsigned int* /*__restrict__*/ pTimestamps
//											, unsigned int* /*__restrict__*/ pUnusedElementMasks, unsigned int* /*__restrict__*/ pUsedElementMasks
//											, const unsigned int pResolution, const unsigned int pPitch )
//{
//	// Retrieve global data index
//	const unsigned int lineSize = __uimul( blockDim.x, gridDim.x );
//	const unsigned int index = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );
//
//	/*__shared__ unsigned int resolution;
//	if ( threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0 )
//	{
//		resolution = pResolution;
//	}
//	__syncthreads();*/
//
//	// Check bounds
//	if ( index < pNbElements )
//	{
//		// Retrieve element processed by current thread
//		const unsigned int elementPackedAddress = pSortedElements[ index ];
//
//		// Unpack its address
//		const uint3 elemAddress = GvStructure::VolTreeBrickAddress::unpackAddress( elementPackedAddress );
//
//		// Retrieve element processed by current thread
//		const unsigned int elementIndex = elemAddress.x + __uimul( elemAddress.y, /*resolution*/pResolution ) + __uimul( elemAddress.z, /*resolution*/pPitch );
//	//	const unsigned int elementIndex = ( elemAddress.z * 28/*resolution*/ * 4/*sizeof( unsigned int )*/ ) + ( ( elemAddress.y * 28/*resolution*/ ) + elemAddress.x );
//
//		// Check element's timestamp and set associated masks accordingly
//		if ( pTimestamps[ elementIndex ] == k_currentTime )
//		{
//			pUnusedElementMasks[ index ] = 0;
//			pUsedElementMasks[ index ] = 1;
//		}
//		else
//		{
//			pUnusedElementMasks[ index ] = 1;
//			pUsedElementMasks[ index ] = 0;
//		}
//	}
//}

} // namespace GvCache

#endif // !_GV_CACHE_MANAGEMENT_KERNEL_H_
