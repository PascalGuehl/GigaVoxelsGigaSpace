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

#include "PtApplication.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

//#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
//#include <windows.h>

// STL
#include <iostream>

// OpenGL
#include <glad/glad.h>

#include <GLFW/glfw3.h>

//// ImGui
//#include <imgui.h>
//#include "imgui_impl_glfw_gl3.h"

// Project
//#include "SampleCore.h"
#include "PtViewer.h"

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

 // Project
//using namespace Pt;
using namespace PtGraphics;
using namespace PtGUI;

// STL
using namespace std;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * The unique instance of the singleton.
 */
PtApplication* PtApplication::msInstance = nullptr;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Get the unique instance.
 *
 * @return the unique instance
 ******************************************************************************/
PtApplication& PtApplication::get()
{
    if ( msInstance == nullptr )
    {
        msInstance = new PtApplication();
    }

    return *msInstance;
}

/******************************************************************************
 * Default constructor
 ******************************************************************************/
PtApplication::PtApplication()
:	mMainWindow( nullptr )
{
	// User interface
	mMainWindow = new PtViewer();
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
PtApplication::~PtApplication()
{
	// User interface
	delete mMainWindow;
}

/******************************************************************************
 * Initialize
 ******************************************************************************/
void PtApplication::initialize()
{
	// Initialize the GLFW library
	initializeGLFW();

	// Initialize windows
	// - at least, create one graphics context
	initializeWindows();

	// Initialize GL library
	initializeGL();
	
	// Initialize user interface
	//initializeImGuiUserInterface(); // already done in MainWindow...
}

/******************************************************************************
 * Finalize
 ******************************************************************************/
void PtApplication::finalize()
{
	// Finalize the GLFW library
	finalizeGLFW();
}

/******************************************************************************
 * Initialize GLFW
 ******************************************************************************/
void PtApplication::initializeGLFW()
{
	// Initialize GLFW library
	if ( ! glfwInit() )
	{
		// Initialization failed
		exit( EXIT_FAILURE );
	}

	// Set error callback
	glfwSetErrorCallback( error_callback );
}

/******************************************************************************
 * Finalize GLFW
 ******************************************************************************/
void PtApplication::finalizeGLFW()
{
	glfwTerminate();
}

/******************************************************************************
 * GLFW error callback
 ******************************************************************************/
void PtApplication::error_callback( int error, const char* description )
{
	fprintf( stderr, "Error: %s\n", description );
}

/******************************************************************************
 * Initialize windows
 ******************************************************************************/
void PtApplication::initializeWindows()
{
	// Initialize windows
	mMainWindow->initializeWindow();
}

/******************************************************************************
 * Initialize GL library
 ******************************************************************************/
void PtApplication::initializeGL()
{
	// NOTE: the following GLFW functions require a context to be current
	glfwMakeContextCurrent( mMainWindow->getWindow() );

	// Load OpenGL API/extensions
	gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );
	
	// Managing swap interval
	//glfwSwapInterval( 1 );
	glfwSwapInterval( 0 );

	// Initialize
	mMainWindow->initializeGL();
}

/******************************************************************************
* Initialize ImGui user interface
******************************************************************************/
void PtApplication::initializeImGuiUserInterface()
{
	// Need a GL context?
	glfwMakeContextCurrent( mMainWindow->getWindow() );

	//// Initialize ImGui
	//// - GL binding
	//ImGui_ImplGlfwGL3_Init( mMainWindow->getWindow(), true ); // TODO: check callback: erased/overlapped by ImGui: use custom and call ImGui one !!! () => use FALSE !!!!
	//// - style
	//ImGui::StyleColorsClassic();
	////ImGui::StyleColorsDark();

	mMainWindow->initializeImGuiUserInterface();
}

/******************************************************************************
 * Execute
 * - main event loop
 ******************************************************************************/
void PtApplication::execute()
{
	while ( ! glfwWindowShouldClose( mMainWindow->getWindow() ) )
	{
		// Set context
		// NOTE:
		// By default, making a context non-current implicitly forces a pipeline flush
		// On machines that support GL_KHR_context_flush_control, you can control whether a context performs this flush by setting the GLFW_CONTEXT_RELEASE_BEHAVIOR window hint.
		glfwMakeContextCurrent( mMainWindow->getWindow() );

		// Render scene
		mMainWindow->refresh();

		// Handle requests
		// - data model loading, descriptors computation, texture synthesis, painting, etc...
		handleRequests();

		// Process events
		glfwPollEvents();
	}
}

/******************************************************************************
 * Handle requests
 ******************************************************************************/
void PtApplication::handleRequests()
{
}

/******************************************************************************
 * Post a request
 ******************************************************************************/
void PtApplication::postRequest( PtPipelineRequest pRequest )
{
	switch ( pRequest )
	{
		case eLoadDataModel:
			mOmniScaleModelUpdateRequested = true;
			break;

		case eSynthesizeTexture:
			mTextureSynthesisRequested = true;
			break;

		default:
			break;
	}
}

/******************************************************************************
 * Get synthesizer
 ******************************************************************************/
//PtGraphicsSynthesizer* PtApplication::getSynthesizer()
//{
//	return mMainWindow->getSynthesizer();
//}
