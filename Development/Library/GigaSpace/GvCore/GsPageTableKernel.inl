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
#include "GvStructure/GsNode.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

// TO DO : attention ce KERNEL n'est pas dans un namespace...
/******************************************************************************
 * KERNEL : CreateLocalizationLists
 *
 * Extract localization informations associated with a list of given elements.
 *
 * Special version for gpuProducerLoadCache that transform loccodes to usable ones (warning, loose one bit of depth !)
 *
 * @param pNbElements number of elements to process
 * @param pLoadAddressList List of input node addresses
 * @param pLocCodeList List of localization code coming from the main page table of the data structure and referenced by cache managers (nodes and bricks)
 * @param pLocDepthList List of localization depth coming from the main page table of the data structure and referenced by cache managers (nodes and bricks)
 * @param pResLocCodeList Resulting output localization code list
 * @param pResLocDepthList Resulting output localization depth list
 ******************************************************************************/
template< class NodeTileRes >
__global__
void CreateLocalizationLists( const uint pNbElements, const uint* pLoadAddressList, 
							 const GvCore::GsLocalizationInfo::CodeType* pLocCodeList, const GvCore::GsLocalizationInfo::DepthType* pLocDepthList,
							 GvCore::GsLocalizationInfo::CodeType* pResLocCodeList, GvCore::GsLocalizationInfo::DepthType* pResLocDepthList )
{
	// Retrieve global indexes
	const uint lineSize = __uimul( blockDim.x, gridDim.x );
	const uint elem = threadIdx.x + __uimul( blockIdx.x, blockDim.x ) + __uimul( blockIdx.y, lineSize );

	// Check bounds
	if ( elem < pNbElements )
	{
		// Retrieve node address and its localisation info
		// along with new element address

		// Retrieve node address
		const uint nodeAddressEnc = pLoadAddressList[ elem ];
		const uint3 nodeAddress = GvStructure::GsNode::unpackNodeAddress( nodeAddressEnc );

		// Retrieve its "node tile" address
		const uint nodeTileAddress = nodeAddress.x / NodeTileRes::getNumElements();

		// Retrieve its "localization info"
		const GvCore::GsLocalizationInfo::CodeType *tileLocCodeEnc = &pLocCodeList[ nodeTileAddress ];

		// Linear offset of the node in the node tile
		const uint linearOffset = nodeAddress.x - ( nodeTileAddress * NodeTileRes::getNumElements() );
		// 3D offset of the node in the node tile
		const uint3 nodeOffset = NodeTileRes::toFloat3( linearOffset );

		const GvCore::GsLocalizationInfo::CodeType nodeLocCodeEnc = tileLocCodeEnc->addLevel< NodeTileRes >( nodeOffset );

		// Write localization info
		pResLocDepthList[ elem ] = pLocDepthList[ nodeTileAddress ];
		pResLocCodeList[ elem ] = nodeLocCodeEnc;
	}
}

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * This method returns the LocalizationInfo structure associated with the given
 * node address.
 *
 * @param nodeAddress ...
 *
 * @return ...
 ******************************************************************************/
template< typename Derived, typename AddressType, typename KernelArrayType >
__device__
__forceinline__ GsLocalizationInfo PageTableKernel< Derived, AddressType, KernelArrayType >
::getLocalizationInfo( uint nodeAddress ) const
{
	return static_cast< const Derived* >( this )->getLocalizationInfoImpl( nodeAddress );
}

/******************************************************************************
 * This method should...
 *
 * @param elemAddress ...
 * @param elemPointer ...
 * @param flag ...
 ******************************************************************************/
template< typename Derived, typename AddressType, typename KernelArrayType >
__device__
__forceinline__ void PageTableKernel< Derived, AddressType, KernelArrayType >
::setPointer( uint elemAddress, uint3 elemPointer, uint flag )
{
	static_cast< Derived* >( this )->setPointerImpl( elemAddress, elemPointer, flag );
}

} // namespace GvCore

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Return the localization info of a node in the node pool
 *
 * @param nodeAddress Address of the node in the node pool
 *
 * @return The localization info of the node
 ******************************************************************************/
template< typename NodeTileRes, typename ElementRes, typename AddressType, typename KernelArrayType, typename LocCodeArrayType, typename LocDepthArrayType >
__device__
__forceinline__ GsLocalizationInfo PageTableNodesKernel< NodeTileRes, ElementRes, AddressType, KernelArrayType, LocCodeArrayType, LocDepthArrayType >
::getLocalizationInfoImpl( uint nodeAddress ) const
{
	// Compute the address of the current node tile (and its offset in the node tile)
	uint nodeTileIndex = nodeAddress / NodeTileRes::getNumElements();
	uint nodeTileAddress = nodeTileIndex * NodeTileRes::getNumElements();
	uint nodeTileOffset = nodeAddress - nodeTileAddress;

	// Compute the node offset (in 3D, in the node tile)
	uint3 nodeOffset = NodeTileRes::toFloat3( nodeTileOffset );

	// Fetch associated localization infos
	GsLocalizationInfo::CodeType parentLocCode = locCodeArray.get( nodeTileIndex );
	GsLocalizationInfo::DepthType parentLocDepth = locDepthArray.get( nodeTileIndex );

	// Localization info initialization
	GsLocalizationInfo locInfo;
	locInfo.locCode = parentLocCode.addLevel< NodeTileRes >( nodeOffset );
	locInfo.locDepth = parentLocDepth;

	return locInfo;
}

/******************************************************************************
 * ...
 *
 * @param elemAddress ...
 * @param elemPointer ...
 * @param flags ...
 ******************************************************************************/
