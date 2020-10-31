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

#include "GsGraphics/GsShaderProgram.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <string>
#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>

#include <fstream>
#include <cerrno>

//#include <cassert>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GsGraphics;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * GLSL Compute shader features
 */
#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER 0x91B9
#endif

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsShaderProgram::GsShaderProgram()
:	_vertexShaderFilename()
,	_tesselationControlShaderFilename()
,	_tesselationEvaluationShaderFilename()
,	_geometryShaderFilename()
,	_fragmentShaderFilename()
,	_computeShaderFilename()
,	_vertexShaderSourceCode()
,	_tesselationControlShaderSourceCode()
,	_tesselationEvaluationShaderSourceCode()
,	_geometryShaderSourceCode()
,	_fragmentShaderSourceCode()
,	_computeShaderSourceCode()
,	_program( 0 )
,	_vertexShader( 0 )
,	_tesselationControlShader( 0 )
,	_tesselationEvaluationShader( 0 )
,	_geometryShader( 0 )
,	_fragmentShader( 0 )
,	_computeShader( 0 )
,	_linked( false )
{
	// Initialize graphics resources
	initialize();
}

/******************************************************************************
 * Desconstructor
 ******************************************************************************/
GsShaderProgram::~GsShaderProgram()
{
	// Release graphics resources
	finalize();
}

/******************************************************************************
 * Initialize
 ******************************************************************************/
bool GsShaderProgram::initialize()
{
	// First, check if a program has already been created
	// ...
	assert( _program == 0 );

	// Create program object
	_program = glCreateProgram();
	if ( _program == 0 )
	{
		// LOG
		// ...

		return false;
	}

	return true;
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
bool GsShaderProgram::finalize()
{
	// Check all data to release

	if ( _vertexShader )
	{
		glDetachShader( _program, _vertexShader );
		glDeleteShader( _vertexShader );
	}
	if ( _tesselationControlShader )
	{
		glDetachShader( _program, _tesselationControlShader );
		glDeleteShader( _tesselationControlShader );
	}
	if ( _tesselationEvaluationShader )
	{
		glDetachShader( _program, _tesselationEvaluationShader );
		glDeleteShader( _tesselationEvaluationShader );
	}
	if ( _geometryShader )
	{
		glDetachShader( _program, _geometryShader );
		glDeleteShader( _geometryShader );
	}
	if ( _fragmentShader )
	{
		glDetachShader( _program, _fragmentShader );
		glDeleteShader( _fragmentShader );
	}
	if ( _computeShader )
	{
		glDetachShader( _program, _computeShader );
		glDeleteShader( _computeShader );
	}
		
	// Delete program object
	if ( _program )
	{
		glDeleteProgram( _program );
	}

	_linked = false;

	return true;
}

/******************************************************************************
 * Compile shader
 ******************************************************************************/
bool GsShaderProgram::addShader( GsShaderProgram::ShaderType pShaderType, const std::string& pShaderFileName )
{
	assert( _program != 0 );

	// Retrieve file content
	std::string shaderSourceCode;
	bool isReadFileOK = getFileContent( pShaderFileName, shaderSourceCode );
	if ( ! isReadFileOK )
	{
		std::cerr<<"Error: can't read file " << pShaderFileName << std::endl;
		// LOG
		// ...

		return false;
	}

	// Create shader object
	GLuint shader = 0;
	switch ( pShaderType )
	{
		case GsShaderProgram::eVertexShader:
			shader = glCreateShader( GL_VERTEX_SHADER );
			break;

		case GsShaderProgram::eTesselationControlShader:
			shader = glCreateShader( GL_TESS_CONTROL_SHADER );
			break;

		case GsShaderProgram::eTesselationEvaluationShader:
			shader = glCreateShader( GL_TESS_EVALUATION_SHADER );
			break;

		case GsShaderProgram::eGeometryShader:
			shader = glCreateShader( GL_GEOMETRY_SHADER );
			break;

		case GsShaderProgram::eFragmentShader:
			shader = glCreateShader( GL_FRAGMENT_SHADER );
			break;

//TODO
			//- protect code if not defined
		case GsShaderProgram::eComputeShader:
			shader = glCreateShader( GL_COMPUTE_SHADER );

			// LOG
			// ...
			// GL_COMPUTE_SHADER is available only if the GL version is 4.3 or higher

			break;

		default:

			// LOG
			// ...

			return false;
	}

	// Check shader creation error
	if ( shader == 0 )
	{
		std::cerr<<"Error creating shader "<< pShaderFileName << std::endl;
		// LOG
		// ...

		return false;
	}

	switch ( pShaderType )
	{
		case GsShaderProgram::eVertexShader:
			_vertexShader = shader;
			_vertexShaderFilename = pShaderFileName;
			_vertexShaderSourceCode = shaderSourceCode;
			break;

		case GsShaderProgram::eTesselationControlShader:
			_tesselationControlShader = shader;
			_tesselationControlShaderFilename = pShaderFileName;
			_tesselationControlShaderSourceCode = shaderSourceCode;
			break;

		case GsShaderProgram::eTesselationEvaluationShader:
			_tesselationEvaluationShader = shader;
			_tesselationEvaluationShaderFilename = pShaderFileName;
			_tesselationEvaluationShaderSourceCode = shaderSourceCode;
			break;

		case GsShaderProgram::eGeometryShader:
			_geometryShader = shader;
			_geometryShaderFilename = pShaderFileName;
			_geometryShaderSourceCode = shaderSourceCode;
			break;

		case GsShaderProgram::eFragmentShader:
			_fragmentShader = shader;
			_fragmentShaderFilename = pShaderFileName;
			_fragmentShaderSourceCode = shaderSourceCode;
			break;

		case GsShaderProgram::eComputeShader:
			_computeShader = shader;
			_computeShaderFilename = pShaderFileName;
			_computeShaderSourceCode = shaderSourceCode;
			break;

		default:
			break;
	}

	// Replace source code in shader object
	const char* source = shaderSourceCode.c_str();
	glShaderSource( shader, 1, &source, NULL );

	// Compile shader object
	glCompileShader( shader );

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		// LOG
		// ...

		GLint logInfoLength = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			// Return information log for shader object
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetShaderInfoLog( shader, logInfoLength, &length, infoLog );

			// LOG
			std::cout << "\nGsShaderProgram::addShader() - compilation ERROR" << std::endl;
			std::cout << "File : " << pShaderFileName << std::endl;
			std::cout << infoLog << std::endl;			

			delete[] infoLog;
		}

		return false;
	}
	else
	{
		// Attach shader object to program object
		glAttachShader( _program, shader );
	}

	return true;
}

