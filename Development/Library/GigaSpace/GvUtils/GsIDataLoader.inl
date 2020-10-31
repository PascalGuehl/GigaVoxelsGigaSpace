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

namespace GvUtils
{

/******************************************************************************
 * Constructor
 ******************************************************************************/
template< typename TDataTypeList >
GsIDataLoader< TDataTypeList >
::GsIDataLoader()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TDataTypeList >
GsIDataLoader< TDataTypeList >
::~GsIDataLoader()
{
}

/******************************************************************************
 * Helper function used to determine the type of regions in the data structure.
 * The data structure is made of regions containing data, empty or constant regions.
 *
 * Retrieve the node and associated brick located in this region of space,
 * and depending of its type, if it contains data, load it.
 *
 * @param pPosition position of a region of space
 * @param pSize size of a region of space
 * @param pBrickPool data cache pool. This is where all data reside for each channel (color, normal, etc...)
 * @param pOffsetInPool offset in the brick pool
 *
 * @return the type of the region (.i.e returns constantness information for that region)
 ******************************************************************************/
template< typename TDataTypeList >
inline GsIDataLoader< TDataTypeList >::VPRegionInfo GsIDataLoader< TDataTypeList >
::getRegion( const float3& pPosition, const float3& pSize, GvCore::GPUPoolHost< GvCore::Array3D, TDataTypeList >* pBrickPool, size_t pOffsetInPool )
{
	return VP_UNKNOWN_REGION;
}

/******************************************************************************
 * Provides constantness information about a region. Resolution is here for compatibility. TODO:Remove resolution.
 *
 * @param pPosition position of a region of space
 * @param pSize size of a region of space
 *
 * @return the type of the region (.i.e returns constantness information for that region)
 ******************************************************************************/
template< typename TDataTypeList >
inline GsIDataLoader< TDataTypeList >::VPRegionInfo GsIDataLoader< TDataTypeList >
::getRegionInfo( const float3& pPosition, const float3& pSize/*, T *constValueOut = NULL*/ )
{
	return VP_UNKNOWN_REGION;
}

/******************************************************************************
 * Retrieve the node located in a region of space,
 * and get its information (i.e. address containing its data type region).
 *
 * @param pPosition position of a region of space
 * @param pSize size of a region of space
 *
 * @return the node encoded information
 ******************************************************************************/
template< typename TDataTypeList >
inline uint GsIDataLoader< TDataTypeList >
::getRegionInfoNew( const float3& pPosition, const float3& pSize )
{
	return 0;
}

/******************************************************************************
 * Provides the size of the smallest features the producer can generate.
 *
 * @return the size of the smallest features the producer can generate.
 ******************************************************************************/
template< typename TDataTypeList >
inline float3 GsIDataLoader< TDataTypeList >
::getFeaturesSize() const
{
	return make_float3( 0.f );
}

/******************************************************************************
 * Set the region resolution.
 *
 * @param pResolution resolution
 *
 * @return ...
 ******************************************************************************/
template< typename TDataTypeList >
inline int GsIDataLoader< TDataTypeList >
::setRegionResolution( const uint3& pResolution )
{
	return 0;
}

/******************************************************************************
 * Get the data resolution
 *
 * @return the data resolution
 ******************************************************************************/
template< typename TDataTypeList >
inline uint3 GsIDataLoader< TDataTypeList >
::getDataResolution() const
{
	return make_uint3( 0, 0, 0 );
}

/******************************************************************************
 * Get the flag telling wheter or not to use cache on CPU to load all dataset content.
 * Note : this may require a huge memory consumption.
 *
 * @return pFlag Flag to tell wheter or not to use cache on CPU to load all dataset content.
 ******************************************************************************/
template< typename TDataTypeList >
bool GsIDataLoader< TDataTypeList >
::isHostCacheActivated() const
{
	return false;
}

/******************************************************************************
 * Set the flag to tell wheter or not to use cache on CPU to load all dataset content.
 * Note : this may require a huge memory consumption.
 *
 * @param pFlag Flag to tell wheter or not to use cache on CPU to load all dataset content.
 ******************************************************************************/
template< typename TDataTypeList >
void GsIDataLoader< TDataTypeList >
::setHostCacheActivated( bool pFlag )
{
}

} // namespace GvUtils
