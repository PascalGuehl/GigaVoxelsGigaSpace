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

#include "GvUtils/GsEnvironment.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// System
#include <cassert>

// STL
#include <iostream>

// TinyXML
#include <tinyxml.h>
//#include <tinystr.h>

#ifndef WIN32
#include <wordexp.h>
#endif

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaSpace
using namespace GvUtils;

// STL
using namespace std;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * Data info
 */
const char* GsEnvironment::_cDataElementName = "Data";
const char* GsEnvironment::_cDataPathAttributeName = "path";

/**
 * Data info
 */
const char* GsEnvironment::_cUserProfileElementName = "UserProfile";
const char* GsEnvironment::_cUserProfilePathAttributeName = "path";

/**
 * Application file path
 */
string GsEnvironment::_sApplicationFilePath = "";

/**
 * Application directory
 */
string GsEnvironment::_sApplicationDirectory = ".";

/**
 * Data path
 */
string GsEnvironment::_sDataDirectory = "./Data";

/**
 * User Profile path
 */
string GsEnvironment::_sUserProfileDirectory = "./Settings";

/**
 * Specifies the names of the different directories
 */
const char* GsEnvironment::cSystemDirNames[ GsEnvironment::eNbSystemDirs ] =
{
	"Settings",
	"Plugins",
	"Resources",
	"Manuals",
	"Licenses",
	"Tools"
};

/**
 * Specifies the names of the different data directories
 */
const char* GsEnvironment::cDataDirNames[ GsEnvironment::eNbDataDirs ] =
{
	"3DModels",
	"Shaders",
	"SkyBox",
	"Terrain",
	"TransferFunctions",
	"Videos",
	"Voxels"
};

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Initialize the environment
 *
 * @param pApplicationFilePath the application file path
 ******************************************************************************/
void GsEnvironment::initialize( const std::string pApplicationFilePath )
{
	_sApplicationDirectory = pApplicationFilePath;
}

/******************************************************************************
 * Initialize the settings
 *
 * @return true if it succeeds
 ******************************************************************************/
bool GsEnvironment::initializeSettings()
{
	// Try to open GigaSpace settings file
	const string settingsFilename = getSystemDir( GsEnvironment::eSettingsDir ) + "/" + "GigaSpace.xml";

	// Model Document
	//
	// NOTE:
	// the node to be added is passed by pointer, and will be henceforth owned (and deleted) by tinyXml.
	// This method is efficient and avoids an extra copy, but should be used with care as it uses a different memory model than the other insert functions.
	TiXmlDocument modelDocument( settingsFilename.c_str() );

	// Try to load the Model file
	bool loadOkay = modelDocument.LoadFile();
	if ( ! loadOkay )
	{
		// LOG
		std::cout << "Unable to load the settings file " << settingsFilename << endl;

		return false;
	}

	// Retrieve Model element
	TiXmlNode* element = modelDocument.FirstChild();
	if ( strcmp( element->Value(), "Settings" ) == 0 )
	{
		// Retrieve Node Tree elements
		element = element->FirstChild();
		while ( element )
		{
			// TODO
			// - add UserProfile Element

			// Look for Model predefined elements
			if ( strcmp( element->Value(), GsEnvironment::_cDataElementName ) == 0 )
			{
				// Read Model attributes
				TiXmlAttribute* attribute = element->ToElement()->FirstAttribute();

				// Look for Node Tree level predefined attributes
				if ( strcmp( attribute->Name(), GsEnvironment::_cDataPathAttributeName ) == 0 )
				{
					// Update internal state
					_sDataDirectory = expand( attribute->Value() );

					// LOG
					cout << "Data path : " << _sDataDirectory << endl;
				}
				else
				{
					// LOG
					cout << "No Data path found. Using default settings" << endl;
				}
			}
			else if ( strcmp( element->Value(), GsEnvironment::_cUserProfileElementName ) == 0 )
			{
				// Read Model attributes
				TiXmlAttribute* attribute = element->ToElement()->FirstAttribute();

				// Look for Node Tree level predefined attributes
				if ( strcmp( attribute->Name(), GsEnvironment::_cUserProfilePathAttributeName ) == 0 )
				{
					// Update internal state
					_sUserProfileDirectory = expand( attribute->Value() );

					// LOG
					cout << "User Profile path : " << attribute->Value() << endl;
				}
				else
				{
					// LOG
					cout << "No User Profile path found. Using default settings" << endl;
				}
			}
			else
			{
				// LOG
				printf( "XML WARNING Unknown token : %s\n", element->Value() );
			}

			// Retrieve next element
			element = element->NextSibling();
		}
	}
	else
	{
		// LOG
		printf( "\nXML ERROR Wrong Syntax" );

		return false;
	}

	return true;
}

/******************************************************************************
 * Returns the application file path
 *
 * @return the application file path
 ******************************************************************************/
const std::string& GsEnvironment::getApplicationFilePath()
{
	return _sApplicationFilePath;
}

/******************************************************************************
 * Returns the application directory
 *
 * @return the application directory
 ******************************************************************************/
const std::string& GsEnvironment::getApplicationDirectory()
{
	return _sApplicationDirectory;
}

/******************************************************************************
 * Returns the data path
 *
 * @return the data path
 ******************************************************************************/
const std::string& GsEnvironment::getDataPath()
{
	return _sDataDirectory;
}

/******************************************************************************
 * Returns the specified system directory
 *
 * @return the directory
 ******************************************************************************/
std::string GsEnvironment::getSystemDir( GsEnvironment::GsSystemDir pDirName )
{
	assert( pDirName < GsEnvironment::eNbSystemDirs );
	return _sApplicationDirectory + "/" + cSystemDirNames[ static_cast< unsigned int >( pDirName ) ];
}

/******************************************************************************
 * Returns the specified data directory
 *
 * @return the directory
 ******************************************************************************/
std::string GsEnvironment::getDataDir( GsEnvironment::GsDataDir pDirName )
{
	assert( pDirName < GsEnvironment::eNbDataDirs );
	return _sDataDirectory + "/" + cDataDirNames[ static_cast< unsigned int >( pDirName ) ];
}

/******************************************************************************
 * Returns the user profile path
 *
 * @return the user profile path
 ******************************************************************************/
const std::string& GsEnvironment::getUserProfilePath()
{
	return _sUserProfileDirectory;
}

/******************************************************************************
 * Expand a directory name
 *
 * @param pDirectory the name to expand
 ******************************************************************************/
string GsEnvironment::expand( const char* pDirectory )
{
	string path = string( pDirectory );

	// Check for empty path
	if ( path.empty() )
	{
		return path;
	}

#ifdef WIN32
 	// TODO
#else
	// Expand ~ to /home/user if necessary
	wordexp_t exp_result;
	if( wordexp( pDirectory, &exp_result, WRDE_NOCMD ) == 0 )
	{
		path = string( exp_result.we_wordv[ 0 ] );
		wordfree( &exp_result );
	}
	else
	{
		cerr << "Error : " << pDirectory << " contain forbidden characters." << endl;
	}

	// Append the directory containing the executable if necessary.
	if ( path[ 0 ] != '/' )
	{
		path = _sApplicationDirectory + '/' + path;
	}
#endif

	return path;
}
