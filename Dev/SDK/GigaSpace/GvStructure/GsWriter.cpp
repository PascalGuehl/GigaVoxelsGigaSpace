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

#include "GvStructure/GsWriter.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>
#include <sstream>

// System
#include <cassert>

// TinyXML
#include <tinyxml.h>
//#include <tinystr.h>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// Project
using namespace GvStructure;

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
GsWriter::GsWriter()
:	GsIWriter()
,	_modelDirectory()
,	_modelName()
,	_nbModelMaxResolution( 0 )
,	_brickWidth( 0 )
,	_nbDataChannels( 0 )
,	_dataTypeNames()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsWriter::~GsWriter()
{
}

/******************************************************************************
 * Export meta data of a voxelization process
 *
 * @return a flag telling whether or not it succeeds
 ******************************************************************************/
bool GsWriter::write()
{
	// TODO
	// - add a validator to check data
	// ...

	// Model Document
	//
	// NOTE:
	// the node to be added is passed by pointer, and will be henceforth owned (and deleted) by tinyXml.
	// This method is efficient and avoids an extra copy, but should be used with care as it uses a different memory model than the other insert functions.
	TiXmlDocument modelDocument;

	// Model element
	TiXmlElement* modelElement = new TiXmlElement( GsIWriter::_cModelElementName ); 
	modelElement->SetAttribute( GsIWriter::_cNameAttributeName, _modelName.c_str() );
    modelElement->SetAttribute( GsIWriter::_cModelDirectoryAttributeName, "." );
	stringstream nbLevels;
	nbLevels << ( _nbModelMaxResolution + 1 );
	modelElement->SetAttribute( GsIWriter::_cModelNbLODAttributeName, nbLevels.str().c_str() );

	// TODO
	// - move this LOG elsewhere (ex: calling class) or at debug...
	// LOG
	printf( "%d %d\n", _brickWidth, _nbModelMaxResolution );

	// -------- Handle Node Tree info --------

	// Node Tree element
	TiXmlElement* nodeTreeElement = new TiXmlElement( GsIWriter::_cNodeTreeElementName );
		
	// Iterate through level of details
	TiXmlElement* nodeLODElement = NULL;
	for ( unsigned int k = 0; k < ( _nbModelMaxResolution + 1 ); k++ )
	{
		// Level
		nodeLODElement = new TiXmlElement( GsIWriter::_cLODElementName );

		// Id
		stringstream levelK;
		levelK << k;
		nodeLODElement->SetAttribute( GsIWriter::_cIdAttributeName, levelK.str().c_str() );

		// Filename
		stringstream nodeFilename;
		// - node base filename
		nodeFilename << _modelName;
		// - brick width
		nodeFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cBrickWidthSymbol << _brickWidth;
		// - brick border size
		nodeFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cBrickBorderSizeSymbol << "1";
		// - node LOD index
		nodeFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cLODIndexSymbol << k;
		// - node file extension
		nodeFilename << "." << GsIWriter::_cNodeFileExtension;
		nodeLODElement->SetAttribute( GsIWriter::_cFilenameAttributeName, nodeFilename.str().c_str() );

		// Add Node LOD info
		nodeTreeElement->LinkEndChild( nodeLODElement );
	}

	// Add Node Tree info
	modelElement->LinkEndChild( nodeTreeElement );
	
	// -------- Handle Brick Data info --------

	// Brick Data element
	TiXmlElement* brickDataElement = new TiXmlElement( GsIWriter::_cBrickDataElementName );
	stringstream brickRes;
	brickRes << _brickWidth;
	brickDataElement->SetAttribute( GsIWriter::_cBrickResolutionAttributeName, brickRes.str().c_str() );
	brickDataElement->SetAttribute( GsIWriter::_cBrickBorderSizeAttributeName, "1" );

	// Iterate through data channels
	TiXmlElement* dataChannelElement;
	TiXmlElement* brickLODElement = NULL;
	for ( unsigned int i = 0; i < _nbDataChannels; i++ )
	{
		// Data channel
		dataChannelElement = new TiXmlElement( GsIWriter::_cBrickDataChannelElementName );

		// Id
		stringstream channelP;
		channelP << i;
		dataChannelElement->SetAttribute( GsIWriter::_cIdAttributeName, channelP.str().c_str() );

		// Name and type
		// TODO
		// - use "_dataTypeNames" member variable instead hardcoded types...
		///if ( i == 0 ) 
		///{
		///	dataChannelElement->SetAttribute( GsIWriter::_cNameAttributeName, "color" );
		///	dataChannelElement->SetAttribute( GsIWriter::_cBrickDataTypeAttributeName, "uchar4" );
		///}
		///else if ( i == 1 )
		///{
		///	dataChannelElement->SetAttribute( GsIWriter::_cNameAttributeName, "normal" );
		///	dataChannelElement->SetAttribute( GsIWriter::_cBrickDataTypeAttributeName, "half4" );
		///}
		///else
		///{
		///	dataChannelElement->SetAttribute( GsIWriter::_cNameAttributeName, "unknown" );
		///	dataChannelElement->SetAttribute( GsIWriter::_cBrickDataTypeAttributeName, "unknown" );
		///}
		dataChannelElement->SetAttribute( GsIWriter::_cNameAttributeName, "unknown" );
		dataChannelElement->SetAttribute( GsIWriter::_cBrickDataTypeAttributeName, _dataTypeNames[ i ].c_str() );

		// Iterate through level of details
		for ( unsigned int k = 0; k < ( _nbModelMaxResolution + 1 ); k++ )
		{
			// Level
			brickLODElement = new TiXmlElement( GsIWriter::_cLODElementName );

			// Id
			stringstream levelK;
			levelK << k;
			brickLODElement->SetAttribute( GsIWriter::_cIdAttributeName, levelK.str().c_str() );

			// Filename
			stringstream brickFilename;
			// - brick base filename
			brickFilename << _modelName;
			// - brick width
			brickFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cBrickWidthSymbol << _brickWidth;
			// - brick border size
			brickFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cBrickBorderSizeSymbol << "1";
			// - brick LOD index
			brickFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cLODIndexSymbol << k;
			// - brick data channel index
			brickFilename << GsIWriter::_cFileSymbolSeperator << GsIWriter::_cDataChannelIndexSymbol << i;
			{
				brickFilename << GsIWriter::_cFileSymbolSeperator;
				//if ( i == 0 ) 
				//{
				//	brickFilename << "uchar4";
				//}
				//else if ( i == 1 )
				//{
				//	brickFilename << "half4";
				//}
				//else
				//{
				//	brickFilename << "unknown";
				//}
				brickFilename << _dataTypeNames[i];
			}
			// - brick file extension
			brickFilename << "." << GsIWriter::_cBrickFileExtension;
			brickLODElement->SetAttribute( GsIWriter::_cFilenameAttributeName, brickFilename.str().c_str() );

			// Add Brick LOD info
			dataChannelElement->LinkEndChild( brickLODElement );
		}

		// Add Brick Data Channel info
		brickDataElement->LinkEndChild( dataChannelElement );
	}

	// Add Brick Data info
	modelElement->LinkEndChild( brickDataElement );

	// Add Model info
	modelDocument.LinkEndChild( modelElement );

	// Write file
	stringstream modelFilename;
	// TODO : check if "/" separator is OK on Windows and Linux
	modelFilename << _modelDirectory << "/" << _modelName << "." << GsIWriter::_cModelFileExtension;
	modelDocument.SaveFile( modelFilename.str().c_str() );
	
	return true;
}

