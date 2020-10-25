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

#include "SampleCore.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GigaVoxels
#include <GvCore/GsVector.h>
#include <GvStructure/GsVolumeTree.h>
#include <GvStructure/GsDataProductionManager.h>
#include <GvRendering/GsRendererCUDA.h>
#include <GvUtils/GsSimplePipeline.h>
#include <GvUtils/GsSimpleHostProducer.h>
#include <GvUtils/GsSimpleHostShader.h>
#include <GvUtils/GsSimplePriorityPoliciesManagerKernel.h>
#include <GvUtils/GsCommonGraphicsPass.h>
#include <GvCore/GsError.h>
#include <GvPerfMon/GsPerformanceMonitor.h>
#include <GsGraphics/GsShaderProgram.h>
//#include <GvUtils/GsEnvironment.h>
#include <GsSpaceCore/GsEnvironment.h>

// Project
#include "ProducerKernel.h"
#include "ShaderKernel.h"

//// Qt
//#include <QCoreApplication>
//#include <QString>
//#include <QDir>
//#include <QFileInfo>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GvRendering;
using namespace GvUtils;
using namespace GsGraphics;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// Defines the size allowed for each type of pool
#define NODEPOOL_MEMSIZE	( 8U * 1024U * 1024U )		// 8 Mo
#define BRICKPOOL_MEMSIZE	( 256U * 1024U * 1024U )	// 256 Mo

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Constructor
 ******************************************************************************/
