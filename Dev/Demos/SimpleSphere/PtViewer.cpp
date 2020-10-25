/*
 * Publication: By-Example Point Process Texture Basis Functions
 * Authors: anonymous
 *
 * Code author: anonymous
 *
 * ICube laboratory (the Engineering science, computer science and imaging laboratory) [https://icube.unistra.fr/en/]
 * IGG group (Computer Graphics and Geometry) [http://icube-igg.unistra.fr/en/index.php/Main_Page]
 */

/** 
 * @version 1.0
 */

#include "PtViewer.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GL
//#include <GL/glew.h>
//#include <glad/glad.h>

// STL
#include <string>
#include <iostream>
#include <fstream>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//// System
//#ifdef WIN32
//	#ifndef WIN32_LEAN_AND_MEAN
//		#define WIN32_LEAN_AND_MEAN
//	#endif
//	#include <windows.h>
//#endif

#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <imgui_internal.h>

// Project
#include "PtApplication.h"
#include "PtShaderProgram.h"

// Project
#include "SampleCore.h"

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

 // Project
//using namespace Pt;
using namespace PtGraphics;
using namespace PtGUI;

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
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Default constructor
 ******************************************************************************/
PtViewer::PtViewer()
:	mWindow( nullptr )
,	mCamera( nullptr )
,	mLaunchRequest( false )
,	mShowUI( true )
,	mGraphicsSynthesizer( nullptr )
,	mSynthesizerUpdateRequested( true )
,	mHistogramUpdateRequested( false )
,	mUIShowHistogram( false )
,	mDisplayExemplarPyramidInfo( false )
,	mDisplaySynthesisPyramidColorInfo( false )
,	mDisplaySynthesisPyramidPositionInfo( false )
,	mDisplayDistanceMapPyramidInfo( false )
,	mUseGLCoreProfile( false )
,	mGLCoreProfileVAO( 0 )
,	mGLVendor()
,	mGLRenderer()
,	mGLVersion()
,	mGLShadingLanguageVersion()
,	mGLMajorVersion( 0 )
,	mGLMinorVersion( 0 )
{
	uiModelFilename = std::string();
	uiDistanceFilename = std::string();

	uiTextureSynthesis_launch = false;

	// Initialize camera
	mCamera = new PtCamera();
	mCamera->setCameraType( PtCamera::ePerspective );
	mCamera->setEye( glm::vec3( 0.f, 0.f, 5.f ) );

	// Synthesizer
	mGraphicsSynthesizer = new SampleCore();
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
PtViewer::~PtViewer()
{
	//mGraphicsSynthesizer->finalize();
	delete mGraphicsSynthesizer;

	delete mCamera;
	mCamera = nullptr;
}

/******************************************************************************
 * Initialize Window
 ******************************************************************************/
void PtViewer::initializeWindow()
{
	// uncomment these lines if on Apple OS X
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	// Ask for GL profile
#if 1
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	mUseGLCoreProfile = true;
#else
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
	mUseGLCoreProfile = false;
#endif

	// Create window
	mWindow = glfwCreateWindow( 512, 512, "GigaVoxels / GigaSpace Viewer", nullptr, nullptr );
	if ( ! mWindow )
	{
		// Window or OpenGL context creation failed
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	// Initialize ImGui
	// Need a GL context?
	glfwMakeContextCurrent( mWindow );
	// - GL binding
	const bool install_callbacks = false; // customize call: call ImGui ones then custom ones
	ImGui_ImplGlfwGL3_Init( mWindow, install_callbacks );
	// - style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();
	// User interface parameters
	// - pyramid
	uiPyramid_maxLevel = 9;
	uiPyramid_minSize = 32;
	// - block initializtion
	uiBlockInitialization_blockSize = 8;
	// - correction
	uiNbCorrectionPasses = 2;
	uiCorrectionSubBlockSize = 2;
	// - NN search
	uiNNSearch_depth = 3;
	uiNNSearch_nbSamples = 5;
	uiNNSearch_radius = 4;
	uiNNSearch_patchSize = 2;
	// - distance map
	uiDistance_weight = 0.0f;
		
	// Initialize window event handling
	// - custom user data
	glfwSetWindowUserPointer( mWindow, this );
	// - window
	glfwSetWindowCloseCallback( mWindow, window_close_callback );
	glfwSetWindowSizeCallback( mWindow, window_size_callback );
	glfwSetWindowPosCallback( mWindow, window_pos_callback );
	// - frame buffer
	glfwSetFramebufferSizeCallback( mWindow, framebuffer_size_callback );
	// - mouse
	glfwSetCursorPosCallback( mWindow, cursor_position_callback );
	glfwSetMouseButtonCallback( mWindow, mouse_button_callback );
	glfwSetScrollCallback( mWindow, scroll_callback );
	// - keyboard
	glfwSetKeyCallback( mWindow, key_callback );
	// - text input
	glfwSetCharCallback( mWindow, character_callback );
}

/******************************************************************************
* Initialize ImGui user interface
******************************************************************************/
void PtViewer::initializeImGuiUserInterface()
{
	// Initialize ImGui
	// - GL binding
	ImGui_ImplGlfwGL3_Init( getWindow(), true ); // TODO: check callback: erased/overlapped by ImGui: use custom and call ImGui one !!! () => use FALSE !!!!
	// - style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();
}

/******************************************************************************
 * ...
 ******************************************************************************/
static void APIENTRY openglCallbackFunction(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam
)
{
  /*(void)source; (void)type; (void)id; 
  (void)severity; (void)length; (void)userParam;
  
  fprintf( stderr, "%s\n", message );
  
  if ( severity == GL_DEBUG_SEVERITY_HIGH )
  {
    fprintf( stderr, "Aborting...\n" );
    abort();
  }*/

	if ( severity != GL_DEBUG_SEVERITY_HIGH )
	{
		return;
	}

	std::cout << "---------------------OpenGL DEBUG:" << std::endl;
	std::cout << "message: "<< message << std::endl;
	std::cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
        break;
    }
	std::cout << std::endl;
 
	std::cout << "id: " << id << std::endl;
	std::cout << "severity: ";
    switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
        break;
    }
}

