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
// #include "GvCache/GsCacheManagementKernel.h"
#include "GsCompute/GsDataParallelPrimitives.h"

// System
#include <cassert>

// STL
#include <algorithm>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

namespace GvCache
{

/**
 * In the GigaSpace engine, Cache management requires to :
 * - protect "null" reference (element address)
 * - root nodes in the data structure (i.e. octree, etc...)
 * So, each array managed by the Cache needs to take care of these particular elements.
 *
 * Note : still a bug when too much loading - TODO: check this
 */
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
#ifndef GS_USE_MULTI_OBJECTS
const uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >::_cNbLockedElements = 1/*null reference*/ + 1/*root node*/;
#else
const uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >::_cNbLockedElements = 1/*null reference*/ + 1/*root node*/ + 1/*add 1 for each additional object*/;
#endif // TODO : use _cNbLockedElements where its mandatory with an accessor that will add number of objects automatically !!!

} // namespace GvCache

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCache
{

/******************************************************************************
 * Constructor
 *
 * @param pCachesize size of the cache
 * @param pPageTableArray the array of elements that the cache has to managed
 * @param pGraphicsInteroperability a flag used to map buffers to OpenGL graphics library
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::GsCacheElementManager( const uint3& pCachesize, PageTableArrayType* pPageTableArray, uint pGraphicsInteroperability )
:	GsCore::GsISerializable()
,	_cacheSize( pCachesize )
,	_policy( ePreventReplacingUsedElementsPolicy )
,	_exceededCapacity( false )
{
	// Compute elements cache size
	_elemsCacheSize = _cacheSize / ElementRes::get();

	// Page table initialization
	_d_pageTableArray = pPageTableArray;
	_pageTable = new PageTableType();

	// Initialize the timestamp buffer
	_d_TimeStampArray = new GvCore::GsLinearMemory< uint >( _elemsCacheSize, pGraphicsInteroperability );
	_d_TimeStampArray->fill( 0 );

	this->_numElements = _elemsCacheSize.x * _elemsCacheSize.y * _elemsCacheSize.z - _cNbLockedElements;

	_numElemsNotUsed = _numElements;

	// List of elements in the cache
	_d_elemAddressList = new GvCore::GsLinearMemory< uint >( make_uint3( this->_numElements, 1, 1 ) );
	_d_elemAddressListTmp = new GvCore::GsLinearMemory< uint >( make_uint3( this->_numElements, 1, 1 ) );

	uint3 pageTableRes = _d_pageTableArray->getResolution();
	uint pageTableResLinear = pageTableRes.x * pageTableRes.y * pageTableRes.z;

	// Buffer of requests (masks of associated request in the current frame)
	//_d_TempUpdateMaskList = GsCacheManagerResources::getTempUsageMask1( pageTableRes.x * pageTableRes.y  *pageTableRes.z );
	// TODO :  check if this temporary buffer could be stored only one times in the wrapper class (data production and cache magement)
	_d_TempUpdateMaskList = new GvCore::GsLinearMemory< uint >( make_uint3( pageTableResLinear, 1, 1 ) );
	_d_UpdateCompactList = new GvCore::GsLinearMemory< uint >( make_uint3( pageTableResLinear, 1, 1 ) );

	// Init
	//
	// Allocate size to speed up insert without need for push_back
	GvCore::Array3D< uint > tmpelemaddress( make_uint3( _elemsCacheSize.x * _elemsCacheSize.y * _elemsCacheSize.z, 1, 1 ) );
	uint3 pos;
	uint index = 0;
	for ( pos.z = 0; pos.z < _elemsCacheSize.z; pos.z++ )
	for ( pos.y = 0; pos.y < _elemsCacheSize.y; pos.y++ )
	for ( pos.x = 0; pos.x < _elemsCacheSize.x; pos.x++ )
	{
		tmpelemaddress.get( index ) = AddressType::packAddress( pos );
		index++;
	}
	// Dont use element zero !
	GvCore::memcpyArray( _d_elemAddressList, tmpelemaddress.getPointer() + _cNbLockedElements, this->_numElements );

	GS_CUDA_SAFE_CALL( cudaMalloc( (void**) &_d_numElementsPtr, sizeof( size_t ) ) );

	// The associated GPU side object receive a reference on the timestamp buffer
	_d_cacheManagerKernel._timeStampArray = _d_TimeStampArray->getDeviceArray();

	// LOG info
	std::cout << "\nCache Manager [ id " << Id::value << " ]" << std::endl;
	std::cout << "- cache size : " << _cacheSize << std::endl;
	std::cout << "- elements cache size : " << _elemsCacheSize << std::endl;
	std::cout << "- nb elements : " << this->_numElements << std::endl;
	std::cout << "- page table's linear resolution : " << pageTableResLinear << std::endl;
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::~GsCacheElementManager()
{
	GS_CUDA_SAFE_CALL( cudaFree( _d_numElementsPtr ) );
	
	// Page table
	delete _pageTable;
	_pageTable = NULL;

	delete _d_TimeStampArray;
	_d_TimeStampArray = NULL;

	delete _d_elemAddressList;
	_d_elemAddressList = NULL;
	delete _d_elemAddressListTmp;
	_d_elemAddressListTmp = NULL;

	delete _d_TempUpdateMaskList;
	_d_TempUpdateMaskList = NULL;
	delete _d_UpdateCompactList;
	_d_UpdateCompactList = NULL;
}

/******************************************************************************
 * Get the associated device side object
 *
 * @return the associated device side object
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
GsCacheManagerKernel< ElementRes, AddressType > GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getKernelObject()
{
	return	_d_cacheManagerKernel;
}

/******************************************************************************
 * Set the cache policy
 *
 * @param pPolicy the cache policy
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::setPolicy( GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >::ECachePolicy pPolicy )
{
	this->_policy = pPolicy;
}

/******************************************************************************
 * Get the cache policy
 *
 * @return the cache policy
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >::ECachePolicy GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getPolicy() const
{
	return this->_policy;
}

/******************************************************************************
 * Get the number of elements managed by the cache.
 *
 * @return
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getNumElements() const
{
	return this->_numElements;
}

/******************************************************************************
 * Get the number of elements managed by the cache.
 *
 * @return
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getNbUnusedElements() const
{
	return this->_numElemsNotUsed;
}

/******************************************************************************
 * Get the timestamp list of the cache.
 * There is as many timestamps as elements in the cache.
 *
 * @return ...
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
GvCore::GsLinearMemory< uint >* GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getTimeStampList() const
{
	return _d_TimeStampArray;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
GvCore::GsLinearMemory< uint >* GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getElementList() const
{
	return _d_elemAddressList;
}
/******************************************************************************
 * Set buffers of usages (masks of non-used and used elements in the current frame)
 *
 * @return ...
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::setElementMaskBuffers( GvCore::GsLinearMemory< uint >* pUnusedElementMaskBuffer, GvCore::GsLinearMemory< uint >* pUsedElementMaskBuffer )
{
	_d_UnusedElementMasksTemp = pUnusedElementMaskBuffer;
	_d_UsedElementMasksTemp = pUsedElementMaskBuffer;
}

/******************************************************************************
 * Update symbols
 * (variables in constant memory)
 ******************************************************************************/
//template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
//void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >::updateSymbols(){
//
//	CUDAPM_START_EVENT(gpucachemgr_updateSymbols);
//
//	/*GS_CUDA_SAFE_CALL(cudaMemcpyToSymbol(k_VTC_TimeStampArray,
//		(&_d_TimeStampArray->getDeviceArray()),
//		sizeof(_d_TimeStampArray->getDeviceArray()), 0, cudaMemcpyHostToDevice));*/
//	
//	CUDAPM_STOP_EVENT(gpucachemgr_updateSymbols);
//}

/******************************************************************************
 * Clear the cache
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::clearCache()
{
	// FIXME: do we really need to do it with a cuda kernel ?
	// Anyway, need to take account for the locked elements.
	//
	// Allocate size to speed up insert without need for push_back
	GvCore::Array3D< uint > tmpelemaddress( make_uint3( _elemsCacheSize.x * _elemsCacheSize.y * _elemsCacheSize.z, 1, 1 ) );
	uint3 pos;
	uint index = 0;
	for ( pos.z = 0; pos.z < _elemsCacheSize.z; pos.z++ )
	for ( pos.y = 0; pos.y < _elemsCacheSize.y; pos.y++ )
	for ( pos.x = 0; pos.x < _elemsCacheSize.x; pos.x++ )
	{
		tmpelemaddress.get( index ) = AddressType::packAddress( pos );
		index++;
	}
	// Don't use element zero !
	GvCore::memcpyArray( _d_elemAddressList, tmpelemaddress.getPointer() + _cNbLockedElements, this->_numElements );

	// Init
	//CUDAPM_START_EVENT( gpucachemgr_clear_cpyAddr )

	//// Uses kernel filling
	//uint* timeStampsElemAddressListPtr = thrust::raw_pointer_cast( &(*_d_elemAddressList)[ 0 ] );

	//const dim3 blockSize( 128, 1, 1 );
	//uint nbBlocks = iDivUp( this->_numElements, blockSize.x );
	//dim3 gridSize = dim3( std::min( nbBlocks, 65535U ), iDivUp( nbBlocks, 65535U ), 1 );

	//InitElemAddressList< AddressType ><<< gridSize, blockSize, 0 >>>( timeStampsElemAddressListPtr, this->_numElements, _elemsCacheSize );
	//GV_CHECK_CUDA_ERROR( "InitElemAddressList" );

	//CUDAPM_STOP_EVENT( gpucachemgr_clear_cpyAddr )

	// This is done in the wrapper class (DataProduction and Cache Manager)
	// - clear buffers holding mask of used and non-used elements
	//CUDAPM_START_EVENT( gpucachemgr_clear_fillML )
	//thrust::fill( _d_UnusedElementMasksTemp->begin(), _d_UnusedElementMasksTemp->end(), 0 );
	//thrust::fill( _d_UsedElementMasksTemp->begin(), _d_UsedElementMasksTemp->end(), 0 );
	//CUDAPM_STOP_EVENT( gpucachemgr_clear_fillML )

	// Clear the time-stamp buffer
	CUDAPM_START_EVENT( gpucachemgr_clear_fillTimeStamp )
	_d_TimeStampArray->fill( 0 );
	CUDAPM_STOP_EVENT( gpucachemgr_clear_fillTimeStamp )

	// Reset flag
	_exceededCapacity = false;
}

/******************************************************************************
 * Update the list of available elements according to their timestamps.
 * Unused and recycled elements will be placed first.
 *
 * @param pManageUpdatesOnly ...
 *
 * @return the number of available elements (i.e. un-used ones)
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::updateTimeStamps( bool pManageUpdatesOnly )
{
	if ( true || this->_lastNumLoads > 0 ) // TODO : check the utility of "true"
	{
		// LOG
		// std:cout << "manageUpdatesOnly "<< (int)pManageUpdatesOnly << "\n";

 		_numElemsNotUsed = 0;
		
		const uint cacheNbElements = getNumElements();
		uint nbActiveElements = cacheNbElements;
		// TODO: re-enable pManageUpdatesOnly
		/*if ( pManageUpdatesOnly )
		{
			nbActiveElements = this->_lastNumLoads;
		}*/
		const uint nbElementsToSort = nbActiveElements;
		if ( nbElementsToSort > 0 )
		{
			const uint nbInactiveElements = cacheNbElements - nbActiveElements; // not used for the moment
			const uint sortingStartPos = 0; // not used for the moment

			// Find "used" and "unused" elements during current frame in one single pass
			CUDAPM_START_EVENT( cache_updateTimestamps_createMasks );
			retrieveUsageMasks( nbElementsToSort, sortingStartPos );
			CUDAPM_STOP_EVENT( cache_updateTimestamps_createMasks );

			// Stream compaction to collect non-used elements at the beginning
			CUDAPM_START_EVENT( cache_updateTimestamps_threadReduc1 );
			retrieveUnusedElements( nbElementsToSort, sortingStartPos, nbInactiveElements );
			CUDAPM_STOP_EVENT( cache_updateTimestamps_threadReduc1 );
			
			// Retrieve the number of non-used elements
			// TODO : add events
			retrieveNbUnusedElements( nbInactiveElements );
			
			// Stream compaction to collect used elements at the end
			// - optimization : if _numElemsNotUsed is equal to max elements in cache, exit to avoid launching all following kernels
			if ( _numElemsNotUsed < cacheNbElements )
			{
				CUDAPM_START_EVENT( cache_updateTimestamps_threadReduc2 );
				retrieveUsedElements( nbElementsToSort, sortingStartPos );
				CUDAPM_STOP_EVENT( cache_updateTimestamps_threadReduc2 );
			}
		}
		else
		{
			_numElemsNotUsed = cacheNbElements;
		}

		// Swap internal element buffers
		swapElementBuffers();

		// Update internal counter
		this->_lastNumLoads = 0;
	}
	
	// Return number of un-used elements
	return _numElemsNotUsed;
}

