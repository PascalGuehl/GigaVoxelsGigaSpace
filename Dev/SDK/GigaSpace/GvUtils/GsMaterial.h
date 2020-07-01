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

#ifndef _GV_MATERIAL_H_
#define _GV_MATERIAL_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvUtils/GsMaterialKernel.h"

// CUDA
#include <vector_types.h>

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

namespace GvUtils
{

/** 
 * @class GsMaterial
 *
 * @brief The GsMaterial class provides interface to materials.
 *
 * ...
 */
class GsMaterial
{

	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/****************************** INNER TYPES *******************************/

	/**
	 * Enumeration of different types of materials
	 */
	enum EMaterialType
	{
		eNbMaterialTypes
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	GsMaterial();

	/**
	 * Destructor
	 */
	virtual ~GsMaterial();

	/**
	 * Get the emissive color
	 *
	 * @return the emissive color
	 */
	const float3& getKe() const;

	/**
	 * Set the emissive color
	 *
	 * @param pValue the emissive color
	 */
	void setKe( const float3& pValue );

	/**
	 * Get the ambient color
	 *
	 * @return the ambient color
	 */
	const float3& getKa() const;

	/**
	 * Set the ambient color
	 *
	 * @param pValue the ambient color
	 */
	void setKa( const float3& pValue );

	/**
	 * Get the diffuse color
	 *
	 * @return the diffuse color
	 */
	const float3& getKd() const;

	/**
	 * Set the diffuse color
	 *
	 * @param pValue the diffuse color
	 */
	void setKd( const float3& pValue );

	/**
	 * Get the specular color
	 *
	 * @return the specular color
	 */
	const float3& getKs() const;

	/**
	 * Set the specular color
	 *
	 * @param pValue the specular color
	 */
	void setKs( const float3& pValue );

	/**
	 * Get the shininess
	 *
	 * @return the shininess
	 */
	float getShininess() const;

	/**
	 * Set the shininess
	 *
	 * @param pValue the shininess
	 */
	void setShininess( float pValue );

	/**
	 * Get the associated device-side object
	 *
	 * @return the associated device-side object
	 */
	const GsMaterialKernel& getKernelObject() const;

	/**
	 * Get the associated device-side object
	 *
	 * @return the associated device-side object
	 */
	GsMaterialKernel& editKernelObject();

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:

	/****************************** INNER TYPES *******************************/

	/******************************* ATTRIBUTES *******************************/

	/**
	 * Material's emissive color
	 */
	float3 _Ke;

	/**
	 * Material's ambient color
	 */
	float3 _Ka;

	/**
	 * Material's diffuse color
	 */
	float3 _Kd;

	/**
	 * Material's specular color
	 */
	float3 _Ks;

	/**
	 * Shininess, associated to specular term, tells how shiny the surface is
	 */
	float _shininess;

	/**
	 * Associated device-side object
	 */
	GsMaterialKernel _kernelObject;
	
	/******************************** METHODS *********************************/

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
	GsMaterial( const GsMaterial& );

	/**
	 * Copy operator forbidden.
	 */
	GsMaterial& operator=( const GsMaterial& );

};

} // namespace GvUtils

/**************************************************************************
 ***************************** INLINE SECTION *****************************
 **************************************************************************/

#include "GsMaterial.inl"

#endif
