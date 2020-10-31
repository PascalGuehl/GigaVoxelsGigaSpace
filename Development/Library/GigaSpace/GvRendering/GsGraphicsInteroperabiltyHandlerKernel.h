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

//#ifndef _GV_GRAPHICS_INTEROPERABILTY_HANDLER_KERNEL_
//#define _GV_GRAPHICS_INTEROPERABILTY_HANDLER_KERNEL_
//
///******************************************************************************
// ******************************* INCLUDE SECTION ******************************
// ******************************************************************************/
//
//// GigaVoxels
//#include "GvCore/GsCoreConfig.h"
////#include "GvRendering/GsRendererHelpersKernel.h"
//
//// Cuda
//#include <host_defines.h>
//#include <vector_types.h>
//#include <texture_types.h>
//#include <surface_types.h>
//#include <device_functions.h>
//#include <cuda_texture_types.h>
//#include <cuda_surface_types.h>
//#include <texture_fetch_functions.h>
//#include <surface_functions.h>
//
///******************************************************************************
// ************************* DEFINE AND CONSTANT SECTION ************************
// ******************************************************************************/
//
///******************************************************************************
// ***************************** TYPE DEFINITION ********************************
// ******************************************************************************/
//
//namespace GvRendering
//{
//
///**
// * Texture references used to read input color/depth buffers from graphics library (i.e. OpenGL)
// */
//texture< uchar4, cudaTextureType2D, cudaReadModeElementType > _inputColorTexture;
//texture< float, cudaTextureType2D, cudaReadModeElementType > _inputDepthTexture;
//
///**
// * Surface references used to read input/output color/depth buffers from graphics library (i.e. OpenGL)
// */
//surface< void, cudaSurfaceType2D > _colorSurface;
//surface< void, cudaSurfaceType2D > _depthSurface;
//
//}
//
///******************************************************************************
// ******************************** CLASS USED **********************************
// ******************************************************************************/
//
///******************************************************************************
// ****************************** CLASS DEFINITION ******************************
// ******************************************************************************/
//
//namespace GvRendering
//{
//
///** 
// * @class GsGraphicsInteroperabiltyHandlerKernel
// *
// * @brief The GsGraphicsInteroperabiltyHandlerKernel class provides methods
// * to read/ write color and depth buffers.
// *
// * ...
// */
////class GsGraphicsInteroperabiltyHandlerKernel
////{
//
//	/**************************************************************************
//	 ***************************** PUBLIC SECTION *****************************
//	 **************************************************************************/
//
////public:
//
//	/****************************** INNER TYPES *******************************/
//
//	/******************************* ATTRIBUTES *******************************/
//
//	/******************************** METHODS *********************************/
//
//	/**
//	 * Get the color at given pixel from input color buffer
//	 *
//	 * @param pPixel pixel coordinates
//	 *
//	 * @return the pixel color
//	 */
//	__device__
//	/*static*/ __forceinline__ uchar4 getInputColor( const uint2 pPixel );
//
//	/**
//	 * Set the color at given pixel into output color buffer
//	 *
//	 * @param pPixel pixel coordinates
//	 * @param pColor color
//	 */
//	__device__
//	/*static*/ __forceinline__ void setOutputColor( const uint2 pPixel, uchar4 pColor );
//
//	/**
//	 * Get the depth at given pixel from input depth buffer
//	 *
//	 * @param pPixel pixel coordinates
//	 *
//	 * @return the pixel depth
//	 */
//	__device__
//	/*static*/ __forceinline__ float getInputDepth( const uint2 pPixel );
//
//	/**
//	 * Set the depth at given pixel into output depth buffer
//	 *
//	 * @param pPixel pixel coordinates
//	 * @param pDepth depth
//	 */
//	__device__
//	/*static*/ __forceinline__ void setOutputDepth( const uint2 pPixel, float pDepth );
//
//	/**************************************************************************
//	 **************************** PROTECTED SECTION ***************************
//	 **************************************************************************/
//
////protected:
//
//	/****************************** INNER TYPES *******************************/
//
//	/******************************* ATTRIBUTES *******************************/
//
//	/******************************** METHODS *********************************/
//
//	/**************************************************************************
//	 ***************************** PRIVATE SECTION ****************************
//	 **************************************************************************/
//
////private:
//
//	/****************************** INNER TYPES *******************************/
//
//	/******************************* ATTRIBUTES *******************************/
//
//	/******************************** METHODS *********************************/
//
////};
//
//}
//
///**************************************************************************
// ***************************** INLINE SECTION *****************************
// **************************************************************************/
//
//#include "GsGraphicsInteroperabiltyHandlerKernel.inl"
//
//#endif // !_GV_GRAPHICS_INTEROPERABILTY_HANDLER_KERNEL_
