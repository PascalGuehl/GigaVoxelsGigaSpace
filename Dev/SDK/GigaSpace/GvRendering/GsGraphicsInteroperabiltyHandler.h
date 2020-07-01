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

#ifndef _GV_GRAPHICS_INTEROPERABILTY_HANDLER_H_
#define _GV_GRAPHICS_INTEROPERABILTY_HANDLER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// OpenGL
#include <GL/glew.h>

// Cuda
#include <driver_types.h>

// STL
#include <vector>
#include <utility>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

namespace GvRendering
{
	class GsGraphicsResource;
	struct GsRendererContext;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvRendering
{

/** 
 * @class GsGraphicsInteroperabiltyHandler
 *
 * @brief The GsGraphicsInteroperabiltyHandler class provides interface to
 *
 * Some resources from OpenGL may be mapped into the address space of CUDA,
 * either to enable CUDA to read data written by OpenGL, or to enable CUDA
 * to write data for consumption by OpenGL.
 */
class GIGASPACE_EXPORT GsGraphicsInteroperabiltyHandler
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
	enum GraphicsResourceSlot
	{
		eColorReadSlot,
		eColorWriteSlot,
		eColorReadWriteSlot,
		eDepthReadSlot,
		eDepthWriteSlot,
		eDepthReadWriteSlot,
		eNbGraphicsResourceSlots
	};

	/**
	 * Enumeration of the graphics IO slots
	 * connected to the GigaVoxles engine
	 * (i.e. color and depth inputs/outputs)
	 */
	enum GraphicsResourceDeviceSlot
	{
		eUndefinedGraphicsResourceDeviceSlot = -1,
		eColorInput,
		eColorOutput,
		eDepthInput,
		eDepthOutput,
		eNbGraphicsResourceDeviceSlots
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsGraphicsInteroperabiltyHandler();

	/**
	 * Destructor
	 */
	 virtual ~GsGraphicsInteroperabiltyHandler();

	/**
	 * Initiliaze
	 *
	 * @param pWidth width
	 * @param pHeight height
	 */
	//void initialize( int pWidth, int pHeight );
	 void initialize();

	/**
	 * Finalize
	 */
	void finalize();

	/**
	 * Reset
	 */
	bool reset();

	/**
	 * Attach an OpenGL buffer object (i.e. a PBO, a VBO, etc...) to an internal graphics resource 
	 * that will be mapped to a color or depth slot used during rendering.
	 *
	 * @param pGraphicsResourceSlot the associated internal graphics resource (color or depth) and its type of access (read, write or read/write)
	 * @param pBuffer the OpenGL buffer
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool connect( GraphicsResourceSlot pSlot, GLuint pBuffer );

	/**
	 * Attach an OpenGL texture or renderbuffer object to an internal graphics resource 
	 * that will be mapped to a color or depth slot used during rendering.
	 *
	 * @param pGraphicsResourceSlot the associated internal graphics resource (color or depth) and its type of access (read, write or read/write)
	 * @param pImage the OpenGL texture or renderbuffer object
	 * @param pTarget the target of the OpenGL texture or renderbuffer object
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool connect( GraphicsResourceSlot pSlot, GLuint pImage, GLenum pTarget );

	/**
	 * Dettach an OpenGL buffer object (i.e. a PBO, a VBO, etc...), texture or renderbuffer object
	 * to its associated internal graphics resource mapped to a color or depth slot used during rendering.
	 *
	 * @param pGraphicsResourceSlot the internal graphics resource slot (color or depth)
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool disconnect( GraphicsResourceSlot pSlot );
		
	/**
	 * Map graphics resources into CUDA memory in order to be used during rendering.
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool mapResources();

	/**
	 * Unmap graphics resources from CUDA memory in order to be used by OpenGL.
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	bool unmapResources();

	/** For each graphics resources, store the associated mapped address
	 *
	 * @param pRendererContext the renderer context in which graphics resources mapped addresses will be stored
	 *
	 * @return a flag telling whether or not it succeeds
	 */
	bool setRendererContextInfo( GsRendererContext& pRendererContext );

	/**
	 * ...
	 */
	inline const std::vector< std::pair< GraphicsResourceSlot, GsGraphicsResource* > >& getGraphicsResources() const;
	
	/**
	 * ...
	 */
	inline std::vector< std::pair< GraphicsResourceSlot, GsGraphicsResource* > >& editGraphicsResources();

	//bool bindTo( const struct surfaceReference* surfref );
		
	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	//GsGraphicsResource* _graphicsResources[ GraphicsResourceDeviceSlot ];
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< std::pair< GraphicsResourceSlot, GsGraphicsResource* > > _graphicsResources;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	///**
	// * Width
	// */
	//int _width;

	///**
	// * Height
	// */
	//int _height;

	///**
	// * ...
	// */
	//bool _hasColorInput;
	//bool _hasColorOutput;
	//bool _hasDepthInput;
	//bool _hasDepthOutput;

	/******************************** METHODS *********************************/

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

#include "GsGraphicsInteroperabiltyHandler.inl"

#endif
