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

// Cuda SDK
#include <helper_math.h>

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvPerfMon/GsPerformanceMonitor.h"
#include "GvCore/GsFunctionalExt.h"
#include "GvCore/GsError.h"
#include "GsCompute/GsDataParallelPrimitives.h"

// TEST
#include "GvCore/GsLinearMemory.h"

// System
#include <cassert>

// STL
#include <algorithm>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvStructure
{

/******************************************************************************
 * Constructor
 *
 * @param voltree a pointer to the data structure.
 * @param gpuprod a pointer to the user's producer.
 * @param nodepoolres the 3d size of the node pool.
 * @param brickpoolres the 3d size of the brick pool.
 * @param graphicsInteroperability Graphics interoperability flag to be able to map buffers to graphics interoperability mode
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::GsDataProductionManager( TDataStructure* pDataStructure, uint3 nodepoolres, uint3 brickpoolres, uint graphicsInteroperability )
:	GvStructure::GsIDataProductionManager()
,	_d_nbValidRequests( NULL )
,	_d_validRequestMasks( NULL )
,	_dUnusedElementMasks( NULL )
,	_dUsedElementMasks( NULL )
,	_currentProducer( NULL )
,	_producers()
,	_leafNodes( NULL )
,	_emptyNodeVolume( NULL )
,	_nbLeafNodes( 0 )
,	_nbNodes( 0 )
,	_hasTreeDataStructureMonitoring( false )
,	_isProductionTimeLimited( false )
,	_lastProductionTimed( false )
,	_productionTimeLimit( 50.f )
,	_totalNodesProductionTime( 0.f )
,	_totalBrickProductionTime( 0.f )
,	_totalProducedBricks( 0u )
,	_totalProducedNodes( 0u )
,	_hasProductionPriority( false )
,	_nbObjects( 0 )
{
	// Handle 1 object by default
	_nbObjects = 1;
	
	// Reference on a data structure
	_dataStructure = pDataStructure;

	// linearize the resolution
	_nodePoolRes = make_uint3( nodepoolres.x * nodepoolres.y * nodepoolres.z, 1, 1 );
	_brickPoolRes = brickpoolres;

	// Cache managers creation : nodes and bricks
	_nodesCacheManager = new NodesCacheManager( _nodePoolRes, _dataStructure->_childArray, graphicsInteroperability );
	_bricksCacheManager = new BricksCacheManager( _brickPoolRes, _dataStructure->_dataArray, graphicsInteroperability );

	//@todo The creation of the localization arrays should be moved here, not in the data structure (this is cache implementation details/features)

	// Node cache manager initialization
	_nodesCacheManager->_pageTable->locCodeArray = _dataStructure->_localizationCodeArray;
	_nodesCacheManager->_pageTable->locDepthArray = _dataStructure->_localizationDepthArray;
	_nodesCacheManager->_pageTable->getKernel().childArray = _dataStructure->_childArray->getDeviceArray();
	_nodesCacheManager->_pageTable->getKernel().locCodeArray = _dataStructure->_localizationCodeArray->getDeviceArray();
	_nodesCacheManager->_pageTable->getKernel().locDepthArray = _dataStructure->_localizationDepthArray->getDeviceArray();
//#ifndef GS_USE_MULTI_OBJECTS
//	_nodesCacheManager->_totalNumLoads = 2;
//	_nodesCacheManager->_lastNumLoads = 1;
//#else
//	_nodesCacheManager->_totalNumLoads = 2 + 1;
//	_nodesCacheManager->_lastNumLoads = 1 + 1;
//#endif
	_nodesCacheManager->_totalNumLoads = 1/*protect first element*/ + _nbObjects;
	_nodesCacheManager->_lastNumLoads = _nbObjects;

	// Data cache manager initialization
	_bricksCacheManager->_pageTable->locCodeArray = _dataStructure->_localizationCodeArray;
	_bricksCacheManager->_pageTable->locDepthArray = _dataStructure->_localizationDepthArray;
	_bricksCacheManager->_pageTable->getKernel().childArray = _dataStructure->_childArray->getDeviceArray();
	_bricksCacheManager->_pageTable->getKernel().dataArray = _dataStructure->_dataArray->getDeviceArray();
	_bricksCacheManager->_pageTable->getKernel().locCodeArray = _dataStructure->_localizationCodeArray->getDeviceArray();
	_bricksCacheManager->_pageTable->getKernel().locDepthArray = _dataStructure->_localizationDepthArray->getDeviceArray();
	_bricksCacheManager->_totalNumLoads = 0;
	_bricksCacheManager->_lastNumLoads = 0;

	// Nodes and bricks managers
	// - common temporary buffers to store ununsed and used elements in a GigaSpace pipeline pass*
	uint cacheManagerMaxNbElements = std::max( _nodesCacheManager->getNumElements(), _bricksCacheManager->getNumElements() );
	_dUnusedElementMasks = new GvCore::GsLinearMemory< uint >( make_uint3( cacheManagerMaxNbElements, 1, 1 ) );
	_dUsedElementMasks = new GvCore::GsLinearMemory< uint >( make_uint3( cacheManagerMaxNbElements, 1, 1 ) );
	_dUnusedElementMasks->fillAsync( 0 );
	_dUsedElementMasks->fill( 0 );
	_nodesCacheManager->setElementMaskBuffers( _dUnusedElementMasks, _dUsedElementMasks );
	_bricksCacheManager->setElementMaskBuffers( _dUnusedElementMasks, _dUsedElementMasks );
	cacheManagerMaxNbElements = std::max( _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z, cacheManagerMaxNbElements );
	GsCompute::GsDataParallelPrimitives::get().initializeCompactPlan( cacheManagerMaxNbElements );
	GsCompute::GsDataParallelPrimitives::get().initializeSortPlan( cacheManagerMaxNbElements );

	// Request buffers initialization
	_updateBufferArray = new GvCore::GsLinearMemory< uint >( _nodePoolRes, graphicsInteroperability );
	_updateBufferCompactList = new GvCore::GsLinearMemory< uint >( _nodePoolRes );

	// Priority buffers (for production requests)
	_priorityBufferArray = new GvCore::GsLinearMemory< int >( _nodePoolRes, graphicsInteroperability );
	_priorityBufferCompactList = new GvCore::GsLinearMemory< uint >( _nodePoolRes );

#ifdef GS_USE_MULTI_OBJECTS
	_objectIDBuffer = new GvCore::GsLinearMemory< uint >( _nodePoolRes, graphicsInteroperability );
	_objectIDBufferCompactList = new GvCore::GsLinearMemory< uint >( _nodePoolRes );
//	_nodesCacheManager->_d_ObjectIDs = _objectIDBufferCompactList;
//	_bricksCacheManager->_d_ObjectIDs = _objectIDBufferCompactList;
#endif

	_totalNumBricksLoaded = 0;

	// Device-side cache manager initialization
	_dataProductionManagerKernel._updateBufferArray = this->_updateBufferArray->getDeviceArray();
	_dataProductionManagerKernel._nodeCacheManager = this->_nodesCacheManager->getKernelObject();
	_dataProductionManagerKernel._brickCacheManager = this->_bricksCacheManager->getKernelObject();
	_dataProductionManagerKernel._priorityBufferArray = this->_priorityBufferArray->getDeviceArray();
#ifdef GS_USE_MULTI_OBJECTS
	_dataProductionManagerKernel._objectIDBuffer = this->_objectIDBuffer->getDeviceArray();
#endif

	// Initialize max number of requests the cache has to handle
	_maxNbNodeSubdivisions = 5000;
	_maxNbBrickLoads = 3000;
	this->_nbNodeSubdivisionRequests = 0;
	this->_nbBrickLoadRequests = 0;

#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_DATAPRODUCTIONMANAGER
	GS_CUDA_SAFE_CALL( cudaMalloc( (void**) &_d_nbValidRequests, sizeof( size_t ) ) );
#else
	#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_WITH_COMBINED_COPY
	GS_CUDA_SAFE_CALL( cudaMalloc( (void**) &_d_nbValidRequests, sizeof( size_t ) ) );
	#else
	GS_CUDA_SAFE_CALL( cudaMalloc( (void**) &_d_nbValidRequests, 3 * sizeof( size_t ) ) );
	_nodesCacheManager->_d_nbValidRequests = _d_nbValidRequests;
	_bricksCacheManager->_d_nbValidRequests = _d_nbValidRequests;
	#endif
#endif

	_d_validRequestMasks = new GvCore::GsLinearMemory< uint >( _nodePoolRes, graphicsInteroperability );

	// TO DO : do lazy evaluation => ONLY ALLOCATE WHEN REQUESTED AND USED + free memory just after ?
	_leafNodes = new thrust::device_vector< uint >( _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z );
	_emptyNodeVolume = new thrust::device_vector< float >( _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z );

	cudaEventCreate( &_startProductionNodes );
	cudaEventCreate( &_stopProductionNodes );
	cudaEventCreate( &_startProductionBricks );
	cudaEventCreate( &_stopProductionBricks );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::~GsDataProductionManager()
{
	// Delete cache manager (nodes and bricks)
	delete _nodesCacheManager;
	delete _bricksCacheManager;

	delete _dUnusedElementMasks;
	delete _dUsedElementMasks;
	_dUnusedElementMasks = NULL;
	_dUsedElementMasks = NULL;

	delete _updateBufferArray;
	delete _updateBufferCompactList;
	delete _priorityBufferArray;
	delete _priorityBufferCompactList;

#ifdef GS_USE_MULTI_OBJECTS
	delete _objectIDBuffer;
	_objectIDBuffer = NULL;
	delete _objectIDBufferCompactList;
	_objectIDBufferCompactList = NULL;
#endif

	GS_CUDA_SAFE_CALL( cudaFree( _d_nbValidRequests ) );
	delete _d_validRequestMasks;

	delete _leafNodes;
	delete _emptyNodeVolume;

	cudaEventDestroy( _startProductionNodes );
	cudaEventDestroy( _stopProductionNodes );
	cudaEventDestroy( _startProductionBricks );
	cudaEventDestroy( _stopProductionBricks );
}

/******************************************************************************
 * This method is called before the rendering process. We just clear the request buffer.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::preRenderPass()
{
	CUDAPM_START_EVENT( gpucache_preRenderPass );

	// Clear the buffer of requests
	_updateBufferArray->fillAsync( 0 );	// TO DO : use a kernel instead of cudaMemSet(), copy engine could overlap

#ifdef GS_USE_MULTI_OBJECTS
	_objectIDBuffer->fillAsync( 0 );
#endif

	// Number of requests cache has handled
	//_nbNodeSubdivisionRequests = 0;
	//_nbBrickLoadRequests = 0;

#if CUDAPERFMON_CACHE_INFO==1
	_nodesCacheManager->_d_CacheStateBufferArray->fill( 0 );
	_nodesCacheManager->_numPagesUsed = 0;
	_nodesCacheManager->_numPagesWrited = 0;

	_bricksCacheManager->_d_CacheStateBufferArray->fill( 0 );
	_bricksCacheManager->_numPagesUsed = 0;
	_bricksCacheManager->_numPagesWrited = 0;
#endif

	CUDAPM_STOP_EVENT( gpucache_preRenderPass );
}

/******************************************************************************
 * This method is called after the rendering process. She's responsible for processing requests.
 *
 * @return the number of requests processed.
 *
 * @todo Check whether or not the inversion call of updateTimeStamps() with manageUpdates() has side effects
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
uint GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::handleRequests()
{
	// Measure the time taken by the last production.
	if ( _lastProductionTimed
			&& ( _nbBrickLoadRequests != 0 || _nbNodeSubdivisionRequests != 0 ) )
	{
		cudaEventSynchronize( _stopProductionBricks );
		float lastProductionNodesTime, lastProductionBricksTime;

		cudaEventElapsedTime( &lastProductionNodesTime, _startProductionNodes, _stopProductionNodes );
		cudaEventElapsedTime( &lastProductionBricksTime, _startProductionBricks, _stopProductionBricks );

		// Don't take too low number of requests into account (in this cases, the additional 
		// costs of launching the kernel, compacting the array... is greater than the 
		// brick/node production time)
		if ( _nbNodeSubdivisionRequests > 63 ) // hard-coded value...
		{
			_totalProducedNodes += _nbNodeSubdivisionRequests;
			_totalNodesProductionTime += lastProductionNodesTime;
		}

		if ( _nbBrickLoadRequests > 63 ) // hard-coded value...
		{
			_totalProducedBricks += _nbBrickLoadRequests;
			_totalBrickProductionTime += lastProductionBricksTime;
		}

		// Update the vector of statistics.
		struct GsProductionStatistics stats;
		//stats._frameId = ??? TODO
		stats._nNodes = _nbNodeSubdivisionRequests;
		stats._nodesProductionTime = lastProductionNodesTime;
		stats._nBricks = _nbBrickLoadRequests;
		stats._bricksProductionTime = lastProductionBricksTime;
		_productionStatistics.push_back( stats );	// BEWARE : stack limit
	}
	// _isProductionTimeLimited should not be used inside this function since it can be changed 
	// by the user at anytime and we need to have a constant value throughout the whole function.
	_lastProductionTimed = _isProductionTimeLimited;

	// TO DO
	// Check whether or not the inversion call of updateTimeStamps() with manageUpdates() has side effects

	// Generate the requests buffer
	//
	// Collect and compact update informations for both nodes and bricks
	/*CUDAPM_START_EVENT( dataProduction_manageRequests );
	uint nbRequests = manageUpdates();
	CUDAPM_STOP_EVENT( dataProduction_manageRequests );*/

	// Stop post-render pass if no request
	//if ( nbRequests > 0 )
	//{
		// Update time stamps
		//
		// - TO DO : the update time stamps could be done in parallel for nodes and bricks using streams
		CUDAPM_START_EVENT( cache_updateTimestamps );
		updateTimeStamps();
		CUDAPM_STOP_EVENT( cache_updateTimestamps );

		// Manage requests
		//
		// - TO DO : if updateTimestamps is before, this task could also be done in parallel using streams
		CUDAPM_START_EVENT( dataProduction_manageRequests );
		uint nbRequests = manageUpdates();
		CUDAPM_STOP_EVENT( dataProduction_manageRequests );

#ifdef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_DATAPRODUCTIONMANAGER
		// Get number of elements
		// BEWARE : synchronization to avoid an expensive final call to cudaDeviceSynchronize()
	#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_WITH_COMBINED_COPY
		size_t nbElementsTemp;
		_nodesCacheManager->updateTimeStampsCopy( _intraFramePass );
		_bricksCacheManager->updateTimeStampsCopy( _intraFramePass );
		GS_CUDA_SAFE_CALL( cudaMemcpy( &nbElementsTemp, _d_nbValidRequests, sizeof( size_t ), cudaMemcpyDeviceToHost ) );
		nbRequests = static_cast< uint >( nbElementsTemp );
	#else
		size_t nbElementsTemp[ 3 ];
		GS_CUDA_SAFE_CALL( cudaMemcpy( nbElementsTemp, _d_nbValidRequests, 3 * sizeof( size_t ), cudaMemcpyDeviceToHost ) );
		nbRequests = static_cast< uint >( nbElementsTemp[ 0 ] );
		// BEWARE : in nodes/bricks managers, real value should be _numElemsNotUsed = (uint)numElemsNotUsedST + inactiveNumElems
		_nodesCacheManager->_numElemsNotUsedST = static_cast< uint >( nbElementsTemp[ 1 ] );
		_bricksCacheManager->_numElemsNotUsedST = static_cast< uint >( nbElementsTemp[ 2 ] );
	#endif
		// Launch final "stream compaction" steps for "used" elements
		this->_numNodeTilesNotInUse = _nodesCacheManager->updateTimeStampsFinal( _intraFramePass );
		this->_numBricksNotInUse = _bricksCacheManager->updateTimeStampsFinal( _intraFramePass );
#endif

		// Handle requests :
#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_PRODUCER

		// [ 1 ] - Handle the "subdivide nodes" requests

		// Limit production according to the time limit.
		// First, do as if all the requests were node subdivisions
		if ( _lastProductionTimed )
		{
			if ( _totalProducedNodes != 0u )
			{
				nbRequests = min(
						nbRequests,
						static_cast< uint >( _productionTimeLimit * _totalProducedNodes / _totalNodesProductionTime ) );
			}
			cudaEventRecord( _startProductionNodes );
		}

		CUDAPM_START_EVENT( producer_nodes );
		_nbNodeSubdivisionRequests = manageNodeProduction( nbRequests );
		CUDAPM_STOP_EVENT( producer_nodes );

		if ( _lastProductionTimed )
		{
			cudaEventRecord( _stopProductionNodes );
			cudaEventRecord( _startProductionBricks );
		}

		//  [ 2 ] - Handle the "load/produce bricks" requests

		// Now, we know how many requests are node and how many are bricks, we can limit
		// the number of bricks requests according to the number of node requests performed.
		uint nbBricks = nbRequests;
		if ( _lastProductionTimed && _totalProducedNodes != 0 && _totalProducedBricks != 0 )
		{
			// Evaluate how much time will be left after nodes subdivision
			float remainingTime = _productionTimeLimit - _nbNodeSubdivisionRequests * _totalNodesProductionTime / _totalProducedNodes;
			// Limit the number of request to fit in the remaining time
			nbBricks = min(
					nbBricks,
					static_cast< uint >( remainingTime * _totalProducedBricks / _totalBrickProductionTime ) );
		}

		CUDAPM_START_EVENT( producer_bricks );
		if ( nbBricks > 0 )
		{
			_nbBrickLoadRequests = manageDataProduction( nbBricks );
		}
		CUDAPM_STOP_EVENT( producer_bricks );
#else
		if ( nbRequests > 0 ) {
			produceData( nbRequests );
		}
#endif
	//}

	// Tree data structure monitoring
	if ( _hasTreeDataStructureMonitoring )
	{
		//if ( _nbNodeSubdivisionRequests > 0 )
		//{
			// TEST
			dim3 blockSize( 128, 1, 1 );
			dim3 gridSize( ( _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z ) / 128 + 1, 1, 1 );
			GVKernel_TrackLeafNodes< typename TDataStructure::VolTreeKernelType ><<< gridSize, blockSize >>>( _dataStructure->volumeTreeKernel, _nodesCacheManager->_pageTable->getKernel()/*page table*/, ( _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z )/*nb nodes*/, _dataStructure->getMaxDepth()/*max depth*/, thrust::raw_pointer_cast( &( *this->_leafNodes )[ 0 ] ), thrust::raw_pointer_cast( &( *this->_emptyNodeVolume )[ 0 ] ) );
			_nbLeafNodes = thrust::reduce( (*_leafNodes).begin(), (*_leafNodes).end(), static_cast< unsigned int >( 0 ), thrust::plus< unsigned int >() );
			const float _emptyVolume = thrust::reduce( (*_emptyNodeVolume).begin(), (*_emptyNodeVolume).end(), static_cast< float >( 0.f ), thrust::plus< float >() );
			//std::cout << "------------------------------------------------" << _nbLeafNodes << std::endl;
			//std::cout << "Volume of empty nodes : " << ( _emptyVolume * 100.f ) << std::endl;
			//std::cout << "------------------------------------------------" << _nbLeafNodes << std::endl;
			//std::cout << "TOTAL number of leaf nodes : " << _nbLeafNodes << std::endl;
			GVKernel_TrackNodes< typename TDataStructure::VolTreeKernelType ><<< gridSize, blockSize >>>( _dataStructure->volumeTreeKernel, _nodesCacheManager->_pageTable->getKernel()/*page table*/, ( _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z )/*nb nodes*/, _dataStructure->getMaxDepth()/*max depth*/, thrust::raw_pointer_cast( &( *this->_leafNodes )[ 0 ] ), thrust::raw_pointer_cast( &( *this->_emptyNodeVolume )[ 0 ] ) );
			_nbNodes = thrust::reduce( (*_leafNodes).begin(), (*_leafNodes).end(), static_cast< unsigned int >( 0 ), thrust::plus< unsigned int >() );
			//std::cout << "TOTAL number of nodes : " << _nbNodes << std::endl;
		//}
		if ( _nbBrickLoadRequests > 0 )
		{
			//...
		}
	}

	if ( _lastProductionTimed && nbRequests > 0 )
	{
		cudaEventRecord( _stopProductionBricks );
	}

	return nbRequests;
}

