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

#ifndef _GV_DATA_PRODUCTION_MANAGER_H_
#define _GV_DATA_PRODUCTION_MANAGER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// System
#include <iostream>

// Cuda
#include <vector_types.h>

// Thrust
#include <thrust/device_vector.h>
#include <thrust/copy.h>

// GigaSpace
#include "GvStructure/GsIDataProductionManager.h"
#include "GvCore/GsVector.h"
#include "GvCore/GsArray.h"
#include "GvCore/GsLinearMemory.h"
#include "GvCore/GsRendererTypes.h"
#include "GvCore/GsPool.h"
#include "GvCore/GsVector.h"
#include "GvCore/GsPageTable.h"
#include "GvCore/GsIProvider.h"
#include "GvRendering/GsRendererHelpersKernel.h"
#include "GvCore/GsOracleRegionInfo.h"
#include "GvCore/GsLocalizationInfo.h"
#include "GvCache/GsCacheElementManager.h"
//#include "GvCache/GsNodeCacheManager.h"
#include "GvPerfMon/GsPerformanceMonitor.h"
#include "GvStructure/GsVolumeTreeAddressType.h"
#include "GvStructure/GsDataProductionManagerKernel.h"

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

namespace GvStructure
{

/**
 * @struct GsProductionStatistics
 *
 * @brief The GsProductionStatistics struct provides storage for production statistics.
 *
 * Production management can be monitored by storing statistics
 * with the help of the GsProductionStatistics struct.
 */
struct GsProductionStatistics
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Time (or index of pass)
	 */
	uint _frameId;

	/**
	  * Number of nodes
	 */
	uint _nNodes;

	/**
	 * Time to produce nodes
	 */
	float _nodesProductionTime;

	/**
	 * Number of bricks
	 */
	uint _nBricks;

	/**
	 * Time to produce bricks
	 */
	float _bricksProductionTime;

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

/**
 * @class GsDataProductionManager
 *The GsDataProductionManager class provides the concept of cache.
 * @brief The GsDataProductionManager class provides the concept of cache.
 *
 * This class implements the cache mechanism for the VolumeTree data structure.
 * As device memory is limited, it is used to store the least recently used element in memory.
 * It is responsible to handle the data requests list generated during the rendering process.
 * (ray-tracing - N-tree traversal).
 * Request are then sent to producer to load or produced data on the host or on the device.
 *
 * @param TDataStructure The volume tree data structure (nodes and bricks)
 * @param ProducerType The associated user producer (host or device)
 */
template< typename TDataStructure, typename TPriorityPoliciesManager >
class GsDataProductionManager : public GvStructure::GsIDataProductionManager
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Type definition of the node tile resolution
	 */
	typedef typename TDataStructure::NodeTileResolution NodeTileRes;

	/**
	 * Type definition of the full brick resolution (i.e. with border)
	 */
	typedef typename TDataStructure::FullBrickResolution BrickFullRes;

	/**
	 * Linear representation of a node tile
	 */
	typedef GvCore::GsVec3D< NodeTileRes::numElements, 1, 1 > NodeTileResLinear;

	/**
	 * Defines the types used to store the localization infos
	 */
	typedef GvCore::GsLinearMemory< GvCore::GsLocalizationInfo::CodeType > LocCodeArrayType;
	typedef GvCore::GsLinearMemory< GvCore::GsLocalizationInfo::DepthType > LocDepthArrayType;

	// FIXME: GsVec3D. Need to move the "linearization" of the resolution
	// into the GPUCache so we have the correct values
	/**
	 * Type definition for nodes page table
	 */
	typedef GvCore::PageTableNodes
	<
		NodeTileRes, NodeTileResLinear,
		VolTreeNodeAddress,	GvCore::GsLinearMemoryKernel< uint >,
			LocCodeArrayType, LocDepthArrayType
	>
	NodePageTableType;

	/**
	 * Type definition for bricks page table
	 */
	typedef GvCore::PageTableBricks
	<
		NodeTileRes,
		VolTreeNodeAddress, GvCore::GsLinearMemoryKernel< uint >,
		VolTreeBrickAddress, GvCore::GsLinearMemoryKernel< uint >,
		LocCodeArrayType, LocDepthArrayType
	>
	BrickPageTableType;

	/**
	 * Type definition for the nodes cache manager
	 */
	typedef GvCache::GsCacheElementManager
	<
		0, NodeTileResLinear, VolTreeNodeAddress, GvCore::GsLinearMemory< uint >, NodePageTableType
	>
	NodesCacheManager;
	//typedef GvCache::GsNodeCacheManager< TDataStructure > NodesCacheManager;

