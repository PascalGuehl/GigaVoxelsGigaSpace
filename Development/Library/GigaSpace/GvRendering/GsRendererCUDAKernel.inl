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

// GigaVoxels
#include "GvStructure/GsNode.h"

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvRendering
{

/******************************************************************************
 * CUDA kernel
 * This is the main GigaVoxels KERNEL
 * It is in charge of casting rays and found the color and depth values at pixels.
 *
 * @param pVolumeTree data structure
 * @param pCache cache
 ******************************************************************************/
template< class TBlockResolution, bool TFastUpdateMode, bool TPriorityOnBrick, 
	class TSampleShaderType, class TVolTreeKernelType, 
	class TCacheType >
__global__
// TO DO : Prolifing / Optimization
// - use "Launch Bounds" feature to profile / optimize code
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor ) Ex : (128, 10)
void RenderKernelSimple( TVolTreeKernelType pVolumeTree, TCacheType pCache )
{
	CUDAPM_KERNEL_DEFINE_EVENT( 0 )
	CUDAPM_KERNEL_DEFINE_EVENT( 1 )

	// Per-pixel shader instance
	typename TSampleShaderType::KernelType sampleShader;

	// Shared memory
	__shared__ float3 smRayStart;
	// TO DO : Prolifing / Optimization
	// - try to remove smRayStart from __shared__ memory, because genertaed assembler code on GPU shows that it uses more registers
	
	// Compute thread ID
	const uint pixelID = threadIdx.x + threadIdx.y * TBlockResolution::x;

	// Retrieve current processed pixel position
	// This function modifies the pixel accessing pattern (i.e. z-curve)
	uint2 pixelCoords;
	/*uint2 blockPos;*/ // NOTE : this "block position" parameter seemed not used anymore
	GsRendererKernel::initPixelCoords< TBlockResolution >( pixelID, /*blockPos,*/ pixelCoords );

	CUDAPM_KERNEL_START_EVENT( pixelCoords, 0 );

	// Check if were are inside the frame (window or viewport ?)
	const bool outOfFrame = ( pixelCoords.x >= k_renderViewContext.frameSize.x ) || ( pixelCoords.y >= k_renderViewContext.frameSize.y );
	// FUTUR optimization
	//
	//const bool outOfFrame = ( ( pixelCoords.x >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool outOfFrame = ( ( pixelCoords.x > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool inFrame = ( ( pixelCoords.x < k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y < k_renderViewContext._projectedBBox.w ) );
	//if ( inFrame )
	if ( ! outOfFrame )
	{
		// Read depth from the input depth buffer.
		// Depth buffer contains the Zwindow (distance to camera plane) which is different from Zeye (distance to camera)
		// Zwindow is between 0.0 and 1.0
		// The depth buffer doesn't contain distance values from the camera.
		// The depth values are the perpendicular distance to the plane of the camera.
		float frameDepth = getInputDepth( pixelCoords );

		// FUTUR optimization
		//
		//// Add offset of the projected BBox bottom left corner
		//pixelCoords.x += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.x;
		//pixelCoords.y += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.y;

		// Note on untransformed and transformed objects with the original and inverse transformed rays.
		//
		// Given a point P on an object, applying a model transformation T on it involves modifications.
		//
		// Given a starting point "O" on a ray of direction "D" (unit vector), a point at distance "t" on this ray
		// is represented by in World space by :
		//    P' = O + t D
		// And the corresponding point in Object space is :
		//    P = O' + t D'
		// where O' = T-1 O and D' = T-1 D

		// Calculate eye ray in tree space
		//
		// Apply the inverse set of transformations to the ray to produce an "inverse transformed ray"
		float3 rayDir = k_renderViewContext.viewPlaneDirTP
							+ k_renderViewContext.viewPlaneXAxisTP * static_cast< float >( pixelCoords.x )
							+ k_renderViewContext.viewPlaneYAxisTP * static_cast< float >( pixelCoords.y );
		
		// Ray start
		smRayStart = k_renderViewContext.viewCenterTP;
		
		// Not sure to normalize
		// - traditionaly, in ray-tracing, for the object space, we don't have to normalize the ray
		rayDir = normalize( rayDir );
					
		// Intersect the inverse transformed ray with the untransformed object
		// - a BBox in [ 0.0; 1.0 ] x [ 0.0; 1.0 ] x [ 0.0; 1.0 ]
		float boxInterMin = 0.0f;
		float boxInterMax = 10000.0f;
		int hit = intersectBox( smRayStart, rayDir, make_float3( 0.001f ), make_float3( 0.999f ), boxInterMin, boxInterMax );
		bool masked = ! ( hit && ( boxInterMax > 0.0f ) );
		
		// Set closest hit point
		boxInterMin = maxcc( boxInterMin, k_renderViewContext.frustumNear );	// TO DO : attention, c'est faux => frustumNear est en "espace camera" !!
		float t = boxInterMin + sampleShader.getConeAperture( boxInterMin );
		
		// Set farthest hit point
		float tMax = boxInterMax;
		if ( frameDepth < 1.0f )
		{
			// Retrieve the view-space depth from the depth buffer. Only works if w was 1.0f.
			// See: http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=304624&page=2

			// Compute z in NDC space
			//
			// True equation is :
			//     float zNDC = ( 2.f * frameDepth - k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) / ( k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) - 1.f;
			// but a simplication occurs if glDepthRange values are ( 0.0; 1.0 )
			//     float zNDC = 2.0f * frameDepth - 1.0f;
			const float zNDC = 2.0f * frameDepth - 1.0f;

			// Compute z in Eye space
			const float zEye = k_renderViewContext.frustumD / ( -zNDC - k_renderViewContext.frustumC );
			
			// Take minimum value between input depth and bbox output
			tMax = mincc( -zEye, boxInterMax );
		}

		// Discard special cases
		if ( t == 0.0f || t >= tMax )
		{
			masked = true;
		}

		// If intersection
		if ( ! masked )
		{
			// Read color from the input color buffer
			uchar4 frameColor = getInputColor( pixelCoords );

			// Launch N3-tree traversal and rendering
			CUDAPM_KERNEL_START_EVENT( pixelCoords, 1 );
			GsRendererKernel::render< TFastUpdateMode, TPriorityOnBrick >( pVolumeTree, sampleShader, pCache, pixelCoords, smRayStart, rayDir, tMax, t );
			CUDAPM_KERNEL_STOP_EVENT( pixelCoords, 1 );

			// Retrieve the accumulated color along the ray
			const float4 accCol = sampleShader.getColor();

			// Convert color from uchar [ 0 ; 255 ] to float [ 0.0 ; 1.0 ]
			float4 scenePixelColorF = make_float4( (float)frameColor.x / 255.0f, (float)frameColor.y / 255.0f, (float)frameColor.z / 255.0f, (float)frameColor.w / 255.0f );
		
			// TO DO
			// - let OpenGL do that operation with blending ?

			// Blend colors (ray and framebuffer)
			float4 pixelColorF = accCol + scenePixelColorF * ( 1.0f - accCol.w );

			// Clamp color to be within the interval [+0.0, 1.0]
			pixelColorF.x = __saturatef( pixelColorF.x );
			pixelColorF.y = __saturatef( pixelColorF.y );
			pixelColorF.z = __saturatef( pixelColorF.z );
			//pixelColorF.w = 1.0f;		// <== why 1.0f and not __saturatef( pixelColorF.w ) ?	// Pour éviter une opération OpenGL de ROP ? Car ça a été penser pour rendre en dernier au départ ?
			pixelColorF.w = __saturatef( pixelColorF.w );
			//pixelColorF.w = __saturatef( pixelColorF.w );
			
			// Convert color from float [ 0.0 ; 1.0 ] to uchar [ 0 ; 255 ]
			frameColor = make_uchar4( (uchar)( pixelColorF.x * 255.0f ), (uchar)( pixelColorF.y * 255.0f ), (uchar)( pixelColorF.z * 255.0f ), (uchar)( pixelColorF.w * 255.0f ) );
			
			// Project the depth and check against the current one
			float pixDepth = 1.0f;
			if ( accCol.w > cOpacityDepthThreshold )
			{
				const float VP = -fabsf( t * rayDir.z );
				//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=234519&page=2
				const float clipZ = ( VP * k_renderViewContext.frustumC + k_renderViewContext.frustumD ) / -VP;
				
				//pixDepth = clamp( ( clipZ + 1.0f ) / 2.0f, 0.0f, 1.0f ); // TO DO : use __saturatef instead !!	=====> ( [ x 0.5f ] instead ) ??
				pixDepth = __saturatef( clipZ * 0.5f + 0.5f );
				
				//const float zNDC = - ( k_renderViewContext.frustumC + k_renderViewContext.frustumD / ( -t ) );
				//const float zWindow = zNDC * 0.5f + * 0.5f;
				//pixDepth = zWindow;
			}
			frameDepth = min( frameDepth, pixDepth );

			// Write color in color buffer
			setOutputColor( pixelCoords, frameColor );
			
			// Write depth in depth buffer
			setOutputDepth( pixelCoords, frameDepth );
		}
	} // !outOfFrame

	CUDAPM_KERNEL_STOP_EVENT( pixelCoords, 0 );
}

