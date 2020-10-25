/*
 * GigaVoxels - GigaSpace
 *
 * Website: http://gigavoxels.inrialpes.fr/
 *
 * Contributors: GigaVoxels Team
 *
 * Copyright (C) 2007-2015 INRIA - LJK (CNRS - Grenoble University), All rights reserved.
 */

/** 
 * @version 1.0
 */

#include "PtPipeline.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

//// GigaVoxels
//#include <GvCore/GsVector.h>
//#include <GvStructure/GsVolumeTree.h>
//#include <GvStructure/GsDataProductionManager.h>
//#include <GvUtils/GsSimplePipeline.h>
//#include <GvUtils/GsSimpleHostProducer.h>
//#include <GvUtils/GsSimplePriorityPoliciesManagerKernel.h>
//#include <GvCore/GsError.h>
//#include <GvPerfMon/GsPerformanceMonitor.h>
//#include <GvStructure/GsNode.h>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
GsvPipeline::GsvPipeline()
:/*	_pipeline( nullptr )
,*//*	_renderer( nullptr )
,*/	mColorTex( 0 )
,	mDepthTex( 0 )
,	mFrameBuffer( 0 )
,	mColorBuffer( 0 )
,	mDepthBuffer( 0 )
,	mColorResource( 0 )
,	mDepthResource( 0 )
,	mDisplayOctree( false )
,	mDisplayPerfmon( 0 )
,	mMaxVolTreeDepth( 6 )
{
	// Initialize matrices
	//memset( _modelMatrix._array, 0, 16 * sizeof( float ) );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsvPipeline::~GsvPipeline()
{
/	delete _pipeline;
}

/******************************************************************************
 * Initialize the GigaVoxels pipeline
 ******************************************************************************/
void GsvPipeline::init()
{
//	CUDAPM_INIT();

	//// Initialize CUDA with OpenGL Interoperability
	////cudaGLSetGLDevice( gpuGetMaxGflopsDeviceId() );	// to do : deprecated, use cudaSetDevice()
	////GV_CHECK_CUDA_ERROR( "cudaGLSetGLDevice" );
	//cudaSetDevice( gpuGetMaxGflopsDeviceId() );
	//GV_CHECK_CUDA_ERROR( "cudaSetDevice" );

	//// Compute the size of one element in the cache for nodes and bricks
	//size_t nodeElemSize = NodeRes::numElements * sizeof( GvStructure::GsNode );
	////size_t brickElemSize = RealBrickRes::numElements * GvCore::DataTotalChannelSize< DataType >::value;

	//// Compute how many we can fit into the given memory size
	//size_t nodePoolNumElems = NODEPOOL_MEMSIZE / nodeElemSize;
	////size_t brickPoolNumElems = BRICKPOOL_MEMSIZE / brickElemSize;

	//// Compute the resolution of the pools
	//uint3 nodePoolRes = make_uint3( (uint)floorf( powf( (float)nodePoolNumElems, 1.0f / 3.0f ) ) ) * NodeRes::get();
	////uint3 brickPoolRes = make_uint3( (uint)floorf( powf( (float)brickPoolNumElems, 1.0f / 3.0f ) ) ) * RealBrickRes::get();

	////std::cout << "nodePoolRes: " << nodePoolRes << std::endl;
	////std::cout << "brickPoolRes: " << brickPoolRes << std::endl;

	//// Build default model transformation matrix
	//// - translation to center GigaSpace BBox (originally in [0;1]x[0;1]x[0;1])
	//// scale
	//_modelMatrix._array[ 0 ] = 1.0f;
	//_modelMatrix._array[ 5 ] = 1.0f;
	//_modelMatrix._array[ 10 ] = 1.0f;
	//_modelMatrix._array[ 15 ] = 1.0f;
	//// translation
	//_modelMatrix._array[ 12 ] = -0.5f;
	//_modelMatrix._array[ 13 ] = -0.5f;
	//_modelMatrix._array[ 14 ] = -0.5f;

	//// Pipeline creation
	//_pipeline = new PipelineType();

	//// Shader creation
	//ShaderType* shader = new ShaderType();

	//// Pipeline initialization
	//const bool useGraphicsLibraryInteroperability = true;
	//_pipeline->initialize( NODEPOOL_MEMSIZE, BRICKPOOL_MEMSIZE, shader, useGraphicsLibraryInteroperability );

	//// Producer initialization
	//_producer = new ProducerType();
	//assert( _producer != nullptr );
	//_pipeline->addProducer( _producer );

	//// Renderer initialization
	//_renderer = new RendererType( _pipeline->editDataStructure(), _pipeline->editCache() );
	//assert( _renderer != nullptr );
	//_pipeline->addRenderer( _renderer );

	//// Pipeline configuration
	//_pipeline->editDataStructure()->setMaxDepth( mMaxVolTreeDepth );
}

/******************************************************************************
 * Draw function called of frame
 ******************************************************************************/
//void GsvPipeline::draw()
void GsvPipeline::render( const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, const glm::vec4& pViewport )
{
	/*CUDAPM_START_FRAME;
	CUDAPM_START_EVENT( frame );
	CUDAPM_START_EVENT( app_init_frame );*/

	glClearColor( 0.0f, 0.1f, 0.3f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );

	//glMatrixMode( GL_MODELVIEW);

	// Display the data structure (space partitioning)
	//glPushMatrix();
	//glTranslatef( -0.5f, -0.5f, -0.5f );
	if ( mDisplayOctree )
	{
//		_pipeline->editDataStructure()->render( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );
	}
	//glPopMatrix();

	// extract view transformations
	//float4x4 viewMatrix;
	//float4x4 projectionMatrix;
	// FIXME
	//glPushMatrix();
	//glTranslatef( -0.5f, -0.5f, -0.5f );
	// FIXME
	//glGetFloatv( GL_MODELVIEW_MATRIX, viewMatrix._array );
	//glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix._array );
	// FIXME
	//glPopMatrix();

	// build and extract tree transformations
	//float4x4 modelMatrix;

	//glPushMatrix();
	///glLoadIdentity();
	//glTranslatef(-0.5f, -0.5f, -0.5f);
	//glGetFloatv( GL_MODELVIEW_MATRIX, modelMatrix._array );
	//glPopMatrix();

	//// extract viewport
	//GLint params[4];
	//glGetIntegerv( GL_VIEWPORT, params );
	//int4 viewport = make_int4( params[0], params[1], params[2], params[3] );

//	CUDAPM_STOP_EVENT( app_init_frame );

	// Render the scene into textures
//	_pipeline->execute( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );

//	/*_pipeline->editRenderer()*/_renderer->nextFrame();

	//CUDAPM_STOP_EVENT( frame );
	//CUDAPM_STOP_FRAME;

	// Display the performance monitor
	if ( mDisplayPerfmon )
	{
//		GvPerfMon::CUDAPerfMon::get().displayFrameGL( mDisplayPerfmon - 1 );
	}
}

/******************************************************************************
 * Resize the frame
 *
 * @param width the new width
 * @param height the new height
 ******************************************************************************/
void GsvPipeline::resize( int width, int height )
{
	mWidth = width;
	mHeight = height;

	// Re-init Perfmon subsystem
//	CUDAPM_RESIZE( make_uint2( mWidth, mHeight ) );

	/*uchar *timersMask = GvPerfMon::CUDAPerfMon::get().getKernelTimerMask();
	cudaMemset(timersMask, 255, mWidth * mHeight);*/

	// Create frame-dependent objects
	if ( mColorResource )
	{
//		GS_CUDA_SAFE_CALL( cudaGraphicsUnregisterResource(mColorResource) );
	}
	if ( mDepthResource )
	{
//		GS_CUDA_SAFE_CALL( cudaGraphicsUnregisterResource(mDepthResource) );
	}

	if ( mColorBuffer )
	{
		glDeleteBuffers( 1, &mColorBuffer );
	}
	if ( mDepthBuffer )
	{
		glDeleteBuffers( 1, &mDepthBuffer );
	}

	if ( mColorTex )
	{
		glDeleteTextures( 1, &mColorTex );
	}
	if ( mDepthTex )
	{
		glDeleteTextures( 1, &mDepthTex );
	}

	if ( mFrameBuffer )
	{
		glDeleteFramebuffers( 1, &mFrameBuffer );
	}

	glGenBuffers( 1, &mColorBuffer );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, mColorBuffer );
	glBufferData( GL_PIXEL_PACK_BUFFER, width * height * sizeof(GLubyte) * 4, nullptr, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
//	GV_CHECK_GL_ERROR();

	glGenTextures( 1, &mColorTex );
	glBindTexture( GL_TEXTURE_RECTANGLE, mColorTex );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );
	glBindTexture( GL_TEXTURE_RECTANGLE, 0  );
//	GV_CHECK_GL_ERROR();

	glGenBuffers( 1, &mDepthBuffer );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, mDepthBuffer );
	glBufferData( GL_PIXEL_PACK_BUFFER, width * height * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
//	GV_CHECK_GL_ERROR();

	glGenTextures( 1, &mDepthTex );
	glBindTexture( GL_TEXTURE_RECTANGLE, mDepthTex );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_DEPTH24_STENCIL8_EXT, width, height, 0, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, nullptr );
	glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
