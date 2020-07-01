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
#include "GvCore/GsIProviderKernel.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{

/******************************************************************************
 * Constructor
 *
 * @param pDataStructure data structure
 ******************************************************************************/
template< typename TKernelProducerType, typename TDataStructureType, typename TDataProductionManager >
inline GsSimpleHostProducer< TKernelProducerType, TDataStructureType, TDataProductionManager >
::GsSimpleHostProducer()
:	GvCore::GsProvider< TDataStructureType, TDataProductionManager >()
,	_kernelProducer()
,	_cacheHelper()
,	_nodePageTable( NULL )
,	_dataPageTable( NULL )
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TKernelProducerType, typename TDataStructureType, typename TDataProductionManager >
inline GsSimpleHostProducer< TKernelProducerType, TDataStructureType, TDataProductionManager >
::~GsSimpleHostProducer()
{
	finalize();
}

/******************************************************************************
 * Initialize
 *
 * @param pDataStructure data structure
 * @param pDataProductionManager data production manager
 ******************************************************************************/
template< typename TKernelProducerType, typename TDataStructureType, typename TDataProductionManager >
inline void GsSimpleHostProducer< TKernelProducerType, TDataStructureType, TDataProductionManager >
::initialize( GvStructure::GsIDataStructure* pDataStructure, GvStructure::GsIDataProductionManager* pDataProductionManager )
{
	// Call parent class
	GvCore::GsProvider< TDataStructureType, TDataProductionManager >::initialize( pDataStructure, pDataProductionManager );

	// TO DO
	// - use virtual method : useDataStructureOnDevice() ?
	_kernelProducer.initialize( this->_dataStructure->volumeTreeKernel );

	// Specialization
	_nodePageTable = this->_dataProductionManager->getNodesCacheManager()->_pageTable;
	_dataPageTable = this->_dataProductionManager->getBricksCacheManager()->_pageTable;
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
template< typename TKernelProducerType, typename TDataStructureType, typename TDataProductionManager >
inline void GsSimpleHostProducer< TKernelProducerType, TDataStructureType, TDataProductionManager >
::finalize()
{
}

/******************************************************************************
 * This method is called by the cache manager when you have to produce data for a given pool.
 * Implement the produceData method for the channel 0 (nodes)
 *
 * @param pNumElems the number of elements you have to produce.
 * @param pNodeAddressCompactList a list containing the addresses of the numElems nodes concerned.
 * @param pElemAddressCompactList a list containing numElems addresses where you need to store the result.
 * @param Loki::Int2Type< 0 > corresponds to the index of the node pool
 ******************************************************************************/
template< typename TKernelProducerType, typename TDataStructureType, typename TDataProductionManager >
inline void GsSimpleHostProducer< TKernelProducerType, TDataStructureType, TDataProductionManager >
::produceData( uint pNumElems,
				GvCore::GsLinearMemory< uint >* pNodesAddressCompactList,
				GvCore::GsLinearMemory< uint >* pElemAddressCompactList,
				Loki::Int2Type< 0 > )
{
	/*printf( "---- GPUCacheManager< 0 > : %d requests ----\n", pNumElems );

	for ( size_t i = 0; i < pNumElems; ++i )
	{
		uint nodeAddressEnc = (*pNodesAddressCompactList)[ i ];
		uint3 nodeAddress = GvStructure::GsNode::unpackNodeAddress( nodeAddressEnc );
		printf( " request %d: 0x%x (%d, %d, %d)\n", i, nodeAddressEnc, nodeAddress.x, nodeAddress.y, nodeAddress.z );
	}*/

	// Initialize the device-side producer
	GvCore::GsIProviderKernel< 0, TKernelProducerType > kernelProvider( _kernelProducer );

	// Define kernel block size
	const uint3 kernelBlockSize = TKernelProducerType::NodesKernelBlockSize::get();
	const dim3 blockSize( kernelBlockSize.x, kernelBlockSize.y, kernelBlockSize.z );

	// Retrieve updated addresses
	//uint* nodesAddressList = pNodesAddressCompactList->getPointer();
	uint* nodesAddressList = pNodesAddressCompactList->getPointer() + this->_productionInfo._offset;
	//uint* elemAddressList = pElemAddressCompactList->getPointer();
	uint* elemAddressList = pElemAddressCompactList->getPointer() + this->_productionInfo._offset;
	
	// Call cache helper to write into cache
	//
	// - this function call encapsulates a kernel launch to produce data on device
	// - i.e. the associated device-side producer will call its device function "ProducerKernel::produceData< 0 >()"
	NodePoolType* pool = this->_nodePool;
	NodePageTableType* pageTable = _nodePageTable;
	_cacheHelper.template genericWriteIntoCache< NodeTileResLinear >( pNumElems, nodesAddressList, elemAddressList, pool, kernelProvider, pageTable, blockSize );
}

/******************************************************************************
 * This method is called by the cache manager when you have to produce data for a given pool.
 * Implement the produceData method for the channel 1 (bricks)
 *
 * @param pNumElems the number of elements you have to produce.
 * @param pNodeAddressCompactList a list containing the addresses of the pNumElems nodes concerned.
 * @param pElemAddressCompactList a list containing pNumElems addresses where you need to store the result.
 * @param Loki::Int2Type< 1 > corresponds to the index of the brick pool
 ******************************************************************************/
template< typename TKernelProducerType, typename TDataStructureType, typename TDataProductionManager >
inline void GsSimpleHostProducer< TKernelProducerType, TDataStructureType, TDataProductionManager >
::produceData( uint pNumElems,
				GvCore::GsLinearMemory< uint >* pNodesAddressCompactList,
				GvCore::GsLinearMemory< uint >* pElemAddressCompactList,
				Loki::Int2Type< 1 > )
{
	/*printf( "---- GPUCacheManager< 1 > : %d requests ----\n", pNumElems );

	for ( size_t i = 0; i < pNumElems; ++i )
	{
		uint nodeAddressEnc = (*pNodesAddressCompactList)[ i ];
		uint3 nodeAddress = GvStructure::GsNode::unpackNodeAddress( nodeAddressEnc );
		printf( " request %d: 0x%x (%d, %d, %d)\n", i, nodeAddressEnc, nodeAddress.x, nodeAddress.y, nodeAddress.z );
	}*/

	// Initialize the device-side producer
	GvCore::GsIProviderKernel< 1, TKernelProducerType > kernelProvider( _kernelProducer );

	// Define kernel block size
	const uint3 kernelBlockSize = TKernelProducerType::BricksKernelBlockSize::get();
	const dim3 blockSize( kernelBlockSize.x, kernelBlockSize.y, kernelBlockSize.z );

	// Retrieve updated addresses
	//uint* nodesAddressList = pNodesAddressCompactList->getPointer();
	uint* nodesAddressList = pNodesAddressCompactList->getPointer() + this->_productionInfo._offset;
	//uint* elemAddressList = pElemAddressCompactList->getPointer();
	uint* elemAddressList = pElemAddressCompactList->getPointer() + this->_productionInfo._offset;

	// Call cache helper to write into cache
	//
	// - this function call encapsulates a kernel launch to produce data on device
	// - i.e. the associated device-side producer will call its device function "ProducerKernel::produceData< 1 >()"
	DataPoolType* pool = this->_dataPool;
	DataPageTableType* pageTable = _dataPageTable;
	_cacheHelper.template genericWriteIntoCache< BrickFullRes >( pNumElems, nodesAddressList, elemAddressList, pool, kernelProvider, pageTable, blockSize );
}

} // namespace GvUtils