template< typename NodeTileRes, typename ElementRes, typename AddressType, typename KernelArrayType, typename LocCodeArrayType, typename LocDepthArrayType >
__device__
__forceinline__ void PageTableNodesKernel< NodeTileRes, ElementRes, AddressType, KernelArrayType, LocCodeArrayType, LocDepthArrayType >
::setPointerImpl( uint elemAddress, ElemAddressType elemPointer, uint flags )
{
	ElemPackedAddressType packedChildAddress	= childArray.get( elemAddress );
	ElemPackedAddressType packedAddress			= AddressType::packAddress( elemPointer );

	// Update node tile's pointer
	childArray.set( elemAddress,
		/*remove the fact that a node could be terminal*/( packedChildAddress & 0x40000000 ) | /*add children's nodetile address*/( packedAddress & 0x3FFFFFFF ) );

	// Compute the address of the current node tile
	uint nodeTileIndex = elemAddress / NodeTileRes::getNumElements();
	uint nodeTileAddress = nodeTileIndex * NodeTileRes::getNumElements();
	uint nodeTileOffset = elemAddress - nodeTileAddress;

	// Compute the node offset
	uint3 nodeOffset = NodeTileRes::toFloat3( nodeTileOffset );

	// Fetch associated localization infos
	GsLocalizationInfo::CodeType parentLocCode = locCodeArray.get( nodeTileIndex );
	GsLocalizationInfo::DepthType parentLocDepth = locDepthArray.get( nodeTileIndex );

	// Compute the address of the new node tile
	uint newNodeTileIndex = elemPointer.x / ElementRes::getNumElements();
	//uint newNodeTileAddress = newNodeTileIndex * ElementRes::getNumElements();	// --> semble ne pas être utilisé ?

	// Update associated localization infos
	GsLocalizationInfo::CodeType newLocCode = parentLocCode.addLevel< NodeTileRes >( nodeOffset );
	GsLocalizationInfo::DepthType newLocDepth = parentLocDepth.addLevel();

	locCodeArray.set( newNodeTileIndex, newLocCode );
	locDepthArray.set( newNodeTileIndex, newLocDepth );
}

} // namespace GvCore

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Return the localization info of a node in the node pool
 *
 * @param nodeAddress Address of the node in the node pool
 *
 * @return The localization info of the node
 ******************************************************************************/
template< typename NodeTileRes, typename ChildAddressType, typename ChildKernelArrayType, typename DataAddressType, typename DataKernelArrayType, typename LocCodeArrayType, typename LocDepthArrayType >
__device__
__forceinline__ GsLocalizationInfo PageTableBricksKernel< NodeTileRes, ChildAddressType, ChildKernelArrayType, DataAddressType, DataKernelArrayType, LocCodeArrayType, LocDepthArrayType >
::getLocalizationInfoImpl( uint nodeAddress ) const
{
	// Compute the address of the current node tile (and its offset in the node tile)
	uint nodeTileIndex = nodeAddress / NodeTileRes::getNumElements();
	uint nodeTileAddress = nodeTileIndex * NodeTileRes::getNumElements();
	uint nodeTileOffset = nodeAddress - nodeTileAddress;

	// Compute the node offset (in 3D, in the node tile)
	uint3 nodeOffset = NodeTileRes::toFloat3( nodeTileOffset );

	// Fetch associated localization infos
	GsLocalizationInfo::CodeType parentLocCode = locCodeArray.get( nodeTileIndex );
	GsLocalizationInfo::DepthType parentLocDepth = locDepthArray.get( nodeTileIndex );

	// Localization info initialization
	GsLocalizationInfo locInfo;
	locInfo.locCode = parentLocCode.addLevel< NodeTileRes >( nodeOffset );
	locInfo.locDepth = parentLocDepth;

	return locInfo;
}

/******************************************************************************
 * ...
 *
 * @param ...
 * @param ...
 * @param flags this vlaue is retrieves from Producer::produceData< 1 > methods)
 ******************************************************************************/
template< typename NodeTileRes, typename ChildAddressType, typename ChildKernelArrayType, typename DataAddressType, typename DataKernelArrayType, typename LocCodeArrayType, typename LocDepthArrayType >
__device__
__forceinline__ void PageTableBricksKernel< NodeTileRes, ChildAddressType, ChildKernelArrayType, DataAddressType, DataKernelArrayType, LocCodeArrayType, LocDepthArrayType >
::setPointerImpl( uint elemAddress, ElemAddressType elemPointer, uint flags )
{
	// XXX: Should be removed
	ElemAddressType brickPointer = elemPointer + make_uint3( 1 ); // Warning: fixed border size !	=> QUESTION ??

	PackedChildAddressType packedChildAddress	= childArray.get( elemAddress );
	ElemPackedAddressType packedBrickAddress	= DataAddressType::packAddress( brickPointer );

	// We store brick
	packedChildAddress |= 0x40000000;

	// Check flags value and modify address accordingly.
	// If flags is greater than 0, it means that the node containing the brick is terminal
	if ( flags > 0 )
	{
		// If flags equals 2, it means that the brick is empty
		if ( flags == 2 )
		{
			// Empty brick flag
			packedBrickAddress = 0;
			packedChildAddress &= 0xBFFFFFFF;
		}

		// Terminal flag
		packedChildAddress |= 0x80000000;
	}

	childArray.set( elemAddress, packedChildAddress );
	dataArray.set( elemAddress, packedBrickAddress );
}

} // namespace GvCore
