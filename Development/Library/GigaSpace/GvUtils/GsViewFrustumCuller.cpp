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

#include "GvUtils/GsViewFrustumCuller.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <limits>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvUtils;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsViewFrustumCuller::GsViewFrustumCuller()
{
}

/******************************************************************************
 * Desconstructor
 ******************************************************************************/
GsViewFrustumCuller::~GsViewFrustumCuller()
{
}

/******************************************************************************
 * Fast extraction of viewing frustum planes from the Model-View-Projection matrix
 *
 * @param pMatrix ...
 * @param pNormalize ...
 ******************************************************************************/
void GsViewFrustumCuller::extractViewingFrustumPlanes( const float4x4& pMatrix, bool pNormalize )
{
	// Matrices are stored in column-major order :
	//
	// 0  4  8 12
	// 1  5  9 13
	// 2  6 10 14
	// 3  7 11 15

	// Left clipping plane
	_planes[ eLeft ].x = pMatrix._array[ 3 ] + pMatrix._array[ 0 ];
	_planes[ eLeft ].y = pMatrix._array[ 7 ] + pMatrix._array[ 4 ];
	_planes[ eLeft ].z = pMatrix._array[ 11 ] + pMatrix._array[ 8 ];
	_planes[ eLeft ].w = pMatrix._array[ 15 ] + pMatrix._array[ 12 ];

	// Right clipping plane
	_planes[ eRight ].x = pMatrix._array[ 3 ] - pMatrix._array[ 0 ];
	_planes[ eRight ].y = pMatrix._array[ 7 ] - pMatrix._array[ 4 ];
	_planes[ eRight ].z = pMatrix._array[ 11 ] - pMatrix._array[ 8 ];
	_planes[ eRight ].w = pMatrix._array[ 15 ] - pMatrix._array[ 12 ];

	// Bottom clipping plane
	_planes[ eBottom ].x = pMatrix._array[ 3 ] + pMatrix._array[ 1 ];
	_planes[ eBottom ].y = pMatrix._array[ 7 ] + pMatrix._array[ 5 ];
	_planes[ eBottom ].z = pMatrix._array[ 11 ] + pMatrix._array[ 9 ];
	_planes[ eBottom ].w = pMatrix._array[ 15 ] + pMatrix._array[ 13 ];

	// Top clipping plane
	_planes[ eTop ].x = pMatrix._array[ 3 ] - pMatrix._array[ 1 ];
	_planes[ eTop ].y = pMatrix._array[ 7 ] - pMatrix._array[ 5 ];
	_planes[ eTop ].z = pMatrix._array[ 11 ]- pMatrix._array[ 9 ];
	_planes[ eTop ].w = pMatrix._array[ 15 ] - pMatrix._array[ 13 ];

	// Near clipping plane
	_planes[ eNear ].x = pMatrix._array[ 3 ] + pMatrix._array[ 2 ];
	_planes[ eNear ].y = pMatrix._array[ 7 ] + pMatrix._array[ 6 ];
	_planes[ eNear ].z = pMatrix._array[ 11 ] + pMatrix._array[ 10 ];
	_planes[ eNear ].w = pMatrix._array[ 15 ] + pMatrix._array[ 14 ];

	// Far clipping plane
	_planes[ eFar ].x = pMatrix._array[ 3 ] - pMatrix._array[ 2 ];
	_planes[ eFar ].y = pMatrix._array[ 7 ] - pMatrix._array[ 6 ];
	_planes[ eFar ].z = pMatrix._array[ 11 ] - pMatrix._array[ 10 ];
	_planes[ eFar ].w = pMatrix._array[ 15 ] - pMatrix._array[ 14 ];

	// Normalize the plane equations, if requested
	if ( pNormalize )
	{
		_planes[ eLeft ] = normalize( _planes[ eLeft ] );
		_planes[ eRight ] = normalize( _planes[ eRight ] );
		_planes[ eBottom ] = normalize( _planes[ eBottom ] );
		_planes[ eTop ] = normalize( _planes[ eTop ] );
		_planes[ eNear ] = normalize( _planes[ eNear ] );
		_planes[ eFar ] = normalize( _planes[ eFar ] );
	}
}

/******************************************************************************
 * Frustum / Box intersection
 ******************************************************************************/
int GsViewFrustumCuller::frustumBoxIntersect()
{
	bool intersecting = false;
	int result;

	// Iterate through viewing frustum planes
	for ( int i = 0; i < eNbViewingFrustumPlanes; i++ )
	{
		result = planeAABBIntersect();

		if ( result == eOutside )
		{
			return eOutside;
		}
		else if ( result == eIntersecting )
		{
			intersecting = true;
		}
	}

	if ( intersecting )
	{
		return eIntersecting;
	}
	else
	{
		return eInside;
	}
}

/******************************************************************************
 * Plane / AABB intersection
 ******************************************************************************/
int GsViewFrustumCuller::planeAABBIntersect()
{
	return 0;
}

/******************************************************************************
 * Frustum culling test
 *
 * @param pMatrix the matrix in which frustum planes will be extracted
 *
 * @return false if fully outside, true if inside or intersects
 ******************************************************************************/
bool GsViewFrustumCuller::boxInFrustum( const float4x4& pMatrix )
{
	// TODO
	// - if pMatrix is MVP, the extracted planes are in Model-space where GigaSpace BBox lies in [0;1]x[0;1]x[0;1]
	// - if not, it won't work...

	// Extract planes and normalize them
	extractViewingFrustumPlanes( pMatrix, true );
		
    // Check box outside/inside of frustum
	int out = 0;
    for ( int i = 0; i < eNbViewingFrustumPlanes; i++ )
    {        
        out += ( ( dot( _planes[ i ], make_float4( 0.0f, 0.0f, 0.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 1.0f, 0.0f, 0.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 0.0f, 1.0f, 0.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 1.0f, 1.0f, 0.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 0.0f, 0.0f, 1.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 1.0f, 0.0f, 1.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 0.0f, 1.0f, 1.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );
        out += ( ( dot( _planes[ i ], make_float4( 1.0f, 1.0f, 1.0f, 1.0f ) ) < 0.0 ) ? 1 : 0 );

        if ( out == 8 )
		{
			return false;
		}
	}

	// TODO
	// - add 8 corners points of frustum to correctly handled intersection test
	//
	//// Check frustum outside/inside box
	//out = 0;
	//for ( int i = 0; i < 8; i++ )
	//{
	//	out += ( ( fru.mPoints[ i ].x > 1.0f ) ? 1 : 0 );
	//}
	//if ( out == 8 )
	//{
	//	return false;
	//}
	//out = 0;
	//for ( int i = 0; i < 8; i++ )
	//{
	//	out += ( ( fru.mPoints[ i ].x < 0.0f ) ? 1 : 0 );
	//}
	//if ( out == 8 )
	//{
	//	return false;
	//}
	//out = 0;
	//for ( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y > 1.0f )?1:0); if( out==8 ) return false;
	//out = 0;
	//for ( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y < 0.0f )?1:0); if( out==8 ) return false;
	//out = 0;
	//for ( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z > 1.0f )?1:0); if( out==8 ) return false;
	//out = 0;
	//for ( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z < 0.0f )?1:0); if( out==8 ) return false;
	
	return true;
}
