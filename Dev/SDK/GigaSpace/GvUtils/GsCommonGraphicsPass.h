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

#ifndef _GV_COMMON_GRAPHICS_PASS_H_
#define _GV_COMMON_GRAPHICS_PASS_H_

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

namespace GvUtils
{

/** 
 * @class GsCommonGraphicsPass
 *
 * @brief The GsCommonGraphicsPass class provides interface to
 *
 * Some resources from OpenGL may be mapped into the address space of CUDA,
 * either to enable CUDA to read data written by OpenGL, or to enable CUDA
 * to write data for consumption by OpenGL.
 */
class GIGASPACE_EXPORT GsCommonGraphicsPass
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsCommonGraphicsPass();

	/**
	 * Destructor
	 */
	 virtual ~GsCommonGraphicsPass();

	/**
	 * Initiliaze
	 */
	virtual void initialize();

	/**
	 * Finalize
	 */
	virtual void finalize();

	/**
	 * Reset
	 *
	 * @param pWidth width
	 * @param pHeight height
	 */
	virtual void reset();

	/**
	 * Get the color texture
	 *
	 * @return the color texture
	 */
	GLuint getColorTexture() const;

	/**
	 * Get the color render buffer
	 *
	 * @return the color render buffer
	 */
	GLuint getColorRenderBuffer() const;

	/**
	 * Get the depth buffer
	 *
	 * @return the depth buffer
	 */
	GLuint getDepthBuffer() const;

	/**
	 * Get the depth texture
	 *
	 * @return the depth texture
	 */
	GLuint getDepthTexture() const;

	/**
	 * Get the framebuffer object
	 *
	 * @return the framebuffer object
	 */
	GLuint getFrameBuffer() const;

	/**
	 * Get the buffer width
	 *
	 * @return the buffer width
	 */
	int getBufferWidth() const;

	/**
	 * Set the buffer width
	 *
	 * @param the buffer width
	 */
	void setBufferWidth( int pWidth );

	/**
	 * Get the height
	 *
	 * @return the height
	 */
	int getBufferHeight() const;

	/**
	 * Set the buffer height
	 *
	 * @param the buffer height
	 */
	void setBufferHeight( int pHeight );

	/**
	 * Set the buffer size
	 *
	 * @param the buffer size
	 */
	void setBufferSize( int pWidth, int pHeight );

	/**
	 * Tell wheter or not the pipeline uses image downscaling.
	 *
	 * @return the flag telling wheter or not the pipeline uses image downscaling
	 */
	bool hasImageDownscaling() const;

	/**
	 * Set the flag telling wheter or not the pipeline uses image downscaling
	 *
	 * @param pFlag the flag telling wheter or not the pipeline uses image downscaling
	 */
	void setImageDownscaling( bool pFlag );

	/**
	 * Get the image downscaling width
	 *
	 * @return the image downscaling width 
	 */
	int getImageDownscalingWidth() const;

	/**
	 * Get the image downscaling height
	 *
	 * @return the image downscaling width
	 */
	int getImageDownscalingHeight() const;

	/**
	 * Set the image downscaling width
	 *
	 * @param pValue the image downscaling width 
	 */
	void setImageDownscalingWidth( int pValue );

	/**
	 * Set the image downscaling height
	 *
	 * @param pValue the image downscaling height 
	 */
	void setImageDownscalingHeight( int pValue );

	/**
	 * Set the image downscaling size
	 *
	 * @param pWidth the image downscaling size 
	 * @param pHeight the image downscaling size 
	 */
	void setImageDownscalingSize( int pWidth, int pHeight );

	/**
	 * Get the type
	 *
	 * @return the type
	 */
	unsigned int getType() const;

	/**
	 * Set the type
	 *
	 * @param  pValue the type
	 */
	void setType( unsigned int pValue );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Color texture
	 */
	GLuint _colorTexture;

	/**
	 * Color render buffer
	 */
	GLuint _colorRenderBuffer;

	/**
	 * Depth texture
	 */
	GLuint _depthTexture;

	/**
	 * Depth buffer
	 */
	GLuint _depthBuffer;

	/**
	 * Frame buffer
	 */
	GLuint _frameBuffer;

	/**
	 * Internal graphics buffer's width
	 */
	int _width;

	/**
	 * Internal graphics buffer's height
	 */
	int _height;

	/**
	 * Flag telling wheter or not the pipeline uses image downscaling
	 */
	bool _hasImageDownscaling;

	/**
	 * Image downscaling width
	 */
	int _imageDownscalingWidth;

	/**
	 * Image downscaling height
	 */
	int _imageDownscalingHeight;

	/**
	 * Type
	 */
	unsigned int _type;

	/******************************** METHODS *********************************/

	/**
	 * Initiliaze buffers
	 *
	 * @param pWidth width
	 * @param pHeight height
	 */
	virtual void initializeBuffers();

	/**
	 * Finalize buffers
	 */
	virtual void finalizeBuffers();

	/**
	 * Reset buffers
	 *
	 * @param pWidth width
	 * @param pHeight height
	 */
	virtual void resetBuffers();

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
	GsCommonGraphicsPass( const GsCommonGraphicsPass& );

	/**
	 * Copy operator forbidden.
	 */
	GsCommonGraphicsPass& operator=( const GsCommonGraphicsPass& );

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsCommonGraphicsPass.inl"

#endif
