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

#ifndef _GS_DATA_STRUCTURE_IO_HANDLER_H_
#define _GS_DATA_STRUCTURE_IO_HANDLER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvVoxelizer/GsDataTypeHandler.h"

// STL
#include <vector>
#include <string>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvVoxelizer
{

/** 
 * GsDataStructureIOHandler ...
 */
class GIGASPACE_EXPORT GsDataStructureIOHandler
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/
	
	/******************************* ATTRIBUTES *******************************/

	/**
	 * Maximum level of resolution
	 */
	const unsigned int _level;

	/**
	 * Node grid size of the underlying data structure (i.e. octree, N-Tree, etc...).
	 * This is the number of nodes in each dimension.
	 */
	const unsigned int _nodeGridSize;

	/**
	 * Voxel grid size of the underlying data structure (i.e. octree, N-Tree, etc...).
	 * This is the number of voxels in each dimension.
	 */
	const unsigned int _voxelGridSize;

	/**
	 * Brick width.
	 * This is the number of voxels in each dimension of a brick
	 * of the underlying data structure (i.e. octree, N-Tree, etc...).
	 */
	const unsigned int _brickWidth;

	/**
	 * Brick size.
	 * This is the total number of voxels in a brick by taking to account borders.
	 * Currently, there is only a border of one voxel on each side of bricks.
	 */
	const unsigned int _brickSize;	
	
	/******************************** METHODS *********************************/

	/**
     * Constructor
	 *
	 * @param pName Name of the data (.i.e. sponza, dragon, sibenik, etc...)
	 * @param pLevel level of resolution of the data structure
	 * @param pBrickWidth width of bricks in the data structure
	 * @param pDataType type of voxel data (i.e. uchar4, float, float4, etc...)
	 * @param pNewFiles a flag telling whether or not "new files" are used
	 */
	GsDataStructureIOHandler( const std::string& pName, 
								unsigned int pLevel,
								unsigned int pBrickWidth,
								GsDataTypeHandler::VoxelDataType pDataType,
								bool pNewFiles );

	/**
     * Constructor
	 *
	 * @param pName Name of the data (.i.e. sponza, dragon, sibenik, etc...)
	 * @param pLevel level of resolution of the data structure
	 * @param pBrickWidth width of bricks in the data structure
	 * @param pDataTypes types of voxel data (i.e. uchar4, float, float4, etc...)
	 * @param pNewFiles a flag telling whether or not "new files" are used
	 */
	GsDataStructureIOHandler( const std::string& pName, 
								unsigned int pLevel,
								unsigned int pBrickWidth,
								const std::vector< GsDataTypeHandler::VoxelDataType >& pDataTypes,
								bool pNewFiles );

	/**
     * Destructor
	 */
	virtual ~GsDataStructureIOHandler();
	
	/**
	 * Get the node info associated to an indexed node position
	 *
	 * @param pNodePos an indexed node position
	 *
	 * @return node info (address + brick index)
	 */
	unsigned int getNode( unsigned int nodePos[ 3 ] );

	/**
	 * Set data in a voxel at given data channel
	 *
	 * @param pVoxelPos voxel position
	 * @param pVoxelData voxel data
	 * @param pDataChannel data channel index
	 * @param pInputDataSize size of the input data (in bytes), it may be smaller 
	 * than the type of the channel. If 0, use sizeof( channel ) instead.
	 */
	void setVoxel( unsigned int pVoxelPos[ 3 ], const void* pVoxelData, unsigned int pDataChannel, unsigned int pInputDataSize = 0 );

	/**
	 * Set data in a voxel at given data channel
	 *
	 * @param pNormalizedVoxelPos float normalized voxel position
	 * @param pVoxelData voxel data
	 * @param pDataChannel data channel index
	 * @param pInputDataSize size of the input data (in bytes), it may be smaller 
	 * than the type of the channel. If 0, use sizeof( channel ) instead.
	 */
	void setVoxel( float pNormalizedVoxelPos[ 3 ], const void* pVoxelData, unsigned int pDataChannel, unsigned int pInputDataSize = 0 );

	/**
	 * Get data in a voxel at given data channel
	 *
	 * @param pVoxelPos voxel position
	 * @param pVoxelData voxel data
	 * @param pDataChannel data channel index
	 */
	void getVoxel( unsigned int pVoxelPos[ 3 ], void* voxelData, unsigned int pDataChannel );

	/**
	 * Get data in a voxel at given data channel
	 *
	 * @param pNormalizedVoxelPos float normalized voxel position
	 * @param pVoxelData voxel data
	 * @param pDataChannel data channel index
	 */
	void getVoxel( float pNormalizedVoxelPos[ 3 ], void* pVoxelData, unsigned int pDataChannel );

	/**
	 * Get the current brick number
	 *
	 * @return the current brick number
	 */
	unsigned int getBrickNumber() const;

	/**
	 * Get brick data in a node at given data channel
	 *
	 * @param pNodePos node position
	 * @param pBrickData brick data
	 * @param pDataChannel data channel index
	 */
	void getBrick( unsigned int pNodePos[ 3 ], void* pBrickData, unsigned int pDataChannel );

	/**
	 * Set brick data in a node at given data channel
	 *
	 * @param pNodePos node position
	 * @param pBrickData brick data
	 * @param pDataChannel data channel index
	 */
	void setBrick( unsigned int pNodePos[ 3 ], void* pBrickData, unsigned int pDataChannel );

	/**
	 * Get the voxel size at current level of resolution
	 *
	 * @return the voxel size
	 */
	float getVoxelSize() const;

	/** @name Position functions
	 *  Position functions
	 */
	/**@{*/

	/**
	 * Convert a normalized node position to its indexed node position
	 *
	 * @param pNormalizedNodePos normalized node position
	 * @param pNodePos indexed node position
	 */
	void getNodePosition(  float pNormalizedNodePos[ 3 ], unsigned int pNodePos[ 3 ] );

	/**
	 * Convert a normalized voxel position to its indexed voxel position
	 *
	 * @param pNormalizedVoxelPos normalized voxel position
	 * @param pVoxelPos indexed voxel position
	 */
	void getVoxelPosition( float pNormalizedVoxelPos[ 3 ], unsigned int pVoxelPos[ 3 ] );

	/**
	 * Convert a normalized voxel position to its indexed voxel position in its associated brick
	 *
	 * @param pNormalizedVoxelPos normalized voxel position
	 * @param pVoxelPosInBrick indexed voxel position in its associated brick
	 */
	void getVoxelPositionInBrick( float pNormalizedVoxelPos[ 3 ], unsigned int pVoxelPosInBrick[ 3 ] );
	
	/**@}*/

	/**
	 * Fill all brick borders of the data structure with data.
	 */
	void computeBorders();

	/**
	 * Tell whether or not a node is empty given its node info.
	 *
	 * @param pNode a node info
	 *
	 * @return a flag telling whether or not a node is empty
	 */
	static bool isEmpty( unsigned int pNode );
	
	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/
	
	/** @name Files
	 *  Files
	 */
	/**@{*/

	/**
	 * Node file
	 */
	FILE* _nodeFile;

	/**
	 * List of brick files
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< FILE* > _brickFiles;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Node filename
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _fileNameNode;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * List of brick filenames
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< std::string > _fileNamesBrick;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * List of data types associated to the data structure channels (unsigned char, unsigned char4, float, float4, etc...)
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< GsDataTypeHandler::VoxelDataType > _dataTypes;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**@}*/

	/**
	 * Real bricks number (empty regions contain no brick)
	 */
	unsigned int _brickNumber;

	// current brick and node

	/**
	 * Flag to tell whether or not the current node and brick have been loaded in memory (and stored in buffers)
	 */
	bool _isBufferLoaded;

	/**
	 * Buffer of node position.
	 * It corresponds to the current indexed node position.
	 */
	unsigned int _nodeBufferPos[ 3 ];

	/**
	 * Buffer of node info associated to current node position.
	 * It corresponds to the childAddress of an GvStructure::GsNode.
	 * If node is not empty, the associated brick index is also stored inside.
	 */
	unsigned int _nodeBuffer;

	/**
	 * Brick data buffer associated to current nodeBuffer.
	 * This is where all data reside for each channel (color, normal, etc...)
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::vector< void* > _brickBuffers;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Empty node flag
	 */
	static const unsigned int _cEmptyNodeFlag;

	/******************************** METHODS *********************************/	

	/**
	 * Retrieve node info and brick data associated to a node position.
	 * Data is retrieved from disk if not already in cache, otherwise exit.
	 *
	 * Data is stored in buffers if not yet in cache.
	 *
	 * Note : Data is written on disk a previous node have been processed
	 * and a new one is requested.
	 *
	 * @param pNodePos node position
	 */
	void loadNodeandBrick( unsigned int pNodePos[ 3 ] );

	/**
	 * Save node info and brick data associated to current node position on disk.
	 */
	void saveNodeandBrick();

	/**
	 * Initialize all the files that will be generated.
	 *
	 * Note : Associated brick buffer(s) will be created/initialized.
	 *
	 * @param pName name of the data (i.e. sponza, sibenik, dragon, etc...)
	 * @param pNewFiles a flag telling whether or not new files are used
	 */
	void openFiles( const std::string& name, bool newFiles );

	/**
	 * Retrieve the node file name.
	 * An example of GigaVoxels node file could be : "fux_BR8_B1_L0.nodes"
	 * where "fux" is the name, "8" is the brick width BR, "1" is the brick border size B,
	 * "0" is the level of resolution L and "nodes" is the file extension.
	 *
	 * @param pName name of the data file
	 * @param pLevel data structure level of resolution
	 * @param pBrickWidth width of bricks
	 *
	 * @return the node file name in GigaVoxels format.
	 */
	static std::string getFileNameNode( const std::string& pName, unsigned int pLevel, unsigned int pBrickWidth );

	/**
	 * Retrieve the brick file name.
	 * An example of GigaVoxels brick file could be : "fux_BR8_B1_L0_C0_uchar4.bricks"
	 * where "fux" is the name, "8" is the brick width BR, "1" is the brick border size B,
	 * "0" is the level of resolution L, "0" is the data channel index C,
	 * "uchar4" the data type name and "bricks" is the file extension.
	 *
	 * @param pName name of the data file
	 * @param pLevel data structure level of resolution
	 * @param pBrickWidth width of bricks
	 * @param pDataChannelIndex data channel index
	 * @param pDataTypeName data type name
	 *
	 * @return the brick file name in GigaVoxels format.
	 */
	static std::string getFileNameBrick( const std::string& pName, unsigned int pLevel, unsigned int pBrickWidth, unsigned int pDataChannelIndex, const std::string& pDataTypeName );

	/**
	 * Create a brick node info (address + brick index)
	 *
	 * @param pBrickNumber a brick index
	 *
	 * @return a brick node info
	 */
	static unsigned int createBrickNode( unsigned int pBrickNumber );

	/**
	 * Retrieve the brick offset of a brick given a node info.
	 *
	 * @param pNode a node info
	 *
	 * @return the brick offset
	 */
	static unsigned int getBrickOffset( unsigned int pNode );

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
	GsDataStructureIOHandler( const GsDataStructureIOHandler& );

	/**
	 * Copy operator forbidden.
	 */
	GsDataStructureIOHandler& operator=( const GsDataStructureIOHandler& );

};

}

#endif