SampleCore::SampleCore()
:	_pipeline( nullptr )
,	_renderer( nullptr )
,	_graphicsEnvironment( nullptr )
,	_displayOctree( false )
,	_displayPerfmon( 0 )
,	_maxVolTreeDepth( 6 )
,	_depthBuffer( 0 )
,	_colorTex( 0 )
,	_depthTex( 0 )
,	_frameBuffer( 0 )
,	_width( 0 )
,	_height( 0 )
,	_shaderProgram( NULL )
,	_fullscreenQuadVAO( 0 )
,	_fullscreenQuadVertexBuffer( 0 )
,	_textureSamplerUniformLocation( 0 )
,	m_material_opacity( 512.f )
{
	// Initialize matrices
	memset( _modelMatrix._array, 0, 16 * sizeof( float ) );
	memset( _modelViewMatrix._array, 0, 16 * sizeof( float ) );
	memset( _projectionMatrix._array, 0, 16 * sizeof( float ) );
	_viewport = make_int4( 0 );
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
SampleCore::~SampleCore()
{
	delete _pipeline;
	delete _graphicsEnvironment;
}

/******************************************************************************
 * Initialize the GigaVoxels pipeline
 ******************************************************************************/
void SampleCore::init()
{
	CUDAPM_INIT();

	// Initialize CUDA with OpenGL Interoperability
	//cudaGLSetGLDevice( gpuGetMaxGflopsDeviceId() );	// to do : deprecated, use cudaSetDevice()
	//GV_CHECK_CUDA_ERROR( "cudaGLSetGLDevice" );
	cudaSetDevice( gpuGetMaxGflopsDeviceId() );
	GV_CHECK_CUDA_ERROR( "cudaSetDevice" );

	// Pipeline creation
	_pipeline = new PipelineType();
	ShaderType* shader = new ShaderType();

	// Pipeline initialization
	_pipeline->initialize( NODEPOOL_MEMSIZE, BRICKPOOL_MEMSIZE, shader );

	// Producer initialization
	_producer = new ProducerType();
	assert( _producer != NULL );
	_pipeline->addProducer( _producer );

	// Renderer initialization
	_renderer = new RendererType( _pipeline->editDataStructure(), _pipeline->editCache() );
	assert( _renderer != NULL );
	_pipeline->addRenderer( _renderer );

	// Pipeline configuration
	_pipeline->editDataStructure()->setMaxDepth( _maxVolTreeDepth );
	
	// Graphics environment creation
	_graphicsEnvironment = new GsCommonGraphicsPass();

	// Build default model transformation matrix
	// - translation to center GigaSpace BBox (originally in [0;1]x[0;1]x[0;1])
	// scale
	_modelMatrix._array[ 0 ] = 1.0f;
	_modelMatrix._array[ 5 ] = 1.0f;
	_modelMatrix._array[ 10 ] = 1.0f;
	_modelMatrix._array[ 15 ] = 1.0f;
	// translation
	_modelMatrix._array[ 12 ] = -0.5f;
	_modelMatrix._array[ 13 ] = -0.5f;
	_modelMatrix._array[ 14 ] = -0.5f;

	// Create and link a GLSL shader program
	//QString shaderRepository = GsEnvironment::getDataDir( GsEnvironment::eShadersDir ).c_str();
	//std::string shaderRepository = GsEnvironment::getDataDir( GsEnvironment::eShadersDir ).c_str();
	std::string shaderRepository = GsCore::GsEnvironment::getDataDir( GsCore::GsEnvironment::eShadersDir ).c_str();
	//shaderRepository += QDir::separator();
	shaderRepository += std::string( "/" );
	//shaderRepository += QString( "SimpleSphere" );
	shaderRepository += std::string( "SimpleSphere" );
	shaderRepository += std::string( "/" );
	// Initialize points shader program
	//QString vertexShaderFilename = shaderRepository + QString( "fullscreenQuad_vert.glsl" );
	std::string vertexShaderFilename = shaderRepository + std::string( "fullscreenQuad_vert.glsl" );
	//QString fragmentShaderFilename = shaderRepository + QString( "fullscreenQuad_frag.glsl" );
	std::string fragmentShaderFilename = shaderRepository + std::string( "fullscreenQuad_frag.glsl" );
	// Initialize shader program
	_shaderProgram = new GsShaderProgram();
	//bool statusOK = _shaderProgram->addShader( GsShaderProgram::eVertexShader, vertexShaderFilename.toStdString() );
	bool statusOK = _shaderProgram->addShader( GsShaderProgram::eVertexShader, vertexShaderFilename );
	assert( statusOK );
	if ( ! statusOK )
	{
		// TO DO
		// - handle error
	}
	//statusOK = _shaderProgram->addShader( GsShaderProgram::eFragmentShader, fragmentShaderFilename.toStdString() );
	statusOK = _shaderProgram->addShader( GsShaderProgram::eFragmentShader, fragmentShaderFilename );
	assert( statusOK );
	if ( ! statusOK )
	{
		// TO DO
		// - handle error
	}
	statusOK = _shaderProgram->link();
	assert( statusOK );
	if ( ! statusOK )
	{
		// TO DO
		// - handle error
	}

	// After linking has occurred, the command glGetUniformLocation can be used to obtain the location of a uniform variable
	// - beware, after "shader link", this location my changed
	_textureSamplerUniformLocation = glGetUniformLocation( _shaderProgram->_program, "uTextureSampler" );
	assert( _textureSamplerUniformLocation != -1 );
	if ( _textureSamplerUniformLocation == -1 )
	{
		// TO DO
		// - handle error
	}
	// Avoid redundant call by initializing data
	// - by default, after been linked, a program has already its uniforms set to 0
	_shaderProgram->use();
	glUniform1i( _textureSamplerUniformLocation, 0 );
	GsShaderProgram::unuse();

	//// Vertex position buffer initialization
	//glGenBuffers( 1, &_fullscreenQuadVertexBuffer );
	//glBindBuffer( GL_ARRAY_BUFFER, _fullscreenQuadVertexBuffer );
	//GLsizeiptr fullscreenQuadVertexBufferSize = sizeof( GLfloat ) * 4/*nbVertices*/ * 2/*nb components per vertex*/;
	//float2 fullscreenQuadVertices[ 4/*nbVertices*/ ] =
	//{
	//	{ -1.0, -1.0 },
	//	{ 1.0, -1.0 },
	//	{ 1.0, 1.0 },
	//	{ -1.0, 1.0 }
	//};
	//glBufferData( GL_ARRAY_BUFFER, fullscreenQuadVertexBufferSize, &fullscreenQuadVertices[ 0 ], GL_STATIC_DRAW );
	//glBindBuffer( GL_ARRAY_BUFFER, 0 );

	// Vertex array object initialization
	glGenVertexArrays( 1, &_fullscreenQuadVAO );
	//glBindVertexArray( _fullscreenQuadVAO );
	//glEnableVertexAttribArray( 0 );	// vertex position
	//glBindBuffer( GL_ARRAY_BUFFER, _fullscreenQuadVertexBuffer );
	//glVertexAttribPointer( 0/*attribute index*/, 2/*nb components per vertex*/, GL_FLOAT/*type*/, GL_FALSE/*un-normalized*/, 0/*memory stride*/, static_cast< GLubyte* >( NULL )/*byte offset from buffer*/ );
	//glBindBuffer( GL_ARRAY_BUFFER, 0 );
	//glBindVertexArray( 0 );

	// Avoid redundant call by initializing data
	glActiveTexture( GL_TEXTURE0 );

	// Material
	gs_set_material_opacity( 512.f );
}

/******************************************************************************
 * Draw function called of frame
 ******************************************************************************/
void SampleCore::draw()
{
	CUDAPM_START_FRAME;
	CUDAPM_START_EVENT( frame );
	CUDAPM_START_EVENT( app_init_frame );

	//glMatrixMode( GL_MODELVIEW );	// already done by QGLViewer
	
	glBindFramebuffer( GL_FRAMEBUFFER, _frameBuffer );
	if ( _displayOctree )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

		// Display the GigaVoxels N3-tree space partitioning structure
		glEnable( GL_DEPTH_TEST );
//		glPushMatrix();
//		glTranslatef( -0.5f, -0.5f, -0.5f );
		//_pipeline->editDataStructure()->render();
		_pipeline->editDataStructure()->render( _modelMatrix, _modelViewMatrix, _projectionMatrix, _viewport );
//		glPopMatrix();
		glDisable( GL_DEPTH_TEST );

		// Clear the depth PBO (pixel buffer object) by reading from the previously cleared FBO (frame buffer object)
		glBindBuffer( GL_PIXEL_PACK_BUFFER, _depthBuffer );
		glReadPixels( 0, 0, _width, _height, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, 0 );
		//glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );	// deprecated
		GV_CHECK_GL_ERROR();
	}
	else
	{
		glClear( GL_COLOR_BUFFER_BIT );
	}
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	//----------------------------------------------------------------------------------------------------
	// TO DO
	// - optimization : don't use glGetFloatv() it's slow, store matrices with glm (check NSight)
	//
	// - QGLViewer has already theses matrices in preDraw()
	//----------------------------------------------------------------------------------------------------

	// render the scene into textures
	CUDAPM_STOP_EVENT( app_init_frame );

	// Render
	_pipeline->execute( _modelMatrix, _modelViewMatrix, _projectionMatrix, _viewport );

#if 1
	// Draw fullscreen textured quad
	_shaderProgram->use();
	//glUniform1i( _textureSamplerUniformLocation, 0 );	// avoid redundant call
	//glActiveTexture( GL_TEXTURE0 );	// avoid redundant call
	//glBindTexture( GL_TEXTURE_RECTANGLE_EXT, _colorTex );
	//glBindTexture( GL_TEXTURE_RECTANGLE, _colorTex );
	//glBindTexture( GL_TEXTURE_2D, _colorTex );

	const GLint unit = 0;
	glBindTextureUnit( unit, _colorTex );
	glUniform1i( _textureSamplerUniformLocation, unit );

	glBindVertexArray( _fullscreenQuadVAO );
	//glDrawArrays( GL_QUADS, 0, 4 );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glBindVertexArray( 0 );
	//glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );	// deprecated
	GsShaderProgram::unuse();
#endif

	// TEST - optimization due to early unmap() graphics resource from GigaVoxels
	///*_pipeline->editRenderer()*/_renderer->doPostRender();
	
	// Update GigaVoxels info
	/*_pipeline->editRenderer()*/_renderer->nextFrame();
	
	CUDAPM_STOP_EVENT( frame );
	CUDAPM_STOP_FRAME;

	// Display the GigaVoxels performance monitor (if it has been activated during GigaVoxels compilation)
	if ( _displayPerfmon )
	{
		GvPerfMon::CUDAPerfMon::get().displayFrameGL( _displayPerfmon - 1 );
	}
}