	/**
	 * Type definition for the bricks cache manager
	 */
	typedef GvCache::GsCacheElementManager
	<
		1, BrickFullRes, VolTreeBrickAddress, GvCore::GsLinearMemory< uint >, BrickPageTableType
	>
	BricksCacheManager;

	/**
	 * Type definition for the associated device-side object
	 */
	typedef GsDataProductionManagerKernel
	<
		NodeTileResLinear, BrickFullRes, VolTreeNodeAddress, VolTreeBrickAddress, TPriorityPoliciesManager
	>
	DataProductionManagerKernelType;

	/**
	 * Type definition of producers
	 */
	typedef GvCore::GsIProvider ProducerType;

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Use brick usage optimization flag
	 *
	 * @todo Describe its use
	 */
	bool _useBrickUsageOptim;

	/**
	 * Intra frame pass flag
	 *
	 * @todo Describe its use
	 */
	bool _intraFramePass;

	/**
	 * Leaf node tracker
	 */
	thrust::device_vector< unsigned int >* _leafNodes;
	thrust::device_vector< float >* _emptyNodeVolume;
	unsigned int _nbLeafNodes;
	unsigned int _nbNodes;
	bool _isRealTimeTreeDataStructureMonitoringEnabled;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param voltree a pointer to the data structure.
	 * @param gpuprod a pointer to the user's producer.
	 * @param nodepoolres the 3d size of the node pool.
	 * @param brickpoolres the 3d size of the brick pool.
	 * @param graphicsInteroperability Graphics interoperability flag to be able to map buffers to graphics interoperability mode
	 */
	GsDataProductionManager( TDataStructure* pDataStructure, uint3 nodepoolres, uint3 brickpoolres, uint graphicsInteroperability = 0 );

	/**
	 * Destructor
	 */
	virtual ~GsDataProductionManager();

	/**
	 * This method is called before the rendering process. We just clear the request buffer.
	 */
	virtual void preRenderPass();

	/**
	 * This method is called after the rendering process. She's responsible for processing requests.
	 *
	 * @return the number of requests processed.
	 *
	 * @todo Check whether or not the inversion call of updateTimeStamps() with manageUpdates() has side effects
	 */
	virtual uint handleRequests();

	/**
	 * This method destroy the current N-tree and clear the caches.
	 */
	virtual void clearCache();

	/**
	 * Get the associated device-side object
	 *
	 * @return The device-side object
	 */
	inline DataProductionManagerKernelType getKernelObject() const;

	/**
	 * Get the update buffer
	 *
	 * @return The update buffer
	 */
	inline GvCore::GsLinearMemory< uint >* getUpdateBuffer() const;

	/**
	 * Get the nodes cache manager
	 *
	 * @return the nodes cache manager
	 */
	inline const NodesCacheManager* getNodesCacheManager() const;

	/**
	 * Get the bricks cache manager
	 *
	 * @return the bricks cache manager
	 */
	inline const BricksCacheManager* getBricksCacheManager() const;

	/**
	 * Get the nodes cache manager
	 *
	 * @return the nodes cache manager
	 */
	inline NodesCacheManager* editNodesCacheManager();

	/**
	 * Get the bricks cache manager
	 *
	 * @return the bricks cache manager
	 */
	inline BricksCacheManager* editBricksCacheManager();

	/**
	 * Get the max number of requests of node subdivisions the cache has to handle.
	 *
	 * @return the max number of requests
	 */
	inline uint getMaxNbNodeSubdivisions() const;

	/**
	 * Set the max number of requests of node subdivisions the cache has to handle.
	 *
	 * @param pValue the max number of requests
	 */
	void setMaxNbNodeSubdivisions( uint pValue );

	/**
	 * Get the max number of requests of brick of voxel loads  the cache has to handle.
	 *
	 * @return the max number of requests
	 */
	inline uint getMaxNbBrickLoads() const;

	/**
	 * Set the max number of requests of brick of voxel loads the cache has to handle.
	 *
	 * @param pValue the max number of requests
	 */
	void setMaxNbBrickLoads( uint pValue );

	/**
	 * Get the number of requests of node subdivisions the cache has handled.
	 *
	 * @return the number of requests
	 */
	unsigned int getNbNodeSubdivisionRequests() const;

	/**
	 * Get the number of requests of brick of voxel loads the cache has handled.
	 *
	 * @return the number of requests
	 */
	unsigned int getNbBrickLoadRequests() const;

	/**
	 * Add a producer
	 *
	 * @param pProducer the producer to add
	 */
	void addProducer( ProducerType* pProducer );

