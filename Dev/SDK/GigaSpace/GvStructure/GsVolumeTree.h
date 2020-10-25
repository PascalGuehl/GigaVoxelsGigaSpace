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

#ifndef _GV_VOLUME_TREE_H_
#define _GV_VOLUME_TREE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// Cuda
#include <vector_types.h>

// Cuda SDK
#include <helper_math.h>

// Thrust
#include <thrust/device_vector.h>

// GigaVoxels
#include "GvStructure/GsIDataStructure.h"
#include "GvCore/GsVector.h"
#include "GvPerfMon/GsPerformanceMonitor.h"
#include "GvStructure/GsVolumeTreeKernel.h"		// TO DO : remove it because of template !!
#include "GvStructure/GsNode.h"
#include "GvCore/GsArray.h"
#include "GvCore/GsLinearMemory.h"
#include "GvCore/GsDeviceTexturingMemory.h"
#include "GvCore/GsPool.h"
#include "GvCore/GsLocalizationInfo.h"
#include "GvCore/GsRendererTypes.h"

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
namespace GsGraphics
{
	class GsShaderProgram;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvStructure
{

/** 
 * @struct GsVolumeTree
 *
 * @brief The GsVolumeTree struct provides a generalized N-Tree data structure
 *
 * Volume Tree encapsulates nodes and bricks data.
 *
 * - Nodes are used for space partitioning strategy. There are organized in node tiles
 * basee on their node tile resolution. Octree is the more common organization
 * (2x2x2 nodes by node tile). N-Tree represents a hierarchical structure containg
 * multi-resolution pyramid of data.
 * - Bricks are used to store user defined data as color, normal, density, etc...
 * Data is currently stored in 3D textures. In each node, we have one brick of voxels
 * based on its brick resolution (ex : 8x8x8 voxels by brick).
 *
 * Nodes and bricks are organized in pools that ara managed by a cahe mecanism.
 * LRU mecanism (Least recently Used) is used to efficiently store data in device memory.
 *
 * For each type of data defined by the user, the brick pool stores a 3D texture that
 * can be read and/or write. It corresponds to a channel in the pool.
 *
 * @param DataTList Data type list provided by the user
 * (exemple with a normal and a color by voxel : typedef Loki::TL::MakeTypelist< uchar4, half4 >::Result DataType;)
 * @param NodeTileRes Node tile resolution
 * @param NodeTileRes Brick resolution
 * @param BorderSize Brick border size (1 for the moment)
 * @param TDataStructureKernelType data structure device-side associated object (ex : GvStructure::VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >)
 *
 * @todo : see how to handle a border superior than and if it can be useful
 */
template
<
	class DataTList, class NodeTileRes, class BrickRes, uint BorderSize,
	typename TDataStructureKernelType
>
struct GsVolumeTree : public GsIDataStructure
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	//typedef GsLinearMemory NodeArrayType;

	/**
	 * Typedef used to describe a node type element
	 * It holds two addresses : one for node, one for brick
	 */
#ifndef GS_USE_NODE_META_DATA
	typedef typename Loki::TL::MakeTypelist< /*node address*/uint, /*data address*/uint >::Result NodeTList;
#else
	typedef typename Loki::TL::MakeTypelist< /*node address*/uint, /*data address*/uint, /*meta-data*/uint >::Result NodeTList;
#endif

	/**
	 * Typedef for the Volume Tree on GPU side
	 */
	typedef TDataStructureKernelType VolTreeKernelType;

	/**
	 * Type definition for the node tile resolution
	 */
	typedef NodeTileRes NodeTileResolution;

	/**
	 * Type definition for the brick resolution
	 */
	typedef BrickRes BrickResolution;

	/**
	 * Enumeration to define the brick border size
	 */
	enum
	{
		BrickBorderSize = BorderSize
	};

	/**
	 * Defines the total size of a brick
	 */
	typedef GvCore::GsVec1D< BrickResolution::x + 2 * BrickBorderSize > FullBrickResolution;	// TO DO : NodeTileResolution::x (problem ?)

	/**
	 * Defines the data type list
	 */
	typedef DataTList DataTypeList;

	/**
	 * Type definition of the node pool type
	 */
	typedef GvCore::GPUPoolHost< GvCore::GsLinearMemory, NodeTList > NodePoolType;

	/**
	 * Type definition of the data pool type
	 */
	typedef GvCore::GPUPoolHost< GvCore::GsDeviceTexturingMemory, DataTList > DataPoolType;

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Helper object : it is a reference on nodes in the node pool
	 */
	GvCore::GsLinearMemory< uint >* _childArray;

	/**
	 * Helper object : it is a reference on bricks in the node pool
	 */
	GvCore::GsLinearMemory< uint >* _dataArray;

#ifdef GS_USE_NODE_META_DATA
	/**
	 * Helper object : it is a reference on related node meta data in the node pool
	 */
	GvCore::GsLinearMemory< uint >* _metaDataArray;
#endif

	/**
	 * Node pool
	 * It is implemented as an GsLinearMemory, i.e linear memory
	 */
	NodePoolType* _nodePool;

	/**
	 * Brick pool (i.e data pool)
	 * It is implemented as an GsDeviceTexturingMemory, i.e a 3D texture
	 * There is one 3D texture for each element in the data type list DataTList defined by the user
	 */
	DataPoolType* _dataPool;

