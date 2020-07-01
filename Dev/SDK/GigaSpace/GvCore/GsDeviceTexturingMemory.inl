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
#include "GvCore/GsTemplateHelpers.h"
#include "GvCore/GsTypeHelpers.h"
#include "GvCore/GsError.h"

// System
#include <cstring>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * Constructor
 *
 * @param res resolution
 * @param options options
 *
 * @TODO Modify API to handle other texture types than GL_RGBA8 : ex "float" textures for voxelization, etc...
 ******************************************************************************/
template< typename T >
inline GsDeviceTexturingMemory< T >::GsDeviceTexturingMemory( const uint3& res, uint options )
:	_dataArray( NULL )
,	_textureReferenceName()
{
	_arrayOptions = options;

	if ( _arrayOptions & static_cast< uint >( GLInteroperability ) )
	{
		_resolution = res;
		_channelFormatDesc = cudaCreateChannelDesc< T >();

		// Allocate the buffer with OpenGL
		glGenTextures( 1, &_bufferObject );
		
		glBindTexture( GL_TEXTURE_3D, _bufferObject );
		
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		const char* type = typeToString< T >();
		if ( strcmp( type, "float" ) != 0 )
		{
			if ( strcmp( type, "half4" ) == 0 )
			{
				//glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA32F, res.x, res.y, res.z, 0, GL_RGBA, GL_FLOAT, NULL );
				glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA16F, res.x, res.y, res.z, 0, GL_RGBA, GL_FLOAT, NULL );
			}
			else
			{
				glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA8, res.x, res.y, res.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
			}
		}
		else
		{
			// "float" type
			glTexImage3D( GL_TEXTURE_3D, 0, GL_R32F, res.x, res.y, res.z, 0, GL_RED, GL_FLOAT, NULL );
		}

		glBindTexture( GL_TEXTURE_3D, 0 );
		
		GS_CUDA_SAFE_CALL( cudaGraphicsGLRegisterImage( &_bufferResource, _bufferObject, GL_TEXTURE_3D, cudaGraphicsRegisterFlagsSurfaceLoadStore ) );

		// Register it inside Cuda
		//cudaGraphicsGLRegisterBuffer( &_bufferResource, _bufferObject, cudaGraphicsMapFlagsNone );
		
		mapResource();
	}
	else
	{
		allocArray( res, cudaCreateChannelDesc< T >() );
	}
}

/******************************************************************************
 * Constructor
 *
 * @param res resolution
 * @param cfd channel format descriptor
 ******************************************************************************/
