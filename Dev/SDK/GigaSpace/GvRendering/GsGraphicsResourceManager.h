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

#ifndef _GV_GRAPHICS_RESOURCE_MANAGER_H_
#define _GV_GRAPHICS_RESOURCE_MANAGER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

// STL
#include <vector>
#include <cstddef>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaVoxels
namespace GvRendering
{
	class GsGraphicsResource;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvRendering
{
	
	/** 
	 * @class GsGraphicsResourceManager
	 *
	 * @brief The GsGraphicsResourceManager class provides way to access graphics resource.
	 *
	 * @ingroup GsRenderer
	 *
	 * The GsGraphicsResourceManager class is the main accessor of all graphics resources.
	 */
	class GIGASPACE_EXPORT GsGraphicsResourceManager
	{

		/**************************************************************************
		 ***************************** PUBLIC SECTION *****************************
		 **************************************************************************/

	public:

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**
		 * Get the device manager
		 *
		 * @return the device manager
		 */
		static GsGraphicsResourceManager& get();

		/**
		 * Initialize the device manager
		 */
		bool initialize();

		/**
		 * Finalize the device manager
		 */
		void finalize();

		/**
		 * Get the number of devices
		 *
		 * @return the number of devices
		 */
		size_t getNbResources() const;

		/**
		 * Get the device given its index
		 *
		 * @param the index of the requested device
		 *
		 * @return the requested device
		 */
		const GsGraphicsResource* getResource( int pIndex ) const;

		/**
		 * Get the device given its index
		 *
		 * @param the index of the requested device
		 *
		 * @return the requested device
		 */
		GsGraphicsResource* editResource( int pIndex );
		
		/**************************************************************************
		 **************************** PROTECTED SECTION ***************************
		 **************************************************************************/

	protected:

		/******************************* ATTRIBUTES *******************************/

		/******************************** TYPEDEFS ********************************/

		/**
		 * The unique device manager
		 */
		static GsGraphicsResourceManager* msInstance;

		/**
		 * The container of devices
		 */
#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable:4251 )
#endif
		std::vector< GsGraphicsResource* > _graphicsResources;
#if defined _MSC_VER
#pragma warning( pop )
#endif

		/**
		 * Flag to tell wheter or not the device manager is initialized
		 */
		bool _isInitialized;

		/******************************** METHODS *********************************/

		/**
		 * Constructor
		 */
		GsGraphicsResourceManager();

		/**
		 * Destructor
		 */
		~GsGraphicsResourceManager();

		/**************************************************************************
		 ***************************** PRIVATE SECTION ****************************
		 **************************************************************************/

	private:

		/******************************* ATTRIBUTES *******************************/

		/******************************** METHODS *********************************/

		/**
		 * Copy constructor forbidden.
		 */
		GsGraphicsResourceManager( const GsGraphicsResourceManager& );

		/**
		 * Copy operator forbidden.
		 */
		GsGraphicsResourceManager& operator=( const GsGraphicsResourceManager& );

	};

} // namespace GvRendering

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#endif // !_GV_GRAPHICS_RESOURCE_MANAGER_H_