/******************************************************************************
 * Initialize GL
 ******************************************************************************/
void PtViewer::initializeGL()
{
	/**
	 * OpenGL and GLSL versions
	 */
	/*const GLubyte* mGLVendor;
	const GLubyte* mGLRenderer;
	const GLubyte* mGLVersion;
	const GLubyte* mGLShadingLanguageVersion;
	GLint mGLMajorVersion;
	GLint mGLMinorVersion;*/

	// Determine the OpenGL and GLSL versions
	mGLVendor = std::string( reinterpret_cast< const char* const >( glGetString( GL_VENDOR ) ) );
	mGLRenderer = std::string( reinterpret_cast< const char* const >( glGetString( GL_RENDERER ) ) );
	mGLVersion = std::string( reinterpret_cast< const char* const >( glGetString( GL_VERSION ) ) );
	mGLShadingLanguageVersion = std::string( reinterpret_cast< const char* const >( glGetString( GL_SHADING_LANGUAGE_VERSION ) ) );
	glGetIntegerv( GL_MAJOR_VERSION, &mGLMajorVersion );
	glGetIntegerv( GL_MINOR_VERSION, &mGLMinorVersion );
	
	// Determine the OpenGL and GLSL versions
	printf( "\n[Renderer Info]\n" );
	printf( "\tGL Vendor: %s\n", mGLVendor.data() );
	printf( "\tGL Renderer: %s\n", mGLRenderer.data() );
	printf( "\tGL Version (string): %s\n", mGLVersion.data() );
	printf( "\tGL Version (integer): %d.%d\n", mGLMajorVersion, mGLMinorVersion );
	printf( "\tGLSL Version: %s\n", mGLShadingLanguageVersion.data() );

	std::cout << "\n[Path configuration]" << std::endl;
	std::string projectPath = "";
	std::string projectDataPath = "";
	std::string projectImagePath = "";
	std::string projectShaderPath = "";
	std::string projectBinPath = "";
#ifdef PT_PROJECT_PATH
	projectPath = PT_PROJECT_PATH;
#endif
#ifdef PT_DATA_PATH
	projectDataPath = PT_DATA_PATH;
#endif
#ifdef PT_IMAGE_PATH
	projectImagePath = PT_IMAGE_PATH;
#endif
#ifdef PT_SHADER_PATH
	projectShaderPath = PT_SHADER_PATH;
#endif
#ifdef PT_BIN_PATH
	projectBinPath = PT_BIN_PATH;
#endif
	printf( "\tPROJECT\t%s\n", projectPath.c_str() );
	printf( "\tDATA\t%s\n", projectDataPath.c_str() );
	printf( "\tIMAGE\t%s\n", projectImagePath.c_str() );
	printf( "\tSHADER\t%s\n", projectShaderPath.c_str() );
	printf( "\tBIN\t%s\n", projectBinPath.c_str() );

#if _DEBUG // Use a boolean in User Interface instead !!!!!!!!!!!!!!!!!!!!!
	// Enable graphics callback
	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( openglCallbackFunction, nullptr );
	glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true );
