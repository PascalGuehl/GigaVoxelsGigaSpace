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

#ifndef _SAMPLECORE_H_
#define _SAMPLECORE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GvCore/GsCoreConfig.h"

// Cuda
#include <cuda_runtime.h>
#include <vector_types.h>
//#include <helper_math.h>

// Loki
#include <loki/Typelist.h>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// Forward references
namespace GvCore
{
	template< uint r >
	struct GsVec1D;
}

namespace GvStructure
{
	template<
		class DataTList, class NodeTileRes,
		class BrickRes, uint BorderSize >
	struct GsVolumeTree;

	template <
		typename VolumeTreeType, typename ProducerType,
		typename NodeTileRes, typename BrickFullRes >
	class GsDataProductionManager;
}

namespace GvRendering
{	
	template<
		typename VolumeTreeType, typename VolumeTreeCacheType,
		typename ProducerType, typename SampleShader >
	class GsRendererCUDA;
}

// Producers
template< typename DataTList, typename NodeRes, typename BrickRes, uint BorderSize >
class ProducerLoad;

// Shaders
class ShaderLoad;

// Defines the type list representing the content of one voxel
typedef Loki::TL::MakeTypelist< uchar4 >::Result DataType;

// Defines the size of a node tile
typedef GvCore::GsVec1D< 2 > NodeRes;

// Defines the size of a brick
typedef GvCore::GsVec1D< 8 > BrickRes;

// Defines the size of the border around a brick
enum { BrickBorderSize = 1 };

// Defines the total size of a brick
typedef GvCore::GsVec1D<8 + 2 * BrickBorderSize> RealBrickRes;

// Defines the type of the producer
typedef ShaderLoad ShaderType;

// Defines the type of the shader
typedef ProducerLoad< DataType,
	NodeRes, BrickRes, BrickBorderSize >		ProducerType;

// Defines the type of structure we want to use.
typedef GvStructure::GsVolumeTree< DataType,
	NodeRes, BrickRes, BrickBorderSize >		VolumeTreeType;

// Defines the type of the cache we want to use.
typedef GvStructure::GsDataProductionManager<
	VolumeTreeType, ProducerType,
	NodeRes, RealBrickRes >						VolumeTreeCacheType;

// Defines the type of the renderer we want to use.
typedef GvRendering::GsRendererCUDA<
	VolumeTreeType, VolumeTreeCacheType,
	ProducerType, ShaderType >					VolumeTreeRendererType;

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
	void draw();
	/**
	 * ...
	 *
	 * @param width ...
	 * @param height ...
	 */
	void resize( int width, int height );

	/**
	 * Clear the cache
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
	 * ...
	 */
	VolumeTreeType			*mVolumeTree;
	/**
	 * ...
	 */
	VolumeTreeCacheType		*mVolumeTreeCache;
	/**
	 * ...
	 */
	VolumeTreeRendererType	*mVolumeTreeRenderer;
	/**
	 * ...
	 */
	ProducerType			*mProducer;

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

#endif // !_SAMPLECORE_H_
