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

#ifndef GVLOCALIZATIONINFO_H
#define GVLOCALIZATIONINFO_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"

// Cuda
#include <vector_types.h>
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

namespace GvCore
{

/** 
 * @class GvLocalizationCode
 *
 * @brief The GvLocalizationCode class provides the concept used to retrieve
 * a node in an N-tree (spatial position) based on its address in the node pool
 * (GPU memory).
 *
 * @ingroup GvCore
 *
 * Localization code deals with data requests. It concepts in wrapped in the
 * global localization info mecanism that works in conjonction with localization depth.
 *
 * To each node, we associate a code, which we call localization code
 * that encodes the node's position in the N-tree and is stored in three times
 * 10 bits, grouped in a single 32-bits integer. Each 10 bits represents one axis.
 * Bit by bit, this series encodes a sequence of space subdivisions, so basically
 * a descent in the octree. More precisely, the nth-bit of the first 10 bit value
 * represents the child taken on the X axis at level n. Each bit represents the
 * choice (left or right child) along this axis. This is similar to the descent in
 * the N-tree from top-to-bottom.
 *
 * Code and depth localization values describe exactly one node in an N-tree
 * subdivision structure. This allows to derive exactly what information needs
 * to be loaded or produced.
 *
 * @see GsLocalizationInfo GvLocalizationDepth
 */
class GvLocalizationCode
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/
	
	/**
	 * Type definition of the localization code value
	 */
	typedef uint3 ValueType;

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Set the localization code value
	 *
	 * @param plocalizationCode The localization code value
	 */
	__host__ __device__
	inline void set( ValueType plocalizationCode );

	/**
	 * Get the localization code value
	 *
	 * @return The localization code value
	 */
	__host__ __device__
	inline ValueType get() const;

	/**
	 * Given an offset position in a node tile,
	 * this
	 *
	 * @param pOffset The offset in a node tile
	 *
	 * @return ...
	 */
	template< typename TNodeTileResolution >
	__host__ __device__
	inline GvLocalizationCode addLevel( ValueType pOffset ) const;

	/** Given an offset position in a node tile,
	 * this
	 *
	 * @return The localization code of the node tile at coarser level
	 */
	template< typename TNodeTileResolution >
	__host__ __device__
	inline GvLocalizationCode removeLevel() const;

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

	/**
	 * Localization code value
	 */
	ValueType _localizationCode;

	/******************************** METHODS *********************************/

};

} // namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @class GvLocalizationDepth
 *
 * @brief The GvLocalizationDepth class provides the concept used to retrieve
 * a node in an N-tree (spatial position) based on its address in the node pool
 * (GPU memory).
 *
 * @ingroup GvCore
 *
 * Localization depth deals with data requests. It concepts in wrapped in the
 * global localization info mecanism that works in conjonction with localization code.
 *
 * Each node stores a localization depth value in form of a 32-bits integer.
 * It encodes how deep in the tree the node is located.
 *
 * A localization depth of n means that only the first n bits of the localization
 * code are needed to reach the node in the tree.
 *
 * Code and depth localization values describe exactly one node in an N-tree
 * subdivision structure. This allows to derive exactly what information needs
 * to be loaded or produced.
 *
 * @see GsLocalizationInfo GvLocalizationCode
 */
class GvLocalizationDepth
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/
	
	/**
	 * Type definition of the localization depth value
	 */
	typedef uint ValueType;

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Get the localization depth value
	 *
	 * @return The localization depth value
	 */
	__host__ __device__
	inline ValueType get() const;

	/**
	 * Set the localization depth value
	 *
	 * @param pLocalizationDepth The localization depth value
	 */
	__host__ __device__
	inline void set( ValueType pLocalizationDepth );

	/**
	 * ...
	 *
	 * @return ...
	 */
	__host__ __device__
	inline GvLocalizationDepth addLevel() const;

	/**
	 * ...
	 *
	 * @return ...
	 */
	__host__ __device__
	inline GvLocalizationDepth removeLevel() const;

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
	
	/**
	 * Localization depth value
	 */
	ValueType _localizationDepth;

	/******************************** METHODS *********************************/

};

} // namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @struct GsLocalizationInfo
 *
 * @brief The GsLocalizationInfo struct provides the concept used to retrieve
 * a node in an N-tree (spatial position) based on its address in the node pool
 * (GPU memory).
 *
 * @ingroup GvCore
 *
 * Localization info deals with data requests.
 *
 * Data requests correspond to missing data. This information has to be
 * loaded on the CPU or produced on the GPU (or CPU side).
 * Thus, the data request list needs to be sent to the Producer.
 *
 * Producers needs two kinds of information in order to update the data:
 * what to load and where to store it. Focusing on what to load, what we need
 * is information about the spatial extent of the node addressed in the data
 * request. Only this localization information allows to fetch the corresponding
 * data (from the disk or procedurally generated).
 *
 * Unfortunately, there is no relationship between the address of a node,
 * which is contained in the data requests and the spatial extent it represents.
 * In fact, due to the cache mechanism, the organization of the nodes in
 * the node pool can be arbitrary and it has nothing to do with the actual
 * scene.
 *
 * To be able to provide the information about the spatial organization,
 * GigaVoxels stores two arrays in GPU memory:
 * - to each node, we associate a code, which we call localization code
 * that encodes the node's position in the octree.
 * - each node will also store a localization depth value that encodes
 * how deep in the tree the node is located.
 *
 * Code and depth localization values describe exactly one node in an N-tree
 * subdivision structure. This allows to derive exactly what information needs
 * to be loaded or produced.
 *
 * @see GvLocalizationCode GvLocalizationDepth
 */
struct GsLocalizationInfo
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/**
	 * Max possible depth.
	 * Locked to 32 due to the localization info mecanism:
	 * - code and depth encode a descent in the N-tree along each axis.
	 * 
	 * @todo Should it be here ? 
	 */
	enum
	{
		maxDepth = 31
	};

	/**
	 * Type definition for localization code
	 */
	typedef GvLocalizationCode CodeType;

	/**
	 * Type definition for localization depth
	 */
	typedef GvLocalizationDepth DepthType;

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Localization code
	 */
	CodeType locCode;

	/**
	 * Localization depth
	 */
	DepthType locDepth;

	/******************************** METHODS *********************************/

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

	/******************************** METHODS *********************************/

};

} // namespace GvCore

//struct LocalizationInfo{
//	enum{maxDepth=31};
//
//	typedef LocalizationCode	CodeType;
//	typedef uchar				DepthType;
//
//	///Depth
//	__device__ __host__
//	static uint getDepth(DepthType d);
//
//	__device__ __host__
//	static uchar getDepthUCHAR(DepthType d);
//
//	__device__ __host__
//	static DepthType setDepth(uint d);
//
//	__device__ __host__
//	static DepthType addLevelDepth(DepthType d);
//
//	__device__ __host__
//	static DepthType removeLevelDepth(DepthType d);
//};

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsLocalizationInfo.inl"

#endif