/******************************************************************************
 * Link program
 ******************************************************************************/
bool GsShaderProgram::link()
{
	assert( _program != 0 );

	if ( _linked )
	{
		return true;
	}

	if ( _program == 0 )
	{
		return false;
	}

	// Link program object
	glLinkProgram( _program );

	// Check linking status
	GLint linkStatus = 0;
	glGetProgramiv( _program, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_FALSE )
	{
		// LOG
		// ...

		GLint logInfoLength = 0;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			// Return information log for program object
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetProgramInfoLog( _program, logInfoLength, &length, infoLog );

			// LOG
			std::cout << "\nGsShaderProgram::link() - compilation ERROR" << std::endl;
			std::cout << infoLog << std::endl;

			delete[] infoLog;
		}

		return false;
	}
	
	// Update internal state
	_linked = true;
	
	return true;
}

/******************************************************************************
 * ...
 *
 * @param pFilename ...
 *
 * @return ...
 ******************************************************************************/
bool GsShaderProgram::getFileContent( const std::string& pFilename, std::string& pFileContent )
{
	std::ifstream file( pFilename.c_str(), std::ios::in );
	if ( file )
	{
		// Initialize a string to store file content
		file.seekg( 0, std::ios::end );
		pFileContent.resize( file.tellg() );
		file.seekg( 0, std::ios::beg );

		// Read file content
		file.read( &pFileContent[ 0 ], pFileContent.size() );

		// Close file
		file.close();

		return true;
	}
	else
	{
		// LOG
		// ...
	}

	return false;
}

/******************************************************************************
 * Tell wheter or not pipeline has a given type of shader
 *
 * @param pShaderType the type of shader to test
 *
 * @return a flag telling wheter or not pipeline has a given type of shader
 ******************************************************************************/
bool GsShaderProgram::hasShaderType( ShaderType pShaderType ) const
{
	bool result = false;

	GLuint shader = 0;
	switch ( pShaderType )
	{
		case GsShaderProgram::eVertexShader:
			shader = _vertexShader;
			break;

		case GsShaderProgram::eTesselationControlShader:
			shader = _tesselationControlShader;
			break;

		case GsShaderProgram::eTesselationEvaluationShader:
			shader = _tesselationEvaluationShader;
			break;

		case GsShaderProgram::eGeometryShader:
			shader = _geometryShader;
			break;

		case GsShaderProgram::eFragmentShader:
			shader = _fragmentShader;
			break;

		case GsShaderProgram::eComputeShader:
			shader = _computeShader;
			break;

		default:

			assert( false );

			break;
	}

	return ( shader != 0 );
}

