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

#ifndef _GV_RENDERER_H_
#define _GV_RENDERER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvRendering/GsIRenderer.h"
#include "GvPerfMon/GsPerformanceTimer.h"

// Cuda
#include <vector_types.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// TO DO : faut-il, peut-on, mettre cette variable dans le namespace ?
/**
 * ...
 */
extern uint GsCacheManager_currentTime;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaVoxels
//namespace GvPerfMon
//{
//	class GsPerformanceTimer;
//}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvRendering
{
	
/** 
 * @class GsRenderer
 *
 * @brief The GsRenderer class provides the base interface to render a N-tree data structure.
 *
 * This class is used to render a data structure with the help of a cache and a producer.
 * While rendering (ray-tracing phase), requests are emitted to obtain missing data :
 * - node subdivisions,
 * - and brick loads.
 *
 * @param TDataStructureType The data stucture to render
 * @param VolumeTreeCacheType The cache used to store the data structure and handle produce data requests efficiently
 * (node subdivisions and brick loads)
 * @param ProducerType The producer used to provide data following requests emitted during rendering phase
 * (node subdivisions and brick loads)
 */
template< typename TDataStructureType, typename VolumeTreeCacheType/*, typename TRendererKernelType*/ >
class GsRenderer : public GsIRenderer
// TO DO
// - it seems the 2 templates can be removed ?
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/
	
	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Destructor
	 */
	virtual ~GsRenderer();

	/**
	 * Returns the current value of the general quality.
	 *
	 * @return the current value of the general quality
	 */
	float getGeneralQuality() const;

	/**
	 * Modify the current value of the general quality.
	 *
	 * @param pValue the current value of the general quality
	 */
	void setGeneralQuality( float pValue );

	/**
	 * Returns the current value of the update quality.
	 *
	 * @return the current value of the update quality
	 */
	float getUpdateQuality() const;

	/**
	 * Modify the current value of the update quality.
	 *
	 * @param pValue the current value of the update quality
	 */
	void setUpdateQuality( float pValue );

	/**
	 * Update the stored current time that represents the number of elapsed frames.
	 * Increment by one.
	 */
	inline void nextFrame();

	/**
	 * Tell if, during data structure traversal, priority of requests is set on brick
	 * loads or on node subdivisions first.
	 *
	 * @return the flag indicating the request strategy
	 */
	bool hasPriorityOnBricks() const;

	/**
	 * Set the request strategy indicating if, during data structure traversal,
	 * priority of requests is set on brick loads or on node subdivisions first.
	 *
	 * @param pFlag the flag indicating the request strategy
	 */
	void setPriorityOnBricks( bool pFlag );

	/**
	 * Specify clear values for the color buffers
	 */
	const uchar4& getClearColor() const;
	void setClearColor( const uchar4& pColor );

	/**
	 * Specify the clear value for the depth buffer
	 */
	float getClearDepth() const;
	void setClearDepth( float pDepth );

	/**
	 * Get the voxel size multiplier
	 *
	 * @return the voxel size multiplier
	 */
	float getVoxelSizeMultiplier() const;

	/**
	 * Set the voxel size multiplier
	 *
	 * @param the voxel size multiplier
	 */
	void setVoxelSizeMultiplier( float pValue );

	/**
	 * Projected 2D Bounding Box of the GigaVoxels 3D BBox.
	 * It holds its bottom left corner and its size.
	 * ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
	 *
	 * @return The projected 2D Bounding Box of the GigaVoxels 3D BBox
	 */
	const uint4& getProjectedBBox() const;

	/**
	 * Projected 2D Bounding Box of the GigaVoxels 3D BBox.
	 * It holds its bottom left corner and its size.
	 * ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
	 *
	 * @param pProjectedBBox The projected 2D Bounding Box of the GigaVoxels 3D BBox
	 */
	void setProjectedBBox( const uint4& pProjectedBBox );

	/**
	 * Get the time budget
	 *
	 * @return the time budget
	 */
	float getTimeBudget() const;

	/**
	 * Set the time budget
	 *
	 * @param pValue the time budget
	 */
	void setTimeBudget( float pValue );

	/**
	 * Start the timer
	 */
	void startTimer();

	/**
	 * Stop the timer
	 */
	void stopTimer();

	/**
	 * This method return the duration of the timer event between start and stop event
	 *
	 * @return the duration of the event in milliseconds
	 */
	float getElapsedTime();

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

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Current time.
	 * This represents the number of elapsed frames.
	 */
	uint _currentTime;

	/**
	 * Data stucture to render.
	 */
	TDataStructureType* _volumeTree;

	/**
	 * Cache used to store and data structure efficiently.
	 * It handles requests emitted during rendering phase
	 * (node subdivisions and brick loads).
	 */
	VolumeTreeCacheType* _volumeTreeCache;

	/**
	 * General quality value
	 *
	 * @todo explain
	 */
	float _generalQuality;

	/**
	 * Update quality value
	 *
	 * @todo explain
	 */
	float _updateQuality;

	/**
	 * Flag to tell if, during data structure traversal, priority is set on bricks or nodes.
	 * I.e request for a node subdivide strategy first or load bricks strategy first.
	 */
	bool _hasPriorityOnBricks;

	/**
	 * Specify clear values for the color buffers
	 */
	uchar4 _clearColor;

	/**
	 * Specify the clear value for the depth buffer
	 */
	float _clearDepth;

	/**
	 * Voxel size multiplier
	 */
	float _voxelSizeMultiplier;

	/**
	 * Projected 2D Bounding Box of the GigaVoxels 3D BBox.
	 * It holds its bottom left corner and its size.
	 * ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
	 */
	uint4 _projectedBBox;

	/**
	 * Time budget (in milliseconds)
	 */
	float _timeBudget;

	/**
	 * Performance timer
	 */
	GvPerfMon::GsPerformanceTimer* _performanceTimer;
	GvPerfMon::GsPerformanceTimer::Event _timerEvent;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param pDataStructure the data stucture to render.
	 * @param pCache the cache used to store the data structure and handle produce data requests efficiently.
	 * It handles requests emitted during rendering phase (node subdivisions and brick loads).
	 * @param pProducer the producer used to provide data following requests emitted during rendering phase
	 * (node subdivisions and brick loads).
	 */
	GsRenderer( TDataStructureType* pDataStructure, VolumeTreeCacheType* pCache );

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
	GsRenderer( const GsRenderer& );

	/**
	 * Copy operator forbidden.
	 */
	GsRenderer& operator=( const GsRenderer& );

};

} // namespace GvRendering

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsRenderer.inl"

#endif // !_GV_RENDERER_H_