/******************************************************************************
 * Main method to launch the production of nodes or data (i.e. bricks) 
 *
 * @param pGlobalRequestList global buffer of requests of elements to be produced (i.e. nodes and data)
 * @param pGlobalNbRequests total number of requests (all sorts of requests for nodes and data)
 * @param pRequestMask type of request to handle (node subdivision, brick load/produce, etc...)
 * @param pMaxAllowedNbRequests max allowed number of requests to handle (user customizable)
 * @param numValidNodes ...
 *
 * @return the number of produced elements
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::handleRequests( const uint* pGlobalRequestList, uint pGlobalNbRequests, uint pRequestMask,
			   uint pMaxAllowedNbRequests, uint numValidNodes )
{
	uint nbRequests = 0;

	if ( pGlobalNbRequests > 0 )
	{
		const uint cacheId = Id::value;

		// ---- [ 1 ] ---- 1st step
		//
		// Fill the buffer of masks of requests
				
		// The "_d_UpdateCompactList" buffer is filled with only elements addresses that have a type of request equal to "pRequestMask"
		// - it returns the number of elements concerned
		CUDAPM_START_EVENT_CHANNEL( 0, cacheId, gpucache_nodes_manageUpdates );
		nbRequests = retrieveRequests( pGlobalRequestList, pGlobalNbRequests, pRequestMask );
		CUDAPM_STOP_EVENT_CHANNEL( 0, cacheId, gpucache_nodes_manageUpdates );

		// Prevent loading more than the cache size
		nbRequests = std::min( nbRequests, getNumElements() );

		// Warning to prevent user that there are no more available slots
		if ( nbRequests > _numElemsNotUsed )
		{
#ifdef _DEBUG
			// LOG info
			std::cout << "CacheManager< " << cacheId << " >: Warning: " << _numElemsNotUsed << " non-used slots available, but ask for : " << nbRequests << std::endl;
#endif
		}

		// Update internal flag telling whether or not cache capacity cannot afford new production requests
		_exceededCapacity = ( _numElemsNotUsed == 0 && nbRequests > 0 );
				
		// Handle cache policy
		if ( this->_policy & ePreventReplacingUsedElementsPolicy )
		{
			// Prevent replacing elements in use
			nbRequests = std::min( nbRequests, _numElemsNotUsed );
		}
		if ( this->_policy & eSmoothLoadingPolicy )
		{
			// Smooth loading
			nbRequests = std::min( nbRequests, pMaxAllowedNbRequests );
		}

		// Production of elements if any
		if ( nbRequests > 0 )
		{
		//	std::cout << "CacheManager<" << cacheId << ">: " << nbRequests << " requests" << std::endl;

			// ---- [ 2 ] ---- 2nd step
			//
			// Invalidation phase

			// Update internal counter
			_totalNumLoads += nbRequests;
			_lastNumLoads = nbRequests;

		/*	std::cout << "\t_totalNumLoads : " << _totalNumLoads << std::endl;
			std::cout << "\t_lastNumLoads : " << _lastNumLoads << std::endl;
			std::cout << "\t_numElemsNotUsed : " << _numElemsNotUsed << std::endl;*/

			CUDAPM_START_EVENT_CHANNEL( 1, cacheId, gpucache_bricks_bricksInvalidation );
			invalidateElements( nbRequests, numValidNodes );		// WARNING !!!! nbRequests a été modifié auparavant !!!! ===> ERREUR !!!!!!!!!!!!!!
			CUDAPM_STOP_EVENT_CHANNEL( 1, cacheId, gpucache_bricks_bricksInvalidation );
		}
	}

	return nbRequests;
}