/******************************************************************************
 * Get the source code associated to a given type of shader
 *
 * @param pShaderType the type of shader
 *
 * @return the associated shader source code
 ******************************************************************************/
std::string GsShaderProgram::getShaderSourceCode( ShaderType pShaderType ) const
{
	std::string shaderSourceCode( "" );
	switch ( pShaderType )
	{
		case GsShaderProgram::eVertexShader:
			shaderSourceCode = _vertexShaderSourceCode;
			break;
			
		case GsShaderProgram::eTesselationControlShader:
			shaderSourceCode = _tesselationControlShaderSourceCode;
			break;

		case GsShaderProgram::eTesselationEvaluationShader:
			shaderSourceCode = _tesselationEvaluationShaderSourceCode;
			break;
			
		case GsShaderProgram::eGeometryShader:
			shaderSourceCode = _geometryShaderSourceCode;
			break;

		case GsShaderProgram::eFragmentShader:
			shaderSourceCode = _fragmentShaderSourceCode;
			break;

		case GsShaderProgram::eComputeShader:
			shaderSourceCode = _computeShaderSourceCode;
			break;

		default:

			assert( false );

			break;
	}

	return shaderSourceCode;
}

/******************************************************************************
 * Get the filename associated to a given type of shader
 *
 * @param pShaderType the type of shader
 *
 * @return the associated shader filename
 ******************************************************************************/
std::string GsShaderProgram::getShaderFilename( ShaderType pShaderType ) const
{
	std::string shaderFilename( "" );
	switch ( pShaderType )
	{
		case GsShaderProgram::eVertexShader:
			shaderFilename = _vertexShaderFilename;
			break;
			
		case GsShaderProgram::eTesselationControlShader:
			shaderFilename = _tesselationControlShaderFilename;
			break;

		case GsShaderProgram::eTesselationEvaluationShader:
			shaderFilename = _tesselationEvaluationShaderFilename;
			break;
			
		case GsShaderProgram::eGeometryShader:
			shaderFilename = _geometryShaderFilename;
			break;

		case GsShaderProgram::eFragmentShader:
			shaderFilename = _fragmentShaderFilename;
			break;

		case GsShaderProgram::eComputeShader:
			shaderFilename = _computeShaderFilename;
			break;

		default:

			assert( false );

			break;
	}

	return shaderFilename;
}

/******************************************************************************
 * ...
 *
 * @param pShaderType the type of shader
 *
 * @return ...
 ******************************************************************************/
bool GsShaderProgram::reloadShader( ShaderType pShaderType )
{
	if ( ! hasShaderType( pShaderType ) )
	{
		// LOG
		// ...

		return false;
	}

	// Retrieve file content
	std::string shaderSourceCode;
	std::string shaderFilename = getShaderFilename( pShaderType );
	bool isReadFileOK = getFileContent( shaderFilename, shaderSourceCode );
	if ( ! isReadFileOK )
	{
		// LOG
		// ...

		return false;
	}
	
	GLuint shader = 0;
	switch ( pShaderType )
	{
		case GsShaderProgram::eVertexShader:
			shader = _vertexShader;
			break;

		case GsShaderProgram::eTesselationControlShader:
			shader = _tesselationControlShader;
			break;

		case GsShaderProgram::eTesselationEvaluationShader:
			shader = _tesselationEvaluationShader;
			break;

		case GsShaderProgram::eGeometryShader:
			shader = _geometryShader;
			break;

		case GsShaderProgram::eFragmentShader:
			shader = _fragmentShader;
			break;

		case GsShaderProgram::eComputeShader:
			shader = _computeShader;
			break;

		default:
			break;
	}

	// Check shader creation error
	if ( shader == 0 )
	{
		// LOG
		// ...

		return false;
	}

	// Replace source code in shader object
	const char* source = shaderSourceCode.c_str();
	glShaderSource( shader, 1, &source, NULL );

	// Compile shader object
	glCompileShader( shader );

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		// LOG
		// ...

		GLint logInfoLength = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			// Return information log for shader object
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetShaderInfoLog( shader, logInfoLength, &length, infoLog );

			// LOG
			std::cout << "\nGsShaderProgram::reloadShader() - compilation ERROR" << std::endl;
			std::cout << infoLog << std::endl;

			delete[] infoLog;
		}

		return false;
	}

	// Link program
	//
	// - first, unliked the program
	_linked = false;
	if ( ! link() )
	{
		return false;
	}
	
	return true;
}