/******************************************************************************
 * Resize the frame
 *
 * @param width the new width
 * @param height the new height
 ******************************************************************************/
void SampleCore::resize( int pWidth, int pHeight )
{
	// LOG
	//
	// @todo : check and avoid 0 values, replace by 1 and warn user
	if ( pWidth == 0 )
	{
		// TO DO
		// ...
	}
	if ( pHeight == 0 )
	{
		// TO DO
		// ...
	}

	_width = pWidth;
	_height = pHeight;

	// Update internal viewport
	_viewport = make_int4( 0, 0, pWidth, pHeight );

	// Reset default active frame region for rendering
	/*_pipeline->editRenderer()*/_renderer->setProjectedBBox( make_uint4( 0, 0, pWidth, pHeight ) );
	// Re-init Perfmon subsystem
	CUDAPM_RESIZE( make_uint2( pWidth, pHeight ) );

	// Update graphics environment
	_graphicsEnvironment->setBufferSize( pWidth, pHeight );

	// Reset graphics resources
	resetGraphicsresources();
}

/******************************************************************************
 * Reset graphics resources
 ******************************************************************************/
void SampleCore::resetGraphicsresources()
{
	// [ 1 ] - Reset graphics resources

	// Disconnect all registered graphics resources
	/*_pipeline->editRenderer()*/_renderer->resetGraphicsResources();
	
	// Update graphics environment
	_graphicsEnvironment->reset();
	
	// Update internal variables
	_depthBuffer = _graphicsEnvironment->getDepthBuffer();
	_colorTex = _graphicsEnvironment->getColorTexture();
	//_colorRenderBuffer = _graphicsEnvironment->getColorRenderBuffer();
	_depthTex = _graphicsEnvironment->getDepthTexture();
	_frameBuffer = _graphicsEnvironment->getFrameBuffer();
	
	// [ 2 ] - Connect graphics resources

	// Create CUDA resources from OpenGL objects
	if ( _displayOctree )
	{
		/*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eColorReadWriteSlot, _colorTex, /*GL_TEXTURE_2D*/GL_TEXTURE_RECTANGLE/*_EXT*/ );
		/*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eDepthReadSlot, _depthBuffer );
	}
	else
	{
		/*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eColorWriteSlot, _colorTex, GL_TEXTURE_RECTANGLE/*_EXT*/ );
		///*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eColorWriteSlot, _colorTex, GL_TEXTURE_2D );
	}
}

