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

#ifndef _GV_PERFORMANCE_MONITOR_H_
#define _GV_PERFORMANCE_MONITOR_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvPerfMon/GsPerformanceTimer.h"
#include "GvPerfMon/GsDevicePerformanceTimer.h"
#include "GvPerfMon/GsPerformanceMonitorKernel.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvCore/GsTypes.h"
#include "GvCore/GsArray.h"
#include "GvCore/GsLinearMemory.h"
#include "GvCore/GsLinearMemoryKernel.h"
#include "GvCore/GsError.h"

// STL
#include <vector>
#include <string>
#include <iostream>

// System
#include <cassert>

// Cuda
#include <cuda.h>
#include <vector_types.h>
#include <vector_functions.h>

// Cuda SDK
#include <helper_cuda.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * ...
 */
//#define CUDAPERFMON_CACHE_INFO 1
/**
 * ...
 */
#define CUDAPERFMON_GPU_TIMER_ENABLED 1
/**
 * ...
 */
#define GV_PERFMON_INTERNAL_DEVICE_TIMER_ENABLED 0
/**
 * ...
 */
#define CUDAPERFMON_GPU_TIMER_MAX_INSTANCES 32 // Prevent getting out of memory error
/**
 * ...
 */
#define CUDAPERFTIMERCPU_GPUSYNC 0

namespace GvPerfMon
{

	/**
	 * Timers array
	 */
	//__constant__ uint64 *k_timersArray;
	__constant__ GvCore::GsLinearMemoryKernel< GvCore::uint64 > k_timersArray;

	/**
	 * Timers mask
	 */
	__constant__ uchar* k_timersMask;

}

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
 * CUDA Performance monitoring class.
 */
class GIGASPACE_EXPORT CUDAPerfMon
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Flag to tell wherer or not to activate Monitoring
	 */
	static bool _isActivated;

	/**
	 * Define events list
	 */
#define CUDAPM_DEFINE_EVENT( evtName ) evtName,
	enum ApplicationEvent
	{
		// TO DO : enlever ce #include
		#include "GvPerfMon/GsPerformanceMonitorEvents.h"
		NumApplicationEvents
	};
