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

// GigaVoxels
#include "GvCore/GsError.h"

// STL
#include <algorithm>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Create localization info list (code and depth)
 *
 * Given a list of N nodes, it retrieves their localization info (code + depth)
 *
 * @param pNumElems number of elements to process (i.e. nodes)
 * @param pNodesAddressCompactList a list of nodes from which to retrieve localization info
 * @param pResLocCodeList the resulting localization code array of all requested elements
 * @param pResLocDepthList the resulting localization depth array of all requested elements
 ******************************************************************************/
template< typename NodeTileRes, typename LocCodeArrayType, typename LocDepthArrayType >
inline void PageTable< NodeTileRes, LocCodeArrayType, LocDepthArrayType >
::createLocalizationLists( uint pNumElems, uint* pNodesAddressCompactList,
							thrust::device_vector< GsLocalizationInfo::CodeType >* pResLocCodeList,
							thrust::device_vector< GsLocalizationInfo::DepthType >* pResLocDepthList )
{
	// Set kernel execution configuration
	dim3 blockSize( 64, 1, 1 );
	uint numBlocks = iDivUp( pNumElems, blockSize.x );
	dim3 gridSize = dim3( std::min( numBlocks, 65535U ), iDivUp( numBlocks, 65535U ), 1 );

	// Launch kernel
	//
	// Create the lists containing localization and depth of each element.
	CreateLocalizationLists< NodeTileRes >
			<<< gridSize, blockSize, 0 >>>( /*in*/pNumElems, /*in*/pNodesAddressCompactList,
											/*in*/locCodeArray->getPointer(), /*in*/locDepthArray->getPointer(),
											/*out*/thrust::raw_pointer_cast( &( *pResLocCodeList )[ 0 ] ), 
											/*out*/thrust::raw_pointer_cast( &( *pResLocDepthList )[ 0 ] ) );

	GV_CHECK_CUDA_ERROR( "CreateLocalizationLists" );
}

} // namespace GvCore

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Get the associated device-side object
 *
 * @return the associated device-side object
 ******************************************************************************/
template< typename NodeTileRes, typename ElementRes, typename AddressType, typename KernelArrayType, typename LocCodeArrayType, typename LocDepthArrayType >
inline typename PageTableNodes< NodeTileRes, ElementRes, AddressType, KernelArrayType, LocCodeArrayType, LocDepthArrayType >::KernelType& PageTableNodes< NodeTileRes, ElementRes, AddressType, KernelArrayType, LocCodeArrayType, LocDepthArrayType >
::getKernel()
{
	return pageTableKernel;
}
	
} // namespace GvCore

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Get the associated device-side object
 *
 * @return the associated device-side object
 ******************************************************************************/
template< typename NodeTileRes, typename ChildAddressType, typename ChildKernelArrayType, typename DataAddressType, typename DataKernelArrayType, typename LocCodeArrayType, typename LocDepthArrayType >
inline typename PageTableBricks< NodeTileRes, ChildAddressType, ChildKernelArrayType, DataAddressType, DataKernelArrayType, LocCodeArrayType, LocDepthArrayType >::KernelType& PageTableBricks< NodeTileRes, ChildAddressType, ChildKernelArrayType, DataAddressType, DataKernelArrayType, LocCodeArrayType, LocDepthArrayType >
::getKernel()
{
	return pageTableKernel;
}
	
} // namespace GvCore