	/**
	 * Localization code array
	 *
	 * @todo The creation of the localization arrays should be moved in the Cache Management System, not in the data structure (this is cache implementation details/features)
	 */
	GvCore::GsLinearMemory< GvCore::GsLocalizationInfo::CodeType >* _localizationCodeArray;

	/**
	 * Localization depth array
	 *
	 * @todo The creation of the localization arrays should be moved in the Cache Management System, not in the data structure (this is cache implementation details/features)
	 */
	GvCore::GsLinearMemory< GvCore::GsLocalizationInfo::DepthType >* _localizationDepthArray;

	/**
	 * Volume tree on GPU side
	 */
	VolTreeKernelType volumeTreeKernel;

	/******************************** METHODS *********************************/

	/**
	 * Constructor.
	 *
	 * @param nodesCacheSize Cache size used to store nodes
	 * @param bricksCacheRes Cache size used to store bricks
	 * @param graphicsInteroperability Flag used for graphics interoperability
	 */
	GsVolumeTree( const uint3& nodesCacheSize, const uint3& bricksCacheRes, uint graphicsInteroperability = 0 );

	/**
	 * Destructor.
	 */
	virtual ~GsVolumeTree();

	/**
	 * Cuda specific initialization
	 */
	void cuda_Init();

	/**
	 * Clear the volume tree information
	 * It clears the node pool and its associated localization info
	 */
	void clearVolTree();

	/**
	 * Get the max depth of the volume tree
	 *
	 * @return max depth
	 */
	uint getMaxDepth() const;

	/**
	 * Set the max depth of the volume tree
	 *
	 * @param maxDepth Max depth
	 */
	void setMaxDepth( uint maxDepth );

	/**
	 * Debugging helpers
	 */
	void render();
	void render( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );

	/**
	 * Get the node tile resolution.
	 *
	 * @return the node tile resolution
	 */
	const NodeTileRes& getNodeTileResolution() const;

	/**
	 * Get the brick resolution (voxels).
	 *
	 * @param the brick resolution
	 */
	const BrickRes& getBrickResolution() const;

	/**
	 * Get the appearance of the N-tree (octree) of the data structure
	 */
	void getDataStructureAppearance( bool& pShowNodeHasBrickTerminal, bool& pShowNodeHasBrickNotTerminal, bool& pShowNodeIsBrickNotInCache, bool& pShowNodeEmptyOrConstant,
										float4& pNodeHasBrickTerminalColor, float4& pNodeHasBrickNotTerminalColor, float4& pNodeIsBrickNotInCacheColor, float4& pNodeEmptyOrConstantColor ) const;

	/**
	 * Set the appearance of the N-tree (octree) of the data structure
	 */
	void setDataStructureAppearance( bool pShowNodeHasBrickTerminal, bool pShowNodeHasBrickNotTerminal, bool pShowNodeIsBrickNotInCache, bool pShowNodeEmptyOrConstant,
										const float4& pNodeHasBrickTerminalColor, const float4& pNodeHasBrickNotTerminalColor, const float4& pNodeIsBrickNotInCacheColor, const float4& pNodeEmptyOrConstantColor );
	
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

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Node tile resolution
	 */
	NodeTileRes _nodeTileResolution;

	/**
	 * Brick resolution (voxels)
	 */
	BrickRes _brickResolution;

	/**
	 * Used to display the N-tree
	 */
	GvCore::Array3D< uint >* _childArraySync;
		
	/**
	 * Used to display the N-tree
	 */
	GvCore::Array3D< uint >* _dataArraySync;

	/**
	 * Used to display the N-tree. This is the max possible depth of the stucture.
	 */
	uint _maxDepth;

	/**
	 * Data structure appearance
	 */
	bool _showNodeHasBrickTerminal;
	bool _showNodeHasBrickNotTerminal;
	bool _showNodeIsBrickNotInCache;
	bool _showNodeEmptyOrConstant;
	float4 _nodeHasBrickTerminalColor;
	float4 _nodeHasBrickNotTerminalColor;
	float4 _nodeIsBrickNotInCacheColor;
	float4 _nodeEmptyOrConstantColor;

	/**
	 * Shader program
	 */
	GsGraphics::GsShaderProgram* _renderingShaderProgram;
	
	/**
	 * For debugging purpose
	 */
	unsigned int _modelMatrixUniformLocation;
	unsigned int _colorUniformLocation;

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	//! Debugging helpers

	/**
	 * Used to display the N-tree
	 */
	void syncDebugVolTree();

	/**
	 * Used to display the N-tree
	 *
	 * @param depth Depth
	 * @param address Address
	 * @param pos Position
	 * @param size Size
	 */
	void debugDisplay( uint depth, const uint3& address, const float3& pos, const float3& size );

	/**
	 * Used to display the N-tree
	 *
	 * @param p1 Position
	 * @param p2 Position
	 */
	void drawCube( const float3& p1, const float3& p2 );

	/**
	 * Used to display the N-tree
	 *
	 * @param offset Offset
	 *
	 * @return ...
	 */
	GsNode getOctreeNodeSync( const uint3& offset );

	/**
	 * Copy constructor forbidden.
	 */
	GsVolumeTree( const GsVolumeTree& );

	/**
	 * Copy operator forbidden.
	 */
	GsVolumeTree& operator=( const GsVolumeTree& );

};

} // namespace GvStructure

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsVolumeTree.inl"

#endif // !_GV_VOLUME_TREE_H_
