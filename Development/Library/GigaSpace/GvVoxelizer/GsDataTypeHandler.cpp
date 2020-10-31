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

#include "GvVoxelizer/GsDataTypeHandler.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// System 
#include <cassert>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GvVoxelizer
using namespace GvVoxelizer;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Retrieve the number of bytes representing a given data type
 *
 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
 *
 * @return the number of bytes representing the data type
 ******************************************************************************/
unsigned int GsDataTypeHandler::canalByteSize( VoxelDataType pDataType )
{
	unsigned int result = 0;

	switch ( pDataType )
	{
		case gvUCHAR:
			result = sizeof( unsigned char );
			break;

		case gvUCHAR4:
			result = 4 * sizeof( unsigned char );
			break;

		case gvUSHORT:
			result = sizeof( unsigned short );
			break;

		case gvFLOAT:
			result = sizeof( float );
			break;

		case gvFLOAT4:
			result = 4 * sizeof( float );
			break;

		default:
			// TO DO
			// Handle error
			assert( false );
			break;
	}

	return result;
}

/******************************************************************************
 * Allocate memory associated to a number of elements of a given data type
 *
 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
 * @param pNbElements the number of elements to allocate
 *
 * @return a pointer on the allocated memory space
 ******************************************************************************/
void* GsDataTypeHandler::allocateVoxels( VoxelDataType pDataType, unsigned int pNbElements )
{
	void* result = NULL;

	switch ( pDataType )
	{
//		case gvUCHAR4:
//			result = new unsigned char[ 4 * pNbElements ];
//			break;
//
//		case gvFLOAT:
//			result = new float[ pNbElements ];
//			break;
//
//		case gvFLOAT4:
//			result = new float[ 4 * pNbElements ];
//			break;


				case gvUCHAR:
                        result = operator new( sizeof(unsigned char) * pNbElements );
                        break;

                case gvUCHAR4:
                        result = operator new( sizeof(unsigned char) * 4 * pNbElements );
                        break;

				case gvUSHORT:
                        result = operator new( sizeof(unsigned short) * pNbElements );
                        break;

                case gvFLOAT:
                        result = operator new( sizeof(float) * pNbElements );
                        break;

                case gvFLOAT4:
                        result = operator new( sizeof(float) * 4 * pNbElements );
                        break;

		default:
			// TO DO
			// Handle error
			assert( false );
			break;
	}

	return result;
}

/******************************************************************************
 * Retrieve the address of an element of a given data type in an associated buffer
 *
 * Note : this is used to retrieve voxel addresses in brick data buffers.
 *
 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
 * @param pDataBuffer a buffer associated to elements of the given data type
 * @param pElementPosition the position of the element in the associated buffer
 *
 * @return the address of the element in the buffer
 ******************************************************************************/
void* GsDataTypeHandler::getAddress( VoxelDataType pDataType, void* pDataBuffer, unsigned int pElementPosition )
{
	void* result = NULL;

	switch ( pDataType )
	{
		case gvUCHAR:
			result = &( static_cast< unsigned char* >( pDataBuffer )[ pElementPosition ] );
			break;

		case gvUCHAR4:
			result = &( static_cast< unsigned char* >( pDataBuffer )[ 4 * pElementPosition ] );
			break;

		case gvUSHORT:
			result = &( static_cast< unsigned short* >( pDataBuffer )[ pElementPosition ] );
			break;

		case gvFLOAT:
			result = &(static_cast< float* >( pDataBuffer )[ pElementPosition ]);
			break;

		case gvFLOAT4:
			result = &(static_cast< float* >( pDataBuffer )[ 4 * pElementPosition ] );
			break;

		default:
			// TO DO
			// Handle error
			assert( false );
			break;
	}

	return result;
}

/******************************************************************************
 * Retrieve the name representing a given data type
 *
 * @param pDataType a data type (i.e. uchar4, float, float4, etc...)
 *
 * @return the name of the data type
 ******************************************************************************/
std::string GsDataTypeHandler::getTypeName( VoxelDataType pDataType )
{
	std::string result;

	switch( pDataType )
	{
		case gvUCHAR:
			result = std::string( "uchar" );
			break;

		case gvUCHAR4:
			result = std::string( "uchar4" );
			break;

		case gvUSHORT:
			result = std::string( "ushort" );
			break;

		case gvFLOAT:
			result = std::string( "float" );
			break;

		case gvFLOAT4:
			result = std::string( "float4" );
			break;

		default:
			// TO DO
			// Handle error
			assert( false );
			break;
	}

	return result;
}
