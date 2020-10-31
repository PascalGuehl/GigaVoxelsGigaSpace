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

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvPerfMon
{

/******************************************************************************
 * This method create and initialize a new Event object.
 *
 * @return the created event
 ******************************************************************************/
inline GsPerformanceTimer::Event GsPerformanceTimer::createEvent() const
{
	GsPerformanceTimer::Event evt;

	getHighResolutionTime( &evt.timerStartEvt );
	getHighResolutionTime( &evt.timerStopEvt );

	return evt;
}

/******************************************************************************
 * This method set the start time of the given event to the current time.
 *
 * @param evt a reference to the event.
 ******************************************************************************/
inline void GsPerformanceTimer::startEvent( GsPerformanceTimer::Event& pEvent ) const
{
#if CUDAPERFTIMERCPU_GPUSYNC
	cudaDeviceSynchronize();
#endif
	getHighResolutionTime( &pEvent.timerStartEvt );
}

/******************************************************************************
 * This method set the stop time of the given event to the current time.
 *
 * @param evt a reference to the event.
 ******************************************************************************/
inline void GsPerformanceTimer::stopEvent( GsPerformanceTimer::Event& pEvent ) const
{
#if CUDAPERFTIMERCPU_GPUSYNC
	cudaDeviceSynchronize();
#endif

	getHighResolutionTime( &pEvent.timerStopEvt );
}

/******************************************************************************
 * This method return the duration of the given event
 *
 * @param evt a reference to the event.
 *
 * @return the duration of the event in milliseconds
 ******************************************************************************/
inline float GsPerformanceTimer::getEventDuration( GsPerformanceTimer::Event& pEvent ) const
{
	float tms;

	/*tms = ( pEvent.timerStopEvt.tv_sec - pEvent.timerStartEvt.tv_sec ) * 1000.0f // sec -> msec
	+ ( pEvent.timerStopEvt.tv_nsec - pEvent.timerStartEvt.tv_nsec ) * 1e-6f;  // nano -> milli*/
	tms = convertTimeDifferenceToSec( &pEvent.timerStopEvt, &pEvent.timerStartEvt ) * 1000.0f;

	return tms;
}

/******************************************************************************
 * This method return the difference between the begin of two events
 *
 * @param evt0 a reference to the first event
 * @param evt1 a reference to the second event
 *
 * @return the difference between the two events in milliseconds
 ******************************************************************************/
inline float GsPerformanceTimer::getStartToStartTime( GsPerformanceTimer::Event& pEvent0, GsPerformanceTimer::Event& pEvent1 ) const
{
	float tms;

	/*tms = ( pEvent1.timerStartEvt.tv_sec - pEvent0.timerStartEvt.tv_sec ) * 1000.0f // sec -> msec
	+ ( pEvent1.timerStartEvt.tv_nsec - pEvent0.timerStartEvt.tv_nsec ) * 1e-6f;  // nano -> milli*/

	tms = convertTimeDifferenceToSec( &pEvent1.timerStartEvt, &pEvent0.timerStartEvt ) * 1000.0f;

	return tms;
}

/******************************************************************************
 * Get high resolution time
 *
 * @param pPerformanceCount ...
 ******************************************************************************/
inline void GsPerformanceTimer::getHighResolutionTime( GsPerformanceTimer::timerStruct* pPerformanceCount ) const
{
#ifdef WIN32
	// Retrieves the current value of the high-resolution performance counter
	// - parameter :
	// ---- A pointer to a variable that receives the current performance-counter value, in counts.
	QueryPerformanceCounter( pPerformanceCount );
#else
	clock_gettime( CLOCK_REALTIME, pPerformanceCount );
#endif
}

/******************************************************************************
 * Convert time difference to sec
 *
 * @param end ...
 * @param begin ...
 *
 * @return ...
 ******************************************************************************/
inline float GsPerformanceTimer::convertTimeDifferenceToSec( GsPerformanceTimer::timerStruct* pEnd, GsPerformanceTimer::timerStruct* pBegin ) const
{
#ifdef WIN32
	timerStruct frequency;
	// Retrieves the frequency of the high-resolution performance counter, if one exists. The frequency cannot change while the system is running.
	// - parameter :
	// ---- A pointer to a variable that receives the current performance-counter frequency, in counts per second.
	// ---- If the installed hardware does not support a high-resolution performance counter, this parameter can be zero.
	QueryPerformanceFrequency( &frequency );

	return ( pEnd->QuadPart - pBegin->QuadPart ) / static_cast< float >( frequency.QuadPart );
#else
	return ( pEnd->tv_sec - pBegin->tv_sec ) + ( 1e-9 ) * ( pEnd->tv_nsec - pBegin->tv_nsec );
#endif
}

} // namespace GvPerfMon
