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
#include "GvCore/GsError.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/////////////// GPU Pool Kernel ////////////////

/******************************************************************************
 * Set the value at a given position in the pool.
 *
 * @param pos position the pool
 * @param val value
 ******************************************************************************/
template< template< typename > class KernelArray, class TList >
template< uint i, typename ST >
__device__
__forceinline__ void GPUPoolKernel< KernelArray, TList >
::setValue( const uint3& pos, ST val )
{
	typename Loki::TL::TypeAt< TList, i >::Result res;
	convert_type( val, res );

	// Retrieve the channel at index i
	typename GPUPool_TypeAtChannel< KernelArray, TList, GPUPoolChannelUnitValue, i >::Result& channel = getChannel( Loki::Int2Type< i >() );

	// Write data in the channel (i.e. its associated surface)
	channel.set< i >( pos, res );
}

/******************************************************************************
 * Get the value at a given position in the pool.
 *
 * @param pos position the pool
 * @param val value
 ******************************************************************************/
template< template< typename > class KernelArray, class TList >
template< uint i, typename ST >
__device__
__forceinline__ ST GPUPoolKernel< KernelArray, TList >
::getValue( const uint3& pos )
{
	typename Loki::TL::TypeAt< TList, i >::Result res;

	// Retrieve the channel at index i
	typename GPUPool_TypeAtChannel< KernelArray, TList, GPUPoolChannelUnitValue, i >::Result& channel = getChannel( Loki::Int2Type< i >() );

	// Write data in the channel (i.e. its associated surface)
	res = channel.get< i >( pos );

	ST val;
	convert_type( res, val );
	return val;
}

/////////////// GPU Pool host ////////////////

/******************************************************************************
 * Constructor
 *
 * @param pResolution resolution
 * @param pOptions options
 ******************************************************************************/
template< template< typename > class HostArray, class TList >
GPUPoolHost< HostArray, TList >
::GPUPoolHost( const uint3& pResolution, uint pOptions )
:	_resolution( pResolution )
,	_options( pOptions )
{
	// After User chooses all channels for color, normal, density, etc... data have to be allocated on device.
	// ChannelAllocator is a helper struct used to allocate theses data in a pool.
	GPUPoolHost< HostArray, TList >::ChannelAllocator channelAllocator( this, pResolution, pOptions );
	StaticLoop< ChannelAllocator, GPUPool< HostArray, TList, GPUPoolChannelUnitPointer >::numChannels - 1 >::go( channelAllocator );

	// Retrieve and initialize all device-side channel arrays
	GPUPoolHost< HostArray, TList >::ChannelInitializer channelInitializer( this );
	StaticLoop< ChannelInitializer, GPUPool< HostArray, TList, GPUPoolChannelUnitPointer >::numChannels - 1>::go( channelInitializer );

	GV_CHECK_CUDA_ERROR( "GPUPoolHost:GPUPoolHost" );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< template< typename > class HostArray, class TList >
GPUPoolHost< HostArray, TList >
::~GPUPoolHost()
{
	// Free memory of all data channels in the associated pool
	GPUPoolHost< HostArray, TList >::GvChannelDesallocator channelDesallocator( this );
	StaticLoop< GvChannelDesallocator, GPUPool< HostArray, TList, GPUPoolChannelUnitPointer >::numChannels - 1 >::go( channelDesallocator );
	
	GV_CHECK_CUDA_ERROR( "GPUPoolHost:~GPUPoolHost" );
}

/******************************************************************************
 * Get the device-side associated object
 *
 * @return the device-side associated object
 ******************************************************************************/
template< template< typename > class HostArray, class TList >
typename GPUPool_KernelPoolFromHostPool< HostArray, TList >::Result& GPUPoolHost< HostArray, TList >
::getKernelPool()
{
	return gpuPoolKernel;
}

/******************************************************************************
 * ...
 *
 * @param Loki::Int2Type< poolName > ...
 * @param normalizedResult ...
 * @param normalizedAccess ...
 * @param filterMode ...
 * @param addressMode ...
 ******************************************************************************/
template< template< typename > class HostArray, class TList >
template< int poolName >
void GPUPoolHost< HostArray, TList >
::bindPoolToTextureReferences( Loki::Int2Type< poolName >, bool normalizedResult, bool normalizedAccess, cudaTextureFilterMode filterMode, cudaTextureAddressMode addressMode )
{
	BindToTexRef< poolName > tempFunctor( this, normalizedResult, normalizedAccess, filterMode, addressMode );
	StaticLoop< BindToTexRef< poolName >, GPUPool< HostArray, TList, GPUPoolChannelUnitPointer >::numChannels - 1 >::go( tempFunctor );
}

/******************************************************************************
 * ...
 ******************************************************************************/
template< template< typename > class HostArray, class TList >
void GPUPoolHost< HostArray, TList >
::bindPoolToSurfaceReferences()
{
	BindToSurfRef tempFunctor( this );
	StaticLoop< BindToSurfRef, GPUPool< HostArray, TList, GPUPoolChannelUnitPointer >::numChannels - 1 >::go( tempFunctor );
}

/******************************************************************************
 * ...
 ******************************************************************************/
template< template< typename > class HostArray, class TList >
typename GPUPoolHost< HostArray, TList >::KernelPoolType GPUPoolHost< HostArray, TList >
::getKernelObject()
{
	return gpuPoolKernel;
}

} // namespace GvCore
