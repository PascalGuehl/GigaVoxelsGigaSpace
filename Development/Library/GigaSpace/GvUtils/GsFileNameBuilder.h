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

#ifndef GV_FILENAME_BUILDER_H
#define GV_FILENAME_BUILDER_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GvCore/GsCoreConfig.h"

// STL
#include <vector>

// System
#include <string>
#include <sstream>

// Loki
#include <loki/Typelist.h>

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
 * @struct GsFileNameBuilder
 *
 * @brief The GsFileNameBuilder struct provides...
 *
 * ...
 */
template< typename TDataTypeList >
class GsFileNameBuilder
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 *
	 * @param brickSize Brick size
	 * @param borderSize Border size
	 * @param level Level
	 * @param fileName File name
	 * @param fileExt File extension
	 * @param result List of built filenames
	 */
	inline GsFileNameBuilder( uint pBrickSize, uint pBorderSize, uint pLevel, const std::string& pFileName,
							const std::string& pFileExt, std::vector< std::string >& pResult );

	/**
	 * ...
	 *
	 * @param Loki::Int2Type< TChannel > ...
	 */
	template< int TChannel >
	inline void run( Loki::Int2Type< TChannel > );

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

	/**
	 * Brick size
	 */
	uint mBrickSize;

	/**
	 * Border size
	 */
	uint mBorderSize;

	/**
	 * Level of resolution
	 */
	uint mLevel;

	/**
	 * File name
	 */
	std::string mFileName;

	/**
	 * File extension
	 */
	std::string mFileExt;

	/**
	 * List of built filenames
	 */
	std::vector< std::string >* mResult;

	/******************************** METHODS *********************************/

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsFileNameBuilder.inl"

#endif // GV_FILENAME_BUILDER_H
