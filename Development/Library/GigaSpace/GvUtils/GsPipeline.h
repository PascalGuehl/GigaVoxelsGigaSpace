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

#ifndef _GV_PIPELINE_H_
#define _GV_PIPELINE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaSpace
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsIPipeline.h"
#include "GvCore/GsVectorTypesExt.h"

// Cuda
#include <vector_types.h>

// System
#include <cassert>

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// GigaSpace
namespace GvCore
{
	class GsIProvider;
}
namespace GvStructure
{
	class GsIDataStructure;
	class GsIDataProductionManager;
}
namespace GvRendering
{
	class GsIRenderer;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

 namespace GvUtils
 {
 
/** 
 * @class GsPipeline
 *
 * @brief The GsPipeline class provides the interface to manage GigaSpace pipelines
 * (i.e. data structure, cache, producers, renders, etc...)
 * 
 * @ingroup GvUtils
 *
 * This class is ...
 */
class GIGASPACE_EXPORT GsPipeline : public GvCore::GsIPipeline
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Destructor
	 */
	virtual ~GsPipeline();

	/**
	 * Initialize
	 *
	 * @return pDataStructure the associated data structure
	 * @return pProducer the associated producer
	 * @return pRenderer the associated  renderer
	 */
	//virtual void initialize( GvStructure::GsIDataStructure* pDataStructure, GvCore::GsIProvider* pProducer, GvRendering::GsIRenderer* pRenderer );

	/**
	 * Finalize
	 */
	virtual void finalize();
		
	/**
	 * Launch the main GigaSpace flow sequence
	 */
	//virtual void execute();
	//virtual void execute( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport );

	/**
	 * Get the data structure
	 *
	 * @return The data structure
	 */
	virtual const GvStructure::GsIDataStructure* getDataStructure() const;

	/**
	 * Get the data structure
	 *
	 * @return The data structure
	 */
	virtual GvStructure::GsIDataStructure* editDataStructure();

	/**
	 * Get the data production manager
	 *
	 * @return The data production manager
	 */
	virtual const GvStructure::GsIDataProductionManager* getCache() const;

	/**
	 * Get the data production manager
	 *
	 * @return The data production manager
	 */
	virtual GvStructure::GsIDataProductionManager* editCache();

	/**
	 * Get the producer
	 *
	 * @return The producer
	 */
	virtual const GvCore::GsIProvider* getProducer( unsigned int pIndex = 0 ) const;

	/**
	 * Get the producer
	 *
	 * @return The producer
	 */
	virtual GvCore::GsIProvider* editProducer( unsigned int pIndex = 0 );

	/**
	 * Get the renderer
	 *
	 * @return The renderer
	 */
	virtual const GvRendering::GsIRenderer* getRenderer( unsigned int pIndex = 0 ) const;

	/**
	 * Get the renderer
	 *
	 * @return The renderer
	 */
	virtual GvRendering::GsIRenderer* editRenderer( unsigned int pIndex );

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/******************************* ATTRIBUTES *******************************/
	
	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsPipeline();

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsPipeline( const GsPipeline& );

	/**
	 * Copy operator forbidden.
	 */
	GsPipeline& operator=( const GsPipeline& );

	/******************************** METHODS *********************************/

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsPipeline.inl"

#endif // !_GV_PIPELINE_H_
