/*
 * GigaVoxels - GigaSpace
 *
 * [GigaVoxels] is a ray-guided out-of-core, on demand production, and smart caching library
 * used for efficient 3D real-time visualization of highly large and detailed
 * sparse volumetric scenes (SVO : Sparse Voxel Octree).
 *
 * [GigaSpace] is a full-customizable out-of-core, on demand production, and smart caching library
 * based on user-defined hierachical arbitrary space partitionning, space & brick visitors,
 * and brick producer using arbitrary data types.
 *
 * GigaVoxels and GigaSpace are indeed the same tool.
 * 
 * Website : http://gigavoxels.inrialpes.fr/
 *
 * Copyright (C) 2006-2015 INRIA - LJK ( CNRS - Grenoble University )
 * - INRIA <http://www.inria.fr/en/>
 * - CNRS  <http://www.cnrs.fr/index.php>
 * - LJK   <http://www-ljk.imag.fr/index_en.php>
 *
 * Authors : GigaVoxels Team
 *
 * GigaVoxels is distributed under a dual-license scheme.
 * You can obtain a specific license from Inria at gigavoxels-licensing@inria.fr.
 * Otherwise the default license is the GPL version 3.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** 
 * @version 1.0
 */

#include "GvRendering/GsGraphicsResourceManager.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvRendering/GsGraphicsResource.h"

// CUDA toolkit
#include <cuda_runtime.h>

// System
#include <cassert>
#include <iostream>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// Gigavoxels
using namespace GvRendering;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * The unique device manager
 */
GsGraphicsResourceManager* GsGraphicsResourceManager::msInstance = NULL;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Get the device manager.
 *
 * @return the device manager
 ******************************************************************************/
GsGraphicsResourceManager& GsGraphicsResourceManager::get()
{
	if ( msInstance == NULL )
	{
		msInstance = new GsGraphicsResourceManager();
	}
	assert( msInstance != NULL );
	return *msInstance;
}

/******************************************************************************
 * Constructor.
 ******************************************************************************/
GsGraphicsResourceManager::GsGraphicsResourceManager()
:	_graphicsResources()
,	_isInitialized( false )
{
}

/******************************************************************************
 * Destructor.
 ******************************************************************************/
GsGraphicsResourceManager::~GsGraphicsResourceManager()
{
	finalize();
}

/******************************************************************************
 * Initialize the device manager
 ******************************************************************************/
bool GsGraphicsResourceManager::initialize()
{
	return false;
}

/******************************************************************************
 * Finalize the device manager
 ******************************************************************************/
void GsGraphicsResourceManager::finalize()
{
}

/******************************************************************************
 * Get the number of devices
 *
 * @return the number of devices
 ******************************************************************************/
size_t GsGraphicsResourceManager::getNbResources() const
{
	return _graphicsResources.size();
}

/******************************************************************************
 * Get the device given its index
 *
 * @param the index of the requested device
 *
 * @return the requested device
 ******************************************************************************/
const GsGraphicsResource* GsGraphicsResourceManager::getResource( int pIndex ) const
{
	assert( pIndex < _graphicsResources.size() );
	return _graphicsResources[ pIndex ];
}

/******************************************************************************
 * Get the device given its index
 *
 * @param the index of the requested device
 *
 * @return the requested device
 ******************************************************************************/
GsGraphicsResource* GsGraphicsResourceManager::editResource( int pIndex )
{
	assert( pIndex < _graphicsResources.size() );
	return _graphicsResources[ pIndex ];
}
