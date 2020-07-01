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

#ifndef _TEMPLATEHELPERS_H_
#define _TEMPLATEHELPERS_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * MACRO.
 */
#define TEMPLATE_INSTANCIATE_CLASS( T1, T2 ) \
	template class T1< T2 >;

/**
 * MACRO.
 */
#define TEMPLATE_INSTANCIATE_CLASS_UCHAR_TYPES( T1 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uchar ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uchar2 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uchar3 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uchar4 )

/**
 * MACRO.
 */
#define TEMPLATE_INSTANCIATE_CLASS_UINT_TYPES( T1 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uint ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uint2 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uint3 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, uint4 )

/**
 * MACRO.
 */
#define TEMPLATE_INSTANCIATE_CLASS_FLOAT_TYPES(T1) \
	TEMPLATE_INSTANCIATE_CLASS( T1, float ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, float2 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, float3 ) \
	TEMPLATE_INSTANCIATE_CLASS( T1, float4 )

/**
 * MACRO.
 */
#define TEMPLATE_INSTANCIATE_CLASS_TYPES( T1 ) \
	TEMPLATE_INSTANCIATE_CLASS_UCHAR_TYPES( T1 ) \
	TEMPLATE_INSTANCIATE_CLASS_UINT_TYPES( T1 ) \
	TEMPLATE_INSTANCIATE_CLASS_FLOAT_TYPES( T1 )

#endif // !_TEMPLATEHELPERS_H_

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/