/******************************************************************************
 * Main method to launch the production of nodes or data (i.e. bricks) 
 *
 * @param pGlobalRequestList global buffer of requests of elements to be produced (i.e. nodes and data)
 * @param pGlobalNbRequests total number of requests (all sorts of requests for nodes and data)
 * @param pRequestMask type of request to handle (node subdivision, brick load/produce, etc...)
 * @param pMaxAllowedNbRequests max allowed number of requests to handle (user customizable)
 * @param numValidNodes ...
 *
 * @return the number of produced elements
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
uint GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::handleRequests( const uint* pGlobalRequestList, uint pGlobalNbRequests, uint pRequestMask,
			   uint pMaxAllowedNbRequests, uint numValidNodes,
			   const std::vector< unsigned int >& pObjectIDs, const GvCore::GsLinearMemory< uint >* pObjectIDBuffer, std::vector< unsigned int >& pNbRequestList )
{
	uint nbRequests = 0;

	if ( pGlobalNbRequests > 0 )
	{
		const uint cacheId = Id::value;

		// ---- [ 1 ] ---- 1st step
		//
		// Fill the buffer of masks of requests
				
		// The "_d_UpdateCompactList" buffer is filled with only elements addresses that have a type of request equal to "pRequestMask"
		// - it returns the number of elements concerned
		CUDAPM_START_EVENT_CHANNEL( 0, cacheId, gpucache_nodes_manageUpdates );
		nbRequests = retrieveRequests( pGlobalRequestList, pGlobalNbRequests, pRequestMask, pObjectIDs, pObjectIDBuffer, pNbRequestList );
		CUDAPM_STOP_EVENT_CHANNEL( 0, cacheId, gpucache_nodes_manageUpdates );

		// Prevent loading more than the cache size
		nbRequests = std::min( nbRequests, getNumElements() );

		// Warning to prevent user that there are no more available slots
		if ( nbRequests > _numElemsNotUsed )
		{
#ifdef _DEBUG
			// LOG info
			std::cout << "CacheManager< " << cacheId << " >: Warning: " << _numElemsNotUsed << " non-used slots available, but ask for : " << nbRequests << std::endl;
#endif
		}

		// Update internal flag telling whether or not cache capacity cannot afford new production requests
		_exceededCapacity = ( _numElemsNotUsed == 0 && nbRequests > 0 );
				
		// Handle cache policy
		if ( this->_policy & ePreventReplacingUsedElementsPolicy )
		{
			// Prevent replacing elements in use
			nbRequests = std::min( nbRequests, _numElemsNotUsed );
		}
		if ( this->_policy & eSmoothLoadingPolicy )
		{
			// Smooth loading
			nbRequests = std::min( nbRequests, pMaxAllowedNbRequests );
		}

		// Production of elements if any
		if ( nbRequests > 0 )
		{
		//	std::cout << "CacheManager<" << cacheId << ">: " << nbRequests << " requests" << std::endl;

			// ---- [ 2 ] ---- 2nd step
			//
			// Invalidation phase

			// Update internal counter
			_totalNumLoads += nbRequests;
			_lastNumLoads = nbRequests;

		/*	std::cout << "\t_totalNumLoads : " << _totalNumLoads << std::endl;
			std::cout << "\t_lastNumLoads : " << _lastNumLoads << std::endl;
			std::cout << "\t_numElemsNotUsed : " << _numElemsNotUsed << std::endl;*/

			CUDAPM_START_EVENT_CHANNEL( 1, cacheId, gpucache_bricks_bricksInvalidation );
			invalidateElements( nbRequests, numValidNodes );		// WARNING !!!! nbRequests a été modifié auparavant !!!! ===> ERREUR !!!!!!!!!!!!!!
			CUDAPM_STOP_EVENT_CHANNEL( 1, cacheId, gpucache_bricks_bricksInvalidation );
		}
	}

	return nbRequests;
}

