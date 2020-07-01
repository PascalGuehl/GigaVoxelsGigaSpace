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

#ifndef _TYPEHELPERS_H_
#define _TYPEHELPERS_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"

// Cuda
#include <vector_types.h>

// System
#include <cassert>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * MACRO definition.
 * Used to convert a type to a string.
 */
#define GS_DECLARE_TYPE_STRING( TType ) \
	template<> \
	const char* typeToString< TType >() \
	{ \
		return #TType; \
	}

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{
	/**
	 * Convert a type to a string
	 *
	 * @return the type as a string
	 */
	template< typename TType >
	const char* typeToString()
	{
		bool Unsupported_Type = false;
		assert( Unsupported_Type );
		return "<unsupported-type>";
	}

	// Template specialization of the typeToString() method

	// Char types
	GS_DECLARE_TYPE_STRING( char )
	GS_DECLARE_TYPE_STRING( char2 )
	GS_DECLARE_TYPE_STRING( char4 )

	// Unsigned char types
	GS_DECLARE_TYPE_STRING( uchar )
	GS_DECLARE_TYPE_STRING( uchar2 )
	GS_DECLARE_TYPE_STRING( uchar4 )

	// Short types
	GS_DECLARE_TYPE_STRING( short )
	GS_DECLARE_TYPE_STRING( short2 )
	GS_DECLARE_TYPE_STRING( short4 )

	// Unsigned short types
	GS_DECLARE_TYPE_STRING( ushort )
	GS_DECLARE_TYPE_STRING( ushort2 )
	GS_DECLARE_TYPE_STRING( ushort4 )

	// Half types
	//GS_DECLARE_TYPE_STRING( half )
	GS_DECLARE_TYPE_STRING( half2 )
	GS_DECLARE_TYPE_STRING( half4 )

	// Float types
	GS_DECLARE_TYPE_STRING( float )
	GS_DECLARE_TYPE_STRING( float2 )
	GS_DECLARE_TYPE_STRING( float4 )

}

#endif // !_TYPEHELPERS_H_
