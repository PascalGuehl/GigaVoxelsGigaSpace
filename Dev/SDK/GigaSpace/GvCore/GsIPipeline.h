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

#ifndef _GV_I_PIPELINE_H_
#define _GV_I_PIPELINE_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsISerializable.h"

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

namespace GvCore
{

/** 
 * @class GsIPipeline
 *
 * @brief The GsIPipeline class provides the interface to manage GigaSpace pipelines
 * (i.e. data structure, cache, producers, renders, etc...)
 * 
 * @ingroup GvCore
 *
 * This class is the base class for all pipeline objects.
 */
class GIGASPACE_EXPORT GsIPipeline : public GvCore::GsISerializable
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Cache overflow policy
	 */
	enum ECacheOverflowPolicy
	{
		eCacheOverflow_Default = 0,
		eCacheOverflow_QualityFirst = 1,
		eCacheOverflow_FranckLag = 1 << 1,
		eCacheOverflow_DecreaseResolution = 1 << 2,
		eCacheOverflow_PriorityInProduction = 1 << 3,
		eCacheOverflow_All = ( 1 << 4 ) - 1
	};

	/**
	 * Cache full policy
	 */
	enum ECacheFullPolicy
	{
		eCacheFull_Default = 0,
		eCacheFull_QualityFirst = 1,
		eCacheFull_LockCache = 1 << 1,
		eCacheFull_LockLastFrameCache = 1 << 2,
		eCacheFull_PriorityInFreeing = 1 << 3,
		eCacheFull_All = ( 1 << 4 ) - 1
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Destructor
	 */
	virtual ~GsIPipeline();

	/**
	 * Set the cache overflow policy
	 *
	 * @param pPolicy the cache overflow policy
	 */
	void setCacheOverflowPolicy( ECacheOverflowPolicy pPolicy );

	/**
	 * Get the cache overflow policy
	 *
	 * @return the cache overflow policy
	 */
	ECacheOverflowPolicy getCacheOverflowPolicy() const;

	/**
	 * Set the cache full policy
	 *
	 * @param pPolicy the cache full policy
	 */
	void setCacheFullPolicy( ECacheFullPolicy pPolicy );

	/**
	 * Get the cache full policy
	 *
	 * @return the cache full policy
	 */
	ECacheFullPolicy getCacheFullPolicy() const;

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Cache overflow policy
	 */
	ECacheOverflowPolicy _cacheOverflowPolicy;

	/**
	 * Cache full policy
	 */
	ECacheFullPolicy _cacheFullPolicy;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsIPipeline();

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Copy constructor forbidden.
	 */
	GsIPipeline( const GsIPipeline& );

	/**
	 * Copy operator forbidden.
	 */
	GsIPipeline& operator=( const GsIPipeline& );

};

} // namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsIPipeline.inl"

#endif // !_GV_I_PIPELINE_H_