/******************************************************************************
 * CUDA kernel
 * This is the main GigaVoxels KERNEL
 * It is in charge of casting rays and found the color and depth values at pixels.
 *
 * @param pVolumeTree data structure
 * @param pCache cache
 ******************************************************************************/
template< class TBlockResolution, bool TFastUpdateMode, bool TPriorityOnBrick, 
	class TSampleShaderType, class TVolTreeKernelType, 
	class TCacheType >
__global__
// TO DO : Prolifing / Optimization
// - use "Launch Bounds" feature to profile / optimize code
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor ) Ex : (128, 10)
void GsKernel_QualityRenderer_Initialize( TVolTreeKernelType pVolumeTree, TCacheType pCache,
								GvCore::GsLinearMemoryKernel< float > pRayDepthBuffer,
								GvCore::GsLinearMemoryKernel< float > pRayMaxDepthBuffer,
								GvCore::GsLinearMemoryKernel< float4 > pRayAccumulatedColorBuffer )
{
	// Per-pixel shader instance
	typename TSampleShaderType::KernelType sampleShader;

	// Shared memory
	__shared__ float3 smRayStart;
	// TO DO : Prolifing / Optimization
	// - try to remove smRayStart from __shared__ memory, because genertaed assembler code on GPU shows that it uses more registers
	
	// Compute thread ID
	const uint pixelID = threadIdx.x + threadIdx.y * TBlockResolution::x;

	// Retrieve current processed pixel position
	// This function modifies the pixel accessing pattern (i.e. z-curve)
	uint2 pixelCoords;
	/*uint2 blockPos;*/ // NOTE : this "block position" parameter seemed not used anymore
	GsRendererKernel::initPixelCoords< TBlockResolution >( pixelID, /*blockPos,*/ pixelCoords );

	// Check if were are inside the frame (window or viewport ?)
	const bool outOfFrame = ( pixelCoords.x >= k_renderViewContext.frameSize.x ) || ( pixelCoords.y >= k_renderViewContext.frameSize.y );
	// FUTUR optimization
	//
	//const bool outOfFrame = ( ( pixelCoords.x >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool outOfFrame = ( ( pixelCoords.x > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool inFrame = ( ( pixelCoords.x < k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y < k_renderViewContext._projectedBBox.w ) );
	//if ( inFrame )
	if ( ! outOfFrame )
	{
		// Read depth from the input depth buffer.
		// Depth buffer contains the Zwindow (distance to camera plane) which is different from Zeye (distance to camera)
		// Zwindow is between 0.0 and 1.0
		// The depth buffer doesn't contain distance values from the camera.
		// The depth values are the perpendicular distance to the plane of the camera.
		//float frameDepth = getInputDepth( pixelCoords );
		// ---------------------------------------
		// TODO : here, read from previous pass
		// ---------------------------------------

		// FUTUR optimization
		//
		//// Add offset of the projected BBox bottom left corner
		//pixelCoords.x += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.x;
		//pixelCoords.y += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.y;

		// Note on untransformed and transformed objects with the original and inverse transformed rays.
		//
		// Given a point P on an object, applying a model transformation T on it involves modifications.
		//
		// Given a starting point "O" on a ray of direction "D" (unit vector), a point at distance "t" on this ray
		// is represented by in World space by :
		//    P' = O + t D
		// And the corresponding point in Object space is :
		//    P = O' + t D'
		// where O' = T-1 O and D' = T-1 D

		// Calculate eye ray in tree space
		//
		// Apply the inverse set of transformations to the ray to produce an "inverse transformed ray"
		float3 rayDir = k_renderViewContext.viewPlaneDirTP
							+ k_renderViewContext.viewPlaneXAxisTP * static_cast< float >( pixelCoords.x )
							+ k_renderViewContext.viewPlaneYAxisTP * static_cast< float >( pixelCoords.y );
		
		// Ray start
		smRayStart = k_renderViewContext.viewCenterTP;
		
		// Not sure to normalize
		// - traditionaly, in ray-tracing, for the object space, we don't have to normalize the ray
		rayDir = normalize( rayDir );
					
		// Intersect the inverse transformed ray with the untransformed object
		// - a BBox in [ 0.0; 1.0 ] x [ 0.0; 1.0 ] x [ 0.0; 1.0 ]
		float boxInterMin = 0.0f;
		float boxInterMax = 10000.0f;
		int hit = intersectBox( smRayStart, rayDir, make_float3( 0.001f ), make_float3( 0.999f ), boxInterMin, boxInterMax );
		// ---------------------------------------
		// TODO : here, read from previous pass
		// ---------------------------------------
		bool masked = ! ( hit && ( boxInterMax > 0.0f ) );
		
		// Set closest hit point
		boxInterMin = maxcc( boxInterMin, k_renderViewContext.frustumNear );	// TO DO : attention, c'est faux => frustumNear est en "espace camera" !!
		float t = boxInterMin + sampleShader.getConeAperture( boxInterMin );
		
		// Set farthest hit point
		float tMax = boxInterMax;
		//if ( frameDepth < 1.0f )
		//{
		//	// Retrieve the view-space depth from the depth buffer. Only works if w was 1.0f.
		//	// See: http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=304624&page=2

		//	// Compute z in NDC space
		//	//
		//	// True equation is :
		//	//     float zNDC = ( 2.f * frameDepth - k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) / ( k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) - 1.f;
		//	// but a simplication occurs if glDepthRange values are ( 0.0; 1.0 )
		//	//     float zNDC = 2.0f * frameDepth - 1.0f;
		//	const float zNDC = 2.0f * frameDepth - 1.0f;

		//	// Compute z in Eye space
		//	const float zEye = k_renderViewContext.frustumD / ( -zNDC - k_renderViewContext.frustumC );
		//	
		//	// Take minimum value between input depth and bbox output
		//	tMax = mincc( -zEye, boxInterMax );
		//}

		// Discard special cases
		if ( t == 0.0f || t >= tMax )
		{
			masked = true;
		}

		// If intersection
		if ( ! masked )
		{
			// Read color from the input color buffer
			//uchar4 frameColor = getInputColor( pixelCoords );

			// Launch N3-tree traversal and rendering
			//GsRendererKernel::render< TFastUpdateMode, TPriorityOnBrick >( pVolumeTree, sampleShader, pCache, pixelCoords, smRayStart, rayDir, tMax, t );
		
			//// Retrieve the accumulated color along the ray
			//const float4 accCol = sampleShader.getColor();

			//// Convert color from uchar [ 0 ; 255 ] to float [ 0.0 ; 1.0 ]
			//float4 scenePixelColorF = make_float4( (float)frameColor.x / 255.0f, (float)frameColor.y / 255.0f, (float)frameColor.z / 255.0f, (float)frameColor.w / 255.0f );
		
			//// TO DO
			//// - let OpenGL do that operation with blending ?

			//// Blend colors (ray and framebuffer)
			//float4 pixelColorF = accCol + scenePixelColorF * ( 1.0f - accCol.w );

			//// Clamp color to be within the interval [+0.0, 1.0]
			//pixelColorF.x = __saturatef( pixelColorF.x );
			//pixelColorF.y = __saturatef( pixelColorF.y );
			//pixelColorF.z = __saturatef( pixelColorF.z );
			////pixelColorF.w = 1.0f;		// <== why 1.0f and not __saturatef( pixelColorF.w ) ?	// Pour éviter une opération OpenGL de ROP ? Car ça a été penser pour rendre en dernier au départ ?
			//pixelColorF.w = __saturatef( pixelColorF.w );
			////pixelColorF.w = __saturatef( pixelColorF.w );
			//
			//// Convert color from float [ 0.0 ; 1.0 ] to uchar [ 0 ; 255 ]
			//frameColor = make_uchar4( (uchar)( pixelColorF.x * 255.0f ), (uchar)( pixelColorF.y * 255.0f ), (uchar)( pixelColorF.z * 255.0f ), (uchar)( pixelColorF.w * 255.0f ) );
			
			//// Project the depth and check against the current one
			//float pixDepth = 1.0f;
			//if ( accCol.w > cOpacityDepthThreshold )
			//{
			//	const float VP = -fabsf( t * rayDir.z );
			//	//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=234519&page=2
			//	const float clipZ = ( VP * k_renderViewContext.frustumC + k_renderViewContext.frustumD ) / -VP;
			//	
			//	//pixDepth = clamp( ( clipZ + 1.0f ) / 2.0f, 0.0f, 1.0f ); // TO DO : use __saturatef instead !!	=====> ( [ x 0.5f ] instead ) ??
			//	pixDepth = __saturatef( clipZ * 0.5f + 0.5f );
			//	
			//	//const float zNDC = - ( k_renderViewContext.frustumC + k_renderViewContext.frustumD / ( -t ) );
			//	//const float zWindow = zNDC * 0.5f + * 0.5f;
			//	//pixDepth = zWindow;
			//}
			//frameDepth = min( frameDepth, pixDepth );

			// Write color in color buffer
			//setOutputColor( pixelCoords, frameColor );
			
			// Write depth in depth buffer
			//setOutputDepth( pixelCoords, frameDepth );

			// ---------------------------------------
			// Update data
			pRayDepthBuffer.set( pixelCoords, t );
			pRayMaxDepthBuffer.set( pixelCoords, tMax );
			pRayAccumulatedColorBuffer.set( pixelCoords, make_float4( 0.f ) );
			// ---------------------------------------
		}
	} // !outOfFrame
}

