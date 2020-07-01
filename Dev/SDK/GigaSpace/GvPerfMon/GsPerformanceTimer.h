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

#ifndef _GV_PERFORMANCE_TIMER_H_
#define _GV_PERFORMANCE_TIMER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// System
#ifdef WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
#else
	#include <time.h>
#endif

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

namespace GvPerfMon
{

/** 
 * @class GsPerformanceTimer
 *
 * @brief The GsPerformanceTimer class provides a host performance timer.
 *
 * Allows timing CPU events.
 */
class GIGASPACE_EXPORT GsPerformanceTimer
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Type definition of types required by timer functions (Operating System dependent)
	 */
#ifdef WIN32
		typedef LARGE_INTEGER timerStruct;
#else
		typedef struct timespec timerStruct;
#endif

	/**
	 * Structure used to store start end stop time of an event
	 */
	struct Event
	{
		timerStruct timerStartEvt;
		timerStruct timerStopEvt;
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsPerformanceTimer();

	/**
	 * Destructor
	 */
	virtual ~GsPerformanceTimer();

	/**
	 * This method create and initialize a new Event object.
	 *
	 * @return the created event
	 */
	inline Event createEvent() const;

	/**
	 * This method set the start time of the given event to the current time.
	 *
	 * @param evt a reference to the event.
	 */
	inline void startEvent( Event& pEvent ) const;

	/**
	 * This method set the stop time of the given event to the current time.
	 *
	 * @param evt a reference to the event.
	 */
	inline void stopEvent( Event& pEvent ) const;

	/**
	 * This method return the duration of the given event
	 *
	 * @param evt a reference to the event.
	 *
	 * @return the duration of the event in milliseconds
	 */
	inline float getEventDuration( Event& pEvent )  const;

	/**
	 * This method return the difference between the begin of two events
	 *
	 * @param evt0 a reference to the first event
	 * @param evt1 a reference to the second event
	 *
	 * @return the difference between the two events in milliseconds
	 */
	inline float getStartToStartTime( Event& pEvent0, Event& pEvent1 ) const;
		
	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Get high resolution time
	 *
	 * @param pPerformanceCount ...
	 */
	inline void getHighResolutionTime( timerStruct* pPerformanceCount ) const;

	/**
	 * Convert time difference to sec
	 *
	 * @param end ...
	 * @param begin ...
	 *
	 * @return ...
	 */
	inline float convertTimeDifferenceToSec( timerStruct* pEnd, timerStruct* pBegin ) const;

	/******************************** METHODS *********************************/

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
	//GsPerformanceTimer( const GsPerformanceTimer& );

	/**
	 * Copy operator forbidden.
	 */
	//GsPerformanceTimer& operator=( const GsPerformanceTimer& );

};

} // namespace GvPerfMon

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsPerformanceTimer.inl"

#endif
