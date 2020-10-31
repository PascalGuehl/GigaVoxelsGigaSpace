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

#ifndef _GS_I_READER_H_
#define _GS_I_READER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GvCore/GsCoreConfig.h"

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
 * @class GsIReader
 *
 * @brief The GsIReader class provides an interface to read meta data of a GigaSpace model
 */
class GIGASPACE_EXPORT GsIReader
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * File info
	 */
	static const char* _cModelFileExtension;
	static const char* _cNodeFileExtension;
	static const char* _cBrickFileExtension;
	static const char* _cFileSymbolSeperator;
	static const char* _cBrickWidthSymbol;
	static const char* _cBrickBorderSizeSymbol;
	static const char* _cLODIndexSymbol;
	static const char* _cDataChannelIndexSymbol;
			
	/**
	 * Model info
	 */
	static const char* _cModelElementName;
	static const char* _cModelDirectoryAttributeName;
	static const char* _cModelNbLODAttributeName;

	/**
	 * Node Tree info
	 */
	static const char* _cNodeTreeElementName;

	/**
	 * Brick Data info
	 */
	static const char* _cBrickDataElementName;
	static const char* _cBrickResolutionAttributeName;
	static const char* _cBrickBorderSizeAttributeName;
	static const char* _cBrickDataChannelElementName;
	static const char* _cBrickDataTypeAttributeName;

	/**
	 * Generic info
	 */
	static const char* _cNameAttributeName;
	static const char* _cIdAttributeName;
	static const char* _cFilenameAttributeName;
	static const char* _cLODElementName;

	/******************************** METHODS *********************************/

	/**
	 * Destructor
	 */
	virtual ~GsIReader();

	/**
	 * Read meta data of a GigaSpace model
	 *
	 * @pFilename filename to read
	 *
	 * @return a flag telling whether ot not it succeeds
	 */
	virtual bool read( const char* pFilename ) = 0;

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsIReader();

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
	GsIReader( const GsIReader& );

	/**
	 * Copy operator forbidden.
	 */
	GsIReader& operator=( const GsIReader& );

};

}

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

//#include "GsIReader.inl"

#endif
