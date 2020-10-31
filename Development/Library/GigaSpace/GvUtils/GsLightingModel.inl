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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
//#include "GvRendering/GsRendererContext.h"

/******************************************************************************
 ****************************** KERNEL DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{

/**
	 * Do the shading equation at a givent point
	 *
	 * @param pMaterialColor material color
	 * @param pNormalVec normal
	 * @param pLightVec light vector
	 * @param pEyeVec eye vector
	 * @param pAmbientTerm ambiant term
	 * @param pDiffuseTerm diffuse term
	 * @param pSpecularTerm specular term
	 *
	 * @return the computed color
	 */
	__device__
	static __forceinline__ float3 ambientLightingModel( const float3 pMaterialColor, const float3 pAmbientTerm )
	{
		// Ambient
		return pMaterialColor * pAmbientTerm;
	}

	/**
	 * Do the shading equation at a givent point
	 *
	 * @param pMaterialColor material color
	 * @param pNormalVec normal
	 * @param pLightVec light vector
	 * @param pEyeVec eye vector
	 * @param pAmbientTerm ambiant term
	 * @param pDiffuseTerm diffuse term
	 * @param pSpecularTerm specular term
	 *
	 * @return the computed color
	 */
	__device__
	static __forceinline__ float3 ambientAndDiffuseLightingModel( const float3 pMaterialColor, const float3 pNormalVec, const float3 pLightVec,
								const float3 pAmbientTerm, const float3 pDiffuseTerm )
	{
		// Ambient
		float3 final_color = pMaterialColor * pAmbientTerm;

		// Diffuse
		//float lightDist = length(pLightVec);
		float3 lightVecNorm = ( pLightVec );
		const float lambertTerm = dot( pNormalVec, lightVecNorm );
		if ( lambertTerm > 0.0f )
		{
			// Diffuse
			final_color += pMaterialColor * pDiffuseTerm * lambertTerm;
		}

		return final_color;
	}

	/**
	 * Do the shading equation at a givent point
	 *
	 * @param pMaterialColor material color
	 * @param pNormalVec normal
	 * @param pLightVec light vector
	 * @param pEyeVec eye vector
	 * @param pAmbientTerm ambiant term
	 * @param pDiffuseTerm diffuse term
	 * @param pSpecularTerm specular term
	 *
	 * @return the computed color
	 */
	__device__
	static __forceinline__ float3 ADSLightingModel( const float3 pMaterialColor, const float3 pNormalVec, const float3 pLightVec, const float3 pEyeVec,
								const float3 pAmbientTerm, const float3 pDiffuseTerm, const float3 pSpecularTerm )
	{
		// Ambient
		float3 final_color = pMaterialColor * pAmbientTerm;

		// Diffuse
		//float lightDist=length(pLightVec);
		float3 lightVecNorm = ( pLightVec );
		float lambertTerm = ( dot( pNormalVec, lightVecNorm ) );
		if ( lambertTerm > 0.0f )
		{
			// Diffuse
			final_color += pMaterialColor * pDiffuseTerm * lambertTerm;

			// Specular
			float3 halfVec = normalize( lightVecNorm + pEyeVec );//*0.5f;
			float specular = __powf( max( dot( pNormalVec, halfVec ), 0.0f ), 64.0f );
			final_color += make_float3( specular ) * pSpecularTerm;
		}

		return final_color;
	}

/******************************************************************************
 * This method is called for each sample. For example, shading or secondary rays
 * should be done here.
 *
 * @param pBrickSampler brick sampler
 * @param pSamplePosScene position of the sample in the scene
 * @param pRayDir ray direction
 * @param pRayStep ray step
 * @param pConeAperture cone aperture
 ******************************************************************************/
template< typename SamplerType >
__device__
__forceinline__ void GsCommonShaderKernel::runImpl( const SamplerType& brickSampler, const float3 samplePosScene,
										const float3 rayDir, float& rayStep, const float coneAperture )
{
}

} // namespace GvUtils