	/**
	 * Remove a producer
	 *
	 * @param pProducer the producer to remove
	 */
	void removeProducer( ProducerType* pProducer );

	/**
	 * Set the current producer
	 *
	 * @param pProducer The producer
	 */
	void setCurrentProducer( ProducerType* pProducer );

	/**
	 * Get the flag telling whether or not tree data structure monitoring is activated
	 *
	 * @return the flag telling whether or not tree data structure monitoring is activated
	 */
	inline bool hasTreeDataStructureMonitoring() const;

	/**
	 * Set the flag telling whether or not tree data structure monitoring is activated
	 *
	 * @param pFlag the flag telling whether or not tree data structure monitoring is activated
	 */
	void setTreeDataStructureMonitoring( bool pFlag );

	/**
	 * Get the flag telling whether or not cache has exceeded its capacity
	 *
	 * @return flag telling whether or not cache has exceeded its capacity
	 */
	bool hasCacheExceededCapacity() const;

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
	 * Get the flag telling whether or not the production time limit is activated.
	 *
	 * @return the flag telling whether or not the production time limit is activated.
	 */
	bool isProductionTimeLimited() const;

	/**
	 * Set or unset the flag used to tell whether or not the production time is limited.
	 *
	 * @param pFlag the flag value.
	 */
	void useProductionTimeLimit( bool pFlag );

	/**
	 * Get the time limit actually in use.
	 *
	 * @return the time limit.
	 */
	float getProductionTimeLimit() const;

	/**
	 * Set the time limit for the production.
	 *
	 * @param pTime the time limit (in ms).
	 */
	void setProductionTimeLimit( float pTime );

	/**
	 * Tell whether or not to use priority at production
	 *
	 * @return a flag telling whether or not to use priority at production
	 */
	bool hasProductionPriority() const;

	/**
	 * Set the flag telling whether or not to use priority at production
	 *
	 * @param pFlag a flag telling whether or not to use priority at production
	 */
	void setProductionPriority( bool pFlag );

	/**
	 * Get the number of objects handled
	 *
	 * @return the number of objects handled
	 */
	inline unsigned int getNbObjects( unsigned int pValue );
	
	/**
	 * Set the number of objects to be handled
	 *
	 * @param pValue the number of objects to be handled
	 */
	void setNbObjects( unsigned int pValue );
	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	///**
	// * Nodes page table
	// */
	//NodePageTableType* _nodesPageTable;		// Seemed to be not used any more ?

	///**
	// * Bricks page table
	// */
	//BrickPageTableType* _bricksPageTable;	// Seemed to be not used any more ?

	/**
	 * Volume tree data structure
	 */
	TDataStructure* _dataStructure;

	/**
	 * Node pool resolution
	 */
	uint3 _nodePoolRes;

	/**
	 * Brick pool resolution
	 */
	uint3 _brickPoolRes;

	/**
	 * The associated device-side object
	 */
	DataProductionManagerKernelType _dataProductionManagerKernel;

	/**
	 * Update buffer array
	 *
	 * Buffer used to store node addresses updated with node subdivisions and/or load requests
	 */
	GvCore::GsLinearMemory< uint >* _updateBufferArray;

	/**
	 * Update buffer compact list
	 *
	 * Buffer resulting from the "_updateBufferArray stream compaction" to only keep nodes associated to a request
	 */
	GvCore::GsLinearMemory< uint >* _updateBufferCompactList;

	/**
	 * Buffers used for Production priority management
	 */
	GvCore::GsLinearMemory< int >* _priorityBufferArray;
	GvCore::GsLinearMemory< uint >* _priorityBufferCompactList;

#ifdef GS_USE_MULTI_OBJECTS
	/**
	 * Update buffer array
	 *
	 * Buffer used to store node addresses updated with node subdivisions and/or load requests
	 */
	GvCore::GsLinearMemory< uint >* _objectIDBuffer;
	/**
	 * Update buffer compact list
	 *
	 * Buffer resulting from the "_updateBufferArray stream compaction" to only keep nodes associated to a request
	 */
	GvCore::GsLinearMemory< uint >* _objectIDBufferCompactList;
#endif

	/**
	 * Number of node tiles not in use
	 */
	uint _numNodeTilesNotInUse;

	/**
	 * Number of bricks not in used
	 */
	uint _numBricksNotInUse;

	/**
	 * Total number of loaded bricks
	 */
	uint _totalNumBricksLoaded;				// Seemed to be not used any more ?

	/**
	 * Nodes cache manager
	 */
	NodesCacheManager* _nodesCacheManager;

	/**
	 * Bricks cache manager
	 */
	BricksCacheManager* _bricksCacheManager;

