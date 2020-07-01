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

#ifndef _GV_PAGE_TABLE_KERNEL_H_
#define _GV_PAGE_TABLE_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsLocalizationInfo.h"

// TO DO : attention ce KERNEL n'est pas dans un namespace...
/******************************************************************************
 * KERNEL : CreateLocalizationLists
 *
 * Extract localization informations associated with a list of given elements.
 *
 * Special version for gpuProducerLoadCache that transform loccodes to usable ones (warning, loose one bit of depth !)
 *
 * @param pNbElements number of elements to process
 * @param pLoadAddressList List of input node addresses
 * @param pLocCodeList List of localization code coming from the main page table of the data structure and referenced by cache managers (nodes and bricks)
 * @param pLocDepthList List of localization depth coming from the main page table of the data structure and referenced by cache managers (nodes and bricks)
 * @param pResLocCodeList Resulting output localization code list
 * @param pResLocDepthList Resulting output localization depth list
 ******************************************************************************/
template< class NodeTileRes >
__global__
void CreateLocalizationLists( const uint pNbElements, const uint* pLoadAddressList, 
							 const GvCore::GsLocalizationInfo::CodeType* pLocCodeList, const GvCore::GsLocalizationInfo::DepthType* pLocDepthList,
							 GvCore::GsLocalizationInfo::CodeType* pResLocCodeList, GvCore::GsLocalizationInfo::DepthType* pResLocDepthList );

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @struct PageTableKernel
 *
 * @brief The PageTableKernel struct provides...
 *
 * @ingroup GvCore
 *
 * This is the base class for all gpu page table implementations.
 */
template< typename Derived, typename AddressType, typename KernelArrayType >
struct PageTableKernel
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * This method returns the LocalizationInfo structure associated with the given
	 * node address.
	 *
	 * @param nodeAddress ...
	 *
	 * @return ...
	 */
	__device__
	__forceinline__ GsLocalizationInfo getLocalizationInfo( uint nodeAddress ) const;

	/**
	 * This method should...
	 *
	 * @param elemAddress ...
	 * @param elemPointer ...
	 * @param flag ...
	 */
	__device__
	__forceinline__ void setPointer( uint elemAddress, uint3 elemPointer, uint flag = 0 );

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
 * @struct PageTableNodesKernel
 *
 * @brief The PageTableNodesKernel struct provides...
 *
 * @ingroup GvCore
 *
 * ...
 */
template
<
	typename NodeTileRes, typename ElementRes,
	typename AddressType, typename KernelArrayType,
	typename LocCodeArrayType, typename LocDepthArrayType
>
struct PageTableNodesKernel : public PageTableKernel< PageTableNodesKernel< NodeTileRes, ElementRes, AddressType, KernelArrayType, LocCodeArrayType, LocDepthArrayType >,
	AddressType, KernelArrayType >
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	typedef AddressType								ElemType;
	typedef typename AddressType::AddressType		ElemAddressType;
	typedef typename AddressType::PackedAddressType	ElemPackedAddressType;

	/******************************* ATTRIBUTES *******************************/

	KernelArrayType		childArray;
	LocCodeArrayType	locCodeArray;
	LocDepthArrayType	locDepthArray;

	/******************************** METHODS *********************************/

	// FIXME: Move into the parent class
	/**
	 * Return the localization info of a node in the node pool
	 *
	 * @param nodeAddress Address of the node in the node pool
	 *
	 * @return The localization info of the node
	 */
	__device__
	__forceinline__ GsLocalizationInfo getLocalizationInfoImpl( uint nodeAddress ) const;

	/**
	 * ...
	 *
	 * @param elemAddress ...
	 * @param elemPointer ...
	 * @param flags ...
	 */
	__device__
	__forceinline__ void setPointerImpl( uint elemAddress, ElemAddressType elemPointer, uint flags = 0 );

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
 * @struct PageTableBricksKernel
 *
 * @brief The PageTableBricksKernel struct provides...
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
struct PageTableBricksKernel : public PageTableKernel< PageTableBricksKernel< NodeTileRes, ChildAddressType, ChildKernelArrayType,
	DataAddressType, DataKernelArrayType, LocCodeArrayType, LocDepthArrayType >, DataAddressType, DataKernelArrayType >
{
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	typedef DataAddressType									ElemType;
	typedef typename DataAddressType::AddressType			ElemAddressType;
	typedef typename DataAddressType::PackedAddressType		ElemPackedAddressType;

	//typedef typename ChildAddressType::AddressType			UnpackedChildAddressType;
	typedef typename ChildAddressType::PackedAddressType	PackedChildAddressType;

	/******************************* ATTRIBUTES *******************************/

	ChildKernelArrayType	childArray;
	DataKernelArrayType		dataArray;
	LocCodeArrayType		locCodeArray;
	LocDepthArrayType		locDepthArray;

	/******************************** METHODS *********************************/

	// FIXME: Move into the parent class
	/**
	 * Return the localization info of a node in the node pool
	 *
	 * @param nodeAddress Address of the node in the node pool
	 *
	 * @return The localization info of the node
	 */
	__device__
	__forceinline__ GsLocalizationInfo getLocalizationInfoImpl( uint nodeAddress ) const;

	/**
	 * ...
	 *
	 * @param ...
	 * @param ...
	 * @param flags this vlaue is retrieves from Producer::produceData< 1 > methods)
	 */
	__device__
	__forceinline__ void setPointerImpl( uint elemAddress, ElemAddressType elemPointer, uint flags = 0 );

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

#include "GsPageTableKernel.inl"

#endif // !_GV_PAGE_TABLE_KERNEL_H_
