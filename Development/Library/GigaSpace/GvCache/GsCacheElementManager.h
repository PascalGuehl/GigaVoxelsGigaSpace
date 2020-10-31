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

#ifndef _GS_CACHE_ELEMENT_MANAGER_H_
#define _GS_CACHE_ELEMENT_MANAGER_H_

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

// GigaVoxels
#include "GvCache/GsCacheManagerKernel.h"
#include "GvCore/GsArray.h"
#include "GvCore/GsLinearMemory.h"
#include "GvCore/GsFunctionalExt.h"
#include "GvCore/GsISerializable.h"

// STL
#include <vector>

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
 * @class GsCacheElementManager
 *
 * @brief The GsCacheElementManager class provides mecanisms to handle a cache on device (i.e. GPU)
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
class GsCacheElementManager : public GvCore::GsISerializable
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
	 * Reference on the Object IDs buffer
	 */
	//GvCore::GsLinearMemory< uint >* _d_ObjectIDs;

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
	GsCacheElementManager( const uint3& pCachesize, PageTableArrayType* pPageTableArray, uint pGraphicsInteroperability = 0 );

	/**
	 * Destructor
	 */
	virtual ~GsCacheElementManager();

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
	 * Main method to launch the production of nodes or data (i.e. bricks) 
	 *
	 * @param pGlobalRequestList global buffer of requests of elements to be produced (i.e. nodes and data)
	 * @param pGlobalNbRequests total number of requests (all sorts of requests for nodes and data)
	 * @param pRequestMask type of request to handle (node subdivision, brick load/produce, etc...)
	 * @param pMaxAllowedNbRequests max allowed number of requests to handle (user customizable)
	 * @param numValidNodes ...
	 *
	 * @return the number of produced elements
	 */
	uint handleRequests( const uint* pGlobalRequestList, uint pGlobalNbRequests, uint pRequestMask,
							uint pMaxAllowedNbRequests,
							uint numValidNodes );
	uint handleRequests( const uint* pGlobalRequestList, uint pGlobalNbRequests, uint pRequestMask,
							uint pMaxAllowedNbRequests,
							uint numValidNodes,
							const std::vector< unsigned int >& pObjectIDs, const GvCore::GsLinearMemory< uint >* pObjectIDBuffer, std::vector< unsigned int >& pNbRequestList );

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
	 * ...
	 */
	void setElementMaskBuffers( GvCore::GsLinearMemory< uint >* pUnusedElementMaskBuffer, GvCore::GsLinearMemory< uint >* pUsedElementMaskBuffer );

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

	/**
	 * Retrieve usage masks of elements (i.e. used or unused)
	 *
	 * @param pNbElements number of elements to process
	 * @param pStartIndex first index of elements from which to process elements
	 */
	void retrieveUsageMasks( unsigned int pNbElements, unsigned int pStartIndex );

	/**
	 * Retrieve usage masks of elements (i.e. used or unused)
	 *
	 * @param pNbElements number of elements to process
	 * @param pStartIndex first index of elements from which to process elements
	 * @param pNbInactiveElements number of inactive elements
	 */
	void retrieveUnusedElements( unsigned int pNbElements, unsigned int pStartIndex, unsigned int pNbInactiveElements );

	/**
	 * Retrieve usage masks of elements (i.e. used or unused)
	 *
	 * @param pNbElements number of elements to process
	 * @param pStartIndex first index of elements from which to process elements
	 * @param pNbInactiveElements number of inactive elements
	 */
	void retrieveNbUnusedElements( unsigned int pNbInactiveElements );

	/**
	 * Retrieve usage masks of elements (i.e. used or unused)
	 *
	 * @param pNbElements number of elements to process
	 * @param pStartIndex first index of elements from which to process elements
	 * @param pNbInactiveElements number of inactive elements
	 */
	void retrieveUsedElements( unsigned int pNbElements, unsigned int pStartIndex );

	/**
	 * Swap buffers of elements
	 */
	void swapElementBuffers();

	/**
	 * Get the sorted list of elements in cache
	 *
	 * @return the sorted list of elements in cache
	 */
	inline const GvCore::GsLinearMemory< uint >* getSortedElements() const;

	/**
	 * Get the sorted list of elements in cache
	 *
	 * @return the sorted list of elements in cache
	 */
	inline GvCore::GsLinearMemory< uint >* editSortedElements();

	/**
	 * Get the list of elements to produce
	 *
	 * @return the list of elements to produce
	 */
	inline const GvCore::GsLinearMemory< uint >* getElementRequests() const;

	/**
	 * Get the list of elements to produce
	 *
	 * @return the list of elements to produce
	 */
	inline GvCore::GsLinearMemory< uint >* editElementRequests();

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
	 * List of elements (with their requests) to produce (each element is unique due to compaction processing)
	 */
	GvCore::GsLinearMemory< uint >* _d_UpdateCompactList;
	GvCore::GsLinearMemory< uint >* _d_TempUpdateMaskList; // the buffer of masks of valid requests

	/**
	 * Temporary buffers used to store resulting mask list of used and non-used elements
	 * during the current rendering frame
	 */
	GvCore::GsLinearMemory< uint >* _d_UnusedElementMasksTemp;
	GvCore::GsLinearMemory< uint >* _d_UsedElementMasksTemp;
	
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
	 * @param pGlobalRequestList Buffer of node addresses and their associated requests. First two bits of their 32 bit addresses stores the type of request
	 * @param pNbGlobalRequests Number of elements to process
	 * @param pRequestType type of request (node subdivision or brick load/produce)
	 *
	 * @return the number of requests of given type
	 */
	unsigned int retrieveRequests( const uint* pGlobalRequestList, uint pNbGlobalRequests, uint pRequestType );
	
	/**
	 * Create the "update" list of a given type.
	 *
	 * "Update list" is the list of elements and their associated requests of a given type (node subdivision or brick load/produce)
	 *
	 * @param pGlobalRequestList Buffer of node addresses and their associated requests. First two bits of their 32 bit addresses stores the type of request
	 * @param pNbGlobalRequests Number of elements to process
	 * @param pRequestType type of request (node subdivision or brick load/produce)
	 *
	 * @return the number of requests of given type
	 */
	unsigned int retrieveRequests( const uint* pGlobalRequestList, uint pNbGlobalRequests, uint pRequestType, const std::vector< unsigned int >& pObjectIDs, const GvCore::GsLinearMemory< uint >* pObjectIDBuffer, std::vector< unsigned int >& pNbRequestList );
	
	/**
	 * Invalidate elements
	 *
	 * Timestamps are reset to 1 and node addresses to 0 (but not the 2 first flags)
	 *
	 * @param pNbElements number of elements to invalidate (this is done for the pNbElements first elements, i.e. the unused ones)
	 * @param numValidPageTableSlots ...
	 */
	void invalidateElements( uint pNbElements, int numValidPageTableSlots = -1 );

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
	GsCacheElementManager( const GsCacheElementManager& );

	/**
	 * Copy operator forbidden.
	 */
	GsCacheElementManager& operator=( const GsCacheElementManager& );

};

} // namespace GvCache

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsCacheElementManager.inl"

#endif
