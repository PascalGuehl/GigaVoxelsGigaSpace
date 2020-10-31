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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{
	
/******************************************************************************
 * Constructor
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::GsSimplePipeline()
:	GsPipeline()
,	_dataStructure( NULL )
,	_cache( NULL )
,	_renderer( NULL )
,	_nodePoolMemorySize( 0 )
,	_brickPoolMemorySize( 0 )
,	_producer( NULL )
,	_shader( NULL )
,	_clearRequested( false )
,	_dynamicUpdate( true )
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::~GsSimplePipeline()
{
	// Free memory
	finalize();
}

/******************************************************************************
 * Initialize
 *
 * @param pNodePoolMemorySize Node pool memory size
 * @param pBrickPoolMemorySize Brick pool memory size
 * @param pShader the shader
 * @param pUseGraphicsLibraryInteroperability a flag telling whether or not to use GL interoperability mode
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::initialize( size_t pNodePoolMemorySize, size_t pBrickPoolMemorySize, TShaderType* pShader, bool pUseGraphicsLibraryInteroperability )
{
	assert( pNodePoolMemorySize > 0 );
	assert( pBrickPoolMemorySize > 0 );
	assert( pShader != NULL );

	// Print datatype info
	printDataTypeInfo();

	// Store shader
	_shader = pShader;

	// Store global memory size of the pools
	_nodePoolMemorySize = pNodePoolMemorySize;
	_brickPoolMemorySize = pBrickPoolMemorySize;

	// Compute the resolution of the pools
	uint3 nodePoolResolution;
	uint3 brickPoolResolution;
	computePoolResolution( nodePoolResolution, brickPoolResolution );

	std::cout << "\nNode pool resolution : " << nodePoolResolution << std::endl;
	std::cout << "Brick pool resolution : " << brickPoolResolution << std::endl;

	// Retrieve the requested graphics library interoperability mode
	const unsigned int useGraphicsLibraryInteroperability = pUseGraphicsLibraryInteroperability ? 1 : 0;

	// Data structure
	_dataStructure = new DataStructureType( nodePoolResolution, brickPoolResolution, useGraphicsLibraryInteroperability );
	assert( _dataStructure != NULL );

	// Cache
	_cache = new CacheType( _dataStructure, nodePoolResolution, brickPoolResolution, useGraphicsLibraryInteroperability );
	assert( _cache != NULL );
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::finalize()
{
	// Free memory
	delete _renderer;	// TO DO free all renderers
	_renderer = NULL;
	delete _cache;
	_cache = NULL;
	delete _dataStructure;
	_dataStructure = NULL;

	// Free memory
	delete _producer;	// TO DO free all producers
	_producer = NULL;
	delete _shader;
	_shader = NULL;
}

/******************************************************************************
 * Launch the main GigaSpace flow sequence
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::execute()
{
	// [ 1 ] - Rendering stage
	//_renderer->render();

	// [ 2 ] - Data Production Management stage
	//_cache->handleRequests();
}

/******************************************************************************
 * Launch the main GigaSpace flow sequence
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::execute( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	// Check if a "clear request" has been asked
	if ( _clearRequested )
	{
		CUDAPM_START_EVENT( gpucache_clear );

		// Clear the cache
		_cache->clearCache();

		// Bug [#16161] "Cache : not cleared as it should be"
		//
		// Without the following clear of the data structure node pool, artefacts should appear.
		// - it is visible in the Slisesix and ProceduralTerrain demos.
		//
		// It seems that the brick addresses of the node pool need to be reset.
		// Maybe it's a problem of "time stamp" and index of current frame (or time).
		// 
		// @todo : study this problem
		_dataStructure->clearVolTree();

		CUDAPM_STOP_EVENT( gpucache_clear );

		// Update "clear request" flag
		_clearRequested = false;
	}

#ifndef GS_USE_MULTI_OBJECTS

	// Map resources()
	// - this function provides the synchronization guarantee that any graphics calls issued before cudaGraphicsMapResources() will complete before any subsequent CUDA work in stream begins.
	_renderer->preRender( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );
	
	// [ 1 ] - Pre-render stage
	_cache->preRenderPass();

	// [ 2 ] - Rendering stage
	_renderer->render( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );

	// Unmap resources()
	_renderer->postRender( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );

#else

	// [ 1 ] - Pre-render stage
	//_cache->preRenderPass();

	// [ 2 ] - Rendering stage
	//for ( size_t i = 0; i < _renderers.size(); i++ )
	//{
	//}

#endif

	// [ 3 ] - Post-render stage (i.e. Data Production Management)
	CUDAPM_START_EVENT( dataProduction_handleRequests );
	if ( _dynamicUpdate )
	{
		_cache->_intraFramePass = false;

		// Post render pass
		// This is where requests are processed : produce or load data
		_cache->handleRequests();
	}
	CUDAPM_STOP_EVENT( dataProduction_handleRequests );
}

/******************************************************************************
 * Get the data structure
 *
 * @return The data structure
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline const GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::DataStructureType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::getDataStructure() const
{
	return _dataStructure;
}

/******************************************************************************
 * Get the data structure
 *
 * @return The data structure
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::DataStructureType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::editDataStructure()
{
	return _dataStructure;
}

/******************************************************************************
 * Get the cache
 *
 * @return The cache
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline const GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::CacheType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::getCache() const
{
	return _cache;
}

/******************************************************************************
 * Get the cache
 *
 * @return The cache
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::CacheType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::editCache()
{
	return _cache;
}

/******************************************************************************
 * Get the renderer
 *
 * @param pIndex index of the renderer
 *
 * @return The renderer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline const GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::RendererType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::getRenderer( unsigned int pIndex ) const
{
	assert( pIndex < _renderers.size() );
	return _renderers[ pIndex ];
}

/******************************************************************************
 * Get the renderer
 *
 * @param pIndex index of the renderer
 *
 * @return The renderer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::RendererType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::editRenderer( unsigned int pIndex )
{
	assert( pIndex < _renderers.size() );
	return _renderers[ pIndex ];
}

/******************************************************************************
 * Add a renderer
 *
 * @param pRenderer The renderer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::addRenderer( RendererType* pRenderer )
{
	_renderers.push_back( pRenderer );

	// TO DO
	// - do it properly...
	_renderer = pRenderer;
}

/******************************************************************************
 * Remove a renderer
 *
 * @param pRenderer The renderer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::removeRenderer( RendererType* pRenderer )
{
	// TO DO
	assert( false );
}

/******************************************************************************
 * Get the producer
 *
 * @return The producer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline const GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::ProducerType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::getProducer( unsigned int pIndex ) const
{
	assert( pIndex < _producers.size() );
	return _producers[ pIndex ];
}

/******************************************************************************
 * Get the producer
 *
 * @return The producer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >::ProducerType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::editProducer( unsigned int pIndex )
{
	assert( pIndex < _producers.size() );
	return _producers[ pIndex ];
}

/******************************************************************************
 * Add a producer
 *
 * @param pProducer The producer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::addProducer( ProducerType* pProducer )
{
	assert( pProducer != NULL );
	assert( _dataStructure != NULL );
	assert( _cache != NULL );

	_producers.push_back( pProducer );

	// TO DO
	// - do it properly...
	setCurrentProducer( pProducer );

	// Initialize the producer with the data structure
	//
	// TO DO : add a virtual base function "hasDataStructure() = false;" to GsSimpleHostProducer class
	pProducer->initialize( _dataStructure, _cache );

	// Add producer to data prouction manager
	_cache->addProducer( pProducer );
}

/******************************************************************************
 * Remove a producer
 *
 * @param pProducer The producer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::removeProducer( ProducerType* pProducer )
{
	// TO DO
	assert( false );
}

/******************************************************************************
 * Set the current producer
 *
 * @param pProducer The producer
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::setCurrentProducer( ProducerType* pProducer )
{
	// TO DO
	// - do it properly...
	_producer = pProducer;

	// Add producer to data prouction manager
	_cache->setCurrentProducer( pProducer );
}

/******************************************************************************
 * Get the shader
 *
 * @return The shader
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline const TShaderType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::getShader() const
{
	return _shader;
}

/******************************************************************************
 * Get the shader
 *
 * @return The shader
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline TShaderType*
GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::editShader()
{
	return _shader;
}

/******************************************************************************
 * Compute the resolution of the pools
 *
 * @param pNodePoolResolution Node pool resolution
 * @param pBrickPoolResolution Brick pool resolution
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
inline void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::computePoolResolution( uint3& pNodePoolResolution, uint3& pBrickPoolResolution )
{
	assert( _nodePoolMemorySize != 0 );
	assert( _brickPoolMemorySize != 0 );
		
	// Compute the size of one element in the cache for nodes and bricks
	size_t nodeTileMemorySize = NodeTileResolution::numElements * sizeof( GvStructure::GsNode );
	size_t brickTileMemorySize = RealBrickTileResolution::numElements * GvCore::DataTotalChannelSize< DataTypeList >::value;

	// Compute how many we can fit into the given memory size
	size_t nodePoolNbElements = _nodePoolMemorySize / nodeTileMemorySize;
	size_t brickPoolNbElements = _brickPoolMemorySize / brickTileMemorySize;

	// Compute the resolution of the pools
	pNodePoolResolution = make_uint3( static_cast< uint >( floorf( powf( static_cast< float >( nodePoolNbElements ), 1.0f / 3.0f ) ) ) ) * NodeTileResolution::get();
	pBrickPoolResolution = make_uint3( static_cast< uint >( floorf( powf( static_cast< float >( brickPoolNbElements ), 1.0f / 3.0f ) ) ) ) * RealBrickTileResolution::get();
}

/******************************************************************************
 * Return the flag used to request a dynamic update mode.
 *
 * @return the flag used to request a dynamic update mode
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
bool GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::hasDynamicUpdate() const
{
	return _dynamicUpdate;
}

/******************************************************************************
 * Set the flag used to request a dynamic update mode.
 *
 * @param pFlag the flag used to request a dynamic update mode
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::setDynamicUpdate( bool pFlag )
{
	_dynamicUpdate = pFlag;

	// Update renderer state
	_renderer->setDynamicUpdate( pFlag );
}

/******************************************************************************
 * Set the flag used to request clearing the cache.
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::clear()
{
	_clearRequested = true;
}

/******************************************************************************
 * Print datatype info
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::printDataTypeInfo()
{
	std::cout << "\nVoxel datatype(s) : " << GvCore::DataNumChannels< DataTypeList >::value << " channel(s)" << std::endl;
	GvCore::GvDataTypeInspector< DataTypeList > dataTypeInspector;
	GvCore::StaticLoop< GvCore::GvDataTypeInspector< DataType >, GvCore::DataNumChannels< DataTypeList >::value - 1 >::go( dataTypeInspector );
	for ( int i = 0; i < dataTypeInspector._dataTypes.size(); i++ )
	{
		std::cout << "- " << dataTypeInspector._dataTypes[ i ] << std::endl;
	}
}

/******************************************************************************
 * This method is called to serialize an object
 *
 * @param pStream the stream where to write
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::write( std::ostream& pStream ) const
{
	// TO DO
	// ...
}

/******************************************************************************
 * This method is called deserialize an object
 *
 * @param pStream the stream from which to read
 ******************************************************************************/
template< typename TShaderType, typename TDataStructureType, typename TCacheType >
void GsSimplePipeline< TShaderType, TDataStructureType, TCacheType >
::read( std::istream& pStream )
{
	// TO DO
	// ...
}

} // namespace GvUtils
