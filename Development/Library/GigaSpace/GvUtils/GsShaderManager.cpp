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

#include "GvUtils/GsShaderManager.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvUtils;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * Size of the string, the shorter is better
 */
#define STRING_BUFFER_SIZE 2048

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/**
 * Size of the string, the shorter is better
 */
char stringBuffer[ STRING_BUFFER_SIZE ];

/**
 * ...
 */
bool linkNeeded = false;

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * GLSL shader program creation
 *
 * @param pFileNameVS ...
 * @param pFileNameGS ...
 * @param pFileNameFS ...
 * @param pProgramID ...
 * @param pLazyRecompile ...
 *
 * @return ...
 ******************************************************************************/
GLuint GsShaderManager::createShaderProgram( const char* pFileNameVS, const char* pFileNameGS, const char* pFileNameFS, GLuint pProgramID, bool pLazyRecompile )
{
	bool reload = pProgramID != 0;

	if ( reload && pLazyRecompile )
	{
		return pProgramID;
	}

	linkNeeded = true;

	GLuint vertexShaderID = 0;
	GLuint geometryShaderID = 0;
	GLuint fragmentShaderID = 0;

	if ( ! reload )
	{
		// Create GLSL program
		pProgramID = glCreateProgram();
	}
	else
	{
		GLsizei count;
		GLuint shaders[ 3 ];
		glGetAttachedShaders( pProgramID, 3, &count, shaders );

		for ( int i = 0; i < count; i++ )
		{
			GLint shadertype;
			glGetShaderiv( shaders[ i ], GL_SHADER_TYPE, &shadertype );

			if ( shadertype == GL_VERTEX_SHADER )
			{
				vertexShaderID = shaders[ i ];
			}
			else if ( shadertype == GL_GEOMETRY_SHADER )
			{
				geometryShaderID = shaders[ i ];
			}
			else if ( shadertype == GL_FRAGMENT_SHADER )
			{
				fragmentShaderID = shaders[ i ];
			}
		}
	}
	
	if ( pFileNameVS )
	{
		// Create vertex shader
		vertexShaderID = createShader( pFileNameVS, GL_VERTEX_SHADER, vertexShaderID );
		if ( ! reload )
		{
			// Attach vertex shader to program object
			glAttachShader( pProgramID, vertexShaderID );
		}
	}

	if ( pFileNameGS )
	{
		// Create geometry shader
		geometryShaderID = createShader( pFileNameGS, GL_GEOMETRY_SHADER, geometryShaderID );
		if ( ! reload )
		{
			// Attach vertex shader to program object
			glAttachShader( pProgramID, geometryShaderID );
		}
	}
	
	if ( pFileNameFS )
	{
		// Create fragment shader
		fragmentShaderID = createShader( pFileNameFS, GL_FRAGMENT_SHADER, fragmentShaderID );
		if ( ! reload )
		{
			// Attach fragment shader to program object
			glAttachShader( pProgramID, fragmentShaderID );
		}
	}
	
	return pProgramID;
}


/******************************************************************************
 * GLSL shader creation (of a certain type, vertex shader, fragment shader or geometry shader)
 *
 * @param pFileName ...
 * @param pShaderType ...
 * @param pShaderID ...
 *
 * @return ...
 ******************************************************************************/
GLuint GsShaderManager::createShader( const char* pFileName, GLuint pShaderType, GLuint pShaderID )
{
	if ( pShaderID == 0 )
	{
		pShaderID = glCreateShader( pShaderType );
	}
	
	std::string shaderSource = loadTextFile( pFileName );

	// Manage #includes
    shaderSource = manageIncludes( shaderSource, std::string( pFileName ) );

    // Passing shader source code to GL
	// Source used for "pShaderID" shader, there is only "1" source code and the string is NULL terminated (no sizes passed)
	const char* src = shaderSource.c_str();
	glShaderSource( pShaderID, 1, &src, NULL );

	// Compile shader object
	glCompileShader( pShaderID );

	// Check compilation status
	GLint ok;
	glGetShaderiv( pShaderID, GL_COMPILE_STATUS, &ok );
	if ( ! ok )
	{
		int ilength;
		glGetShaderInfoLog( pShaderID, STRING_BUFFER_SIZE, &ilength, stringBuffer );
		
		std::cout << "Compilation error (" << pFileName << ") : " << stringBuffer; 
	}

	return pShaderID;
}

/******************************************************************************
 * ...
 *
 * @param pProgramID ...
 * @param pStat ...
 ******************************************************************************/
void GsShaderManager::linkShaderProgram( GLuint pProgramID )
{
	int linkStatus;
	glGetProgramiv( pProgramID, GL_LINK_STATUS, &linkStatus );
	if ( linkNeeded )
	{
		// Link all shaders togethers into the GLSL program
		glLinkProgram( pProgramID );
		checkProgramInfos( pProgramID, GL_LINK_STATUS );

		// Validate program executability giving current OpenGL states
		glValidateProgram( pProgramID );
		checkProgramInfos( pProgramID, GL_VALIDATE_STATUS );
		//std::cout << "Program " << pProgramID << " linked\n";

		linkNeeded = false;
	}
}

/******************************************************************************
 * Text file loading for shaders sources
 *
 * @param pMacro ...
 *
 * @return ...
 ******************************************************************************/
