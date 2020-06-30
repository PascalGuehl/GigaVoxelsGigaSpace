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

#ifndef _GS_I_GRAPHICS_OBJECT_H_
#define _GS_I_GRAPHICS_OBJECT_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// OpenGL
#include <glad/glad.h>

// GigaSpace
#include "GsGraphics/GsGraphicsCoreConfig.h"
#include "GsGraphics/GsShaderProgram.h"

// STL
#include <string>
#include <vector>
#include <map>

// glm
#include <glm/glm.hpp>

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
 * @class GsIGraphicsObject
 *
 * @brief The GsIGraphicsObject class provides an interface for mesh management.
 *
 * ...
 */
class GSGRAPHICS_EXPORT GsIGraphicsObject
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Mesh attributes
	 */
	enum EMeshAttributes
	{
		eVertex = 1,
		eNormal = 1 << 1,
		eTexCoord = 1 << 2,
		eIndex = 1 << 3,
		eAllAttributes = (1 << 4) - 1
	};

	/**
	 * Shader program configuration
	 */
	struct ShaderProgramConfiguration
	{
		/**
		 * ...
		 */
		std::map< GsShaderProgram::ShaderType, std::string > _shaders;

		/**
		 * ...
		 */
		void reset() { _shaders.clear(); }
	};

	/******************************* ATTRIBUTES *******************************/

	// Mesh bounds
	//
	// @todo add accessors and move to protected section
	float _minX;
	float _minY;
	float _minZ;
	float _maxX;
	float _maxY;
	float _maxZ;
	
	/**
	 * ...
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	glm::vec3 _min;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * ...
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	glm::vec3 _max;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/******************************** METHODS *********************************/

	/**
	 * Destructor
	 */
	virtual ~GsIGraphicsObject();

	/**
	 * Initialize
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	virtual bool initialize();

	/**
	 * Finalize
	 *
	 * @return a flag telling wheter or not it succeeds
	 */
	virtual bool finalize();

	/**
	 * Load mesh
	 */
	virtual bool load( const char* pFilename );

	/**
	 * This function is the specific implementation method called
	 * by the parent GsIRenderer::render() method during rendering.
	 *
	 * @param pModelMatrix the current model matrix
	 * @param pViewMatrix the current view matrix
	 * @param pProjectionMatrix the current projection matrix
	 * @param pViewport the viewport configuration
	 */
	virtual void render( const glm::mat4& pModelViewMatrix, const glm::mat4& pProjectionMatrix, const glm::uvec4& pViewport );
	// TO DO : add GLM version also
	// ...

	// TO DO
	// add a user-defined callback

	/**
	 * Get the shape color
	 *
	 * @return the shape color
	 */
	const glm::vec3& getColor() const;

	/**
	 * Set the shape color
	 *
	 * @param pColor the shape color
	 */
	void setColor( const glm::vec3& pColor );

	/**
	 * Tell wheter or not the spiral arms are enabled.
	 *
	 * @return a flag telling wheter or not the spiral arms are enabled
	 */
	bool isWireframeEnabled() const;

	/**
	 * Tell wheter or not the spiral arms are enabled.
	 *
	 * @param pFlag a flag telling wheter or not the spiral arms are enabled
	 */
	void setWireframeEnabled( bool pFlag );

	/**
	 * Get the shape color
	 *
	 * @return the shape color
	 */
	const glm::vec3& getWireframeColor() const;

	/**
	 * Set the shape color
	 *
	 * @param pColor the shape color
	 */
	void setWireframeColor( const glm::vec3& pColor );

	/**
	 * Tell wheter or not the spiral arms are enabled.
	 *
	 * @return a flag telling wheter or not the spiral arms are enabled
	 */
	float getWireframeLineWidth() const;

	/**
	 * Tell wheter or not the spiral arms are enabled.
	 *
	 * @param pFlag a flag telling wheter or not the spiral arms are enabled
	 */
	void setWireframeLineWidth( float pValue );

	/**
	 * Set shader program configuration
	 */
	void setShaderProgramConfiguration( const ShaderProgramConfiguration& pShaderProgramConfiguration );
	
	/**
	 * TODO
	 * - add setters to be able to build mesh without loading data from a file
	 */
	void setVertexBuffer( const std::vector< glm::vec3 >& pVertices );
	void setNormalBuffer( const std::vector< glm::vec3 >& pNormals );
	void setTexCoordsBuffer( const std::vector< glm::vec2 >& pTexCoords );
	void setIndexBuffer( const std::vector< unsigned int >& pIndices );

	/**
	 * Get the object/scene bound
	 *
	 * @param pMin Bottom left corner
	 * @param pMax Upper right corner
	 */
	void getBounds( glm::vec3& pMin, glm::vec3& pMax ) const;

	/**
	 * Get the object/scene bound
	 *
	 * @param pMin Bottom left corner
	 * @param pMax Upper right corner
	 */
	void getGigaSpaceBounds( glm::vec3& pMin, glm::vec3& pMax ) const;

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Shader program
	 */
	GsShaderProgram* _shaderProgram;

	/**
	 * VAO for mesh rendering (vertex array object)
	 */
	GLuint _vertexArray;
	GLuint _vertexBuffer;
	GLuint _normalBuffer;
	GLuint _texCoordsBuffer;
	GLuint _indexBuffer;

	/**
	 * Flag to tell wheter or not to use interleaved buffers
	 */
	bool _useInterleavedBuffers;

	/**
	 *
	 */
	unsigned int _nbVertices;
	unsigned int _nbFaces;

	/**
	 * Flag to tell wheter or not mesh has normals
	 */
	bool _hasNormals;

	/**
	 * Flag to tell wheter or not mesh has texture coordinates
	 */
	bool _hasTextureCoordinates;

	/**
	 * Flag to tell wheter or not mesh uses indexed rendering
	 */
	bool _useIndexedRendering;
	
	/**
	 * Spiral arms color
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	glm::vec3 _color;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Enable spiral arms
	 */
	bool _isWireframeEnabled;

	/**
	 * Spiral arms color
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	glm::vec3 _wireframeColor;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Spiral arms nb sections
	 */
	float _wireframeLineWidth;

	/**
	 * Shader program configuration
	 */
	ShaderProgramConfiguration _shaderProgramConfiguration;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsIGraphicsObject();

	/**
	 * Read mesh data
	 */
	virtual bool read( const char* pFilename, std::vector< glm::vec3 >& pVertices, std::vector< glm::vec3 >& pNormals, std::vector< glm::vec2 >& pTexCoords, std::vector< unsigned int >& pIndices );
	virtual bool initializeGraphicsResources( std::vector< glm::vec3 >& pVertices, std::vector< glm::vec3 >& pNormals, std::vector< glm::vec2 >& pTexCoords, std::vector< unsigned int >& pIndices );
	
	/**
	 * Initialize shader program
	 */
	virtual bool initializeShaderProgram();

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
	GsIGraphicsObject( const GsIGraphicsObject& );

	/**
	 * Copy operator forbidden.
	 */
	GsIGraphicsObject& operator=( const GsIGraphicsObject& );

};

} // namespace GsGraphics

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsIGraphicsObject.inl"

#endif // _GS_I_GRAPHICS_OBJECT_H_
