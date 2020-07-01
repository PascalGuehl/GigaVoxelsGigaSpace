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

#include "GvUtils/GsCommonGraphicsPass.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsError.h"

// System
#include <cassert>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvUtils;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsCommonGraphicsPass::GsCommonGraphicsPass()
:	_colorTexture( 0 )
,	_colorRenderBuffer( 0 )
,	_depthTexture( 0 )
,	_depthBuffer( 0 )
,	_frameBuffer( 0 )
,	_width( 1 )
,	_height( 1 )
,	_hasImageDownscaling( false )
,	_imageDownscalingWidth( 512 )
,	_imageDownscalingHeight( 512 )
,	_type( 0 )
{
}

/******************************************************************************
 * Desconstructor
 ******************************************************************************/
GsCommonGraphicsPass::~GsCommonGraphicsPass()
{
	// Finalize
	finalize();
}

/******************************************************************************
 * Initiliaze
 *
 * @param pWidth width
 * @param pHeight height
 ******************************************************************************/
void GsCommonGraphicsPass::initialize()
{
	// Initialize buffers
	initializeBuffers();
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
void GsCommonGraphicsPass::finalize()
{
	// Finalize buffers
	finalizeBuffers();
}

/******************************************************************************
 * Reset
 *
 * @param pWidth width
 * @param pHeight height
 ******************************************************************************/
void GsCommonGraphicsPass::reset()
{
	// Reset buffers
	resetBuffers();
}

/******************************************************************************
 * Initiliaze buffers
 *
 * @param pWidth width
 * @param pHeight height
 ******************************************************************************/
void GsCommonGraphicsPass::initializeBuffers()
{
	// Handle image downscaling if activated
	int bufferWidth = _width;
	int bufferHeight = _height;
	if ( _hasImageDownscaling )
	{
		bufferWidth = _imageDownscalingWidth;
		bufferHeight = _imageDownscalingHeight;
	}

	// [ 1 ] - initialize buffer used to read/write color
	if ( _type == 0 )
	{
		// Create a texture that will be used to display the output color buffer data
		// coming from the GigaVoxels volume rendering pipeline.
		// Texture will be filled with data coming from previous color PBO.
		// A full-screen quad will be used.
		glGenTextures( 1, &_colorTexture );
		glBindTexture( GL_TEXTURE_RECTANGLE_EXT, _colorTexture );
		glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexImage2D( GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGBA8, bufferWidth, bufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );
		GV_CHECK_GL_ERROR();
	}
	else
	{	// Create a render buffer that will be used to display the output color buffer data
		// coming from the GigaVoxels volume rendering pipeline.
		// Render buffer will be filled with data coming from previous color PBO.
		// A full-screen quad will be used.
		glGenRenderbuffers( 1, &_colorRenderBuffer );
		glBindRenderbuffer( GL_RENDERBUFFER, _colorRenderBuffer );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, bufferWidth, bufferHeight );
		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		GV_CHECK_GL_ERROR();
	}

	// [ 2 ] - initialize buffers used to read/write depth

	// Create a Pixel Buffer Object that will be used to read depth buffer data
	// coming from the default OpenGL framebuffer.
	// This graphics resource will be mapped in the GigaVoxels CUDA memory.
	glGenBuffers( 1, &_depthBuffer );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, _depthBuffer );
	glBufferData( GL_PIXEL_PACK_BUFFER, bufferWidth * bufferHeight * sizeof( GLuint ), NULL, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
	GV_CHECK_GL_ERROR();

	// Create a texture that will be used to display the output depth buffer data
	// coming from the GigaVoxels volume rendering pipeline.
	// Texture will be filled with data coming from previous depth PBO.
	// A full-screen quad will be used.
	glGenTextures( 1, &_depthTexture );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, _depthTexture );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_RECTANGLE_EXT, 0, GL_DEPTH24_STENCIL8_EXT, bufferWidth, bufferHeight, 0, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, NULL );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );
	GV_CHECK_GL_ERROR();

	// [ 3 ] - initialize framebuffer used to read/write color and depth

	// Create a Frame Buffer Object that will be used to read/write color and depth buffer data
	// coming from the default OpenGL framebuffer.
	// This graphics resource will be mapped in the GigaVoxels CUDA memory.
	glGenFramebuffers( 1, &_frameBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, _frameBuffer );
	if ( _type == 0 )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_EXT, _colorTexture, 0 );
	}
	else
	{
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer );
	}
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE_EXT, _depthTexture, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_RECTANGLE_EXT, _depthTexture, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	GV_CHECK_GL_ERROR();
}