/******************************************************************************
 * Clear the GigaVoxels cache
 ******************************************************************************/
void SampleCore::clearCache()
{
	_pipeline->clear();
}

/******************************************************************************
 * Toggle the display of the N-tree (octree) of the data structure
 ******************************************************************************/
void SampleCore::toggleDisplayOctree()
{
	_displayOctree = !_displayOctree;

	// Disconnect all registered graphics resources
	/*_pipeline->editRenderer()*/_renderer->resetGraphicsResources();

	if ( _displayOctree )
	{
		/*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eColorReadWriteSlot, _colorTex, /*GL_TEXTURE_2D*/GL_TEXTURE_RECTANGLE/*_EXT*/ );
		/*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eDepthReadSlot, _depthBuffer );
	}
	else
	{
		/*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eColorWriteSlot, _colorTex, GL_TEXTURE_RECTANGLE/*_EXT*/ );
		///*_pipeline->editRenderer()*/_renderer->connect( GsGraphicsInteroperabiltyHandler::eColorWriteSlot, _colorTex, GL_TEXTURE_2D );
	}
}

/******************************************************************************
 * Toggle the GigaVoxels dynamic update mode
 ******************************************************************************/
void SampleCore::toggleDynamicUpdate()
{
	const bool status = _pipeline->hasDynamicUpdate();
	_pipeline->setDynamicUpdate( ! status );
}

/******************************************************************************
 * Toggle the display of the performance monitor utility if
 * GigaVoxels has been compiled with the Performance Monitor option
 *
 * @param mode The performance monitor mode (1 for CPU, 2 for DEVICE)
 ******************************************************************************/
void SampleCore::togglePerfmonDisplay( uint mode )
{
	if ( _displayPerfmon )
	{
		_displayPerfmon = 0;
	}
	else
	{
		_displayPerfmon = mode;
	}
}

/******************************************************************************
 * Increment the max resolution of the data structure
 ******************************************************************************/
void SampleCore::incMaxVolTreeDepth()
{
	if ( _maxVolTreeDepth < 32 )
	{
		_maxVolTreeDepth++;
	}

	_pipeline->editDataStructure()->setMaxDepth( _maxVolTreeDepth );
}

