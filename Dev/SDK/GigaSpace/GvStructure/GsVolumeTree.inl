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

// GigaVoxels
#include "GvCore/GsError.h"
#include <GsSpaceCore/GsEnvironment.h>
#include <GsGraphics/GsShaderProgram.h>

// STL
#include <iostream>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvStructure
{

/******************************************************************************
 * Constructor.
 *
 * @param nodesCacheSize Cache size used to store nodes
 * @param bricksCacheRes Cache size used to store bricks
 * @param graphicsInteroperability Flag used for graphics interoperability
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::GsVolumeTree( const uint3& nodesCacheSize, const uint3& bricksCacheRes, uint graphicsInteroperability )
:	GsIDataStructure()
,	_childArray( NULL )
,	_dataArray( NULL )
#ifdef GS_USE_NODE_META_DATA
,	_metaDataArray( NULL )
#endif
,	_nodePool( NULL )
,	_dataPool( NULL )
,	_localizationCodeArray( NULL )
,	_localizationDepthArray( NULL )
,	_childArraySync( NULL )
,	_dataArraySync( NULL )
,	_showNodeHasBrickTerminal( true )
,	_showNodeHasBrickNotTerminal( true )
,	_showNodeIsBrickNotInCache( true )
,	_showNodeEmptyOrConstant( true )
,	_nodeHasBrickTerminalColor( make_float4( 1.0f, 1.0f, 0.0f, 1.0f ) )		// yellow
,	_nodeHasBrickNotTerminalColor( make_float4( 1.0f, 0.0f, 0.0f, 1.0f ) )	// red
,	_nodeIsBrickNotInCacheColor( make_float4( 0.0f, 1.0f, 0.0f, 1.0f ) )	// green
,	_nodeEmptyOrConstantColor( make_float4( 0.0f, 0.0f, 1.0f, 1.0f ) )		// blue
,	_renderingShaderProgram( nullptr )
{
	// LOG info
	std::cout << "\nData Structure ( N3-Tree )" << std::endl;
	std::cout << "- node cache size : " << nodesCacheSize << std::endl;
	std::cout << "- bricks cache resolution : " << bricksCacheRes << std::endl;

	// Node pool initialization
	_nodePool = new GvCore::GPUPoolHost< GvCore::GsLinearMemory, NodeTList >( nodesCacheSize, graphicsInteroperability );

	// Data pool initialization
	_dataPool = new GvCore::GPUPoolHost< GvCore::GsDeviceTexturingMemory, DataTList >( bricksCacheRes, graphicsInteroperability );

	// Helper object : it is a reference on nodes in the node pool
	_childArray = _nodePool->getChannel( Loki::Int2Type< 0 >() );
	_childArray->fill( 0 );

	// Helper object : it is a reference on bricks in the node pool
	_dataArray = _nodePool->getChannel( Loki::Int2Type< 1 >() );
	_dataArray->fill( 0 );

#ifdef GS_USE_NODE_META_DATA
	// Helper object : it is a reference on related node meta data in the node pool
	_metaDataArray = _nodePool->getChannel( Loki::Int2Type< 2 >() );
	_metaDataArray->fill( 0 );
#endif

	//volumeTreeKernel.dataGPUPoolKernel = this->dataGPUPool->getKernelPool();
	volumeTreeKernel.brickCacheResINV = make_float3( 1.0f ) / make_float3( bricksCacheRes );	// Size of 1 voxel in the pool of bricks of voxels (3D texture)
	volumeTreeKernel._rootAddress = NodeTileRes::getNumElements();
	volumeTreeKernel.brickSizeInCacheNormalized = make_float3( BrickRes::get() ) / make_float3( bricksCacheRes );

	////Localization codes////
	uint3 nodeCacheRes = nodesCacheSize / NodeTileRes::get();

	// Localization code array initialization
	_localizationCodeArray = new GvCore::GsLinearMemory< GvCore::GsLocalizationInfo::CodeType >( nodeCacheRes );
	_localizationCodeArray->fill( 0 );

	// Localization depth array initialization
	_localizationDepthArray = new GvCore::GsLinearMemory< GvCore::GsLocalizationInfo::DepthType >( nodeCacheRes );
	_localizationDepthArray->fill( 0 );

	_childArraySync	= NULL;
	_dataArraySync = NULL;

	setMaxDepth( 9 ); // this call cudaMemcpyToSymbol()

	this->cuda_Init();
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::~GsVolumeTree()
{
	delete _nodePool;
	delete _dataPool;

	delete _localizationCodeArray;
	delete _localizationDepthArray;

	delete _childArraySync;
	delete _dataArraySync;
}

/******************************************************************************
 * Clear the volume tree information
 * It clears the node pool and its associated localization info
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::clearVolTree()
{
	// Clear the child/brick addresses arrays.
	_childArray->fill( 0 );
	_dataArray->fill( 0 );
#ifdef GS_USE_NODE_META_DATA
	_metaDataArray->fill( 0 );
#endif

	// Clear the locations code/depth arrays.
	_localizationCodeArray->fill( 0 );
	_localizationDepthArray->fill( 0 );
}

/******************************************************************************
 * Cuda specific initialization
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::cuda_Init()
{
	////VOLTREE////

#if USE_LINEAR_VOLTREE_TEX
	volumeTreeChildTexLinear.normalized = false;					  // access with normalized texture coordinates
	volumeTreeChildTexLinear.addressMode[0] = cudaAddressModeClamp;   // wrap texture coordinates
	volumeTreeChildTexLinear.addressMode[1] = cudaAddressModeClamp;
	volumeTreeChildTexLinear.addressMode[2] = cudaAddressModeClamp;
	volumeTreeChildTexLinear.filterMode = cudaFilterModePoint;		// nearest interpolation

	cudaChannelFormatDesc indicesTexChannelDesc = cudaCreateChannelDesc<uint>();

	GS_CUDA_SAFE_CALL(cudaBindTexture(NULL, volumeTreeChildTexLinear,
		this->_childArray->getPointer()));

	volumeTreeDataTexLinear.normalized = false;					  // access with normalized texture coordinates
	volumeTreeDataTexLinear.addressMode[0] = cudaAddressModeClamp;   // wrap texture coordinates
	volumeTreeDataTexLinear.addressMode[1] = cudaAddressModeClamp;
	volumeTreeDataTexLinear.addressMode[2] = cudaAddressModeClamp;
	volumeTreeDataTexLinear.filterMode = cudaFilterModePoint;		// nearest interpolation

	GS_CUDA_SAFE_CALL(cudaBindTexture(NULL, volumeTreeDataTexLinear,
		this->_dataArray->getPointer()));
#endif

	// Data pool

	// LOG info
	std::cout << "\nData Pool" << std::endl;
	this->_dataPool->bindPoolToTextureReferences( Loki::Int2Type< TEXDATAPOOL >(), true, true, cudaFilterModeLinear, cudaAddressModeClamp ); //true, true, ...
	this->_dataPool->bindPoolToSurfaceReferences();

	// Copy node pool's nodes to constant memory (its pointer)
	GvCore::GsLinearMemoryKernel< uint > h_volumeTreeChildArray = this->_childArray->getDeviceArray();
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_volumeTreeChildArray, &h_volumeTreeChildArray, sizeof( h_volumeTreeChildArray ), 0, cudaMemcpyHostToDevice ) );

	// Copy node pool's bricks to constant memory (its pointer)
	GvCore::GsLinearMemoryKernel< uint > h_volumeTreeDataArray = this->_dataArray->getDeviceArray();
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_volumeTreeDataArray, &h_volumeTreeDataArray, sizeof( h_volumeTreeDataArray ), 0, cudaMemcpyHostToDevice ) );

#ifdef GS_USE_NODE_META_DATA
	GvCore::GsLinearMemoryKernel< uint > h_dataStructureMetaDataArray = this->_metaDataArray->getDeviceArray();
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_dataStructureMetaDataArray, &h_dataStructureMetaDataArray, sizeof( h_dataStructureMetaDataArray ), 0, cudaMemcpyHostToDevice ) );
#endif

	GV_CHECK_CUDA_ERROR( "GsVolumeTree::cuda_Init end" );

	// Create and link a GLSL shader program
	//QString shaderRepository = GsEnvironment::getDataDir( GsEnvironment::eShadersDir ).c_str();
	//std::string shaderRepository = GvUtils::GsEnvironment::getDataDir( GvUtils::GsEnvironment::eShadersDir ).c_str();
	std::string shaderRepository = GsCore::GsEnvironment::getDataDir( GsCore::GsEnvironment::eShadersDir ).c_str();
	//shaderRepository += QDir::separator();
	shaderRepository += std::string( "/" );
	//shaderRepository += QString( "SimpleSphere" );
	shaderRepository += std::string( "SimpleSphere" );
	shaderRepository += std::string( "/" );
	// Initialize points shader program
	//QString vertexShaderFilename = shaderRepository + QString( "fullscreenQuad_vert.glsl" );
	std::string vertexShaderFilename = shaderRepository + std::string( "cube_vert.glsl" );
	//QString fragmentShaderFilename = shaderRepository + QString( "fullscreenQuad_frag.glsl" );
	std::string fragmentShaderFilename = shaderRepository + std::string( "cube_frag.glsl" );
	// Initialize shader program
	_renderingShaderProgram = new GsGraphics::GsShaderProgram();
	//bool statusOK = _renderingShaderProgram->addShader( GsShaderProgram::eVertexShader, vertexShaderFilename.toStdString() );
	bool statusOK = _renderingShaderProgram->addShader( GsGraphics::GsShaderProgram::eVertexShader, vertexShaderFilename );
	assert( statusOK );
	if ( ! statusOK )
	{
		// TO DO
		// - handle error
	}
	//statusOK = _renderingShaderProgram->addShader( GsShaderProgram::eFragmentShader, fragmentShaderFilename.toStdString() );
	statusOK = _renderingShaderProgram->addShader( GsGraphics::GsShaderProgram::eFragmentShader, fragmentShaderFilename );
	assert( statusOK );
	if ( ! statusOK )
	{
		// TO DO
		// - handle error
	}
	statusOK = _renderingShaderProgram->link();
	assert( statusOK );
	if ( ! statusOK )
	{
		// TO DO
		// - handle error
	}
}

/******************************************************************************
 * Get the node tile resolution.
 *
 * @return the node tile resolution
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
const NodeTileRes& GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::getNodeTileResolution() const
{
	return _nodeTileResolution;
}

/******************************************************************************
 * Get the brick resolution (voxels).
 *
 * @param the brick resolution
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
const BrickRes& GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::getBrickResolution() const
{
	return _brickResolution;
}

/******************************************************************************
 * Get the max depth of the volume tree
 *
 * @return max depth
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
uint GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::getMaxDepth() const
{
	return _maxDepth;
}

/******************************************************************************
 * Set the max depth of the volume tree
 *
 * @param maxDepth Max depth
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::setMaxDepth( uint maxDepth )
{
	_maxDepth = maxDepth;

	// Update CUDA memory with value
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_maxVolTreeDepth, &_maxDepth, sizeof( _maxDepth ), 0, cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * Debugging helpers
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::render()
{
	syncDebugVolTree();

//	glDisable( GL_LIGHTING );
//	glDisable( GL_BLEND );
	//glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );



	
	_renderingShaderProgram->use();
	static bool isVAOInitialized = false;
	static GLuint vao;
	if ( ! isVAOInitialized )
	{
		glCreateVertexArrays( 1, &vao );

		isVAOInitialized = true;
	}

	//----------------------------------------------------------------------------------------------------------
	static bool isUniformUnitialized = false;
	if ( ! isUniformUnitialized )
	{
		_modelMatrixUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uModelMatrix" );
		assert( _modelMatrixUniformLocation != -1 );
		if ( _modelMatrixUniformLocation == -1 )
		{
			// TO DO
			// - handle error
		}

		_colorUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uColor" );
		assert( _colorUniformLocation != -1 );
		if ( _colorUniformLocation == -1 )
		{
			// TO DO
			// - handle error
		}
	
		// Update flag
		isUniformUnitialized = true;
	}
	//----------------------------------------------------------------------------------------------------------

	glBindVertexArray( vao );
	glEnable( GL_PROGRAM_POINT_SIZE );

	debugDisplay( 0/*depth*/, make_uint3( NodeTileRes::numElements, 0, 0 )/*address*/, make_float3( 0.0f )/*pos*/, make_float3( 1.0f )/*size*/ );

	glDisable( GL_PROGRAM_POINT_SIZE );
	glBindVertexArray( 0 );
	GsGraphics::GsShaderProgram::unuse();

