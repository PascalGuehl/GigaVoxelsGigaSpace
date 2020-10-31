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

// GigaVoxels
#include "GvCore/GsPool.h"
#include "GvUtils/GsDataLoader.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{

/******************************************************************************
 * Constructor
 *
 * @param pCaller Reference on a brick producer
 * @param pIndexValue Index value
 * @param pBlockMemSize Block memory size
 * @param pLevel level of resolution
 * @param pDataPool reference on a data pool
 * @param pOffsetInPool offset in the referenced data pool
 ******************************************************************************/
template< typename TDataTypeList >
inline GsBrickLoaderChannelInitializer< TDataTypeList >
::GsBrickLoaderChannelInitializer( GsDataLoader< TDataTypeList >* pCaller,
									unsigned int pIndexValue, unsigned int pBlockMemSize, unsigned int pLevel,
									GvCore::GPUPoolHost< GvCore::Array3D, TDataTypeList >* pDataPool, size_t pOffsetInPool )
:	_caller( pCaller )
,	_indexValue( pIndexValue )
,	_blockMemorySize( pBlockMemSize )
,	_level( pLevel )
,	_dataPool( pDataPool )
,	_offsetInPool( pOffsetInPool )
{
}

/******************************************************************************
 * Concrete method used to read a brick
 *
 * @param Loki::Int2Type< channel > index of the channel (i.e. color, normal, etc...)
 ******************************************************************************/
template< typename TDataTypeList >
template< int TChannelIndex >
inline void GsBrickLoaderChannelInitializer< TDataTypeList >
::run( Loki::Int2Type< TChannelIndex > )
{
	// Type definition of the channel's data type at given channel index.
	typedef typename Loki::TL::TypeAt< TDataTypeList, TChannelIndex >::Result ChannelType;

	// Retrieve the data array associated to the data pool at given channel index.
	GvCore::Array3D< ChannelType >* dataArray = _dataPool->template getChannel< TChannelIndex >();

	// Ask the referenced brick producer to read a brick
	_caller->template readBrick< ChannelType >( TChannelIndex, _indexValue, _blockMemorySize, _level, dataArray, _offsetInPool );
}

} // namespace GvUtils
