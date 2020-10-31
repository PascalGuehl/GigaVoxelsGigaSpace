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

#ifndef _GV_GRAPHICS_RESOURCE_H_
#define _GV_GRAPHICS_RESOURCE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// OpenGL
#include <GL/glew.h>

// Cuda
#include <driver_types.h>

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

namespace GvRendering
{

/** 
 * @class GsGraphicsResource
 *
 * @brief The GsGraphicsResource class provides interface to handle
 * graphics resources from graphics libraries like OpenGL (or DirectX),
 * in the CUDA memory context.
 *
 * Some resources from OpenGL may be mapped into the address space of CUDA,
 * either to enable CUDA to read data written by OpenGL, or to enable CUDA
 * to write data for consumption by OpenGL.
 */
class GIGASPACE_EXPORT GsGraphicsResource
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Enumeration of the graphics IO slots
	 * connected to the GigaVoxles engine
	 * (i.e. color and depth inputs/outputs)
	 */
	enum Access
	{
		eNone,
		eRead,
		eWrite,
		eReadWrite,
	};

	/**
	 * Enumeration of the graphics IO types
	 * connected to the GigaVoxles engine
	 * (i.e. color and depth inputs/outputs)
	 */
	enum Type
	{
		eUndefinedType = -1,
		eBuffer,
		eImage,
		eNbTypes
	};

	/**
	 * Enumeration of the graphics IO types
	 * connected to the GigaVoxles engine
	 * (i.e. color and depth inputs/outputs)
	 */
	enum MappedAddressType
	{
		eUndefinedMappedAddressType,
		//eNone,
		ePointer,
		eTexture,
		eSurface,
		eNbMappedAddressTypes
	};

	/**
	 * Memory type
	 */
	enum MemoryType
	{
		eUndefinedMemoryType = -1,
		eDevicePointer,
		eCudaArray,
		eNbMemoryTypes
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsGraphicsResource();

	/**
	 * Destructor
	 */
	 virtual ~GsGraphicsResource();

	/**
	 * Initiliaze
	 */
	void initialize();

	/**
	 * Finalize
	 */
	void finalize();

	/**
	 * Reset
	 */
	void reset();

	/**
	 * Registers an OpenGL buffer object.
	  */
	cudaError_t registerBuffer( GLuint pBuffer, unsigned int pFlags );

	/**
	 * Register an OpenGL texture or renderbuffer object.
	 */
	cudaError_t registerImage( GLuint pImage, GLenum pTarget, unsigned int pFlags );

	/**
	 * Unregisters a graphics resource for access by CUDA.
	 */
	cudaError_t unregister();

	/**
	 * Map graphics resources for access by CUDA.
	 */
	cudaError_t map();

	/**
	 * Unmap graphics resources for access by CUDA.
	 */
	cudaError_t unmap();

	/**
	 * Get an device pointer through which to access a mapped graphics resource.
	 * Get an array through which to access a subresource of a mapped graphics resource.
	 */
	void* getMappedAddress();

	/**
	 * ...
	 */
	inline MemoryType getMemoryType() const;

	/**
	 * ...
	 */
	inline MappedAddressType getMappedAddressType() const;

	/**
	 * ...
	 */
	MappedAddressType _mappedAddressType;

	/**
	 * Tell wheter or not the associated CUDA graphics resource has already been registered
	 *
	 * @return a flag telling wheter or not the associated CUDA graphics resource has already been registered
	 */
	bool isRegistered() const;

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * OpenGL buffer that will be registered in the GigaVoxels engine
	 */
	GLuint _graphicsBuffer;

	/**
	 * CUDA graphics resource associated to registered OpenGL buffer
	 */
	cudaGraphicsResource* _graphicsResource;

	/**
	 * CUDA graphics resource types associated to OpenGL buffers (i.e buffer or image)
	 */
	Type _type;

	/**
	 * ...
	 */
	Access _access;

	///**
	// * ...
	// */
	//MappedAddressType _mappedAddressType;

	/**
	 * CUDA graphics resource mapped address associated to registered OpenGL buffers
	 */
	void* _mappedAddress;

	/**
	 * Offset (in texel unit) to apply during texture fetches
	 */
	size_t _textureOffset;

	/**
	 * Indentifier
	 */
	unsigned int _id;

	/**
	 * ...
	 */
	bool _isRegistered;

	/**
	 * ...
	 */
	bool _isMapped;

	/**
	 * ...
	 */
	unsigned int _flags;

	/**
	 * Memory type
	 */
	MemoryType _memoryType;

	/******************************** METHODS *********************************/

	/**
	 * ...
	 */
	inline cudaError_t getMappedPointer( void** pDevicePointer, size_t* pSize );

	/**
	 * ...
	 */
	inline cudaError_t getMappedArray( cudaArray** pArray, unsigned int pArrayIndex, unsigned int pMipLevel );

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GvRendering

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsGraphicsResource.inl"

#endif
