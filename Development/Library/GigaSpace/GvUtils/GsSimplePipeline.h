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

#ifndef _GV_SIMPLE_PIPELINE_H_
#define _GV_SIMPLE_PIPELINE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsIProvider.h"
#include "GvCore/GsVectorTypesExt.h"
#include "GvCache/GsCacheHelper.h"
#include "GvUtils/GsPipeline.h"
#include "GvCore/GsIProvider.h"
#include "GvRendering/GsIRenderer.h"

// STL
#include <vector>

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

namespace GvUtils
{

/** 
 * @class GsSimplePipeline
 *
 * @brief The GsSimplePipeline class provides the mecanisms to produce data
 * following data requests emitted during N-tree traversal (rendering).
 *
 * It is the main user entry point to produce data from CPU, for instance,
 * loading data from disk or procedurally generating data.
 *
 * This class is implements the mandatory functions of the GsIProvider base class.
 */
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
class GsSimplePipeline : public GsPipeline
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Defines the size of a node tile
	 */
	typedef typename TDataStructureType::NodeTileResolution NodeTileResolution;

	/**
	 * Defines the size of a brick tile
	 */
	typedef typename TDataStructureType::BrickResolution BrickTileResolution;

	/**
	 * Defines the size of the border around a brick tile
	 */
	enum
	{
		BrickTileBorderSize = TDataStructureType::BrickBorderSize
	};

	/**
	 * Defines the total size of a brick tile (with borders)
	 */
	typedef typename TDataStructureType::FullBrickResolution RealBrickTileResolution;

	/**
	 * Defines the data type list
	 */
	typedef typename TDataStructureType::DataTypeList DataTypeList;

	/**
	 * Type defition of the data structure
	 */
	typedef TDataStructureType DataStructureType;

	/**
	 * Type defition of the cache
	 */
	typedef TCacheType CacheType;

	/**
	 * Type defition of the producer
	 */
	typedef GvCore::GsIProvider ProducerType;

	/**
	 * Type defition of renderers
	 */
	typedef GvRendering::GsIRenderer RendererType;
	
	/**
	 * Type defition of the shader
	 */
	typedef TShaderType ShaderType;
	
	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsSimplePipeline();

	/**
	 * Destructor
	 */
	virtual ~GsSimplePipeline();

	/**
	 * Initialize
	 *
	 * @param pNodePoolMemorySize Node pool memory size
	 * @param pBrickPoolMemorySize Brick pool memory size
	 * @param pShader the shader
	 * @param pUseGraphicsLibraryInteroperability a flag telling whether or not to use GL interoperability mode
	 */
	virtual void initialize( size_t pNodePoolMemorySize, size_t pBrickPoolMemorySize, TShaderType* pShader, bool pUseGraphicsLibraryInteroperability = false );

	/**
	 * Finalize
	 */
	virtual void finalize();

	/**
	 * Launch the main GigaSpace flow sequence
	 */
	virtual void execute();
	virtual void execute( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );
	
	/**
	 * Get the data structure
	 *
	 * @return The data structure
	 */
	virtual const DataStructureType* getDataStructure() const;

	/**
	 * Get the data structure
	 *
	 * @return The data structure
	 */
	virtual DataStructureType* editDataStructure();

	/**
	 * Get the cache
	 *
	 * @return The cache
	 */
	virtual const CacheType* getCache() const;

	/**
	 * Get the cache
	 *
	 * @return The cache
	 */
	virtual CacheType* editCache();

	/**
	 * Get the renderer
	 *
	 * @param pIndex index of the renderer
	 *
	 * @return The renderer
	 */
	virtual const RendererType* getRenderer( unsigned int pIndex = 0 ) const;

	/**
	 * Get the renderer
	 *
	 * @param pIndex index of the renderer
	 *
	 * @return The renderer
	 */
	virtual RendererType* editRenderer( unsigned int pIndex = 0 );

	/**
	 * Add a renderer
	 *
	 * @param pRenderer The renderer
	 */
	void addRenderer( RendererType* pRenderer );

	/**
	 * Remove a renderer
	 *
	 * @param pRenderer The renderer
	 */
	void removeRenderer( RendererType* pRenderer );

	/**
	 * Set the current renderer
	 *
	 * @param pRenderer The renderer
	 */
	//void setCurrentRenderer( RendererType* pRenderer );

	/**
	 * Get the producer
	 *
	 * @param pIndex index of the producer
	 *
	 * @return The producer
	 */
	virtual const ProducerType* getProducer( unsigned int pIndex = 0 ) const;

	/**
	 * Get the producer
	 *
	 * @param pIndex index of the producer
	 *
	 * @return The producer
	 */
	virtual ProducerType* editProducer( unsigned int pIndex = 0 );

	/**
	 * Add a producer
	 *
	 * @param pProducer The producer
	 */
	void addProducer( ProducerType* pProducer );

	/**
	 * Remove a producer
	 *
	 * @param pProducer The producer
	 */
	void removeProducer( ProducerType* pProducer );

	/**
	 * Set the current producer
	 *
	 * @param pProducer The producer
	 */
	void setCurrentProducer( ProducerType* pProducer );

	/**
	 * Get the shader
	 *
	 * @return The shader
	 */
	const TShaderType* getShader() const;

	/**
	 * Get the shader
	 *
	 * @return The shader
	 */
	TShaderType* editShader();

	/**
	 * Return the flag used to request a dynamic update mode.
	 *
	 * @return the flag used to request a dynamic update mode
	 */
	bool hasDynamicUpdate() const;

	/**
	 * Set the flag used to request a dynamic update mode.
	 *
	 * @param pFlag the flag used to request a dynamic update mode
	 */
	void setDynamicUpdate( bool pFlag );

	/**
	 * Set the flag used to request clearing the cache.
	 */
	void clear();

	/**
	 * Print datatype info
	 */
	void printDataTypeInfo();

	/**
	 * This method is called to serialize an object
	 *
	 * @param pStream the stream where to write
	 */
	virtual void write( std::ostream& pStream ) const;

	/**
	 * This method is called deserialize an object
	 *
	 * @param pStream the stream from which to read
	 */
	virtual void read( std::istream& pStream );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Data structure
	 */
	DataStructureType* _dataStructure;
	
	/**
	 * Cache
	 */
	CacheType* _cache;
	
	/**
	 * Renderer(s)
	 */
	RendererType* _renderer;
	std::vector< RendererType* > _renderers;

	/**
	 * Node pool memory size
	 */
	size_t _nodePoolMemorySize;

	/**
	 * Brick pool memory size
	 */
	size_t _brickPoolMemorySize;

	/**
	 * Producer
	 */
	ProducerType* _producer;
	std::vector< ProducerType* > _producers;

	/**
	 * Shader
	 */
	TShaderType* _shader;

	/**
	 * Flag used to request clearing the cache
	 */
	bool _clearRequested;

	/**
	 * Flag used to request a dynamic update mode.
	 *
	 * @todo explain
	 */
	bool _dynamicUpdate;

	/******************************** METHODS *********************************/

	/**
	 * Compute the resolution of the pools
	 *
	 * @param pNodePoolResolution Node pool resolution
	 * @param pBrickPoolResolution Brick pool resolution
	 */
	void computePoolResolution( uint3& pNodePoolResolution, uint3& pBrickPoolResolution );

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsSimplePipeline( const GsSimplePipeline& );

	/**
	 * Copy operator forbidden.
	 */
	GsSimplePipeline& operator=( const GsSimplePipeline& );

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsSimplePipeline.inl"

#endif // !_GV_SIMPLE_PIPELINE_H_
