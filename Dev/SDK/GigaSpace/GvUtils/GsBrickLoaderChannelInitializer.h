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

#ifndef _GV_BRICK_LOADER_CHANNEL_INITIALIZER_H_
#define _GV_BRICK_LOADER_CHANNEL_INITIALIZER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Gigavoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsArray.h"

// Loki
#include <loki/Typelist.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaVoxels
namespace GvCore
{
	template
	<
		template< typename > class THostArray, class TList
	>
	class GPUPoolHost;
}

namespace GvUtils
{
	template< typename TDataTypeList >
	class GsDataLoader;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvUtils
{

/** 
 * @struct GsBrickLoaderChannelInitializer
 *
 * @brief The GsBrickLoaderChannelInitializer struct provides a generalized functor
 * to read a brick of voxels.
 *
 * GsBrickLoaderChannelInitializer is used with GsDataLoader to read a brick from HOST.
 */
template< typename TDataTypeList >
struct GsBrickLoaderChannelInitializer
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Reference on a brick producer
	 */
	GsDataLoader< TDataTypeList >* _caller;

	/**
	 * Index value
	 */
	unsigned int _indexValue;

	/**
	 * Block memory size
	 */
	unsigned int _blockMemorySize;

	/**
	 * Level of resolution
	 */
	unsigned int _level;

	/**
	 * Reference on a data pool
	 */
	GvCore::GPUPoolHost< GvCore::Array3D, TDataTypeList >* _dataPool;

	/**
	 * Offset in the referenced data pool
	 */
	size_t _offsetInPool;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param pCaller Reference on a brick producer
	 * @param pIndexValue Index value
	 * @param pBlockMemSize Block memory size
	 * @param pLevel level of resolution
	 * @param pDataPool reference on a data pool
	 * @param pOffsetInPool offset in the referenced data pool
	 */
	inline GsBrickLoaderChannelInitializer( GsDataLoader< TDataTypeList >* pCaller,
											unsigned int pIndexValue, unsigned int pBlockMemSize, unsigned int pLevel,
											GvCore::GPUPoolHost< GvCore::Array3D, TDataTypeList >* pDataPool, size_t pOffsetInPool );

	/**
	 * Concrete method used to read a brick
	 *
	 * @param Loki::Int2Type< TChannelIndex > index of the channel (i.e. color, normal, etc...)
	 */
	template< int TChannelIndex >
	inline void run( Loki::Int2Type< TChannelIndex > );

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

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsBrickLoaderChannelInitializer.inl"

#endif
