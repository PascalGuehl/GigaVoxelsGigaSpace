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
 * ...
 ******************************************************************************/
__host__ __device__
inline VolTreeNodeAddress::PackedAddressType VolTreeNodeAddress::packAddress( const uint3& address )
{
	return address.x;
}

/******************************************************************************
 * ...
 ******************************************************************************/
__host__ __device__
inline VolTreeNodeAddress::PackedAddressType VolTreeNodeAddress::packAddress( uint address )
{
	return address;
}

/******************************************************************************
 * ...
 ******************************************************************************/
__host__ __device__
inline uint3 VolTreeNodeAddress::unpackAddress( VolTreeNodeAddress::PackedAddressType address )
{
	return make_uint3( address & 0x3FFFFFFF, 0, 0 );
}

/******************************************************************************
 * ...
 ******************************************************************************/
__host__ __device__
inline bool VolTreeNodeAddress::isNull( uint pa )
{
	return (pa & packedMask) == 0;
}

} // namespace GvStructure


/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvStructure
{

/******************************************************************************
 * ...
 ******************************************************************************/
__host__ __device__
inline VolTreeBrickAddress::PackedAddressType VolTreeBrickAddress::packAddress( const uint3& address )
{
	return (address.x << 20 | address.y << 10 | address.z);
}

/******************************************************************************
 * ...
 ******************************************************************************/
__host__ __device__
inline uint3 VolTreeBrickAddress::unpackAddress( VolTreeBrickAddress::PackedAddressType address )
{
	uint3 res;

	res.x = (address & 0x3FF00000) >> 20;
	res.y = (address & 0x000FFC00) >> 10;
	res.z = (address & 0x000003FF);

	return res;
}

/******************************************************************************
 * ...
 ******************************************************************************/
__host__ __device__
inline bool VolTreeBrickAddress::isNull( uint pa )
{
	return (pa & packedMask) == 0;
}

} // namespace GvStructure
