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

#ifndef _ARRAY3DKERNELLINEAR_H_
#define _ARRAY3DKERNELLINEAR_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Gigavoxels
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
 * @class GsLinearMemoryKernel
 *
 * @brief The GsLinearMemoryKernel class provides an interface to manipulate
 * arrays on device (i.e. GPU).
 *
 * @ingroup GvCore
 *
 * Device-side class interface to 1D, 2D or 3D array located in device memory.
 * Internally, it does not take ownership of data but references a 1D array.
 * Users can map their multi-dimensions host arrays on this 1D device memory
 * buffer by accessing it with 1D, 2D, or 3D indexes.
 *
 * This is a device-side helper class used by host array. It is associated
 * to the following arrays : 
 * - Array3D
 * - GsLinearMemory
 *
 * @param T type of the array (uint, int2, float3, etc...)
 *
 * @see Array3D, GsLinearMemory
 */
template< typename T >
class GsLinearMemoryKernel
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Inititialize.
	 *
	 * @param data pointer on data
	 * @param res resolution
	 * @param pitch pitch
	 */
	void init( T* pData, const uint3& pRes, size_t pPitch );

	/**
	 * Get the resolution.
	 *
	 * @return the resolution
	 */
	__device__
	__forceinline__ uint3 getResolution() const;

	/**
	 * Get the memory size.
	 *
	 * @return the memory size
	 */
	__device__
	__forceinline__ size_t getMemorySize() const;

	/**
	 * Get the value at a given 1D address.
	 *
	 * @param pAddress a 1D address
	 *
	 * @return the value at the given address
	 */
	__device__
	__forceinline__ /*const*/ T get( uint pAddress ) const;

	/**
	 * Get the value at a given 2D position.
	 *
	 * @param pPosition a 2D position
	 *
	 * @return the value at the given position
	 */
	__device__
	__forceinline__ /*const*/ T get( const uint2& pPosition ) const;

	/**
	 * Get the value at a given 3D position.
	 *
	 * @param pPosition a 3D position
	 *
	 * @return the value at the given position
	 */
	__device__
	__forceinline__ /*const*/ T get( const uint3& pPosition ) const;

	/**
	 * Get the value at a given 1D address in a safe way.
	 * Bounds are checked and address is modified if needed (as a clamp).
	 *
	 * @param pAddress a 1D address
	 *
	 * @return the value at the given address
	 */
	__device__
	__forceinline__ /*const*/ T getSafe( uint pAddress ) const;

	/**
	 * Get the value at a given 3D position in a safe way.
	 * Bounds are checked and position is modified if needed (as a clamp).
	 *
	 * @param pPosition a 3D position
	 *
	 * @return the value at the given position
	 */
	__device__
	__forceinline__ /*const*/ T getSafe( uint3 pPosition ) const;

	/**
	 * Get a pointer on data at a given 1D address.
	 *
	 * @param pAddress a 1D address
	 *
	 * @return the pointer at the given address
	 */
	__device__
	__forceinline__ T* getPointer( uint pAddress = 0 );

	/**
	 * Set the value at a given 1D address in the data array.
	 *
	 * @param pAddress a 1D address
	 * @param pVal a value
	 */
	__device__
	__forceinline__ void set( const uint pAddress, T val );

	/**
	 * Set the value at a given 2D position in the data array.
	 *
	 * @param pPosition a 2D position
	 * @param pVal a value
	 */
	__device__
	__forceinline__ void set( const uint2& pPosition, T val );

	/**
	 * Set the value at a given 3D position in the data array.
	 *
	 * @param pPosition a 3D position
	 * @param pVal a value
	 */
	__device__
	__forceinline__ void set( const uint3& pPosition, T val );

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

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Pointer on array data
	 */
	T* _data;

	/**
	 * Array resolution
	 */
	uint3 _resolution;

	/**
	 * Pitch in bytes
	 */
	size_t _pitch;

	/**
	 * Pitch in elements
	 */
	size_t _pitchxy;

	/******************************** METHODS *********************************/

	/**
	 * Helper function used to get the corresponding index array at a given
	 * 3D position in a safe way.
	 * Position is checked and modified if needed (as a clamp).
	 *
	 * @param pPosition a 3D position
	 *
	 * @return the corresponding index array at the given 3D position
	 */
	__device__
	__forceinline__ uint3 getSecureIndex( uint3 pPosition ) const;

	/**
	 * Helper function used to get the offset in the 1D linear data array
	 * given a 2D position.
	 *
	 * @param pPosition a 2D position
	 *
	 * @return the corresponding offset in the 1D linear data array
	 */
	__device__
	__forceinline__ uint getOffset( const uint2& pPosition ) const;

	/**
	 * Helper function used to get the offset in the 1D linear data array
	 * given a 3D position.
	 *
	 * @param pPosition a 3D position
	 *
	 * @return the corresponding offset in the 1D linear data array
	 */
	__device__
	__forceinline__ uint getOffset( const uint3& pPosition ) const;

};

} // namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsLinearMemoryKernel.inl"

/******************************************************************************
 ************************** INSTANTIATION SECTION *****************************
 ******************************************************************************/

namespace GvCore
{
	//GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT GsLinearMemoryKernel< uint >;
	//GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT GsLinearMemoryKernel< float >;
}

#endif // !_ARRAY3DKERNELLINEAR_H_