std::string GsShaderManager::loadTextFile( const char* pName )
{
	//Source file reading
	std::string buff("");
	
	std::ifstream file;
	file.open( pName );
	if ( file.fail() )
	{
		std::cout<< "loadFile: unable to open file: " << pName;
	}
	
	buff.reserve( 1024 * 1024 );

	std::string line;
	while ( std::getline( file, line ) )
	{
		buff += line + "\n";
	}

	const char* txt = buff.c_str();

	return std::string( txt );
}

/******************************************************************************
 * ...
 *
 * @param pSrc ...
 * @param pSourceFileName ...
 *
 * @return ...
 ******************************************************************************/
std::string GsShaderManager::manageIncludes( std::string pSrc, std::string pSourceFileName )
{
	std::string res;
	res.reserve( 100000 );

	char buff[ 512 ];
	sprintf( buff, "#include" );
	
	size_t includepos = pSrc.find( buff, 0 );

	while ( includepos != std::string::npos )
	{
		bool comment = pSrc.substr( includepos - 2, 2 ) == std::string( "//" );

		if ( ! comment )
		{
			size_t fnamestartLoc = pSrc.find( "\"", includepos );
			size_t fnameendLoc = pSrc.find( "\"", fnamestartLoc + 1 );

			size_t fnamestartLib = pSrc.find( "<", includepos );
			size_t fnameendLib = pSrc.find( ">", fnamestartLib + 1 );

			size_t fnameEndOfLine = pSrc.find( "\n", includepos );

			size_t fnamestart;
			size_t fnameend;

			bool uselibpath = false;
			if ( ( fnamestartLoc == std::string::npos || fnamestartLib < fnamestartLoc ) && fnamestartLib < fnameEndOfLine )
			{
				fnamestart = fnamestartLib;
				fnameend = fnameendLib;
				uselibpath = true;
			}
			else if ( fnamestartLoc != std::string::npos && fnamestartLoc < fnameEndOfLine )
			{
				fnamestart = fnamestartLoc;
				fnameend = fnameendLoc;
				uselibpath = false;
			}
			else
			{
                std::cerr << "manageIncludes : invalid #include directive into \"" << pSourceFileName.c_str() << "\"\n";
				return pSrc;
			}

			std::string incfilename = pSrc.substr( fnamestart + 1, fnameend - fnamestart - 1 );
			std::string incsource;

			if ( uselibpath )
			{
				std::string usedPath;

				// TODO: Add paths types into the manager -> search only onto shaders paths.
				std::vector< std::string > pathsList;
				// ResourcesManager::getManager()->getPaths( pathsList );
                pathsList.push_back( "./" );

				for ( std::vector< std::string >::iterator it = pathsList.begin(); it != pathsList.end(); it++ )
				{
					std::string fullpathtmp = (*it) + incfilename;
					
					FILE* file = 0;
					file = fopen( fullpathtmp.c_str(), "r" );
					if ( file )
					{
						usedPath = (*it);
						fclose( file );
						break;
					}
					else
					{
						usedPath = "";
					}
				}
				
				if ( usedPath != "" )
				{
					incsource = loadTextFile( ( usedPath + incfilename ).c_str() );
				}
				else
				{
                    std::cerr << "manageIncludes : Unable to find included file \"" << incfilename.c_str() << "\" in system paths.\n";
					return pSrc;
				}
			} else
			{
				incsource = loadTextFile(
					( pSourceFileName.substr( 0, pSourceFileName.find_last_of( "/", pSourceFileName.size() ) + 1 )
						+ incfilename ).c_str()
				);
			}

			incsource = manageIncludes( incsource, pSourceFileName );
			incsource = incsource.substr( 0, incsource.size() - 1 );
			
			std::string preIncludePart = pSrc.substr( 0, includepos );
			std::string postIncludePart = pSrc.substr( fnameend + 1, pSrc.size() - fnameend );

			int numline = 0;
			size_t newlinepos = 0;
			do
			{
				newlinepos = preIncludePart.find( "\n", newlinepos + 1 );
				numline++;
			}
			while ( newlinepos != std::string::npos );
			numline--;
			
			char buff2[ 512 ];
			sprintf( buff2, "\n#line 0\n" );
			std::string linePragmaPre( buff2 );
			sprintf( buff2, "\n#line %d\n", numline );
			std::string linePragmaPost( buff2 );
			
			res = preIncludePart + linePragmaPre + incsource + linePragmaPost + postIncludePart;

			pSrc = res;
		}
		includepos = pSrc.find( buff, includepos + 1 );
	}

	return pSrc;
}

/******************************************************************************
 * ...
 *
 * @param pProgramID ...
 * @param pStat ...
 ******************************************************************************/
void GsShaderManager::checkProgramInfos( GLuint pProgramID, GLuint pStat )
{
	GLint ok = 0;
	glGetProgramiv( pProgramID, pStat, &ok );
	if ( ! ok )
	{
		int ilength;
		glGetProgramInfoLog( pProgramID, STRING_BUFFER_SIZE, &ilength, stringBuffer );
		
		std::cout << "Program error :\n" << stringBuffer << "\n"; 
		
		int cc;
		std::cin >> cc;
	}
}
