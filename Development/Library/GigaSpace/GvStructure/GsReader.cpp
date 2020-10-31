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

#include "GvStructure/GsReader.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>
#include <sstream>

// System
#include <cassert>
#include <cmath>

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
GsReader::GsReader()
:	GsIReader()
,	_modelResolution( 0 )
,	_filenames()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsReader::~GsReader()
{
}

/******************************************************************************
 * Read meta data of a GigaSpace model
 *
 * @pFilename filename to read
 *
 * @return a flag telling whether ot not it succeeds
 ******************************************************************************/
bool GsReader::read( const char* pFilename )
{
	// Reset internal state
	_modelResolution = 1;
	_filenames.clear();

	string directory = string( pFilename );
	directory = directory.substr( 0, directory.find_last_of( "\\/" ) + 1 );
	
	int nbLevels = 0;
	int nbChannels = 0;

	vector< string > nodeFilenames;
	vector< string > brickFilenames;

	// Model Document
	//
	// NOTE:
	// the node to be added is passed by pointer, and will be henceforth owned (and deleted) by tinyXml.
	// This method is efficient and avoids an extra copy, but should be used with care as it uses a different memory model than the other insert functions.
	TiXmlDocument modelDocument( pFilename );

	// Try to load the Model file
	bool loadOkay = modelDocument.LoadFile();
	if ( loadOkay )
	{
		// Retrieve Model element
		TiXmlNode* element = modelDocument.FirstChild();
		if ( strcmp( element->Value(), GsIReader::_cModelElementName ) == 0 )
		{
			// Read Model attributes
			TiXmlAttribute* attribute = element->ToElement()->FirstAttribute();
			// - internal validity flags
			bool isDirectoryAttributeFound = false;
			bool isNbLevelsAttributeFound = false;
			while ( attribute )
			{
				// Look for predefined attributes
				if ( strcmp( attribute->Name(), GsIReader::_cNameAttributeName ) == 0 )
				{
					// Model name

					// LOG
					printf( "Loading model %s\n", attribute->Value() );	
				}
				else if ( strcmp( attribute->Name(), GsIReader::_cModelDirectoryAttributeName ) == 0 )
				{	
					// Model directory
					
#ifdef _DEBUG
					// LOG
					printf( "In directory %s\n", attribute->Value() );
#endif
					
					directory = directory + string( attribute->Value() ) + "/";
					
					// Set internal validity flag
					isDirectoryAttributeFound = true;
				} 
				else if ( strcmp( attribute->Name(), GsIReader::_cModelNbLODAttributeName ) == 0 )
				{
					// Model number of level of details
					
#ifdef _DEBUG
					// LOG
					printf( "Nb levels %s\n", attribute->Value() );
#endif
					
					nbLevels = atoi( attribute->Value() );
					_modelResolution *= static_cast< unsigned int >( powf( 2.0f, static_cast< float >( nbLevels - 1 ) ) );	// Check if it's alwas true for something else than octrees
					
#ifdef _DEBUG
					// LOG
					printf( "%d\n", (int)( _modelResolution ) );
#endif

					// Set internal validity flag
					isNbLevelsAttributeFound = true;
				}
				else 
				{
					// LOG
					printf( "XML WARNING Unknown attribute: %s\n", attribute->Value() );
				}

				// Retrieve next Model attribute
				attribute = attribute->Next();
			}

			// Check internal validity flags
			if ( isDirectoryAttributeFound && isNbLevelsAttributeFound )
			{
				// Retrieve Node Tree elements
				element = element->FirstChild();
				while ( element )
				{
					TiXmlNode* level;
					TiXmlNode* channel;

					// Look for Model predefined elements
					if ( strcmp( element->Value(), GsIReader::_cNodeTreeElementName ) == 0 )
					{
						// Node Tree element

						// Retrieve Node Tree child element
						level = element->FirstChild();
						while ( level )
						{
							// Look for Node Tree predefined elements
							if ( strcmp( level->Value(), GsIReader::_cLODElementName ) == 0 )
							{
								// Node Tree level

#ifdef _DEBUG
								// LOG
								printf( "Node\n" );
#endif
								
								// Retrieve Node Tree "level" attributes
								attribute = level->ToElement()->FirstAttribute();
								while ( attribute )
								{
									// Look for Node Tree level predefined attributes
									if ( strcmp( attribute->Name(), GsIReader::_cIdAttributeName ) == 0 )
									{	
										// Id

#ifdef _DEBUG
										// LOG
										printf( "Id : %s\n", attribute->Value() );
#endif
									}
									else if ( strcmp( attribute->Name(), GsIReader::_cFilenameAttributeName ) == 0 )
									{
										// Filename

#ifdef _DEBUG
										// LOG
										printf( "Filename : %s\n", ( directory + string( attribute->Value() ) ).c_str() );
#endif
										
										nodeFilenames.push_back( directory + string( attribute->Value() ) );
									}
									else
									{
#ifdef _DEBUG
										// LOG
										printf( "XML WARNING Unknown attribute: %s\n", attribute->Value() );
#endif
									}

									// Retrieve next Node Tree "level" attribute
									attribute = attribute->Next();
								}
							}
							else
							{
								// LOG
								printf( "XML WARNING Unexpected token : %s expected Level\n", level->Value() );
							}

							// Retrieve next Node Tree "level" element
							level = level->NextSibling();
						}
					}
					else if ( strcmp( element->Value(), GsIReader::_cBrickDataElementName ) == 0 )
					{
						// Brick Data element

						// Retrieve Brick Data attributes
						attribute = element->ToElement()->FirstAttribute();
						while ( attribute )
						{
							if ( strcmp( attribute->Name(), GsIReader::_cBrickResolutionAttributeName ) == 0 )
							{	
								// Brick resolution

#ifdef _DEBUG
								// LOG
								printf( "Resolution : %s\n", attribute->Value() );
#endif
								
								_modelResolution *= atoi( attribute->Value() );
							}
							else if ( strcmp( attribute->Name(), GsIReader::_cBrickBorderSizeAttributeName ) == 0 )
							{
								// Brick border size

#ifdef _DEBUG
								// LOG
								printf( "Border size : %s\n", attribute->Value() );
#endif
							}
							else 
							{
								// LOG
								//printf( "XML WARNING Unknown attribute: %s\n", attribute->Value() );
							}

							// Retrieve next Brick Data attribute
							attribute = attribute->Next();
						}
						
						// Retrieve Brick Data elements
						channel = element->FirstChild();
						while ( channel )
						{
							// Update counter
							nbChannels++;

							// Look for Brick Data predefined elements
							if ( strcmp( channel->Value(), GsIReader::_cBrickDataChannelElementName ) == 0 )
							{
								// Brick Data channel

#ifdef _DEBUG
								// LOG
								printf( "Channel\n" );
#endif
								
								// Retrieve Brick Data "channel" attributes
								attribute = channel->ToElement()->FirstAttribute();
								while ( attribute )
								{
									// Look for Brick Data "channel" predefined attributes
									if ( strcmp( attribute->Name(), GsIReader::_cIdAttributeName ) == 0 )
									{	
										// Id

#ifdef _DEBUG
										// LOG
										printf( "Id : %s\n", attribute->Value() );
#endif
									}
									else if ( strcmp( attribute->Name(), GsIReader::_cNameAttributeName ) == 0 )
									{
										// Name

#ifdef _DEBUG
										// LOG
										printf( "Name : %s\n", attribute->Value() );
#endif
									}
									else if ( strcmp( attribute->Name(), GsIReader::_cBrickDataTypeAttributeName ) == 0 )
									{
										// Type

#ifdef _DEBUG
										// LOG
										printf( "Type : %s\n", attribute->Value() );
#endif
									}
									else 
									{
										// LOG
										printf( "XML WARNING Unknown attribute: %s\n", attribute->Value() );
									}

									// Retrieve next Brick Data "channel" attribute
									attribute = attribute->Next();
								}

								// Retrieve Brick Data "channel" elements
								level = channel->FirstChild();
								while ( level )
								{
									// Look for Brick Data "channel" predefined elements
									if ( strcmp( level->Value(), GsIReader::_cLODElementName ) == 0 )
									{
										// Brick Data channel's "Level"

#ifdef _DEBUG
										// LOG
										printf( "Brick\n" );
#endif
										
										// Retrieve Brick Data channel's "level" attributes
										attribute = level->ToElement()->FirstAttribute();
										while ( attribute )
										{
											// Look for Brick Data channel's "level" predefined attributes
											if ( strcmp( attribute->Name(), GsIReader::_cIdAttributeName ) == 0 )
											{	
												// Id

#ifdef _DEBUG
												// LOG
												printf( "Id : %s\n", attribute->Value() );
#endif
											}
											else if ( strcmp( attribute->Name(), GsIReader::_cFilenameAttributeName ) == 0 )
											{
												// Filename

#ifdef _DEBUG
												// LOG
												printf( "Filename : %s\n", ( directory + string( attribute->Value() ) ).c_str() );
#endif
												
												brickFilenames.push_back( directory + string( attribute->Value() ) );
											}
											else 
											{
												// LOG
												printf( "XML WARNING Unknown attribute: %s\n", attribute->Value() );
											}

											// Retrieve next Brick Data channel's "level" attribute
											attribute = attribute->Next();
										}
									}
									else
									{
										// LOG
										printf( "XML WARNING Unexpected token : %s expected Level\n", level->Value() );
									}

									// Retrieve next element
									level = level->NextSibling();
								}
							}
							else 
							{
								// LOG
								printf( "XML WARNING Unexpected token : %s expected Channel\n", channel->Value() );
							}

							// Retrieve next Brick Data "channel" element
							channel = channel->NextSibling();
						}
					}
					else 
					{
						// LOG
						printf( "XML WARNING Unknown token : %s\n", element->Value() );
					}

					// Retrieve next Model element
					element = element->NextSibling();
				}				
			}
			else
			{
				// LOG
				printf( "XML ERROR Wrong Syntax : Missing Model informations (directory or nbLevels)\n" );
				
				return false;
			}
		}
		else
		{
			// LOG
			printf( "XML ERROR Wrong Syntax : expected \"%s\", read \"%s\"\n", GsIReader::_cModelElementName, element->Value() );
			
			return false;
		}
	}
	else
	{
		// LOG
		printf( "XML ERROR Failed to load file %s \n", pFilename );
		
		return false;
	}

	// LOG
	//printf( "%d %d\n", nbLevels, nbChannels );
	
	// Iterate through level of details
	for ( int k = 0; k < nbLevels ; k++ )
	{
		_filenames.push_back( nodeFilenames[ k ] );
		
		// LOG
		//printf( "%s\n", nodeFilenames[ k ].c_str() );
		
		// Iterate through brick data channels
		for ( int p = 0; p < nbChannels ; p++ )
		{
			_filenames.push_back( brickFilenames[ k + p * nbLevels ] );

			// LOG
			//printf( "%s\n", brickFilenames[ k + p * nbLevels ].c_str() );
		}
	}
	
	return true;
}

/******************************************************************************
 * Get the Model resolution (i.e. number of voxels in each dimension)
 *
 * @return pValue Model resolution
 ******************************************************************************/
unsigned int GsReader::getModelResolution() const
{
	return _modelResolution;
}

/******************************************************************************
 * Get the list of all filenames that producer will have to load (nodes and bricks)
 *
 * @return list of all filenames
 ******************************************************************************/
/*const*/ vector< string >/*&*/ GsReader::getFilenames() const
{
	return _filenames;
}