/******************************************************************************
 * This method destroy the current N-tree and clear the caches.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::clearCache()
{
	// Launch Kernel
	dim3 blockSize( 32, 1, 1 ); // BEWARE : only works if nodetileSize < 32
	dim3 gridSize( 1, 1, 1 );
	// This clears node pool child and brick 1st nodetile after root node
	GsKernel_ClearVolTreeRoot<<< gridSize, blockSize >>>( _dataStructure->volumeTreeKernel, NodeTileRes::getNumElements() );
	GV_CHECK_CUDA_ERROR( "GsKernel_ClearVolTreeRoot" );
	// TODO:
	// - this is not good for Multi-Objects => it requires a modified or dedicated kernel
	// ...

	// Clear buffers holding mask of used and non-used elements
	CUDAPM_START_EVENT( gpucachemgr_clear_fillML )
	_dUnusedElementMasks->fillAsync( 0 );
	_dUsedElementMasks->fill( 0 );
	CUDAPM_STOP_EVENT( gpucachemgr_clear_fillML )
	
	// Reset nodes cache manager
	_nodesCacheManager->clearCache();
//#ifndef GS_USE_MULTI_OBJECTS
//	_nodesCacheManager->_totalNumLoads = 2;
//	_nodesCacheManager->_lastNumLoads = 1;
//#else
//	_nodesCacheManager->_totalNumLoads = 2 + 1;
//	_nodesCacheManager->_lastNumLoads = 1 + 1;
//#endif
	_nodesCacheManager->_totalNumLoads = 1/*protect first element*/ + _nbObjects;
	_nodesCacheManager->_lastNumLoads = _nbObjects;

	// Reset bricks cache manager
	_bricksCacheManager->clearCache();
	_bricksCacheManager->_totalNumLoads = 0;
	_bricksCacheManager->_lastNumLoads = 0;
}

