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

#include "GvUtils/GsRayMap.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvRendering/GsGraphicsResource.h"
#include "GvCore/GsError.h"
#include "GvUtils/GsShaderManager.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GsGraphics/GsShaderProgram.h"

// CUDA
#include <driver_types.h>

// System
#include <cassert>
#include <cstddef>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvUtils;
using namespace GsGraphics;
using namespace GvRendering;

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
GsRayMap::GsRayMap()
:	_rayMap( 0 )
,	_rayMapType( eClassical )
,	_graphicsResource( NULL )
,	_isInitialized( false )
,	_shaderProgram( NULL )
,	_width( 0 )
,	_height( 0 )
,	_frameBuffer( 0 )
{
}

/******************************************************************************
 * Desconstructor
 ******************************************************************************/
GsRayMap::~GsRayMap()
{
	// Check if graphics resources have been initialized
	finalize();
}

/******************************************************************************
 * Initialize
 *
 * @return a flag to tell wheter or not it succeeds.
 ******************************************************************************/
bool GsRayMap::initialize()
{
	// First, check if graphics resources have already been initialized
	if ( _isInitialized )
	{
		finalize();
	}

	// Create associated CUDA graphics resource
	_graphicsResource = new GsGraphicsResource();
	if ( _graphicsResource == NULL )
	{
		// TO DO
		// Handle error
		// ...

		return false;
	}
	
	// Create associated OpenGL buffer
	glGenTextures( 1, &_rayMap );
	GV_CHECK_GL_ERROR();

	// Create associated OpenGL frame buffer object
	glGenFramebuffers(1, &_frameBuffer );
	GV_CHECK_GL_ERROR();

	// Update internal state
	_isInitialized = true;

	return true;
}

/******************************************************************************
 * Finalize
 *
 * @return a flag to tell wheter or not it succeeds.
 ******************************************************************************/
bool GsRayMap::finalize()
{
	if ( _graphicsResource != NULL )
	{
		delete _graphicsResource;
		_graphicsResource = NULL;
	}

	if ( _rayMap != 0 )
	{
		glDeleteBuffers( 1, &_rayMap );

		GV_CHECK_GL_ERROR();
	}

	if ( _frameBuffer != 0 )
	{
		glDeleteFramebuffers( 1, &_frameBuffer );
	}

	// Update internal state
	_isInitialized = false;

	return true;
}

/******************************************************************************
 * Set the ray map dimensions
 *
 * @param pWidth width
 * @param pHeight height
 ******************************************************************************/
void GsRayMap::setResolution( unsigned int pWidth, unsigned int pHeight )
{
	assert( ! ( pWidth == 0 || pHeight == 0 ) );

	if ( ! _isInitialized )
	{
		initialize();
	}

	// Update internal state
	_width = pWidth;
	_height= pHeight;

	cudaError_t error;

	// Unregister graphics resource
	if ( _graphicsResource->isRegistered() )
	{
		error = _graphicsResource->unregister();
		if ( error != cudaSuccess )
		{
			// TO DO
			// Handle error
			// ...

			assert( false );
		}
	}

	glBindTexture( GL_TEXTURE_RECTANGLE, _rayMap );

	// Texture parameters
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// Allocate texture storage
	GLenum target = GL_TEXTURE_RECTANGLE;
	GLint level = 0;
	GLint internalFormat = GL_RGBA32F;
	GLsizei width = pWidth;
	GLsizei height = pHeight;
	GLint border = 0;
	GLenum format = GL_RGBA;
	GLenum type = GL_FLOAT;
	const GLvoid* pixels = NULL;
	glTexImage2D( target, level, internalFormat, width, height, border, format, type, pixels );
	
	glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
	
	GV_CHECK_GL_ERROR();

	// Register graphics resource
	//
	// Attach an OpenGL texture or renderbuffer object to an internal graphics resource 
	// that will be mapped in CUDA memory space during rendering.
	error = _graphicsResource->registerImage( _rayMap, GL_TEXTURE_RECTANGLE, cudaGraphicsRegisterFlagsReadOnly );
	if ( error != cudaSuccess )
	{
		// TO DO
		// Handle error
		// ...

		assert( false );
	}
}