/******************************************************************************
 * CUDA kernel
 * This is the main GigaVoxels KERNEL
 * It is in charge of casting rays and found the color and depth values at pixels.
 *
 * @param pVolumeTree data structure
 * @param pCache cache
 ******************************************************************************/
template< class TBlockResolution, bool TFastUpdateMode, bool TPriorityOnBrick, 
	class TSampleShaderType, class TVolTreeKernelType, 
	class TCacheType >
__global__
// TO DO : Prolifing / Optimization
// - use "Launch Bounds" feature to profile / optimize code
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor ) Ex : (128, 10)
void GsKernel_QualityRenderer_Continue( TVolTreeKernelType pVolumeTree, TCacheType pCache,
								GvCore::GsLinearMemoryKernel< float > pRayDepthBuffer,
								GvCore::GsLinearMemoryKernel< float > pRayMaxDepthBuffer,
								GvCore::GsLinearMemoryKernel< float4 > pRayAccumulatedColorBuffer )
{
	// Per-pixel shader instance
	typename TSampleShaderType::KernelType sampleShader;

	// Shared memory
	__shared__ float3 smRayStart;
	// TO DO : Prolifing / Optimization
	// - try to remove smRayStart from __shared__ memory, because genertaed assembler code on GPU shows that it uses more registers
	
	// Compute thread ID
	const uint pixelID = threadIdx.x + threadIdx.y * TBlockResolution::x;

	// Retrieve current processed pixel position
	// This function modifies the pixel accessing pattern (i.e. z-curve)
	uint2 pixelCoords;
	/*uint2 blockPos;*/ // NOTE : this "block position" parameter seemed not used anymore
	GsRendererKernel::initPixelCoords< TBlockResolution >( pixelID, /*blockPos,*/ pixelCoords );

	// Check if were are inside the frame (window or viewport ?)
	const bool outOfFrame = ( pixelCoords.x >= k_renderViewContext.frameSize.x ) || ( pixelCoords.y >= k_renderViewContext.frameSize.y );
	// FUTUR optimization
	//
	//const bool outOfFrame = ( ( pixelCoords.x >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool outOfFrame = ( ( pixelCoords.x > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool inFrame = ( ( pixelCoords.x < k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y < k_renderViewContext._projectedBBox.w ) );
	//if ( inFrame )
	if ( ! outOfFrame )
	{
		// Read depth from the input depth buffer.
		// Depth buffer contains the Zwindow (distance to camera plane) which is different from Zeye (distance to camera)
		// Zwindow is between 0.0 and 1.0
		// The depth buffer doesn't contain distance values from the camera.
		// The depth values are the perpendicular distance to the plane of the camera.
		//float frameDepth = getInputDepth( pixelCoords );
		// ---------------------------------------
		// TODO : here, read from previous pass
		// ---------------------------------------

		// FUTUR optimization
		//
		//// Add offset of the projected BBox bottom left corner
		//pixelCoords.x += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.x;
		//pixelCoords.y += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.y;

		// Note on untransformed and transformed objects with the original and inverse transformed rays.
		//
		// Given a point P on an object, applying a model transformation T on it involves modifications.
		//
		// Given a starting point "O" on a ray of direction "D" (unit vector), a point at distance "t" on this ray
		// is represented by in World space by :
		//    P' = O + t D
		// And the corresponding point in Object space is :
		//    P = O' + t D'
		// where O' = T-1 O and D' = T-1 D

		// Calculate eye ray in tree space
		//
		// Apply the inverse set of transformations to the ray to produce an "inverse transformed ray"
		float3 rayDir = k_renderViewContext.viewPlaneDirTP
							+ k_renderViewContext.viewPlaneXAxisTP * static_cast< float >( pixelCoords.x )
							+ k_renderViewContext.viewPlaneYAxisTP * static_cast< float >( pixelCoords.y );
		
		// Ray start
		smRayStart = k_renderViewContext.viewCenterTP;
		
		// Not sure to normalize
		// - traditionaly, in ray-tracing, for the object space, we don't have to normalize the ray
		rayDir = normalize( rayDir );
					
		// Intersect the inverse transformed ray with the untransformed object
		// - a BBox in [ 0.0; 1.0 ] x [ 0.0; 1.0 ] x [ 0.0; 1.0 ]
		//float boxInterMin = 0.0f;
		//float boxInterMax = 10000.0f;
		//int hit = intersectBox( smRayStart, rayDir, make_float3( 0.001f ), make_float3( 0.999f ), boxInterMin, boxInterMax );
		// ---------------------------------------
		// TODO : here, read from previous pass
		// ---------------------------------------
		//bool masked = ! ( hit && ( boxInterMax > 0.0f ) );
		
		// ---------------------------------------
		// Retrieve data from previous pass
		float currentDepth = pRayDepthBuffer.get( pixelCoords );
		float depthMax = pRayMaxDepthBuffer.get( pixelCoords );
		//float4 accumulatedColor = pRayAccumulatedColorBuffer.get( pixelCoords );
		// ---------------------------------------

		// ---------------------------------------
		// Check data
		if ( currentDepth < 0.f )
		{
			return;
		}
		if ( depthMax < 0.f )
		{
			return;
		}
		// ---------------------------------------

		// ---------------------------------------
		// Normal stuff
		// ...
		
		float boxInterMin = currentDepth;
		float boxInterMax = depthMax;
		int hit = ( boxInterMin > 0.f ) && ( boxInterMax > 0.f );	// use >= instead ?
		bool masked = ! hit;
		
		// Set closest hit point
		boxInterMin = maxcc( boxInterMin, k_renderViewContext.frustumNear );	// TO DO : attention, c'est faux => frustumNear est en "espace camera" !!
		float t = boxInterMin + sampleShader.getConeAperture( boxInterMin );
		
		// Set farthest hit point
		float tMax = boxInterMax;
		//if ( frameDepth < 1.0f )
		//{
		//	// Retrieve the view-space depth from the depth buffer. Only works if w was 1.0f.
		//	// See: http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=304624&page=2

		//	// Compute z in NDC space
		//	//
		//	// True equation is :
		//	//     float zNDC = ( 2.f * frameDepth - k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) / ( k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) - 1.f;
		//	// but a simplication occurs if glDepthRange values are ( 0.0; 1.0 )
		//	//     float zNDC = 2.0f * frameDepth - 1.0f;
		//	const float zNDC = 2.0f * frameDepth - 1.0f;

		//	// Compute z in Eye space
		//	const float zEye = k_renderViewContext.frustumD / ( -zNDC - k_renderViewContext.frustumC );
		//	
		//	// Take minimum value between input depth and bbox output
		//	tMax = mincc( -zEye, boxInterMax );
		//}

		// Discard special cases
		if ( t == 0.0f || t >= tMax )
		{
			masked = true;
		}

		// If intersection
		if ( ! masked )
		{
			// Read color from the input color buffer
			//uchar4 frameColor = getInputColor( pixelCoords );

			// ---------------------------------------
			// Retrieve data from previous pass
			float4 accumulatedColor = pRayAccumulatedColorBuffer.get( pixelCoords );
			// ---------------------------------------

			// Launch N3-tree traversal and rendering
			//GsRendererKernel::render< TFastUpdateMode, TPriorityOnBrick >( pVolumeTree, sampleShader, pCache, pixelCoords, smRayStart, rayDir, tMax, t );
			// ---------------------------------------
			GsRendererKernel::render_quality< TFastUpdateMode, TPriorityOnBrick >( pVolumeTree, sampleShader, pCache, pixelCoords, smRayStart, rayDir, tMax, t, accumulatedColor );
			// ---------------------------------------
			
			// Retrieve the accumulated color along the ray
			const float4 accCol = sampleShader.getColor();

			//// Convert color from uchar [ 0 ; 255 ] to float [ 0.0 ; 1.0 ]
			//float4 scenePixelColorF = make_float4( (float)frameColor.x / 255.0f, (float)frameColor.y / 255.0f, (float)frameColor.z / 255.0f, (float)frameColor.w / 255.0f );
		
			//// TO DO
			//// - let OpenGL do that operation with blending ?

			//// Blend colors (ray and framebuffer)
			//float4 pixelColorF = accCol + scenePixelColorF * ( 1.0f - accCol.w );

			//// Clamp color to be within the interval [+0.0, 1.0]
			//pixelColorF.x = __saturatef( pixelColorF.x );
			//pixelColorF.y = __saturatef( pixelColorF.y );
			//pixelColorF.z = __saturatef( pixelColorF.z );
			////pixelColorF.w = 1.0f;		// <== why 1.0f and not __saturatef( pixelColorF.w ) ?	// Pour éviter une opération OpenGL de ROP ? Car ça a été penser pour rendre en dernier au départ ?
			//pixelColorF.w = __saturatef( pixelColorF.w );
			////pixelColorF.w = __saturatef( pixelColorF.w );
			//
			//// Convert color from float [ 0.0 ; 1.0 ] to uchar [ 0 ; 255 ]
			//frameColor = make_uchar4( (uchar)( pixelColorF.x * 255.0f ), (uchar)( pixelColorF.y * 255.0f ), (uchar)( pixelColorF.z * 255.0f ), (uchar)( pixelColorF.w * 255.0f ) );
			
			//// Project the depth and check against the current one
			//float pixDepth = 1.0f;
			//if ( accCol.w > cOpacityDepthThreshold )
			//{
			//	const float VP = -fabsf( t * rayDir.z );
			//	//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=234519&page=2
			//	const float clipZ = ( VP * k_renderViewContext.frustumC + k_renderViewContext.frustumD ) / -VP;
			//	
			//	//pixDepth = clamp( ( clipZ + 1.0f ) / 2.0f, 0.0f, 1.0f ); // TO DO : use __saturatef instead !!	=====> ( [ x 0.5f ] instead ) ??
			//	pixDepth = __saturatef( clipZ * 0.5f + 0.5f );
			//	
			//	//const float zNDC = - ( k_renderViewContext.frustumC + k_renderViewContext.frustumD / ( -t ) );
			//	//const float zWindow = zNDC * 0.5f + * 0.5f;
			//	//pixDepth = zWindow;
			//}
			//frameDepth = min( frameDepth, pixDepth );

			// Write color in color buffer
			//setOutputColor( pixelCoords, frameColor );
			
			// Write depth in depth buffer
			//setOutputDepth( pixelCoords, frameDepth );

			// ---------------------------------------
			// Update data
			pRayDepthBuffer.set( pixelCoords, t );
			pRayMaxDepthBuffer.set( pixelCoords, tMax );
			pRayAccumulatedColorBuffer.set( pixelCoords, /*accumulatedColor*/accCol );
			// ---------------------------------------
		}
	} // !outOfFrame
}

