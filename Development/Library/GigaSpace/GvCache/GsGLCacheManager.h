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

#ifndef _GS_GL_CACHE_MANAGER_H_
#define _GS_GL_CACHE_MANAGER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvPerfMon/GsPerformanceMonitor.h"

// CUDA
#include <vector_types.h>

// CUDA SDK
#include <helper_math.h>

// Thrust
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

// GigaVoxels
#include "GvCache/GsCacheManagerKernel.h"
#include "GvCore/GsArray.h"
#include "GvCore/GsLinearMemory.h"
#include "GvCore/GsFunctionalExt.h"
#include "GvCache/GsCacheManagerResources.h"
#include "GvCore/GsISerializable.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * CUDPP library
 *
 * By default, the cudpp library is used
 */
#define USE_CUDPP_LIBRARY 1

/**
 * ...
 */
#define GPUCACHE_BENCH_CPULRU 0

/**
 * ...
 */
#if GPUCACHE_BENCH_CPULRU
	extern uint GsCacheManager_currentTime;
#endif

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
 * @class GsGLCacheManager
 *
 * @brief The GsGLCacheManager class provides mecanisms to handle a cache on device (i.e. GPU)
 *
 * @ingroup GvCore
 * @namespace GvCache
 *
 * This class is used to manage a cache on the GPU.
 * It is based on a LRU mecanism (Least Recently Used) to get temporal coherency in data.
 *
 * Aide PARAMETRES TEMPLATES :
 * dans VolumeTreeCache.h :
 * - TId identifier (ex : 0 for nodes, 1 fors bricks of data, etc...)
 * - PageTableArrayType == GsLinearMemory< uint >
 * - PageTableType == PageTableNodes< ... GsLinearMemoryKernel< uint > ... > ou PageTableBricks< ... >
 * - GPUProviderType == IProvider< 1, GPUProducer > ou bien avec 0
 *
 * @todo add "virtual" to specific methods
 */