template< typename T >
inline GsDeviceTexturingMemory< T >::GsDeviceTexturingMemory( const uint3& res, cudaChannelFormatDesc channelFormatDesc )
{
	allocArray( res, channelFormatDesc );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename T >
inline GsDeviceTexturingMemory< T >::~GsDeviceTexturingMemory()
{
	unbindTexture( _textureReferenceName.c_str() );

	if ( _arrayOptions & (uint)GLInteroperability )
	{
		unmapResource();
		GS_CUDA_SAFE_CALL( cudaGraphicsUnregisterResource( _bufferResource ) );
	}

	GS_CUDA_SAFE_CALL( cudaFreeArray( _dataArray ) );
	_dataArray = NULL;
}

/******************************************************************************
 * Get the resolution
 *
 * @return the resolution
 ******************************************************************************/
template< typename T >
inline uint3 GsDeviceTexturingMemory< T >::getResolution() const
{
	return _resolution;
}

/******************************************************************************
 * Bind texture to array
 *
 * @param symbol device texture symbol
 * @param texRefName texture reference name
 * @param normalizedAccess Type of access
 * @param filterMode Type of filtering mode
 * @param addressMode Type of address mode
 ******************************************************************************/
template< typename T >
inline void GsDeviceTexturingMemory< T >::bindToTextureReference( const void* pTextureReferenceSymbol, const char* texRefName, bool normalizedAccess, cudaTextureFilterMode filterMode, cudaTextureAddressMode addressMode )
{
	std::cout << "bindToTextureReference : " << texRefName << std::endl;

	_textureReferenceName = std::string( texRefName );

	textureReference* texRefPtr = NULL;
	GS_CUDA_SAFE_CALL( cudaGetTextureReference( (const textureReference **)&texRefPtr, pTextureReferenceSymbol ) );

	// Update internal storage
	_textureSymbol = pTextureReferenceSymbol;

	texRefPtr->normalized = normalizedAccess; // Access with normalized texture coordinates
	texRefPtr->filterMode = filterMode;
	texRefPtr->addressMode[ 0 ] = addressMode; // Wrap texture coordinates
	texRefPtr->addressMode[ 1 ] = addressMode;
	texRefPtr->addressMode[ 2 ] = addressMode;

	// Bind array to 3D texture
	GS_CUDA_SAFE_CALL( cudaBindTextureToArray( (const textureReference *)texRefPtr, _dataArray, &_channelFormatDesc ) );
}

/******************************************************************************
 * Unbind texture to array
 *
 * @param texRefName texture reference name
 ******************************************************************************/
template< typename T >
inline void GsDeviceTexturingMemory< T >::unbindTexture( const char* texRefName )
{
	std::cout << "unbindTexture : " << texRefName << std::endl;

	textureReference* texRefPtr = NULL;
	GS_CUDA_SAFE_CALL( cudaGetTextureReference( (const textureReference **)&texRefPtr, _textureSymbol ) );
	
	if ( texRefPtr != NULL )
	{
		GS_CUDA_SAFE_CALL( cudaUnbindTexture( static_cast< const textureReference* >( texRefPtr ) ) );
	}
}

/******************************************************************************
 * Bind surface to array
 *
 * @param surfRefName device surface symbol
 * @param surfRefName surface reference name
 ******************************************************************************/
template< typename T >
inline void GsDeviceTexturingMemory< T >::bindToSurfaceReference( const void* pSurfaceReferenceSymbol, const char* surfRefName )
{
	std::cout << "bindToSurfaceReference : " << surfRefName << std::endl;

	const surfaceReference* surfRefPtr;
	GS_CUDA_SAFE_CALL( cudaGetSurfaceReference( &surfRefPtr, pSurfaceReferenceSymbol ) );
	GS_CUDA_SAFE_CALL( cudaBindSurfaceToArray( surfRefPtr, _dataArray, &_channelFormatDesc ) );
}

/******************************************************************************
 * Get the associated device-side object
 *
 * @return the associated device-side object
 ******************************************************************************/
template< typename T >
inline GsDeviceTexturingMemoryKernel< T > GsDeviceTexturingMemory< T >::getDeviceArray()
{
	GsDeviceTexturingMemoryKernel< T > kat;
	return kat;
}

/******************************************************************************
 * Get the internal device memory array
 *
 * @return the internal device memory array
 ******************************************************************************/
template< typename T >
inline cudaArray* GsDeviceTexturingMemory< T >::getCudaArray()
{
	return _dataArray;
}

/******************************************************************************
 * Get the associated graphics library handle if graphics library interoperability is used
 *
 * @return the associated graphics library handle
 ******************************************************************************/
template< typename T >
inline GLuint GsDeviceTexturingMemory< T >::getBufferName() const
{
	return _bufferObject;
}

/******************************************************************************
 * Map the associated graphics resource if graphics library interoperability is used
 ******************************************************************************/
template< typename T >
inline void GsDeviceTexturingMemory< T >::mapResource()
{
	if ( _arrayOptions & (uint)GLInteroperability )
	{
		GS_CUDA_SAFE_CALL( cudaGraphicsMapResources( 1, &_bufferResource, 0 ) );
		//cudaGraphicsResourceGetMappedPointer((void **)&_dataArray, &bufferSize, _bufferResource);
		GS_CUDA_SAFE_CALL( cudaGraphicsSubResourceGetMappedArray( &_dataArray, _bufferResource, 0, 0 ) );
	}
}

/******************************************************************************
 * Unmap the associated graphics resource if graphics library interoperability is used
 ******************************************************************************/
template< typename T >
inline void GsDeviceTexturingMemory< T >::unmapResource()
{
	if ( _arrayOptions & (uint)GLInteroperability )
	{
		GS_CUDA_SAFE_CALL( cudaGraphicsUnmapResources( 1, &_bufferResource, 0 ) );
		_dataArray = 0;
	}
}

/******************************************************************************
 * Helper method to allocate internal device memory array
 *
 * @param res resolution
 * @param channelFormatDesc channel format descriptor
 ******************************************************************************/
template< typename T >
inline void GsDeviceTexturingMemory< T >::allocArray( const uint3& res, cudaChannelFormatDesc channelFormatDesc )
{
	_resolution = res;
	_channelFormatDesc = channelFormatDesc;

	GS_CUDA_SAFE_CALL( cudaMalloc3DArray( &_dataArray, &channelFormatDesc, make_cudaExtent( _resolution ), cudaArraySurfaceLoadStore ) );
}

/******************************************************************************
 * Get the associated CUDA graphics resource
 *
 * return the associated CUDA graphics resource
 ******************************************************************************/
template< typename T >
inline cudaGraphicsResource* GsDeviceTexturingMemory< T >::getGraphicsResource()
{
	return _bufferResource;
}

} //namespace GvCore

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

