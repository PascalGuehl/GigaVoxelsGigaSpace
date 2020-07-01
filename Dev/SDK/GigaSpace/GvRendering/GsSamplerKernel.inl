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

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvRendering
{

/******************************************************************************
 * Sample data at given cone aperture
 *
 * @param coneAperture the cone aperture
 *
 * @return the sampled value
 ******************************************************************************/
template< typename VolumeTreeKernelType >
template< int channel >
__device__
__forceinline__ float4 GsSamplerKernel< VolumeTreeKernelType >::getValue( const float coneAperture ) const
{
	return _volumeTree->template getSampleValue< channel >( _brickChildPosInPool, _brickParentPosInPool, _scaleTree2BrickPool * _sampleOffsetInNodeTree,
														   coneAperture,
														   _mipMapOn, _mipMapInterpCoef );
}

/******************************************************************************
 * Sample data at given cone aperture and offset in tree
 *
 * @param coneAperture the cone aperture
 * @param offsetTree the offset in the tree
 *
 * @return the sampled value
 ******************************************************************************/
template< typename VolumeTreeKernelType >
template< int channel >
__device__
__forceinline__ float4 GsSamplerKernel< VolumeTreeKernelType >::getValue( const float coneAperture, const float3 offsetTree ) const
{
	return _volumeTree->template getSampleValue< channel >( _brickChildPosInPool, _brickParentPosInPool, _scaleTree2BrickPool * ( _sampleOffsetInNodeTree + offsetTree ),
														   coneAperture,
														   _mipMapOn, _mipMapInterpCoef );
}

/******************************************************************************
 * Move sample offset in node tree
 *
 * @param offsetTree offset in tree
 ******************************************************************************/
template< typename VolumeTreeKernelType >
__device__
__forceinline__ void GsSamplerKernel< VolumeTreeKernelType >::moveSampleOffsetInNodeTree( const float3 offsetTree )
{
	_sampleOffsetInNodeTree = _sampleOffsetInNodeTree + offsetTree;
}

/******************************************************************************
 * Update MipMap parameters given cone aperture
 *
 * @param coneAperture the cone aperture
 *
 * @return It returns false if coneAperture > voxelSize in parent brick
 ******************************************************************************/
template< typename VolumeTreeKernelType >
__device__
__forceinline__ bool GsSamplerKernel< VolumeTreeKernelType >::updateMipMapParameters( const float pConeAperture )
{
	_mipMapInterpCoef = 0.0f;

	if ( _mipMapOn )
	{
		_mipMapInterpCoef = getMipMapInterpCoef< VolumeTreeKernelType::NodeResolution, VolumeTreeKernelType::BrickResolution >( pConeAperture, _nodeSizeTree );
		if ( _mipMapInterpCoef > 1.0f )
		{
			return false;
		}
	}

	return true;
}

} // namespace GvRendering
