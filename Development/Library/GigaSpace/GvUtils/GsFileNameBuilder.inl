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
 *
 * @param brickSize Brick size
 * @param borderSize Border size
 * @param level Level
 * @param fileName File name
 * @param fileExt File extension
 * @param result List of built filenames
 ******************************************************************************/
template< typename TDataTypeList >
inline GsFileNameBuilder< TDataTypeList >
::GsFileNameBuilder( uint pBrickSize, uint pBorderSize, uint pLevel, const std::string& pFileName,
					  const std::string& pFileExt, std::vector< std::string >& pResult )
:	mBrickSize( pBrickSize )
,	mBorderSize( pBorderSize )
,	mLevel( pLevel )
,	mFileName( pFileName )
,	mFileExt( pFileExt )
,	mResult( &pResult )
{
}

/******************************************************************************
 * ...
 *
 * @param Loki::Int2Type< TChannel > ...
 ******************************************************************************/
template< typename TDataTypeList >
template< int TChannel >
inline void GsFileNameBuilder< TDataTypeList >
::run( Loki::Int2Type< TChannel > )
{
	// Typedef to access the channel in the data type list
	typedef typename Loki::TL::TypeAt< TDataTypeList, TChannel >::Result ChannelType;

	// Build filename according to GigaVoxels internal syntax
	std::stringstream filename;
	filename << mFileName << "_BR" << mBrickSize << "_B" << mBorderSize << "_L" << mLevel
		<< "_C" << TChannel << "_" << GvCore::typeToString< ChannelType >() << mFileExt;

	// Store generated filename
	mResult->push_back( filename.str() );
}

} // namespace GvUtils
