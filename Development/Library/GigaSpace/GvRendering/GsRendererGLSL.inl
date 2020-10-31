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
#include "GvUtils/GsShaderManager.h"

//// Qt
//#include <QCoreApplication>
//#include <QString>
//#include <QDir>
//#include <QFileInfo>

/******************************************************************************
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvRendering
{

/******************************************************************************
 * Constructor
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
GsRendererGLSL< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::GsRendererGLSL()
:	GsRenderer< TVolumeTreeType, TVolumeTreeCacheType >()
{
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
GsRendererGLSL< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::~GsRendererGLSL()
{
	finalize();
}

/******************************************************************************
 * Initialize
 *
 * @param pDataStructure data structure
 * @param pDataProductionManager data production manager
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
inline void GsRendererGLSL< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::initialize( TVolumeTreeType* pDataStructure, TVolumeTreeCacheType* pDataProductionManager )
{
	// Call parent class
	GsRenderer< TVolumeTreeType, TVolumeTreeCacheType >::initialize( pDataStructure, pDataProductionManager );

	// Create a buffer object
	glGenBuffers( 1, &_textBuffer );
	glBindBuffer( GL_TEXTURE_BUFFER, _textBuffer );
	// Creates and initializes buffer object's data store 
	glBufferData( GL_TEXTURE_BUFFER, 8192 * sizeof( GLfloat ), NULL, GL_STATIC_DRAW );
	glBindBuffer( GL_TEXTURE_BUFFER, 0 );
	GV_CHECK_GL_ERROR();

	// Create a buffer texture
	glGenTextures( 1, &_textBufferTBO );
	glBindTexture( GL_TEXTURE_BUFFER, _textBufferTBO );
	// Attach the storage for a buffer object to the active buffer texture
	glTexBuffer( GL_TEXTURE_BUFFER, GL_R32F, _textBuffer );
	glBindTexture( GL_TEXTURE_BUFFER, 0 );
	GV_CHECK_GL_ERROR();

	// Retrieve useful GigaVoxels arrays
	GvCore::GsLinearMemory< uint >* volTreeChildArray = this->_volumeTree->_childArray;
	GvCore::GsLinearMemory< uint >* volTreeDataArray = this->_volumeTree->_dataArray;
	GvCore::GsLinearMemory< uint >* updateBufferArray = this->_volumeTreeCache->getUpdateBuffer();
	GvCore::GsLinearMemory< uint >* nodeTimeStampArray = this->_volumeTreeCache->getNodesCacheManager()->getTimeStampList();
	GvCore::GsLinearMemory< uint >* brickTimeStampArray = this->_volumeTreeCache->getBricksCacheManager()->getTimeStampList();

	// Unmap GigaVoxels graphics resources from CUDA environment in order to be used by OpenGL
	volTreeChildArray->unmapResource();
	volTreeDataArray->unmapResource();
	updateBufferArray->unmapResource();
	nodeTimeStampArray->unmapResource();
	brickTimeStampArray->unmapResource();

	// Create a buffer texture associated to the GigaVoxels update buffer array
	glGenTextures( 1, &_updateBufferTBO );
	glBindTexture( GL_TEXTURE_BUFFER, _updateBufferTBO );
	// Attach the storage of buffer object to buffer texture
	glTexBuffer( GL_TEXTURE_BUFFER, GL_R32UI, updateBufferArray->getBufferName() );
	glBindTexture( GL_TEXTURE_BUFFER, 0 );

	// Create a buffer texture associated to the GigaVoxels node time stamp array
	glGenTextures( 1, &_nodeTimeStampTBO );
	glBindTexture( GL_TEXTURE_BUFFER, _nodeTimeStampTBO );
	// Attach the storage of buffer object to buffer texture
	glTexBuffer( GL_TEXTURE_BUFFER, GL_R32UI, nodeTimeStampArray->getBufferName() );
	glBindTexture( GL_TEXTURE_BUFFER, 0 );

	// Create a buffer texture associated to the GigaVoxels brick time stamp array
	glGenTextures( 1, &_brickTimeStampTBO );
	glBindTexture( GL_TEXTURE_BUFFER, _brickTimeStampTBO );
	// Attach the storage of buffer object to buffer texture
	glTexBuffer( GL_TEXTURE_BUFFER, GL_R32UI, brickTimeStampArray->getBufferName() );
	glBindTexture( GL_TEXTURE_BUFFER, 0 );

	// Create a buffer texture associated to the GigaVoxels data structure's child array
	glGenTextures( 1, &_childArrayTBO );
	glBindTexture( GL_TEXTURE_BUFFER, _childArrayTBO );
	// Attach the storage of buffer object to buffer texture
	glTexBuffer( GL_TEXTURE_BUFFER, GL_R32UI, volTreeChildArray->getBufferName() );
	glBindTexture( GL_TEXTURE_BUFFER, 0 );

	// Create a buffer texture associated to the GigaVoxels data structure's data array
	glGenTextures( 1, &_dataArrayTBO );
	glBindTexture( GL_TEXTURE_BUFFER, _dataArrayTBO );
	// Attach the storage of buffer object to buffer texture
	glTexBuffer( GL_TEXTURE_BUFFER, GL_R32UI, volTreeDataArray->getBufferName() );
	glBindTexture( GL_TEXTURE_BUFFER, 0 );

	// Check for OpenGL error(s)
	GV_CHECK_GL_ERROR();

	// Map GigaVoxels graphics resources in order to be used by CUDA environment
	volTreeChildArray->mapResource();
	volTreeDataArray->mapResource();
	updateBufferArray->mapResource();
	nodeTimeStampArray->mapResource();
	brickTimeStampArray->mapResource();

	//// Create and link a GLSL shader program
	//QString dataRepository = QCoreApplication::applicationDirPath() + QDir::separator() + QString( "Data" );
	//QString vertexShaderFilename = dataRepository + QDir::separator() + QString( "Shaders" ) + QDir::separator() + QString( "rayCastVert.glsl" );
	////QString vertexShaderFilename = QString( "rayCastVert.glsl" );
	////QFileInfo vertexShaderFileInfo( vertexShaderFilename );
	////if ( ( ! vertexShaderFileInfo.isFile() ) || ( ! vertexShaderFileInfo.isReadable() ) )
	////{
	////	// Idea
	////	// Maybe use Qt function : bool QFileInfo::permission ( QFile::Permissions permissions ) const

	////	// TO DO
	////	// Handle error : free memory and exit
	////	// ...
	////	std::cout << "ERROR. Check filename : " << vertexShaderFilename.toLatin1().constData() << std::endl;
	////}
	//QString fragmentShaderFilename = dataRepository + QDir::separator() + QString( "Shaders" ) + QDir::separator() + QString( "rayCastFrag.glsl" );
	////QString fragmentShaderFilename = QString( "rayCastFrag.glsl" );
	////QFileInfo fragmentShaderFileInfo( vertexShaderFilename );
	////if ( ( ! fragmentShaderFileInfo.isFile() ) || ( ! fragmentShaderFileInfo.isReadable() ) )
	////{
	////	// Idea
	////	// Maybe use Qt function : bool QFileInfo::permission ( QFile::Permissions permissions ) const

	////	// TO DO
	////	// Handle error : free memory and exit
	////	// ...
	////	std::cout << "ERROR. Check filename : " << fragmentShaderFilename.toLatin1().constData() << std::endl;
	////}
	//_rayCastProg = GvUtils::GsShaderManager::createShaderProgram( vertexShaderFilename.toLatin1().constData(), NULL, fragmentShaderFilename.toLatin1().constData() );
	//GvUtils::GsShaderManager::linkShaderProgram( _rayCastProg );
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
inline void GsRendererGLSL< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::finalize()
{
	// Destroy TBO
	if ( _updateBufferTBO )
	{
		glDeleteTextures( 1, &_updateBufferTBO );
	}

	// Destroy TBO
	if (_nodeTimeStampTBO)
	{
		glDeleteTextures( 1, &_nodeTimeStampTBO );
	}

	// Destroy TBO
	if ( _brickTimeStampTBO )
	{
		glDeleteTextures( 1, &_brickTimeStampTBO );
	}

	// Destroy TBO
	if ( _childArrayTBO )
	{
		glDeleteTextures( 1, &_childArrayTBO );
	}

	// Destroy TBO
	if ( _dataArrayTBO )
	{
		glDeleteTextures( 1, &_dataArrayTBO );
	}

	// TO DO
	//
	// Destroy :
	// _textBuffer
	// _textBufferTBO
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
void GsRendererGLSL< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::render( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	// Call internal render method
	doRender( pModelMatrix, pViewMatrix, pProjectionMatrix, pViewport );
}

/******************************************************************************
 * Start the rendering process.
 *
 * @param pModelMatrix the current model matrix
 * @param pViewMatrix the current view matrix
 * @param pProjectionMatrix the current projection matrix
 * @param pViewport the viewport configuration
 ******************************************************************************/
