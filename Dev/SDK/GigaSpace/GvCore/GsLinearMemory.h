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

#ifndef _ARRAY3DGPULINEAR_H_
#define _ARRAY3DGPULINEAR_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// OpenGL
#include <GL/glew.h>

// Cuda
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <vector_types.h>

// Cuda SDK
#include <helper_math.h>

// GigaVoxels
#include "GvCore/GsVectorTypesExt.h"
#include "GvCore/GsLinearMemoryKernel.h"
#include "GvCore/GsArray.h"

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
 * @class GsLinearMemory
 *
 * @brief The GsLinearMemory class provides a wrapper to linear memory on device (i.e. GPU)
 *
 * @ingroup GvCore
 *
 * 3D Array located in GPU linear memory manipulation class.
 */
template< typename T >
class GsLinearMemory
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Enumeration used as an option to use Graphics Interoperability
	 */
	enum ArrayOptions
	{
		GLInteroperability = 1
	};

	/**
	 * Defines the type of the associated device kernel array
	 */
	typedef GsLinearMemoryKernel< T > KernelArrayType;

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor.
	 * Create a 3D array of the given resolution using GPU linear memory.
	 *
	 * @param res requested memory size (equivalent to user user 3D array)
	 * @param options option to allocate the memory using Graphics Interoperability
	 */
	GsLinearMemory( const uint3& res, uint options = 0 );

	/**
	 * Constructor.
	 * Create a 3D array of the given resolution.
	 *
	 * @param data ...
	 * @param res ...
	 */
	GsLinearMemory( T* data, const uint3& res );

	/**
	 * Destructor.
	 */
 	~GsLinearMemory();

	/**
	 * ...
	 *
	 * @return ...
	 */
	uint3 getResolution() const;
	/**
	 * ...
	 *
	 * @return ...
	 */
	size_t getNumElements() const;
	/**
	 * ...
	 *
	 * @return ...
	 */
	size_t getMemorySize() const;

	/**
	 * ...
	 *
	 * @param pos ...
	 *
	 * @return ...
	 */
	T* getPointer( const uint3& pos ) const;
	/**
	 * ...
	 *
	 * @param offset ...
	 *
	 * @return ...
	 */
	T* getPointer( size_t offset ) const;
	/**
	 * ...
	 *
	 * @return ...
	 */
	T* getPointer() const;

	/**
	 * ...
	 *
	 * @param pos ...
	 *
	 * @return ...
	 */
	const T* getConstPointer( const uint3& pos ) const;
	/**
	 * ...
	 *
	 * @param address ...
	 *
	 * @return ...
	 */
	const T* getConstPointer( const size_t& address ) const;

	/**
	 * ...
	 *
	 * @param dptr ...
	 */
	void manualSetDataStorage( T* dptr );
	/**
	 * ...
	 *
	 * @return
	 */
	T** getDataStoragePtrAddress();

	/*void zero();*/

	/**
	 *  Fill array with a value
	 *
	 * @param v value
	 */
	void fill( int v );

	/**
	 * Fill array asynchronously with a value
	 *
	 * @param v value
	 */
	void fillAsync( int v );

	///
	///GPU related stuff
	///

	/**
	 * ...
	 *
	 * @return ...
	 */
	KernelArrayType getDeviceArray() const;
	
	/**
	 * ...
	 *
	 * @return ...
	 */
	cudaPitchedPtr getCudaPitchedPtr() const;
	
	/**
	 * ...
	 *
	 * @return ...
	 */
	cudaExtent getCudaExtent() const;

	/**
	 * Map the associated graphics resource
	 */
	void mapResource();

	/**
	 * Unmap the associated graphics resource
	 */
	void unmapResource();

	/**
	 * Get the associated OpenGL handle
	 *
	 * @return the associated OpenGL buffer
	 */
	GLuint getBufferName() const;

	/**
	 * Get the associated CUDA graphics resource
	 *
	 * return the associated CUDA graphics resource
	 */
	cudaGraphicsResource* getGraphicsResource();

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
	 * ...
	 */
	enum ArrayPrivateOptions
	{
		SharedData = 0x80000000
	};

	/******************************* ATTRIBUTES *******************************/

	/**
	 * ...
	 */
	T* _data;

	/**
	 * ...
	 */
	uint3 _resolution;

	/**
	 * ...
	 */
	size_t _pitch;

	/**
	 * ...
	 */
	uint _arrayOptions;

	/**
	 * ...
	 */
	GLuint _bufferObject;

	/**
	 * The associated CUDA graphics resource
	 */
	struct cudaGraphicsResource* _bufferResource;
	
	/******************************** METHODS *********************************/

	/**
	 * ...
	 *
	 * @param position ...
	 *
	 * @return ...
	 */
	uint3 getSecureIndex( uint3 position ) const;

};

} // namespace GvCore

namespace GvCore
{
	/**
	 * ...
	 *
	 * @param dstarray ...
	 * @param h_srcptr ...
	 */
	template< typename T >
	void memcpyArray( GsLinearMemory< T >* dstarray, T* h_srcptr );

	/**
	 * ...
	 *
	 * @param dstarray ...
	 * @param h_srcptr ...
	 * @param numElems ...
	 */
	template< typename T >
	void memcpyArray( GsLinearMemory< T >* dstarray, T* h_srcptr, uint numElems );

	/**
	 * ...
	 *
	 * @param dstarray ...
	 * @param srcarray ...
	 */
	template< typename T >
	void memcpyArray( GsLinearMemory< T >* dstarray, Array3D< T >* srcarray );

	/**
	 * ...
	 *
	 * @param dstarray ...
	 * @param srcarray ...
	 */
	template< typename T >
	void memcpyArray( Array3D< T >* dstarray, GsLinearMemory< T >* srcarray, cudaStream_t pStream = NULL );

	/**
	 * ...
	 *
	 * @param dstarray ...
	 * @param srcarray ...
	 * @param numElems ...
	 */
	template< typename T >
	void memcpyArray( Array3D< T >* dstarray, GsLinearMemory< T >* srcarray, uint numElems );

} // namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsLinearMemory.inl"

/******************************************************************************
 ************************** INSTANTIATION SECTION *****************************
 ******************************************************************************/

namespace GvCore
{
	//GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT GsLinearMemory< uint >;
	//GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT GsLinearMemory< uchar4 >;
	//GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT GsLinearMemory< float >;
	//GIGASPACE_TEMPLATE_EXPORT template class GIGASPACE_EXPORT GsLinearMemory< float4 >;
}

#endif
