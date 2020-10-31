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

// System
#include <cstring>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvPerfMon
{

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsDevicePerformanceTimer::GsDevicePerformanceTimer()
{
}

/******************************************************************************
 * Desconstructor
 ******************************************************************************/
GsDevicePerformanceTimer::~GsDevicePerformanceTimer()
{
}

/******************************************************************************
 * This method create and initialize a new Event object.
 *
 * @return the created event
 ******************************************************************************/
inline GsDevicePerformanceTimer::Event GsDevicePerformanceTimer::createEvent() const
{
	GsDevicePerformanceTimer::Event evt;

	cudaEventCreate( &evt.cudaTimerStartEvt );
	cudaEventCreate( &evt.cudaTimerStopEvt );
	cudaDeviceSynchronize();
	
	GV_CHECK_CUDA_ERROR( "GsDevicePerformanceTimer::createEvent" );
	
	// Initialize values to zero
	::memset( evt.timersArray, 0, sizeof( evt.timersArray ) );

	return evt;
}

/******************************************************************************
 * This method set the start time of the given event to the current time.
 *
 * @param pEvent a reference to the event.
 ******************************************************************************/
inline void GsDevicePerformanceTimer::startEvent( GsDevicePerformanceTimer::Event& pEvent ) const
{
	cudaEventRecord( pEvent.cudaTimerStartEvt, 0 );
}

/******************************************************************************
 * This method set the stop time of the given event to the current time.
 *
 * @param pEvent a reference to the event.
 ******************************************************************************/
inline void GsDevicePerformanceTimer::stopEvent( GsDevicePerformanceTimer::Event& pEvent ) const
{
	cudaEventRecord( pEvent.cudaTimerStopEvt, 0 );
}

/******************************************************************************
 * This method return the duration of the given event
 *
 * @param pEvent a reference to the event.
 *
 * @return the duration of the event in milliseconds
 ******************************************************************************/
inline float GsDevicePerformanceTimer::getEventDuration( GsDevicePerformanceTimer::Event& pEvent ) const
{
	float time;
	cudaEventElapsedTime( &time, pEvent.cudaTimerStartEvt, pEvent.cudaTimerStopEvt );

	return time;
}

/******************************************************************************
 * This method return the difference between the begin of two events
 *
 * @param pEvent0 a reference to the first event
 * @param pEvent1 a reference to the second event
 *
 * @return the difference between the two events in milliseconds
 ******************************************************************************/
inline float GsDevicePerformanceTimer::getStartToStartTime( GsDevicePerformanceTimer::Event& pEvent0, GsDevicePerformanceTimer::Event& pEvent1 ) const
{
	float time;
	cudaEventElapsedTime( &time, pEvent0.cudaTimerStartEvt, pEvent1.cudaTimerStartEvt );

	return time;
}

} // namespace GvPerfMon