/******************************************************************************
 * CUDA kernel
 * This is the main GigaVoxels KERNEL
 * It is in charge of casting rays and found the color and depth values at pixels.
 *
 * @param pVolumeTree data structure
 * @param pCache cache
 ******************************************************************************/
template< class TBlockResolution, bool TFastUpdateMode, bool TPriorityOnBrick, 
	class TSampleShaderType, class TVolTreeKernelType, 
	class TCacheType >
__global__
// TO DO : Prolifing / Optimization
// - use "Launch Bounds" feature to profile / optimize code
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor ) Ex : (128, 10)
void GsKernel_QualityRenderer_Finalize( TVolTreeKernelType pVolumeTree, TCacheType pCache,
								GvCore::GsLinearMemoryKernel< float > pRayDepthBuffer,
								GvCore::GsLinearMemoryKernel< float > pRayMaxDepthBuffer,
								GvCore::GsLinearMemoryKernel< float4 > pRayAccumulatedColorBuffer )
{
	// Per-pixel shader instance
	typename TSampleShaderType::KernelType sampleShader;

	// Shared memory
	__shared__ float3 smRayStart;
	// TO DO : Prolifing / Optimization
	// - try to remove smRayStart from __shared__ memory, because genertaed assembler code on GPU shows that it uses more registers
	
	// Compute thread ID
	const uint pixelID = threadIdx.x + threadIdx.y * TBlockResolution::x;

	// Retrieve current processed pixel position
	// This function modifies the pixel accessing pattern (i.e. z-curve)
	uint2 pixelCoords;
	/*uint2 blockPos;*/ // NOTE : this "block position" parameter seemed not used anymore
	GsRendererKernel::initPixelCoords< TBlockResolution >( pixelID, /*blockPos,*/ pixelCoords );

	// Check if were are inside the frame (window or viewport ?)
	const bool outOfFrame = ( pixelCoords.x >= k_renderViewContext.frameSize.x ) || ( pixelCoords.y >= k_renderViewContext.frameSize.y );
	// FUTUR optimization
	//
	//const bool outOfFrame = ( ( pixelCoords.x >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y >= /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool outOfFrame = ( ( pixelCoords.x > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y > /*projectedBBoxSize*/k_renderViewContext._projectedBBox.w ) );
	//const bool inFrame = ( ( pixelCoords.x < k_renderViewContext._projectedBBox.z ) || ( pixelCoords.y < k_renderViewContext._projectedBBox.w ) );
	//if ( inFrame )
	if ( ! outOfFrame )
	{
		// Read depth from the input depth buffer.
		// Depth buffer contains the Zwindow (distance to camera plane) which is different from Zeye (distance to camera)
		// Zwindow is between 0.0 and 1.0
		// The depth buffer doesn't contain distance values from the camera.
		// The depth values are the perpendicular distance to the plane of the camera.
		float frameDepth = getInputDepth( pixelCoords );
		// ---------------------------------------
		// TODO : here, read from previous pass
		// ---------------------------------------

		// FUTUR optimization
		//
		//// Add offset of the projected BBox bottom left corner
		//pixelCoords.x += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.x;
		//pixelCoords.y += /*projectedBBoxBottomLeft*/k_renderViewContext._projectedBBox.y;

		// Note on untransformed and transformed objects with the original and inverse transformed rays.
		//
		// Given a point P on an object, applying a model transformation T on it involves modifications.
		//
		// Given a starting point "O" on a ray of direction "D" (unit vector), a point at distance "t" on this ray
		// is represented by in World space by :
		//    P' = O + t D
		// And the corresponding point in Object space is :
		//    P = O' + t D'
		// where O' = T-1 O and D' = T-1 D

		// Calculate eye ray in tree space
		//
		// Apply the inverse set of transformations to the ray to produce an "inverse transformed ray"
		float3 rayDir = k_renderViewContext.viewPlaneDirTP
							+ k_renderViewContext.viewPlaneXAxisTP * static_cast< float >( pixelCoords.x )
							+ k_renderViewContext.viewPlaneYAxisTP * static_cast< float >( pixelCoords.y );
		
		// Ray start
		smRayStart = k_renderViewContext.viewCenterTP;
		
		// Not sure to normalize
		// - traditionaly, in ray-tracing, for the object space, we don't have to normalize the ray
		rayDir = normalize( rayDir );
					
		//// Intersect the inverse transformed ray with the untransformed object
		//// - a BBox in [ 0.0; 1.0 ] x [ 0.0; 1.0 ] x [ 0.0; 1.0 ]
		//float boxInterMin = 0.0f;
		//float boxInterMax = 10000.0f;
		//int hit = intersectBox( smRayStart, rayDir, make_float3( 0.001f ), make_float3( 0.999f ), boxInterMin, boxInterMax );
		//// ---------------------------------------
		//// TODO : here, read from previous pass
		//// ---------------------------------------
		//bool masked = ! ( hit && ( boxInterMax > 0.0f ) );

			// ---------------------------------------
		// Retrieve data from previous pass
		float currentDepth = pRayDepthBuffer.get( pixelCoords );
		float depthMax = pRayMaxDepthBuffer.get( pixelCoords );
		//float4 accumulatedColor = pRayAccumulatedColorBuffer.get( pixelCoords );
		// ---------------------------------------

		// ---------------------------------------
		// Check data
		if ( currentDepth < 0.f )
		{
			return;
		}
		if ( depthMax < 0.f )
		{
			return;
		}
		// ---------------------------------------

		// ---------------------------------------
		// Normal stuff
		// ...
		
		float boxInterMin = currentDepth;
		float boxInterMax = depthMax;
		int hit = ( boxInterMin > 0.f ) && ( boxInterMax > 0.f );	// use >= instead ?
		bool masked = ! hit;
		
		// Set closest hit point
		boxInterMin = maxcc( boxInterMin, k_renderViewContext.frustumNear );	// TO DO : attention, c'est faux => frustumNear est en "espace camera" !!
		float t = boxInterMin + sampleShader.getConeAperture( boxInterMin );
		
		// Set farthest hit point
		float tMax = boxInterMax;
		if ( frameDepth < 1.0f )
		{
			// Retrieve the view-space depth from the depth buffer. Only works if w was 1.0f.
			// See: http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=304624&page=2

			// Compute z in NDC space
			//
			// True equation is :
			//     float zNDC = ( 2.f * frameDepth - k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) / ( k_renderViewContext.depthRangeFar - k_renderViewContext.depthRangeNear ) - 1.f;
			// but a simplication occurs if glDepthRange values are ( 0.0; 1.0 )
			//     float zNDC = 2.0f * frameDepth - 1.0f;
			const float zNDC = 2.0f * frameDepth - 1.0f;

			// Compute z in Eye space
			const float zEye = k_renderViewContext.frustumD / ( -zNDC - k_renderViewContext.frustumC );
			
			// Take minimum value between input depth and bbox output
			tMax = mincc( -zEye, boxInterMax );
		}

		// Discard special cases
		if ( t == 0.0f || t >= tMax )
		{
			masked = true;
		}

		// If intersection
		if ( ! masked )
		{
			// Read color from the input color buffer
			uchar4 frameColor = getInputColor( pixelCoords );

			// Launch N3-tree traversal and rendering
			//GsRendererKernel::render< TFastUpdateMode, TPriorityOnBrick >( pVolumeTree, sampleShader, pCache, pixelCoords, smRayStart, rayDir, tMax, t );
		
			// Retrieve the accumulated color along the ray
			//const float4 accCol = sampleShader.getColor();
			// ---------------------------------------
			// Retrieve data from previous pass
			const float4 accCol = pRayAccumulatedColorBuffer.get( pixelCoords );
			// ---------------------------------------

			// Convert color from uchar [ 0 ; 255 ] to float [ 0.0 ; 1.0 ]
			float4 scenePixelColorF = make_float4( (float)frameColor.x / 255.0f, (float)frameColor.y / 255.0f, (float)frameColor.z / 255.0f, (float)frameColor.w / 255.0f );
		
			// TO DO
			// - let OpenGL do that operation with blending ?

			// Blend colors (ray and framebuffer)
			float4 pixelColorF = accCol + scenePixelColorF * ( 1.0f - accCol.w );

			// Clamp color to be within the interval [+0.0, 1.0]
			pixelColorF.x = __saturatef( pixelColorF.x );
			pixelColorF.y = __saturatef( pixelColorF.y );
			pixelColorF.z = __saturatef( pixelColorF.z );
			//pixelColorF.w = 1.0f;		// <== why 1.0f and not __saturatef( pixelColorF.w ) ?	// Pour éviter une opération OpenGL de ROP ? Car ça a été penser pour rendre en dernier au départ ?
			pixelColorF.w = __saturatef( pixelColorF.w );
			//pixelColorF.w = __saturatef( pixelColorF.w );
			
			// Convert color from float [ 0.0 ; 1.0 ] to uchar [ 0 ; 255 ]
			frameColor = make_uchar4( (uchar)( pixelColorF.x * 255.0f ), (uchar)( pixelColorF.y * 255.0f ), (uchar)( pixelColorF.z * 255.0f ), (uchar)( pixelColorF.w * 255.0f ) );
			
			// Project the depth and check against the current one
			float pixDepth = 1.0f;
			if ( accCol.w > cOpacityDepthThreshold )
			{
				const float VP = -fabsf( t * rayDir.z );
				//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=234519&page=2
				const float clipZ = ( VP * k_renderViewContext.frustumC + k_renderViewContext.frustumD ) / -VP;
				
				//pixDepth = clamp( ( clipZ + 1.0f ) / 2.0f, 0.0f, 1.0f ); // TO DO : use __saturatef instead !!	=====> ( [ x 0.5f ] instead ) ??
				pixDepth = __saturatef( clipZ * 0.5f + 0.5f );
				
				//const float zNDC = - ( k_renderViewContext.frustumC + k_renderViewContext.frustumD / ( -t ) );
				//const float zWindow = zNDC * 0.5f + * 0.5f;
				//pixDepth = zWindow;
			}
			frameDepth = min( frameDepth, pixDepth );

			// Write color in color buffer
			setOutputColor( pixelCoords, frameColor );
			
			// Write depth in depth buffer
			setOutputDepth( pixelCoords, frameDepth );
		}
	} // !outOfFrame
}