/******************************************************************************
 * Get the associated device-side object
 *
 * @return The device-side object
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::DataProductionManagerKernelType GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::getKernelObject() const
{
	return _dataProductionManagerKernel;
}

/******************************************************************************
 * Get the update buffer
 *
 * @return The update buffer
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline GvCore::GsLinearMemory< uint >* GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getUpdateBuffer() const
{
	return _updateBufferArray;
}

/******************************************************************************
 * Get the nodes cache manager
 *
 * @return the nodes cache manager
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline const GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::NodesCacheManager*
GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::getNodesCacheManager() const
{
	return _nodesCacheManager;
}

/******************************************************************************
 * Get the bricks cache manager
 *
 * @return the bricks cache manager
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline const GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::BricksCacheManager*
GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::getBricksCacheManager() const
{
	return _bricksCacheManager;
}

/******************************************************************************
 * Get the nodes cache manager
 *
 * @return the nodes cache manager
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::NodesCacheManager*
GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::editNodesCacheManager()
{
	return _nodesCacheManager;
}

/******************************************************************************
 * Get the bricks cache manager
 *
 * @return the bricks cache manager
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::BricksCacheManager*
GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >::editBricksCacheManager()
{
	return _bricksCacheManager;
}

/******************************************************************************
 * ...
 ******************************************************************************/