/******************************************************************************
 * Create the "update" list of a given type.
 *
 * "Update list" is the list of elements and their associated requests of a given type (node subdivision or brick load/produce)
 *
 * @param inputList Buffer of node addresses and their associated requests. First two bits of their 32 bit addresses stores the type of request
 * @param pNbGlobalRequests Number of elements to process
 * @param pRequestType type of request (node subdivision or brick load/produce)
 *
 * @return the number of requests of given type
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
unsigned int GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::retrieveRequests( const uint* pGlobalRequestList, uint pNbGlobalRequests, uint pRequestType )
{
	size_t nbValidRequests = 0;

	// ---- [ 1 ] ---- 1st step
	//
	// Fill the buffer of masks of valid elements whose attribute is equal to "pRequestType"
	// Result is placed in "_d_TempUpdateMaskList"

	// Set kernel execution configuration
	const dim3 blockSize( 64, 1, 1 ); // TODO : compute size based on warpSize and multiProcessorCount instead
	const uint nbBlocks = iDivUp( pNbGlobalRequests, blockSize.x );
	const dim3 gridSize = dim3( std::min( nbBlocks, 65535U ) , iDivUp( nbBlocks, 65535U ), 1 );

	// Given a flag indicating a type of request (subdivision or load), and the updated global buffer of requests,
	// it fills a resulting mask buffer.
	// In fact, this mask indicates, for each element of the usage list, if it was used in the current rendering pass
	CUDAPM_START_EVENT( gpucachemgr_createUpdateList_createMask );
	CacheManagerCreateUpdateMask<<< gridSize, blockSize, 0 >>>(
		/*in*/pNbGlobalRequests,
		/*in*/pGlobalRequestList,
		/*out*/_d_TempUpdateMaskList->getPointer( 0 ),
		/*in*/pRequestType );
	GV_CHECK_CUDA_ERROR( "UpdateCreateSubdivMask" );
	CUDAPM_STOP_EVENT( gpucachemgr_createUpdateList_createMask );

	// ---- [ 2 ] ---- 2nd step
	//
	// Concatenate only previous valid elements from input data in "pGlobalRequestList" into the buffer of requests
	// Result is placed in "_d_UpdateCompactList"

	CUDAPM_START_EVENT( gpucachemgr_createUpdateList_elemsReduction );

	// Stream compaction
	GsCompute::GsDataParallelPrimitives::get().compact(
		/*output*/_d_UpdateCompactList->getPointer( 0 ),
		/*nbValidElements*/_d_numElementsPtr,
		/*input*/pGlobalRequestList,
		/*isValid*/_d_TempUpdateMaskList->getConstPointer( 0 ),
		/*nbElements*/pNbGlobalRequests );
	GV_CHECK_CUDA_ERROR( "cudppCompact" );

	// Get number of elements
	GS_CUDA_SAFE_CALL( cudaMemcpy( &nbValidRequests, _d_numElementsPtr, sizeof( size_t ), cudaMemcpyDeviceToHost ) );

	CUDAPM_STOP_EVENT( gpucachemgr_createUpdateList_elemsReduction );

	// Return total number of valid requests
	return static_cast< uint >( nbValidRequests );
}

