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

#ifndef _SAMPLE_CORE_H_
#define _SAMPLE_CORE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Cuda
#include <cuda_runtime.h>
#include <vector_types.h>

// Cuda SDK
#include <helper_math.h>

// GL
//#include <GL/glew.h>
#include <glad/glad.h>

// Loki
#include <loki/Typelist.h>

// GigaVoxels
#include <GvCore/GsVectorTypesExt.h>
#include <GvUtils/GsForwardDeclarationHelper.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// Custom Producer
template< typename TDataStructureType >
class ProducerKernel;

// Custom Shader
class Shader {};	// Define a NullType and/or EmptyType

// Custom Renderer
template
<
	typename VolumeTreeType, typename VolumeTreeCacheType,
	typename SampleShader
>
class VolumeTreeRendererGLSL;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

// Defines the type list representing the content of one voxel
typedef Loki::TL::MakeTypelist< uchar4 >::Result DataType;

// Defines the size of a node tile
typedef GvCore::GsVec1D< 2 > NodeRes;

// Defines the size of a brick
typedef GvCore::GsVec1D< 8 > BrickRes;

// Defines the size of the border around a brick
enum { BrickBorderSize = 1 };

// Defines the total size of a brick
typedef GvCore::GsVec1D< 8 + 2 * BrickBorderSize > RealBrickRes;

// Defines the type of structure we want to use.
typedef GvStructure::GsVolumeTree
<
	DataType,
	NodeRes, BrickRes
>
DataStructureType;

// Defines the type of the producer
typedef GvStructure::GsDataProductionManager< DataStructureType > DataProductionManagerType;

// Defines the type of the producer
typedef GvUtils::GsSimpleHostProducer
<
	ProducerKernel< DataStructureType >,
	DataStructureType,
	DataProductionManagerType
>
ProducerType;

// Defines the type of the shader
typedef Shader ShaderType;

// Defines the type of the renderer we want to use.
typedef VolumeTreeRendererGLSL
<
	DataStructureType,
	DataProductionManagerType,
	ShaderType
>
RendererType;

// Simple Pipeline
typedef GvUtils::GsSimplePipeline
<
	ShaderType,
	DataStructureType,
	DataProductionManagerType
>
PipelineType;

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/** 
 * @class SampleCore
 *
 * @brief The SampleCore class provides...
 *
 * ...
 */
class SampleCore
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Internal matrices
	 */
	float4x4 _modelMatrix;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	SampleCore();

	/**
	 * Destructor
	 */
	~SampleCore();

	/**
	 * ...
	 */
	void init();
	/**
	 * ...
	 */
	//void draw();
	void render( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );

	/**
	 * ...
	 *
	 * @param width ...
	 * @param height ...
	 */
	void resize( int width, int height );

	/**
	 * ...
	 */
	void clearCache();

	/**
	 * ...
	 */
	void toggleDisplayOctree();
	/**
	 * ...
	 */
	void toggleDynamicUpdate();
	/**
	 * ...
	 *
	 * @param mode ...
	 */
	void togglePerfmonDisplay( uint mode );

	/**
	 * ...
	 */
	void incMaxVolTreeDepth();
	/**
	 * ...
	 */
	void decMaxVolTreeDepth();

	/**
	 * Set the light position
	 *
	 * @param pX the X light position
	 * @param pY the Y light position
	 * @param pZ the Z light position
	 */
	void setLightPosition( float pX, float pY, float pZ );

	/**
	 * Get the max depth of the volume tree
	 *
	 * @return max depth
	 */
	uint get_dataStructure_MaxDepth() const;

	/**
	 * Set the max depth of the volume tree
	 *
	 * @param maxDepth Max depth
	 */
	void set_dataStructure_MaxDepth( const uint pValue );

	/**
	 * Get the max depth of the volume tree
	 *
	 * @return max depth
	 */
	uint get_dataStructure_DisplayMode() const;

	/**
	 * Set the max depth of the volume tree
	 *
	 * @param maxDepth Max depth
	 */
	void set_dataStructure_DisplayMode( const bool pFlag );

	/**
	 * Render the data structure
	 */
	void gs_dataStructure_render();

	/**
	 * Material
	 */
	float gs_get_material_opacity() const;
	void gs_set_material_opacity( const float pValue );
	float m_material_opacity;

	bool gs_pipeline_get_dynamic_update() const;
	void gs_pipeline_set_dynamic_update( const bool pFlag );

	bool gs_renderer_hasPriorityOnBricks() const;
	void gs_renderer_setPriorityOnBricks( const bool pFlag );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * GigaVoxels pipeline
	 */
	PipelineType* _pipeline;

	/**
	 * GigaSpace producer
	 */
	ProducerType* _producer;

	/**
	 * GigaSpace renderer
	 */
	RendererType* _renderer;

	/**
	 * ...
	 */
	GLuint					mColorBuffer;
	/**
	 * ...
	 */
	GLuint					mDepthBuffer;

	/**
	 * ...
	 */
	GLuint					mColorTex;
	/**
	 * ...
	 */
	GLuint					mDepthTex;

	/**
	 * ...
	 */
	GLuint					mFrameBuffer;

	/**
	 * ...
	 */
	struct cudaGraphicsResource	*mColorResource;
	/**
	 * ...
	 */
	struct cudaGraphicsResource	*mDepthResource;

	/**
	 * ...
	 */
	int				mWidth;
	/**
	 * ...
	 */
	int				mHeight;

	/**
	 * ...
	 */
	bool			mDisplayOctree;
	/**
	 * ...
	 */
	uint			mDisplayPerfmon;
	/**
	 * ...
	 */
	uint			mMaxVolTreeDepth;

	/******************************** METHODS *********************************/

};

#endif // !_SAMPLE_CORE_H_