	/**
	 * Maximum number of subdivision requests the cache has to handle
	 */
	uint _maxNbNodeSubdivisions;

	/**
	 *  Maximum number of load requests the cache has to handle
	 */
	uint _maxNbBrickLoads;

	/**
	 * Number of subdivision requests the cache has handled
	 */
	uint _nbNodeSubdivisionRequests;

	/**
	 *  Number of load requests the cache has handled
	 */
	uint _nbBrickLoadRequests;

	/**
	 * CUDPP stream compaction parameters to process the requests buffer
	 */
	size_t* _d_nbValidRequests;
	GvCore::GsLinearMemory< uint >* _d_validRequestMasks;

	/**
	 * Temporary buffers used to store resulting mask list of used and non-used elements
	 * during the current pass.
	 */
	GvCore::GsLinearMemory< uint >* _dUnusedElementMasks;
	GvCore::GsLinearMemory< uint >* _dUsedElementMasks;

	/**
	 * List of producers
	 */
	ProducerType* _currentProducer;
	std::vector< ProducerType* > _producers;

	/**
	 * Flag to tell whether or not tree data structure monitoring is activated
	 */
	bool _hasTreeDataStructureMonitoring;

	/**
	 * Events used to measure the production time.
	 */
	cudaEvent_t _startProductionNodes, _stopProductionNodes, _stopProductionBricks, _startProductionBricks;

	/**
	 * Total production time for the bricks/nodes since the start of GigaVoxels.
	 */
	float _totalNodesProductionTime, _totalBrickProductionTime;

	/**
	 * Number of bricks/nodes produced since the start of GigaVoxels.
	 */
	uint _totalProducedBricks, _totalProducedNodes;

	/**
	 * Vector containing statistics about production.
	 */
	std::vector< GsProductionStatistics > _productionStatistics;

	/**
	 * Limit of time we are allowed to spend during production.
	 * This is not an hard limit, the ProductionManager will try to limit the number
	 * of requests according to the mean production time it observed so far.
	 */
	float _productionTimeLimit;

	/**
	 * Flag indicating whether or not the production time is limited.
	 */
	bool _isProductionTimeLimited;
	
	/**
	 * Flag indicating whether or not the last production was timed.
	 * It is used to now if the cudaEvent were correctly initialised.
	 */
	bool _lastProductionTimed;

	/**
	 * Flag to tell whether or not to use priority at production
	 */
	bool _hasProductionPriority;

	/**
	 * Number of objects
	 */
	unsigned int _nbObjects;

	/******************************** METHODS *********************************/

	/**
	 * Update time stamps
	 */
	virtual void updateTimeStamps();

	/**
	 * This method gather all requests by compacting the list.
	 *
	 * @return The number of elements in the requests list
	 */
	virtual uint manageUpdates();

	/**
	 * This method handle the subdivisions requests.
	 *
	 * @param numUpdateElems the number of requests available in the buffer (of any kind).
	 *
	 * @return the number of subdivision requests processed.
	 */
	virtual uint manageNodeProduction( uint numUpdateElems );

	/**
	 * This method handle the load requests.
	 *
	 * @param numUpdateElems the number of requests available in the buffer (of any kind).
	 *
	 * @return the number of load requests processed.
	 */
	virtual uint manageDataProduction( uint numUpdateElems );

#ifdef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_PRODUCER
	/**
	 * ...
	 */
	virtual void produceData( uint numUpdateElems );
#endif

	/**
	 * Retrieve masks of valid and unvalid requests
	 *
	 * @param pNbElements number of elements to process
	 */
	void retrieveRequestMasks( unsigned int pNbElements );

	/**
	 * Retrieve list of valid requests
	 *
	 * @param pNbElements number of elements to process
	 */
	void retrieveValidRequests( unsigned int pNbElements );

	/**
	 * Retrieve number of valid requests
	 *
	 * @return the number of valid requests
	 */
	unsigned int retrieveNbValidRequests();

	/**
	 * Retrieve list of valid request priorities (for production)
	 *
	 * @param pNbElements number of elements to process
	 */
	void retrieveValidRequestPriorities( unsigned int pNbElements );

	/**
	 * Sort requests using priorities (for production)
	 *
	 * @param pNbElements number of elements to process
	 */
	void sortRequestPriorities( unsigned int pNbElements );

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
	GsDataProductionManager( const GsDataProductionManager& );

	/**
	 * Copy operator forbidden.
	 */
	GsDataProductionManager& operator=( const GsDataProductionManager& );

};

} // namespace GvStructure

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsDataProductionManager.inl"

#endif