/******************************************************************************
 * ...
 *
 * @param dstarray ...
 * @param posDst ...
 * @param resDst ...
 * @param h_srcptr ...
 ******************************************************************************/
template< typename T >
inline void memcpyArray( GsDeviceTexturingMemory< T >* dstarray, uint3 posDst, uint3 resDst, const T* h_srcptr )
{
	cudaMemcpy3DParms copyParams = { 0 };

	copyParams.kind = cudaMemcpyHostToDevice;

	copyParams.srcPtr = make_cudaPitchedPtr( (void*)h_srcptr, resDst.x * sizeof( T ), resDst.x, resDst.y );
	copyParams.srcPos = make_cudaPos( 0, 0, 0 );
	
	copyParams.dstArray = dstarray->getCudaArray();
	copyParams.dstPos	= make_cudaPos( posDst.x, posDst.y, posDst.z );
	copyParams.extent   = make_cudaExtent( resDst );
		
	GS_CUDA_SAFE_CALL( cudaMemcpy3D( &copyParams ) );
}

/******************************************************************************
 * Copy a GsDeviceTexturingMemory device array to a Array3D  host array
 ******************************************************************************/
template< typename T >
inline void memcpyArray( Array3D< T >* pDestinationArray, GsDeviceTexturingMemory< T >* pSourceArray )
{
	cudaMemcpy3DParms copyParams = { 0 };

	copyParams.kind = cudaMemcpyDeviceToHost;

	copyParams.dstPtr = pDestinationArray->getCudaPitchedPtr();
	copyParams.srcPos = make_cudaPos( 0, 0, 0 );
	
	copyParams.srcArray = pSourceArray->getCudaArray();
	copyParams.dstPos	= make_cudaPos( 0, 0, 0 );

	copyParams.extent   = make_cudaExtent( pDestinationArray->getResolution() );
		
	GS_CUDA_SAFE_CALL( cudaMemcpy3D( &copyParams ) );
}

// Explicit instantiations
//TEMPLATE_INSTANCIATE_CLASS_TYPES(GsDeviceTexturingMemory);

// memcpyArray : Explicit instanciations
//template void memcpyArray<uchar4>(GsDeviceTexturingMemory<uchar4> *dstarray, uint3 posDst, uint3 resDst, const uchar4* h_srcptr);

} //namespace GvCore