/******************************************************************************
 * Create the "update" list of a given type.
 *
 * "Update list" is the list of elements and their associated requests of a given type (node subdivision or brick load/produce)
 *
 * @param inputList Buffer of node addresses and their associated requests. First two bits of their 32 bit addresses stores the type of request
 * @param pNbGlobalRequests Number of elements to process
 * @param pRequestType type of request (node subdivision or brick load/produce)
 *
 * @return the number of requests of given type
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
unsigned int GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::retrieveRequests( const uint* pGlobalRequestList, uint pNbGlobalRequests, uint pRequestType,
					const std::vector< unsigned int >& pObjectIDs, const GvCore::GsLinearMemory< uint >* pObjectIDBuffer, std::vector< unsigned int >& pNbRequestList )
{
	size_t nbValidRequests = 0;

	const size_t nbObjects = pObjectIDs.size();
	assert( nbObjects > 0 );
	assert( pObjectIDs.size() == pNbRequestList.size() );

	// Set kernel execution configuration
	dim3 blockSize( 64, 1, 1 ); // TODO : compute size based on warpSize and multiProcessorCount instead
	uint nbBlocks = iDivUp( pNbGlobalRequests, blockSize.x );
	dim3 gridSize = dim3( std::min( nbBlocks, 65535U ) , iDivUp( nbBlocks, 65535U ), 1 );

	if ( nbObjects == 1 )
	{
		// ---- [ 1 ] ---- 1st step
		//
		// Fill the buffer of masks of valid elements whose attribute is equal to "pRequestType"
		// Result is placed in "_d_TempUpdateMaskList"

		// Given a flag indicating a type of request (subdivision or load), and the updated global buffer of requests,
		// it fills a resulting mask buffer.
		// In fact, this mask indicates, for each element of the usage list, if it was used in the current rendering pass
		CUDAPM_START_EVENT( gpucachemgr_createUpdateList_createMask );
		CacheManagerCreateUpdateMask<<< gridSize, blockSize, 0 >>>(
			/*in*/pNbGlobalRequests,
			/*in*/pGlobalRequestList,
			/*out*/_d_TempUpdateMaskList->getPointer( 0 ),
			/*in*/pRequestType );
		GV_CHECK_CUDA_ERROR( "UpdateCreateSubdivMask" );
		CUDAPM_STOP_EVENT( gpucachemgr_createUpdateList_createMask );

		// ---- [ 2 ] ---- 2nd step
		//
		// Concatenate only previous valid elements from input data in "pGlobalRequestList" into the buffer of requests
		// Result is placed in "_d_UpdateCompactList"

		CUDAPM_START_EVENT( gpucachemgr_createUpdateList_elemsReduction );

		// Stream compaction
		GsCompute::GsDataParallelPrimitives::get().compact(
			/*output*/_d_UpdateCompactList->getPointer( 0 ),
			/*nbValidElements*/_d_numElementsPtr,
			/*input*/pGlobalRequestList,
			/*isValid*/_d_TempUpdateMaskList->getConstPointer( 0 ),
			/*nbElements*/pNbGlobalRequests );
		GV_CHECK_CUDA_ERROR( "cudppCompact" );
	
		// Get number of elements
		GS_CUDA_SAFE_CALL( cudaMemcpy( &nbValidRequests, _d_numElementsPtr, sizeof( size_t ), cudaMemcpyDeviceToHost ) );

		CUDAPM_STOP_EVENT( gpucachemgr_createUpdateList_elemsReduction );
	}
	else
	{	
		// Iterate through Object IDs
		size_t nbElements = 0;
		for ( size_t i = 0; i < nbObjects; i++ )
		{
			// Generate masks of requests (matching "request type" and "object ID")
			CacheManagerCreateUpdateMask<<< gridSize, blockSize, 0 >>>(
				pNbGlobalRequests,
				pGlobalRequestList,
				/*out*/_d_TempUpdateMaskList->getPointer( 0 ),
				pRequestType,
				//_d_ObjectIDs->getPointer( 0 ),
				pObjectIDBuffer->getConstPointer( 0 ),
				pObjectIDs[ i ] );
			GV_CHECK_CUDA_ERROR( "UpdateCreateSubdivMask" );
		
			// Stream compaction : retrieve valid requests (and concatenate them after previous valid ones)
			GsCompute::GsDataParallelPrimitives::get().compact(
				/*out*/_d_UpdateCompactList->getPointer( nbValidRequests/*offset*/ ),
				/*out*/_d_numElementsPtr/*nbValidElements*/,
				pGlobalRequestList,
				_d_TempUpdateMaskList->getConstPointer( 0 )/*isValid*/,
				pNbGlobalRequests/*nbElements*/ );
			GV_CHECK_CUDA_ERROR( "cudppCompact" );
	
			// Retrieve nb 
			nbElements = 0; // reset in case of cudaMemcpy error
			GS_CUDA_SAFE_CALL( cudaMemcpy( &nbElements, _d_numElementsPtr, sizeof( size_t ), cudaMemcpyDeviceToHost ) );
			nbValidRequests += nbElements;
		
			// Store production info
			//pNbRequestList.push_back( nbElements );
			pNbRequestList[ i ] = static_cast< unsigned int >( nbElements );
		}
	}
	
	// Return total number of valid requests
	return static_cast< uint >( nbValidRequests );
}

