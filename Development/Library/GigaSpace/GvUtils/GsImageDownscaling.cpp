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

#include "GvUtils/GsImageDownscaling.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvRendering/GsGraphicsResource.h"
#include "GvCore/GsError.h"
#include "GvUtils/GsShaderManager.h"

// CUDA
#include <driver_types.h>

// System
#include <cassert>
#include <cstddef>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvUtils;
using namespace GvRendering;

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
GsImageDownscaling::GsImageDownscaling()
:	_width( 0 )
,	_height( 0 )
{
}

/******************************************************************************
 * Desconstructor
 ******************************************************************************/
GsImageDownscaling::~GsImageDownscaling()
{
	finalize();
}

/******************************************************************************
 * Initialize
 *
 * @return a flag to tell wheter or not it succeeds.
 ******************************************************************************/
bool GsImageDownscaling::initialize()
{
	return false;
}

/******************************************************************************
 * Finalize
 *
 * @return a flag to tell wheter or not it succeeds.
 ******************************************************************************/
bool GsImageDownscaling::finalize()
{
	return false;
}

/******************************************************************************
 * Get the buffer's width
 *
 * @return the buffer's width
 ******************************************************************************/
unsigned int GsImageDownscaling::getWidth() const
{
	return _width;
}

/******************************************************************************
 * Get the buffer's height
 *
 * @return the buffer's height
 ******************************************************************************/
unsigned int GsImageDownscaling::getHeight() const
{
	return _height;
}

/******************************************************************************
 * Set the buffer's resolution
 *
 * @param pWidth width
 * @param pHeight height
 ******************************************************************************/
void GsImageDownscaling::setResolution( unsigned int pWidth, unsigned int pHeight )
{
	assert( ! ( pWidth == 0 || pHeight == 0 ) );

	_width = pWidth;
	_height= pHeight;
}
