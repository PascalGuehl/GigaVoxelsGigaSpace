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

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Inititialize.
 *
 * @param pData pointer on data
 * @param pRes resolution
 * @param pPitch pitch
 ******************************************************************************/
template< typename T >
inline void GsLinearMemoryKernel< T >::init( T* pData, const uint3& pRes, size_t pPitch )
{
	_resolution = pRes;
	_data = pData;
	_pitch = pPitch;
	_pitchxy = _resolution.x * _resolution.y;
}

/******************************************************************************
 * Get the resolution.
 *
 * @return the resolution
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ uint3 GsLinearMemoryKernel< T >::getResolution() const
{
	return _resolution;
}

/******************************************************************************
 * Get the memory size.
 *
 * @return the memory size
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ size_t GsLinearMemoryKernel< T >::getMemorySize() const
{
	return __uimul( __uimul( __uimul( _resolution.x, _resolution.y ), _resolution.z ), sizeof( T ) );
}

/******************************************************************************
 * Get the value at a given 1D address.
 *
 * @param pAddress a 1D address
 *
 * @return the value at the given address
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ /*const*/ T GsLinearMemoryKernel< T >::get( uint pAddress ) const
{
	return _data[ pAddress ];
}

/******************************************************************************
 * Get the value at a given 2D position.
 *
 * @param pPosition a 2D position
 *
 * @return the value at the given position
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ /*const*/ T GsLinearMemoryKernel< T >::get( const uint2& pPosition ) const
{
	return _data[ getOffset( pPosition ) ];
}

/******************************************************************************
 * Get the value at a given 3D position.
 *
 * @param pPosition a 3D position
 *
 * @return the value at the given position
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ /*const*/ T GsLinearMemoryKernel< T >::get( const uint3& pPosition ) const
{
	return _data[ getOffset( pPosition ) ];
}

/******************************************************************************
 * Get the value at a given 1D address in a safe way.
 * Bounds are checked and address is modified if needed (as a clamp).
 *
 * @param pAddress a 1D address
 *
 * @return the value at the given address
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ /*const*/ T GsLinearMemoryKernel< T >::getSafe( uint pAddress ) const
{
	uint numelem = _pitchxy * _resolution.z;
	if
		( pAddress >= numelem )
	{
		pAddress = numelem - 1;
	}

	return _data[ pAddress ];
}

/******************************************************************************
 * Get the value at a given 3D position in a safe way.
 * Bounds are checked and position is modified if needed (as a clamp).
 *
 * @param pPosition a 3D position
 *
 * @return the value at the given position
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ /*const*/ T GsLinearMemoryKernel< T >::getSafe( uint3 pPosition ) const
{
	pPosition = getSecureIndex( pPosition );

	return _data[ getOffset( pPosition ) ];
}

/******************************************************************************
 * Get a pointer on data at a given 1D address.
 *
 * @param pAddress a 1D address
 *
 * @return the pointer at the given address
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ T* GsLinearMemoryKernel< T >::getPointer( uint pAddress )
{
	return _data + pAddress;
}

/******************************************************************************
 * Set the value at a given 1D address in the data array.
 *
 * @param pAddress a 1D address
 * @param pVal a value
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ void GsLinearMemoryKernel< T >::set( const uint pAddress, T pVal )
{
	_data[ pAddress ] = pVal;
}

/******************************************************************************
 * Set the value at a given 2D position in the data array.
 *
 * @param pPosition a 2D position
 * @param pVal a value
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ void GsLinearMemoryKernel< T >::set( const uint2& pPosition, T pVal )
{
	_data[ getOffset( pPosition ) ] = pVal;
}

/******************************************************************************
 * Set the value at a given 3D position in the data array.
 *
 * @param pPosition a 3D position
 * @param pVal a value
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ void GsLinearMemoryKernel< T >::set( const uint3& pPosition, T pVal )
{
	_data[ getOffset( pPosition ) ] = pVal;
}

/******************************************************************************
 * Helper function used to get the corresponding index array at a given
 * 3D position in a safe way.
 * Position is checked and modified if needed (as a clamp).
 *
 * @param pPosition a 3D position
 *
 * @return the corresponding index array at the given 3D position
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ uint3 GsLinearMemoryKernel< T >::getSecureIndex( uint3 pPosition ) const
{
	if ( pPosition.x >= _resolution.x )
	{
		pPosition.x = _resolution.x - 1;
	}

	if ( pPosition.y >= _resolution.y )
	{
		pPosition.y = _resolution.y - 1;
	}

	if ( pPosition.z >= _resolution.z )
	{
		pPosition.z = _resolution.z - 1;
	}

	return pPosition;
}

/******************************************************************************
 * Helper function used to get the offset in the 1D linear data array
 * given a 3D position.
 *
 * @param pPosition a 3D position
 *
 * @return the corresponding offset in the 1D linear data array
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ uint GsLinearMemoryKernel< T >::getOffset( const uint3& pPosition ) const
{
	//return position.x + position.y * _resolution.x + position.z * _pitchxy;
	return pPosition.x + __uimul( pPosition.y, _resolution.x ) + __uimul( pPosition.z, _pitchxy );
}

/******************************************************************************
 * Helper function used to get the offset in the 1D linear data array
 * given a 2D position.
 *
 * @param pPosition a 2D position
 *
 * @return the corresponding offset in the 1D linear data array
 ******************************************************************************/
template< typename T >
__device__
__forceinline__ uint GsLinearMemoryKernel< T >::getOffset( const uint2& pPosition ) const
{
	//return pPosition.x + pPosition.y * _resolution.x ;
	return pPosition.x + __uimul( pPosition.y, _resolution.x ) ;
}

} // namespace GvCore
