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

// System
#include <cassert>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GsCompute
{

/******************************************************************************
 * Get the singleton
 *
 * @return the singleton
 ******************************************************************************/
inline GsDataParallelPrimitives& GsDataParallelPrimitives::get()
{
	if ( _sInstance == NULL )
	{
		_sInstance = new GsDataParallelPrimitives();
	}
	assert( _sInstance != NULL );
	return *_sInstance;
}

/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
inline bool GsDataParallelPrimitives::compact( void* d_out, size_t* d_numValidElements, const void* d_in, const unsigned int* d_isValid, size_t numElements )
{
	return ( cudppCompact( _compactPlan, d_out, d_numValidElements, d_in, d_isValid, numElements ) == CUDPP_SUCCESS );
}

/******************************************************************************
 * Get a CUDPP plan given a number of elements to be processed.
 *
 * @param pSize The maximum number of elements to be processed
 *
 * @return a handle on the plan
 ******************************************************************************/
inline bool GsDataParallelPrimitives::sort( void* d_keys, void* d_values, size_t numElements )
{
#ifdef GS_USE_RADIX_SORT
	return ( cudppRadixSort( _sortPlan, d_keys, d_values, numElements ) == CUDPP_SUCCESS );
#else
	return ( cudppMergeSort( _sortPlan, d_keys, d_values, numElements ) == CUDPP_SUCCESS );
#endif

}

} // namespace GsCompute