template< typename TVolumeTreeType, typename TVolumeTreeCacheType, typename TSampleShader >
void GsRendererGLSL< TVolumeTreeType, TVolumeTreeCacheType, TSampleShader >
::doRender( const float4x4& pModelMatrix, const float4x4& pViewMatrix, const float4x4& pProjectionMatrix, const int4& pViewport )
{
	CUDAPM_START_EVENT( vsrender_pre_frame );
	CUDAPM_START_EVENT( vsrender_pre_frame_mapbuffers );
	CUDAPM_STOP_EVENT( vsrender_pre_frame_mapbuffers );
	CUDAPM_STOP_EVENT( vsrender_pre_frame );

	// Check if a "clear request" has been asked
	if ( this->_clearRequested )
	{
		CUDAPM_START_EVENT( vsrender_clear );

		//frameNumAfterUpdate = 0;
		//fastBuildMode = true;

		// Clear the cache
		if ( this->_volumeTreeCache )
		{
			this->_volumeTreeCache->clearCache();
		}

		CUDAPM_STOP_EVENT( vsrender_clear );

		this->_clearRequested = false;
	}

	// Create a render view context to access to useful variables during (view matrix, model matrix, etc...)
	GvRendering::GsRendererContext viewContext;

	// Extract zNear, zFar as well as the distance in view space
	// from the center of the screen to each side of the screen.
	float fleft   = pProjectionMatrix._array[ 14 ] * ( pProjectionMatrix._array[ 8 ] - 1.0f ) / ( pProjectionMatrix._array[ 0 ] * ( pProjectionMatrix._array[ 10 ] - 1.0f ) );
	float fright  = pProjectionMatrix._array[ 14 ] * ( pProjectionMatrix._array[ 8 ] + 1.0f ) / ( pProjectionMatrix._array[ 0 ] * ( pProjectionMatrix._array[ 10 ] - 1.0f ) );
	float ftop    = pProjectionMatrix._array[ 14 ] * ( pProjectionMatrix._array[ 9 ] + 1.0f ) / ( pProjectionMatrix._array[ 5 ] * ( pProjectionMatrix._array[ 10 ] - 1.0f ) );
	float fbottom = pProjectionMatrix._array[ 14 ] * ( pProjectionMatrix._array[ 9 ] - 1.0f ) / ( pProjectionMatrix._array[ 5 ] * ( pProjectionMatrix._array[ 10 ] - 1.0f ) );
	float fnear   = pProjectionMatrix._array[ 14 ] / ( pProjectionMatrix._array[ 10 ] - 1.0f );
	float ffar    = pProjectionMatrix._array[ 14 ] / ( pProjectionMatrix._array[ 10 ] + 1.0f );

	//float2 viewSurfaceVS[2];
	//viewSurfaceVS[0]=make_float2(fleft, fbottom);
	//viewSurfaceVS[1]=make_float2(fright, ftop);

	//float2 viewSurfaceVS_Size=viewSurfaceVS[1]-viewSurfaceVS[0];
	/////////////////////////////////////////////

	// transfor matrices
	float4x4 invprojectionMatrixT=transpose(inverse(pProjectionMatrix));
	float4x4 invViewMatrixT=transpose(inverse(pViewMatrix));

	float4x4 projectionMatrixT=transpose(pProjectionMatrix);
	float4x4 viewMatrixT=transpose(pViewMatrix);

	CUDAPM_START_EVENT(vsrender_copyconsts_frame);

	viewContext.invViewMatrix=invViewMatrixT;
	viewContext.viewMatrix=viewMatrixT;
	//viewContext.invProjMatrix=invprojectionMatrixT;
	//viewContext.projMatrix=projectionMatrixT;

	// Store frustum parameters
	viewContext.frustumNear = fnear;
	viewContext.frustumNearINV = 1.0f / fnear;
	viewContext.frustumFar = ffar;
	viewContext.frustumRight = fright;
	viewContext.frustumTop = ftop;
	viewContext.frustumC = pProjectionMatrix._array[ 10 ]; // - ( ffar + fnear ) / ( ffar - fnear );
	viewContext.frustumD = pProjectionMatrix._array[ 14 ]; // ( -2.0f * ffar * fnear ) / ( ffar - fnear );

	float3 viewPlanePosWP = mul( viewContext.invViewMatrix, make_float3( fleft, fbottom, -fnear ) );
	viewContext.viewCenterWP = mul( viewContext.invViewMatrix, make_float3( 0.0f, 0.0f, 0.0f ) );
	viewContext.viewPlaneDirWP = viewPlanePosWP - viewContext.viewCenterWP;

	// Resolution dependant stuff
	viewContext.frameSize = make_uint2( pViewport.z, pViewport.w );
	//float2 pixelSize=viewSurfaceVS_Size/make_float2((float)viewContext.frameSize.x, (float)viewContext.frameSize.y);
	//viewContext.pixelSize=pixelSize;
	viewContext.viewPlaneXAxisWP = ( mul( viewContext.invViewMatrix, make_float3( fright, fbottom, -fnear ) ) - viewPlanePosWP ) / static_cast< float >( viewContext.frameSize.x );
	viewContext.viewPlaneYAxisWP = ( mul( viewContext.invViewMatrix, make_float3( fleft, ftop, -fnear ) ) - viewPlanePosWP ) / static_cast< float >( viewContext.frameSize.y );

	// Copy data to CUDA memory
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_renderViewContext, &viewContext, sizeof( viewContext ) ) );
	GS_CUDA_SAFE_CALL( cudaMemcpyToSymbol( k_currentTime, &(this->_currentTime), sizeof( this->_currentTime ), 0, cudaMemcpyHostToDevice ) );

	CUDAPM_STOP_EVENT( vsrender_copyconsts_frame );

	// Specify values of uniform variables for shader program object
	glProgramUniform3fEXT( _rayCastProg, glGetUniformLocation( _rayCastProg, "viewPos" ), viewContext.viewCenterWP.x, viewContext.viewCenterWP.y, viewContext.viewCenterWP.z );
	glProgramUniform3fEXT( _rayCastProg, glGetUniformLocation( _rayCastProg, "viewPlane" ), viewContext.viewPlaneDirWP.x, viewContext.viewPlaneDirWP.y, viewContext.viewPlaneDirWP.z );
	glProgramUniform3fEXT( _rayCastProg, glGetUniformLocation( _rayCastProg, "viewAxisX" ), viewContext.viewPlaneXAxisWP.x, viewContext.viewPlaneXAxisWP.y, viewContext.viewPlaneXAxisWP.z );
	glProgramUniform3fEXT( _rayCastProg, glGetUniformLocation( _rayCastProg, "viewAxisY" ), viewContext.viewPlaneYAxisWP.x, viewContext.viewPlaneYAxisWP.y, viewContext.viewPlaneYAxisWP.z );
	glProgramUniform2fEXT( _rayCastProg, glGetUniformLocation( _rayCastProg, "frameSize" ), static_cast< float >( viewContext.frameSize.x ), static_cast< float >( viewContext.frameSize.y ) );
	glProgramUniformMatrix4fvEXT( _rayCastProg, glGetUniformLocation( _rayCastProg, "modelViewMat" ), 1, GL_FALSE, pViewMatrix._array );

	// Do pre-render pass
	this->_volumeTreeCache->preRenderPass();

	//frameNumAfterUpdate = 0;

	//fastBuildMode = false;

	//uint numLoop = 0;

