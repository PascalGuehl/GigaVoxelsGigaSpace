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

#ifndef _GV_CACHE_MANAGER_RESOURCES_H_
#define _GV_CACHE_MANAGER_RESOURCES_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Gigavoxels
#include "GvCore/GsCoreConfig.h"

// Thrust
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

// cudpp
#include <cudpp.h>

// CUDA
#include <helper_math.h>

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
 * @class GsCacheManagerResources
 *
 * @brief The GsCacheManagerResources class provides...
 *
 * @ingroup GvCore
 * @namespace GvCache
 *
 * ...
 */
class GIGASPACE_EXPORT GsCacheManagerResources
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Initialize
	 */
	static void initialize();

	/**
	 * Finalize
	 */
	static void finalize();

	/**
	 * Get the temp usage mask1
	 *
	 * @param pSize ...
	 *
	 * @return ...
	 */
	static thrust::device_vector< uint >* getTempUsageMask1( size_t pSize );

	/**
	 * Get the temp usage mask2
	 *
	 * @param pSize ...
	 *
	 * @return ...
	 */
	static thrust::device_vector< uint >* getTempUsageMask2( size_t pSize );

	/**
	 * Get a CUDPP plan given a number of elements to be processed.
	 *
	 * @param pSize The maximum number of elements to be processed
	 *
	 * @return a handle on the plan
	 */
	static CUDPPHandle getScanPlan( uint pSize );

	/**
	 * Get a CUDPP plan given a number of elements to be processed.
	 *
	 * @param pSize The maximum number of elements to be processed
	 *
	 * @return a handle on the plan
	 */
	static CUDPPHandle getCudppLibrary();

	/**
	 * Get a CUDPP plan given a number of elements to be processed.
	 *
	 * @param pSize The maximum number of elements to be processed
	 *
	 * @return a handle on the plan
	 */
	static CUDPPHandle getSortPlan( uint pSize );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Temp usage mask1
	 */
	static thrust::device_vector< uint >* _d_tempUsageMask1;

	/**
	 * Temp usage mask2
	 */
	static thrust::device_vector< uint >* _d_tempUsageMask2;

	/**
	 * Handle on an instance of the CUDPP library.
	 */
	static CUDPPHandle _cudppLibrary;

	/**
	 * Scan plan.
	 * A plan is a data structure containing state and intermediate storage space that CUDPP uses to execute algorithms on data.
	 */
	static CUDPPHandle _scanPlan;

	/**
	 * Scan plan size, i.e. the maximum number of elements to be processed
	 */
	static uint _scanPlanSize;
	/**
	 * Scan plan.
	 * A plan is a data structure containing state and intermediate storage space that CUDPP uses to execute algorithms on data.
	 */
	static CUDPPHandle _sortPlan;

	/**
	 * Scan plan size, i.e. the maximum number of elements to be processed
	 */
	static uint _sortPlanSize;

};

} //namespace GvCache

#endif