/******************************************************************************
 * Set the model directory
 *
 * @param pName model directory
 ******************************************************************************/
void GsWriter::setModelDirectory( const char* pName )
{
	_modelDirectory = pName;
}

/******************************************************************************
 * Set the model name
 *
 * @param pName model name
 ******************************************************************************/
void GsWriter::setModelName( const char* pName )
{
	_modelName = pName;
}

/******************************************************************************
 * Set the model max resolution
 *
 * @param pValue model max resolution
 ******************************************************************************/
void GsWriter::setModelMaxResolution( unsigned int pValue )
{
	_nbModelMaxResolution = pValue;
}

/******************************************************************************
 * Set the brick width
 *
 * @param pValue brick width
 ******************************************************************************/
void GsWriter::setBrickWidth( unsigned int pValue )
{
	_brickWidth = pValue;
}

/******************************************************************************
 * Set the number of data channels
 *
 * @param pValue number of data channels
 ******************************************************************************/
void GsWriter::setNbDataChannels( unsigned int pValue )
{
	_nbDataChannels = pValue;
}

/******************************************************************************
 * Set the list of data type names
 *
 * @param pNames list of data type names
 ******************************************************************************/
void GsWriter::setDataTypeNames( const std::vector< std::string >& pNames )
{
	_dataTypeNames.clear();
	_dataTypeNames = pNames;
}