//	glPolygonMode( GL_FRONT_AND_BACK , GL_FILL);
}

/******************************************************************************
 * Debugging helpers
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::render( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	syncDebugVolTree();

//	glDisable( GL_LIGHTING );
//	glDisable( GL_BLEND );
	//glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );



	
	_renderingShaderProgram->use();
	static bool isVAOInitialized = false;
	static GLuint vao;
	if ( ! isVAOInitialized )
	{
		glCreateVertexArrays( 1, &vao );

		isVAOInitialized = true;
	}

	//----------------------------------------------------------------------------------------------------------
	static bool isUniformUnitialized = false;
	static GLuint _sceneModelMatrixUniformLocation;
	static GLuint _sceneViewMatrixUniformLocation;
	static GLuint _sceneProjectionMatrixUniformLocation;
	if ( ! isUniformUnitialized )
	{
		_modelMatrixUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uModelMatrix" );
		assert( _modelMatrixUniformLocation != -1 );
		if ( _modelMatrixUniformLocation == -1 )
		{
			// TO DO
			// - handle error
		}

		_colorUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uColor" );
		assert( _colorUniformLocation != -1 );
		if ( _colorUniformLocation == -1 )
		{
			// TO DO
			// - handle error
		}

		_sceneModelMatrixUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uSceneModelMatrix" );
		_sceneViewMatrixUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uSceneViewMatrix" );
		_sceneProjectionMatrixUniformLocation = glGetUniformLocation( _renderingShaderProgram->_program, "uSceneProjectionMatrix" );
	
		// Update flag
		isUniformUnitialized = true;
	}
	//----------------------------------------------------------------------------------------------------------

	glBindVertexArray( vao );
	glEnable( GL_PROGRAM_POINT_SIZE );

	glUniformMatrix4fv( _sceneModelMatrixUniformLocation, 1/*count*/, GL_FALSE/*GL_TRUE*//*transpose*/, pModelMatrix._array );
	glUniformMatrix4fv( _sceneViewMatrixUniformLocation, 1/*count*/, GL_FALSE/*GL_TRUE*//*transpose*/, pViewMatrix._array );
	glUniformMatrix4fv( _sceneProjectionMatrixUniformLocation, 1/*count*/, GL_FALSE/*GL_TRUE*//*transpose*/, pProjectionMatrix._array );

	debugDisplay( 0/*depth*/, make_uint3( NodeTileRes::numElements, 0, 0 )/*address*/, make_float3( 0.0f )/*pos*/, make_float3( 1.0f )/*size*/ );

	glDisable( GL_PROGRAM_POINT_SIZE );
	glBindVertexArray( 0 );
	GsGraphics::GsShaderProgram::unuse();

