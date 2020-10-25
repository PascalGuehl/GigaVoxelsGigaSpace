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

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>
#include <string>

// System
#include <cstdlib>
#include <ctime>

// Project
#include "PtApplication.h"

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
 * Main entry program
 *
 * @param pArgc Number of arguments
 * @param pArgv List of arguments
 *
 * @return flag telling whether or not it succeeds
 ******************************************************************************/
int main( int pArgc, char** pArgv )
{
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "- Pixel-based Texture Synthesizer on GPU: Console Tool --" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;

	PtGUI::PtApplication::get();
	PtGUI::PtApplication::get().initialize();
	PtGUI::PtApplication::get().execute();
	PtGUI::PtApplication::get().finalize();
	// todo: add a GLOBAL static finalize();

	return 0;
}
