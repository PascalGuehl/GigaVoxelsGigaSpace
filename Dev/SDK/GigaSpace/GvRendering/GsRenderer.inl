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

//// GigaVoxels
//#include "GvPerfMon/GsPerformanceTimer.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvRendering
{

/******************************************************************************
 * Constructor
 *
 * @param pDataStructure the data stucture to render.
 * @param pCache the cache used to store the data structure and handle produce data requests efficiently.
 * It handles requests emitted during rendering phase (node subdivisions and brick loads).
 * @param pProducer the producer used to provide data following requests emitted during rendering phase
 * (node subdivisions and brick loads).
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
GsRenderer< TDataStructureType, VolumeTreeCacheType >
::GsRenderer( TDataStructureType* pDataStructure, VolumeTreeCacheType* pCache )
:	GsIRenderer()
,	_timeBudget( 0.f )
,	_performanceTimer( NULL )
{
	assert( pDataStructure );
	assert( pCache );

	this->_volumeTree		= pDataStructure;
	this->_volumeTreeCache	= pCache;

	_updateQuality			= 0.3f;
	_generalQuality			= 1.0f;

	// This method update the associated "constant" in device memory
	setVoxelSizeMultiplier(	1.0f );

	_currentTime			= 10;
	
	// By default, during data structure traversal, request for load bricks strategy first
	// (not node subdivision first)
	_hasPriorityOnBricks	= true;

	// Specify clear values for the color and depth buffers
	_clearColor = make_uchar4( 0, 0, 0, 0 );
	_clearDepth = 1.f;

	// Projected 2D Bounding Box of the GigaVoxels 3D BBox.
	_projectedBBox = make_uint4( 0, 0, 0, 0 );

	// Time budget (in milliseconds)
	//
	// 60 fps
	_timeBudget = 1.f / 60.f;
	_performanceTimer = new GvPerfMon::GsPerformanceTimer();
	_timerEvent = _performanceTimer->createEvent();
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
GsRenderer< TDataStructureType, VolumeTreeCacheType >
::~GsRenderer()
{
	delete _performanceTimer;
}

/******************************************************************************
 * Returns the current value of the general quality.
 *
 * @return the current value of the general quality
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
float GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getGeneralQuality() const
{
	return _generalQuality;
}

/******************************************************************************
 * Modify the current value of the general quality.
 *
 * @param pValue the current value of the general quality
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setGeneralQuality( float pValue )
{
	_generalQuality = pValue;
}

/******************************************************************************
 * Returns the current value of the update quality.
 *
 * @return the current value of the update quality
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
float GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getUpdateQuality() const
{
	return _updateQuality;
}

/******************************************************************************
 * Modify the current value of the update quality.
 *
 * @param pValue the current value of the update quality
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setUpdateQuality( float pValue )
{
	_updateQuality = pValue;
}

/******************************************************************************
 * Update the stored current time that represents the number of elapsed frames.
 * Increment by one.
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
inline void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::nextFrame()
{
	_currentTime++;
}

/******************************************************************************
 * Tell if, during data structure traversal, priority of requests is set on brick
 * loads or on node subdivisions first.
 *
 * @return the flag indicating the request strategy
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
bool GsRenderer< TDataStructureType, VolumeTreeCacheType >
::hasPriorityOnBricks() const
{
	return _hasPriorityOnBricks;
}

/******************************************************************************
 * Set the request strategy indicating if, during data structure traversal,
 * priority of requests is set on brick loads or on node subdivisions first.
 *
 * @param pFlag the flag indicating the request strategy
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setPriorityOnBricks( bool pFlag )
{
	_hasPriorityOnBricks = pFlag;
}

/******************************************************************************
 * Specify clear values for the color buffers
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
const uchar4& GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getClearColor() const
{
	return _clearColor;
}

/******************************************************************************
 * Specify clear values for the color buffers
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setClearColor( const uchar4& pColor )
{
	_clearColor = pColor;
}

/******************************************************************************
 * Specify the clear value for the depth buffer
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
float GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getClearDepth() const
{
	return _clearDepth;
}

/******************************************************************************
 * Specify the clear value for the depth buffer
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setClearDepth( float pDepth )
{
	_clearDepth = pDepth;
}

/******************************************************************************
 * Get the voxel size multiplier
 *
 * @return the voxel size multiplier
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
float GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getVoxelSizeMultiplier() const
{
	return _voxelSizeMultiplier;
}

/******************************************************************************
 * Set the voxel size multiplier
 *
 * @param pValue the voxel size multiplier
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setVoxelSizeMultiplier( float pValue )
{
	_voxelSizeMultiplier = pValue;

	// Update CUDA memory with value
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_voxelSizeMultiplier, &_voxelSizeMultiplier, sizeof( _voxelSizeMultiplier ), 0, cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * Projected 2D Bounding Box of the GigaVoxels 3D BBox.
 * It holds its bottom left corner and its size.
 * ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
 *
 * @return The projected 2D Bounding Box of the GigaVoxels 3D BBox
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
const uint4& GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getProjectedBBox() const
{
	return _projectedBBox;
}

/******************************************************************************
 * Projected 2D Bounding Box of the GigaVoxels 3D BBox.
 * It holds its bottom left corner and its size.
 * ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
 *
 * @param pProjectedBBox The projected 2D Bounding Box of the GigaVoxels 3D BBox
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setProjectedBBox( const uint4& pProjectedBBox )
{
	_projectedBBox = pProjectedBBox;
}

/******************************************************************************
 * Get the time budget
 *
 * @return the time budget
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
inline float GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getTimeBudget() const
{
	return _timeBudget;
}

/******************************************************************************
 * Set the time budget
 *
 * @param pValue the time budget
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
inline void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::setTimeBudget( float pValue )
{
	_timeBudget = pValue;
}

/******************************************************************************
 * Start the timer
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
inline void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::startTimer()
{
	return _performanceTimer->startEvent( _timerEvent );
}

/******************************************************************************
 * Stop the timer
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
inline void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::stopTimer()
{
	return _performanceTimer->stopEvent( _timerEvent );
}

/******************************************************************************
 * This method return the duration of the timer event between start and stop event
 *
 * @return the duration of the event in milliseconds
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
float GsRenderer< TDataStructureType, VolumeTreeCacheType >
::getElapsedTime()
{
	return _performanceTimer->getEventDuration( _timerEvent );
}

/******************************************************************************
 * This method is called to serialize an object
 *
 * @param pStream the stream where to write
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::write( std::ostream& pStream ) const
{
}

/******************************************************************************
 * This method is called deserialize an object
 *
 * @param pStream the stream from which to read
 ******************************************************************************/
template< typename TDataStructureType, typename VolumeTreeCacheType >
void GsRenderer< TDataStructureType, VolumeTreeCacheType >
::read( std::istream& pStream )
{
}


} // namespace GvRendering
