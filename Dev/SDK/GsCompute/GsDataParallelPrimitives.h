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

#ifndef _GS_DATA_PARALLEL_PRIMITIVES_H_
#define _GS_DATA_PARALLEL_PRIMITIVES_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GsCompute/GsComputeConfig.h"

// cudpp
#include <cudpp.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/**
 * Definition to choose either radix or merge CUDPP sort
 */
#define GS_USE_RADIX_SORT

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GsCompute
{

/** 
 * @class GsDataParallelPrimitives
 *
 * @brief The GsDataParallelPrimitives class provides data-parallel algoritmn primitives.
 *
 * @ingroup GsCompute
 *
 * Primitives could be stream compaction, sorting, etc... on device (GPU).
 */
class GSCOMPUTE_EXPORT GsDataParallelPrimitives
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
	 * Get the singleton
	 *
	 * @return the singleton
	 */
	static GsDataParallelPrimitives& get();

	/**
	 * Get a CUDPP plan given a number of elements to be processed.
	 *
	 * @param pSize The maximum number of elements to be processed
	 *
	 * @return a handle on the plan
	 */
	bool initializeCompactPlan( size_t pSize );

	/**
	 * Get a CUDPP plan given a number of elements to be processed.
	 *
	 * @param pSize The maximum number of elements to be processed
	 *
	 * @return a handle on the plan
	 */
	bool initializeSortPlan( size_t pSize );

	/**
	 * Compact
	 */
	bool compact( void* d_out, size_t* d_numValidElements, const void* d_in, const unsigned int* d_isValid, size_t numElements );

	/**
	 * Sort
	 */
	bool sort( void* d_keys, void* d_values, size_t numElements );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsDataParallelPrimitives();

	/**
	 * Destructor
	 */
	~GsDataParallelPrimitives();

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * The unique device manager
	 */
	static GsDataParallelPrimitives* _sInstance;

	/**
	 * Handle on an instance of the CUDPP library.
	 */
	CUDPPHandle _cudppLibrary;

	/**
	 * Scan plan.
	 * A plan is a data structure containing state and intermediate storage space that CUDPP uses to execute algorithms on data.
	 */
	CUDPPHandle _compactPlan;

	/**
	 * Scan plan size, i.e. the maximum number of elements to be processed
	 */
	size_t _compactPlanSize;

	/**
	 * Sort plan.
	 * A plan is a data structure containing state and intermediate storage space that CUDPP uses to execute algorithms on data.
	 */
	CUDPPHandle _sortPlan;

	/**
	 * Sort plan size, i.e. the maximum number of elements to be processed
	 */
	size_t _sortPlanSize;

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsDataParallelPrimitives( const GsDataParallelPrimitives& );

	/**
	 * Copy operator forbidden.
	 */
	GsDataParallelPrimitives& operator=( const GsDataParallelPrimitives& );

};

} // namespace GsCompute

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsDataParallelPrimitives.inl"

#endif // _GS_DATA_PARALLEL_PRIMITIVES_H_
