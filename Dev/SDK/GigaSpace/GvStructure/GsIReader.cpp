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

#include "GvStructure/GsIReader.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// Project
using namespace GvStructure;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * File info
 */
const char* GsIReader::_cModelFileExtension = "xml";
const char* GsIReader::_cNodeFileExtension = "nodes";
const char* GsIReader::_cBrickFileExtension = "bricks";
const char* GsIReader::_cFileSymbolSeperator = "_";
const char* GsIReader::_cBrickWidthSymbol = "BR";
const char* GsIReader::_cBrickBorderSizeSymbol = "B";
const char* GsIReader::_cLODIndexSymbol = "L";
const char* GsIReader::_cDataChannelIndexSymbol = "C";

/**
 * Model info
 */
const char* GsIReader::_cModelElementName = "Model";
const char* GsIReader::_cModelDirectoryAttributeName = "directory";
const char* GsIReader::_cModelNbLODAttributeName = "nbLevels";

/**
 * Node Tree info
 */
const char* GsIReader::_cNodeTreeElementName = "NodeTree";

/**
 * Brick Data info
 */
const char* GsIReader::_cBrickDataElementName = "BrickData";
const char* GsIReader::_cBrickResolutionAttributeName = "brickResolution";
const char* GsIReader::_cBrickBorderSizeAttributeName = "borderSize";
const char* GsIReader::_cBrickDataChannelElementName = "Channel";
const char* GsIReader::_cBrickDataTypeAttributeName = "type";

/**
 * Generic info
 */
const char* GsIReader::_cNameAttributeName = "name";
const char* GsIReader::_cIdAttributeName = "id";
const char* GsIReader::_cFilenameAttributeName = "filename";
const char* GsIReader::_cLODElementName = "Level";

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsIReader::GsIReader()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsIReader::~GsIReader()
{
}

/******************************************************************************
 * Read meta data of a GigaSpace model
 *
 * @pFilename filename to read
 *
 * @return a flag telling whether ot not it succeeds
 ******************************************************************************/
bool GsIReader::read( const char* pFilename )
{
	return false;
}
