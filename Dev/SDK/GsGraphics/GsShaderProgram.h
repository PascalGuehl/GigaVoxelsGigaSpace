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

#ifndef _GS_SHADER_PROGRAM_H_
#define _GS_SHADER_PROGRAM_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GsGraphics/GsGraphicsCoreConfig.h"

// OpenGL
#include <glad/glad.h>

// System
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

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GsGraphics
{

/** 
 * @class GsShaderProgram
 *
 * @brief The GsShaderProgram class provides interface to handle a ray map.
 *
 * Ray map is a container of ray initialized for the rendering phase.
 */
class GSGRAPHICS_EXPORT GsShaderProgram
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Shader type enumeration
	 */
	enum ShaderType
	{
		eVertexShader = 0,
		eTesselationControlShader,
		eTesselationEvaluationShader,
		eGeometryShader,
		eFragmentShader,
		eComputeShader,
		eNbShaderTypes
	};

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Main shader program
	 */
	GLuint _program;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsShaderProgram();

	/**
	 * Destructor
	 */
	virtual ~GsShaderProgram();

	/**
	 * Initialize
	 *
	 * @return a flag to tell wheter or not it succeeds.
	 */
	bool initialize();

	/**
	 * Finalize
	 *
	 * @return a flag to tell wheter or not it succeeds.
	 */
	bool finalize();

	/**
	 * Compile shader
	 */
	bool addShader( ShaderType pShaderType, const std::string& pShaderFileName );

	/**
	 * Link program
	 */
	bool link();

	/**
	 * Use program
	 */
	inline void use();

	/**
	 * Unuse program
	 */
	static inline void unuse();

	/**
	 * Set fixed pipeline
	 */
	static inline void setFixedPipeline();

	/**
	 * Tell wheter or not pipeline has a given type of shader
	 *
	 * @param pShaderType the type of shader to test
	 *
	 * @return a flag telling wheter or not pipeline has a given type of shader
	 */
	bool hasShaderType( ShaderType pShaderType ) const;

	/**
	 * Get the source code associated to a given type of shader
	 *
	 * @param pShaderType the type of shader
	 *
	 * @return the associated shader source code
	 */
	std::string getShaderSourceCode( ShaderType pShaderType ) const;

	/**
	 * Get the filename associated to a given type of shader
	 *
	 * @param pShaderType the type of shader
	 *
	 * @return the associated shader filename
	 */
	std::string getShaderFilename( ShaderType pShaderType ) const;

	/**
	 * ...
	 *
	 * @param pShaderType the type of shader
	 *
	 * @return ...
	 */
	bool reloadShader( ShaderType pShaderType );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Vertex shader file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _vertexShaderFilename;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Tesselation Control shader file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _tesselationControlShaderFilename;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Tesselation Evaluation shader file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _tesselationEvaluationShaderFilename;
#if defined _MSC_VER
#pragma warning( pop )
#endif
	/**
	 * Geometry shader file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _geometryShaderFilename;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Fragment shader file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _fragmentShaderFilename;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Compute shader file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _computeShaderFilename;
#if defined _MSC_VER
#pragma warning( pop )
#endif
	
	/**
	 * Vertex shader source code
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _vertexShaderSourceCode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Tesselation Control shader source code
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _tesselationControlShaderSourceCode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Tesselation Evaluation shader source code
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _tesselationEvaluationShaderSourceCode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Geometry shader source code
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _geometryShaderSourceCode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Fragment shader source code
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _fragmentShaderSourceCode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Compute shader source code
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _computeShaderSourceCode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	///**
	// * Main shader program
	// */
	//GLuint _program;

	/**
	 * Vertex shader
	 */
	GLuint _vertexShader;

	/**
	 * Tesselation Control shader
	 */
	GLuint _tesselationControlShader;

	/**
	 * Tesselation Evaluation shader
	 */
	GLuint _tesselationEvaluationShader;

	/**
	 * Geometry shader
	 */
	GLuint _geometryShader;

	/**
	 * Fragment shader
	 */
	GLuint _fragmentShader;

	/**
	 * Compute shader
	 */
	GLuint _computeShader;

	/**
	 * ...
	 */
	bool _linked;

	/******************************** METHODS *********************************/

	/**
	 * ...
	 *
	 * @param pFilename ...
	 * @param pFileContent ...
	 *
	 * @return ...
	 */
	static bool getFileContent( const std::string& pFilename, std::string& pFileContent );

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
	GsShaderProgram( const GsShaderProgram& );

	/**
	 * Copy operator forbidden.
	 */
	GsShaderProgram& operator=( const GsShaderProgram& );

};

} // namespace GsGraphics

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsShaderProgram.inl"

#endif
