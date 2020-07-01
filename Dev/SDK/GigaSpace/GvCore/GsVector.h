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

#ifndef GVSTATICRES3D_H
#define GVSTATICRES3D_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsMath.h"
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
	 * @struct GsVec3D
	 *
	 * @brief The GsVec3D struct provides the concept of a 3D resolution.
	 *
	 * @ingroup GvCore
	 *
	 * This is notably used to define space resolution/extent of node tiles and bricks of voxels.
	 *
	 * @note All members are computed at compile-time.
	 */
	template< uint Trx, uint Try, uint Trz >
	struct GsVec3D
	{

		/**************************************************************************
		 ***************************** PUBLIC SECTION *****************************
		 **************************************************************************/

		/****************************** INNER TYPES *******************************/

		/**
		 * Resolution in each axis.
		 */
		enum
		{
			x = Trx,
			y = Try,
			z = Trz
		};

		/**
		 * Total number of elements.
		 */
		enum
		{
			numElements = x * y * z
		};

		/**
		 * Precomputed log2() value of each axis resolution.
		 */
		enum
		{
			xLog2 = Log2< Trx >::value,
			yLog2 = Log2< Try >::value,
			zLog2 = Log2< Trz >::value
		};

		/**
		 * Precomputed min resolution.
		 */
		enum
		{
			minRes = Min< Min< x, y >::value, z >::value
		};

		/**
		 * Precomputed max resolution
		 */
		enum
		{
			maxRes = Max< Max< x, y >::value, z >::value
		};

		/**
		 * Precomputed boolean value to specify if each axis resolution is a power of two.
		 */
		enum
		{
			xIsPOT = ( x & ( x - 1 ) ) == 0,
			yIsPOT = ( y & ( y - 1 ) ) == 0,
			zIsPOT = ( z & ( z - 1 ) ) == 0
		};

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**
		 * Return the resolution as a uint3.
		 *
		 * @return the resolution
		 */
		__host__ __device__
		static uint3 get();

		/**
		 * Return the resolution as a float3.
		 *
		 * @return the resolution
		 */
		__host__ __device__
		static float3 getFloat3();

		/**
		 * Return the number of elements
		 *
		 * @return the number of elements
		 */
		__device__ __host__
		static uint getNumElements();

		//__host__ __device__
		//static uint getNumElementsLog2();

		/**
		 * Return the log2(resolution) as an uint3.
		 *
		 * @return the log2(resolution)
		 */
		__host__ __device__
		static uint3 getLog2();

		/**
		 * Convert a three-dimensionnal value to a linear value.
		 *
		 * @param pValue The 3D value to convert
		 *
		 * @return the 1D linearized converted value
		 */
		__host__ __device__
		static uint toFloat1( uint3 pValue );

		/**
		 * Convert a linear value to a three-dimensionnal value.
		 *
		 * @param pValue The 1D value to convert
		 *
		 * @return the 3D converted value
		 */
		__host__ __device__
		static uint3 toFloat3( uint pValue );

		/**************************************************************************
		 **************************** PROTECTED SECTION ***************************
		 **************************************************************************/

	protected:

		/****************************** INNER TYPES *******************************/

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**************************************************************************
		 ***************************** PRIVATE SECTION ****************************
		 **************************************************************************/

	private:

		/****************************** INNER TYPES *******************************/

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/
		
	};

} //namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

	/** 
	 * @struct GsVec1D
	 *
	 * @brief The GsVec1D struct provides the concept of a uniform 3D resolution.
	 *
	 * @ingroup GvCore
	 *
	 * This is a specialization of a GsVec3D 3D resolution where each dimension is equal.
	 * This is notably used to define space resolution/extent of node tiles and bricks of voxels.
	 *
	 * @note All members are computed at compile-time.
	 *
	 * @todo Not sure this is the best way to do it.
	 */
	template< uint Tr >
	struct GsVec1D : GsVec3D< Tr, Tr, Tr >
	{

		/**************************************************************************
		 ***************************** PUBLIC SECTION *****************************
		 **************************************************************************/

		/****************************** INNER TYPES *******************************/

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**************************************************************************
		 **************************** PROTECTED SECTION ***************************
		 **************************************************************************/

	protected:

		/****************************** INNER TYPES *******************************/

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**************************************************************************
		 ***************************** PRIVATE SECTION ****************************
		 **************************************************************************/

	private:

		/****************************** INNER TYPES *******************************/

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

	};

} //namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsVector.inl"

#endif
