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

namespace GvCore
{

// Localization Code

/******************************************************************************
 * Set the localization code value
 *
 * @param plocalizationCode The localization code value
 ******************************************************************************/
__host__ __device__
inline void GvLocalizationCode::set( uint3 plocalizationCode )
{
	_localizationCode = plocalizationCode;
}

/******************************************************************************
 * Get the localization code value
 *
 * @return The localization code value
 ******************************************************************************/
__host__ __device__
inline uint3 GvLocalizationCode::get() const
{
	return _localizationCode;
}

/******************************************************************************
 * Given the current localization code and an offset in a node tile
 *
 * @param pOffset The offset in a node tile
 *
 * @return ...
 ******************************************************************************/
template< typename TNodeTileResolution >
__host__ __device__
inline GvLocalizationCode GvLocalizationCode::addLevel( uint3 pOffset ) const
{
	uint3 localizationCode;
	localizationCode.x = _localizationCode.x << TNodeTileResolution::xLog2 | pOffset.x;
	localizationCode.y = _localizationCode.y << TNodeTileResolution::yLog2 | pOffset.y;
	localizationCode.z = _localizationCode.z << TNodeTileResolution::zLog2 | pOffset.z;

	GvLocalizationCode result;
	result.set( localizationCode );

	return result;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
template< typename TNodeTileResolution >
__host__ __device__
inline GvLocalizationCode GvLocalizationCode::removeLevel() const
{
	uint3 localizationCode;
	localizationCode.x = _localizationCode.x >> TNodeTileResolution::xLog2;
	localizationCode.y = _localizationCode.y >> TNodeTileResolution::yLog2;
	localizationCode.z = _localizationCode.z >> TNodeTileResolution::zLog2;

	GvLocalizationCode result;
	result.set( localizationCode );

	return result;
}

} // namespace GvCore

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvCore
{

// Localization Depth

/******************************************************************************
 * Get the localization depth value
 *
 * @return The localization depth value
 ******************************************************************************/
__host__ __device__
inline uint GvLocalizationDepth::get() const
{
	return _localizationDepth;
}

/******************************************************************************
 * Set the localization depth value
 *
 * @param pLocalizationDepth The localization depth value
 ******************************************************************************/
__host__ __device__
inline void GvLocalizationDepth::set( uint pLocalizationDepth )
{
	_localizationDepth = pLocalizationDepth;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
inline GvLocalizationDepth GvLocalizationDepth::addLevel() const
{
	GvLocalizationDepth result;
	result.set( _localizationDepth + 1 );

	return result;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
__host__ __device__
inline GvLocalizationDepth GvLocalizationDepth::removeLevel() const
{
	GvLocalizationDepth result;
	result.set( _localizationDepth - 1 );

	return result;
}

} // namespace GvCore
