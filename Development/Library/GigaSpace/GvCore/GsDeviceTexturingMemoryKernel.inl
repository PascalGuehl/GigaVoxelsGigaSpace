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
#include "GvStructure/GsVolumeTreeKernel.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

	/******************************************************************************
	 * Get the value at given position
	 *
	 * @param position position
	 *
	 * @return the value at given position
	 ******************************************************************************/
	template< typename T >
	template< uint channel >
	__device__
	__forceinline__ T GsDeviceTexturingMemoryKernel< T >::get( const uint3& position ) const
	{
		T data;

#if (__CUDA_ARCH__ >= 200)
		// FIXME : better way to do this ?
		switch ( channel )
		{
		case 0:
			surf3Dread( &data, CreateGPUPoolChannelSurfaceReferenceName( 0 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 1:
			surf3Dread( &data, CreateGPUPoolChannelSurfaceReferenceName( 1 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 2:
			surf3Dread( &data, CreateGPUPoolChannelSurfaceReferenceName( 2 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 3:
			surf3Dread( &data, CreateGPUPoolChannelSurfaceReferenceName( 3 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 4:
			surf3Dread( &data, CreateGPUPoolChannelSurfaceReferenceName( 4 ), position.x * sizeof( T ), position.y, position.z );
			break;

		default:
			assert( false );	// TO DO : handle this.
			break;
		}
#endif
		return (data);
	}

	/******************************************************************************
	 * Set the value at given position
	 *
	 * @param position position
	 * @param val the value to write
	 ******************************************************************************/
	template< typename T >
	template< uint channel >
	__device__
	__forceinline__ void GsDeviceTexturingMemoryKernel< T >::set( const uint3& position, T val )
	{
#if (__CUDA_ARCH__ >= 200)
		// FIXME : better way to do this ?
		switch ( channel )
		{
		case 0:
            surf3Dwrite( val, CreateGPUPoolChannelSurfaceReferenceName( 0 ), position.x * sizeof( T ), position.y, position.z );
            break;

		case 1:
            surf3Dwrite( val, CreateGPUPoolChannelSurfaceReferenceName( 1 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 2:
            surf3Dwrite( val, CreateGPUPoolChannelSurfaceReferenceName( 2 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 3:
            surf3Dwrite( val, CreateGPUPoolChannelSurfaceReferenceName( 3 ), position.x * sizeof( T ), position.y, position.z );
			break;

		case 4:
            surf3Dwrite( val, CreateGPUPoolChannelSurfaceReferenceName( 4 ), position.x * sizeof( T ), position.y, position.z );
			break;

		default:
			assert( false );	// TO DO : handle this.
			break;
		}
#endif
	}

} // namespace GvCore
