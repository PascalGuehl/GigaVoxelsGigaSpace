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

#ifndef GVMATH_H
#define GVMATH_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

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
	 * @struct Log2
	 *
	 * @brief The Log2 struct provides...
	 *
	 * @ingroup GvCore
	 *
	 * ...
	 *
	 * @note All is done at compile-time.
	 */
	template< uint TValue >
	struct Log2
	{
		/**
		 * ...
		 */
		enum
		{
			value = Log2< ( TValue >> 1 ) >::value + 1
		};
	};

	/**
	 * Log2 struct specialization
	 *
	 * @note All is done at compile-time.
	 */
	template<>
	struct Log2< 1 >
	{
		/**
		 * ...
		 */
		enum
		{
			value = 0
		};
	};

	/**
	 * Log2 struct specialization
	 *
	 * @note All is done at compile-time.
	 *
	 * @todo Need a Log2<> template which round-up the results.
	 */
	template<>
	struct Log2< 3 >
	{
		/**
		 * ...
		 */
		enum
		{
			value = 2
		};
	};

} // namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

	/** 
	 * @struct Max
	 *
	 * @brief The Max struct provides...
	 *
	 * @ingroup GvCore
	 *
	 * ...
	 *
	 * @note All is done at compile-time.
	 */
	template< int Ta, int Tb >
	struct Max
	{
		/**
		 * ...
		 */
		enum
		{
			value = Ta > Tb ? Ta : Tb
		};
	};

} //namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

	/** 
	 * @struct Min
	 *
	 * @brief The Min struct provides...
	 *
	 * @ingroup GvCore
	 *
	 * ...
	 *
	 * @note All is done at compile-time.
	 */
	template< int Ta, int Tb >
	struct Min
	{
		/**
		 * ...
		 */
		enum
		{
			value = Ta < Tb ? Ta : Tb
		};
	};

} //namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

	/** 
	 * @struct Min
	 *
	 * @brief The Min struct provides...
	 *
	 * @ingroup GvCore
	 *
	 * ...
	 *
	 * @note All is done at compile-time.
	 */
	template< int Ta, int Tb >
	struct IDivUp
	{
		/**
		 * ...
		 */
		enum
		{
			value = ( Ta % Tb != 0 ) ? ( Ta / Tb + 1 ) : ( Ta / Tb )
		};
	};

} //namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#endif
