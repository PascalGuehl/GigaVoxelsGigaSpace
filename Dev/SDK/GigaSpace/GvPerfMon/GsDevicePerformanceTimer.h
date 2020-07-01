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

#ifndef _GV_DEVICE_PERFORMANCE_TIMER_H_
#define _GV_DEVICE_PERFORMANCE_TIMER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsTypes.h"

// System
#ifdef WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
#else
	#include <time.h>
#endif

// Cuda
#include <cuda_runtime.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * Defines the number of timers available per-pixel in a kernel.
 */
#define CUDAPERFMON_KERNEL_TIMER_MAX 8

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
 * @class GsDevicePerformanceTimer
 *
 * @brief The GsDevicePerformanceTimer class provides a device performance timer.
 *
 * Allows timing GPU events.
 */
class GIGASPACE_EXPORT GsDevicePerformanceTimer
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * This structure contains informations we gather during the timing phase.
	 *
	 * @field cudaTimerStartEvent the time when we started the timer.
	 * @field cudaTimerStopEvt the time when we stopped the timer.
	 */
	struct Event
	{
		/**************************************************************************
		 ***************************** PUBLIC SECTION *****************************
		 **************************************************************************/

		/****************************** INNER TYPES *******************************/

		/******************************* ATTRIBUTES *******************************/

		/**
		 *  Cuda start event
		 */
		cudaEvent_t cudaTimerStartEvt;

		/**
		 *  Cuda stop event
		 */
		cudaEvent_t cudaTimerStopEvt;

		/**
		 *  Kernel timer min
		 */
		GvCore::uint64 kernelTimerMin[ CUDAPERFMON_KERNEL_TIMER_MAX ];
		
		/**
		 *  Kernel timer max
		 */
		GvCore::uint64 kernelTimerMax[ CUDAPERFMON_KERNEL_TIMER_MAX ];

		/**
		 * Timers array
		 */
		GvCore::uint64 timersArray[ CUDAPERFMON_KERNEL_TIMER_MAX ];

		/******************************** METHODS *********************************/

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

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsDevicePerformanceTimer();

	/**
	 * Destructor
	 */
	virtual ~GsDevicePerformanceTimer();

	/**
	 * This method create and initialize a new Event object.
	 *
	 * @return the created event
	 */
	Event createEvent() const;

	/**
	 * This method set the start time of the given event to the current time.
	 *
	 * @param pEvent a reference to the event.
	 */
	void startEvent( Event& pEvent ) const;

	/**
	 * This method set the stop time of the given event to the current time.
	 *
	 * @param pEvent a reference to the event.
	 */
	void stopEvent( Event& pEvent ) const;

	/**
	 * This method return the duration of the given event
	 *
	 * @param pEvent a reference to the event.
	 *
	 * @return the duration of the event in milliseconds
	 */
	float getEventDuration( Event& pEvent ) const;

	/**
	 * This method return the difference between the begin of two events
	 *
	 * @param pEvent0 a reference to the first event
	 * @param pEvent1 a reference to the second event
	 *
	 * @return the difference between the two events in milliseconds
	 */
	float getStartToStartTime( Event& pEvent0, Event& pEvent1 ) const;

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

	/**
	 * Copy constructor forbidden.
	 */
	//GsDevicePerformanceTimer( const GsDevicePerformanceTimer& );

	/**
	 * Copy operator forbidden.
	 */
	//GsDevicePerformanceTimer& operator=( const GsDevicePerformanceTimer& );

};

} // namespace GvPerfMon

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsDevicePerformanceTimer.inl"

#endif
