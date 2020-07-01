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

#ifndef _GV_NODE_H_
#define _GV_NODE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"

// CUDA
#include <host_defines.h>

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

namespace GvStructure
{

/** 
 * @struct GsNode
 *
 * @brief The GsNode struct provides the interface to nodes of data structures (N3-tree, etc...)
 *
 * @ingroup GvStructure
 *
 * The GsNode struct holds :
 * - the address of its child nodes in cache (the address of its first child nodes organized in tiles)
 * - and the address of its associated brick of data in cache
 *
 * @todo: Rename OctreeNode as GsNode or GvDataStructureNode.
 * @todo: Rename functions isBrick() hasBrick() (old naming convention when we had constant values).
 */
struct GsNode
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Address to its child nodes
	 *
	 * It is encoded (i.e. packed)
	 */
	uint childAddress;

	/**
	 * Address to its associated brick of data
	 *
	 * It is encoded (i.e. packed)
	 */
	uint brickAddress;

#ifdef GV_USE_BRICK_MINMAX
	uint metaDataAddress;
#endif

#ifdef GS_USE_NODE_META_DATA
	/**
	 * Meta data
	 */
	uint _metaData;
#endif

	/******************************** METHODS *********************************/

	/**
	 * Unpack a node address
	 *
	 * @param pAddress a packed address of a node in cache
	 *
	 * @return the associated unpacked address
	 */
	__host__ __device__
	static __forceinline__ uint3 unpackNodeAddress( const uint pAddress );

	/**
	 * Pack a node address
	 *
	 * @param pAddress ...
	 *
	 * @return the associated packed address
	 */
	__host__ __device__
	static __forceinline__ uint packNodeAddress( const uint3 pAddress );

	/**
	 * Unpack a brick address
	 *
	 * @param pAddress a packed address of a brick of data in cache
	 *
	 * @return the associated unpacked address
	 */
	__host__ __device__
	static __forceinline__ uint3 unpackBrickAddress( const uint pAddress );

	/**
	 * Pack a brick address
	 *
	 * @param pAddress ...
	 *
	 * @return the associated packed address
	 */
	__host__ __device__
	static __forceinline__ uint packBrickAddress( const uint3 pAddress );

	/** @name Child Nodes Managment
	 *
	 *  Child nodes managment methods
	 */
	///@{

	/**
	 * Set the child nodes address
	 *
	 * @param dpcoord ...
	 */
	__host__ __device__
	__forceinline__ void setChildAddress( const uint3 dpcoord );

	/**
	 * Get the child nodes address
	 *
	 * @return ...
	 */
	__host__ __device__
	__forceinline__ uint3 getChildAddress() const;

	/**
	 * Set the child nodes encoded address
	 *
	 * @param addr ...
	 */
	__host__ __device__
	__forceinline__ void setChildAddressEncoded( uint addr );

	/**
	 * Get the child nodes encoded address
	 *
	 * @return ...
	 */
	__host__ __device__
	__forceinline__ uint getChildAddressEncoded() const;

	/**
	 * Tell wheter or not the node has children
	 *
	 * @return a flag telling wheter or not the node has children
	 */
	__host__ __device__
	__forceinline__ bool hasSubNodes() const;

	/**
	 * Flag the node as beeing terminal or not
	 *
	 * @param pFlag a flag telling wheter or not the node is terminal
	 */
	__host__ __device__
	__forceinline__ void setTerminal( bool pFlag );

	/**
	 * Tell wheter or not the node is terminal
	 *
	 * @return a flag telling wheter or not the node is terminal
	 */
	__host__ __device__
	__forceinline__ bool isTerminal() const;

	///@}

	/** @name Bricks Managment
	 *
	 *  Bricks managment methods
	 */
	///@{

	/**
	 * Set the brick address
	 *
	 * @param dpcoord ...
	 */
	__host__ __device__
	__forceinline__ void setBrickAddress( const uint3 dpcoord );

	/**
	 * Get the brick address
	 *
	 * @return ...
	 */
	__host__ __device__
	__forceinline__ uint3 getBrickAddress() const;

	/**
	 * Set the brick encoded address
	 *
	 * @param addr ...
	 */
	__host__ __device__
	__forceinline__ void setBrickAddressEncoded( const uint addr );

	/**
	 * Get the brick encoded address
	 *
	 * @return ...
	 */
	__host__ __device__
	__forceinline__ uint getBrickAddressEncoded() const;

	/**
	 * Flag the node as containg data or not
	 *
	 * @param pFlag a flag telling wheter or not the node contains data
	 */
	__host__ __device__
	__forceinline__ void setStoreBrick();

	/**
	 * Tell wheter or not the node is a brick
	 *
	 * @return a flag telling wheter or not the node is a brick
	 */
	__host__ __device__
	__forceinline__ bool isBrick() const;

	/**
	 * Tell wheter or not the node has a brick,
	 * .i.e the node is a brick and its brick address is not null.
	 *
	 * @return a flag telling wheter or not the node has a brick
	 */
	__host__ __device__
	__forceinline__ bool hasBrick() const;

	///@}

	/** @name Initialization State
	 *
	 *  Initialization state
	 */
	///@{

	/**
	 * Tell wheter or not the node is initializated
	 *
	 * @return a flag telling wheter or not the node is initializated
	 */
	__host__ __device__
	__forceinline__ bool isInitializated() const;

	///@}

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

};

} //namespace GvStructure

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsNode.inl"

#endif // !_GV_NODE_H_