//template< typename TDataStructure, typename GPUProducer, typename NodeTileRes, typename BrickFullRes >
//void VolTreeGPUCache< TDataStructure, GPUProducer/*, NodeTileRes, BrickFullRes*/ >::updateSymbols()
//{
//	CUDAPM_START_EVENT(gpucache_updateSymbols);
//
//	_nodesCacheManager->updateSymbols();
//	_bricksCacheManager->updateSymbols();
//
//	CUDAPM_STOP_EVENT(gpucache_updateSymbols);
//
//	_useBrickUsageOptim = true;
//	_intraFramePass = false;
//}

/******************************************************************************
 * Update time stamps
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::updateTimeStamps()
{
	// Ask nodes cache manager to update time stamps
	CUDAPM_START_EVENT(cache_updateTimestamps_dataStructure);
#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_DATAPRODUCTIONMANAGER
	this->_numNodeTilesNotInUse = _nodesCacheManager->updateTimeStamps( _intraFramePass );
#else
	_nodesCacheManager->updateTimeStamps( _intraFramePass );
#endif
	CUDAPM_STOP_EVENT(cache_updateTimestamps_dataStructure);

	// Ask bricks cache manager to update time stamps
	CUDAPM_START_EVENT(cache_updateTimestamps_bricks);
#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_DATAPRODUCTIONMANAGER
	this->_numBricksNotInUse = _bricksCacheManager->updateTimeStamps( _intraFramePass );
#else
	_bricksCacheManager->updateTimeStamps( _intraFramePass );
#endif
	CUDAPM_STOP_EVENT(cache_updateTimestamps_bricks);
}

/******************************************************************************
 * This method gather all requests by compacting the list.
 *
 * @return The number of elements in the requests list
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
uint GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::manageUpdates()
{
	uint nbElements = 0;

	uint totalNbElements = _nodePoolRes.x * _nodePoolRes.y * _nodePoolRes.z;
	
	// Optimisation test for case where the cache is not full
	if ( _nodesCacheManager->_totalNumLoads < _nodesCacheManager->getNumElements() )
	{
		totalNbElements = ( _nodesCacheManager->_totalNumLoads ) * NodeTileRes::getNumElements();

		// LOG
		//std::cout << "totalNbElements : " << totalNbElements << "/" << nodePoolRes.x * nodePoolRes.y * nodePoolRes.z << "\n";
	}

	CUDAPM_START_EVENT( dataProduction_manageRequests_elemsReduction );

	// Fill the buffer used to store node addresses updates with subdivision or load requests

	// Retrieve masks of valid and unvalid requests
	retrieveRequestMasks( totalNbElements );

	// Retrieve list of valid requests
	retrieveValidRequests( totalNbElements );

#ifdef GS_USE_MULTI_OBJECTS
	const bool result = GsCompute::GsDataParallelPrimitives::get().compact(
		/* OUT : compacted output */_objectIDBufferCompactList->getPointer( 0 ),
		/* OUT :  number of elements valid flags in the d_isValid input array */_d_nbValidRequests,
		/* input to compact */_objectIDBuffer->getPointer(),
		/* which elements in input are valid */_d_validRequestMasks->getPointer(),
		/* nb of elements in input */totalNbElements );
	GV_CHECK_CUDA_ERROR( "KERNEL manageUpdates::cudppCompact objectIDs" );
	assert( result );
