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

#ifndef _GV_RAY_MAP_H_
#define _GV_RAY_MAP_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

#include "GvCore/GsVectorTypesExt.h"

// OpenGL
#include <glad/glad.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaVoxels
namespace GvRendering
{
	class GsGraphicsResource;
}
namespace GsGraphics
{
	class GsShaderProgram;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvUtils
{

/** 
 * @class GsRayMap
 *
 * @brief The GsRayMap class provides interface to handle a ray map.
 *
 * Ray map is a container of ray initialized for the rendering phase.
 */
class GIGASPACE_EXPORT GsRayMap
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Ray map type
	 */
	enum RayMapType
	{
		eClassical,
		eFishEye,
		eReflectionMap,
		eRefractionMap
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsRayMap();

	/**
	 * Destructor
	 */
	virtual ~GsRayMap();

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
	 * Set the ray map dimensions
	 *
	 * @param pWidth width
	 * @param pHeight height
	 */
	void setResolution( unsigned int pWidth, unsigned int pHeight );

	/**
	 * ...
	 */
	bool createShaderProgram( const char* pFileNameVS, const char* pFileNameFS );

	/**
	 * Render
	 */
	void render();

	/**
	 * Get the associated graphics resource
	 *
	 * @return the associated graphics resource
	 */
	GvRendering::GsGraphicsResource* getGraphicsResource();

	/**
	 * Get the shader program
	 *
	 * @return the shader program
	 */
	const GsGraphics::GsShaderProgram* getShaderProgram() const;

	/**
	 * Edit the shader program
	 *
	 * @return the shader program
	 */
	GsGraphics::GsShaderProgram* editShaderProgram();

	/**
	 * Get the ray map type
	 *
	 * @return the ray map type
	 */
	RayMapType getRayMapType() const;

	/**
	 * Set the ray map type
	 *
	 * @param pValue the ray map type
	 */
	void setRayMapType( RayMapType pValue );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Ray map type
	 */
	RayMapType _rayMapType;

	/**
	 * OpenGL ray map buffer
	 */
	GLuint _rayMap;

	/**
	 * Associated graphics resource
	 */
	GvRendering::GsGraphicsResource* _graphicsResource;

	/**
	 * Flag to tell wheter or not the associated instance is initialized
	 */
	 bool _isInitialized;

	/**
	 * Ray map generator's GLSL shader program
	 */
	GsGraphics::GsShaderProgram* _shaderProgram;

	/**
	 * Frame width
	 */
	unsigned int _width;

	/**
	 * Frame height
	 */
	unsigned int _height;

	/**
	 * Frame buffer object
	 */
	GLuint _frameBuffer;

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

//#include "GsRayMap.inl"

#endif