//	glPolygonMode( GL_FRONT_AND_BACK , GL_FILL);
}

/******************************************************************************
 * Used to display the N-tree
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::syncDebugVolTree()
{
	if ( ! _childArraySync )
	{
		_childArraySync = new GvCore::Array3D< uint >( _childArray->getResolution(), GvCore::Array3D< uint >::StandardHeapMemory );
	}

	if ( ! _dataArraySync )
	{
		_dataArraySync = new GvCore::Array3D< uint >( _dataArray->getResolution(), GvCore::Array3D< uint >::StandardHeapMemory );
	}

	// TO DO : asynchronous copy
	memcpyArray( _childArraySync, _childArray );
	memcpyArray( _dataArraySync, _dataArray );

	//dumpOctree(_childArraySync->getPointer(), 8);
}

/******************************************************************************
 * Get the appearance of the N-tree (octree) of the data structure
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::getDataStructureAppearance( bool& pShowNodeHasBrickTerminal, bool& pShowNodeHasBrickNotTerminal, bool& pShowNodeIsBrickNotInCache, bool& pShowNodeEmptyOrConstant,
	float4& pNodeHasBrickTerminalColor, float4& pNodeHasBrickNotTerminalColor, float4& pNodeIsBrickNotInCacheColor, float4& pNodeEmptyOrConstantColor  ) const
{
	pShowNodeHasBrickTerminal = _showNodeHasBrickTerminal;
	pShowNodeHasBrickNotTerminal = _showNodeHasBrickNotTerminal;
	pShowNodeIsBrickNotInCache = _showNodeIsBrickNotInCache;
	pShowNodeEmptyOrConstant = _showNodeEmptyOrConstant;

	pNodeHasBrickTerminalColor = _nodeHasBrickTerminalColor;
	pNodeHasBrickNotTerminalColor = _nodeHasBrickNotTerminalColor;
	pNodeIsBrickNotInCacheColor = _nodeIsBrickNotInCacheColor;
	pNodeEmptyOrConstantColor = _nodeEmptyOrConstantColor;
}
	
/******************************************************************************
 * Set the appearance of the N-tree (octree) of the data structure
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::setDataStructureAppearance( bool pShowNodeHasBrickTerminal, bool pShowNodeHasBrickNotTerminal, bool pShowNodeIsBrickNotInCache, bool pShowNodeEmptyOrConstant,
	const float4& pNodeHasBrickTerminalColor, const float4& pNodeHasBrickNotTerminalColor, const float4& pNodeIsBrickNotInCacheColor, const float4& pNodeEmptyOrConstantColor )
{
	_showNodeHasBrickTerminal = pShowNodeHasBrickTerminal;
	_showNodeHasBrickNotTerminal = pShowNodeHasBrickNotTerminal;
	_showNodeIsBrickNotInCache = pShowNodeIsBrickNotInCache;
	_showNodeEmptyOrConstant = pShowNodeEmptyOrConstant;

	_nodeHasBrickTerminalColor = pNodeHasBrickTerminalColor;
	_nodeHasBrickNotTerminalColor = pNodeHasBrickNotTerminalColor;
	_nodeIsBrickNotInCacheColor = pNodeIsBrickNotInCacheColor;
	_nodeEmptyOrConstantColor = pNodeEmptyOrConstantColor;
}
	
/******************************************************************************
 * Used to display the N-tree
 *
 * @param depth Depth
 * @param address Address
 * @param pos Position
 * @param size Size
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::debugDisplay( uint depth, const uint3& address, const float3& pos, const float3& size )
{
	// TO DO : avoid temp copies...
	GsNode node;
	node = getOctreeNodeSync( address );

	// If node has sub nodes AND we have not reached the max depth resolution
	if ( node.hasSubNodes() && depth < _maxDepth )
	{
		float3 subsize = size / NodeTileRes::getFloat3();

		for( int k=0; k<NodeTileRes::z; k++ )
			for( int j=0; j<NodeTileRes::y; j++ )
				for( int i=0; i<NodeTileRes::x; i++ )
				{
					float3 subpos = pos + make_float3( (float)i, (float)j, (float)k ) * subsize;
					debugDisplay( depth + 1, node.getChildAddress() + make_uint3( i + j * NodeTileRes::x + k * NodeTileRes::x * NodeTileRes::y, 0, 0 ), subpos, subsize );
				}

				// DARK GREEN
				//glColor4f( 0.1f, 0.7f, 0.1f, 0.5f );
				glUniform4f( _colorUniformLocation, 0.1f, 0.7f, 0.1f, 0.5f );
	}
	else
	{
		// If node has brick, it means that the associated brick has been produced and placed in GPU memory
		if ( node.hasBrick() )
		{
			if ( node.isTerminal() )
			{
				if ( _showNodeHasBrickTerminal )
				{
					//glColor4f( _nodeHasBrickTerminalColor.x, _nodeHasBrickTerminalColor.y, _nodeHasBrickTerminalColor.z, _nodeHasBrickTerminalColor.w );
					glUniform4f( _colorUniformLocation, _nodeHasBrickTerminalColor.x, _nodeHasBrickTerminalColor.y, _nodeHasBrickTerminalColor.z, _nodeHasBrickTerminalColor.w );
					drawCube( pos, pos + size );
				}
			}
			else
			{
				if ( _showNodeHasBrickNotTerminal )
				{
					//glColor4f( _nodeHasBrickNotTerminalColor.x, _nodeHasBrickNotTerminalColor.y, _nodeHasBrickNotTerminalColor.z, _nodeHasBrickNotTerminalColor.w );
					glUniform4f( _colorUniformLocation, _nodeHasBrickNotTerminalColor.x, _nodeHasBrickNotTerminalColor.y, _nodeHasBrickNotTerminalColor.z, _nodeHasBrickNotTerminalColor.w );
					drawCube( pos, pos + size );
				}
			}
		}
		// If node is a brick, it means that it holds data but the associated brick of voxels is not in GPU memory (i.e. not yet produced/loaded or removed from cache)
		else if ( node.isBrick() )
		{
			if ( _showNodeIsBrickNotInCache )
			{
				//glColor4f( _nodeIsBrickNotInCacheColor.x, _nodeIsBrickNotInCacheColor.y, _nodeIsBrickNotInCacheColor.z, _nodeIsBrickNotInCacheColor.w );
				glUniform4f( _colorUniformLocation, _nodeIsBrickNotInCacheColor.x, _nodeIsBrickNotInCacheColor.y, _nodeIsBrickNotInCacheColor.z, _nodeIsBrickNotInCacheColor.w );
				drawCube( pos, pos + size );
			}
		}
		else
		{
			// Empty node
			if ( _showNodeEmptyOrConstant )
			{
				//glColor4f( _nodeEmptyOrConstantColor.x, _nodeEmptyOrConstantColor.y, _nodeEmptyOrConstantColor.z, _nodeEmptyOrConstantColor.w );
				glUniform4f( _colorUniformLocation, _nodeEmptyOrConstantColor.x, _nodeEmptyOrConstantColor.y, _nodeEmptyOrConstantColor.z, _nodeEmptyOrConstantColor.w );
				drawCube( pos, pos + size );
			}
		}

		//drawCube( pos, pos + size );
	}
}

/******************************************************************************
 * Used to display the N-tree
 *
 * @param p1 Position
 * @param p2 Position
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::drawCube( const float3& p1, const float3& p2 )
{
	//glBegin(GL_QUADS);

	//// Front Face
	//glVertex3f(p1.x, p1.y, p2.z);	// Bottom Left Of The Texture and Quad
	//glVertex3f(p1.x, p2.y, p2.z);	// Top Left Of The Texture and Quad
	//glVertex3f(p2.x, p2.y, p2.z);	// Top Right Of The Texture and Quad
	//glVertex3f(p2.x, p1.y, p2.z);	// Bottom Right Of The Texture and Quad

	//// Back Face
	//glVertex3f(p1.x, p1.y, p1.z);	// Bottom Right Of The Texture and Quad
	//glVertex3f(p2.x, p1.y, p1.z);	// Bottom Left Of The Texture and Quad
	//glVertex3f(p2.x, p2.y, p1.z);	// Top Left Of The Texture and Quad
	//glVertex3f(p1.x, p2.y, p1.z);	// Top Right Of The Texture and Quad

	//// Top Face
	//glVertex3f(p1.x, p2.y, p1.z);	// Top Left Of The Texture and Quad
	//glVertex3f(p2.x, p2.y, p1.z);	// Top Right Of The Texture and Quad
	//glVertex3f(p2.x, p2.y, p2.z);	// Bottom Right Of The Texture and Quad
	//glVertex3f(p1.x, p2.y, p2.z);	// Bottom Left Of The Texture and Quad

	//// Bottom Face
	//glVertex3f(p1.x, p1.y, p1.z);	// Top Right Of The Texture and Quad
	//glVertex3f(p1.x, p1.y, p2.z);	// Bottom Right Of The Texture and Quad
	//glVertex3f(p2.x, p1.y, p2.z);	// Bottom Left Of The Texture and Quad
	//glVertex3f(p2.x, p1.y, p1.z);	// Top Left Of The Texture and Quad

	//// Right face
	//glVertex3f(p2.x, p1.y, p1.z);	// Bottom Right Of The Texture and Quad
	//glVertex3f(p2.x, p1.y, p2.z);	// Bottom Left Of The Texture and Quad
	//glVertex3f(p2.x, p2.y, p2.z);	// Top Left Of The Texture and Quad
	//glVertex3f(p2.x, p2.y, p1.z);	// Top Right Of The Texture and Quad

	//// Left Face
	//glVertex3f(p1.x, p1.y, p1.z);	// Bottom Left Of The Texture and Quad
	//glVertex3f(p1.x, p2.y, p1.z);	// Top Left Of The Texture and Quad
	//glVertex3f(p1.x, p2.y, p2.z);	// Top Right Of The Texture and Quad
	//glVertex3f(p1.x, p1.y, p2.z);	// Bottom Right Of The Texture and Quad

	//glEnd();

	/*static bool isCubeInitialized = false;
	if ( ! isCubeInitialized )
	{
		const std::string vertexShader = "";
		GLuint GsShaderManager::createShader( const char* pFileName, GLuint pShaderType, GLuint pShaderID )

		isCubeInitialized = true;
	}*/

	// Draw a cube procedurally

	//----------------------------------------------------------------------------------------------------------

	/*const GLfloat modelMatrix[ 16 ] = {
		p2.x,  0.f,  0.f, p1.x,
		 0.f, p2.y,  0.f, p1.y,
		 0.f,  0.f, p2.z, p1.z,
		 0.f,  0.f,  0.f, 1.f
	};*/
	const GLfloat modelMatrix[ 16 ] = {
		p2.x - p1.x,  0.f,  0.f, 0.f,
		 0.f, p2.y - p1.y,  0.f, 0.f,
		 0.f,  0.f, p2.z - p1.z, 0.f,
		p1.x, p1.y, p1.z, 1.f
	};
	glUniformMatrix4fv( _modelMatrixUniformLocation, 1/*count*/, GL_FALSE/*GL_TRUE*//*transpose*/, modelMatrix );
	//----------------------------------------------------------------------------------------------------------

	glDrawArrays( GL_LINE_STRIP, 0, 14 );
	//glDrawArrays( GL_POINTS, 0, 14 );
}

