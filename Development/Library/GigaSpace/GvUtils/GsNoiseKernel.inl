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
//#include "GvUtils/GsNoise.h"

// Cuda
#include <math_functions.h>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{
/******************************************************************************
 * Compute the Perlin noise given a 3D position
 *
 * @param x x coordinate position
 * @param y y coordinate position
 * @param z z coordinate position
 *
 * @return the noise at given position
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::getValue( float x, float y, float z )
{
	int X = int( floorf( x )) & 255,
		Y = int( floorf( y )) & 255,
		Z = int( floorf( z )) & 255;

	x -= floorf( x );
	y -= floorf( y );
	z -= floorf( z );

	float u = fade( x );
	float v = fade( y );
	float w = fade( z );

	int A = gs_permutationTable[X]+Y, AA = gs_permutationTable[A]+Z, AB = gs_permutationTable[A+1]+Z,
		B = gs_permutationTable[X+1]+Y, BA = gs_permutationTable[B]+Z, BB = gs_permutationTable[B+1]+Z;

	return lerp( 
			lerp( 
				lerp( grad( gs_permutationTable[AA], x, y, z ),
					  grad( gs_permutationTable[BA], x - 1, y, z ), 
					  u ),
				lerp( grad( gs_permutationTable[AB], x, y - 1, z ),
					  grad( gs_permutationTable[BB], x - 1, y - 1, z ), 
					  u ),
			   	v ),
			lerp( 
				lerp( grad( gs_permutationTable[AA + 1], x, y, z - 1 ),
					  grad( gs_permutationTable[BA + 1], x - 1, y, z - 1 ), 
					  u ),
			    lerp( grad( gs_permutationTable[AB + 1], x, y - 1, z - 1 ),
					  grad( gs_permutationTable[BB + 1], x - 1, y - 1, z - 1 ), 
					  u ), 
			    v ),
			w );
}

/******************************************************************************
 * Compute the Perlin noise given a 3D position
 *
 * @param pPoint 3D position
 *
 * @return the noise at given position
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::getValue( float3 pPoint )
{
	return getValue( pPoint.x, pPoint.y, pPoint.z );
}

/******************************************************************************
 * Fade function
 *
 * @param t parameter
 *
 * @return ...
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::fade( float t )
{
	return t * t * t * ( t * ( t * 6.f - 15.f ) + 10.f );
}

/******************************************************************************
 * Grad function
 *
 * @param hash hash
 * @param x x
 * @param y y
 * @param z z
 *
 * @return ...
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::grad( int hash, float x, float y, float z )
{
	// Compute conditions prior to doing branching
	// (help the compiler parallelize computation).
	const int h = hash & 15;
	bool c1 = h < 8;
	bool c2 = h < 4;
	bool c3 = h == 12 || h == 14;
	bool c4 = (h & 1) == 0;
	bool c5 = (h & 2) == 0;
	const float u = c1 ? x : y;
	const float v = c2 ? y : c3 ? x : z;
	return (c4 ? u : -u) + ( c5 ? v : -v);

	// Clean version
	//const int h = hash & 15;
	//const float u = h < 8 ? x : y;
	//const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	//return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

/******************************************************************************
 * Compute the Perlin noise given a 3D position using preinitialized textures.
 *
 * @param x x coordinate position
 * @param y y coordinate position
 * @param z z coordinate position
 *
 * @return the noise at given position
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::getValueT( const float x, const float y, const float z )
{
	return GsNoiseKernel::getValueT( make_float3( x, y, z ) );
}

/******************************************************************************
 * Compute the Perlin noise given a 3D position using preinitialized textures.
 *
 * @param pPoint 3D position
 *
 * @return the noise at given position
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::getValueT( const float3 point )
{
	float3 P;
	// The next 6 lines are equivalents (but faster) to:
	// P = static_cast< float >( static_cast< int >( floor( point )) % 256.f ) / 256.f
	// The division by 256 is necessary because P will be used to access a denormalized 
	// 	texture.
	P.x = floorf( point.x ) * ( 1.f / 256.f );
	P.y = floorf( point.y ) * ( 1.f / 256.f );
	P.z = floorf( point.z ) * ( 1.f / 256.f );
	P.x -= floorf( P.x );
	P.y -= floorf( P.y );
	P.z -= floorf( P.z );

	float3 p = point;
	p.x -= floorf( point.x );
	p.y -= floorf( point.y );
	p.z -= floorf( point.z );

	float3 f;
	f.x = fade( p.x );
	f.y = fade( p.y );
	f.z = fade( p.z );

	// Get the 4 needed values from the texture in a single fetch.
	const float4 perm = permSampleT( P.x, P.y ) + P.z;
	const float AA = perm.x;
	const float AB = perm.y;
	const float BA = perm.z;
	const float BB = perm.w;

	return lerp(
				lerp(
					lerp( gradT( AA, p ),
						  gradT( BA, p + make_float3( -1.f, 0.f, 0.f )),
						  f.x ),
					lerp( gradT( AB, p + make_float3( 0.f, -1.f, 0.f )),
						  gradT( BB, p + make_float3( -1.f, -1.f, 0.f )),
						  f.x ),
					f.y ),
				lerp(
					// Normally we should add 1 but AA, BA, AB and BB are still normalized, 
					// 	so we only add 1/256.
					lerp( gradT( AA + 1.f / 256.f, p + make_float3( 0.f, 0.f, -1.f )),
						  gradT( BA + 1.f / 256.f, p + make_float3( -1.f, 0.f, -1.f )),
						  f.x ),
					lerp( gradT( AB + 1.f / 256.f, p + make_float3( 0.f, -1.f, -1.f )),
						  gradT( BB + 1.f / 256.f, p + make_float3( -1.f, -1.f, -1.f )),
						  f.x ),
					f.y ),
				f.z );
}

/******************************************************************************
 * Take a sample in the permutation table.
 ******************************************************************************/
__device__
__forceinline__ float4 GsNoiseKernel::permSampleT( float x, float y )
{
	// x and y are already divided by 256.
	// The result is immediately divided by 256, we need it this way to prepare the 
	// fetch in the next texture (grad).
	return tex2D( gs_permutationTableTexture, x, y ) * 255.f / 256.f;
}

/******************************************************************************
 * Grad function using textures
 *
 * @param hash hash
 * @param p p
 *
 * @return ...
 ******************************************************************************/
__device__
__forceinline__ float GsNoiseKernel::gradT( float hash, float3 p )
{
	// The lookup table is filled with numbers such as there is no need to denormalized the result.
	float3 g = make_float3( tex1D( gs_gradientTexture, hash ));
	return dot( g, p );
}

} // namespace GvUtils