#undef CUDAPM_DEFINE_EVENT

	/******************************* ATTRIBUTES *******************************/

	//-------------------------- TEST
	bool _requestResize;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	CUDAPerfMon();

	/**
	 * ...
	 *
	 * @return ...
	 */
	static CUDAPerfMon& get()
	{
		if ( ! _sInstance )
		{
			_sInstance = new CUDAPerfMon();
		}

		return (*_sInstance);
	}

	/**
	 * Initialize
	 */
	void init( /*int xres, int yres*/ );

	/**
	 * Start an event
	 *
	 * @param evtName event name's index
	 * @param hasKernelTimers flag to tell wheter or not to handle internal GPU timers
	 */
	void startEvent( ApplicationEvent evtName, bool hasKernelTimers = false );

	/**
	 * Stop an event
	 *
	 * @param evtName event name's index
	 * @param hasKernelTimers flag to tell wheter or not to handle internal GPU timers
	 */
	void stopEvent( ApplicationEvent evtName, bool hasKernelTimers = false );

	/**
	 * Start the main frame event
	 */
	void startFrame();

	/**
	 * Stop the main frame event
	 */
	void stopFrame();

	/**
	 * ...
	 *
	 * @param evtName ...
	 * @param n ...
	 */
	void setEventNumElems( ApplicationEvent evtName, uint n );

	/**
	 * ...
	 */
	void displayFrame();

	/**
	 * ...
	 *
	 * @param eventType ...
	 */
	void displayFrameGL( uint eventType = 0 );	// 0 GPU evts, 1 CPU evts

	/**
	 * ...
	 *
	 * @param overlayBuffer ...
	 */
	void displayOverlayGL( uchar* overlayBuffer );

	/**
	 * ...
	 */
	void displayCacheInfoGL();

	/**
	 * ...
	 *
	 * @param numNodePagesUsed ...
	 * @param numNodePagesWrited ...
	 * @param numBrickPagesUsed ...
	 * @param numBrickPagesWrited ...
	 */
	void saveFrameStats( uint numNodePagesUsed, uint numNodePagesWrited, uint numBrickPagesUsed, uint numBrickPagesWrited );

	/**
	 * ...
	 *
	 * @param frameRes ...
	 */
	void frameResized( uint2 frameRes );
	
	//--------------------- TEST -----------------------------------
	/**
	 * ...
	 */
	GvCore::GsLinearMemory< GvCore::uint64 >* getKernelTimerArray();
	//--------------------- TEST -----------------------------------
	
	/**
	 * ...
	 *
	 * @return ...
	 */
	uchar* getKernelTimerMask();

	/**
	 * ...
	 *
	 * @return ...
	 */
	GvCore::GsLinearMemory< uchar4 >* getCacheStateArray() const;

	/**
	 * ...
	 *
	 * @param cacheStateArray ...
	 */
	void setCacheStateArray( GvCore::GsLinearMemory< uchar4 >* cacheStateArray );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/
		
	/**
	 * Singleton instance
	 */
	static CUDAPerfMon* _sInstance;

	/**
	 * Device timer manager
	 * - start / stop events
	 * - get elapsed time
	 */
	GsDevicePerformanceTimer _deviceTimer;

	/**
	 * Host timer manager
	 * - start / stop events
	 * - get elapsed time
	 */
	GsPerformanceTimer _hostTimer;

	/**
	 * List of all events name
	 */
	static const char* _eventNames[ NumApplicationEvents + 1 ];

	/**
	 * ...
	 */
	int frameCurrentInstance[ NumApplicationEvents ];

	/**
	 * List of device events
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< GsDevicePerformanceTimer::Event > _deviceEvents[ NumApplicationEvents ];
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * List of host events
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< GsPerformanceTimer::Event > _hostEvents[ NumApplicationEvents ];
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * ...
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< uint > eventsNumElements[ NumApplicationEvents ];
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Flag to tell wheter or not the frame has started
	 */
	bool _frameStarted;

	/**
	 * Kernel timers
	 */
	int _deviceClockRate;

	/**
	 * Internal GPU timer's array
	 *
	 * - 3D array : (width, height) 2D array of window size + (depth) one by internal event's timer
	 */
	GvCore::GsLinearMemory< GvCore::uint64 >* d_timersArray;
	
	/**
	 * ...
	 */
	uchar* d_timersMask;

	/**
	 * ...
	 */
	GLuint overlayTex;

	/**
	 * ...
	 */
	GLuint cacheStateTex;

	/**
	 * ...
	 */
	GvCore::GsLinearMemory< uchar4 >* d_cacheStateArray;

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GvPerfMon

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

#ifdef USE_CUDAPERFMON

//#define CUDAPM_INIT( xres, yres ) ::GvPerfMon::CUDAPerfMon::get().init( xres, yres );
#define CUDAPM_INIT() ::GvPerfMon::CUDAPerfMon::get().init();
#define CUDAPM_RESIZE( frameSize ) ::GvPerfMon::CUDAPerfMon::get().frameResized( frameSize );
#define CUDAPM_END

/**
 * Start / Stop the main frame event
 */
#define CUDAPM_START_FRAME								\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )			\
	{													\
		::GvPerfMon::CUDAPerfMon::get().startFrame();	\
	}
#define CUDAPM_STOP_FRAME								\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )			\
	{													\
		::GvPerfMon::CUDAPerfMon::get().stopFrame();	\
	}

/**
 * Start / Stop an event
 */
#define CUDAPM_START_EVENT( eventName )														\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )												\
	{																						\
		::GvPerfMon::CUDAPerfMon::get().startEvent( ::GvPerfMon::CUDAPerfMon::eventName );	\
	}
