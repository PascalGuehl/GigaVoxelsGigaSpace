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

#ifndef _GV_TRANSFER_FUNCTION_H_
#define _GV_TRANSFER_FUNCTION_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// Cuda
#include <vector_types.h>
#include <driver_types.h>

// STL
#include <string>

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

namespace GvUtils
{

/** 
 * @class GsTransferFunction
 *
 * @brief The GsTransferFunction class provides an implementation
 * of a transfer function on the device.
 *
 * Transfer function is a mathematical tool used tu map an input to an output.
 * In computer graphics, a volume renderer can use it to map a sampled density
 * value to an RGBA value.
 */
class GIGASPACE_EXPORT GsTransferFunction
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsTransferFunction();

	/**
	 * Destructor
	 */
	virtual ~GsTransferFunction();

	/**
	 * Get the filename
	 *
	 * @return the filename
	 */
	const std::string& getFilename() const;

	/**
	 * Set the filename
	 *
	 * @param pName the filename
	 */
	void setFilename( const std::string& pName );

	/**
	 * Get the resolution
	 *
	 * @return the resolution
	 */
	unsigned int getResolution() const;

	///**
	// * Set the resolution
	// *
	// * @param pValue the resolution
	// */
	//void setResolution( unsigned int pValue );

	/**
	 * Get the transfer function's data
	 *
	 * @return the transfer function's data
	 */
	inline const float4* getData() const;

	/**
	 * Get the transfer function's data
	 *
	 * @return the transfer function's data
	 */
	inline float4* editData();

	/**
	 * Create the transfer function
	 *
	 * @param pResolution the dimension of the transfer function
	 */
	bool create( unsigned int pResolution );

	/**
	 * Update device memory
	 */
	void updateDeviceMemory();

	/**
	 * Bind the internal data to a specified texture
	 * that can be used to fetch data on device.
	 *
	 * @param pTexRefName name of the texture reference to bind
	 * @param pNormalizedAccess indicates whether texture reads are normalized or not
	 * @param pFilterMode type of texture filter mode
	 * @param pAddressMode type of texture access mode
	 */
	void bindToTextureReference( const void* pSymbol, const char* pTexRefName, bool pNormalizedAccess, cudaTextureFilterMode pFilterMode, cudaTextureAddressMode pAddressMode );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Transfer function file name
	 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
	std::string _filename;
#if defined _MSC_VER
#pragma warning( pop )
#endif

	/**
	 * Transfer function data
	 */
	float4* _data;

	/**
	 * Transfer function resolution
	 */
	unsigned int _resolution;

	/**
	 * Transfer function data in CUDA memory space
	 */
	cudaArray* _dataArray;

	/**
	 * Channel format descriptor
	 */
	cudaChannelFormatDesc _channelFormatDesc;
	
	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsTransferFunction( const GsTransferFunction& );

	/**
	 * Copy operator forbidden.
	 */
	GsTransferFunction& operator=( const GsTransferFunction& );

};

}

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsTransferFunction.inl"

#endif