// FIXME: Move this to another place
/******************************************************************************
 * CUDA kernel ...
 *
 * @param syntheticBuffer ...
 * @param totalNumElems ...
 ******************************************************************************/
__global__
// __launch_bounds__( maxThreadsPerBlock, minBlocksPerMultiprocessor )
void SyntheticInfo_Render( uchar4 *syntheticBuffer, uint totalNumElems )
{
	uint2 pixelCoords;
	pixelCoords.x = threadIdx.x + __uimul(blockIdx.x, blockDim.x);
	pixelCoords.y = threadIdx.y + __uimul(blockIdx.y, blockDim.y);

	uint elemIdx= pixelCoords.x+pixelCoords.y* (blockDim.x*gridDim.x);

	//uint totalNumElems=syntheticBuffer.getResolution().x;

	uchar4 pixelColor;

	if ( elemIdx < totalNumElems )
	{
		uchar4 sval = syntheticBuffer[ elemIdx ];

		if ( sval.w )
		{
			pixelColor = make_uchar4( 255, 0, 0, 255 );
		}
		else if ( sval.x )
		{
			pixelColor = make_uchar4( 0, 255, 0, 255 );
		}
		else
		{
			pixelColor = make_uchar4( 0, 0, 0, 255 );
		}

		setOutputColor( pixelCoords, pixelColor );
	}
	else
	{
		//pixelColor = make_uchar4( 255, 255, 255, 255 );
	}
}

} // namespace GvRendering

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvRendering
{

/******************************************************************************
 * Initialize the pixel coordinates.
 *
 * @param Pid the input thread identifiant
 * @param blockPos the computed block position
 * @param pixelCoords the computed pixel coordinates
 ******************************************************************************/
template< class TBlockResolution >
__device__
__forceinline__ void GsRendererKernel
::initPixelCoords( const uint Pid, /*uint2& blockPos,*/ uint2& pixelCoords )
{
#if RENDER_USE_SWIZZLED_THREADS==0
	pixelCoords.x = threadIdx.x + __uimul( blockIdx.x, TBlockResolution::x );
	pixelCoords.y = threadIdx.y + __uimul( blockIdx.y, TBlockResolution::y );
#else // Z-curve scheduling

	// Deinterleave bits
	deinterleaveBits32( Pid, pixelCoords );

// As "blockPos" parameter seems to be unused anymore, the following are commented.
// TO DO : are following lines useful ?
//#if 1
//	blockPos = make_uint2( blockIdx.x, blockIdx.y );
//#else
//	uint Bid = blockIdx.x + blockIdx.y * gridDim.x;
//	deinterleaveBits32( Bid, blockPos );
//#endif

	pixelCoords.x += /*blockPos.x*/blockIdx.x * TBlockResolution::x;
	pixelCoords.y += /*blockPos.y*/blockIdx.y * TBlockResolution::y;
#endif
}

/******************************************************************************
 * This function is used to :
 * - traverse the data structure (and emit requests if necessary)
 * - render bricks
 *
 * @param pDataStructure data structure
 * @param pShader shader
 * @param pCache cahce
 * @param pPixelCoords pixel coordinates in window
 * @param pRayStartTree ray start point
 * @param pRayDirTree ray direction
 * @param ptMaxTree max distance from camera found after box intersection test and comparing with input z (from the scene)
 * @param ptTree the distance from camera found after box intersection test and comparing with input z (from the scene)
 ******************************************************************************/
template< bool TFastUpdateMode, bool TPriorityOnBrick, class VolTreeKernelType, class SampleShaderType, class TCacheType >
__device__
__forceinline__ void GsRendererKernel
::render( VolTreeKernelType& pDataStructure, SampleShaderType& pShader, TCacheType& pCache,
		  uint2 pPixelCoords, const float3 pRayStartTree, const float3 pRayDirTree, const float ptMaxTree, float& ptTree )
{
	CUDAPM_KERNEL_DEFINE_EVENT( 2 )
	CUDAPM_KERNEL_DEFINE_EVENT( 3 )
	CUDAPM_KERNEL_DEFINE_EVENT( 4 )
	CUDAPM_KERNEL_DEFINE_EVENT( 5 )

	CUDAPM_KERNEL_START_EVENT( pPixelCoords, 5 );

	// Keep root node in cache
	pCache._nodeCacheManager.setElementUsage( 0 );
	
	// Initialize the brick sampler, a helper class used to sample data in the data structure
	GsSamplerKernel< VolTreeKernelType > brickSampler;
	brickSampler._volumeTree = &pDataStructure;

	// Initialize the position at wich data will be sampled
	float3 samplePosTree = pRayStartTree + ptTree * pRayDirTree;

	// Shader pre-shade process
	pShader.preShade( pRayStartTree, pRayDirTree, ptTree );

	// Ray marching.
	// Step with "ptTree" along ray from start to stop bounds.
	int numLoop = 0;
	while
		( ptTree < ptMaxTree
		&& numLoop < 5000	// TO DO : remove this hard-coded value or let only for DEBUG
		&& !pShader.stopCriterion( samplePosTree ) )
	{
		CUDAPM_KERNEL_START_EVENT( pPixelCoords, 4 );

		//float3 samplePosTree = pRayStartTree + ptTree * pRayDirTree;
		const float coneAperture = pShader.getConeAperture( ptTree );
		
		// Declare an empty node of the data structure.
		// It will be filled during the traversal according to cuurent sample position "samplePosTree"
		GvStructure::GsNode node;

		CUDAPM_KERNEL_START_EVENT( pPixelCoords, 2 );

		// [ 1 ]- Descent the data structure (in general an octree)
		// until max depth is reach or current traversed node has no subnodes,
		// or cone aperture is greater than voxel size.
		float nodeSizeTree;
		float3 sampleOffsetInNodeTree;
		bool modifInfoWriten = false;
		GsNodeVisitorKernel::visit< TPriorityOnBrick, VolTreeKernelType, TCacheType >
							( pDataStructure, pCache, node, samplePosTree, coneAperture,
							nodeSizeTree, sampleOffsetInNodeTree, brickSampler, modifInfoWriten );

		const float rayLengthInNodeTree = getRayLengthInNode( sampleOffsetInNodeTree, nodeSizeTree, pRayDirTree );

		CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 2 );

		// Early loop termination
		if ( TFastUpdateMode && modifInfoWriten )
		{
			break;
		}

		// [ 2 ] - If node is a brick, renderer it.
		if ( node.isBrick() )	// todo : check if it should be hasBrick() instead !??????????
		{
			CUDAPM_KERNEL_START_EVENT( pPixelCoords, 3 );

			// PASCAL
			// This is used to stop the ray with a z-depth value smaller than the remaining brick ray length
			//
			// QUESTION : pas forcément, si objet qui cache est transparent !??
			// => non, comme d'hab en OpenGL => dessiner d'abord les objets opaques
			const float rayLengthInBrick = mincc( rayLengthInNodeTree, ptMaxTree - ptTree );	// ==> attention, ( ptMaxTree - ptTree < 0 ) ?? ==> non, à casue du test du WHILE !! c'est OK !!
																								// MAIS possible en cas d'erreur sur "float" !!!!!

			// This is where shader program occurs
			float dt = GsBrickVisitorKernel::visit< TFastUpdateMode, TPriorityOnBrick >
											( pDataStructure, pShader, pCache, pRayStartTree, pRayDirTree,
											//ptTree, rayLengthInNodeTree, brickSampler, modifInfoWriten );
											ptTree, rayLengthInBrick, brickSampler, modifInfoWriten );

			CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 3 );

			ptTree += dt;
		}
		else
		{
			ptTree += rayLengthInNodeTree;// + coneAperture;
			ptTree += pShader.getConeAperture( ptTree );
		}

		samplePosTree = pRayStartTree + ptTree * pRayDirTree;

		// Update internal counter
		numLoop++;

		CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 4 );
	} // while

	CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 5 );

	// Shader post-shade process
	pShader.postShade();
}