/******************************************************************************
 * ...
 ******************************************************************************/
bool GsRayMap::createShaderProgram( const char* pFileNameVS, const char* pFileNameFS )
{
	// Create and link a GLSL shader program
	// Initialize shader program
	_shaderProgram = new GsShaderProgram();
	std::string vertexShaderFilename( pFileNameVS );
	std::string fragmentShaderFilename( pFileNameFS );
	_shaderProgram->addShader( GsShaderProgram::eVertexShader, vertexShaderFilename );
	_shaderProgram->addShader( GsShaderProgram::eFragmentShader, fragmentShaderFilename );
	_shaderProgram->link();

	return true;
}

/******************************************************************************
 * Render
 ******************************************************************************/
void GsRayMap::render()
{
	assert( _rayMap != 0 );
	assert( _graphicsResource != NULL );
	assert( _isInitialized );

	glBindFramebuffer( GL_FRAMEBUFFER, _frameBuffer );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, _rayMap, 0 );
	GV_CHECK_GL_ERROR();

	_shaderProgram->use();
	GV_CHECK_GL_ERROR();

	GLuint vploc = glGetAttribLocation( _shaderProgram->_program, "iPosition" );
	GV_CHECK_GL_ERROR();

	//// Specify values of uniform variables for shader program object
	//glProgramUniform2fEXT( _rayMapProgram,
	//						glGetUniformLocation( _rayMapProgram, "screenResolutionInverse" )
	//											, static_cast< GLfloat >( 1.0f / _width )
	//											, static_cast< GLfloat >( 1.0f / _height ) );
	//GV_CHECK_GL_ERROR();

	// Specify values of uniform variables for shader program object
	glProgramUniform2fEXT( _shaderProgram->_program,
							glGetUniformLocation( _shaderProgram->_program, "uImageResolution" )
												, static_cast< GLfloat >( _width )
												, static_cast< GLfloat >( _height ) );
	GV_CHECK_GL_ERROR();

		// Extract view transformations
	float4x4 modelViewMatrix;
#if 0 // commit until port to OpenGL 4.6
	glGetFloatv( GL_MODELVIEW_MATRIX, modelViewMatrix._array );
#endif
	float4x4 invModelViewMatrix = inverse( modelViewMatrix );
	float4x4 projectionMatrix;
#if 0 // commit until port to OpenGL 4.6
	glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix._array );
