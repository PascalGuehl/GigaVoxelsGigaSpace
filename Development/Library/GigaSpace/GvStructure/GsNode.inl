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
 * Unpack a node address
 *
 * @param pAddress ...
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint3 GsNode::unpackNodeAddress( const uint pAddress )
{
	uint3 res;

	res.x = pAddress & 0x3FFFFFFF;
	res.y = 0;
	res.z = 0;

	return res;
}

/******************************************************************************
 * Pack a node address
 *
 * @param pAddress ...
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint GsNode::packNodeAddress( const uint3 pAddress )
{
	return pAddress.x;
}

/******************************************************************************
 * Unpack a brick address
 *
 * @param pAddress ...
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint3 GsNode::unpackBrickAddress( const uint pAddress )
{
	uint3 res;

	res.x = ( pAddress & 0x3FF00000 ) >> 20;
	res.y = ( pAddress & 0x000FFC00 ) >> 10;
	res.z = ( pAddress & 0x000003FF );

	return res;
}

/******************************************************************************
 * Pack a brick address
 *
 * @param pAddress ...
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint GsNode::packBrickAddress( const uint3 pAddress )
{
	return ( pAddress.x << 20 | pAddress.y << 10 | pAddress.z );
}

/******************************************************************************
 * Set the child nodes address
 *
 * @param dpcoord ...
 ******************************************************************************/
__host__ __device__
__forceinline__ void GsNode::setChildAddress( const uint3 dpcoord )
{
	setChildAddressEncoded( packNodeAddress( dpcoord ) );
}

/******************************************************************************
 * Get the child nodes address
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint3 GsNode::getChildAddress() const
{
	return unpackNodeAddress( childAddress );
}

/******************************************************************************
 * Set the child nodes encoded address
 *
 * @param addr ...
 ******************************************************************************/
__host__ __device__
__forceinline__ void GsNode::setChildAddressEncoded( uint addr )
{
	childAddress = ( childAddress & 0x40000000 ) | ( addr & 0x3FFFFFFF );
}

/******************************************************************************
 * Get the child nodes encoded address
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint GsNode::getChildAddressEncoded() const
{
	return childAddress;
}

/******************************************************************************
 * Tell whether or not the node has children
 *
 * @return a flag telling whether or not the node has children
 ******************************************************************************/
__host__ __device__
__forceinline__ bool GsNode::hasSubNodes() const
{
	return ( ( childAddress & 0x3FFFFFFF ) != 0 );
}

/******************************************************************************
 * Flag the node as terminal or not
 *
 * @param pFlag a flag telling whether or not the node is terminal
 ******************************************************************************/
__host__ __device__
__forceinline__ void GsNode::setTerminal( bool pFlag )
{
	if ( pFlag )
	{
		childAddress = ( childAddress | 0x80000000 );
	}
	else
	{
		childAddress = ( childAddress & 0x7FFFFFFF );
	}
}

/******************************************************************************
 * Tell whether or not the node is terminal
 *
 * @return a flag telling whether or not the node is terminal
 ******************************************************************************/
__host__ __device__
__forceinline__ bool GsNode::isTerminal() const
{
	return ( ( childAddress & 0x80000000 ) != 0 );
}

/******************************************************************************
 * Set the brick address
 *
 * @param dpcoord ...
 ******************************************************************************/
__host__ __device__
__forceinline__ void GsNode::setBrickAddress( const uint3 dpcoord )
{
	setBrickAddressEncoded( packBrickAddress( dpcoord ) );
}

/******************************************************************************
 * Get the brick address
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint3 GsNode::getBrickAddress() const
{
	return unpackBrickAddress( brickAddress );
}

/******************************************************************************
 * Set the brick encoded address
 *
 * @param addr ...
 ******************************************************************************/
__host__ __device__
__forceinline__ void GsNode::setBrickAddressEncoded( const uint addr )
{
	brickAddress = addr;
	setStoreBrick();
}

/******************************************************************************
 * Get the brick encoded address
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
__forceinline__ uint GsNode::getBrickAddressEncoded() const
{
	return brickAddress;
}

/******************************************************************************
 * Flag the node as containing data or not
 *
 * @param pFlag a flag telling whether or not the node contains data
 ******************************************************************************/
__host__ __device__
__forceinline__ void GsNode::setStoreBrick()
{
	childAddress = childAddress | 0x40000000;
}

/******************************************************************************
 * Tell whether or not the node is a brick
 *
 * @return a flag telling whether or not the node is a brick
 ******************************************************************************/
__host__ __device__
__forceinline__ bool GsNode::isBrick() const
{
	return ( ( childAddress & 0x40000000 ) != 0 );
}

/******************************************************************************
 * Tell whether or not the node has a brick,
 * .i.e the node is a brick and its brick address is not null.
 *
 * @return a flag telling whether or not the node has a brick
 ******************************************************************************/
__host__ __device__
__forceinline__ bool GsNode::hasBrick() const
{
	return ( brickAddress != 0 ) && ( ( childAddress & 0x40000000 ) != 0 );
}

/******************************************************************************
 * Tell whether or not the node is initialized
 *
 * @return a flag telling whether or not the node is initialized
 ******************************************************************************/
__host__ __device__
__forceinline__ bool GsNode::isInitializated() const
{
	return ( ( childAddress != 0 ) || ( brickAddress != 0 ) );
}

} // namespace GvStructure