/******************************************************************************
 * Decrement the max resolution of the data structure
 ******************************************************************************/
void SampleCore::decMaxVolTreeDepth()
{
	if ( _maxVolTreeDepth > 0 )
	{
		_maxVolTreeDepth--;
	}

	_pipeline->editDataStructure()->setMaxDepth( _maxVolTreeDepth );
}

/******************************************************************************
 * Specify color to clear the color buffer
 *
 * @param pRed red component
 * @param pGreen green component
 * @param pBlue blue component
 * @param pAlpha alpha component
 ******************************************************************************/
void SampleCore::setClearColor( unsigned char pRed, unsigned char pGreen, unsigned char pBlue, unsigned char pAlpha )
{
	/*_pipeline->editRenderer()*/_renderer->setClearColor( make_uchar4( pRed, pGreen, pBlue, pAlpha ) );
}

/******************************************************************************
 * Set the light position
 *
 * @param pX the X light position
 * @param pY the Y light position
 * @param pZ the Z light position
 ******************************************************************************/
void SampleCore::setLightPosition( float pX, float pY, float pZ )
{
	// Update DEVICE memory with "light position"
	float3  lightPosition = make_float3( pX, pY, pZ );
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( cLightPosition, &lightPosition, sizeof( lightPosition ), 0, cudaMemcpyHostToDevice ) );
}

///******************************************************************************
// * GigaSpace pipeline
// ******************************************************************************/
//const PipelineType* const SampleCore::getPipeline() const
//{
//	return _pipeline;
//}
//
///******************************************************************************
// * GigaSpace pipeline
// ******************************************************************************/
//PipelineType* const SampleCore::editPipeline()
//{
//	return _pipeline;
//}

/******************************************************************************
 * Get the max depth of the volume tree
 *
 * @return max depth
 ******************************************************************************/
uint SampleCore::get_dataStructure_MaxDepth() const
{
	return _pipeline->getDataStructure()->getMaxDepth();
}

/******************************************************************************
 * Set the max depth of the volume tree
 *
 * @param maxDepth Max depth
 ******************************************************************************/
void SampleCore::set_dataStructure_MaxDepth( const uint pValue )
{
	_pipeline->editDataStructure()->setMaxDepth( pValue );
}

/******************************************************************************
 * Get the max depth of the volume tree
 *
 * @return max depth
 ******************************************************************************/
uint SampleCore::get_dataStructure_DisplayMode() const
{
	return _displayOctree;
}

/******************************************************************************
 * Set the max depth of the volume tree
 *
 * @param maxDepth Max depth
 ******************************************************************************/
void SampleCore::set_dataStructure_DisplayMode( const bool pFlag )
{
	if ( _displayOctree != pFlag )
	{
		toggleDisplayOctree();
	}
}

/******************************************************************************
 * Render the data structure
 ******************************************************************************/
void SampleCore::gs_dataStructure_render()
{
	_pipeline->editDataStructure()->render();

}

/******************************************************************************
 * Material
 ******************************************************************************/
float SampleCore::gs_get_material_opacity() const
{
	return m_material_opacity;
}

/******************************************************************************
 * Material
 ******************************************************************************/
void SampleCore::gs_set_material_opacity( const float pValue )
{
	m_material_opacity = pValue;
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( cOpacityLength, &m_material_opacity, sizeof( m_material_opacity ), 0, cudaMemcpyHostToDevice ) );
}

/******************************************************************************
 * ...
 ******************************************************************************/
bool SampleCore::gs_pipeline_get_dynamic_update() const
{
	return _pipeline->hasDynamicUpdate();
}

/******************************************************************************
 * ...
 ******************************************************************************/
void SampleCore::gs_pipeline_set_dynamic_update( const bool pFlag )
{
	_pipeline->setDynamicUpdate( pFlag );
}

/******************************************************************************
 * ...
 ******************************************************************************/
bool SampleCore::gs_renderer_hasPriorityOnBricks() const
{
	return _renderer->hasPriorityOnBricks();
}

/******************************************************************************
 * ...
 ******************************************************************************/
void SampleCore::gs_renderer_setPriorityOnBricks( const bool pFlag )
{
	_renderer->setPriorityOnBricks( pFlag );
}