template< unsigned int TId, typename ElementRes, typename AddressType, typename PageTableArrayType, typename PageTableType >
class GsGLCacheManager : public GvCore::GsISerializable
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Type definition for the GPU side associated object
	 *
	 * @todo pass this parameter as a template parameter in order to be able to overload this component easily
	 */
	typedef GsCacheManagerKernel< ElementRes, AddressType > KernelType;

	/**
	 * The cache identifier
	 */
	typedef Loki::Int2Type< TId > Id;

	/**
	 * Cache policy
	 */
	enum ECachePolicy
	{
		eDefaultPolicy = 0,
		ePreventReplacingUsedElementsPolicy = 1,
		eSmoothLoadingPolicy = 1 << 1,
		eAllPolicies = ( 1 << 2 ) - 1
	};

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Page table
	 */
	PageTableType* _pageTable;

	/**
	 * Internal counters
	 */
	uint _totalNumLoads;
	uint _lastNumLoads;
	uint _numElemsNotUsed;

	/**
	 * In the GigaSpace engine, Cache management requires to :
	 * - protect "null" reference (element address)
	 * - root nodes in the data structure (i.e. octree, etc...)
	 * So, each array managed by the Cache needs to take care of these particular elements.
	 *
	 * Note : still a bug when too much loading - TODO: check this
	 */
	static const uint _cNbLockedElements;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param pCachesize size of the cache
	 * @param pPageTableArray the array of elements that the cache has to managed
	 * @param pGraphicsInteroperability a flag used to map buffers to OpenGL graphics library
	 */
	GsGLCacheManager( const uint3& pCachesize, PageTableArrayType* pPageTableArray, uint pGraphicsInteroperability = 0 );

	/**
	 * Destructor
	 */
	virtual ~GsGLCacheManager();

	/**
	 * Get the number of elements managed by the cache.
	 *
	 * @return the number of elements managed by the cache
	 */
	uint getNumElements() const;

	/**
	 * Get the associated device side object
	 *
	 * @return the associated device side object
	 */
	KernelType getKernelObject();

	/**
	 * Clear the cache
	 */
	void clearCache();

	/**
	 * Update symbols
	 * (variables in constant memory)
	 */
	void updateSymbols();

	/**
	 * Update the list of available elements according to their timestamps.
	 * Unused and recycled elements will be placed first.
	 *
	 * @param manageUpdatesOnly ...
	 *
	 * @return the number of available elements
	 */
	uint updateTimeStamps( bool manageUpdatesOnly );
	
	/**
	 * Main method to launch the production of nodes or bricks 
	 *
	 * @param updateList global buffer of requests of used elements only (node subdivision an brick lod/produce)
	 * @param numUpdateElems ...
	 * @param updateMask Type of request to handle (node subdivision or brick load/produce)
	 * @param maxNumElems Max number of elements to process
	 * @param numValidNodes ...
	 * @param gpuPool pool used to write new produced data inside (node pool or data pool)
	 *
	 * @return the number of produced elements
	 */
	template< typename GPUPoolType, typename TProducerType >
	uint genericWrite( uint* updateList, uint numUpdateElems, uint updateMask,
							uint maxNumElems, uint numValidNodes, const GPUPoolType& gpuPool, TProducerType* pProducer );

	/**
	 * Set the cache policy
	 *
	 * @param pPolicy the cache policy
	 */
	void setPolicy( ECachePolicy pPolicy );

	/**
	 * Get the cache policy
	 *
	 * @return the cache policy
	 */
	ECachePolicy getPolicy() const;

	/**
	 * Get the number of elements managed by the cache.
	 *
	 * @return the number of elements managed by the cache
	 */
	uint getNbUnusedElements() const;

	/**
	 * Get the timestamp list of the cache.
	 * There is as many timestamps as elements in the cache.
	 */
	GvCore::GsLinearMemory< uint >* getTimeStampList() const;

	/**
	 * Get the sorted list of cache elements, least recently used first.
	 * There is as many timestamps as elements in the cache.
	 */
	GvCore::GsLinearMemory< uint >* getElementList() const;
	
	/**
	 * Get the flag telling wheter or not cache has exceeded its capacity
	 *
	 * @return flag telling wheter or not cache has exceeded its capacity
	 */
	bool hasExceededCapacity() const;

	/**
	 * This method is called to serialize an object
	 *
	 * @param pStream the stream where to write
	 */
	virtual void write( std::ostream& pStream ) const;

	/**
	 * This method is called deserialize an object
	 *
	 * @param pStream the stream from which to read
	 */
	virtual void read( std::istream& pStream );

	//---------------------------------------------------------------
	GvCore::GsLinearMemory< uint >* editUpdateCompactList() { return _d_UpdateCompactList; }
	GvCore::GsLinearMemory< uint >* editElementAddressList() { return _d_elemAddressList; }
	//---------------------------------------------------------------

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Cache size
	 */
	uint3 _cacheSize;

	/**
	 * Cache size for elements
	 */
	uint3 _elemsCacheSize;

	/**
	 * Number of managed elements
	 */
	uint _numElements;

	/**
	 * Cache policy
	 */
	ECachePolicy _policy;

	/**
	 * Timestamp buffer.
	 *
	 * It attaches a 32-bit integer timestamp to each element (node tile or brick) of the pool.
	 * Timestamp corresponds to the time of the current rendering pass.
	 */
	GvCore::GsLinearMemory< uint >* _d_TimeStampArray;

	/**
	 * This list contains all elements addresses, sorted correctly so the unused one
	 * are at the beginning.
	 */
	GvCore::GsLinearMemory< uint >* _d_elemAddressList;
	GvCore::GsLinearMemory< uint >* _d_elemAddressListTmp;	// tmp buffer

	/**
	 * List of elements (with their requests) to process (each element is unique due to compaction processing)
	 */
	GvCore::GsLinearMemory< uint >* _d_UpdateCompactList;
	thrust::device_vector< uint >* _d_TempUpdateMaskList; // the buffer of masks of valid requests

	/**
	 * Temporary buffers used to store resulting mask list of used and non-used elements
	 * during the current rendering frame
	 */
	thrust::device_vector< uint >* _d_TempMaskList;
	thrust::device_vector< uint >* _d_TempMaskList2; // for cudpp approach

	/**
	 * Reference on the node pool's "child array" or "data array"
	 */
	PageTableArrayType* _d_pageTableArray;

	/**
	 * The associated device side object
	 */
	KernelType _d_cacheManagerKernel;

	/**
	 * CUDPP
	 */
	size_t* _d_numElementsPtr;
	CUDPPHandle _scanplan;
	
	/**
	 * Flag telling wheter or not cache has exceeded its capacity
	 */
	bool _exceededCapacity;

	/******************************** METHODS *********************************/

	/**
	 * Create the "update" list of a given type.
	 *
	 * "Update list" is the list of elements and their associated requests of a given type (node subdivision or brick load/produce)
	 *
	 * @param inputList Buffer of node addresses and their associated requests. First two bits of their 32 bit addresses stores the type of request
	 * @param inputNumElem Number of elements to process
	 * @param testFlag type of request (node subdivision or brick load/produce)
	 *
	 * @return the number of requests of given type
	 */
	uint createUpdateList( uint* inputList, uint inputNumElem, uint testFlag );

	/**
	 * Invalidate elements
	 *
	 * Timestamps are reset to 1 and node addresses to 0 (but not the 2 first flags)
	 *
	 * @param numElems ...
	 * @param numValidPageTableSlots ...
	 */
	void invalidateElements( uint numElems, int numValidPageTableSlots = -1 );

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsGLCacheManager( const GsGLCacheManager& );

	/**
	 * Copy operator forbidden.
	 */
	GsGLCacheManager& operator=( const GsGLCacheManager& );

};

} // namespace GvCache

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsGLCacheManager.inl"

#endif