#ifdef _DEBUG
	// DEBUG : text buffer
	{
		glBindBuffer( GL_TEXTURE_BUFFER, _textBuffer );
		GLfloat* textBuffer = (GLfloat *)glMapBuffer( GL_TEXTURE_BUFFER, GL_WRITE_ONLY );

		for ( int i = 0; i < 8192; i++ )
		{
			textBuffer[ i ] = 0.0f;
		}

		glUnmapBuffer( GL_TEXTURE_BUFFER );
		glBindBuffer( GL_TEXTURE_BUFFER, 0 );
	}
#endif

	CUDAPM_START_EVENT_GPU( gv_rendering );

	/*if ( this->_dynamicUpdate )
	{
	}
	else
	{
	}*/

	// Disable writing into the depth buffer
	//glEnable(GL_DEPTH_TEST);
	glDepthMask( GL_FALSE );

	// Activate blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Installs program object as part of current rendering state
	glUseProgram( _rayCastProg );

	// Retrieve useful GigaVoxels arrays
	GvCore::GsLinearMemory< uint >* volTreeChildArray = this->_volumeTree->_childArray;
	GvCore::GsLinearMemory< uint >* volTreeDataArray = this->_volumeTree->_dataArray;
	GvCore::GsLinearMemory< uint >* updateBufferArray = this->_volumeTreeCache->getUpdateBuffer();
	GvCore::GsLinearMemory< uint >* nodeTimeStampArray = this->_volumeTreeCache->getNodesCacheManager()->getTimeStampList();
	GvCore::GsLinearMemory< uint >* brickTimeStampArray = this->_volumeTreeCache->getBricksCacheManager()->getTimeStampList();

	// Unmap GigaVoxels graphics resources from CUDA environment in order to be used by OpenGL
	updateBufferArray->unmapResource();
	nodeTimeStampArray->unmapResource();
	brickTimeStampArray->unmapResource();
	volTreeChildArray->unmapResource();
	volTreeDataArray->unmapResource();
	this->_volumeTree->_dataPool->getChannel( Loki::Int2Type< 0 >() )->unmapResource();

	// Returns location of uniform variables
	GLint volTreeChildArrayLoc = glGetUniformLocation( _rayCastProg, "d_volTreeChildArray" );
	GLint volTreeDataArrayLoc = glGetUniformLocation( _rayCastProg, "d_volTreeDataArray" );
	GLint updateBufferArrayLoc = glGetUniformLocation( _rayCastProg, "d_updateBufferArray" );
	GLint nodeTimeStampArrayLoc = glGetUniformLocation( _rayCastProg, "d_nodeTimeStampArray" );
	GLint brickTimeStampArrayLoc = glGetUniformLocation( _rayCastProg, "d_brickTimeStampArray" );
	GLint currentTimeLoc = glGetUniformLocation( _rayCastProg, "k_currentTime" );

	// Note :
	// glBindImageTextureEXT() command binds a single level of a texture to an image unit
	// for the purpose of reading and writing it from shaders.
	//
	// Specification :
	// void glBindImageTexture( GLuint  unit,  GLuint  texture,  GLint  level,  GLboolean  layered,  GLint  layer,  GLenum  access,  GLenum  format );

	glBindImageTextureEXT( 0, _updateBufferTBO, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI );
	glUniform1i( updateBufferArrayLoc, 0 );

	glBindImageTextureEXT( 1, _nodeTimeStampTBO, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI );
	glUniform1i( nodeTimeStampArrayLoc, 1 );

	glBindImageTextureEXT(2, _brickTimeStampTBO, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI );
	glUniform1i( brickTimeStampArrayLoc, 2 );

	glBindImageTextureEXT(3, _childArrayTBO, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI );
	glUniform1i( volTreeChildArrayLoc, 3 );

	glBindImageTextureEXT( 4, _dataArrayTBO, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI );
	glUniform1i( volTreeDataArrayLoc, 4 );

	glUniform1ui( currentTimeLoc, this->_currentTime );

	glBindImageTextureEXT( 7, _textBufferTBO, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F );
	glUniform1i( glGetUniformLocation( _rayCastProg, "d_textBuffer" ), 7 );

	// Retrieve node pool and brick pool resolution
	uint3 nodePoolRes = this->_volumeTree->_nodePool->getChannel( Loki::Int2Type< 0 >() )->getResolution();
	uint3 brickPoolRes = this->_volumeTree->_dataPool->getChannel( Loki::Int2Type< 0 >() )->getResolution();

	// Retrieve node cache and brick cache size
	uint3 nodeCacheSize = nodeTimeStampArray->getResolution();
	uint3 brickCacheSize = brickTimeStampArray->getResolution();

	glUniform3ui( glGetUniformLocation( _rayCastProg, "nodeCacheSize" ),
		nodeCacheSize.x, nodeCacheSize.y, nodeCacheSize.z );

	glUniform3ui( glGetUniformLocation( _rayCastProg, "brickCacheSize" ),
		brickCacheSize.x, brickCacheSize.y, brickCacheSize.z );

	GLint dataPoolLoc = glGetUniformLocation( _rayCastProg, "dataPool" );
	glUniform1i( dataPoolLoc, 0 );

	glUniform3f( glGetUniformLocation( _rayCastProg, "nodePoolResInv" ),
		1.0f / (GLfloat)nodePoolRes.x, 1.0f / (GLfloat)nodePoolRes.y, 1.0f / (GLfloat)nodePoolRes.z );

	glUniform3f( glGetUniformLocation( _rayCastProg, "brickPoolResInv" ),
		1.0f / (GLfloat)brickPoolRes.x, 1.0f / (GLfloat)brickPoolRes.y, 1.0f / (GLfloat)brickPoolRes.z );

	glUniform1ui( glGetUniformLocation( _rayCastProg, "maxVolTreeDepth" ), this->_volumeTree->getMaxDepth() );

	glUniform1f( glGetUniformLocation( _rayCastProg, "frustumC" ), viewContext.frustumC );
	glUniform1f( glGetUniformLocation( _rayCastProg, "frustumD" ), viewContext.frustumD );

	// Check for OpenGL error(s)
	GV_CHECK_GL_ERROR();

	// Bind user data as 3D texture for rendering
	//
	// Content of one voxel has been defined as :
	// typedef Loki::TL::MakeTypelist< uchar4 >::Result DataType;
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_3D, this->_volumeTree->_dataPool->getChannel( Loki::Int2Type< 0 >() )->getBufferName() );

	GLuint vploc = glGetAttribLocation( _rayCastProg, "vertexPos" );

	// Set projection matrix to identity
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	// Set model/view matrix to identity
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	// Draw a quad on full screen
	glBegin( GL_QUADS );
	glVertexAttrib4f( vploc, -1.0f, -1.0f, 0.0f, 1.0f );
	glVertexAttrib4f( vploc,  1.0f, -1.0f, 0.0f, 1.0f );
	glVertexAttrib4f( vploc,  1.0f,  1.0f, 0.0f, 1.0f );
	glVertexAttrib4f( vploc, -1.0f,  1.0f, 0.0f, 1.0f );
	glEnd();

	// Restore previous projection matrix
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	// Restore previous model/view matrix
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	// Unbind user data as 3D texture for rendering
	glBindTexture( GL_TEXTURE_3D, 0 );

	// Stop using shader program
	//
	// Programmable processors will be disabled and fixed functionality will be used
	// for both vertex and fragment processing.
	glUseProgram( 0 );

	//glMemoryBarrierEXT( GL_BUFFER_UPDATE_BARRIER_BIT_EXT );
	//glMemoryBarrierEXT( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT );
	//glMemoryBarrierEXT( GL_ALL_BARRIER_BITS_EXT );

	// Check for OpenGL error(s)
	GV_CHECK_GL_ERROR();

