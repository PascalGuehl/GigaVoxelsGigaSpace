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

#ifndef _GV_PAGE_TABLE_H_
#define _GV_PAGE_TABLE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsPageTableKernel.h"

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
 * @struct PageTable
 *
 * @brief The PageTable struct provides localisation information of all elements of the data structure
 *
 * @ingroup GvCore
 *
 * It is used to retrieve localization info (code + depth), .i.e 3D world position of associated node's region in space,
 * from nodes stored in the Cache Management System.
 *
 * @param NodeTileRes Node tile resolution
 * @param LocCodeArrayType Type of array storing localization code (ex : GsLinearMemory< LocalizationInfo::CodeType >)
 * @param LocDepthArrayType Type of array storing localization depth (ex : GsLinearMemory< LocalizationInfo::DepthType >)
 */
template< typename NodeTileRes, typename LocCodeArrayType, typename LocDepthArrayType >
struct PageTable
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Localization code array
	 *
	 * Global localization codes of all nodes of the data structure (for the moment, its a reference)
	 */
	LocCodeArrayType* locCodeArray;

	/**
	 * Localization depth array
	 *
	 * Global localization codes of all nodes of the data structure (for the moment, its a reference)
	 */
	LocDepthArrayType* locDepthArray;

	/******************************** METHODS *********************************/

	/**
	 * Create localization info list (code and depth)
	 *
	 * Given a list of N nodes, it retrieves their localization info (code + depth)
	 *
	 * @param pNumElems number of elements to process (i.e. nodes)
	 * @param pNodesAddressCompactList a list of nodes from which to retrieve localization info
	 * @param pResLocCodeList the resulting localization code array of all requested elements
	 * @param pResLocDepthList the resulting localization depth array of all requested elements
	 */
	inline void createLocalizationLists( uint pNumElems, uint* pNodesAddressCompactList,
										thrust::device_vector< GsLocalizationInfo::CodeType >* pResLocCodeList,
										thrust::device_vector< GsLocalizationInfo::DepthType >* pResLocDepthList );

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

} // namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @struct PageTableNodes
 *
 * @brief The PageTableNodes struct provides a page table specialized to handle a node pool
 *
 * @ingroup GvCore
 *
 * ...
 *
 * TIPS : extract from file VolumeTreeCache.h :
 * typedef PageTableNodes< NodeTileRes, NodeTileResLinear,
 *		VolTreeNodeAddress,	GsLinearMemoryKernel< uint >,
 *		LocCodeArrayType, LocDepthArrayType > NodePageTableType;
 */
template
<
	typename NodeTileRes, typename ElementRes,
	typename AddressType, typename KernelArrayType,
	typename LocCodeArrayType, typename LocDepthArrayType
>
struct PageTableNodes : public PageTable< NodeTileRes, LocCodeArrayType, LocDepthArrayType >
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/**
	 * Type definition for the associated device-side object
	 */
	typedef PageTableNodesKernel
	<
		NodeTileRes, ElementRes,
		AddressType, KernelArrayType,
		typename LocCodeArrayType::KernelArrayType,
		typename LocDepthArrayType::KernelArrayType
	>
	KernelType;

	/******************************* ATTRIBUTES *******************************/

	/**
	 * The associated device-side object
	 */
	KernelType pageTableKernel;

	/******************************** METHODS *********************************/

	/**
	 * Get the associated device-side object
	 *
	 * @return the associated device-side object
	 */
	inline KernelType& getKernel();
		
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

} // namespace GvCore

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @struct PageTableBricks
 *
 * @brief The PageTableBricks struct provides a page table specialized to handle a brick pool (i.e data)
 *
 * @ingroup GvCore
 *
 * ...
 */
template
<
	typename NodeTileRes, 
	typename ChildAddressType, typename ChildKernelArrayType,
	typename DataAddressType, typename DataKernelArrayType,
	typename LocCodeArrayType, typename LocDepthArrayType
>
struct PageTableBricks : public PageTable< NodeTileRes, LocCodeArrayType, LocDepthArrayType >
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/**
	 * Type definition for the associated device-side object
	 */
	typedef PageTableBricksKernel
	<
		NodeTileRes,
		ChildAddressType, ChildKernelArrayType,
		DataAddressType, DataKernelArrayType,
		typename LocCodeArrayType::KernelArrayType,
		typename LocDepthArrayType::KernelArrayType
	>
	KernelType;
	
	/******************************* ATTRIBUTES *******************************/

	/**
	 * The associated device-side object
	 */
	KernelType pageTableKernel;

	/******************************** METHODS *********************************/

	/**
	 * Get the associated device-side object
	 *
	 * @return the associated device-side object
	 */
	inline KernelType& getKernel();

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

} // namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsPageTable.inl"

#endif // !_GV_PAGE_TABLE_H_
