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

#include "GvUtils/GsTransferFunction.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsError.h"

// Cuda
#include <cuda_runtime.h>

// STL
#include <iostream>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GvVoxelizer
using namespace GvUtils;

// STL
using namespace std;

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
 * Constructor
 ******************************************************************************/
GsTransferFunction::GsTransferFunction()
:	_filename()
,	_data( NULL )
,	_resolution( 0 )
,	_dataArray( NULL )
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsTransferFunction::~GsTransferFunction()
{
	// Free host memory
	delete [] _data;

	// Free device memory
	GS_CUDA_SAFE_CALL( cudaFreeArray( _dataArray ) );
}

/******************************************************************************
 * 3D model file name
 ******************************************************************************/
const std::string& GsTransferFunction::getFilename() const
{
	return _filename;
}

/******************************************************************************
 * 3D model file name
 ******************************************************************************/
void GsTransferFunction::setFilename( const std::string& pName )
{
	_filename = pName;
}
	
/******************************************************************************
 * Data resolution
 ******************************************************************************/
unsigned int GsTransferFunction::getResolution() const
{
	return _resolution;
}

///******************************************************************************
// * Data resolution
// ******************************************************************************/
//void GsTransferFunction::setResolution( unsigned int pValue )
//{
//	_resolution = pValue;
//}

/******************************************************************************
 * Create the transfer function
 *
 * @param pResolution the dimension of the transfer function
 ******************************************************************************/
bool GsTransferFunction::create( unsigned int pResolution )
{
	bool result = false;

	_resolution = pResolution;

	// Allocate data in host memory
	_data = new float4[ _resolution ];

	// Allocate CUDA array in device memory
	_channelFormatDesc = cudaCreateChannelDesc< float4 >();
	GS_CUDA_SAFE_CALL( cudaMallocArray( &_dataArray, &_channelFormatDesc, _resolution, 1 ) );

	result = true;

	return result;
}

/******************************************************************************
 * Update device memory
 ******************************************************************************/
void GsTransferFunction::updateDeviceMemory()
{
	// Copy to device memory some data located at address _data in host memory
	cudaMemcpyToArray( _dataArray, 0, 0, _data, _resolution * sizeof( float4 ), cudaMemcpyHostToDevice );
}

/******************************************************************************
 * Bind the internal data to a specified texture
 * that can be used to fetch data on device.
 *
 * @param pTexRefName name of the texture reference to bind
 * @param pNormalizedAccess indicates whether texture reads are normalized or not
 * @param pFilterMode type of texture filter mode
 * @param pAddressMode type of texture access mode
 ******************************************************************************/
void GsTransferFunction::bindToTextureReference( const void* pSymbol, const char* pTexRefName, bool pNormalizedAccess, cudaTextureFilterMode pFilterMode, cudaTextureAddressMode pAddressMode )
{
	std::cout << "bindToTextureReference : " << pTexRefName << std::endl;

	textureReference* texRefPtr;
	GS_CUDA_SAFE_CALL( cudaGetTextureReference( (const textureReference **)&texRefPtr, pSymbol ) );

	texRefPtr->normalized = pNormalizedAccess; // Access with normalized texture coordinates
	texRefPtr->filterMode = pFilterMode;
	texRefPtr->addressMode[ 0 ] = pAddressMode; // Wrap texture coordinates
	texRefPtr->addressMode[ 1 ] = pAddressMode;
	texRefPtr->addressMode[ 2 ] = pAddressMode;

	// Bind array to 3D texture
	GS_CUDA_SAFE_CALL( cudaBindTextureToArray( (const textureReference *)texRefPtr, _dataArray, &_channelFormatDesc ) );
}