#endif

	// Initialize Graphics data

	int width, height;
	glfwGetWindowSize( mWindow, &width, &height );
	//glfwGetFramebufferSize( mWindow, &width, &height );
	glViewport( 0, 0, width, height );

	//// Initialize graphics PPTBF
	//mGraphicsSynthesizer->setWidth( width );
	//mGraphicsSynthesizer->setHeight( height );
	//mGraphicsSynthesizer->initialize();
	//mGraphicsSynthesizer->initializeGL();
	// Texture initialization
	//const std::string imagePath = std::string( PT_IMAGE_PATH ) + std::string( "/" );
	//std::string textureFilename = "";
	//// Initialize exemplar data
	//// - exemplar
	//textureFilename = mGraphicsSynthesizer->getExemplarFilename();
	//if ( textureFilename.empty() )
	//{
	//	// Default texture
	//	textureFilename = imagePath + std::string( "DB_Pascal/stains/" ) + std::string( "metal_peeling_paint_4131528.png" );
	//}
	//uiModelFilename = textureFilename;

	// Cannot draw with a bounded VAO in GL Core profile...?
	glGenVertexArrays( 1, &mGLCoreProfileVAO );

	// Initialize the GigaVoxels pipeline
	//mGraphicsSynthesizer = new SampleCore();
	mGraphicsSynthesizer->init();
	mGraphicsSynthesizer->resize( width, height );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::finalize()
{
	// Cannot draw with a bounded VAO in GL Core profile...?
	glDeleteVertexArrays( 1, &mGLCoreProfileVAO );

	// Finalize ImGui
	ImGui_ImplGlfwGL3_Shutdown();

	glfwDestroyWindow( mWindow );
}

/******************************************************************************
 * ...
 ******************************************************************************/
GLFWwindow* PtViewer::getWindow()
{
	return mWindow;
}

/******************************************************************************
 * Get synthesizer
 ******************************************************************************/
