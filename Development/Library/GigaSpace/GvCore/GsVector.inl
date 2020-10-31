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
 * Return the resolution as a uint3.
 *
 * @return the resolution
 ******************************************************************************/
template< uint Trx, uint Try, uint Trz >
__host__ __device__
inline uint3 GsVec3D< Trx, Try, Trz >::get()
{
	return make_uint3( x, y, z );
}

/******************************************************************************
 * Return the resolution as a float3.
 *
 * @return the resolution
 ******************************************************************************/
template< uint Trx, uint Try, uint Trz >
__host__ __device__
inline float3 GsVec3D< Trx, Try, Trz >::getFloat3()
{
	return make_float3( static_cast< float >( x ), static_cast< float >( y ), static_cast< float >( z ) );
}

/******************************************************************************
 * Return the number of elements
 *
 * @return the number of elements
 ******************************************************************************/
template< uint Trx, uint Try, uint Trz >
__host__ __device__
inline uint GsVec3D< Trx, Try, Trz >::getNumElements()
{
	return x * y * z;
}

/******************************************************************************
 *
 ******************************************************************************/
//template< uint Trx, uint Try, uint Trz >
//__host__ __device__
//inline uint GsVec3D< Trx, Try, Trz >::getNumElementsLog2()
//{
//	return Log2< x * y * z >::value;
//}

/******************************************************************************
 * Return the log2(resolution) as an uint3.
 *
 * @return the log2(resolution)
 ******************************************************************************/
template< uint Trx, uint Try, uint Trz >
__host__ __device__
inline uint3 GsVec3D< Trx, Try, Trz >::getLog2()
{
	return make_uint3( Log2< x >::value, Log2< y >::value, Log2< z >::value );
}

/******************************************************************************
 * Convert a three-dimensionnal value to a linear value.
 *
 * @param pValue The 3D value to convert
 *
 * @return the 1D linearized converted value
 ******************************************************************************/
template< uint Trx, uint Try, uint Trz >
__host__ __device__
inline uint GsVec3D< Trx, Try, Trz >::toFloat1( uint3 pValue )
{
	if ( xIsPOT && yIsPOT && zIsPOT )
	{
		return pValue.x | ( pValue.y << xLog2 ) | ( pValue.z << ( xLog2 + yLog2 ) );
	}
	else
	{
		return pValue.x + pValue.y * x + pValue.z * x * y;
	}
}

/******************************************************************************
 * Convert a linear value to a three-dimensionnal value.
 *
 * @param pValue The 1D value to convert
 *
 * @return the 3D converted value
 ******************************************************************************/
template< uint Trx, uint Try, uint Trz >
__host__ __device__
inline uint3 GsVec3D< Trx, Try, Trz >::toFloat3( uint pValue )
{
	if ( xIsPOT && yIsPOT && zIsPOT )
	{
		/*r.x = n & xLog2;
		r.y = (n >> xLog2) & yLog2;
		r.z = (n >> (xLog2 + yLog2)) & zLog2;*/
		return make_uint3( pValue & xLog2, ( pValue >> xLog2 ) & yLog2, ( pValue >> ( xLog2 + yLog2 ) ) & zLog2 );
	}
	else
	{
		/*r.x = n % x;
		r.y = (n / x) % y;
		r.z = (n / (x * y)) % z;*/
		return make_uint3( pValue % x, ( pValue / x ) % y, ( pValue / ( x * y ) ) % z );
	}
}

} // namespace GvCore
