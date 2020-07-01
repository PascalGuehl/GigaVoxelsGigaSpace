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
#include "GvCore/GsError.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{
	
/******************************************************************************
 * Constructor.
 * Create a 3D array of the given resolution using GPU linear memory.
 *
 * @param res ...
 * @param options ...
 ******************************************************************************/
template< typename T >
inline GsLinearMemory< T >::GsLinearMemory( const uint3& res, uint options )
:	_data( NULL )
{
	_resolution = res;
	_arrayOptions = options & ~SharedData;

	if ( _arrayOptions & (uint)GLInteroperability )
	{
		// Allocate the buffer with OpenGL
		glGenBuffers( 1, &_bufferObject );
		glBindBuffer( GL_TEXTURE_BUFFER, _bufferObject );
		glBufferData( GL_TEXTURE_BUFFER, getMemorySize(), NULL, GL_DYNAMIC_DRAW );
		//glMakeBufferResidentNV( GL_TEXTURE_BUFFER, GL_READ_WRITE );
		//glGetBufferParameterui64vNV( GL_TEXTURE_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &_bufferAddress );
		glBindBuffer( GL_TEXTURE_BUFFER, 0 );
		GV_CHECK_GL_ERROR();

		// Register it inside Cuda
		GS_CUDA_SAFE_CALL( cudaGraphicsGLRegisterBuffer( &_bufferResource, _bufferObject, cudaGraphicsRegisterFlagsNone ) );
		mapResource();
	}
	else
	{
		GS_CUDA_SAFE_CALL( cudaMalloc( (void**)&_data, getMemorySize() ) );
	}

	_pitch = _resolution.x * sizeof( T );
}

/******************************************************************************
 * Constructor.
 * Create a 3D array of the given resolution.
 *
 * @param data ...
 * @param res ...
 ******************************************************************************/
template< typename T >
inline GsLinearMemory< T >::GsLinearMemory( T* data, const uint3& res )
:	_data( NULL )
{
	_resolution = res;

	_data = data;
	_pitch = _resolution.x * sizeof( T );

	_arrayOptions = (uint)SharedData;
}

/******************************************************************************
 * Destructor.
 ******************************************************************************/
