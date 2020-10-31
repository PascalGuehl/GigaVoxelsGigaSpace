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

#include "GvCache/GsCacheManagerResources.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// System
#include <stdio.h>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvCache;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * ...
 */
thrust::device_vector< uint >* GsCacheManagerResources::_d_tempUsageMask1 = NULL;
thrust::device_vector< uint >* GsCacheManagerResources::_d_tempUsageMask2 = NULL;

/**
 * ...
 */
CUDPPHandle GsCacheManagerResources::_scanPlan = 0;
CUDPPHandle GsCacheManagerResources::_cudppLibrary = 0;

/**
 * ...
 */
uint GsCacheManagerResources::_scanPlanSize = 0;
CUDPPHandle GsCacheManagerResources::_sortPlan = 0;
uint GsCacheManagerResources::_sortPlanSize = 0;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Initialize
 ******************************************************************************/
void GsCacheManagerResources::initialize()
{
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
void GsCacheManagerResources::finalize()
{
	delete _d_tempUsageMask1;
	_d_tempUsageMask1 = NULL;

	delete _d_tempUsageMask2;
	_d_tempUsageMask2 = NULL;

	CUDPPResult result = cudppDestroyPlan( _scanPlan );
	if ( CUDPP_SUCCESS != result )
	{
		printf( "Error destroying CUDPPPlan compact\n" );
	}
	result = cudppDestroyPlan( _sortPlan );
	if ( CUDPP_SUCCESS != result )
	{
		printf( "Error destroying CUDPPPlan radix sort\n" );
	}
}

/******************************************************************************
 * Get the temp usage mask1
 *
 * @param pSize ...
 *
 * @return ...
 ******************************************************************************/
thrust::device_vector< uint >* GsCacheManagerResources::getTempUsageMask1( size_t pSize )
{
	if ( ! _d_tempUsageMask1 )
	{
		_d_tempUsageMask1 = new thrust::device_vector< uint >( pSize );
	}
	else if ( _d_tempUsageMask1->size() < pSize )
	{
		_d_tempUsageMask1->resize( pSize );
	}

	return _d_tempUsageMask1;
}

/******************************************************************************
 * Get the temp usage mask2
 *
 * @param pSize ...
 *
 * @return ...
 ******************************************************************************/
thrust::device_vector< uint >* GsCacheManagerResources::getTempUsageMask2( size_t pSize )
{
	if ( ! _d_tempUsageMask2 )
	{
		_d_tempUsageMask2 = new thrust::device_vector< uint >( pSize );
	}
	else if ( _d_tempUsageMask2->size() < pSize )
	{
		_d_tempUsageMask2->resize( pSize );
	}

	return _d_tempUsageMask2;
}

/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
CUDPPHandle GsCacheManagerResources::getScanPlan( uint pSize )
{
	if ( _scanPlanSize < pSize )
	{
		if ( _scanPlanSize > 0 )
		{
			if ( cudppDestroyPlan( _scanPlan ) != CUDPP_SUCCESS )
			{
				printf( "warning: cudppDestroyPlan() failed!\n ");
			}

			if ( cudppDestroy( _cudppLibrary ) != CUDPP_SUCCESS )
			{
				printf( "warning: cudppDestroy() failed!\n" );
			}

			_scanPlanSize = 0;
		}

		// Creates an instance of the CUDPP library, and returns a handle.
		cudppCreate( &_cudppLibrary );

		// Create a CUDPP plan.
		//
		// A plan is a data structure containing state and intermediate storage space that CUDPP uses to execute algorithms on data.
		// A plan is created by passing to cudppPlan() a CUDPPConfiguration that specifies the algorithm, operator, datatype, and options.
		// The size of the data must also be passed to cudppPlan(), in the numElements, numRows, and rowPitch arguments.
		// These sizes are used to allocate internal storage space at the time the plan is created.
		// The CUDPP planner may use the sizes, options, and information about the present hardware to choose optimal settings.
		// Note that numElements is the maximum size of the array to be processed with this plan.
		// That means that a plan may be re-used to process (for example, to sort or scan) smaller arrays.
		//
		// ---- configuration struct specifying algorithm and options
		CUDPPConfiguration config;
		config.op = CUDPP_ADD;
		config.datatype = CUDPP_UINT;
		config.algorithm = CUDPP_COMPACT;
		config.options = CUDPP_OPTION_FORWARD | CUDPP_OPTION_EXCLUSIVE;
		// ---- pointer to an opaque handle to the internal plan
		_scanPlan = 0;
		// ---- create the CUDPP plan.
		CUDPPResult result = cudppPlan( _cudppLibrary, &_scanPlan,
										config,
										pSize,		// The maximum number of elements to be processed
										1,			// The number of rows (for 2D operations) to be processed
										0 );		// The pitch of the rows of input data, in elements

		if ( CUDPP_SUCCESS != result )
		{
			printf( "Error creating CUDPPPlan\n" );
			exit( -1 );			// TO DO : remove this exit and use exception ?
		}

		_scanPlanSize = pSize;
	}

	return _scanPlan;
}

/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
CUDPPHandle GsCacheManagerResources::getSortPlan( uint pSize )
{
	if ( _sortPlanSize < pSize )
	{
		if ( _sortPlanSize > 0 )
		{
			if ( cudppDestroyPlan( _sortPlan ) != CUDPP_SUCCESS )
			{
				printf( "warning: cudppDestroyPlan() failed!\n ");
			}

			if ( cudppDestroy( _cudppLibrary ) != CUDPP_SUCCESS )
			{
				printf( "warning: cudppDestroy() failed!\n" );
			}

			_sortPlanSize = 0;
		}

		// Creates an instance of the CUDPP library, and returns a handle.
		cudppCreate( &_cudppLibrary );

		// Create a CUDPP plan.
		//
		// A plan is a data structure containing state and intermediate storage space that CUDPP uses to execute algorithms on data.
		// A plan is created by passing to cudppPlan() a CUDPPConfiguration that specifies the algorithm, operator, datatype, and options.
		// The size of the data must also be passed to cudppPlan(), in the numElements, numRows, and rowPitch arguments.
		// These sizes are used to allocate internal storage space at the time the plan is created.
		// The CUDPP planner may use the sizes, options, and information about the present hardware to choose optimal settings.
		// Note that numElements is the maximum size of the array to be processed with this plan.
		// That means that a plan may be re-used to process (for example, to sort or scan) smaller arrays.
		//
		// ---- configuration struct specifying algorithm and options
		CUDPPConfiguration config;
		config.op = CUDPP_ADD;
		config.datatype = CUDPP_UINT;
		config.algorithm = CUDPP_SORT_RADIX;
		config.options = CUDPP_OPTION_FORWARD | CUDPP_OPTION_EXCLUSIVE;
		// ---- pointer to an opaque handle to the internal plan
		_sortPlan = 0;
		// ---- create the CUDPP plan.
		CUDPPResult result = cudppPlan( _cudppLibrary, &_sortPlan,
										config,
										pSize,		// The maximum number of elements to be processed
										1,			// The number of rows (for 2D operations) to be processed
										0 );		// The pitch of the rows of input data, in elements

		if ( CUDPP_SUCCESS != result )
		{
			printf( "Error creating CUDPPPlan\n" );
			exit( -1 );			// TO DO : remove this exit and use exception ?
		}

		_sortPlanSize = pSize;
	}

	return _sortPlan;
}
/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
CUDPPHandle GsCacheManagerResources::getCudppLibrary()
{
	return _cudppLibrary;
}
