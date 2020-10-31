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

#include "GvVoxelizer/GsIRAWFileReader.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvVoxelizer/GsDataStructureMipmapGenerator.h"
#include "GvVoxelizer/GsDataTypeHandler.h"

// STL
#include <iostream>
#include <vector>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GvVoxelizer
using namespace GvVoxelizer;

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
GsIRAWFileReader::GsIRAWFileReader()
:	_filename()
,	_dataResolution( 0 )
,	_mode( eUndefinedMode )
,	_dataType( GsDataTypeHandler::gvUCHAR )
,	_dataStructureIOHandler( NULL )
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsIRAWFileReader::~GsIRAWFileReader()
{
}

/******************************************************************************
 * Load/import the scene the scene
 ******************************************************************************/
bool GsIRAWFileReader::read()
{
	bool result = false;

	std::cout << "- [step 1 / 3] - Read data and write voxels..." << std::endl;
	result = readData();	// TO DO : add a boolean return value

	std::cout << "- [step 2 / 3] - Update borders..." << std::endl;
	_dataStructureIOHandler->computeBorders();	// TO DO : add a boolean return value

	std::cout << "- [step 3 / 3] - Mipmap pyramid generation..." << std::endl;
	result = generateMipmapPyramid();

	return result;
}

/******************************************************************************
 * Load/import the scene the scene
 ******************************************************************************/
bool GsIRAWFileReader::readData()
{
	return false;
}

/******************************************************************************
 * Apply the mip-mapping algorithmn.
 * Given a pre-filtered voxel scene at a given level of resolution,
 * it generates a mip-map pyramid hierarchy of coarser levels (until 0).
 ******************************************************************************/
bool GsIRAWFileReader::generateMipmapPyramid()
{
	std::vector< GsDataTypeHandler::VoxelDataType > dataTypes;
	dataTypes.push_back( getDataType() );
	return GsDataStructureMipmapGenerator::generateMipmapPyramid( getFilename(), getDataResolution(), dataTypes );
}

/******************************************************************************
	 * 3D model file name
 ******************************************************************************/
const std::string& GsIRAWFileReader::getFilename() const
{
	return _filename;
}

/******************************************************************************
 * 3D model file name
 ******************************************************************************/
void GsIRAWFileReader::setFilename( const std::string& pName )
{
	_filename = pName;
}
	
/******************************************************************************
 * Data resolution
 ******************************************************************************/
unsigned int GsIRAWFileReader::getDataResolution() const
{
	return _dataResolution;
}

/******************************************************************************
	 * Data resolution
 ******************************************************************************/
void GsIRAWFileReader::setDataResolution( unsigned int pValue )
{
	_dataResolution = pValue;
}

/******************************************************************************
 * Mode (binary or ascii)
 ******************************************************************************/
GsIRAWFileReader::Mode GsIRAWFileReader::getMode() const
{
	return _mode;
}

/******************************************************************************
 * Mode (binary or ascii)
 ******************************************************************************/
void GsIRAWFileReader::setMode( Mode pMode )
{
	_mode = pMode;
}

/******************************************************************************
 * Get the data type
 *
 * @return the data type
 ******************************************************************************/
GsDataTypeHandler::VoxelDataType GsIRAWFileReader::getDataType() const
{
	return _dataType;
}

/******************************************************************************
 * Set the data type
 *
 * @param pType the data type
 ******************************************************************************/
void GsIRAWFileReader::setDataType( GsDataTypeHandler::VoxelDataType pType )
{
	_dataType = pType;
}
