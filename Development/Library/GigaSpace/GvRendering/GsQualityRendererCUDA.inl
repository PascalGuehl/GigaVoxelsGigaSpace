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
#include "GvCore/GsError.h"

// Cuda SDK
#include <helper_cuda.h>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvRendering
{

/******************************************************************************
 * Constructor
 *
 * @param pVolumeTree data structure to render
 * @param pVolumeTreeCache cache
 * @param pProducer producer of data
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::GsQualityRendererCUDA( TVolumeTreeType* pVolumeTree, TVolumeTreeCacheType* pVolumeTreeCache )
:	GsRenderer< TVolumeTreeType, TVolumeTreeCacheType >( pVolumeTree, pVolumeTreeCache )
,	_graphicsInteroperabiltyHandler( NULL )
,	_viewContext()
,	_d_rayDepthBuffer( NULL )
,	_d_rayMaxDepthBuffer( NULL )
,	_d_rayAccumulatedColorBuffer( NULL )
{
	GV_CHECK_CUDA_ERROR( "GsQualityRendererCUDA::GsQualityRendererCUDA prestart" );

	// Init frame size
	_frameSize = make_uint2( 0, 0 );

	// Init frame dependant buffers//
	// Deferred lighting infos
	// ...

	_numUpdateFrames		= 1;
	_frameNumAfterUpdate	= 0;

	_fastBuildMode			= true;

	// Do CUDA initialization
	this->initializeCuda();

	// Initialize graphics interoperability
	_graphicsInteroperabiltyHandler = new GsGraphicsInteroperabiltyHandler();
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::~GsQualityRendererCUDA()
{
	// Destroy internal buffers used during rendering
	deleteFrameObjects();

	// Finalize graphics interoperability
	delete _graphicsInteroperabiltyHandler;

#ifdef _GS_RENDERER_USE_STREAM_
	// Destroy the dedicated stream for renderer
	cudaError_t result = cudaStreamDestroy( _rendererStream );
	GV_CHECK_CUDA_ERROR( "GsQualityRendererCUDA::cudaStreamDestroy" );
#endif
}

/******************************************************************************
 * Get the graphics interoperability handler
 *
 * @return the graphics interoperability handler
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
GsGraphicsInteroperabiltyHandler* GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::getGraphicsInteroperabiltyHandler()
{
	return _graphicsInteroperabiltyHandler;
}

/******************************************************************************
 * Initialize Cuda objects
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::initializeCuda()
{
	GV_CHECK_CUDA_ERROR( "VoxelSceneRenderer::cuda_Init pre-start" );

	// Retrieve device properties
	cudaDeviceProp deviceProps;
	cudaGetDeviceProperties( &deviceProps, gpuGetMaxGflopsDeviceId() );	// TO DO : handle the case where user could want an other device
	// LOG
	std::cout << "\nDevice properties" << std::endl;
	std::cout << "- name : " << deviceProps.name << std::endl;
	std::cout << "- compute capability : "<< deviceProps.major << "." << deviceProps.minor << std::endl;
	std::cout << "- compute mode : " << deviceProps.computeMode << std::endl;
	std::cout << "- can map host memory : " << deviceProps.canMapHostMemory << std::endl;
	std::cout << "- can overlap transfers and kernels : " << deviceProps.deviceOverlap << std::endl;
	std::cout << "- kernels timeout : " << deviceProps.kernelExecTimeoutEnabled << std::endl;
	std::cout << "- integrated chip : " << deviceProps.integrated << std::endl;
	std::cout << "- global memory : " << deviceProps.totalGlobalMem / 1024 / 1024 << "MB" << std::endl;
	std::cout << "- shared memory : " << deviceProps.sharedMemPerBlock / 1024 << "KB" << std::endl;
	std::cout << "- clock rate : " << deviceProps.clockRate / 1000 << "MHz" << std::endl;
	
	GV_CHECK_CUDA_ERROR( "GsQualityRendererCUDA::cuda_Init start" );

#ifdef _GS_RENDERER_USE_STREAM_
	// Create a dedicated stream for renderer
	cudaError_t result = cudaStreamCreate( &_rendererStream );
	GV_CHECK_CUDA_ERROR( "GsQualityRendererCUDA::cudaStreamCreate" );
#endif

	GV_CHECK_CUDA_ERROR( "GsQualityRendererCUDA::cuda_Init end" );
}

/******************************************************************************
 * Finalize Cuda objects
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::finalizeCuda()
{
}

/******************************************************************************
 * Initialize internal buffers used during rendering
 * (i.e. input/ouput color and depth buffers, ray buffers, etc...).
 * Buffers size are dependent of the frame size.
 *
 * @param pFrameSize the frame size
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::initFrameObjects( const uint2& pFrameSize )
{
	// Check if frame size has been modified
	if ( _frameSize.x != pFrameSize.x || _frameSize.y != pFrameSize.y )
	{
		_frameSize = pFrameSize;

		// Destruct frame based objects
		deleteFrameObjects();

	// Ray buffers
	uint2 frameResWithBlock =
	make_uint2(		iDivUp( _frameSize.x, RenderBlockResolution::x ) * RenderBlockResolution::x,
	iDivUp( _frameSize.y, RenderBlockResolution::y ) * RenderBlockResolution::y );
	_d_rayDepthBuffer = new GvCore::GsLinearMemory< float >( make_uint3( frameResWithBlock.x, frameResWithBlock.y, 1 ) );
	_d_rayMaxDepthBuffer = new GvCore::GsLinearMemory< float >( make_uint3( frameResWithBlock.x, frameResWithBlock.y, 1 ) );
	_d_rayAccumulatedColorBuffer = new GvCore::GsLinearMemory< float4 >( make_uint3( frameResWithBlock.x, frameResWithBlock.y, 1 ) );
	}

	GV_CHECK_CUDA_ERROR( "VoxelSceneRenderer::initFrameObjects" );
}

/******************************************************************************
 * Destroy internal buffers used during rendering
 * (i.e. input/ouput color and depth buffers, ray buffers, etc...)
 * Buffers size are dependent of the frame size.
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::deleteFrameObjects()
{
	// Destroy input/output color and depth buffers
	// ...

	// Destroy ray buffers
	delete _d_rayDepthBuffer;
	_d_rayDepthBuffer = NULL;
	delete _d_rayMaxDepthBuffer;
	_d_rayMaxDepthBuffer  = NULL;
	delete _d_rayAccumulatedColorBuffer;
	_d_rayAccumulatedColorBuffer = NULL;
}

/******************************************************************************
 * Attach an OpenGL buffer object (i.e. a PBO, a VBO, etc...) to an internal graphics resource 
 * that will be mapped to a color or depth slot used during rendering.
 *
 * @param pGraphicsResourceSlot the associated internal graphics resource (color or depth) and its type of access (read, write or read/write)
 * @param pBuffer the OpenGL buffer
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
bool GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::connect( GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot pGraphicsResourceSlot, GLuint pBuffer )
{
	return _graphicsInteroperabiltyHandler->connect( pGraphicsResourceSlot, pBuffer );
}

/******************************************************************************
 * Attach an OpenGL texture or renderbuffer object to an internal graphics resource 
 * that will be mapped to a color or depth slot used during rendering.
 *
 * @param pGraphicsResourceSlot the associated internal graphics resource (color or depth) and its type of access (read, write or read/write)
 * @param pImage the OpenGL texture or renderbuffer object
 * @param pTarget the target of the OpenGL texture or renderbuffer object
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
bool GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::connect( GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot pGraphicsResourceSlot, GLuint pImage, GLenum pTarget )
{
	return _graphicsInteroperabiltyHandler->connect( pGraphicsResourceSlot, pImage, pTarget );
}

/******************************************************************************
 * Dettach an OpenGL buffer object (i.e. a PBO, a VBO, etc...), texture or renderbuffer object
 * to its associated internal graphics resource mapped to a color or depth slot used during rendering.
 *
 * @param pGraphicsResourceSlot the internal graphics resource slot (color or depth)
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
bool GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::disconnect( GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot pGraphicsResourceSlot )
{
	return _graphicsInteroperabiltyHandler->disconnect( pGraphicsResourceSlot );
}

/******************************************************************************
 * Disconnect all registered graphics resources
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
bool GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::resetGraphicsResources()
{
	return _graphicsInteroperabiltyHandler->reset();
}

/******************************************************************************
 * This function is the specific implementation method called
 * by the parent GsIRenderer::render() method during rendering.
 *
 * @param pModelMatrix the current model matrix
 * @param pViewMatrix the current view matrix
 * @param pProjectionMatrix the current projection matrix
 * @param pViewport the viewport configuration
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::render( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	// Start rendering
	doRender( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );
}

/******************************************************************************
 * This function is the specific implementation method called
 * by the parent GsIRenderer::render() method during rendering.
 *
 * @param pModelMatrix the current model matrix
 * @param pViewMatrix the current view matrix
 * @param pProjectionMatrix the current projection matrix
 * @param pViewport the viewport configuration
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::preRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	// Initialize frame objects
	uint2 frameSize = make_uint2( pViewport.z - pViewport.x, pViewport.w - pViewport.y );
	initFrameObjects( frameSize );

	// NOTE
	// mapResources()
	// This function provides the synchronization guarantee that any graphics calls issued before cudaGraphicsMapResources() will complete before any subsequent CUDA work in stream begins.

	// Map graphics resources
	CUDAPM_START_EVENT( vsrender_pre_frame_mapbuffers );
	_graphicsInteroperabiltyHandler->mapResources();
	bindGraphicsResources();
	CUDAPM_STOP_EVENT( vsrender_pre_frame_mapbuffers );

	// TEST : put here beacuse the "call" blocks
	_graphicsInteroperabiltyHandler->setRendererContextInfo( _viewContext );	// TO DO : this call blocks
	
	// TO DO : add texture offsets
	// ...
}

/******************************************************************************
 * This function is the specific implementation method called
 * by the parent GsIRenderer::render() method during rendering.
 *
 * @param pModelMatrix the current model matrix
 * @param pViewMatrix the current view matrix
 * @param pProjectionMatrix the current projection matrix
 * @param pViewport the viewport configuration
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::postRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	// Unmap graphics resources
	CUDAPM_START_EVENT( vsrender_post_frame_unmapbuffers );
	unbindGraphicsResources();
	_graphicsInteroperabiltyHandler->unmapResources();
	CUDAPM_STOP_EVENT( vsrender_post_frame_unmapbuffers );
}

/******************************************************************************
 * Start the rendering process.
 *
 * @param pModelMatrix the current model matrix
 * @param pViewMatrix the current view matrix
 * @param pProjectionMatrix the current projection matrix
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::doRender( const float4x4& modelMatrix, const float4x4& viewMatrix, const float4x4& projMatrix, const int4& pViewport )
{
	// TO DO : check if all members of "GsRendererContext _viewContext" are reset to avoid old non-refreshed values.

	// Extract zNear, zFar as well as the distance in view space
	// from the center of the screen to each side of the screen.
	float fleft   = projMatrix._array[ 14 ] * ( projMatrix._array[ 8 ] - 1.0f ) / ( projMatrix._array[ 0 ] * ( projMatrix._array[ 10 ] - 1.0f ) );
	float fright  = projMatrix._array[ 14 ] * ( projMatrix._array[ 8 ] + 1.0f ) / ( projMatrix._array[ 0 ] * ( projMatrix._array[ 10 ] - 1.0f ) );
	float ftop    = projMatrix._array[ 14 ] * ( projMatrix._array[ 9 ] + 1.0f ) / ( projMatrix._array[ 5 ] * ( projMatrix._array[ 10 ] - 1.0f ) );
	float fbottom = projMatrix._array[ 14 ] * ( projMatrix._array[ 9 ] - 1.0f ) / ( projMatrix._array[ 5 ] * ( projMatrix._array[ 10 ] - 1.0f ) );
	float fnear   = projMatrix._array[ 14 ] / ( projMatrix._array[ 10 ] - 1.0f );
	float ffar    = projMatrix._array[ 14 ] / ( projMatrix._array[ 10 ] + 1.0f );
	
	float2 viewSurfaceVS[ 2 ];
	viewSurfaceVS[ 0 ] = make_float2( fleft, fbottom );
	viewSurfaceVS[ 1 ] = make_float2( fright, ftop );

	//float3 viewPlane[ 2 ];
	//viewPlane[ 0 ] = make_float3( fleft, fbottom, fnear );
	//viewPlane[ 1 ] = make_float3( fright, ftop, fnear );
	// float3 viewSize = ( viewPlane[ 1 ] - viewPlane[ 0 ] );

	// Projected 2D Bounding Box of the GigaVoxels 3D BBox.
	// It holds its bottom left corner and its size.
	// ( bottomLeftCorner.x, bottomLeftCorner.y, frameSize.x, frameSize.y )
	_viewContext._projectedBBox = this->_projectedBBox;
	
	float2 viewSurfaceVS_Size = viewSurfaceVS[ 1 ] - viewSurfaceVS[ 0 ];
	
	// Transform matrices
	float4x4 invModelMatrixT = transpose( inverse( modelMatrix ) );
	float4x4 invViewMatrixT = transpose( inverse( viewMatrix ) );

	float4x4 modelMatrixT = transpose( modelMatrix );
	float4x4 viewMatrixT = transpose( viewMatrix );

	//float4x4 viewMatrix=(inverse(invViewMatrix));

	_viewContext.invViewMatrix = invViewMatrixT;
	_viewContext.viewMatrix = viewMatrixT;
	_viewContext.invModelMatrix = invModelMatrixT;
	_viewContext.modelMatrix = modelMatrixT;

	// Store frustum parameters
	_viewContext.frustumNear = fnear;
	_viewContext.frustumNearINV = 1.0f / fnear;
	_viewContext.frustumFar = ffar;
	_viewContext.frustumRight = fright;
	_viewContext.frustumTop = ftop;
	_viewContext.frustumC = projMatrix._array[ 10 ]; // - ( ffar + fnear ) / ( ffar - fnear );
	_viewContext.frustumD = projMatrix._array[ 14 ]; // ( -2.0f * ffar * fnear ) / ( ffar - fnear );

	// Graphics resource settings
	_viewContext._clearColor = this->_clearColor;
	_viewContext._clearDepth = this->_clearDepth;

	// WORLD
	float3 viewPlanePosWP = mul( _viewContext.invViewMatrix, make_float3( fleft, fbottom, -fnear ) );
	_viewContext.viewCenterWP = mul( _viewContext.invViewMatrix, make_float3( 0.0f, 0.0f, 0.0f ) );
	_viewContext.viewPlaneDirWP = viewPlanePosWP - _viewContext.viewCenterWP;
	// TREE
	float3 viewPlanePosTP = mul( _viewContext.invModelMatrix, viewPlanePosWP );
	_viewContext.viewCenterTP = mul( _viewContext.invModelMatrix, _viewContext.viewCenterWP );
	_viewContext.viewPlaneDirTP = viewPlanePosTP - _viewContext.viewCenterTP;

	// Resolution dependant stuff
	_viewContext.frameSize = _frameSize;
	float2 pixelSize = viewSurfaceVS_Size / make_float2( static_cast< float >( _viewContext.frameSize.x ), static_cast< float >( _viewContext.frameSize.y ) );
	_viewContext.pixelSize = pixelSize;
	/*_viewContext.viewPlaneXAxisWP = ( mul( _viewContext.invViewMatrix, make_float3( fright, fbottom, -fnear ) ) - viewPlanePosWP ) / static_cast< float >( _viewContext.frameSize.x );
	_viewContext.viewPlaneYAxisWP = ( mul( _viewContext.invViewMatrix, make_float3( fleft, ftop, -fnear ) ) - viewPlanePosWP ) / static_cast< float >( _viewContext.frameSize.y );*/
	// WORLD
	_viewContext.viewPlaneXAxisWP = mul( _viewContext.invViewMatrix, make_float3( fright, fbottom, -fnear ) );
	_viewContext.viewPlaneYAxisWP = mul( _viewContext.invViewMatrix, make_float3( fleft, ftop, -fnear ) );
	// TREE
	_viewContext.viewPlaneXAxisTP = mul( _viewContext.invModelMatrix, _viewContext.viewPlaneXAxisWP );
	_viewContext.viewPlaneYAxisTP = mul( _viewContext.invModelMatrix, _viewContext.viewPlaneYAxisWP );
	// WORLD
	_viewContext.viewPlaneXAxisWP = ( _viewContext.viewPlaneXAxisWP - viewPlanePosWP ) / static_cast< float >( _viewContext.frameSize.x );
	_viewContext.viewPlaneYAxisWP = ( _viewContext.viewPlaneYAxisWP - viewPlanePosWP ) / static_cast< float >( _viewContext.frameSize.y );
	// TREE
	_viewContext.viewPlaneXAxisTP = ( _viewContext.viewPlaneXAxisTP - viewPlanePosTP ) / static_cast< float >( _viewContext.frameSize.x );
	_viewContext.viewPlaneYAxisTP = ( _viewContext.viewPlaneYAxisTP - viewPlanePosTP ) / static_cast< float >( _viewContext.frameSize.y );

	CUDAPM_START_EVENT( vsrender_copyconsts_frame );

	// Asynchronous call
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbolAsync( k_renderViewContext, &_viewContext, sizeof( _viewContext ) ) );

	// Asynchronous call
	// TO DO : optimization
	// - place this value in the big k_renderViewContext structure, to avoid launching a copy.
	// - Usually, graphics cards have only one copy engine, copies are serialized ? => not sure, check this point.
	// - at least, move this call at beginning to be able to overlap cpu computation to fill k_renderViewContext
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbolAsync( k_currentTime, (&this->_currentTime), sizeof( this->_currentTime ), 0, cudaMemcpyHostToDevice) );
	
	// TO DO : optimization
	// - this variable seems to be unused anymore => remove it
	// - furthermore, this value should not change every frame => remove that and call cudaMemcpyToSymbol only when required
	// - at least, move this call at beginning to be able to overlap cpu computation to fill k_renderViewContext
	//float voxelSizeMultiplier = 1.0f;
	//GS_CUDA_SAFE_CALL( cudaMemcpyToSymbolAsync( k_voxelSizeMultiplier, (&voxelSizeMultiplier), sizeof( voxelSizeMultiplier ), 0, cudaMemcpyHostToDevice ) );

	// TO DO : move this "Performance monitor" piece of code in another place
	#ifdef USE_CUDAPERFMON
		// Performance monitor
		if ( GvPerfMon::CUDAPerfMon::get()._requestResize ) 
		{
			// Update device memory
			GvCore::GsLinearMemory< GvCore::uint64 >* my_d_timersArray = GvPerfMon::CUDAPerfMon::get().getKernelTimerArray();
			GvCore::GsLinearMemoryKernel< GvCore::uint64 > h_timersArray = my_d_timersArray->getDeviceArray();
			GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( GvPerfMon::k_timersArray, &h_timersArray, sizeof( h_timersArray ), 0, cudaMemcpyHostToDevice ) );
		
			// Update device memory
			uchar* my_d_timersMask = GvPerfMon::CUDAPerfMon::get().getKernelTimerMask();
			GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( GvPerfMon::k_timersMask, &my_d_timersMask, sizeof( my_d_timersMask ), 0, cudaMemcpyHostToDevice ) );

			// Update the performnace monitor's state
			GvPerfMon::CUDAPerfMon::get()._requestResize = false;
		}
	#endif
	
	CUDAPM_STOP_EVENT( vsrender_copyconsts_frame );

	CUDAPM_START_EVENT_GPU( gv_rendering );

	dim3 blockSize( RenderBlockResolution::x, RenderBlockResolution::y, 1 );
	dim3 gridSize( iDivUp( _frameSize.x, RenderBlockResolution::x ), iDivUp( _frameSize.y, RenderBlockResolution::y ), 1 );
	// FUTUR optimization
	//
	//dim3 gridSize( iDivUp( /*projectedBBoxSize*/_projectedBBox.z, RenderBlockResolution::x ), iDivUp( /*projectedBBoxSize*/_projectedBBox.w, RenderBlockResolution::y ), 1 );
	
	// TO DO
	// Configuring the amount of shared memory
	// - consider using "cudaDeviceSetCacheConfig()" or on a per-kernel basis using "cudaFuncSetCacheConfig()", cudaDeviceSetSharedMemConfig

	//--------------------------------------------------------------------------------------------------------------------

	// Idea : store _d_rayDepthBuffer, _d_rayMaxDepthBuffer, and _d_rayAccumulatedColorBuffer in constant memory ?
	// ...

	// Step 1 - Initialize rays
	GsKernel_QualityRenderer_Initialize< RenderBlockResolution, false, true, TSampleShader >
		<<< gridSize, blockSize >>>(
			this->_volumeTree->volumeTreeKernel, this->_volumeTreeCache->getKernelObject(),
			_d_rayDepthBuffer->getDeviceArray(), _d_rayMaxDepthBuffer->getDeviceArray(), _d_rayAccumulatedColorBuffer->getDeviceArray() );

	// Step 2 - Continue rays
	
	// Iterate through raycasting pass
	const uint nbPass = 5;
	for ( uint i = 0; i < nbPass; i++ )
	{
		GsKernel_QualityRenderer_Continue< RenderBlockResolution, false, true, TSampleShader >
			<<< gridSize, blockSize >>>(
				this->_volumeTree->volumeTreeKernel, this->_volumeTreeCache->getKernelObject(),
				_d_rayDepthBuffer->getDeviceArray(), _d_rayMaxDepthBuffer->getDeviceArray(), _d_rayAccumulatedColorBuffer->getDeviceArray() );
	}

	// Step 3 - Finalize rays
	GsKernel_QualityRenderer_Finalize< RenderBlockResolution, false, true, TSampleShader >
		<<< gridSize, blockSize >>>(
			this->_volumeTree->volumeTreeKernel, this->_volumeTreeCache->getKernelObject(),
			_d_rayDepthBuffer->getDeviceArray(), _d_rayMaxDepthBuffer->getDeviceArray(), _d_rayAccumulatedColorBuffer->getDeviceArray() );

	//--------------------------------------------------------------------------------------------------------------------

	if ( this->_dynamicUpdate )
	{
		if ( this->_hasPriorityOnBricks )
		{
			// Priority on brick is set to TRUE to force loading data at low resolution first
			RenderKernelSimple< RenderBlockResolution, false, true, TSampleShader >
#ifndef _GS_RENDERER_USE_STREAM_
							<<< gridSize, blockSize >>>(
#else
							<<< gridSize, blockSize, 0/*default shared memory parameter*/, _rendererStream/*dedicated stream*/ >>>(
#endif
								this->_volumeTree->volumeTreeKernel,
								this->_volumeTreeCache->getKernelObject() );
		}
		else
		{
			RenderKernelSimple< RenderBlockResolution, false, false, TSampleShader >
#ifndef _GS_RENDERER_USE_STREAM_
							<<< gridSize, blockSize >>>(
#else
							<<< gridSize, blockSize, 0/*default shared memory parameter*/, _rendererStream/*dedicated stream*/ >>>(
#endif
								this->_volumeTree->volumeTreeKernel,
								this->_volumeTreeCache->getKernelObject() );
		}
	}
	else
	{
		RenderKernelSimple< RenderBlockResolution, false, false, TSampleShader >
#ifndef _GS_RENDERER_USE_STREAM_
						<<< gridSize, blockSize >>>(
#else
						<<< gridSize, blockSize, 0/*default shared memory parameter*/, _rendererStream/*dedicated stream*/ >>>(
#endif
							this->_volumeTree->volumeTreeKernel,
							this->_volumeTreeCache->getKernelObject() );
	}
	GV_CHECK_CUDA_ERROR( "RenderKernelSimple" );

	CUDAPM_STOP_EVENT_GPU( gv_rendering );
	
	CUDAPM_RENDER_CACHE_INFO( 256, 512 );

	/*{
		uint2 poolRes = make_uint2(180, 150);
		uint2 poolScale = make_uint2(2, 2);

		dim3 blockSize(10, 10, 1);
		dim3 gridSize(poolRes.x * poolScale.x / blockSize.x, poolRes.y * poolScale.y / blockSize.y, 1);
		RenderDebug<<<gridSize, blockSize, 0>>>(d_outFrameColor->getDeviceArray(), poolRes, poolScale);
	}*/
}

/******************************************************************************
 * Bind all graphics resources used by the GL interop handler during rendering.
 *
 * Internally, it binds textures and surfaces to arrays associated to mapped graphics reources.
 *
 * NOTE : this method should be in the GsGraphicsInteroperabiltyHandler but it seems that
 * there are conflicts with textures ans surfaces symbols. The binding succeeds but not the
 * read/write operations.
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
bool GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::bindGraphicsResources()
{
	// Iterate through graphics resources info
	std::vector< std::pair< GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot, GsGraphicsResource* > >& graphicsResources = _graphicsInteroperabiltyHandler->editGraphicsResources();
	for ( int i = 0; i < graphicsResources.size(); i++ )
	{
		// Get current graphics resources info
		std::pair< GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot, GsGraphicsResource* >& graphicsResourceInfo = graphicsResources[ i ];
		GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot graphicsResourceSlot = graphicsResourceInfo.first;
		GsGraphicsResource* graphicsResource  = graphicsResourceInfo.second;
		assert( graphicsResource != NULL );

		// [ 2 ] - Bind array to texture or surface if needed
		if ( graphicsResource->getMemoryType() == GsGraphicsResource::eCudaArray )
		{
			struct cudaArray* imageArray = static_cast< struct cudaArray* >( graphicsResource->getMappedAddress() );

			switch ( graphicsResourceSlot )
			{
				case GsGraphicsInteroperabiltyHandler::eColorReadSlot:
					cudaBindTextureToArray( GvRendering::_inputColorTexture, imageArray );
					break;

				case GsGraphicsInteroperabiltyHandler::eColorWriteSlot:
				case GsGraphicsInteroperabiltyHandler::eColorReadWriteSlot:
					cudaBindSurfaceToArray( GvRendering::_colorSurface, imageArray );
					break;

				case GsGraphicsInteroperabiltyHandler::eDepthReadSlot:
					cudaBindTextureToArray( GvRendering::_inputDepthTexture, imageArray );
					break;

				case GsGraphicsInteroperabiltyHandler::eDepthWriteSlot:
				case GsGraphicsInteroperabiltyHandler::eDepthReadWriteSlot:
					cudaBindSurfaceToArray( GvRendering::_depthSurface, imageArray );
					break;

				default:
					assert( false );
					break;
			}
		}
	}

	return false;
}

/******************************************************************************
 * Unbind all graphics resources used by the GL interop handler during rendering.
 *
 * Internally, it unbinds textures and surfaces to arrays associated to mapped graphics reources.
 *
 * NOTE : this method should be in the GsGraphicsInteroperabiltyHandler but it seems that
 * there are conflicts with textures ans surfaces symbols. The binding succeeds but not the
 * read/write operations.
 *
 * @return a flag telling wheter or not it succeeds
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
bool GsQualityRendererCUDA< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::unbindGraphicsResources()
{
	// Iterate through graphics resources info
	std::vector< std::pair< GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot, GsGraphicsResource* > >& graphicsResources = _graphicsInteroperabiltyHandler->editGraphicsResources();
	for ( int i = 0; i < graphicsResources.size(); i++ )
	{
		// Get current graphics resources info
		std::pair< GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot, GsGraphicsResource* >& graphicsResourceInfo = graphicsResources[ i ];
		GsGraphicsInteroperabiltyHandler::GraphicsResourceSlot graphicsResourceSlot = graphicsResourceInfo.first;
		GsGraphicsResource* graphicsResource  = graphicsResourceInfo.second;
		assert( graphicsResource != NULL );

		// [ 2 ] - Bind array to texture or surface if needed
		if ( graphicsResource->getMemoryType() == GsGraphicsResource::eCudaArray )
		{
			struct cudaArray* imageArray = static_cast< struct cudaArray* >( graphicsResource->getMappedAddress() );

			switch ( graphicsResourceSlot )
			{
				case GsGraphicsInteroperabiltyHandler::eColorReadSlot:
					cudaUnbindTexture( GvRendering::_inputColorTexture );
					break;

				case GsGraphicsInteroperabiltyHandler::eColorWriteSlot:
				case GsGraphicsInteroperabiltyHandler::eColorReadWriteSlot:
					// There is no "unbind surface" function in CUDA
					break;

				case GsGraphicsInteroperabiltyHandler::eDepthReadSlot:
					cudaUnbindTexture( GvRendering::_inputDepthTexture );
					break;

				case GsGraphicsInteroperabiltyHandler::eDepthWriteSlot:
				case GsGraphicsInteroperabiltyHandler::eDepthReadWriteSlot:
					// There is no "unbind surface" function in CUDA
					break;

				default:
					assert( false );
					break;
			}
		}
	}

	return false;
}

} // namespace GvRendering
