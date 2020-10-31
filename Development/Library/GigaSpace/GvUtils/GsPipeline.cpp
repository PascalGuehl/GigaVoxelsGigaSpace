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

#include "GvUtils/GsPipeline.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include <GvStructure/GsIDataStructure.h>
#include <GvCore/GsIProvider.h>
#include <GvRendering/GsIRenderer.h>

// STL
#include <iostream>

// System
#include <cassert>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GvVoxelizer
using namespace GvUtils;
using namespace GvCore;
using namespace GvStructure;
using namespace GvRendering;

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
GsPipeline::GsPipeline()
:	GsIPipeline()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsPipeline::~GsPipeline()
{
}

/******************************************************************************
 * Initialize
 *
 * @return pDataStructure the associated data structure
 * @return pProducer the associated producer
 * @return pRenderer the associated  renderer
 ******************************************************************************/
//void GsPipeline::initialize( GvStructure::GsIDataStructure* pDataStructure, GvCore::GsIProvider* pProducer, GvRendering::GsIRenderer* pRenderer )
//{
//}

/******************************************************************************
 * Finalize
 ******************************************************************************/
void GsPipeline::finalize()
{
}

/******************************************************************************
 * Get the data structure
 *
 * @return The data structure
******************************************************************************/
const GsIDataStructure* GsPipeline::getDataStructure() const
{
	return NULL;
}

/******************************************************************************
 * Get the data structure
 *
 * @return The data structure
******************************************************************************/
GvStructure::GsIDataStructure* GsPipeline::editDataStructure()
{
	return NULL;
}

/******************************************************************************
 * Get the data production manager
 *
 * @return The data production manager
******************************************************************************/
const GsIDataProductionManager* GsPipeline::getCache() const
{
	return NULL;
}

/******************************************************************************
 * Get the data production manager
 *
 * @return The data production manager
******************************************************************************/
GvStructure::GsIDataProductionManager* GsPipeline::editCache()
{
	return NULL;
}

/******************************************************************************
 * Get the producer
 *
 * @return The producer
******************************************************************************/
const GsIProvider* GsPipeline::getProducer( unsigned int pIndex ) const
{
	return NULL;
}

/******************************************************************************
 * Get the producer
 *
 * @return The producer
******************************************************************************/
GsIProvider* GsPipeline::editProducer( unsigned int pIndex )
{
	return NULL;
}

/******************************************************************************
 * Get the renderer
 *
 * @return The renderer
******************************************************************************/
const GsIRenderer* GsPipeline::getRenderer( unsigned int pIndex ) const
{
	return NULL;
}

/******************************************************************************
 * Get the renderer
 *
 * @return The renderer
******************************************************************************/
GsIRenderer* GsPipeline::editRenderer( unsigned int pIndex )
{
	return NULL;
}