#endif

	// If priority management is required, first retrieve the associated list of valid elements from the priority buffer
	if ( _hasProductionPriority && TPriorityPoliciesManager::usePriority() )
	{
		// Retrieve list of valid request priorities
		retrieveValidRequestPriorities( totalNbElements );
	}

#ifndef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_DATAPRODUCTIONMANAGER
	// Retrieve number of valid requests
	nbElements = retrieveNbValidRequests();
#endif

	// If priority management is required, then sort the valid elements from the priority buffer
	if ( _hasProductionPriority && TPriorityPoliciesManager::usePriority() )
	{
		// Sort requests using priorities
		sortRequestPriorities( nbElements );
	}

	CUDAPM_STOP_EVENT( dataProduction_manageRequests_elemsReduction );

	// Return number of valid requests
	return nbElements;
}

/******************************************************************************
 * This method handle the subdivisions requests.
 *
 * @param numUpdateElems the number of requests available in the buffer (of any kind).
 *
 * @return the number of subidivision requests processed.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
uint GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::manageNodeProduction( uint numUpdateElems )
{
	// Global buffer of requests of used elements only
	uint* updateCompactList = _updateBufferCompactList->getPointer( 0 );

	// Number of nodes to process
	const uint nbValidNodes = ( _nodesCacheManager->_totalNumLoads ) * NodeTileRes::getNumElements();

	// This will ask nodes producer to subdivide nodes
	assert( _producers.size() > 0 );
#ifndef GS_USE_MULTI_OBJECTS
	assert( _producers[ 0 ] != NULL );
	ProducerType* producer = _producers[ 0 ];
#else
	assert( _currentProducer != NULL );
	ProducerType* producer = _currentProducer;
#endif
	//return _nodesCacheManager->genericWrite(
	//	updateCompactList,
	//	numUpdateElems,
	//	/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_SUBDIV,
	//	_maxNbNodeSubdivisions,
	//	nbValidNodes,
	//	_dataStructure->_nodePool,
	//	producer );

	uint nbRequests = 0;

#ifndef GS_USE_MULTI_OBJECTS
	nbRequests = _nodesCacheManager->handleRequests(
		updateCompactList,
		numUpdateElems,
		/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_SUBDIV,
		_maxNbNodeSubdivisions,
		nbValidNodes );

	// Production of elements if any
	if ( nbRequests > 0 )
	{
		CUDAPM_START_EVENT_CHANNEL( 0, /*cacheId*/0, gpucache_nodes_subdivKernel );
		CUDAPM_START_EVENT_CHANNEL( 1, /*cacheId*/0, gpucache_bricks_gpuFetchBricks );
		CUDAPM_EVENT_NUMELEMS_CHANNEL( 1, /*cacheId*/0, gpucache_bricks_gpuFetchBricks, nbRequests );

		// Launch production
		producer->produceData(
			nbRequests, // number of elements to produce
			_nodesCacheManager->editElementRequests(), // list containing the addresses of the "nbRequests" nodes concerned
			_nodesCacheManager->editSortedElements(), // list containing "nbRequests" addresses where store the result
			Loki::Int2Type< 0 >() ); // cache Id

		CUDAPM_STOP_EVENT_CHANNEL( 0, /*cacheId*/0, gpucache_nodes_subdivKernel );
		CUDAPM_STOP_EVENT_CHANNEL( 1, /*cacheId*/0, gpucache_bricks_gpuFetchBricks );
	}
#else
	//for ( size_t i = 0; i < _producers.size(); i++ )
	//{
	//	_producers[ i ]->produceData( numElems, _nodesCacheManager->_d_UpdateCompactList, _d_elemAddressList, Loki::Int2Type< 0 >() );
	//}

	std::vector< unsigned int > objectIDs;
	objectIDs.resize( 2 );
	objectIDs[ 0 ] = 1;
	objectIDs[ 1 ] = 2;
	std::vector< unsigned int > nbRequestList;
	nbRequestList.resize( 2 );
	
	nbRequests = _nodesCacheManager->handleRequests(
		updateCompactList,
		numUpdateElems,
		/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_SUBDIV,
		_maxNbNodeSubdivisions,
		nbValidNodes,
		objectIDs, _objectIDBufferCompactList, nbRequestList );

	// Production of elements if any
	if ( nbRequests > 0 )
	{
		unsigned int offset = 0;
		for ( size_t i = 0; i < _producers.size(); i++ )
		{
			if ( _nodesCacheManager->_lastNumLoads )
			{
				// Retrieve production info
				_producers[ i ]->_productionInfo._nbElements = nbRequestList[ i ];
				_producers[ i ]->_productionInfo._offset = offset;

				// Update internal counter
				offset += nbRequestList[ i ];

				if ( _producers[ i ]->_productionInfo._nbElements > 0 )
				{
					// Launch production
					_producers[ i ]->produceData(
						_producers[ i ]->_productionInfo._nbElements,
						_nodesCacheManager->editElementRequests(),
						_nodesCacheManager->editSortedElements(),
						Loki::Int2Type< 0 >() );
				}
			}
		}
	}
