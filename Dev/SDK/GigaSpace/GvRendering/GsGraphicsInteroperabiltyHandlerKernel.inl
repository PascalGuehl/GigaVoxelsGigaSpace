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

///******************************************************************************
// ******************************* INCLUDE SECTION ******************************
// ******************************************************************************/
//
///******************************************************************************
// ****************************** INLINE DEFINITION *****************************
// ******************************************************************************/
//
//namespace GvRendering
//{
//
///******************************************************************************
// * Get the color at given pixel from input color buffer
// *
// * @param pPixel pixel coordinates
// *
// * @return the pixel color
// ******************************************************************************/
//__device__
//__forceinline__ uchar4 //GsGraphicsInteroperabiltyHandlerKernel
////::getInputColor( const uint2 pPixel )
//getInputColor( const uint2 pPixel )
//{
//	switch ( k_renderViewContext._graphicsResourceAccess[ GsGraphicsInteroperabiltyHandler::eColorInput ] )
//	{
//		case GsGraphicsResource::ePointer:
//			{
//				int offset = pPixel.x + pPixel.y * k_renderViewContext.frameSize.x;
//				return static_cast< uchar4* >( k_renderViewContext._graphicsResources[ GsGraphicsInteroperabiltyHandler::eColorInput ] )[ offset ];
//			}
//			break;
//
//		case GsGraphicsResource::eTexture:
//			return tex2D( GvRendering::_inputColorTexture, k_renderViewContext._inputColorTextureOffset + pPixel.x, pPixel.y );
//			break;
//
//		case GsGraphicsResource::eSurface:
//			// Note : cudaBoundaryModeTrap means that out-of-range accesses cause the kernel execution to fail.
//			// Possible values can be : cudaBoundaryModeTrap, cudaBoundaryModeClamp or cudaBoundaryModeZero.
//			return surf2Dread< uchar4 >( GvRendering::_colorSurface, pPixel.x * sizeof( uchar4 ), pPixel.y, cudaBoundaryModeTrap );
//			break;
//
//		default:
//			break;
//	}
//
//	return k_renderViewContext._clearColor;
//}
//
///******************************************************************************
// * Set the color at given pixel into output color buffer
// *
// * @param pPixel pixel coordinates
// * @param pColor color
// ******************************************************************************/
//__device__
//__forceinline__ void //GsGraphicsInteroperabiltyHandlerKernel
////::setOutputColor( const uint2 pPixel, uchar4 pColor )
//setOutputColor( const uint2 pPixel, uchar4 pColor )
//{
//	switch ( k_renderViewContext._graphicsResourceAccess[ GsGraphicsInteroperabiltyHandler::eColorOutput ] )
//	{
//		case GsGraphicsResource::ePointer:
//			{
//				int offset = pPixel.x + pPixel.y * k_renderViewContext.frameSize.x;
//				static_cast< uchar4* >( k_renderViewContext._graphicsResources[ GsGraphicsInteroperabiltyHandler::eColorOutput ] )[ offset ] = pColor;
//			}
//			break;
//
//		case GsGraphicsResource::eSurface:
//			// Note : cudaBoundaryModeTrap means that out-of-range accesses cause the kernel execution to fail.
//			// Possible values can be : cudaBoundaryModeTrap, cudaBoundaryModeClamp or cudaBoundaryModeZero.
//			surf2Dwrite( pColor, GvRendering::_colorSurface, pPixel.x * sizeof( uchar4 ), pPixel.y, cudaBoundaryModeTrap );
//			break;
//
//		default:
//			break;
//	}
//}
//
///******************************************************************************
// * Get the depth at given pixel from input depth buffer
// *
// * @param pPixel pixel coordinates
// *
// * @return the pixel depth
// ******************************************************************************/
//__device__
//__forceinline__ float //GsGraphicsInteroperabiltyHandlerKernel
////::getInputDepth( const uint2 pPixel )
//getInputDepth( const uint2 pPixel )
//{
//	float tmpfval = 1.0f;
//
//	// Read depth from Z-buffer
//	switch ( k_renderViewContext._graphicsResourceAccess[ GsGraphicsInteroperabiltyHandler::eDepthInput ] )
//	{
//		case GsGraphicsResource::ePointer:
//			{
//				int offset = pPixel.x + pPixel.y * k_renderViewContext.frameSize.x;
//				tmpfval = static_cast< float* >( k_renderViewContext._graphicsResources[ GsGraphicsInteroperabiltyHandler::eDepthInput ] )[ offset ];
//			}
//			break;
//
//		case GsGraphicsResource::eTexture:
//			tmpfval = tex2D( GvRendering::_inputDepthTexture, k_renderViewContext._inputDepthTextureOffset + pPixel.x, pPixel.y );
//			break;
//
//			//case GsGraphicsInteroperabiltyHandler::eSurface:
//			case GsGraphicsResource::eSurface:
//				// Note : cudaBoundaryModeTrap means that out-of-range accesses cause the kernel execution to fail.
//				// Possible values can be : cudaBoundaryModeTrap, cudaBoundaryModeClamp or cudaBoundaryModeZero.
//				surf2Dread< float >( &tmpfval, GvRendering::_depthSurface, pPixel.x * sizeof( float ), pPixel.y, cudaBoundaryModeTrap );
//				break;
//
//			default:
//				tmpfval = k_renderViewContext._clearDepth;
//				break;
//	}
//					
//	// Decode depth from Z-buffer
//	uint tmpival = __float_as_int( tmpfval );
//	tmpival = ( tmpival & 0xFFFFFF00 ) >> 8;
//
//	return __saturatef( static_cast< float >( tmpival ) / 16777215.0f );
//}
//
///******************************************************************************
// * Set the depth at given pixel into output depth buffer
// *
// * @param pPixel pixel coordinates
// * @param pDepth depth
// ******************************************************************************/
//__device__
//__forceinline__ void //GsGraphicsInteroperabiltyHandlerKernel
////::setOutputDepth( const uint2 pPixel, float pDepth )
//setOutputDepth( const uint2 pPixel, float pDepth )
//{
//	// Encode depth to Z-buffer
//	uint tmpival = static_cast< uint >( floorf( pDepth * 16777215.0f ) );
//	tmpival = tmpival << 8;
//	float Zdepth = __int_as_float( tmpival );
//
//	// Write depth to Z-buffer
//	switch ( k_renderViewContext._graphicsResourceAccess[ GsGraphicsInteroperabiltyHandler::eDepthOutput ] )
//	{
//		case GsGraphicsResource::ePointer:
//			{
//				int offset = pPixel.x + pPixel.y * k_renderViewContext.frameSize.x;
//				static_cast< float* >( k_renderViewContext._graphicsResources[ GsGraphicsInteroperabiltyHandler::eDepthOutput ] )[ offset ] = Zdepth;
//			}
//			break;
//
//		case GsGraphicsResource::eSurface:
//			// Note : cudaBoundaryModeTrap means that out-of-range accesses cause the kernel execution to fail.
//			// Possible values can be : cudaBoundaryModeTrap, cudaBoundaryModeClamp or cudaBoundaryModeZero.
//			surf2Dwrite( pDepth, GvRendering::_depthSurface, pPixel.x * sizeof( float ), pPixel.y, cudaBoundaryModeTrap );
//			break;
//
//		default:
//			break;
//	}
//}
//
//} // namespace GvRendering
