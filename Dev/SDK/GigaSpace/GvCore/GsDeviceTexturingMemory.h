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

#ifndef _GV_ARRAY_3D_GPU_TEX_H_
#define _GV_ARRAY_3D_GPU_TEX_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// OpenGL
#include <GL/glew.h>

// Cuda
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

// Cuda SDK
#include <helper_math.h>

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvCore/GsDeviceTexturingMemoryKernel.h"

// STL
#include <string>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaVoxels
namespace GvCore
{
	//template< typename T > class GsDeviceTexturingMemoryKernel;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @class GsDeviceTexturingMemory
 *
 * @brief The GsDeviceTexturingMemory class provides features to manipulate device memory array.
 *
 * @ingroup GvCore
 *
 * 3D Array manipulation class located in device texture memory. It is not the same as linear memory array.
 * Textures and surfaces should be bound to array in order to read/write data.
 */
template< typename T >
class GsDeviceTexturingMemory
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Defines the type of the associated kernel array
	 */
	typedef GsDeviceTexturingMemoryKernel< T > KernelArrayType;

	/**
	 * Enumeration used to define array in normal or graphics interoperability mode
	 */
	enum ArrayOptions
	{
		GLInteroperability = 1
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param res resolution
	 * @param options options
	 */
	GsDeviceTexturingMemory( const uint3& res, uint options = 0 );

	/**
	 * Constructor
	 *
	 * @param res resolution
	 * @param cfd channel format descriptor
	 */
	GsDeviceTexturingMemory( const uint3& res, cudaChannelFormatDesc cfd );
	
	/**
	 * Destructor
	 */
	virtual ~GsDeviceTexturingMemory();

	/**
	 * Get the resolution
	 *
	 * @return the resolution
	 */
	uint3 getResolution() const;

	/**
	 * Bind texture to array
	 *
	 * @param symbol device texture symbol
	 * @param texRefName texture reference name
	 * @param normalizedAccess Type of access
	 * @param filterMode Type of filtering mode
	 * @param addressMode Type of address mode
	 */
	void bindToTextureReference( const void* symbol, const char* texRefName, bool normalizedAccess, cudaTextureFilterMode filterMode, cudaTextureAddressMode addressMode );

	/**
	 * Unbind texture to array
	 *
	 * @param texRefName texture reference name
	 */
	void unbindTexture( const char* texRefName );

	/**
	 * Bind surface to array
	 *
	 * @param surfRefName device surface symbol
	 * @param surfRefName surface reference name
	 */
	void bindToSurfaceReference( const void* pSurfaceReferenceSymbol, const char* surfRefName );

	/**
	 * Get the associated device-side object
	 *
	 * @return the associated device-side object
	 */
	GsDeviceTexturingMemoryKernel< T > getDeviceArray();

	/**
	 * Get the internal device memory array
	 *
	 * @return the internal device memory array
	 */
	cudaArray* getCudaArray();

	/**
	 * Get the associated graphics library handle if graphics library interoperability is used
	 *
	 * @return the associated graphics library handle
	 */
	GLuint getBufferName() const;

	/**
	 * Map the associated graphics resource if graphics library interoperability is used
	 */
	void mapResource();

	/**
	 * Unmap the associated graphics resource if graphics library interoperability is used
	 */
	void unmapResource();

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

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Flags used to define array in normal or graphics interoperability mode
	 */
	uint _arrayOptions;

	/**
	 * Array resolution (i.e. dimension)
	 */
	uint3 _resolution;

	/**
	 * ...
	 */
	uint2 _atlasRes;

	/**
	 * Underlying device memory array
	 */
	cudaArray* _dataArray;

	/**
	 * Underlying channel format descriptor
	 */
	cudaChannelFormatDesc _channelFormatDesc;

	/**
	 * Associated graphics library handle if graphics library interoperability is used
	 */
	GLuint _bufferObject;

	/**
	 * Associated graphics resource if graphics library interoperability is used
	 */
	struct cudaGraphicsResource* _bufferResource;

	/**
	 * Bounded texture reference name (if any)
	 */
	std::string _textureReferenceName;

	/**
	 * Bounded device texture symbol (if any)
	 */
	const void* _textureSymbol;

	/******************************** METHODS *********************************/

	/**
	 * Helper method to allocate internal device memory array
	 *
	 * @param res resolution
	 * @param channelFormatDesc channel format descriptor
	 */
	void allocArray( const uint3& res, cudaChannelFormatDesc channelFormatDesc );

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsDeviceTexturingMemory( const GsDeviceTexturingMemory& );

	/**
	 * Copy operator forbidden.
	 */
	GsDeviceTexturingMemory& operator=( const GsDeviceTexturingMemory& );
	
};

} //namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{
	/**
	 * ...
	 *
	 * @param dstarray ...
	 * @param posDst ...
	 * @param resDst ...
	 * @param h_srcptr ...
	 */
	template< typename T >
	inline void memcpyArray( GsDeviceTexturingMemory< T >* dstarray, uint3 posDst, uint3 resDst, const T* h_srcptr );

} //namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsDeviceTexturingMemory.inl"

#endif // !_GV_ARRAY_3D_GPU_TEX_H_