/******************************************************************************
 * Invalidate elements
 *
 * Timestamps are reset to 1 and node addresses to 0 (but not the 2 first flags)
 *
 * @param pNbElements number of elements to invalidate (this is done for the pNbElements first elements, i.e. the unused ones)
 * @param numValidPageTableSlots ...
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::invalidateElements( uint pNbElements, int numValidPageTableSlots )
{
	// ---- [ 1 ] ---- 1st step

	// Invalidation procedure
	{
		// Set kernel execution configuration
		// - Note : for the block size, as the kernel is small and wait for memory reading,
		// -------- try to optimize based on pNbElements to have more occupancy (more warps to hide latency):
		// -------- if pNbElements <= 32 use 32, if <= 64 use 64, the same until maybe 256
		dim3 blockSize( 64, 1, 1 );
		uint nbBlocks = iDivUp( pNbElements, blockSize.x );
		dim3 gridSize = dim3( std::min( nbBlocks, 65535U ), iDivUp( nbBlocks, 65535U ), 1 );

		// Reset the time stamp info of given elements to 1
		CacheManagerFlagInvalidations< ElementRes, AddressType ><<< gridSize, blockSize, 0 >>>(
			/*out*/_d_cacheManagerKernel/*time stamps*/,
			/*in*/pNbElements,
			/*in*/_d_elemAddressList->getConstPointer( 0 ) );

		GV_CHECK_CUDA_ERROR( "CacheManagerFlagInvalidations" );
	}
	
	// ---- [ 2 ] ---- 2nd step

	{
		uint3 pageTableRes = _d_pageTableArray->getResolution();
		uint numPageTableElements = pageTableRes.x * pageTableRes.y * pageTableRes.z;
		if ( numValidPageTableSlots >= 0 )
		{
			numPageTableElements = min( numPageTableElements, numValidPageTableSlots );
		}

		// Set kernel execution configuration
		// - Note : for the block size, as the kernel is small and wait for memory reading,
		// -------- try to optimize based on numPageTableElements to have more occupancy (more warps to hide latency):
		// -------- if numPageTableElements <= 32 use 32, if <= 64 use 64, the same until maybe 256
		dim3 blockSize( 64, 1, 1 );
		uint nbBlocks = iDivUp( numPageTableElements, blockSize.x );
		dim3 gridSize = dim3( std::min( nbBlocks, 65535U ), iDivUp( nbBlocks, 65535U ), 1 );

		// Reset all node addresses in the cache to NULL (i.e 0).
		// Only the first 30 bits of address are set to 0, not the 2 first flags.
		CacheManagerInvalidatePointers< ElementRes, AddressType ><<< gridSize, blockSize, 0 >>>(
			/*in*/_d_cacheManagerKernel,
			/*in*/numPageTableElements,
			/*out*/_d_pageTableArray->getDeviceArray() );

		GV_CHECK_CUDA_ERROR( "VolTreeInvalidateNodePointers" );
	}
}

