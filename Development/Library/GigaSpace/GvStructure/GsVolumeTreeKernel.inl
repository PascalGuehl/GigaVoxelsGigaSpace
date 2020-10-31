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

namespace GvStructure
{

/******************************************************************************
 * Sample data in specified channel at a given position.
 * 3D texture are used with hardware tri-linear interpolation.
 *
 * @param pBrickPos Brick position in the pool of bricks
 * @param pPosInBrick Position in brick
 *
 * @return the sampled value
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
template< int TChannel >
__device__
__forceinline__ float4 VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::getSampleValueTriLinear( float3 pBrickPos, float3 pPosInBrick ) const
{
	// Type definition of the data channel type
	typedef typename GvCore::DataChannelType< DataTList, TChannel >::Result ChannelType;

	float4 result = make_float4( 0.0f );
	float3 samplePos = pBrickPos + pPosInBrick;

	// Sample data in texture according to its type (float or not)
	if ( ! ( GvCore::IsFloatFormat< ChannelType >::value ) )
	{
		gpuPoolTexFetch( TEXDATAPOOL, TChannel, 3, ChannelType, cudaReadModeNormalizedFloat, samplePos, result );
	}
	else
	{
		gpuPoolTexFetch( TEXDATAPOOL, TChannel, 3, ChannelType, cudaReadModeElementType, samplePos, result );
	}

	return result;
}

/******************************************************************************
 * Sample data in specified channel at a given position.
 * 3D texture are used with hardware tri-linear interpolation.
 *
 * @param mipMapInterpCoef mipmap interpolation coefficient
 * @param brickChildPosInPool brick child position in pool
 * @param brickParentPosInPool brick parent position in pool
 * @param posInBrick position in brick
 * @param coneAperture cone aperture
 *
 * @return the sampled value
 ******************************************************************************/
// QUESTION : le paramètre "coneAperture" ne semble pas utilisé ? A quoi sert-il (servait ou servira) ?
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
template< int TChannel >
__device__
__forceinline__ float4 VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::getSampleValueQuadriLinear( float mipMapInterpCoef, float3 brickChildPosInPool, float3 brickParentPosInPool, float3 posInBrick, float coneAperture ) const
{
	float3 samplePos0 = posInBrick;
	float3 samplePos1 = posInBrick / NodeTileRes::getFloat3();//* 0.5f;

	float4 vox0, vox1;

	// Sample data in brick
	vox1 = getSampleValueTriLinear< TChannel >( brickParentPosInPool, samplePos1 );

	if ( mipMapInterpCoef <= 1.0f )
	{
		// Sample data in child brick
		vox0 = getSampleValueTriLinear< TChannel >( brickChildPosInPool, samplePos0 );

		// Linear interpolation of results
		vox1 = lerp( vox0, vox1, mipMapInterpCoef );
	}

	return vox1;
}

/******************************************************************************
 * Sample data in specified channel at a given position.
 * 3D texture are used with hardware tri-linear interpolation.
 *
 * @param mipMapInterpCoef mipmap interpolation coefficient
 * @param brickChildPosInPool brick child position in pool
 * @param brickParentPosInPool brick parent position in pool
 * @param posInBrick position in brick
 * @param coneAperture cone aperture
 *
 * @return the sampled value
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
template< int TChannel >
__device__
__forceinline__ float4 VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::getSampleValue( float3 brickChildPosInPool, float3 brickParentPosInPool, float3 sampleOffsetInBrick, float coneAperture, bool mipMapOn, float mipMapInterpCoef ) const
{
	float4 vox;

	// Sample data in texture
	if ( mipMapOn && mipMapInterpCoef > 0.0f )
	{
		vox = getSampleValueQuadriLinear< TChannel >( mipMapInterpCoef,
			brickChildPosInPool, brickParentPosInPool, sampleOffsetInBrick,
			coneAperture );
	}
	else
	{
		vox = getSampleValueTriLinear< TChannel >( brickChildPosInPool, sampleOffsetInBrick );
	}

	return vox;
}

/******************************************************************************
 * ...
 *
 * @param nodeTileAddress ...
 * @param nodeOffset ...
 *
 * @return ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ uint VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::computenodeAddress( uint3 nodeTileAddress, uint3 nodeOffset ) const
{
	return ( nodeTileAddress.x + NodeResolution::toFloat1( nodeOffset ) );
}

/******************************************************************************
 * ...
 *
 * @param nodeTileAddress ...
 * @param nodeOffset ...
 *
 * @return ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ uint3 VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::computeNodeAddress( uint3 nodeTileAddress, uint3 nodeOffset ) const
{
	return make_uint3( nodeTileAddress.x + NodeResolution::toFloat1( nodeOffset ), 0, 0 );
}

/******************************************************************************
 * Retrieve node information (address + flags) from data structure
 *
 * @param resnode ...
 * @param nodeTileAddress ...
 * @param nodeOffset ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::fetchNode( GsNode& resnode, uint3 nodeTileAddress, uint3 nodeOffset ) const
{
	uint nodeAddress = computenodeAddress(nodeTileAddress, nodeOffset);

#if USE_LINEAR_VOLTREE_TEX
	resnode.childAddress = tex1Dfetch( volumeTreeChildTexLinear, nodeAddress );
	resnode.brickAddress = tex1Dfetch( volumeTreeDataTexLinear, nodeAddress );
#else // USE_LINEAR_VOLTREE_TEX
	resnode.childAddress = k_volumeTreeChildArray.get( nodeAddress );
	resnode.brickAddress = k_volumeTreeDataArray.get( nodeAddress );
#ifdef GS_USE_NODE_META_DATA
	resnode._objectID = k_dataStructureMetaDataArray.get( nodeAddress );
#endif
#endif // USE_LINEAR_VOLTREE_TEX
		
#ifdef GV_USE_BRICK_MINMAX
	resnode.metaDataAddress = nodeAddress;
#endif
}

/******************************************************************************
 * Retrieve node information (address + flags) from data structure
 *
 * @param resnode ...
 * @param nodeTileAddress ...
 * @param nodeOffset ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::fetchNode( GsNode& resnode, uint nodeTileAddress, uint nodeOffset ) const
{
	uint nodeAddress = nodeTileAddress + nodeOffset;

#if USE_LINEAR_VOLTREE_TEX
	resnode.childAddress = tex1Dfetch(volumeTreeChildTexLinear, nodeAddress);
	resnode.brickAddress = tex1Dfetch(volumeTreeDataTexLinear, nodeAddress);
#else //USE_LINEAR_VOLTREE_TEX
	resnode.childAddress = k_volumeTreeChildArray.get( nodeAddress );
	resnode.brickAddress = k_volumeTreeDataArray.get( nodeAddress );
#ifdef GS_USE_NODE_META_DATA
	resnode._objectID = k_dataStructureMetaDataArray.get( nodeAddress );
#endif
#endif //USE_LINEAR_VOLTREE_TEX
		
#ifdef GV_USE_BRICK_MINMAX
	resnode.metaDataAddress = nodeAddress;
#endif
}

/******************************************************************************
 * Retrieve node information (address + flags) from data structure
 *
 * @param resnode ...
 * @param nodeAddress ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::fetchNode( GsNode& resnode, uint nodeAddress ) const
{
#if USE_LINEAR_VOLTREE_TEX
	resnode.childAddress = tex1Dfetch(volumeTreeChildTexLinear, nodeAddress);
	resnode.brickAddress = tex1Dfetch(volumeTreeDataTexLinear, nodeAddress);
#else //USE_LINEAR_VOLTREE_TEX
	resnode.childAddress = k_volumeTreeChildArray.get(nodeAddress);
	resnode.brickAddress = k_volumeTreeDataArray.get(nodeAddress);
#ifdef GS_USE_NODE_META_DATA
	resnode._objectID = k_dataStructureMetaDataArray.get( nodeAddress );
#endif
#endif //USE_LINEAR_VOLTREE_TEX

#ifdef GV_USE_BRICK_MINMAX
	resnode.metaDataAddress = nodeAddress;
#endif
}

///******************************************************************************
// * ...
// *
// * @param resnode ...
// * @param nodeTileAddress ...
// * @param nodeOffset ...
// ******************************************************************************/
//template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
//__device__
//__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
//::fetchNodeChild( GsNode& resnode, uint nodeTileAddress, uint nodeOffset )
//{
//	uint address = nodeTileAddress + nodeOffset;
//
//#if USE_LINEAR_VOLTREE_TEX
//	resnode.childAddress = tex1Dfetch( volumeTreeChildTexLinear, address );
//#else //USE_LINEAR_VOLTREE_TEX
//	resnode.childAddress = k_volumeTreeChildArray.get( address );
//#endif //USE_LINEAR_VOLTREE_TEX
//}
//
///******************************************************************************
// * ...
// *
// * @param resnode ...
// * @param nodeTileAddress ...
// * @param nodeOffset ...
// ******************************************************************************/
//template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
//__device__
//__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
//::fetchNodeData( GsNode& resnode, uint nodeTileAddress, uint nodeOffset )
//{
//	uint address = nodeTileAddress + nodeOffset;
//
//#if USE_LINEAR_VOLTREE_TEX
//	resnode.brickAddress = tex1Dfetch( volumeTreeDataTexLinear, address );
//#else //USE_LINEAR_VOLTREE_TEX
//	resnode.brickAddress = k_volumeTreeDataArray.get( address );
//#endif //USE_LINEAR_VOLTREE_TEX
//}

/******************************************************************************
 * Write node information (address + flags) in data structure
 *
 * @param node ...
 * @param nodeTileAddress ...
 * @param nodeOffset ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::setNode( GsNode node, uint3 nodeTileAddress, uint3 nodeOffset )
{
	const uint3 nodeAddress = computeNodeAddress( nodeTileAddress, nodeOffset );

	k_volumeTreeChildArray.set( nodeAddress, node.childAddress );
	k_volumeTreeDataArray.set( nodeAddress, node.brickAddress );
#ifdef GS_USE_NODE_META_DATA
	k_dataStructureMetaDataArray.set( nodeAddress, node._objectID );
#endif
}

/******************************************************************************
 * Write node information (address + flags) in data structure
 *
 * @param node ...
 * @param nodeTileAddress ...
 * @param nodeOffset ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::setNode( GsNode node, uint nodeTileAddress, uint nodeOffset )
{
	const uint nodeAddress = nodeTileAddress + nodeOffset;

	k_volumeTreeChildArray.set( nodeAddress, node.childAddress );
	k_volumeTreeDataArray.set( nodeAddress, node.brickAddress );
#ifdef GS_USE_NODE_META_DATA
	k_dataStructureMetaDataArray.set( nodeAddress, node._objectID );
#endif
}

/******************************************************************************
 * Write node information (address + flags) in data structure
 *
 * @param node ...
 * @param nodeAddress ...
 ******************************************************************************/
template< class DataTList, class NodeTileRes, class BrickRes, uint BorderSize >
__device__
__forceinline__ void VolumeTreeKernel< DataTList, NodeTileRes, BrickRes, BorderSize >
::setNode( GsNode node, uint nodeAddress )
{
	k_volumeTreeChildArray.set( nodeAddress, node.childAddress );
	k_volumeTreeDataArray.set( nodeAddress, node.brickAddress );
#ifdef GS_USE_NODE_META_DATA
	k_dataStructureMetaDataArray.set( nodeAddress, node._objectID );
#endif
}

} // namespace GvStructure