#ifdef _DEBUG
	// DEBUG : text buffer
	glBindBuffer( GL_TEXTURE_BUFFER, _textBuffer );
	GLfloat* textBuffer = (GLfloat *)glMapBuffer( GL_TEXTURE_BUFFER, GL_READ_ONLY );

	for ( int i = 0; i < 8192; i++ )
	{
		if ( textBuffer[ i ] != 0.0f )
		{
			printf( "\ntextBuffer[ %d ] = %f", i, textBuffer[ i ] );
		}
	}

	glUnmapBuffer( GL_TEXTURE_BUFFER );
	glBindBuffer( GL_TEXTURE_BUFFER, 0 );
#endif

	// Map GigaVoxels graphics resources in order to be used by CUDA environment
	updateBufferArray->mapResource();
	nodeTimeStampArray->mapResource();
	brickTimeStampArray->mapResource();
	volTreeChildArray->mapResource();
	volTreeDataArray->mapResource();
	this->_volumeTree->_dataPool->getChannel(Loki::Int2Type<0>())->mapResource();

	// Disable blending and enable writing into depth buffer
	glDisable( GL_BLEND );
	//glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );

	CUDAPM_STOP_EVENT_GPU( gv_rendering );

	CUDAPM_START_EVENT( dataProduction_handleRequests );

	// Bricks loading
	if ( this->_dynamicUpdate )
	{
		//dynamicUpdate = false;

		//uint maxNumSubdiv;
		//uint maxNumBrickLoad;
		//maxNumSubdiv = 5000;
		//maxNumBrickLoad = 3000;

		this->_volumeTreeCache->_intraFramePass = false;

		// Post render pass
		//this->_volumeTreeCache->handleRequests( maxNumSubdiv, maxNumBrickLoad );
		this->_volumeTreeCache->handleRequests();
	}

	CUDAPM_STOP_EVENT( dataProduction_handleRequests );

	CUDAPM_START_EVENT( vsrender_post_frame );
	CUDAPM_START_EVENT( vsrender_post_frame_unmapbuffers );
	CUDAPM_STOP_EVENT( vsrender_post_frame_unmapbuffers );
	CUDAPM_STOP_EVENT( vsrender_post_frame );

	GV_CHECK_CUDA_ERROR( "GsRendererGLSL::render" );
}

} // namespace GvRendering