#define CUDAPM_STOP_EVENT( eventName )														\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )												\
	{																						\
		::GvPerfMon::CUDAPerfMon::get().stopEvent( ::GvPerfMon::CUDAPerfMon::eventName );	\
	}

/**
 * Start / Stop an event with internal GPU timer
 */
#define CUDAPM_START_EVENT_GPU( eventName )															\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )														\
	{																								\
		::GvPerfMon::CUDAPerfMon::get().startEvent( ::GvPerfMon::CUDAPerfMon::eventName, true );	\
	}
#define CUDAPM_STOP_EVENT_GPU( eventName )															\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )														\
	{																								\
		::GvPerfMon::CUDAPerfMon::get().stopEvent( ::GvPerfMon::CUDAPerfMon::eventName, true );		\
	}

/**
 * Start / Stop an event given an identifier
 *
 * - based on the comparison of two identifiers
 */
#define CUDAPM_START_EVENT_CHANNEL( channelRef, channelNum, eventName )								\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )														\
	{																								\
		if ( channelNum == channelRef )																\
		{																							\
			::GvPerfMon::CUDAPerfMon::get().startEvent( ::GvPerfMon::CUDAPerfMon::eventName );		\
		}																							\
	}
#define CUDAPM_STOP_EVENT_CHANNEL( channelRef, channelNum, eventName )								\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )														\
	{																								\
		if ( channelNum == channelRef )																\
		{																							\
			::GvPerfMon::CUDAPerfMon::get().stopEvent( ::GvPerfMon::CUDAPerfMon::eventName );		\
		}																							\
	}

/**
 * ... given an identifier
 *
 * - based on the comparison of two identifiers
 */
#define CUDAPM_EVENT_NUMELEMS_CHANNEL( channelRef, channelNum, eventName, n )								\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )																\
	{																										\
		if ( channelNum == channelRef )																		\
		{																									\
			::GvPerfMon::CUDAPerfMon::get().setEventNumElems( ::GvPerfMon::CUDAPerfMon::eventName, n );		\
		}																									\
	}

/**
 * ...
 */
#define CUDAPM_STAT_EVENT( stuff1, stuff2 ) {}

/**
 * ...
 */
#define CUDAPM_EVENT_NUMELEMS( eventName, n )																\
	if ( GvPerfMon::CUDAPerfMon::_isActivated )																\
	{																										\
		::GvPerfMon::CUDAPerfMon::get().setEventNumElems( ::GvPerfMon::CUDAPerfMon::eventName, n );			\
	}

/**
 * ...
 */
#define CUDAPM_GET_KERNEL_EVENT_MEMORY( stuff1, stuff2 ) {}

/**
 * Define an internal event on device
 */
#if GV_PERFMON_INTERNAL_DEVICE_TIMER_ENABLED==1
	#define CUDAPM_KERNEL_DEFINE_EVENT( evtSlot )		\
	GvCore::uint64 cudaPMKernelEvt##evtSlot##Clk = 0;	\
	GvCore::uint64 cudaPMKernelEvt##evtSlot##In;		
#else
	#define CUDAPM_KERNEL_DEFINE_EVENT( evtSlot ) {}
#endif

/**
 * Start / Stop an internal event on device
 */
#if GV_PERFMON_INTERNAL_DEVICE_TIMER_ENABLED==1
	#define CUDAPM_KERNEL_START_EVENT( pixelCoords, evtSlot )													\
	if ( GvPerfMon::k_timersMask[ pixelCoords.y * k_renderViewContext.frameSize.x + pixelCoords.x ] != 0 )		\
	{																											\
		cudaPMKernelEvt##evtSlot##In = GvPerfMon::getClock();													\
	}
#else
	#define CUDAPM_KERNEL_START_EVENT( pixelCoords, evtSlot ) {}
