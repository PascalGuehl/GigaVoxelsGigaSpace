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

#ifndef _PT_APPLICATION_H_
#define _PT_APPLICATION_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Project
#include "PtViewerConfig.h"
//#include "PtPipelineManager.h"

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

// Project
//namespace PtGraphics
//{
//	class PtGraphicsSynthesizer;
//}
namespace PtGUI
{
	class PtViewer;
}

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

namespace PtGUI
{

/** 
 * @class PtApplication
 *
 * @brief The PtApplication class provides ...
 *
 * ...
 */
class PtApplication
{
	
	/**************************************************************************
	 ***************************** PUBLIC SECTION *****************************
	 **************************************************************************/

public:

	/******************************* INNER TYPES *******************************/

	/**
	 * Type of pipeline request
	 */
	enum PtPipelineRequest
	{
		eLoadDataModel,
		eSynthesizeTexture,
		eUpdateTexture,
		eRequestNbTypes
	};

	/******************************* ATTRIBUTES *******************************/

	/******************************** METHODS *********************************/

	/**
	 * Get the unique instance.
	 *
	 * @return the unique instance
	 */
	static PtApplication& get();

	/**
	 * Destructor.
	 */
	virtual ~PtApplication();

	/**
	 * Initialize
	 */
	virtual void initialize();

	/**
	 * Finalize
	 */
	virtual void finalize();

	/**
	 * Execute
	 * - main event loop
	 */
	virtual void execute();

	/**
	 * Post a request
	 */
	void postRequest( PtPipelineRequest pRequest );

	/**
	 * Get synthesizer
	 */
	//PtGraphics::PtGraphicsSynthesizer* getSynthesizer();

	/**************************************************************************
	 **************************** PROTECTED SECTION ***************************
	 **************************************************************************/

protected:
	
	/******************************* ATTRIBUTES *******************************/

	/**
	 * Main window
	 */
	PtViewer* mMainWindow;

	/**
	 * Requests
	 */
	bool mOmniScaleModelUpdateRequested;
	bool mTextureSynthesisRequested;

	/******************************** METHODS *********************************/

	/**
	 * Initialize GLFW
	 */
	virtual void initializeGLFW();

	/**
	 * Finalize GLFW
	 */
	virtual void finalizeGLFW();

	/**
	 * Initialize windows
	 */
	virtual void initializeWindows();

	/**
	 * Initialize GL library
	 */
	virtual void initializeGL();

	/**
	 * Initialize ImGui user interface
	 */
	virtual void initializeImGuiUserInterface();

	///**
	// * Process Events
	// */
	//virtual void processEvents();

	/**
	 * Handle requests
	 */
	virtual void handleRequests();

	/**
	 * GLFW error callback
	 */
	static void error_callback( int error, const char* description );

	/**************************************************************************
	 ***************************** PRIVATE SECTION ****************************
	 **************************************************************************/

private:
	
	/******************************* ATTRIBUTES *******************************/
	
	/**
	 * The unique instance
	 */
	static PtApplication* msInstance;

	/******************************** METHODS *********************************/

	/**
	 * Constructor
	 */
	PtApplication();
};

}

#endif