#endif

	// TO DO:
	// NOTE => check if nbRequests is the right value to return
	// - maybe an internal process may decrease the number of elements to produce based on priority, time budget, etc...

	return nbRequests;
}

/******************************************************************************
 * This method handle the load requests.
 *
 * @param numUpdateElems the number of requests available in the buffer (of any kind).
 *
 * @return the number of load requests processed.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
uint GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::manageDataProduction( uint numUpdateElems )
{
	// Global buffer of requests of used elements only
	uint* updateCompactList = _updateBufferCompactList->getPointer( 0 );

	// Number of bricks to process
	const uint nbValidNodes = ( _nodesCacheManager->_totalNumLoads ) * NodeTileRes::getNumElements();

	// This will ask bricks producer to load/produce data
	assert( _producers.size() > 0 );
#ifndef GS_USE_MULTI_OBJECTS
	assert( _producers[ 0 ] != NULL );
	ProducerType* producer = _producers[ 0 ];
#else
	assert( _currentProducer != NULL );
	ProducerType* producer = _currentProducer;
#endif
	//return _bricksCacheManager->genericWrite(
	//	updateCompactList,
	//	numUpdateElems,
	//	/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_LOAD,
	//	_maxNbBrickLoads,
	//	nbValidNodes,
	//	_dataStructure->_dataPool,
	//	producer );

		uint nbRequests = 0;

#ifndef GS_USE_MULTI_OBJECTS
	nbRequests = _bricksCacheManager->handleRequests(
		updateCompactList,
		numUpdateElems,
		/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_LOAD,
		_maxNbBrickLoads,
		nbValidNodes );

	// Production of elements if any
	if ( nbRequests > 0 )
	{
		CUDAPM_START_EVENT_CHANNEL( 0, /*cacheId*/1, gpucache_nodes_subdivKernel );
		CUDAPM_START_EVENT_CHANNEL( 1, /*cacheId*/1, gpucache_bricks_gpuFetchBricks );
		CUDAPM_EVENT_NUMELEMS_CHANNEL( 1, /*cacheId*/1, gpucache_bricks_gpuFetchBricks, nbRequests );

		// Launch production
		producer->produceData(
			nbRequests, // number of elements to produce
			_bricksCacheManager->editElementRequests(), // list containing the addresses of the "nbRequests" nodes concerned
			_bricksCacheManager->editSortedElements(), // list containing "nbRequests" addresses where store the result
			Loki::Int2Type< 1 >() ); // cache Id

		CUDAPM_STOP_EVENT_CHANNEL( 0, /*cacheId*/1, gpucache_nodes_subdivKernel );
		CUDAPM_STOP_EVENT_CHANNEL( 1, /*cacheId*/1, gpucache_bricks_gpuFetchBricks );
	}
#else
	//for ( size_t i = 0; i < _producers.size(); i++ )
	//{
	//	_producers[ i ]->produceData( numElems, _bricksCacheManager->_d_UpdateCompactList, _d_elemAddressList, Loki::Int2Type< 0 >() );
	//}

	std::vector< unsigned int > objectIDs;
	objectIDs.resize( _nbObjects );
	for ( unsigned int i = 0; i < _nbObjects; i++ )
	{
		objectIDs[ i ] = i + 1;
	}
	std::vector< unsigned int > nbRequestList;
	nbRequestList.resize( _nbObjects );

	nbRequests = _bricksCacheManager->handleRequests(
		updateCompactList,
		numUpdateElems,
		/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_LOAD,
		_maxNbBrickLoads,
		nbValidNodes,
		objectIDs, _objectIDBufferCompactList, nbRequestList );

	// Production of elements if any
	if ( nbRequests > 0 )
	{
		unsigned int offset = 0;
		for ( size_t i = 0; i < _producers.size(); i++ )
		{
			if ( _bricksCacheManager->_lastNumLoads )
			{
				// Retrieve production info
				_producers[ i ]->_productionInfo._nbElements = nbRequestList[ i ];
				_producers[ i ]->_productionInfo._offset = offset;

				// Update internal counter
				offset += nbRequestList[ i ];

				if ( _producers[ i ]->_productionInfo._nbElements > 0 )
				{
					// Launch production
					_producers[ i ]->produceData(
						_producers[ i ]->_productionInfo._nbElements,
						_bricksCacheManager->editElementRequests(),
						_bricksCacheManager->editSortedElements(),
						Loki::Int2Type< 1 >() );
				}
			}
		}
	}
#endif

	// TO DO:
	// NOTE => check if nbRequests is the right value to return
	// - maybe an internal process may decrease the number of elements to produce based on priority, time budget, etc...

	return nbRequests;
}

#ifdef GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_PRODUCER
/******************************************************************************
 * This method handle the subdivisions requests.
 *
 * @param pGlobalNbRequests the number of requests available in the buffer (of any kind).
 *
 * @return the number of subidivision requests processed.
 ******************************************************************************/
