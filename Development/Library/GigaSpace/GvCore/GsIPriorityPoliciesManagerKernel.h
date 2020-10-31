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

#ifndef _GV_I_PRIORITY_POLICIES_MANAGER_KERNEL_H_
#define _GV_I_PRIORITY_POLICIES_MANAGER_KERNEL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/
	
/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @struct GsPriorityParameters
 * 
 * @brief this struct provides the common parameters used for priority management.
 *
 * @ingroup GvCore
 */
struct GsPriorityParameters
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Cone aperture
	 */
	float _coneAperture;
	
	/**
	 * Node size
	 */
	float _nodeSize;
	
	/**
	 * Node depth
	 */
	int _nodeDepth;

	/******************************** METHODS *********************************/

};

} // namespace GvCore

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace GvCore
{

/** 
 * @class GsIPriorityPoliciesManagerKernel
 * 
 * @brief this class provides the interface for class wanting to provide priority
 * management.
 *
 * @ingroup GvCore
 * 
 * Interface that must be implemented for priority policies managers.
 * This class uses the curiously recurring template pattern.
 */
template< class TDerived >
class GsIPriorityPoliciesManagerKernel
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Return the priority to use for a given node.
	 *
	 * @param pParams the priority parameters
	 *
	 * @return the priority for the node.
	 */
	__device__ __forceinline__
	static int getNodePriority( const GsPriorityParameters& pParams );

	/**
	 * Return the priority to use for a given brick.
	 *
	 * @param pParams the priority parameters
	 *
	 * @return the priority for the brick.
	 */
	__device__ __forceinline__
	static int getBrickPriority( const GsPriorityParameters& pParams );

	/**
	 * Return a flag telling whether or not to use priority.
	 *
	 * @return A flag telling whether or not to use priority
	 */
	__host__ __device__ __forceinline__
	static bool usePriority();

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/
				
};

} // namespace GvCore

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsIPriorityPoliciesManagerKernel.inl"

#endif // !_GV_I_PRIORITY_POLICIES_MANAGER_KERNEL_H_