/******************************************************************************
 * Get the flag telling whether or not cache has exceeded its capacity
 *
 * @return flag telling whether or not cache has exceeded its capacity
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline bool GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::hasExceededCapacity() const
{
	return _exceededCapacity;
}

/******************************************************************************
 * This method is called to serialize an object
 *
 * @param pStream the stream where to write
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::write( std::ostream& pStream ) const
{
	//_pageTable = new PageTableType();
	
	// - timestamp buffer
	GvCore::Array3D< uint >* timeStamps = new GvCore::Array3D< uint >( _d_TimeStampArray->getResolution(), GvCore::Array3D< uint >::StandardHeapMemory );
	memcpyArray( timeStamps, _d_TimeStampArray );
	pStream.write( reinterpret_cast< const char* >( timeStamps->getPointer() ), sizeof( uint ) * timeStamps->getNumElements() );
	delete timeStamps;

	// List of elements in the cache
	GvCore::Array3D< uint >* elemAddresses = new GvCore::Array3D< uint >( make_uint3( _d_elemAddressList->getNumElements(), 1, 1 ) );
	GvCore::memcpyArray( elemAddresses, _d_elemAddressList );
	pStream.write( reinterpret_cast< const char* >( elemAddresses->getPointer() ), sizeof( uint ) * elemAddresses->getNumElements() );
	delete elemAddresses;
	
	// Buffer of requests (masks of associated request in the current frame)
	GvCore::Array3D< uint >* requestBuffer = new GvCore::Array3D< uint >( make_uint3( _d_UpdateCompactList->getNumElements(), 1, 1 ) );
	GvCore::memcpyArray( requestBuffer, _d_UpdateCompactList );
	pStream.write( reinterpret_cast< const char* >( requestBuffer->getPointer() ), sizeof( uint ) * requestBuffer->getNumElements() );
	delete requestBuffer;
}

/******************************************************************************
 * This method is called to deserialize an object
 *
 * @param pStream the stream from which to read
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::read( std::istream& pStream )
{
}

/******************************************************************************
 * Retrieve usage masks of elements (i.e. used or unused)
 *
 * @param pNbElements number of elements to process
 * @param pStartIndex first index of elements from which to process elements
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::retrieveUsageMasks( unsigned int pNbElements, unsigned int pStartIndex )
{
	// Find "used" and "unused" elements during current pass in one single pass

	// Set kernel execution configuration
	//dim3 blockSize( 64, 1, 1 );
	dim3 blockSize( 256, 1, 1 ); // TODO : use multiple of GsCompute::GsDevice::_warpSize instead
	uint nbBlocks = iDivUp( pNbElements, blockSize.x );
	dim3 gridSize = dim3( std::min( nbBlocks, 65535U ) , iDivUp( nbBlocks, 65535U ), 1 );

	// This kernel creates the usage mask list of used and non-used elements (in current rendering pass) in a single pass
	GsKernel_CacheManager_retrieveElementUsageMasks< ElementRes, AddressType ><<< gridSize, blockSize, 0 >>>(
		/*in*/_d_cacheManagerKernel,
		/*in*/pNbElements,
		/*in*/_d_elemAddressList->getConstPointer( pStartIndex ),
		/*out*/_d_UnusedElementMasksTemp->getPointer( 0 ) /*resulting mask list of non-used elements*/,
		/*out*/_d_UsedElementMasksTemp->getPointer( 0 )	/*resulting mask list of used elements*/ );
	GV_CHECK_CUDA_ERROR( "GsKernel_CacheManager_retrieveElementUsageMasks" );
}