/******************************************************************************
 * This function is used to :
 * - traverse the data structure (and emit requests if necessary)
 * - render bricks
 *
 * @param pDataStructure data structure
 * @param pShader shader
 * @param pCache cahce
 * @param pPixelCoords pixel coordinates in window
 * @param pRayStartTree ray start point
 * @param pRayDirTree ray direction
 * @param ptMaxTree max distance from camera found after box intersection test and comparing with input z (from the scene)
 * @param ptTree the distance from camera found after box intersection test and comparing with input z (from the scene)
 ******************************************************************************/
template< bool TFastUpdateMode, bool TPriorityOnBrick, class VolTreeKernelType, class SampleShaderType, class TCacheType >
__device__
__forceinline__ void GsRendererKernel
::render_quality( VolTreeKernelType& pDataStructure, SampleShaderType& pShader, TCacheType& pCache,
		  uint2 pPixelCoords, const float3 pRayStartTree, const float3 pRayDirTree, const float ptMaxTree, float& ptTree,
		  const float4& pRayAccumulatedColor )
{
	CUDAPM_KERNEL_DEFINE_EVENT( 2 )
	CUDAPM_KERNEL_DEFINE_EVENT( 3 )
	CUDAPM_KERNEL_DEFINE_EVENT( 4 )
	CUDAPM_KERNEL_DEFINE_EVENT( 5 )

	CUDAPM_KERNEL_START_EVENT( pPixelCoords, 5 );

	// Keep root node in cache
	pCache._nodeCacheManager.setElementUsage( 0 );
	
	// Initialize the brick sampler, a helper class used to sample data in the data structure
	GsSamplerKernel< VolTreeKernelType > brickSampler;
	brickSampler._volumeTree = &pDataStructure;

	// Initialize the position at wich data will be sampled
	float3 samplePosTree = pRayStartTree + ptTree * pRayDirTree;

	// Shader pre-shade process
	pShader.preShade( pRayStartTree, pRayDirTree, ptTree );
	// ---------------------------------------
	// Retrieve data from previous pass
	pShader.setColor( pRayAccumulatedColor );
	// ---------------------------------------

	// Ray marching.
	// Step with "ptTree" along ray from start to stop bounds.
	int numLoop = 0;
	while
		( ptTree < ptMaxTree
		&& numLoop < 5000	// TO DO : remove this hard-coded value or let only for DEBUG
		&& !pShader.stopCriterion( samplePosTree ) )
	{
		CUDAPM_KERNEL_START_EVENT( pPixelCoords, 4 );

		//float3 samplePosTree = pRayStartTree + ptTree * pRayDirTree;
		const float coneAperture = pShader.getConeAperture( ptTree );
		
		// Declare an empty node of the data structure.
		// It will be filled during the traversal according to cuurent sample position "samplePosTree"
		GvStructure::GsNode node;

		CUDAPM_KERNEL_START_EVENT( pPixelCoords, 2 );

		// [ 1 ]- Descent the data structure (in general an octree)
		// until max depth is reach or current traversed node has no subnodes,
		// or cone aperture is greater than voxel size.
		float nodeSizeTree;
		float3 sampleOffsetInNodeTree;
		bool modifInfoWriten = false;
		GsNodeVisitorKernel::visit< TPriorityOnBrick, VolTreeKernelType, TCacheType >
							( pDataStructure, pCache, node, samplePosTree, coneAperture,
							nodeSizeTree, sampleOffsetInNodeTree, brickSampler, modifInfoWriten );

		const float rayLengthInNodeTree = getRayLengthInNode( sampleOffsetInNodeTree, nodeSizeTree, pRayDirTree );

		CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 2 );

		// Early loop termination
		if ( TFastUpdateMode && modifInfoWriten )
		{
			break;
		}

		// [ 2 ] - If node is a brick, renderer it.
		if ( node.isBrick() )	// todo : check if it should be hasBrick() instead !??????????
		{
			CUDAPM_KERNEL_START_EVENT( pPixelCoords, 3 );

			// PASCAL
			// This is used to stop the ray with a z-depth value smaller than the remaining brick ray length
			//
			// QUESTION : pas forcément, si objet qui cache est transparent !??
			// => non, comme d'hab en OpenGL => dessiner d'abord les objets opaques
			const float rayLengthInBrick = mincc( rayLengthInNodeTree, ptMaxTree - ptTree );	// ==> attention, ( ptMaxTree - ptTree < 0 ) ?? ==> non, à casue du test du WHILE !! c'est OK !!
																								// MAIS possible en cas d'erreur sur "float" !!!!!

			// This is where shader program occurs
			float dt = GsBrickVisitorKernel::visit< TFastUpdateMode, TPriorityOnBrick >
											( pDataStructure, pShader, pCache, pRayStartTree, pRayDirTree,
											//ptTree, rayLengthInNodeTree, brickSampler, modifInfoWriten );
											ptTree, rayLengthInBrick, brickSampler, modifInfoWriten );

			CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 3 );

			ptTree += dt;
		}
		else
		{
			ptTree += rayLengthInNodeTree;// + coneAperture;
			ptTree += pShader.getConeAperture( ptTree );
		}

		samplePosTree = pRayStartTree + ptTree * pRayDirTree;

		// Update internal counter
		numLoop++;

		CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 4 );
	} // while

	CUDAPM_KERNEL_STOP_EVENT( pPixelCoords, 5 );

	// Shader post-shade process
	pShader.postShade();
}

} // namespace GvRendering