#endif
	//float4x4 invMVP = inverse( mul( projectionMatrix, modelViewMatrix ) );
	//------ TEST
	//invModelViewMatrix = invMVP;

	// Specify values of uniform variables for shader program object
	glProgramUniform3fEXT( _shaderProgram->_program,
							glGetUniformLocation( _shaderProgram->_program, "uCameraXAxis" )
												, invModelViewMatrix._array[ 0 ], invModelViewMatrix._array[ 1 ], invModelViewMatrix._array[ 2 ] );
	GV_CHECK_GL_ERROR();

	// Specify values of uniform variables for shader program object
	glProgramUniform3fEXT( _shaderProgram->_program,
							glGetUniformLocation( _shaderProgram->_program, "uCameraYAxis" )
												, invModelViewMatrix._array[ 4 ], invModelViewMatrix._array[ 5 ], invModelViewMatrix._array[ 6 ] );
	GV_CHECK_GL_ERROR();

	// Specify values of uniform variables for shader program object
	glProgramUniform3fEXT( _shaderProgram->_program,
							glGetUniformLocation( _shaderProgram->_program, "uCameraZAxis" )
												, invModelViewMatrix._array[ 8 ], invModelViewMatrix._array[ 9 ], invModelViewMatrix._array[ 10 ] );
	GV_CHECK_GL_ERROR();

	// Specify values of uniform variables for shader program object
	float fnear = projectionMatrix._array[ 14 ] / ( projectionMatrix._array[ 10 ] - 1.0f );
	//std::cout << "fnear = " << fnear << std::endl;
	glProgramUniform1fEXT( _shaderProgram->_program,
							glGetUniformLocation( _shaderProgram->_program, "uCameraViewPlaneDistance" )
												, fnear );
	GV_CHECK_GL_ERROR();

	// Specify values of uniform variables for shader program object
	glProgramUniform1fEXT( _shaderProgram->_program,
							glGetUniformLocation( _shaderProgram->_program, "uFishEyePsiMaxAngle" )
												, 90.0 );
	GV_CHECK_GL_ERROR();

	// TO DO : check for OpenGL extensions "GL_ARB_shader_subroutine"
	GLuint rayDirectionSubroutineIndex = glGetSubroutineIndex( _shaderProgram->_program, GL_FRAGMENT_SHADER, "getClassicalRayDirection" );
	GV_CHECK_GL_ERROR();
	GLuint fishEyeRayDirectionSubroutineIndex = glGetSubroutineIndex(  _shaderProgram->_program, GL_FRAGMENT_SHADER, "getFishEyeRayDirection" );
	GV_CHECK_GL_ERROR();
	GLuint reflectionMapRayDirectionSubroutineIndex = glGetSubroutineIndex(  _shaderProgram->_program, GL_FRAGMENT_SHADER, "getReflectionMapRayDirection" );
	GV_CHECK_GL_ERROR();
	GLuint refractionMapRayDirectionSubroutineIndex = glGetSubroutineIndex(  _shaderProgram->_program, GL_FRAGMENT_SHADER, "getRefractionMapRayDirection" );
	GV_CHECK_GL_ERROR();

	switch ( _rayMapType )
	{
		case eClassical:
			glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &rayDirectionSubroutineIndex );
			break;

		case eFishEye:
			glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &fishEyeRayDirectionSubroutineIndex );
			break;

		case eReflectionMap:
			glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &reflectionMapRayDirectionSubroutineIndex );
			break;

		case eRefractionMap:
			glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &refractionMapRayDirectionSubroutineIndex );
			break;

		default:
			assert( false );
			std::cout << "GsRayMap::render() wrong enum passed to switch statement for rayMap type." << std::endl;
			break;
	}
	GV_CHECK_GL_ERROR();

#if 0 // comment until port to OpenGL 4.6
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glEnable( GL_TEXTURE_RECTANGLE );
	glDisable( GL_DEPTH_TEST );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_RECTANGLE, _rayMap );
	
	// Draw a quad on full screen
	glBegin( GL_QUADS );
	glVertexAttrib2f( vploc, -1.0f, -1.0f );
	glVertexAttrib2f( vploc,  1.0f, -1.0f );
	glVertexAttrib2f( vploc,  1.0f,  1.0f );
	glVertexAttrib2f( vploc, -1.0f,  1.0f );
	glEnd();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_RECTANGLE, 0 );

	glDisable( GL_TEXTURE_RECTANGLE );
	//glEnable( GL_DEPTH_TEST );

	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glUseProgram( 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

#endif
}

/******************************************************************************
 * Get the associated graphics resource
 *
 * @return the associated graphics resource
 ******************************************************************************/
GvRendering::GsGraphicsResource* GsRayMap::getGraphicsResource()
{
	return _graphicsResource;
}

/******************************************************************************
 * Get the shader program
 *
 * @return the shader program
 ******************************************************************************/
const GsGraphics::GsShaderProgram* GsRayMap::getShaderProgram() const
{
	return _shaderProgram;
}

/******************************************************************************
 * Edit the shader program
 *
 * @return the shader program
 ******************************************************************************/
GsGraphics::GsShaderProgram* GsRayMap::editShaderProgram()
{
	return _shaderProgram;
}

/******************************************************************************
 * Get the ray map type
 *
 * @return the ray map type
 ******************************************************************************/
GsRayMap::RayMapType GsRayMap::getRayMapType() const
{
	return _rayMapType;
}

/******************************************************************************
 * Set the ray map type
 *
 * @param pValue the ray map type
 ******************************************************************************/
void GsRayMap::setRayMapType( GsRayMap::RayMapType pValue )
{
	_rayMapType = pValue;
}