template< typename T >
inline GsLinearMemory< T >::~GsLinearMemory()
{
	if ( _arrayOptions & (uint)GLInteroperability )
	{
		unmapResource();
		GS_CUDA_SAFE_CALL( cudaGraphicsUnregisterResource( _bufferResource ) );

		// TO DO : need a glDeleteBuffers() ?
	}

	if ( _data && !( _arrayOptions & (uint)SharedData ) )
	{
		GS_CUDA_SAFE_CALL( cudaFree( _data ) );
		_data = NULL;
	}
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline uint3 GsLinearMemory< T >::getResolution() const
{
	return _resolution;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline size_t GsLinearMemory< T >::getNumElements() const
{
	return _resolution.x * _resolution.y * _resolution.z;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline size_t GsLinearMemory< T >::getMemorySize() const
{
	return getNumElements() * sizeof( T );
}

/******************************************************************************
 * ...
 *
 * @param pos ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline T* GsLinearMemory< T >::getPointer( const uint3& pos ) const
{
	return &_data[ pos.x + pos.y * _resolution.x + pos.z * _resolution.x * _resolution.y ];
}

/******************************************************************************
 * ...
 *
 * @param offset ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline T* GsLinearMemory< T >::getPointer( size_t offset ) const
{
	return &_data[ offset ];
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline T* GsLinearMemory< T >::getPointer() const
{
	return _data;
}

/******************************************************************************
 * ...
 *
 * @param pos ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline const T* GsLinearMemory< T >::getConstPointer( const uint3& pos ) const
{
	return &_data[ pos.x + pos.y * _resolution.x + pos.z * _resolution.x * _resolution.y ];
}

/******************************************************************************
 * ...
 *
 * @param address ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline const T* GsLinearMemory< T >::getConstPointer( const size_t& address ) const
{
	return &_data[ address ];
}

/******************************************************************************
 * ...
 *
 * @param dptr ...
 ******************************************************************************/
template< typename T >
inline void GsLinearMemory< T >::manualSetDataStorage( T* dptr )
{
	_data = dptr;
}

/******************************************************************************
 * ...
 *
 * @return
 ******************************************************************************/
template< typename T >
inline T** GsLinearMemory< T >::getDataStoragePtrAddress()
{
	return &_data;
}

/******************************************************************************
 * ...
 ******************************************************************************/
/*template< typename T >
inline void GsLinearMemory< T >::zero()
{
this->fill(0);
}*/

/******************************************************************************
 *  Fill array with a value
 *
 * @param v value
 ******************************************************************************/
template< typename T >
inline void GsLinearMemory< T >::fill( int v )
{
	//assert(0);	//This should not be used
	//std::cout<<"Warning: GsLinearMemory< T >::fill is VERY slow \n";

	GS_CUDA_SAFE_CALL( cudaMemset( _data, v, getMemorySize() ) );
}

/******************************************************************************
 *  Fill array asynchrounously with a value
 *
 * @param v value
 ******************************************************************************/
template< typename T >
inline void GsLinearMemory< T >::fillAsync( int v )
{
	//assert(0);	//This should not be used
	//std::cout<<"Warning: GsLinearMemory< T >::fill is VERY slow \n";

	GS_CUDA_SAFE_CALL( cudaMemsetAsync( _data, v, getMemorySize() ) );
}

///
///GPU related stuff
///

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline GsLinearMemoryKernel< T > GsLinearMemory< T >::getDeviceArray() const
{
	GsLinearMemoryKernel< T > kal;
	kal.init( _data, make_uint3( _resolution ), _pitch );

	return kal;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline cudaPitchedPtr GsLinearMemory< T >::getCudaPitchedPtr() const
{
	return make_cudaPitchedPtr( (void*)_data, _pitch, (size_t) _resolution.x, (size_t) _resolution.y );
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline cudaExtent GsLinearMemory< T >::getCudaExtent() const
{
	return make_cudaExtent( _pitch, (size_t)_resolution.y, (size_t)_resolution.z );
}

/******************************************************************************
 * ...
 *
 * @param position ...
 *
 * @return ...
 ******************************************************************************/
template< typename T >
inline uint3 GsLinearMemory< T >::getSecureIndex( uint3 position ) const
{
	if
		( position.x >= _resolution.x )
	{
		position.x = _resolution.x - 1;
	}

	if
		( position.y >= _resolution.y )
	{
		position.y = _resolution.y - 1;
	}

	if
		( position.z >= _resolution.z )
	{
		position.z = _resolution.z - 1;
	}

	return position;
}

/******************************************************************************
 * Map the associated graphics resource
 ******************************************************************************/
template< typename T >
inline void GsLinearMemory< T >::mapResource()
{
	if ( _arrayOptions & (uint)GLInteroperability )
	{
		size_t bufferSize;

		GS_CUDA_SAFE_CALL( cudaGraphicsMapResources( 1, &_bufferResource, 0 ) );
		GS_CUDA_SAFE_CALL( cudaGraphicsResourceGetMappedPointer( (void **)&_data, &bufferSize, _bufferResource ) );
	}
}

/******************************************************************************
* Unmap the associated graphics resource
 ******************************************************************************/
template< typename T >
inline void GsLinearMemory< T >::unmapResource()
{
	if ( _arrayOptions & (uint)GLInteroperability )
	{
		GS_CUDA_SAFE_CALL( cudaGraphicsUnmapResources( 1, &_bufferResource, 0 ) );
		_data = 0;
	}
}

/******************************************************************************
 * Get the associated OpenGL handle
 *
 * @return the associated OpenGL buffer
 ******************************************************************************/
template< typename T >
inline GLuint GsLinearMemory< T >::getBufferName() const
{
	return _bufferObject;
}

/******************************************************************************
 * Get the associated CUDA graphics resource
 *
 * return the associated CUDA graphics resource
 ******************************************************************************/
template< typename T >
inline cudaGraphicsResource* GsLinearMemory< T >::getGraphicsResource()
{
	return _bufferResource;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
/*template< typename T >
inline GLuint64EXT GsLinearMemory< T >::getBufferAddress() const
{
return _bufferAddress;
}*/

/******************************************************************************
 * ...
 *
 * @param dstarray ...
 * @param h_srcptr ...
 ******************************************************************************/
template< typename T >
inline void memcpyArray( GsLinearMemory< T >* dstarray, T* h_srcptr )
{
	GS_CUDA_SAFE_CALL( cudaMemcpy( dstarray->getPointer(), h_srcptr, dstarray->getMemorySize(), cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * ...
 *
 * @param dstarray ...
 * @param h_srcptr ...
 * @param numElems ...
 ******************************************************************************/
template< typename T >
inline void memcpyArray( GsLinearMemory< T >* dstarray, T* h_srcptr, uint numElems )
{
	GS_CUDA_SAFE_CALL( cudaMemcpy( dstarray->getPointer(), h_srcptr, numElems * sizeof( T ), cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * ...
 *
 * @param dstarray ...
 * @param srcarray ...
 ******************************************************************************/
template< typename T >
inline void memcpyArray( GsLinearMemory< T >* dstarray, Array3D< T >* srcarray )
{
	memcpyArray( dstarray, srcarray->getPointer() );
}

/******************************************************************************
 * ...
 *
 * @param dstarray ...
 * @param srcarray ...
 ******************************************************************************/
template< typename T >
inline void memcpyArray( Array3D< T >* dstarray, GsLinearMemory< T >* srcarray, cudaStream_t pStream )
{
	cudaMemcpy3DParms copyParams = { 0 };

	copyParams.kind = cudaMemcpyDeviceToHost;

	copyParams.srcPtr = srcarray->getCudaPitchedPtr();
	copyParams.dstPtr = dstarray->getCudaPitchedPtr();

	copyParams.extent = make_cudaExtent( srcarray->getResolution().x * sizeof( T ), srcarray->getResolution().y, srcarray->getResolution().z );

	GS_CUDA_SAFE_CALL( cudaMemcpy3D( &copyParams ) );
	//GS_CUDA_SAFE_CALL( cudaMemcpy3DAsync( &copyParams, pStream ) );
}

/******************************************************************************
 * ...
 *
 * @param dstarray ...
 * @param srcarray ...
 * @param numElems ...
 *
 * TODO : numElems is unused...
 ******************************************************************************/
template< typename T >
void memcpyArray( Array3D< T >* dstarray, GsLinearMemory< T >* srcarray, uint numElems )
{
	cudaMemcpy3DParms copyParams = { 0 };

	copyParams.kind = cudaMemcpyDeviceToHost;

	copyParams.srcPtr = srcarray->getCudaPitchedPtr();
	copyParams.dstPtr = dstarray->getCudaPitchedPtr();

	copyParams.extent = make_cudaExtent( srcarray->getResolution().x * sizeof( T ), srcarray->getResolution().y, srcarray->getResolution().z );

	GS_CUDA_SAFE_CALL( cudaMemcpy3D( &copyParams ) );
}

/******************************************************************************
 *
 ******************************************************************************/
/*template< typename T >
inline void memcpyArray(GsDeviceTexturingMemory<T> *dstarray, const T* h_srcptr)
{
cudaMemcpy3DParms copyParams = { 0 };

copyParams.kind = cudaMemcpyDeviceToHost;

copyParams.srcPtr
= make_cudaPitchedPtr(h_srcptr, dstarray->getResolution().x
* sizeof(T), dstarray->getResolution().x, dstarray->getResolution().y);
copyParams.dstPtr = dstarray->getCudaPitchedPtr();

copyParams.extent = make_cudaExtent(dstarray->getResolution());

GS_CUDA_SAFE_CALL(cudaMemcpy3D(&copyParams));

}*/

/******************************************************************************
 *
 ******************************************************************************/
//template< typename T >
//inline void memcpyArray(GsDeviceTexturingMemory<T> *dstarray, const T* h_srcptr)
//{
//	cudaMemcpy3DParms copyParams = { 0 };
//
//	copyParams.kind = cudaMemcpyHostToDevice;
//
//	copyParams.srcPtr = make_cudaPitchedPtr((void*)h_srcptr, dstarray->getResolution().x* sizeof(T),	dstarray->getResolution().x, dstarray->getResolution().y);
//	copyParams.srcPos	= make_cudaPos(0, 0, 0);
//
//	copyParams.dstArray = dstarray->getCudaArray();
//	copyParams.dstPos	= make_cudaPos(0, 0, 0);
//	copyParams.extent   = make_cudaExtent(dstarray->getResolution());
//
//
//	GS_CUDA_SAFE_CALL(cudaMemcpy3D(&copyParams));
//}

} // namespace GvCore
