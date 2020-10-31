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

namespace GvPerfMon
{

/******************************************************************************
 * ...
 *
 * @param frameRes ...
 ******************************************************************************/
inline void CUDAPerfMon::frameResized( uint2 frameRes )
{
	if ( d_timersArray )
	{
		delete d_timersArray;
		d_timersArray = 0;
	}

	if ( d_timersMask )
	{
		GS_CUDA_SAFE_CALL( cudaFree( d_timersMask ) );
		d_timersMask = 0;
	}

	if ( overlayTex )
	{
		glDeleteTextures( 1, &overlayTex );
		overlayTex = 0;
	}

	if ( cacheStateTex )
	{
		glDeleteTextures( 1, &cacheStateTex );
		cacheStateTex = 0;
	}

	d_timersArray = new GvCore::GsLinearMemory< GvCore::uint64 >( make_uint3( frameRes.x, frameRes.y, CUDAPERFMON_KERNEL_TIMER_MAX ) );

	// TEST --------------------------------- deplacer ça dans le renderer
	//GvCore::GsLinearMemoryKernel< GvCore::uint64 > h_timersArray = d_timersArray->getDeviceArray();
	//GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_timersArray, &h_timersArray, sizeof( h_timersArray ), 0, cudaMemcpyHostToDevice ) );

	GS_CUDA_SAFE_CALL( cudaMalloc( &d_timersMask, frameRes.x * frameRes.y ) );

	// TEST --------------------------------- deplacer ça dans le renderer
	//GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_timersMask, &d_timersMask, sizeof( d_timersMask ), 0, cudaMemcpyHostToDevice ) );

	// TEST
	_requestResize = true;

	// TEST -----------------------------------------------------------------------------------------------
	//	std::cout << "PERFORMANCE COUNTERS" << std::endl;
	//	std::cout << "k_timersArray = " << &k_timersArray << std::endl;
	//	std::cout << "k_timersMask = " << &k_timersMask << std::endl;
	//-----------------------------------------------------------------------------------------------------

	glGenTextures( 1, &overlayTex );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, overlayTex );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );

	glGenTextures( 1, &cacheStateTex );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, cacheStateTex );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
inline GvCore::GsLinearMemory< GvCore::uint64 >* CUDAPerfMon::getKernelTimerArray()
{
	return d_timersArray;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
inline uchar* CUDAPerfMon::getKernelTimerMask()
{
	return d_timersMask;
}

/******************************************************************************
 * ...
 *
 * @return ...
 ******************************************************************************/
inline GvCore::GsLinearMemory< uchar4 >* CUDAPerfMon::getCacheStateArray() const
{
	return d_cacheStateArray;
}

/******************************************************************************
 * ...
 *
 * @param cacheStateArray ...
 ******************************************************************************/
inline void CUDAPerfMon::setCacheStateArray( GvCore::GsLinearMemory< uchar4 >* cacheStateArray )
{
	d_cacheStateArray = cacheStateArray;
}

} // namespace GvPerfMon
