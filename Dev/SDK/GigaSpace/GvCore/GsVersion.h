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

#ifndef GVVERSION_H
#define GVVERSION_H

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

 // STL
 #include <string>
 
 /******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

 /**
  * Version information used to query API's version at compile time
  */
#define GV_API_VERSION_MAJOR 1
#define GV_API_VERSION_MINOR 0
#define GV_API_VERSION_PATCH 0

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @class GsVersion
 *
 * @brief The GsVersion class provides version information for the GigaVoxels API
 *
 * @ingroup GvCore
 *
 * ...
 */
class GIGASPACE_EXPORT GsVersion
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/
		
public:

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Return the major version number, e.g., 1 for "1.2.3"
	 *
	 * @return the major version number
	 */
	static unsigned int getMajor();
	
	/**
	 * Return the minor version number, e.g., 2 for "1.2.3"
	 *
	 * @return the minor version number
	 */
	static unsigned int getMinor();
	
	/**
	 * Return the patch version number, e.g., 3 for "1.2.3"
	 *
	 * @return the patch version number
	 */
	static unsigned int getPatch();

	/**
	 * Return the full version number as a string, e.g., "1.2.3"
	 *
	 * @return the the full version number
	 */
	static std::string getVersion();

	/**
	 * Return true if the current version >= (major, minor, patch)
	 *
	 * @param pMajor The major version
	 * @param pMinor The minor version
	 * @param pPatch The patch version
	 *
	 * @return true if the current version >= (major, minor, patch)
	 */
	static bool isAtLeast( unsigned int pMajor, unsigned int pMinor, unsigned int pPatch );

	/**
	 * Return true if the named feature is available in this version
	 *
	 * @param pName The name of a feature
	 *
	 * @return true if the named feature is available in this version
	 */
	static bool hasFeature( const std::string& pName );
	
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

	/******************************** METHODS *********************************/
	
};

} // namespace GvCore

#endif // !GVVERSION_H
