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

#include "GsCompute/GsDataParallelPrimitives.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// System
#include <cstdio>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaSpace
using namespace GsCompute;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * The unique instance
 */
GsDataParallelPrimitives* GsDataParallelPrimitives::_sInstance = NULL;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Initialize
 ******************************************************************************/
void GsDataParallelPrimitives::initialize()
{
	assert( _sInstance == NULL );
	if ( _sInstance == NULL )
	{
		_sInstance = new GsDataParallelPrimitives();
		assert( _sInstance != NULL );
	
		// Creates an instance of the CUDPP library and returns a handle.
		if ( cudppCreate( &_sInstance->_cudppLibrary ) != CUDPP_SUCCESS )
		{
			// LOG
			printf( "\nError creating CUDPPP library." );
		}
	}
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
void GsDataParallelPrimitives::finalize()
{
	if ( _sInstance != NULL )
	{
		if ( _sInstance->_compactPlan )
		{
			if ( cudppDestroyPlan( _sInstance->_compactPlan ) != CUDPP_SUCCESS )
			{
				// LOG
				printf( "\nError cudppDestroyPlan() failed on compact plan.");
			}
		}
	
		if ( _sInstance->_sortPlan )
		{
			if ( cudppDestroyPlan( _sInstance->_sortPlan ) != CUDPP_SUCCESS )
			{
				// LOG
				printf( "\nError cudppDestroyPlan() failed on sort plan.");
			}
		}

		if ( cudppDestroy( _sInstance->_cudppLibrary ) != CUDPP_SUCCESS )
		{
			// LOG
			printf( "\nError destroying CUDPPP library." );
		}
	}

	delete _sInstance;
	_sInstance = NULL;
}

/******************************************************************************
 * Consrtuctor
 ******************************************************************************/
GsDataParallelPrimitives::GsDataParallelPrimitives()
:	_cudppLibrary( 0 )
,	_compactPlan( 0 )
,	_compactPlanSize( 0 )
,	_sortPlan( 0 )
,	_sortPlanSize( 0 )
{
}

/******************************************************************************
 * Desrtuctor
 ******************************************************************************/
GsDataParallelPrimitives::~GsDataParallelPrimitives()
{
}

/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
bool GsDataParallelPrimitives::initializeCompactPlan( size_t pSize )
{
	bool result = true;

	if ( _compactPlanSize < pSize )
	{
		if ( _compactPlanSize > 0 )
		{
			if ( cudppDestroyPlan( _compactPlan ) != CUDPP_SUCCESS )
			{
				// LOG
				printf( "\nError: cudppDestroyPlan() failed on compact plan.");
			}
			
			_compactPlan = 0;
			_compactPlanSize = 0;
		}

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
		// ---- create the CUDPP plan.
		CUDPPResult result = cudppPlan( _cudppLibrary, &_compactPlan,
										config,
										pSize,		// The maximum number of elements to be processed
										1,			// The number of rows (for 2D operations) to be processed
										0 );		// The pitch of the rows of input data, in elements
		if ( CUDPP_SUCCESS != result )
		{
			// LOG
			printf( "\nError creating CUDPP compact plan." );
		}

		// Update size info
		_compactPlanSize = pSize;
	}

	return result;
}

/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
bool GsDataParallelPrimitives::initializeSortPlan( size_t pSize )
{
	bool result = true;

	if ( _sortPlanSize < pSize )
	{
		if ( _sortPlanSize > 0 )
		{
			if ( cudppDestroyPlan( _sortPlan ) != CUDPP_SUCCESS )
			{
				// LOG
				printf( "\nError: cudppDestroyPlan() failed on sort plan.");
			}
			
			_sortPlan = 0;
			_sortPlanSize = 0;
		}

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
#ifdef GS_USE_RADIX_SORT
		config.algorithm = CUDPP_SORT_RADIX;
#else
		config.algorithm = CUDPP_SORT_MERGE;
#endif
		config.options = CUDPP_OPTION_FORWARD | CUDPP_OPTION_EXCLUSIVE;
		// ---- create the CUDPP plan.
		CUDPPResult result = cudppPlan( _cudppLibrary, &_sortPlan,
										config,
										pSize,		// The maximum number of elements to be processed
										1,			// The number of rows (for 2D operations) to be processed
										0 );		// The pitch of the rows of input data, in elements
		if ( CUDPP_SUCCESS != result )
		{
			// LOG
			printf( "\nError creating CUDPP sort plan." );
		}

		// Update size info
		_sortPlanSize = pSize;
	}

	return result;
}
