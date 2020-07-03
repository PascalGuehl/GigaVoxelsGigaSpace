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

//#include "SampleCore.h"
//
///******************************************************************************
// ******************************* INCLUDE SECTION ******************************
// ******************************************************************************/
//
//// GigaVoxels
//#include <GvCore/GsVector.h>
//#include <GvStructure/GsVolumeTree.h>
//#include <GvStructure/GsDataProductionManager.h>
//#include <GvRendering/GsRendererCUDA.h>
//#include <GvRendering/GsGraphicsInteroperabiltyHandler.h>
//#include <GvUtils/GsSimplePipeline.h>
//#include <GvUtils/GsSimpleHostProducer.h>
//#include <GvUtils/GsSimpleHostShader.h>
//#include <GvCore/GsError.h>
//#include <GvPerfMon/GsPerformanceMonitor.h>
//
//// Project
//#include "ProducerKernel.h"
//#include "ShaderKernel.h"
//
///******************************************************************************
// ****************************** NAMESPACE SECTION *****************************
// ******************************************************************************/
//
//// GigaVoxels
//using namespace GvRendering;
//using namespace GvUtils;
//
///******************************************************************************
// ************************* DEFINE AND CONSTANT SECTION ************************
// ******************************************************************************/
//
//// Defines the size allowed for each type of pool
//#define NODEPOOL_MEMSIZE	( 8U * 1024U * 1024U )		// 8 Mo
//#define BRICKPOOL_MEMSIZE	( 256U * 1024U * 1024U )	// 256 Mo
//
///******************************************************************************
// ***************************** TYPE DEFINITION ********************************
// ******************************************************************************/
//
///******************************************************************************
// ***************************** METHOD DEFINITION ******************************
// ******************************************************************************/
//
///******************************************************************************
// * Constructor
// ******************************************************************************/
//SampleCore::SampleCore()
//:	_pipeline( NULL )
//,	_depthBuffer( 0 )
//,	_colorTex( 0 )
//,	_depthTex( 0 )
//,	_frameBuffer( 0 )
//,	_width( 0 )
//,	_height( 0 )
//,	_displayOctree( false )
//,	_displayPerfmon( 0 )
//,	_maxVolTreeDepth( 0 )
//{
//}
//
///******************************************************************************
// * Destructor
// ******************************************************************************/
//SampleCore::~SampleCore()
//{
//	delete _pipeline;
//}
//
///******************************************************************************
// * Initialize the GigaVoxels pipeline
// ******************************************************************************/
//void SampleCore::init()
//{
//	CUDAPM_INIT();
//
//	// Initialize CUDA with OpenGL Interoperability
//	//cudaGLSetGLDevice( gpuGetMaxGflopsDeviceId() );	// to do : deprecated, use cudaSetDevice()
//	//GV_CHECK_CUDA_ERROR( "cudaGLSetGLDevice" );
//	cudaSetDevice( gpuGetMaxGflopsDeviceId() );
//	GV_CHECK_CUDA_ERROR( "cudaSetDevice" );
//
//	// Pipeline creation
//	_pipeline = new PipelineType();
//	ProducerType* producer = new ProducerType();
//	ShaderType* shader = new ShaderType();
//
//	// Pipeline initialization
//	_pipeline->initialize( NODEPOOL_MEMSIZE, BRICKPOOL_MEMSIZE, producer, shader );
//
//	// Pipeline configuration
//	_pipeline->editDataStructure()->setMaxDepth( 5 );
//}
//
///******************************************************************************
// * Draw function called of frame
// ******************************************************************************/
//void SampleCore::draw()
//{
//	CUDAPM_START_FRAME;
//	CUDAPM_START_EVENT( frame );
//	CUDAPM_START_EVENT( app_init_frame );
//
//	glMatrixMode( GL_MODELVIEW );
//	
//	glBindFramebuffer( GL_FRAMEBUFFER, _frameBuffer );
//	if ( _displayOctree )
//	{
//		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
//
//		// Display the GigaVoxels N3-tree space partitioning structure
//		glEnable( GL_DEPTH_TEST );
//		glPushMatrix();
//		glTranslatef( -0.5f, -0.5f, -0.5f );
//		_pipeline->editDataStructure()->render();
//		glPopMatrix();
//		glDisable( GL_DEPTH_TEST );
//
//		// Clear the depth PBO (pixel buffer object) by reading from the previously cleared FBO (frame buffer object)
//		glBindBuffer( GL_PIXEL_PACK_BUFFER, _depthBuffer );
//		glReadPixels( 0, 0, _width, _height, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, 0 );
//		glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
//		GV_CHECK_GL_ERROR();
//	}
//	else
//	{
//		glClear( GL_COLOR_BUFFER_BIT );
//	}
//	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//
//	// extract view transformations
//	float4x4 viewMatrix;
//	float4x4 projectionMatrix;
//	glGetFloatv( GL_MODELVIEW_MATRIX, viewMatrix._array );
//	glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix._array );
//
//	// extract viewport
//	GLint params[4];
//	glGetIntegerv( GL_VIEWPORT, params );
//	int4 viewport = make_int4( params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] );
//
//	// render the scene into textures
//	CUDAPM_STOP_EVENT( app_init_frame );
//
//	// Build the world transformation matrix
//	float4x4 modelMatrix;
//	glPushMatrix();
//	glLoadIdentity();
//	glTranslatef( -0.5f, -0.5f, -0.5f );
//	glGetFloatv( GL_MODELVIEW_MATRIX, modelMatrix._array );
//	glPopMatrix();
//
//	// Render
//	_pipeline->editRenderer()->render( modelMatrix, viewMatrix, projectionMatrix, viewport );
//
//	// Render the result to the screen
//	glMatrixMode( GL_MODELVIEW );
//	glPushMatrix();
//	glLoadIdentity();
//
//	glMatrixMode( GL_PROJECTION );
//	glPushMatrix();
//	glLoadIdentity();
//
//	glDisable( GL_DEPTH_TEST );
//	glEnable( GL_TEXTURE_RECTANGLE );
//	glActiveTexture( GL_TEXTURE0 );
//	glBindTexture( GL_TEXTURE_RECTANGLE, _colorTex );
//
//	// Draw a full screen quad
//	GLint sMin = 0;
//	GLint tMin = 0;
//	GLint sMax = _width;
//	GLint tMax = _height;
//	glBegin( GL_QUADS );
//	glColor3f( 1.0f, 1.0f, 1.0f );
//	glTexCoord2i( sMin, tMin ); glVertex2i( -1, -1 );
//	glTexCoord2i( sMax, tMin ); glVertex2i(  1, -1 );
//	glTexCoord2i( sMax, tMax ); glVertex2i(  1,  1 );
//	glTexCoord2i( sMin, tMax ); glVertex2i( -1,  1 );
//	glEnd();
//
//	glActiveTexture( GL_TEXTURE0 );
//	glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
//	glDisable( GL_TEXTURE_RECTANGLE );
//	
//	glPopMatrix();
//	glMatrixMode( GL_MODELVIEW );
//	glPopMatrix();
//
//	// TEST - optimization due to early unmap() graphics resource from GigaVoxels
//	//_pipeline->editRenderer()->doPostRender();
//	
//	// Update GigaVoxels info
//	_pipeline->editRenderer()->nextFrame();
//
//	CUDAPM_STOP_EVENT( frame );
//	CUDAPM_STOP_FRAME;
//
//	// Display the GigaVoxels performance monitor (if it has been activated during GigaVoxels compilation)
//	if ( _displayPerfmon )
//	{
//		GvPerfMon::CUDAPerfMon::get().displayFrameGL( _displayPerfmon - 1 );
//	}
//}
//
///******************************************************************************
// * Resize the frame
// *
// * @param width the new width
// * @param height the new height
// ******************************************************************************/
//void SampleCore::resize( int width, int height )
//{
//	_width = width;
//	_height = height;
//
//	// Reset default active frame region for rendering
//	_pipeline->editRenderer()->setProjectedBBox( make_uint4( 0, 0, _width, _height ) );
//
//	// Re-init Perfmon subsystem
//	CUDAPM_RESIZE( make_uint2( _width, _height ) );
//
//	// Create frame-dependent objects
//	
//	// Disconnect all registered graphics resources
//	_pipeline->editRenderer()->resetGraphicsResources();
//	
//	// ...
//	if ( _depthBuffer )
//	{
//		glDeleteBuffers( 1, &_depthBuffer );
//	}
//
//	if ( _colorTex )
//	{
//		glDeleteTextures( 1, &_colorTex );
//	}
//	if ( _depthTex )
//	{
//		glDeleteTextures( 1, &_depthTex );
//	}
//
//	if ( _frameBuffer )
//	{
//		glDeleteFramebuffers( 1, &_frameBuffer );
//	}
//
//	glGenTextures( 1, &_colorTex );
//	glBindTexture( GL_TEXTURE_RECTANGLE, _colorTex );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//	glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
//	glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
//	GV_CHECK_GL_ERROR();
//
//	glGenBuffers( 1, &_depthBuffer );
//	glBindBuffer( GL_PIXEL_PACK_BUFFER, _depthBuffer );
//	glBufferData( GL_PIXEL_PACK_BUFFER, width * height * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW );
//	glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
//	GV_CHECK_GL_ERROR();
//
//	glGenTextures( 1, &_depthTex );
//	glBindTexture( GL_TEXTURE_RECTANGLE, _depthTex );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//	glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_DEPTH24_STENCIL8_EXT, width, height, 0, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, NULL );
//	glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
//	GV_CHECK_GL_ERROR();
//
//	glGenFramebuffers( 1, &_frameBuffer );
//	glBindFramebuffer( GL_FRAMEBUFFER, _frameBuffer );
//	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, _colorTex, 0 );
//	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, _depthTex, 0 );
//	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_RECTANGLE, _depthTex, 0 );
//	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//	GV_CHECK_GL_ERROR();
//
//	// Create CUDA resources from OpenGL objects
//	if ( _displayOctree )
//	{
//		_pipeline->editRenderer()->connect( GsGraphicsInteroperabiltyHandler::eColorReadWriteSlot, _colorTex, GL_TEXTURE_RECTANGLE );
//		_pipeline->editRenderer()->connect( GsGraphicsInteroperabiltyHandler::eDepthReadSlot, _depthBuffer );
//	}
//	else
//	{
//		_pipeline->editRenderer()->connect( GsGraphicsInteroperabiltyHandler::eColorWriteSlot, _colorTex, GL_TEXTURE_RECTANGLE );
//	}
//}
//
///******************************************************************************
// * Clear the GigaVoxels cache
// ******************************************************************************/
//void SampleCore::clearCache()
//{
//	_pipeline->editRenderer()->clearCache();
//}
//
///******************************************************************************
// * Toggle the display of the N-tree (octree) of the data structure
// ******************************************************************************/
//void SampleCore::toggleDisplayOctree()
//{
//	_displayOctree = !_displayOctree;
//
//	// Disconnect all registered graphics resources
//	_pipeline->editRenderer()->resetGraphicsResources();
//
//	if ( _displayOctree )
//	{
//		_pipeline->editRenderer()->connect( GsGraphicsInteroperabiltyHandler::eColorReadWriteSlot, _colorTex, GL_TEXTURE_RECTANGLE );
//		_pipeline->editRenderer()->connect( GsGraphicsInteroperabiltyHandler::eDepthReadSlot, _depthBuffer );
//	}
//	else
//	{
//		_pipeline->editRenderer()->connect( GsGraphicsInteroperabiltyHandler::eColorWriteSlot, _colorTex, GL_TEXTURE_RECTANGLE );
//	}
//}
//
///******************************************************************************
// * Toggle the GigaVoxels dynamic update mode
// ******************************************************************************/
//void SampleCore::toggleDynamicUpdate()
//{
//	_pipeline->editRenderer()->dynamicUpdateState() = !_pipeline->editRenderer()->dynamicUpdateState();
//}
//
///******************************************************************************
// * Toggle the display of the performance monitor utility if
// * GigaVoxels has been compiled with the Performance Monitor option
// *
// * @param mode The performance monitor mode (1 for CPU, 2 for DEVICE)
// ******************************************************************************/
//void SampleCore::togglePerfmonDisplay( uint mode )
//{
//	if ( _displayPerfmon )
//	{
//		_displayPerfmon = 0;
//	}
//	else
//	{
//		_displayPerfmon = mode;
//	}
//}
//
///******************************************************************************
// * Increment the max resolution of the data structure
// ******************************************************************************/
//void SampleCore::incMaxVolTreeDepth()
//{
//	if ( _maxVolTreeDepth < 32 )
//	{
//		_maxVolTreeDepth++;
//	}
//
//	_pipeline->editDataStructure()->setMaxDepth( _maxVolTreeDepth );
//}
//
///******************************************************************************
// * Decrement the max resolution of the data structure
// ******************************************************************************/
//void SampleCore::decMaxVolTreeDepth()
//{
//	if ( _maxVolTreeDepth > 0 )
//	{
//		_maxVolTreeDepth--;
//	}
//
//	_pipeline->editDataStructure()->setMaxDepth( _maxVolTreeDepth );
//}
//
///******************************************************************************
// * Specify color to clear the color buffer
// *
// * @param pRed red component
// * @param pGreen green component
// * @param pBlue blue component
// * @param pAlpha alpha component
// ******************************************************************************/
//void SampleCore::setClearColor( unsigned char pRed, unsigned char pGreen, unsigned char pBlue, unsigned char pAlpha )
//{
//	_pipeline->editRenderer()->setClearColor( make_uchar4( pRed, pGreen, pBlue, pAlpha ) );
//}
//
///******************************************************************************
// * Set the light position
// *
// * @param pX the X light position
// * @param pY the Y light position
// * @param pZ the Z light position
// ******************************************************************************/
//void SampleCore::setLightPosition( float pX, float pY, float pZ )
//{
//	// Update DEVICE memory with "light position"
//	float3  lightPosition = make_float3( pX, pY, pZ );
//	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( cLightPosition, &lightPosition, sizeof( lightPosition ), 0, cudaMemcpyHostToDevice ) );
//}