//	GV_CHECK_GL_ERROR();

	glGenFramebuffers( 1, &mFrameBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, mFrameBuffer );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, mColorTex, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, mDepthTex, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_RECTANGLE, mDepthTex, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//	GV_CHECK_GL_ERROR();

	// Create CUDA resources from OpenGL objects
//	GS_CUDA_SAFE_CALL( cudaGraphicsGLRegisterBuffer( &mColorResource, mColorBuffer, cudaGraphicsRegisterFlagsNone ) );
//	GS_CUDA_SAFE_CALL( cudaGraphicsGLRegisterBuffer( &mDepthResource, mDepthBuffer, cudaGraphicsRegisterFlagsNone ) );

	// Pass resources to the renderer
	/*_pipeline->editRenderer()->setColorResource(mColorResource);
	_pipeline->editRenderer()->setDepthResource(mDepthResource);*/
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::clearCache()
{
//	_pipeline->clear();
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::toggleDisplayOctree()
{
	mDisplayOctree = !mDisplayOctree;
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::toggleDynamicUpdate()
{
//	const bool status = _pipeline->hasDynamicUpdate();
//	_pipeline->setDynamicUpdate( ! status );
}

/******************************************************************************
 * ...
 *
 * @param mode ...
 ******************************************************************************/
void GsvPipeline::togglePerfmonDisplay( uint32_t mode )
{
	if ( mDisplayPerfmon )
	{
		mDisplayPerfmon = 0;
	}
	else
	{
		mDisplayPerfmon = mode;
	}
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::incMaxVolTreeDepth()
{
	if ( mMaxVolTreeDepth < 32 )
	{
		mMaxVolTreeDepth++;
	}

//	_pipeline->editDataStructure()->setMaxDepth( mMaxVolTreeDepth );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::decMaxVolTreeDepth()
{
	if ( mMaxVolTreeDepth > 0 )
	{
		mMaxVolTreeDepth--;
	}

//	_pipeline->editDataStructure()->setMaxDepth( mMaxVolTreeDepth );
}

/******************************************************************************
 * Set the light position
 *
 * @param pX the X light position
 * @param pY the Y light position
 * @param pZ the Z light position
 ******************************************************************************/
void GsvPipeline::setLightPosition( float pX, float pY, float pZ )
{
	// Update DEVICE memory with "light position"
	//
//	float3 lightPos = make_float3( pX, pY, pZ );
	//GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( cLightPosition, &lightPos, sizeof( lightPos ), 0, cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * Get the max depth of the volume tree
 *
 * @return max depth
 ******************************************************************************/
uint32_t GsvPipeline::get_dataStructure_MaxDepth() const
{
	return _pipeline->getDataStructure()->getMaxDepth();
}

/******************************************************************************
 * Set the max depth of the volume tree
 *
 * @param maxDepth Max depth
 ******************************************************************************/
void GsvPipeline::set_dataStructure_MaxDepth( const uint32_t pValue )
{
	_pipeline->editDataStructure()->setMaxDepth( pValue );
}

/******************************************************************************
 * Get the max depth of the volume tree
 *
 * @return max depth
 ******************************************************************************/
uint32_t GsvPipeline::get_dataStructure_DisplayMode() const
{
	return mDisplayOctree;
}

/******************************************************************************
 * Set the max depth of the volume tree
 *
 * @param maxDepth Max depth
 ******************************************************************************/
void GsvPipeline::set_dataStructure_DisplayMode( const bool pFlag )
{
	if ( mDisplayOctree != pFlag )
	{
		toggleDisplayOctree();
	}
}

/******************************************************************************
 * Render the data structure
 ******************************************************************************/
void GsvPipeline::gs_dataStructure_render()
{
	_pipeline->editDataStructure()->render();

}

/******************************************************************************
 * Material
 ******************************************************************************/
float GsvPipeline::gs_get_material_opacity() const
{
	return m_material_opacity;
}

/******************************************************************************
 * Material
 ******************************************************************************/
void GsvPipeline::gs_set_material_opacity( const float pValue )
{
	m_material_opacity = pValue;
	//GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( cOpacityLength, &m_material_opacity, sizeof( m_material_opacity ), 0, cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * ...
 ******************************************************************************/
bool GsvPipeline::gs_pipeline_get_dynamic_update() const
{
	return _pipeline->hasDynamicUpdate();
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::gs_pipeline_set_dynamic_update( const bool pFlag )
{
	_pipeline->setDynamicUpdate( pFlag );
}

/******************************************************************************
 * ...
 ******************************************************************************/
bool GsvPipeline::gs_renderer_hasPriorityOnBricks() const
{
	return _renderer->hasPriorityOnBricks();
}

/******************************************************************************
 * ...
 ******************************************************************************/
void GsvPipeline::gs_renderer_setPriorityOnBricks( const bool pFlag )
{
	_renderer->setPriorityOnBricks( pFlag );
}