SampleCore* PtViewer::getSynthesizer()
{
	return mGraphicsSynthesizer;
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	// Call ImGui callback
	ImGui_ImplGlfwGL3_KeyCallback( window, key, scancode, action, mods );

	//printf( "\nPtTextureSynthesizerViewer::key_callback" );

	/*if ( static_cast< PtViewer* >(glfwGetWindowUserPointer(window)) != this )
	{
		return;
	}*/

    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	{
        glfwSetWindowShouldClose( window, GLFW_TRUE );
	}

	if ( key == GLFW_KEY_SPACE && action == GLFW_RELEASE )
	{
		const bool currentFlag = static_cast< PtViewer* >( glfwGetWindowUserPointer( window ) )->mShowUI;
		static_cast< PtViewer* >( glfwGetWindowUserPointer( window ) )->mShowUI = ( ! currentFlag );
	}

//	printf("key_callback = (%d,%d,%d,%d)", key, scancode, action, mods);
	
	static_cast< PtViewer* >( glfwGetWindowUserPointer( window ) )->onKeyModified( key, scancode, action, mods );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::onKeyModified( const int key, const int scancode, const int action, const int mods )
{
	switch ( key )
	{
		case GLFW_KEY_KP_ADD:
			mGraphicsSynthesizer->incMaxVolTreeDepth();
			break;

		case GLFW_KEY_KP_SUBTRACT:
			mGraphicsSynthesizer->decMaxVolTreeDepth();
			break;

		case GLFW_KEY_C:
			// Tell GigaVoxels to clear its cache
			mGraphicsSynthesizer->clearCache();
			break;

		case GLFW_KEY_D:
			mGraphicsSynthesizer->toggleDynamicUpdate();
			break;

		case GLFW_KEY_I:
			// Toggle GigaVoxels performance monitor mechanism (if it has been activated during GigaVoxels compilation)
			mGraphicsSynthesizer->togglePerfmonDisplay( 1 );
			break;

		case GLFW_KEY_T:
			// Toggle the display of the GigaVoxels space partitioning structure
			mGraphicsSynthesizer->toggleDisplayOctree();
			break;

		case GLFW_KEY_U:
			// Toggle GigaVoxels performance monitor mechanism (if it has been activated during GigaVoxels compilation)
			mGraphicsSynthesizer->togglePerfmonDisplay( 2 );
			break;

		case GLFW_KEY_L:
			// Toggle light manipulation mechanism
			//setLightManipulation( ! getLightManipulation() );
			break;

		case GLFW_KEY_R:
			// Reset light position
			//_light1->setPosition( 0.75f, 0.75f, 0.75f );
			// Update GigaVoxels light position
			mGraphicsSynthesizer->setLightPosition( 0.75f, 0.75f, 0.75f );
			break;

		default:
		//	QGLViewer::keyPressEvent( pEvent );
			break;
	}
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::character_callback( GLFWwindow* window, unsigned int codepoint )
{
	// Call ImGui callback
	ImGui_ImplGlfwGL3_CharCallback( window, codepoint );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::window_close_callback( GLFWwindow* window )
{
	//printf("\nPtTextureSynthesizerViewer::window_close_callback");

    //if ( ! time_to_close )
	{
        glfwSetWindowShouldClose( window, GLFW_TRUE );
	}

	//printf("window_close_callback");
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::window_size_callback( GLFWwindow* window, int width, int height )
{
	//printf( "window_size_callback = (%d,%d)", width, height );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
	glViewport( 0, 0, width, height );
	
	//printf( "framebuffer_size_callback = (%d,%d)", width, height );

	static_cast< PtViewer* >( glfwGetWindowUserPointer( window ) )->onSizeModified( width, height );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::onSizeModified( const int pWidth, const int pHeight )
{
	//mSynthesizerUpdateRequested = true;

	// Update PPTBF framebuffer
	//mGraphicsSynthesizer->onSizeModified( pWidth, pHeight );

	// Scene
	const float aspect = static_cast< float >( pWidth ) / static_cast< float >( pHeight );
	mCamera->setAspect( aspect );

	// Handle GigaVoxels resize
	mGraphicsSynthesizer->resize( pWidth, pHeight );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::window_pos_callback( GLFWwindow* window, int xpos, int ypos )
{
//	printf( "window_pos_callback = (%d,%d)", xpos, ypos );
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("\nPtTextureSynthesizerViewer::cursor_position_callback");

//	printf("cursor_position_callback = (%f,%f)", xpos, ypos);

	static_cast<PtViewer*>(glfwGetWindowUserPointer(window))->processMouseMoveEvent( xpos, ypos );
}

/******************************************************************************
* ...
******************************************************************************/
void PtViewer::processMouseMoveEvent(double xpos, double ypos)
{
	// Global ("static") variables used to track mouse position
	static double lastPosX = xpos;
	static double lastPosY = ypos;

	// Retrieve GLFW mouse and key modifiers states
	const int mouseButtonLeftStatus = glfwGetMouseButton( mWindow, GLFW_MOUSE_BUTTON_LEFT );
	const int mouseButtonRightStatus = glfwGetMouseButton( mWindow, GLFW_MOUSE_BUTTON_RIGHT );
	const int keyStatus = glfwGetKey( mWindow, GLFW_KEY_LEFT_CONTROL );
	//const int keyAltStatus = glfwGetKey( mWindow, GLFW_KEY_LEFT_ALT );
	const int keyShiftStatus = glfwGetKey( mWindow, GLFW_KEY_LEFT_SHIFT );
	
	//PtGraphics::PtGraphicsPPTBF* pptbf = mGraphicsSynthesizer->editPPTBF();

	//// Handle translation movements
	//if ( mouseButtonRightStatus == GLFW_PRESS && keyShiftStatus == GLFW_PRESS )
	//{
	//	// Update PPTBF translation
	//	const float xoffset = static_cast< float >( xpos - lastPosX );
	//	const float yoffset = static_cast< float >( ypos - lastPosY );
	//	pptbf->setShiftX( pptbf->getShiftX() + xoffset * 0.025f );
	//	pptbf->setShiftY( pptbf->getShiftY() + yoffset * 0.025f );

	//	// Update PPTBF
	//	mPPTBFUpdateRequested = true;
	//}

	//// Handle rotation movements
	//if ( mouseButtonLeftStatus == GLFW_PRESS && keyShiftStatus == GLFW_PRESS )
	//{
	//	// Update PPTBF orientation
	//	const float angleOffset = static_cast< float >( xpos - lastPosX );
	//	float alpha = pptbf->getAlpha();
	//	alpha /= static_cast< float >( M_PI );
	//	alpha += angleOffset * 0.025f;
	//	alpha = glm::mod( alpha, 2.f );
	//	pptbf->setAlpha( alpha * static_cast< float >( M_PI ) );

	//	// Update PPTBF
	//	mPPTBFUpdateRequested = true;
	//}

	//-------------------------------------------------------------------------------------
	// Scene Management
	//-------------------------------------------------------------------------------------
	// Handle rotation movements
	if ( mouseButtonLeftStatus == GLFW_PRESS && keyStatus == GLFW_PRESS )
	{
	//	//mCamera->truck( 0.01f * ( xpos - lastPosX ) );
	//	
		const float xoffset = static_cast< float >( xpos - lastPosX );
		const float yoffset = static_cast< float >( ypos - lastPosY );
		glm::vec3 axis = glm::vec3( yoffset, xoffset, 0.f );
		axis = glm::normalize( axis );
		const float angle = glm::length( glm::vec2( xoffset, yoffset ) ) * 0.005f;
		glm::quat quaternion = glm::angleAxis( angle, axis );
		glm::mat4 transform = glm::mat4_cast( quaternion );

	//	//mGraphicsMeshManager->mModelMatrix = transform * mGraphicsMeshManager->mModelMatrix;

	//	// Update transformation matrix
	//	mGraphicsScene->mMatrix = transform * mGraphicsScene->mMatrix;

		glm::mat4 m = transform * glm::make_mat4( mGraphicsSynthesizer->_modelMatrix._array );

		int k = 0;
		for ( int c = 0; c < 4; c++ )
			for ( int r = 0; r < 4; r++ )
		{
				mGraphicsSynthesizer->_modelMatrix._array[ k ] = static_cast< float >( m[ c ][ r ] );
			k++;
		}
	}

	// Handle translation movements
	if ( mouseButtonRightStatus == GLFW_PRESS && keyStatus == GLFW_PRESS )
	{
		mCamera->truck( -0.05f * static_cast< float >( xpos - lastPosX ) );
		mCamera->pedestal( 0.05f * static_cast< float >( ypos - lastPosY ) );
	}
	//-------------------------------------------------------------------------------------

	// Update current mouse position
	lastPosX = xpos;
	lastPosY = ypos;
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
{
	// Call ImGui callback
	ImGui_ImplGlfwGL3_MouseButtonCallback( window, button, action, mods );

	//printf("\nPtTextureSynthesizerViewer::mouse_button_callback");

	//if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
	//{
	//	//popup_menu();
	//}

	//printf("mouse_button_callback = (%d,%d;%d)", button, action, mods);
	static_cast<PtViewer*>( glfwGetWindowUserPointer( window ))->processMouseButtonEvent(button, action, mods);
	//printf("mouse_button_callback = (%d,%d;%d)", button, action, mods );
}
void PtViewer::processMouseButtonEvent(int button, int action, int mods)
{

}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::scroll_callback( GLFWwindow* window, double xoffset, double yoffset )
{
	// Call ImGui callback
	ImGui_ImplGlfwGL3_ScrollCallback( window, xoffset, yoffset );

	//printf( "scroll_callback = (%f,%f)", xoffset, yoffset );

	// Call Pt callback
	static_cast< PtViewer* >( glfwGetWindowUserPointer( window ) )->processScrollEvent( xoffset, yoffset );
}

/******************************************************************************
 * If you wish to be notified when the user scrolls, whether with a mouse wheel or touchpad gesture, set a scroll callback.
 * The callback function receives two-dimensional scroll offsets.
 * A simple mouse wheel, being vertical, provides offsets along the Y-axis.
 ******************************************************************************/
void PtViewer::processScrollEvent( double xoffset, double yoffset )
{
	// Zoom in PPTBF structure
	const int keyShiftStatus = glfwGetKey( mWindow, GLFW_KEY_LEFT_SHIFT );
	//if ( keyShiftStatus == GLFW_PRESS )
	//{
	//	// Update PPTBF resolution
	//	PtGraphics::PtGraphicsPPTBF* pptbf = mGraphicsSynthesizer->editPPTBF();
	//	int resolution = pptbf->getResolution();
	//	resolution += static_cast< int >( - yoffset * 10. );
	//	resolution = glm::clamp( resolution, 1, 10000 );
	//	pptbf->setResolution( resolution );
	//	
	//	// Update PPTBF
	//	mPPTBFUpdateRequested = true;
	//}

	// Scene Management
	const int keyStatus = glfwGetKey( mWindow, GLFW_KEY_LEFT_CONTROL );
	if ( keyStatus == GLFW_PRESS )
	{
		mCamera->dolly( - static_cast< float >( yoffset ) * 1.1f );
	}
}

/******************************************************************************
 * Refresh
 ******************************************************************************/
void PtViewer::refresh()
{
	// Set context
	// NOTE:
	// By default, making a context non-current implicitly forces a pipeline flush
	// On machines that support GL_KHR_context_flush_control, you can control whether a context performs this flush by setting the GLFW_CONTEXT_RELEASE_BEHAVIOR window hint.
	glfwMakeContextCurrent( getWindow() );
				
	// Render scene
	render();

	// Swap buffers
	glfwSwapBuffers( getWindow() );
}

/******************************************************************************
 * Render
 ******************************************************************************/
void PtViewer::render()
{
	// Clear framebuffer
	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
	glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	mGraphicsSynthesizer->setClearColor( 255.f * clear_color.x, 255.f * clear_color.y, 255.f * clear_color.z, 255.f * clear_color.w );

	// [1] - Generate PPTBF
	// - check if PPTBF computation is requested (Update UI edition flag)
	//if ( mSynthesizerUpdateRequested )
	{
#if 1
		// Launch synthesis
	//	mGraphicsSynthesizer->execute();

		// Update GigaVoxels light position
		mGraphicsSynthesizer->setLightPosition( 5.0f, 5.0f, 5.0f );

		// Retrieve camera parameters
		const glm::mat4 viewMatrix = mCamera->getViewMatrix();
		const glm::mat4 projectionMatrix = mCamera->getProjectionMatrix();
		// Update
		//GLdouble modelViewMatrix[ 16 ];
		//GLdouble projectionMatrix[ 16 ];
		//camera()->getModelViewMatrix( modelViewMatrix );
		//camera()->getProjectionMatrix( projectionMatrix );
		float4x4* MV = &mGraphicsSynthesizer->_modelViewMatrix;
		float4x4* P = &mGraphicsSynthesizer->_projectionMatrix;
		//int4 Viewport;
		//for ( int i = 0; i < 16; i++ )
		int k = 0;
		for ( int c = 0; c < 4; c++ )
			for ( int r = 0; r < 4; r++ )
		{
			//MV->_array[ i ] = static_cast< float >( modelViewMatrix[ i ] );
			MV->_array[ k ] = static_cast< float >( viewMatrix[ c ][ r ] );
			P->_array[ k ] = static_cast< float >( projectionMatrix[ c ][ r ] );
			k++;
		}

		// Render GigaVoxels scene
		mGraphicsSynthesizer->draw();
#else
		mGraphicsSynthesizer->gs_dataStructure_render();
#endif

		// Update UI edition flag
		mSynthesizerUpdateRequested = false;
	}

#if 1
	// [2] - Render scene

	// Cannot draw with a bounded VAO in GL Core profile...?
	if ( mUseGLCoreProfile )
	{
		glBindVertexArray( mGLCoreProfileVAO );
	}

#if 0
	// Render exemplar
	displayExemplar();
#else
	// Render final synthesis result
	displaySynthesis();
#endif

	// DEBUG
	// Display all levels of S poyramid
	//const bool showPyramid = true;
	const bool showPyramid = mShowUI; // hack... TODO: add "key" button
	//const bool showPyramid = false; // hack... TODO: add "key" button
	if ( showPyramid )
	{
		renderHUD();
	}
	
	// Cannot draw with a bounded VAO in GL Core profile...?
	if ( mUseGLCoreProfile )
	{
		glBindVertexArray( 0 );
	}

	// [3] - Render GUI
	renderGUI();
#endif
}

/******************************************************************************
 * Render HUD
 ******************************************************************************/
void PtViewer::renderHUD()
{
	GLint last_viewport[ 4 ];
	glGetIntegerv( GL_VIEWPORT, last_viewport );
	glDisable( GL_DEPTH_TEST );

	// Display exemplar pyramid
	if ( mDisplayExemplarPyramidInfo )
	{
		displayExemplarPyramid();
	}

	// Display synthesis pyramid color
	if ( mDisplaySynthesisPyramidColorInfo )
	{
		displaySynthesisPyramidColor();
	}

	// Display synthesis pyramid position
	if ( mDisplaySynthesisPyramidPositionInfo )
	{
		displaySynthesisPyramidPosition();
	}

	// Display distance map pyramid
	if ( mDisplayDistanceMapPyramidInfo )
	{
		displayDistanceMapPyramid();
	}

	// Reset GL state(s)
	glViewport( last_viewport[ 0 ], last_viewport[ 1 ], static_cast< GLsizei >( last_viewport[ 2 ] ), static_cast< GLsizei >( last_viewport[ 3 ] ) );
	glEnable( GL_DEPTH_TEST );
}

/******************************************************************************
 * Display Exemplar
 ******************************************************************************/
void PtViewer::displayExemplar()
{
	//// Set shader program
	//PtShaderProgram* shaderProgram = mGraphicsSynthesizer->editPPTBFViewerShaderProgram();
	//shaderProgram->use();
	//{
	//	// Set uniform(s)
	//	shaderProgram->set( 0, "uTexture" );

	//	// Set texture(s)
	//	const GLuint* const exemplarTextureID = mGraphicsSynthesizer->getExemplarTextureID();
	//	glBindTextureUnit( 0/*unit*/, *exemplarTextureID );

	//	// Set sampler(s)
	//	// - E map (i.e. exemplar) at coarsest resolution
	//	glBindSampler( 0/*unit*/, mGraphicsSynthesizer->mSampler_clampToEdge_nearest );
	//		
	//	// Draw command(s)
	//	glDrawArrays( GL_TRIANGLES, 0, 3 );
	//}
	//// Reset GL state(s)
	//PtShaderProgram::unuse();
}

/******************************************************************************
 * Display final synthesis result
 ******************************************************************************/
void PtViewer::displaySynthesis()
{
	
}

/******************************************************************************
 * Display exemplar pyramid
 ******************************************************************************/
void PtViewer::displayExemplarPyramid()
{
	
}

/******************************************************************************
 * Display synthesis pyramid color
 ******************************************************************************/
void PtViewer::displaySynthesisPyramidColor()
{
	
}

/******************************************************************************
 * Display synthesis pyramid position
 ******************************************************************************/
void PtViewer::displaySynthesisPyramidPosition()
{

}

/******************************************************************************
 * Display Distance Map
 ******************************************************************************/
void PtViewer::displayDistanceMap()
{
	
}

/******************************************************************************
 * Display distance map pyramid
 ******************************************************************************/
void PtViewer::displayDistanceMapPyramid()
{
	
}

/******************************************************************************
 * Render GUI
 ******************************************************************************/
void PtViewer::renderGUI()
{
	// Check whether or not to display the user interface
	bool uiFrame_ModelTransform_show = true;
	bool uiFrame_Deformations_NonStationarity_and_Mixtures_show = true;
	bool uiFrame_PointProcess_show = true;
	bool uiFrame_WindowFunction_show = true;
	bool uiFrame_FeatureFunction_show = true;
	bool uiFrame_TextureSynthesis_show = true;
	bool uiFrame_GlobalSettings_show = true;
	bool uiFrame_FPS_show = true;
	bool uiFrame_Performance_show = true;
	bool uiFrame_BinaryStructureMap_show = true;

	// ImGui new frame
	ImGui_ImplGlfwGL3_NewFrame();

	bool show_app_fixed_overlay = true;
	if ( show_app_fixed_overlay )
	{
		showExampleAppFixedOverlay( &show_app_fixed_overlay );
	}

	//// Data structure
	//bool show_dataStructureInfo = true;
	//ImGui::Begin( "Data Structure", &show_dataStructureInfo );
	//{
	//	int nbLevels = mGraphicsSynthesizer->get_dataStructure_MaxDepth();
	//	if ( ImGui::SliderInt( "nb levels", &nbLevels, 0, 31/*use max value from GS library instead*/ ) )
	//	{
	//		mGraphicsSynthesizer->set_dataStructure_MaxDepth( nbLevels );
	//	}

	//	bool displayMode = mGraphicsSynthesizer->get_dataStructure_DisplayMode();
	//	if ( ImGui::Checkbox( "display", &displayMode ) )
	//	{
	//		mGraphicsSynthesizer->set_dataStructure_DisplayMode( displayMode );
	//	}
	//}
	//ImGui::End();

	//// Renderer
	//bool show_RendererInfo = true;
	//ImGui::Begin( "Renderer", &show_RendererInfo );
	//{
	//	float material_opacity = mGraphicsSynthesizer->gs_get_material_opacity();
	//	if ( ImGui::SliderFloat( "opacity length", &material_opacity, 0.f, 4096.f ) )
	//	{
	//		mGraphicsSynthesizer->gs_set_material_opacity( material_opacity );
	//	}

	//	bool renderer_hasPriorityOnBricks = mGraphicsSynthesizer->gs_renderer_hasPriorityOnBricks();
	//	if ( ImGui::Checkbox( "priority on bricks", &renderer_hasPriorityOnBricks ) )
	//	{
	//		mGraphicsSynthesizer->gs_renderer_setPriorityOnBricks( renderer_hasPriorityOnBricks );
	//	}
	//}
	//ImGui::End();

	//// Cache
	//bool show_CacheInfo = true;
	//ImGui::Begin( "Cache", &show_CacheInfo );
	//{
	//	/*float material_opacity = mGraphicsSynthesizer->gs_get_material_opacity();
	//	if ( ImGui::SliderFloat( "opacity length", &material_opacity, 0.f, 4096.f ) )
	//	{
	//		mGraphicsSynthesizer->gs_set_material_opacity( material_opacity );
	//	}*/
	//}
	//ImGui::End();

	// Pipeline
	bool show_PipelineInfo = true;
	ImGui::Begin( "Pipeline", &show_PipelineInfo );
	{
		bool pipeline_dynamic_update = mGraphicsSynthesizer->gs_pipeline_get_dynamic_update();
		if ( ImGui::Checkbox( "dynamic update", &pipeline_dynamic_update ) )
		{
			mGraphicsSynthesizer->gs_pipeline_set_dynamic_update( pipeline_dynamic_update );
		}

		// Cache
		if ( ImGui::TreeNode( "Cache" ) )
		{
			if ( ImGui::Button( "clear" ) )
			{
				mGraphicsSynthesizer->clearCache();
			}

			ImGui::TreePop();
		}

		// Data Structure
		if ( ImGui::TreeNode( "Data Structure" ) )
		{
			int nbLevels = mGraphicsSynthesizer->get_dataStructure_MaxDepth();
			if ( ImGui::SliderInt( "nb levels", &nbLevels, 0, 31/*use max value from GS library instead*/ ) )
			{
				mGraphicsSynthesizer->set_dataStructure_MaxDepth( nbLevels );
			}

			bool displayMode = mGraphicsSynthesizer->get_dataStructure_DisplayMode();
			if ( ImGui::Checkbox( "display", &displayMode ) )
			{
				mGraphicsSynthesizer->set_dataStructure_DisplayMode( displayMode );
			}

			ImGui::TreePop();
		}

		// Renderer
		if ( ImGui::TreeNode( "Renderer" ) )
		{
			float material_opacity = mGraphicsSynthesizer->gs_get_material_opacity();
			if ( ImGui::SliderFloat( "opacity length", &material_opacity, 0.f, 4096.f ) )
			{
				mGraphicsSynthesizer->gs_set_material_opacity( material_opacity );
			}

			bool renderer_hasPriorityOnBricks = mGraphicsSynthesizer->gs_renderer_hasPriorityOnBricks();
			if ( ImGui::Checkbox( "priority on bricks", &renderer_hasPriorityOnBricks ) )
			{
				mGraphicsSynthesizer->gs_renderer_setPriorityOnBricks( renderer_hasPriorityOnBricks );
			}

			ImGui::TreePop();
		}

		// Producer
		if ( ImGui::TreeNode( "Producer" ) )
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();

	// Render HUD overlay (user interface)
	ImGui::Render();
}

/******************************************************************************
 * Demonstrate creating a simple static window with no decoration + a context-menu to choose which corner of the screen to use.
 ******************************************************************************/
void PtViewer::showExampleAppFixedOverlay( bool* p_open )
{
	const float DISTANCE = 10.0f;
	
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background

	static int corner = 2;
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	if (ImGui::Begin("Mouse Info", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		//ImGui::Text("Simple overlay\nin the corner of the screen.\n(right-click to change position)");
		//ImGui::Separator();
		//ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
		ImGui::Text("Mouse Position: (%d,%d)", static_cast< int >( ImGui::GetIO().MousePos.x ), static_cast< int >( ImGui::GetIO().MousePos.y ) );
		/*if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			ImGui::EndPopup();
		}*/
		ImGui::End();
	}

	static int corner2 = 1;
	window_pos = ImVec2((corner2 & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner2 & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner2 & 1) ? 1.0f : 0.0f, (corner2 & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	if (ImGui::Begin("Synthesis Detailed Info", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::Text( "Profiler" );
		ImGui::Separator();
		//ImGui::Text(const_cast< char* >( mGraphicsSynthesizer->getSynthesisInfo().str().c_str() ), static_cast< int >(ImGui::GetIO().MousePos.x), static_cast< int >(ImGui::GetIO().MousePos.y));
		
		ImGui::End();
	}

	int width, height;
	glfwGetWindowSize( mWindow, &width, &height );
	//glfwGetFramebufferSize( mWindow, &width, &height );
	static int corner3 = 0;
	window_pos = ImVec2((corner3 & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner3 & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	window_pos_pivot = ImVec2((corner3 & 1) ? 1.0f : 0.0f, (corner3 & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	if (ImGui::Begin("Example: Fixed Overlay", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::Text( "Performance" );
		ImGui::Separator();
		ImGui::Text( "FPS         %.1f", ImGui::GetIO().Framerate );
		ImGui::Text( "Frame       %.3f ms", 1000.0f / ImGui::GetIO().Framerate );
		ImGui::Text( "Window      %dx%d", width, height );
		ImGui::Separator();
	/*	ImGui::Text( "Exemplar     %dx%d", mGraphicsSynthesizer->getExemplarWidth(), mGraphicsSynthesizer->getExemplarHeight() );
		ImGui::Text( "Output       %dx%d", mGraphicsSynthesizer->getWidth(), mGraphicsSynthesizer->getHeight() );
		ImGui::Text( "Synthesis    %5.3f ms", mGraphicsSynthesizer->getSynthesisTime() );
*/
		// Graphics library info
	/*	printf("\n[Renderer Info]\n");
		printf("\tGL Vendor: %s\n", mGLVendor);
		printf("\tGL Renderer: %s\n", mGLRenderer);
		printf("\tGL Version (string): %s\n", mGLVersion);
		printf("\tGL Version (integer): %d.%d\n", mGLMajorVersion, mGLMinorVersion);
		printf("\tGLSL Version: %s\n", mGLShadingLanguageVersion);*/
		ImGui::Text( "Graphics Library [OpenGL]" );
		ImGui::Separator();
		ImGui::Text( "Vendor      %s", mGLVendor.data() );
		ImGui::Text( "Renderer    %s", mGLRenderer.data() );
		ImGui::Text( "Version     %s", mGLVersion.data() );
		ImGui::Text( "Shader      %s", mGLShadingLanguageVersion.data() );

		//ImGui::Separator();

		// End widget
		ImGui::End();
	}

	ImGui::PopStyleColor();
}

/******************************************************************************
 * ...
 ******************************************************************************/
void PtViewer::synthesize()
{
}

/******************************************************************************
 * Slot called when the data model has been modified
 ******************************************************************************/
void PtViewer::onDataModelModified()
{
}

/******************************************************************************
 * Handle requests
 ******************************************************************************/
void PtViewer::handleRequests()
{
}
