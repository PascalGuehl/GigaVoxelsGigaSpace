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

#include "GvCore/GsVersion.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <string>
#include <sstream>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// Gigavoxels
using namespace GvCore;

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
 * Return the major version number, e.g., 1 for "1.2.3"
 *
 * @return the major version number
 ******************************************************************************/
unsigned int GsVersion::getMajor()
{
	return static_cast< unsigned int >( GV_API_VERSION_MAJOR );
}

/******************************************************************************
 * Return the minor version number, e.g., 2 for "1.2.3"
 *
 * @return the minor version number
 ******************************************************************************/
unsigned int GsVersion::getMinor()
{
	return static_cast< unsigned int >( GV_API_VERSION_MINOR );
}

/******************************************************************************
 * Return the patch version number, e.g., 3 for "1.2.3"
 *
 * @return the patch version number
 ******************************************************************************/
unsigned int GsVersion::getPatch()
{
	return static_cast< unsigned int >( GV_API_VERSION_PATCH );
}

/******************************************************************************
 * Return the full version number as a string, e.g., "1.2.3"
 *
 * @return the the full version number
 ******************************************************************************/
string GsVersion::getVersion()
{
	static string version( "" );
	if ( version.empty() )
	{
		// Cache the version string
		ostringstream stream;
		stream << GV_API_VERSION_MAJOR << "."
			   << GV_API_VERSION_MINOR << "."
			   << GV_API_VERSION_PATCH;
		version = stream.str();
	}

	return version;
}

/******************************************************************************
 * Return true if the current version >= (major, minor, patch)
 *
 * @param pMajor ...
 * @param pMinor ...
 * @param pPatch ...
 *
 * @return true if the current version >= (major, minor, patch)
 ******************************************************************************/
bool GsVersion::isAtLeast( unsigned int pMajor, unsigned int pMinor, unsigned int pPatch )
{
	if ( GV_API_VERSION_MAJOR < pMajor )
	{
		return false;
	}

	if ( GV_API_VERSION_MAJOR > pMajor )
	{
		return true;
	}

	if ( GV_API_VERSION_MINOR < pMinor )
	{
		return false;
	}

	if ( GV_API_VERSION_MINOR > pMinor )
	{
		return true;
	}

	if ( GV_API_VERSION_PATCH < pPatch )
	{
		return false;
	}

	return true;
}

/******************************************************************************
 * Return true if the named feature is available in this version
 *
 * @param pName The name of a feature
 *
 * @return true if the named feature is available in this version
 ******************************************************************************/
bool GsVersion::hasFeature( const string& pName )
{
	// Not yet implemented
	return false;
}
