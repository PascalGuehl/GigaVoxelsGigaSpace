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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCache
{

/******************************************************************************
 * Update timestamp usage information of an element (node tile or brick)
 * with current time (i.e. current rendering pass)
 * given its address in its corresponding pool (node or brick).
 *
 * @param pElemAddress The address of the element for which we want to update usage information
 ******************************************************************************/
template< class ElementRes, class AddressType >
__device__
__forceinline__ void GsCacheManagerKernel< ElementRes, AddressType >::setElementUsage( uint pElemAddress )
{
	uint elemOffset;
	if ( ElementRes::xIsPOT )
	{
		elemOffset = pElemAddress >> ElementRes::xLog2;
	}
	else
	{
		elemOffset = pElemAddress / ElementRes::x;
	}

	// Update time stamp array with current time (i.e. the time of the current rendering pass)
	_timeStampArray.set( elemOffset, k_currentTime );
}

/******************************************************************************
 * Update timestamp usage information of an element (node tile or brick)
 * with current time (i.e. current rendering pass)
 * given its address in its corresponding pool (node or brick).
 *
 * @param pElemAddress The address of the element for which we want to update usage information
 ******************************************************************************/
template< class ElementRes, class AddressType >
__device__
__forceinline__ void GsCacheManagerKernel< ElementRes, AddressType >::setElementUsage( uint3 pElemAddress )
{
	uint3 elemOffset;
	if ( ElementRes::xIsPOT && ElementRes::yIsPOT && ElementRes::zIsPOT )
	{
		elemOffset.x = pElemAddress.x >> ElementRes::xLog2;
		elemOffset.y = pElemAddress.y >> ElementRes::yLog2;
		elemOffset.z = pElemAddress.z >> ElementRes::zLog2;
	}
	else
	{
		elemOffset = pElemAddress / ElementRes::get();
	}

	// Update time stamp array with current time
	_timeStampArray.set( elemOffset, k_currentTime );
}

} // namespace GvCache

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
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
 *
 * TODO :
 * - Replace "GsCacheManagerKernel< ElementRes, AddressType > pCacheManager" and templates
 *   by a simple array like "const uint* __restrict__ pArray"
 ******************************************************************************/
template< class ElementRes, class AddressType >
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void GsKernel_CacheManager_retrieveElementUsageMasks( GsCacheManagerKernel< ElementRes, AddressType > pCacheManager,
								  const uint pNbElements, const uint* __restrict__ pTimeStampsElemAddressList,
								  uint* __restrict__ pUnusedElementMasks, uint* __restrict__ pUsedElementMasks )
{
	// Retrieve global data index
	const uint lineSize = __uimul( blockDim.x, gridDim.x );
	const uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Check bounds
	if ( elem < pNbElements )
	{
		// Retrieve element processed by current thread
		const uint elemAddressEnc = pTimeStampsElemAddressList[ elem ];

		// Unpack its address
		const uint3 elemAddress = AddressType::unpackAddress( elemAddressEnc );

		// Check timestamp
		if ( pCacheManager._timeStampArray.get( elemAddress ) == k_currentTime )
		{
			pUnusedElementMasks[ elem ] = 0;
			pUsedElementMasks[ elem ] = 1;
		}
		else
		{
			pUnusedElementMasks[ elem ] = 1;
			pUsedElementMasks[ elem ] = 0;
		}
	}
}

} // namespace GvCache