/******************************************************************************
 * FinalizeBuffers
 ******************************************************************************/
void GsCommonGraphicsPass::finalizeBuffers()
{
	// Delete OpenGL depth buffer
	if ( _depthBuffer )
	{
		glDeleteBuffers( 1, &_depthBuffer );
	}

	if ( _depthTexture )
	{
		glDeleteTextures( 1, &_depthTexture );
	}

	//if ( _type == 0 )
	//{
		// Delete OpenGL color and depth textures
		if ( _colorTexture )
		{
			glDeleteTextures( 1, &_colorTexture );
		}
	//}
	//else
	//{
		// Delete OpenGL color render buffer
		if ( _colorRenderBuffer )
		{
			glDeleteRenderbuffers( 1, &_colorRenderBuffer );
		}
	//}

	// Delete OpenGL framebuffer
	if ( _frameBuffer )
	{
		glDeleteFramebuffers( 1, &_frameBuffer );
	}
}

/******************************************************************************
 * Reset buffers
 *
 * @param pWidth width
 * @param pHeight height
 ******************************************************************************/
void GsCommonGraphicsPass::resetBuffers()
{	
	// Finalize buffers
	finalizeBuffers();

	// Initialize buffers
	initializeBuffers();
}

/******************************************************************************
 * Set the buffer width
 *
 * @param the buffer width
 ******************************************************************************/
void GsCommonGraphicsPass::setBufferWidth( int pWidth )
{
	if ( _width != pWidth )
	{
		_width = pWidth;
	}
}

/******************************************************************************
 * Set the buffer height
 *
 * @param the buffer height
 ******************************************************************************/
void GsCommonGraphicsPass::setBufferHeight( int pHeight )
{
	if ( _height != pHeight )
	{
		_height = pHeight;
	}
}

/******************************************************************************
 * Set the buffer size
 *
 * @param the buffer size
 ******************************************************************************/
void GsCommonGraphicsPass::setBufferSize( int pWidth, int pHeight )
{
	if ( ( _width != pWidth ) || ( _height != pHeight ) )
	{
		_width = pWidth;
		_height = pHeight;
	}
}

/******************************************************************************
 * Set the flag telling wheter or not the pipeline uses image downscaling
 *
 * @param pFlag the flag telling wheter or not the pipeline uses image downscaling
 ******************************************************************************/
void GsCommonGraphicsPass::setImageDownscaling( bool pFlag )
{
	_hasImageDownscaling = pFlag;
}

/******************************************************************************
 * Set the image downscaling width
 *
 * @param pValue the image downscaling width 
 ******************************************************************************/
void GsCommonGraphicsPass::setImageDownscalingWidth( int pValue )
{
	if ( _imageDownscalingWidth != pValue )
	{
		_imageDownscalingWidth = pValue;
	}
}

/******************************************************************************
 * Set the image downscaling height
 *
 * @param pValue the image downscaling height 
 ******************************************************************************/
void GsCommonGraphicsPass::setImageDownscalingHeight( int pValue )
{
	if ( _imageDownscalingHeight != pValue )
	{
		_imageDownscalingHeight = pValue;
	}
}

/******************************************************************************
 * Set the image downscaling size
 *
 * @param pWidth the image downscaling size 
 * @param pHeight the image downscaling size 
 ******************************************************************************/
void GsCommonGraphicsPass::setImageDownscalingSize( int pWidth, int pHeight )
{
	if ( ( _imageDownscalingWidth != pWidth ) || ( _imageDownscalingHeight != pHeight ) )
	{
		_imageDownscalingWidth = pWidth;
		_imageDownscalingHeight = pHeight;
	}
}

/******************************************************************************
 * Set the type
 *
 * @param  pValue the type
 ******************************************************************************/
void GsCommonGraphicsPass::setType( unsigned int pValue )
{
	if ( _type != pValue )
	{
		_type = pValue;

		// Reset buffers
		resetBuffers();
	}
}
