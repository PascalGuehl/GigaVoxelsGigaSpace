///*
// * Publication: By-Example Point Process Texture Basis Functions
// * Authors: anonymous
// *
// * Code author: anonymous
// *
// * ICube laboratory (the Engineering science, computer science and imaging laboratory) [https://icube.unistra.fr/en/]
// * IGG group (Computer Graphics and Geometry) [http://icube-igg.unistra.fr/en/index.php/Main_Page]
// */
//
///** 
// * @version 1.0
// */
//
//#ifndef _OS_TEXTURE_SYNTHESIZER_VIEWER_H_
//#define _OS_TEXTURE_SYNTHESIZER_VIEWER_H_
//
///******************************************************************************
// ******************************* INCLUDE SECTION ******************************
// ******************************************************************************/
//
//// System
//#ifdef WIN32
//	#ifndef WIN32_LEAN_AND_MEAN
//		#define WIN32_LEAN_AND_MEAN
//	#endif
//	#include <windows.h>
//#endif
//
//// GLFW
//#include <glad/glad.h>
////#include <GLFW/glfw3.h>
//
//// Project
//#include "PtViewerConfig.h"
//#include "PtCamera.h"
//
//// Project
////#include "SampleCore.h"
//
//// STL
//#include <string>
//
///******************************************************************************
// ************************* DEFINE AND CONSTANT SECTION ************************
// ******************************************************************************/
//
///******************************************************************************
// ***************************** TYPE DEFINITION ********************************
// ******************************************************************************/
//
///******************************************************************************
// ******************************** CLASS USED **********************************
// ******************************************************************************/
//
//struct GLFWwindow;
//
////// Project
////namespace PtGraphics
////{
////	class PtGraphicsSynthesizer;
////}
//
//// Project
//class SampleCore;
//
///******************************************************************************
// ****************************** CLASS DEFINITION ******************************
// ******************************************************************************/
//
//namespace PtGUI
//{
//
///** 
// * @class PtViewer
// *
// * @brief The PtViewer class provides ...
// *
// * ...
// */
//class PtViewer
//{
//
//	/**************************************************************************
//	 ***************************** PUBLIC SECTION *****************************
//	 **************************************************************************/
//
//public:
//
//	/******************************* ATTRIBUTES *******************************/
//
//	/**
//	 * Request: texture synthesis
//	 */
//	bool mLaunchRequest;
//
//	/**
//	 * Request: texture synthesis
//	 */
//	bool mRequestLabelMapViewer;
//
//	/**
//	 * ...
//	 */
//	void synthesize();
//
//	// Main variables
//
//	// Exemplar
//	std::string uiModelFilename;
//	std::string uiDistanceFilename;
//	std::string uiLabelFilename;
//	// Guidance
//	std::string uiGuidanceDistanceFilename;
//	std::string uiGuidanceLabelFilename;
//	std::string uiGuidanceMaskFilename;
//
//	bool uiTextureSynthesis_launch = false;
//
//	/**
//	 * Flag to tell whether or not to display the user interface
//	 */
//	bool mShowUI;
//
//	/******************************** METHODS *********************************/
//
//	/**
//	 * Constructor
//	 *
//	 * @param pTextureSynthesizer texture synthesizer manager
//	 */
//	PtViewer();
//	
//	/**
//	 * Destructor
//	 */
//	virtual ~PtViewer();
//
//	/**
//	 * Finalize
//	 */
//	void finalize();
//
//	/**
//	 * Render
//	 */
//	void render();
//
//	/**
//	 * Graphics window
//	 */
//	GLFWwindow* getWindow();
//
//	/**
//	 * Get synthesizer
//	 */
//	SampleCore* getSynthesizer();
//
//	/**
//	 * ...
//	 */
//	static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods );
//	 /**
//	 * ...
//	 */
//	static void cursor_position_callback( GLFWwindow* window, double xpos, double ypos );
//
//	/**
//	 * Initialize Window
//	 */
//	void initializeWindow();
//
//	/**
//	 * Initialize GL
//	 */
//	void initializeGL();
//
//	/**
//	 * Handle requests
//	 */
//	void handleRequests();
//
//	/**
//	 * Refresh
//	 */
//	void refresh();
//
//	// Exemplar
//	void displayExemplar();
//	void displayExemplarPyramid();
//	// Synthesis
//	void displaySynthesis();
//	void displaySynthesisPyramidColor();
//	void displaySynthesisPyramidPosition();
//	// Distance map
//	void displayDistanceMap();
//	void displayDistanceMapPyramid();
//
//	/**
//	 * Initialize ImGui user interface
//	 */
//	void initializeImGuiUserInterface();
//
//	/**************************************************************************
//	 **************************** PROTECTED SECTION ***************************
//	 **************************************************************************/
//
//protected:
//	
//	/******************************* ATTRIBUTES *******************************/
//
//	/**
//	 * Graphics window
//	 */
//	GLFWwindow* mWindow;
//
//	/**
//	 * Camera
//	 */
//	PtGraphics::PtCamera* mCamera;
//
//	///**
//	// * OpenGL and GLSL versions
//	// */
//	//const GLubyte* mVendor;
//	//const GLubyte* mRenderer;
//	//const GLubyte* mVersion;
//	//const GLubyte* mShadingLanguageVersion;
//	//GLint mMajorVersion;
//	//GLint mMinorVersion;
//
//	/**
//	 * User interface parameters
//	 */
//	// - Point process parameters
//	int uiPointProcess_tilingType;
//	float uiPointProcess_cellSubdivisionProbability;
//	int uiNNSearch_depth;
//	int uiNNSearch_nbSamples;
//	int uiNNSearch_radius;
//	int uiNNSearch_patchSize;
//	int uiBlockInitialization_blockSize;
//	int uiNbCorrectionPasses;
//	int uiCorrectionSubBlockSize;
//	int uiPyramid_maxLevel;
//	int uiPyramid_minSize;
//	float uiDistance_weight;
//	float uiLabel_weight;
//	
//	/**
//	 * Graphics objects
//	 */
//	SampleCore* mGraphicsSynthesizer;
//
//	/**
//	 * UI edition flag
//	 */
//	bool mSynthesizerUpdateRequested;
//
//	/**
//	 * UI edition flag
//	 */
//	bool mHistogramUpdateRequested;
//
//	/**
//	 * UI edition flag
//	 */
//	bool mUIShowHistogram;
//
//	// Display debug info
//	bool mDisplayExemplarPyramidInfo;
//	bool mDisplaySynthesisPyramidColorInfo;
//	bool mDisplaySynthesisPyramidPositionInfo;
//	bool mDisplayDistanceMapPyramidInfo;
//
//	/**
//	 * GL Core profile parameters
//	 */
//	bool mUseGLCoreProfile;
//	GLuint mGLCoreProfileVAO;
//
//	/**
//	 * Graphics library info
//	 */
//	std::string mGLVendor;
//	std::string mGLRenderer;
//	std::string mGLVersion;
//	std::string mGLShadingLanguageVersion;
//	int mGLMajorVersion;
//	int mGLMinorVersion;
//
//	/******************************** METHODS *********************************/
//
//	/**
//	 * ...
//	 */
//	static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods );
//
//	/**
//	 * ...
//	 */
//	static void window_close_callback( GLFWwindow* window );
//
//	/**
//	 * ...
//	 */
//	static void window_size_callback( GLFWwindow* window, int width, int height );
//
//	/**
//	 * ...
//	 */
//	static void framebuffer_size_callback( GLFWwindow* window, int width, int height );
//
//	/**
//	 * ...
//	 */
//	static void window_pos_callback( GLFWwindow* window, int xpos, int ypos );
//
//	///**
//	// * ...
//	// */
//	//static void cursor_position_callback( GLFWwindow* window, double xpos, double ypos );
//
//	///**
//	// * ...
//	// */
//	//static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods );
//
//	/**
//	 * ...
//	 */
//	static void character_callback( GLFWwindow* window, unsigned int codepoint );
//
//	/**
//	 * ...
//	 */
//	void processMouseMoveEvent( double xpos, double ypos );
//
//	/**
//	 * ...
//	 */
//	void processMouseButtonEvent( int button, int action, int mods );
//
//	/**
//	 * ...
//	 */
//	static void scroll_callback( GLFWwindow* window, double xoffset, double yoffset );
//
//	/**
//	 * ...
//	 */
//	void processScrollEvent( double xoffset, double yoffset );
//
//	/**
//	 * Slot called when the data model has been modified
//	 */
//	virtual void onDataModelModified();
//
//	/**
//	 * ...
//	 */
//	void onSizeModified( const int pWidth, const int pHeight );
//
//	/**
//	 * ...
//	 */
//	void onKeyModified( const int key, const int scancode, const int action, const int mods );
//
//	/**
//	 * Render HUD
//	 */
//	void renderHUD();
//
//	/**
//	 * Render GUI
//	 */
//	void renderGUI();
//
//	/**************************************************************************
//	 ***************************** PRIVATE SECTION ****************************
//	 **************************************************************************/
//
//private:
//	
//	/******************************* ATTRIBUTES *******************************/
//	
//	/******************************** METHODS *********************************/
//
//	void showExampleAppFixedOverlay( bool* p_open );
//
//};
//
//}
//
//#endif
