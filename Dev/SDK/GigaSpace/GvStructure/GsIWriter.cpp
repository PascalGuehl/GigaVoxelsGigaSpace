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

#include "GvStructure/GsIWriter.h"

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
const char* GsIWriter::_cModelFileExtension = "xml";
const char* GsIWriter::_cNodeFileExtension = "nodes";
const char* GsIWriter::_cBrickFileExtension = "bricks";
const char* GsIWriter::_cFileSymbolSeperator = "_";
const char* GsIWriter::_cBrickWidthSymbol = "BR";
const char* GsIWriter::_cBrickBorderSizeSymbol = "B";
const char* GsIWriter::_cLODIndexSymbol = "L";
const char* GsIWriter::_cDataChannelIndexSymbol = "C";

/**
 * Model info
 */
const char* GsIWriter::_cModelElementName = "Model";
const char* GsIWriter::_cModelDirectoryAttributeName = "directory";
const char* GsIWriter::_cModelNbLODAttributeName = "nbLevels";

/**
 * Node Tree info
 */
const char* GsIWriter::_cNodeTreeElementName = "NodeTree";

/**
 * Brick Data info
 */
const char* GsIWriter::_cBrickDataElementName = "BrickData";
const char* GsIWriter::_cBrickResolutionAttributeName = "brickResolution";
const char* GsIWriter::_cBrickBorderSizeAttributeName = "borderSize";
const char* GsIWriter::_cBrickDataChannelElementName = "Channel";
const char* GsIWriter::_cBrickDataTypeAttributeName = "type";

/**
 * Generic info
 */
const char* GsIWriter::_cNameAttributeName = "name";
const char* GsIWriter::_cIdAttributeName = "id";
const char* GsIWriter::_cFilenameAttributeName = "filename";
const char* GsIWriter::_cLODElementName = "Level";

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsIWriter::GsIWriter()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsIWriter::~GsIWriter()
{
}

/******************************************************************************
 * Export meta data of a GigaSpace model
 *
 * @return a flag telling whether ot not it succeeds
 ******************************************************************************/
bool GsIWriter::write()
{
	return false;
}
