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

#ifndef _GS_DATA_TYPE_HANDLER_H_
#define _GS_DATA_TYPE_HANDLER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// System
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
 * @class GsDataTypeHandler
 *
 * @brief The GsDataTypeHandler class provides methods to deal with data type
 * used with GigaVoxels.
 *
 * It handles memory allocation and memory localization of data in buffers.
 */
class GIGASPACE_EXPORT GsDataTypeHandler
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Voxel data type enumeration that can be handled during voxelization
	 */
	typedef enum
	{
		gvUCHAR,
		gvUCHAR4,
		gvUSHORT,
		gvFLOAT,
		gvFLOAT4
	}
	VoxelDataType;

	/******************************* ATTRIBUTES *******************************/
	
	/******************************** METHODS *********************************/

	/**
     * Retrieve the number of bytes representing a given data type
	 *
	 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
	 *
	 * @return the number of bytes representing the data type
     */
	static unsigned int canalByteSize( VoxelDataType pDataType );

	/**
     * Allocate memory associated to a number of elements of a given data type
	 *
	 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
	 * @param pNbElements the number of elements to allocate
	 *
	 * @return a pointer on the allocated memory space
     */
	static void* allocateVoxels( VoxelDataType pDataType, unsigned int pNbElements );

	/**
     * Retrieve the address of an element of a given data type in an associated buffer
	 *
	 * Note : this is used to retrieve voxel addresses in brick data buffers.
	 *
	 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
	 * @param pDataBuffer a buffer associated to elements of the given data type
	 * @param pElementPosition the position of the element in the associated buffer
	 *
	 * @return the address of the element in the buffer
     */
	static void* getAddress( VoxelDataType pDataType, void* pDataBuffer, unsigned int pElementPosition );

	/**
     * Retrieve the name representing a given data type
	 *
	 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
	 *
	 * @return the name of the data type
     */
	static std::string getTypeName( VoxelDataType pDataType );

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

}

#endif
