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

#include "GsIGraphicsObject.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>
#include <limits>

// System
#include <cassert>

// glm
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/transform2.hpp>
//#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GsGraphics;

// STL
using namespace std;

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
GsIGraphicsObject::GsIGraphicsObject()
:	_shaderProgram( NULL )
,	_vertexArray( 0 )
,	_vertexBuffer( 0 )
,	_normalBuffer( 0 )
,	_texCoordsBuffer( 0 )
,	_indexBuffer( 0 )
,	_useInterleavedBuffers( false )
,	_nbVertices( 0 )
,	_nbFaces( 0 )
,	_hasNormals( true )
,	_hasTextureCoordinates( false )
,	_useIndexedRendering( true )
,	_color( glm::vec3( 1.f, 1.f, 1.f ) )
,	_isWireframeEnabled( false )
,	_wireframeColor( glm::vec3( 0.f, 0.f, 0.f ) )
,	_wireframeLineWidth( 1.f )
,	_shaderProgramConfiguration()
{
	_minX = 0.f;
	_minY = 0.f;
	_minZ = 0.f;
	_maxX = 0.f;
	_maxY = 0.f;
	_maxZ = 0.f;
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsIGraphicsObject::~GsIGraphicsObject()
{
	finalize();
}

/******************************************************************************
 * Initialize
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
bool GsIGraphicsObject::initialize()
{
	// Initialize shader program
	_shaderProgram = new GsShaderProgram();
		
	return true;
}

/******************************************************************************
 * Finalize
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
bool GsIGraphicsObject::finalize()
{
	delete _shaderProgram;
	_shaderProgram = NULL;

	// TO DO
	// ... check first if it is different of zero !!

	glDeleteBuffers( 1, &_indexBuffer );
	glDeleteBuffers( 1, &_texCoordsBuffer );
	glDeleteBuffers( 1, &_normalBuffer );
	glDeleteBuffers( 1, &_vertexBuffer );
	glDeleteVertexArrays( 1, &_vertexArray );
	
	return true;
}

/******************************************************************************
 * Load mesh
 ******************************************************************************/
bool GsIGraphicsObject::load( const char* pFilename )
{
	assert( pFilename != NULL );
	if ( pFilename == NULL )
	{
		return false;
	}

	// Reset mesh bounds
	_minX = +std::numeric_limits< float >::max();
	_minY = +std::numeric_limits< float >::max();
	_minZ = +std::numeric_limits< float >::max();
	_maxX = -std::numeric_limits< float >::max();
	_maxY = -std::numeric_limits< float >::max();
	_maxZ = -std::numeric_limits< float >::max();

	// Read mesh data
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > texCoords;
	std::vector< unsigned int > indices;
	bool statusOK = read( pFilename, vertices, normals, texCoords, indices );
	assert( statusOK );
	if ( ! statusOK )
	{
		// Clean data
		//...

		return false;
	}

	// Initialize graphics resources
	statusOK = initializeGraphicsResources( vertices, normals, texCoords, indices );
	assert( statusOK );
	if ( ! statusOK )
	{
		// Clean data
		//...

		return false;
	}

	//// Initialize shader program
	//statusOK = initializeShaderProgram();
	//assert( statusOK );
	//if ( ! statusOK )
	//{
	//	// Clean data
	//	//...

	//	return false;
	//}
		
	return true;
}

/******************************************************************************
 * Set shader program configuration
 ******************************************************************************/
void GsIGraphicsObject::setShaderProgramConfiguration( const GsIGraphicsObject::ShaderProgramConfiguration& pShaderProgramConfiguration )
{
	// TO DO
	// - clean internal state
	// - shader program too ?
	_shaderProgramConfiguration.reset();

	_shaderProgramConfiguration = pShaderProgramConfiguration;
}

/******************************************************************************
 * Initialize shader program
 ******************************************************************************/
bool GsIGraphicsObject::initializeShaderProgram()
{
	/*shaders[ GsShaderProgram::eVertexShader ] = "";
	shaders[ GsShaderProgram::eTesselationControlShader ] = "";
	shaders[ GsShaderProgram::eTesselationEvaluationShader ] = "";
	shaders[ GsShaderProgram::eGeometryShader ] = "";
	shaders[ GsShaderProgram::eFragmentShader ] = "";
	shaders[ GsShaderProgram::eComputeShader ] = "";*/

	bool statusOK;

	// Initialize shader program
	_shaderProgram = new GsShaderProgram();
	assert( _shaderProgram != NULL );
	
	// Iterate through shaders
	std::map< GsShaderProgram::ShaderType, std::string >::const_iterator shaderIt = _shaderProgramConfiguration._shaders.begin();
	for ( ; shaderIt != _shaderProgramConfiguration._shaders.end(); ++shaderIt )
	{
		statusOK = _shaderProgram->addShader( shaderIt->first, shaderIt->second );
		assert( statusOK );
	}
	statusOK = _shaderProgram->link();
	assert( statusOK );

	return true;
}

/******************************************************************************
 * Read mesh data
 ******************************************************************************/
bool GsIGraphicsObject::read( const char* pFilename, std::vector< glm::vec3 >& pVertices, std::vector< glm::vec3 >& pNormals, std::vector< glm::vec2 >& pTexCoords, std::vector< unsigned int >& pIndices )
{
	return false;
}

/******************************************************************************
 * Read mesh
 ******************************************************************************/
bool GsIGraphicsObject::initializeGraphicsResources( std::vector< glm::vec3 >& pVertices, std::vector< glm::vec3 >& pNormals, std::vector< glm::vec2 >& pTexCoords, std::vector< unsigned int >& pIndices )
{
	// Vertex buffer initialization
	assert( pVertices.size() > 0 );
	glGenBuffers( 1, &_vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, _vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * pVertices.size(), &pVertices[ 0 ], GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	// Normal buffer initialization
	if ( _hasNormals )
	{
		assert( pNormals.size() > 0 );
		glGenBuffers( 1, &_normalBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, _normalBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * pNormals.size(), &pNormals[ 0 ], GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	// Textute coordinates buffer initialization
	if ( _hasTextureCoordinates )
	{
		assert( pTexCoords.size() > 0 );
		glGenBuffers( 1, &_texCoordsBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, _texCoordsBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * pTexCoords.size(), &pTexCoords[ 0 ], GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	// Index buffer initialization
	if ( _useIndexedRendering )
	{
		assert( pIndices.size() > 0 );
		glGenBuffers( 1, &_indexBuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * pIndices.size(), &pIndices[ 0 ], GL_STATIC_DRAW );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

	// Vertex array initialization
	glGenVertexArrays( 1, &_vertexArray );
	glBindVertexArray( _vertexArray );
	// Vertex position attribute
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, _vertexBuffer );
	glVertexAttribPointer( 0/*attribute index*/, 3/*nb components per vertex*/, GL_FLOAT/*type*/, GL_FALSE/*un-normalized*/, 0/*memory stride*/, static_cast< GLubyte* >( NULL )/*byte offset from buffer*/ );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	// Vertex normal attribute
	if ( _hasNormals )
	{
		glEnableVertexAttribArray( 1 );
		glBindBuffer( GL_ARRAY_BUFFER, _normalBuffer );
		glVertexAttribPointer( 1/*attribute index*/, 3/*nb components per vertex*/, GL_FLOAT/*type*/, GL_FALSE/*un-normalized*/, 0/*memory stride*/, static_cast< GLubyte* >( NULL )/*byte offset from buffer*/ );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}
	// Vertex texture coordinates attribute
	if ( _hasTextureCoordinates )
	{
		glEnableVertexAttribArray( 2 );
		glBindBuffer( GL_ARRAY_BUFFER, _texCoordsBuffer );
		glVertexAttribPointer( 2/*attribute index*/, 2/*nb components per vertex*/, GL_FLOAT/*type*/, GL_FALSE/*un-normalized*/, 0/*memory stride*/, static_cast< GLubyte* >( NULL )/*byte offset from buffer*/ );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}
	// Required for indexed rendering
	if ( _useIndexedRendering )
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );
	}
	glBindVertexArray( 0 );

	return true;
}

/******************************************************************************
 * Render
 ******************************************************************************/
void GsIGraphicsObject::render( const glm::mat4& pModelViewMatrix, const glm::mat4& pProjectionMatrix, const glm::uvec4& pViewport )
{
	// Configure rendering state
	_shaderProgram->use();
	glBindVertexArray( _vertexArray );

	// Set uniforms
	GLuint location = glGetUniformLocation( _shaderProgram->_program, "uModelViewMatrix" );
	if ( location >= 0 )
	{
		glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( pModelViewMatrix ) );
	}
	location = glGetUniformLocation( _shaderProgram->_program, "uProjectionMatrix" );
	if ( location >= 0 )
	{
		glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( pProjectionMatrix ) );
	}
	location = glGetUniformLocation( _shaderProgram->_program, "uMeshColor" );
	if ( location >= 0 )
	{
		glm::vec4 color = glm::vec4( 1.f, 0.f, 0.f, 1.f );
		glUniform4f( location, color.x, color.y, color.z, color.w );
	}
	if ( _hasNormals )
	{
		location = glGetUniformLocation( _shaderProgram->_program, "uNormalMatrix" );
		if ( location >= 0 )
		{
			// TO DO
			// - retrieve or compute Normal matrix
			//
			float normalMatrix[ 9 ];
			glUniformMatrix3fv( location, 1, GL_FALSE, normalMatrix );
		}
	}

	// Draw mesh
	//glDrawArrays( GL_TRIANGLES, 0, 0 );
	glDrawElements( GL_TRIANGLES, _nbFaces * 3, GL_UNSIGNED_INT, NULL );
	
	// Reset rendering state
	glBindVertexArray( 0 );
	glUseProgram( 0 );
}

/******************************************************************************
 * Get the shape color
 *
 * @return the shape color
 ******************************************************************************/
const glm::vec3& GsIGraphicsObject::getColor() const
{
	return _color;
}

/******************************************************************************
 * Set the shape color
	 *
	 * @param pColor the shape color
 ******************************************************************************/
void GsIGraphicsObject::setColor( const glm::vec3& pColor )
{
	_color = pColor;
}

/******************************************************************************
 * Get the shape color
 *
 * @return the shape color
 ******************************************************************************/
bool GsIGraphicsObject::isWireframeEnabled() const
{
	return _isWireframeEnabled;
}

/******************************************************************************
 * Set the shape color
	 *
	 * @param pColor the shape color
 ******************************************************************************/
void GsIGraphicsObject::setWireframeEnabled( bool pFlag )
{
	_isWireframeEnabled = pFlag;
}

/******************************************************************************
 * Get the shape color
 *
 * @return the shape color
 ******************************************************************************/
const glm::vec3& GsIGraphicsObject::getWireframeColor() const
{
	return _wireframeColor;
}

/******************************************************************************
 * Set the shape color
	 *
	 * @param pColor the shape color
 ******************************************************************************/
void GsIGraphicsObject::setWireframeColor( const glm::vec3& pColor )
{
	_wireframeColor = pColor;
}

/******************************************************************************
 * Get the shape color
 *
 * @return the shape color
 ******************************************************************************/
float GsIGraphicsObject::getWireframeLineWidth() const
{
	return _wireframeLineWidth;
}

/******************************************************************************
 * Set the shape color
 *
 * @param pColor the shape color
 ******************************************************************************/
void GsIGraphicsObject::setWireframeLineWidth( float pValue )
{
	_wireframeLineWidth = pValue;
}