/******************************************************************************
 * Used to display the N-tree
 *
 * @param offset Offset
 *
 * @return ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
GsNode GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::getOctreeNodeSync( const uint3& offset )
{
	GsNode node;
	node.childAddress = _childArraySync->get( offset );
	node.brickAddress = _dataArraySync->get( offset );

	return node;
}

/******************************************************************************
 * This method is used to serialize a pool
 *
 * @param pStream the stream from which to read
 ******************************************************************************/
template< typename TDataTypeList, unsigned int TChannelIndex, typename TDataPool >
void writeDataChannel( const TDataPool* pDataPool, std::ostream& pStream )
{
	assert( pDataPool != NULL );
	if ( pDataPool != NULL )
	{
		// Allocate temporary host array
		typedef typename GvCore::DataChannelType< TDataTypeList, TChannelIndex >::Result dataType;
		GvCore::Array3D< dataType >* dataArray = new GvCore::Array3D< dataType >( pDataPool->getChannel( Loki::Int2Type< TChannelIndex >() )->getResolution(), GvCore::Array3D< dataType >::StandardHeapMemory );
		
		// Copy data from device to host
		memcpyArray( dataArray, pDataPool->getChannel( Loki::Int2Type< TChannelIndex >() ) );
		
		// Serialize data
		pStream.write( reinterpret_cast< const char* >( dataArray->data() ), sizeof( dataType ) * dataArray->getNumElements() );
		
		// Free temporary host array
		delete dataArray;
	}
}

