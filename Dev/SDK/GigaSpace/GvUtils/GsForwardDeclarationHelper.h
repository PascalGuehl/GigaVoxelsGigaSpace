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

#ifndef _GV_FORWARD_DECLARATION_HELPER_H_
#define _GV_FORWARD_DECLARATION_HELPER_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

/**
 * WARNING :
 *
 * This file is useful to define default parameter for GigaVoxels template classes.
 * Due to forward declaration of template classes requested by GigaVoxels,
 * it is not possible to define default parameters in other files.
 */

// GigaVoxels
namespace GvCore
{
	template< uint r >
	struct GsVec1D;
}

namespace GvUtils
{
	class GsSimplePriorityPoliciesManagerKernel;
}

namespace GvStructure
{
	// Data structure device-side
	template
	<
		class DataTList,
		class NodeTileRes, class BrickRes, uint BorderSize = 1U
	>
	struct VolumeTreeKernel;

	// Data structure host-side
	template
	<
		class DataTList,
		class NodeTileRes, class BrickRes, uint BorderSize = 1U,
		typename TDataStructureKernelType = VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
	>
	struct GsVolumeTree;

	// Cache host-side
	template
	<
		typename TDataStructureType,
		typename TPriorityPoliciesManager = GvUtils::GsSimplePriorityPoliciesManagerKernel
	>
	class GsDataProductionManager;
}

namespace GvRendering
{	
	// Renderer host-side
	template
	<
		typename TDataStructureType,
		typename VolumeTreeCacheType,
		typename SampleShader
	>
	class GsRendererCUDA;
}

namespace GvUtils
{	
	// Pass through host producer
	template
	<
		typename TKernelProducerType,
		typename TDataStructureType,
		typename TDataProductionManager = GvStructure::GsDataProductionManager< TDataStructureType >
	>
	class GsSimpleHostProducer;

	// Simple host shader
	template< typename TKernelShaderType >
	class GsSimpleHostShader;

	// Simple Pipeline
	template
	<
		typename TShaderType,
		typename TDataStructureType,
		typename TCacheType = GvStructure::GsDataProductionManager< TDataStructureType >
	>
	class GsSimplePipeline;
}

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#endif // !_GV_FORWARD_DECLARATION_HELPER_H_
