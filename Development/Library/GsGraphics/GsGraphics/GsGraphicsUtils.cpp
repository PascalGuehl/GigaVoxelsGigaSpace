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

#include "GsGraphics/GsGraphicsUtils.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// System
#include <cassert>
#include <cstdio>

// STL
#include <iostream>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GsGraphics;

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
 * Helper functrion to copy GL matrix from GLdouble[ 16 ] to float[ 16 ] array
 * - ex : modelview or projection
 *
 * @param pDestinationMatrix
 * @param pSourceMatrix
 ******************************************************************************/
void GsGraphicsUtils::copyGLMatrix( float* pDestinationMatrix, GLdouble* pSourceMatrix )
{
	for ( int i = 0; i < 16; i++ )
	{
		pDestinationMatrix[ i ] = static_cast< float >( pSourceMatrix[ i ] );
	}
}

/******************************************************************************
 * Helper functrion to copy a pair  of GL matrices from GLdouble[ 16 ] to float[ 16 ] array
 * - ex : modelview and projection
 *
 * @param pDestinationMatrix1
 * @param pSourceMatrix1
 * @param pDestinationMatrix2
 * @param pSourceMatrix2
 ******************************************************************************/
void GsGraphicsUtils::copyGLMatrices( float* pDestinationMatrix1, GLdouble* pSourceMatrix1,
									  float* pDestinationMatrix2, GLdouble* pSourceMatrix2 )
{
	for ( int i = 0; i < 16; i++ )
	{
		pDestinationMatrix1[ i ] = static_cast< float >( pSourceMatrix1[ i ] );
		pDestinationMatrix2[ i ] = static_cast< float >( pSourceMatrix2[ i ] );
	}
}
