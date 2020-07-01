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

#ifndef GVARRAY3D_H
#define GVARRAY3D_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvCore/GsLinearMemoryKernel.h"

// System
#include <cassert>

// Cuda
#include <cuda_runtime.h>
#include <host_defines.h>
#include <vector_types.h> 
#include <driver_types.h>
#include <driver_functions.h>

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
	 * @class Array3D
	 *
	 * @brief The Array3D class provides the concept of a generic 3D array.
	 *
	 * @ingroup GvCore
	 *
	 * It enables to manipulate 3D data whereas, internally, data is stored as a linearized 1D array.
	 */
	template< typename T >
	class Array3D
	{
		/**************************************************************************
		 ***************************** PUBLIC SECTION *****************************
		 **************************************************************************/

	public:

		/****************************** INNER TYPES *******************************/

		/**
		 * Array options
		 */
		enum ArrayOptions
		{
			StandardHeapMemory = 0,		//!< Use standard host memory
			CudaPinnedMemory = 1,		//!< Use pinned memory
			CudaMappedMemory = 2		//!< Use mapped memory
		};

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**
		 * Constructor allocating an array with the given resolution.
		 *
		 * @param pResolution The resolution
		 * @param pMemoryType The memory type
		 */
		Array3D( const uint3& pResolution, uint pMemoryType = 0 );

		/**
		 * Constructor taking storage from external allocation.
		 *
		 * @param pData Pointer on an external allocated data
		 * @param pResolution The resolution
		 */
		Array3D( T* pData, const uint3& pResolution );

		/**
		 * Destructor
		 */
		~Array3D();

		/**
		 * Set manual resolution
		 *
		 * @param pResolution The resolution
		 */
		void manualSetResolution( const uint3& pResolution );

		/**
		 * Set manual data stotage
		 *
		 * @param pData Pointer on data
		 */
		void manualSetDataStorage( T* pData );

		/**
		 * Return the current array size.
		 *
		 * @return The array size
		 */
		__host__ __device__
		uint3 getResolution() const;

		/**
		 * Return the number of elements contained in the array.
		 *
		 * @return The number of elements
		 */
		size_t getNumElements() const;

		/**
		 * Return the amount of memory used by the array.
		 *
		 * @return The used memory size
		 */
		size_t getMemorySize() const;

		/**
		 * Get the stored value at a given position in the array
		 *
		 * @param pPosition Position in the array
		 *
		 * @return The stored value
		 */
		T& get( const uint3& pPosition );

		/**
		 * Get the stored value at a given position in the array
		 *
		 * @param pOffset Offset position in the array
		 *
		 * @return The stored value
		 */
		T& get( size_t offset );

		/**
		 * Get the stored value at a given position in the array
		 *
		 * @param pPosition Position in the array
		 *
		 * @return The stored value
		 */
		T getConst( const uint3& pPosition ) const;

		/**
		 * Get the stored value at a given position in the array
		 *
		 * @param pPosition Position in the array
		 *
		 * @return The stored value
		 */
		T& getSafe( uint3 pPosition ) const;

		/**
		 * Return a pointer to the element located at the given 3D position.
		 * The element cannot be modified.
		 *
		 * @param pPosition The given 3D position
		 *
		 * @return Pointer on the stored value
		 */
		const T* getConstPointer( const uint3& pPosition ) const;

		/**
		 * Return a pointer to the element located at the given 3D position.
		 *
		 * @param pPosition The given 3D position
		 *
		 * @return Pointer on the stored value
		 */
		T* getPointer( const uint3& pPosition ) const;

		/**
		 * Return a pointer to the element located at the given 1D position.
		 *
		 * @param pAddress The 1D address position
		 *
		 * @return Pointer on the stored value
		 */
		T* getPointer( size_t address ) const;

		/**
		 * Return a pointer on the first array element
		 *
		 * @return Pointer on the first array element
		 */
		T* getPointer() const;

		/**
		 * Initialize the array with a given value
		 *
		 * @param pValue The value
		 */
		void fill( int pValue );

		/**
		 * Initialize the array with 0
		 */
		//void zero();

		/**
		 * GPU related stuff.
		 * Get the CUDA pitch pointer.
		 *
		 * @return the CUDA pitch pointer
		 */
		cudaPitchedPtr getCudaPitchedPtr() const;

		/**
		 * Return a pointer on the data mapped into the GPU address space
		 *
		 * @return Pointer on the data
		 */
		T* getGPUMappedPointer() const;

		/**
		 * Returns a device array able to access the system memory data through a mapped pointer.
		 *
		 * @return The associated device array
		 */
		GsLinearMemoryKernel< T > getDeviceArray() const;

		/**************************************************************************
		 **************************** PROTECTED SECTION ***************************
		 **************************************************************************/

	protected:

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**************************************************************************
		 ***************************** PRIVATE SECTION ****************************
		 **************************************************************************/

	private:

		/****************************** INNER TYPES *******************************/

		/**
		 * Array Options
		 */
		enum ArrayPrivateOptions
		{
			SharedData = 0x80000000		//!< The data pointer isn't owned by the array
		};

		/******************************* ATTRIBUTES *******************************/

		/**
		 * Array data
		 */
		T* _data;

		/**
		 * Array resolution
		 */
		uint3 _resolution;

		/**
		 * Array options
		 */
		uint _arrayOptions;

		/******************************** METHODS *********************************/

		/**
		 * Return an index in the array given a 3D position. Out of bounds are check.
		 *
		 * @param pPosition 3D position
		 *
		 * @return An index in the array
		 */
		uint3 getSecureIndex( uint3 pPosition ) const;

	};

} // namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsArray.inl"

/******************************************************************************
 ************************** INSTANTIATION SECTION *****************************
 ******************************************************************************/

namespace GvCore
{

//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< uint >;
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< uint2 >;
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< uint3 >;
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< uint4 >;
//
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< float >;
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< float2 >;
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< float3 >;
//	GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT Array3D< float4 >;
//
//	typedef Array3D< uint > GIGASPACE_EXPORT Array3Dui;
//	typedef Array3D< uint2 > GIGASPACE_EXPORT Array3D2ui;
//	typedef Array3D< uint3 > GIGASPACE_EXPORT Array3D3ui;
//	typedef Array3D< uint4 > GIGASPACE_EXPORT Array3D4ui;
//
//	typedef Array3D< float > GIGASPACE_EXPORT Array3Df;
//	typedef Array3D< float2 > GIGASPACE_EXPORT Array3D2f;
//	typedef Array3D< float3 > GIGASPACE_EXPORT Array3D3f;
//	typedef Array3D< float4 > GIGASPACE_EXPORT Array3D4f;

}

#endif