template< typename TDataStructure >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::produceData( uint pGlobalNbRequests )
{
	// _nbNodeSubdivisionRequests = manageNodeProduction( pGlobalNbRequests );

	// Global buffer of requests of used elements only
	uint* updateCompactList = _updateBufferCompactList->getPointer( 0 );

	// Number of nodes to process
	uint nbValidNodes = ( _nodesCacheManager->_totalNumLoads ) * NodeTileRes::getNumElements();

	// This will ask nodes producer to subdivide nodes
	assert( _producers.size() > 0 );
	assert( _producers[ 0 ] != NULL );
	ProducerType* producer = _producers[ 0 ];

	_nodesCacheManager->handleRequests( updateCompactList, pGlobalNbRequests,
									/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_SUBDIV, 
									_maxNbNodeSubdivisions, nbValidNodes, _dataStructure->_nodePool, producer );

	_bricksCacheManager->handleRequests( updateCompactList, pGlobalNbRequests,
									/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_LOAD, 
									_maxNbBrickLoads, nbValidNodes, _dataStructure->_dataPool, producer );

	// Get number of elements
	size_t numElems[ 2 ];
	GS_CUDA_SAFE_CALL( cudaMemcpy( &numElems, _d_nbValidRequests + 1, 2 * sizeof( size_t ), cudaMemcpyDeviceToHost ) );

	// Limit production according to the time limit.
	// First, consider all the request are node subdivision
	if ( _lastProductionTimed )
	{
		if ( _totalProducedNodes != 0u )
		{
			numElems[ 0 ] = min(
					static_cast< uint >( numElems[ 0 ] ),
					max( 100,
						(uint)( _productionTimeLimit * _totalProducedNodes / _totalNodesProductionTime ) ) );
		}
		cudaEventRecord( _startProductionNodes );
	}

	// Subdivide
	_nbNodeSubdivisionRequests = _nodesCacheManager->handleRequestsAsync( updateCompactList, pGlobalNbRequests,
									/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_SUBDIV,
									_maxNbNodeSubdivisions, nbValidNodes, _dataStructure->_nodePool, producer,
									static_cast< uint >( numElems[ 0 ] ) );

	if ( _lastProductionTimed )
	{
		cudaEventRecord( _stopProductionNodes );
		cudaEventRecord( _startProductionBricks );
	}

	if ( _nbNodeSubdivisionRequests < pGlobalNbRequests )
	{
		if ( _lastProductionTimed && _totalProducedNodes != 0 && _totalProducedBricks != 0 )
		{
			// Evaluate how much time will be left after nodes subdivision
			float remainingTime = _productionTimeLimit - _nbNodeSubdivisionRequests * _totalNodesProductionTime / _totalProducedNodes;
			// Limit the number of request to fit in the remaining time
			numElems[ 1 ] = min(
					static_cast< uint >( numElems[ 1 ] ),
					max( 100,
						(uint)( remainingTime * _totalProducedBricks / _totalBrickProductionTime ) ) );
		}

		_nbBrickLoadRequests = _bricksCacheManager->handleRequestsAsync( updateCompactList, pGlobalNbRequests,
									/*type of request to handle*/DataProductionManagerKernelType::VTC_REQUEST_LOAD,
									_maxNbBrickLoads, nbValidNodes, _dataStructure->_dataPool, producer,
									static_cast< uint >( numElems[ 1 ] ) );
	}
}
#endif

/******************************************************************************
 * Get the max number of requests of node subdivisions.
 *
 * @return the max number of requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
uint GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getMaxNbNodeSubdivisions() const
{
	return _maxNbNodeSubdivisions;
}

/******************************************************************************
 * Set the max number of requests of node subdivisions.
 *
 * @param pValue the max number of requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setMaxNbNodeSubdivisions( uint pValue )
{
	_maxNbNodeSubdivisions = pValue;
}

/******************************************************************************
 * Get the max number of requests of brick of voxel loads.
 *
 * @return the max number of requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
uint GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getMaxNbBrickLoads() const
{
	return _maxNbBrickLoads;
}

/******************************************************************************
 * Set the max number of requests of brick of voxel loads.
 *
 * @param pValue the max number of requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setMaxNbBrickLoads( uint pValue )
{
	_maxNbBrickLoads = pValue;
}

/******************************************************************************
 * Get the number of requests of node subdivisions the cache has handled.
 *
 * @return the number of requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
unsigned int GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getNbNodeSubdivisionRequests() const
{
	return this->_nbNodeSubdivisionRequests;
}

/******************************************************************************
 * Get the number of requests of brick of voxel loads the cache has handled.
 *
 * @return the number of requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
unsigned int GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getNbBrickLoadRequests() const
{
	return this->_nbBrickLoadRequests;
}

/******************************************************************************
 * Add a producer
 *
 * @param pProducer the producer to add
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::addProducer( ProducerType* pProducer )
{
	assert( pProducer != NULL );

	// TO DO
	// - do it properly...
	setCurrentProducer( pProducer );

	// TO DO
	// ...
	_producers.push_back( pProducer );
}

/******************************************************************************
 * Remove a producer
 *
 * @param pProducer the producer to remove
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::removeProducer( ProducerType* pProducer )
{
	assert( pProducer != NULL );

	// TO DO
	// ...
	assert( false );
}

/******************************************************************************
 * Set the current producer
 *
 * @param pProducer The producer
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setCurrentProducer( ProducerType* pProducer )
{
	assert( pProducer != NULL );	// TO DO, maybe having NULL producer could be interesting to deactivate, or maybe boolean would be better ?

	// TO DO
	// - do it properly...
	_currentProducer = pProducer;
}

/******************************************************************************
 * Get the flag telling whether or not tree data dtructure monitoring is activated
 *
 * @return the flag telling whether or not tree data dtructure monitoring is activated
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline bool GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::hasTreeDataStructureMonitoring() const
{
	return _hasTreeDataStructureMonitoring;
}

/******************************************************************************
 * Set the flag telling whether or not tree data dtructure monitoring is activated
 *
 * @param pFlag the flag telling whether or not tree data dtructure monitoring is activated
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setTreeDataStructureMonitoring( bool pFlag )
{
	_hasTreeDataStructureMonitoring = pFlag;
}

/******************************************************************************
 * Get the flag telling whether or not cache has exceeded its capacity
 *
 * @return flag telling whether or not cache has exceeded its capacity
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline bool GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::hasCacheExceededCapacity() const
{
	assert( _nodesCacheManager != NULL );
	assert( _bricksCacheManager != NULL );

	return ( _nodesCacheManager->hasExceededCapacity() || _bricksCacheManager->hasExceededCapacity() );
}

/******************************************************************************
 * This method is called to serialize an object
 *
 * @param pStream the stream where to write
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::write( std::ostream& pStream ) const
{
	// Node cache
	assert( _nodesCacheManager != NULL );
	if ( _nodesCacheManager != NULL )
	{
		_nodesCacheManager->write( pStream );
	}

	// Data cache
	assert( _bricksCacheManager != NULL );
	if ( _bricksCacheManager != NULL )
	{
		_bricksCacheManager->write( pStream );
	}
}

/******************************************************************************
 * This method is called deserialize an object
 *
 * @param pStream the stream from which to read
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::read( std::istream& pStream )
{
	// Node cache
	_nodesCacheManager->read( pStream );

	// Data cache
	_bricksCacheManager->read( pStream );
}

/******************************************************************************
 * Get the flag telling whether or not the production time limit is activated.
 *
 * @return the flag telling whether or not the production time limit is activated.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
bool GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::isProductionTimeLimited() const
{
	return _isProductionTimeLimited;
}

/******************************************************************************
 * Set or unset the flag used to tell whether or not the production time is limited.
 *
 * @param pFlag the flag value.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::useProductionTimeLimit( bool pFlag )
{
	_isProductionTimeLimited = pFlag;
}

/******************************************************************************
 * Get the time limit actually in use.
 *
 * @return the time limit.
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
float GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getProductionTimeLimit() const
{
	return _productionTimeLimit;
}

/******************************************************************************
 * Set the time limit for the production.
 *
 * @param pTime the time limit (in ms).
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setProductionTimeLimit( float pTime )
{
	_productionTimeLimit = pTime;
}

/******************************************************************************
 * Tell whether or not to use priority at production
 *
 * @return a flag telling whether or not to use priority at production
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
bool GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::hasProductionPriority() const
{
	return _hasProductionPriority;
}

/******************************************************************************
 * Set the flag telling whether or not to use priority at production
 *
 * @param pFlag a flag telling whether or not to use priority at production
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setProductionPriority( bool pFlag )
{
	_hasProductionPriority = pFlag;
}

/******************************************************************************
 * Retrieve masks of valid and unvalid requests
 *
 * @param pNbElements number of elements to process
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::retrieveRequestMasks( unsigned int pNbElements )
{
	// Fill the buffer used to store node addresses updates with subdivision or load requests

	// Set kernel execution configuration
	const dim3 blockSize( 64, 1, 1 ); // TODO : compute size based on warpSize and multiProcessorCount instead
	const uint nbBlocks = iDivUp( pNbElements, blockSize.x );
	const dim3 gridSize = dim3( std::min( nbBlocks, 65535U ) , iDivUp( nbBlocks, 65535U ), 1 );

	// This kernel creates the usage mask list of used and un-used elements (in current pass) in a single pass
	GvKernel_PreProcessRequests<<< gridSize, blockSize, 0 >>>(
		/*input*/_updateBufferArray->getPointer(),
		/*output*/_d_validRequestMasks->getPointer(),
		/*input*/pNbElements );
	GV_CHECK_CUDA_ERROR( "GvKernel_PreProcessRequests" );
}