/**
 * Functor used to serialize/deserialize data pool of a data structure
 */
template< typename TDataTypeList, typename TDataPool >
struct GvDataPoolSerializer
{
	/**
	 * ...
	 */
	TDataPool _dataPool;

	/**
	 * Generalized functor method used to bound textures.
	 *
	 * @param Loki::Int2Type< i > channel
	 */
	template< int i >
	inline void run( Loki::Int2Type< i > )
	{
		//const TDataPool* dataPool = NULL;
		//std::ostream& stream;
		//writeDataChannel< TDataTypeList, Loki::Int2Type< i >(), TDataPool >( _dataPool, stream );
	}
};

/******************************************************************************
 * This method is called to serialize an object
 *
 * @param pStream the stream where to write
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
inline void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::write( std::ostream& pStream ) const
{
	// -------- Node pool serialization --------

	// - node info
	GvCore::Array3D< uint >* nodeTileInfo = new GvCore::Array3D< uint >( _childArray->getResolution(), GvCore::Array3D< uint >::StandardHeapMemory );
	memcpyArray( nodeTileInfo, _childArray );
	pStream.write( reinterpret_cast< const char* >( nodeTileInfo->getPointer() ), sizeof( uint ) * nodeTileInfo->getNumElements() );
	delete nodeTileInfo;

	// - data info
	GvCore::Array3D< uint >* nodeDataInfo = new GvCore::Array3D< uint >( _dataArray->getResolution(), GvCore::Array3D< uint >::StandardHeapMemory );
	memcpyArray( nodeDataInfo, _dataArray );
	pStream.write( reinterpret_cast< const char* >( nodeDataInfo->getPointer() ), sizeof( uint ) * nodeDataInfo->getNumElements() );
	delete nodeDataInfo;

	// -------- Data pool serialization --------

//	// - data
////	for ( uint i = 0; i < DataPoolType::numChannels; i++ )
////	{
//		std::cout << "Channel #" << 0 << std::endl;
//		typedef typename GvCore::DataChannelType< DataTList, 0 >::Result dataType0;
//		GvCore::Array3D< dataType0 >* dataArray0 = new GvCore::Array3D< dataType0 >( _dataPool->getChannel( Loki::Int2Type< 0 >() )->getResolution(), GvCore::Array3D< dataType0 >::StandardHeapMemory );
//		memcpyArray( dataArray0, _dataPool->getChannel( Loki::Int2Type< 0 >() ) );
//		pStream.write( reinterpret_cast< const char* >( dataArray0->getPointer() ), sizeof( dataType0 ) * dataArray0->getNumElements() );
//		delete dataArray0;
//
//		std::cout << "Channel #" << 1 << std::endl;
//		typedef typename GvCore::DataChannelType< DataTList, 1 >::Result dataType1;
//		GvCore::Array3D< dataType1 >* dataArray1 = new GvCore::Array3D< dataType1 >( _dataPool->getChannel( Loki::Int2Type< 1 >() )->getResolution(), GvCore::Array3D< dataType1 >::StandardHeapMemory );
//		memcpyArray( dataArray1, _dataPool->getChannel( Loki::Int2Type< 1 >() ) );
//		pStream.write( reinterpret_cast< const char* >( dataArray1->getPointer() ), sizeof( dataType1 ) * dataArray1->getNumElements() );
//		delete dataArray1;
////	}
	//GvDataPoolSerializer< DataTList, DataPoolType > dataPoolSerializerfunctor( this );
	//GvCore::StaticLoop< GvDataPoolSerializer< DataTList, DataPoolType >, Loki::TL::Length< DataTList >::value - 1 >::go( dataPoolSerializerfunctor );

	// -------- Localization information --------
	
	// - localization depth
	GvCore::Array3D< GvCore::GsLocalizationInfo::DepthType >* localizationDepth = new GvCore::Array3D< GvCore::GsLocalizationInfo::DepthType >( _localizationDepthArray->getResolution(), GvCore::Array3D< GvCore::GsLocalizationInfo::DepthType >::StandardHeapMemory );
	memcpyArray( localizationDepth, _localizationDepthArray );
	pStream.write( reinterpret_cast< const char* >( localizationDepth->getPointer() ), sizeof( GvCore::GsLocalizationInfo::DepthType ) * localizationDepth->getNumElements() );
	delete localizationDepth;

	// - localization code
	GvCore::Array3D< GvCore::GsLocalizationInfo::CodeType >* localizationCode = new GvCore::Array3D< GvCore::GsLocalizationInfo::CodeType >( _localizationCodeArray->getResolution(), GvCore::Array3D< GvCore::GsLocalizationInfo::CodeType >::StandardHeapMemory );
	memcpyArray( localizationCode, _localizationCodeArray );
	pStream.write( reinterpret_cast< const char* >( localizationCode->getPointer() ), sizeof( GvCore::GsLocalizationInfo::CodeType ) * localizationCode->getNumElements() );
	delete localizationCode;
}

/******************************************************************************
 * This method is called deserialize an object
 *
 * @param pStream the stream from which to read
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize, typename TDataStructureKernelType >
inline void GsVolumeTree< DataTList, NodeTileRes, BrickRes, BorderSize, TDataStructureKernelType >
::read( std::istream& pStream )
{
	std::ifstream file2( "fifthgrade.txt", std::ios::binary );
	if ( file2.is_open() )
	{
		GvCore::Array3D< uint >* nodeTileArray = new GvCore::Array3D< uint >( _childArray->getResolution(), GvCore::Array3D< uint >::StandardHeapMemory );
		file2.read( reinterpret_cast< char* >( nodeTileArray->getPointer() ), sizeof( uint ) * nodeTileArray->getNumElements() );
		//for ( size_t i = 0; i < nodeTileArray->getNumElements(); i++ )
		//{
		//	std::cout << nodeTileArray->get( i ) << std::endl;
		//}
		//_childArray->fill( 0 );
		memcpyArray( _childArray, nodeTileArray );
		delete nodeTileArray;
		file2.close();
	}
}

} // namespace GvStructure