/******************************************************************************
 * Retrieve usage masks of elements (i.e. used or unused)
 *
 * @param pNbElements number of elements to process
 * @param pStartIndex first index of elements from which to process elements
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::retrieveUnusedElements( unsigned int pNbElements, unsigned int pStartIndex, unsigned int pNbInactiveElements )
{
	// Given an array d_in and an array of 1/0 flags in deviceValid, returns a compacted array in d_out of corresponding only the "valid" values from d_in.
	//
	// Takes as input an array of elements in GPU memory (d_in) and an equal-sized unsigned int array in GPU memory (deviceValid) that indicate which of those input elements are valid.
	// The output is a packed array, in GPU memory, of only those elements marked as valid.
	//
	// Internally, uses cudppScan.
	//
	// Algorithme : WRITE in temporary buffer _d_elemAddressListTmp, the compacted list of NON-USED elements from _d_elemAddressList
	// - number of un-used is returned in _d_numElementsPtr
	// - elements are written at the beginning of the array
	GsCompute::GsDataParallelPrimitives::get().compact(
		/* OUT : compacted output */_d_elemAddressListTmp->getPointer( pNbInactiveElements ),
		/* OUT :  number of elements valid flags in the d_isValid input array */_d_numElementsPtr,
		/* input to compact */_d_elemAddressList->getConstPointer( pStartIndex ),
		/* which elements in input are valid */_d_UnusedElementMasksTemp->getConstPointer( 0 ),
		/* nb of elements in input */pNbElements );
}

/******************************************************************************
 * Retrieve usage masks of elements (i.e. used or unused)
 *
 * @param pNbElements number of elements to process
 * @param pStartIndex first index of elements from which to process elements
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::retrieveNbUnusedElements( unsigned int pNbInactiveElements )
{
	// Get number of non-used elements
	size_t numElemsNotUsedST;
	GS_CUDA_SAFE_CALL( cudaMemcpy( &numElemsNotUsedST, _d_numElementsPtr, sizeof( size_t ), cudaMemcpyDeviceToHost ) );

	// Update value
	_numElemsNotUsed = static_cast< uint >( numElemsNotUsedST ) + pNbInactiveElements;
}

/******************************************************************************
 * Retrieve usage masks of elements (i.e. used or unused)
 *
 * @param pNbElements number of elements to process
 * @param pStartIndex first index of elements from which to process elements
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::retrieveUsedElements( unsigned int pNbElements, unsigned int pStartIndex )
{
	// Given an array d_in and an array of 1/0 flags in deviceValid, returns a compacted array in d_out of corresponding only the "valid" values from d_in.
	//
	// Takes as input an array of elements in GPU memory (d_in) and an equal-sized unsigned int array in GPU memory (deviceValid) that indicate which of those input elements are valid.
	// The output is a packed array, in GPU memory, of only those elements marked as valid.
	//
	// Internally, uses cudppScan.
	//
	// Algorithme : WRITE in temporary buffer _d_elemAddressListTmp, the compacted list of USED elements from _d_elemAddressList
	// - elements are written at the end of the array, i.e. after the previous NON-USED ones
	GsCompute::GsDataParallelPrimitives::get().compact(
		/* OUT : compacted output */_d_elemAddressListTmp->getPointer( _numElemsNotUsed ),
		/* OUT :  number of elements valid flags in the d_isValid input array */_d_numElementsPtr,
		/* input to compact */_d_elemAddressList->getConstPointer( pStartIndex ),
		/* which elements in input are valid */_d_UsedElementMasksTemp->getConstPointer( 0 ),
		/* nb of elements in input */pNbElements );
}

/******************************************************************************
 * Swap buffers
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
void GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::swapElementBuffers()
{
	// Temporary buffer "_d_elemAddressListTmp" where non-used elements are the beginning and used elements at the end,
	// is swapped with _d_elemAddressList
	GvCore::GsLinearMemory< uint >* tmpBuffer = _d_elemAddressList;
	_d_elemAddressList = _d_elemAddressListTmp;
	_d_elemAddressListTmp = tmpBuffer;
}

/******************************************************************************
 * Get the sorted list of elements in cache
 *
 * @return the sorted list of elements in cache
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline const GvCore::GsLinearMemory< uint >* GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getSortedElements() const
{
	return _d_elemAddressList;
}

/******************************************************************************
 * Get the sorted list of elements in cache
 *
 * @return the sorted list of elements in cache
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline GvCore::GsLinearMemory< uint >* GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::editSortedElements()
{
	return _d_elemAddressList;
}

/******************************************************************************
 * Get the list of elements to produce
 *
 * @return the list of elements to produce
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline const GvCore::GsLinearMemory< uint >* GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::getElementRequests() const
{
	return _d_UpdateCompactList;
}

/******************************************************************************
 * Get the list of elements to produce
 *
 * @return the list of elements to produce
 ******************************************************************************/
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
inline GvCore::GsLinearMemory< uint >* GsCacheElementManager< TId, ElementRes, AddressType, PageTableArrayType, PageTableType >
::editElementRequests()
{
	return _d_UpdateCompactList;
}

} // namespace GvCache