/******************************************************************************
 * Retrieve list of valid requests
 *
 * @param pNbElements number of elements to process
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::retrieveValidRequests( unsigned int pNbElements )
{
	// TO DO
	//
	// Optimization
	//
	// Check if, like in Thrust, we could use directly use _updateBufferArray as a predicate (i.e. masks)
	// - does cudpp requires an array of "1"/"0" or "0"/"!0" like in thrust with GvCore::not_equal_to_zero< uint >()
	// - if yes, the GvKernel_PreProcessRequests kernel call could be avoided and _updateBufferArray used as input mask in cudppCompact
	// - ok, cudpp, only check for value > 0 and not == 1, so it could be tested, just check if a speed can occur

	// Given an array d_in and an array of 1/0 flags in deviceValid, returns a compacted array in d_out of corresponding only the "valid" values from d_in.
	//
	// Takes as input an array of elements in GPU memory (d_in) and an equal-sized unsigned int array in GPU memory (deviceValid) that indicate which of those input elements are valid.
	// The output is a packed array, in GPU memory, of only those elements marked as valid.
	//
	// Internally, uses cudppScan.
	const bool result = GsCompute::GsDataParallelPrimitives::get().compact(
		/* OUT : compacted output */_updateBufferCompactList->getPointer( 0 ),
		/* OUT : number of elements valid flags in the d_isValid input array */_d_nbValidRequests,
		/* input to compact */_updateBufferArray->getPointer(),
		/* which elements in input are valid */_d_validRequestMasks->getPointer(),
		/* nb of elements in input */pNbElements );
	GV_CHECK_CUDA_ERROR( "KERNEL manageUpdates::cudppCompact" );
	assert( result );
}

/******************************************************************************
 * Retrieve number of valid requests
 *
 * @return the number of valid requests
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
unsigned int GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::retrieveNbValidRequests()
{
	size_t nbValidRequests;
	GS_CUDA_SAFE_CALL( cudaMemcpy( &nbValidRequests, _d_nbValidRequests, sizeof( size_t ), cudaMemcpyDeviceToHost ) );
	
	return static_cast< uint >( nbValidRequests );
}

/******************************************************************************
 * Retrieve list of valid request priorities (for production)
 *
 * @param pNbElements number of elements to process
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::retrieveValidRequestPriorities( unsigned int pNbElements )
{
	// If priority management is required, first retrieve the associated list of valid elements from the priority buffer
	const bool result = GsCompute::GsDataParallelPrimitives::get().compact(
		/* OUT : compacted output */_priorityBufferCompactList->getPointer( 0 ),
		/* OUT : number of elements valid flags in the d_isValid input array */_d_nbValidRequests,
		/* input to compact */_priorityBufferArray->getPointer(),
		/* which elements in input are valid */_d_validRequestMasks->getPointer(),
		/* nb of elements in input */pNbElements );
	GV_CHECK_CUDA_ERROR( "KERNEL manageUpdates::cudppCompact" );
	assert( result );
}

/******************************************************************************
 * Sort requests using priorities (for production)
 *
 * @param pNbElements number of elements to process
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::sortRequestPriorities( unsigned int pNbElements )
{
	// If priority management is required, then sort the valid elements from the priority buffer
	// Sort element using priority
	const bool result = GsCompute::GsDataParallelPrimitives::get().sort(
		_priorityBufferCompactList->getPointer( 0 ), // Keys
		_updateBufferCompactList->getPointer( 0 ),	// Values
		pNbElements ); // Nb elems
	GV_CHECK_CUDA_ERROR( "KERNEL manageUpdates::cudppRadixSort" );
	assert( result );
}

/******************************************************************************
 * Get the number of objects handled
 *
 * @return the number of objects handled
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
inline unsigned int GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::getNbObjects( unsigned int pValue )
{
	return _nbObjects;
}

/******************************************************************************
 * Set the number of objects to be handled
 *
 * @param pValue the number of objects to be handled
 ******************************************************************************/
template< typename TDataStructure, typename TPriorityPoliciesManager >
void GsDataProductionManager< TDataStructure, TPriorityPoliciesManager >
::setNbObjects( unsigned int pValue )
{
	_nbObjects = pValue;
}

} // namespace GvStructure