#endif
//#define CUDAPM_KERNEL_START_EVENT( pixelCoords, evtSlot ) {}
#if GV_PERFMON_INTERNAL_DEVICE_TIMER_ENABLED==1
	#define CUDAPM_KERNEL_STOP_EVENT( pixelCoords, evtSlot )																	\
	if ( GvPerfMon::k_timersMask[ pixelCoords.y * k_renderViewContext.frameSize.x + pixelCoords.x ] != 0 )						\
	{																															\
		cudaPMKernelEvt##evtSlot##Clk += ( GvPerfMon::getClock() - cudaPMKernelEvt##evtSlot##In );								\
		GvPerfMon::k_timersArray.set( make_uint3( pixelCoords.x, pixelCoords.y, evtSlot ), cudaPMKernelEvt##evtSlot##Clk );		\
	}
#else
	#define CUDAPM_KERNEL_STOP_EVENT( pixelCoords, evtSlot ) {}
#endif
//#define CUDAPM_KERNEL_STOP_EVENT( pixelCoords, evtSlot ) {}

// Not sure we need them
/*#define CUDAPM_KERNEL_START( pixelCoords ) { \
	uint64 kernelEvtIn = GvPerfMon::getClock(); \
}
#define CUDAPM_KERNEL_STOP( pixelCoords ) { \
	k_timersArray.get( make_uint3( pixelCoords.x, pixelCoords.y, 0 ) ) = GvPerfMon::getClock() - kernelEvtIn;\
}*/

# if CUDAPERFMON_CACHE_INFO == 1

#  define CUDAPM_RENDER_CACHE_INFO( xres, yres ) { \
	GvCore::GsLinearMemory< uchar4 >* cacheStateArray = \
		::GvPerfMon::CUDAPerfMon::get().getCacheStateArray(); \
	if ( cacheStateArray ) \
	{ \
		const uint2 syntheticRenderSize = make_uint2( xres, yres ); \
		dim3 blockSize( 8, 8, 1 ); \
		dim3 gridSize( syntheticRenderSize.x / blockSize.x, syntheticRenderSize.y / blockSize.y, 1 ); \
		SyntheticInfo_Render<<< gridSize, blockSize, 0 >>>( cacheStateArray->getPointer(), cacheStateArray->getNumElements() ); \
		GV_CHECK_CUDA_ERROR( "SyntheticInfo_Render" ); \
	} \
}

# else
#  define CUDAPM_RENDER_CACHE_INFO( xres, yres ) {}
# endif

#else  // USE_CUDAPERFMON


#define CUDAPM_INIT()
#define CUDAPM_RESIZE( frameSize )
#define CUDAPM_END

#define CUDAPM_START_FRAME
#define CUDAPM_STOP_FRAME

#define CUDAPM_START_EVENT( stuff ) {}
#define CUDAPM_STOP_EVENT( stuff ) {}
#define CUDAPM_START_EVENT_GPU( stuff ) {}
#define CUDAPM_STOP_EVENT_GPU( stuff ) {}

#define CUDAPM_START_EVENT_CHANNEL( channelRef, channelNum, eventName ) {}
#define CUDAPM_STOP_EVENT_CHANNEL( channelRef, channelNum, eventName ) {}
#define CUDAPM_EVENT_NUMELEMS_CHANNEL( channelRef, channelNum, eventName, n ) {}

#define CUDAPM_STAT_EVENT( stuff1, stuff2 ) {}
#define CUDAPM_EVENT_NUMELEMS( eventName, n ) {}

#define CUDAPM_GET_KERNEL_EVENT_MEMORY( stuff1, stuff2 ){}


#define CUDAPM_KERNEL_DEFINE_EVENT( evtSlot ) {}
#define CUDAPM_KERNEL_START_EVENT( pixelCoords, evtSlot ) {}
#define CUDAPM_KERNEL_STOP_EVENT( pixelCoords, evtSlot ) {}

#define CUDAPM_RENDER_CACHE_INFO( xres, yres ) {}

#endif

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsPerformanceMonitor.inl"

#endif
