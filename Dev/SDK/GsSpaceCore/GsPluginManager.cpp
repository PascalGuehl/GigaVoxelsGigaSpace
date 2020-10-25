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

#include "GsSpaceCore/GsPluginManager.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// GvViewer
#include "GsSpaceCore/GsPluginInterface.h"

// System
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

// Dynamic library
#ifdef _WIN32
	//#include <windows.h>
#else
	#include <dirent.h>
	#include <dlfcn.h>
#endif

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GsCore;

// STL
//using std::string;
using namespace std;

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/**
 * The unique instance of the singleton.
 */
GsPluginManager* GsPluginManager::msInstance = NULL;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

#ifdef _WIN32

#else

/******************************************************************************
 * gvPluginfilter()
 *
 * @param pDir
 *
 * @return
 ******************************************************************************/
int gvPluginfilter( const struct dirent* pDirent )
{
    //** GvPlugin's name end with ".gvp"
    int lLength = strlen( pDirent->d_name );
    if ( lLength < 4 )
    {
        return 0;
    }

    const char* lName = pDirent->d_name;
    if ( lName[ --lLength ] == 'p' && lName[ --lLength ] == 'v' && lName[ --lLength ] == 'g' && lName[ --lLength ] == '.' )
    {
        return 1;
    }

    return 0;
}

#endif

/******************************************************************************
 * GsPluginManager()
 ******************************************************************************/
GsPluginManager::GsPluginManager()
{
}

/******************************************************************************
 * getInstance()
 *
 * @return
 ******************************************************************************/
GsPluginManager& GsPluginManager::get()
{
    if ( msInstance == NULL )
    {
        msInstance = new GsPluginManager();
    }

    return *msInstance;
}

/******************************************************************************
 * loadPlugins()
 *
 * @param pDir
 ******************************************************************************/
void GsPluginManager::loadPlugins( const string& pDir )
{
    vector< string > lFilenames;
    getFilenames( pDir, lFilenames );

    //   cout << "Nombre de plugins potentiels trouvés : " << lFilenames.size() << endl;

    vector< string >::const_iterator it;
    for ( it = lFilenames.begin(); it != lFilenames.end(); ++it )
    {
        const string& lFilename = *it;
       // const string& lFullName = pDir + string( "\\" ) + lFilename;
        const string& lFullName = pDir + string( "/" ) + lFilename;
        loadPlugin( lFullName );
    }
}

/******************************************************************************
 * getFilenames()
 *
 * @param pDir
 * @param pFilenames
 ******************************************************************************/
void GsPluginManager::getFilenames( const string& pDir, vector< string >& pFilenames ) const
{
#ifdef _WIN32

#else

    struct dirent** lNamelist;

    int lNbEntries = scandir( pDir.c_str(), &lNamelist, gvPluginfilter, alphasort );
    if ( lNbEntries < 0 )
    {
      //        cout << "GsPluginManager::getFilenames : Error while using scandir() function." << endl;
    }
    else
    {
        while ( lNbEntries-- )
        {
	  //    printf( "%s\n", lNamelist[ lNbEntries ]->d_name );
            pFilenames.push_back( string( lNamelist[ lNbEntries ]->d_name ) );
           
            free( lNamelist[ lNbEntries ] );
        }
        free( lNamelist );
    }

#endif
}

/******************************************************************************
 * loadPlugin()
 *
 * @param pFilename
 *
 * @return
 ******************************************************************************/
bool GsPluginManager::loadPlugin( const string& pFilename )
{
	GV_CREATE_PLUGIN lFunc;			// Function pointer

#ifdef _WIN32

	HINSTANCE lHandle = LoadLibrary( pFilename.c_str() );
	if ( lHandle == NULL )
	{
		return false;
	}
	
	lFunc = (GV_CREATE_PLUGIN)GetProcAddress( lHandle, "createPlugin" );
	if ( ! lFunc )
	{
		// Handle the error
		FreeLibrary( lHandle );

		// return SOME_ERROR_CODE;
		return false;
	}
	
#else

    char* lError;

    //  cout << "\tTRY dlopen() : " << pFilename.c_str() << endl;
    void* lHandle = dlopen( pFilename.c_str(), RTLD_LAZY );
    if ( lHandle == NULL )
    {
		lError = dlerror();	
		if ( lError != NULL )
		{
	  		cout << lError << endl;
		}

        return false;
    }

    //    cout << "\tTRY dlsym( createPlugin )" << endl;
    dlerror();	//** Clear any existing error
    //GV_CREATE_PLUGIN lFunc;
			// double (*cosine)(double);
			/* Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
              would seem more natural, but the C99 standard leaves
              casting from "void *" to a function pointer undefined.
              The assignment used below is the POSIX.1-2003 (Technical
              Corrigendum 1) workaround; see the Rationale for the
              POSIX specification of dlsym(). */

  //  GV_CREATE_PLUGIN lFunc = (GV_CREATE_PLUGIN)dlsym( lHandle, "createPlugin" );
    *(void **) (&lFunc) = dlsym( lHandle, "createPlugin" );
	lError = dlerror();	
	if ( lError != NULL )
	{
	        cout << lError << endl;
	}
    if ( lFunc == NULL )
	{
		// cout << "\tSymbol createPlugin introuvable..." << endl;

		return false;
    }

#endif

	//** Call the function
    //    cout << "\tTRY createPlugin()" << endl;
    GsPluginInterface* lPlugin = lFunc( *this );
    if ( lPlugin == NULL )
    {
        return false;
    }
    cout << "Plugin chargé : " << lPlugin->getName().c_str() << endl;

	//** Store plugin info
    PluginInfo lInfo;
    lInfo.mPlugin = lPlugin;
    lInfo.mHandle = lHandle;
    mPlugins.push_back( lInfo );

    return true;
}

/******************************************************************************
 * unloadAll()
 ******************************************************************************/
void GsPluginManager::unloadAll()
{
    vector< PluginInfo >::iterator it;
    for ( it = mPlugins.begin(); it != mPlugins.end(); ++it )
    {
        PluginInfo& lPluginInfo = *it;

        delete lPluginInfo.mPlugin;

        //** Unload library
#ifdef _WIN32
		FreeLibrary( lPluginInfo.mHandle );
#else
		dlerror();    //** Clear any existing error
		dlclose( lPluginInfo.mHandle );
		char* lError = dlerror();
		if ( lError != NULL )
		{
			cout << lError << endl;
		}
#endif
	}

    mPlugins.clear();
}

/******************************************************************************
 * getNbPlugins()
 *
 * @return
 ******************************************************************************/
size_t GsPluginManager::getNbPlugins() const
{
    return mPlugins.size();
}

/******************************************************************************
 * getNbPlugins()
 *
 * @param pIndex
 *
 * @return
 ******************************************************************************/
GsPluginInterface* GsPluginManager::getPlugin( int pIndex )
{
    if ( pIndex < 0 || pIndex >= getNbPlugins() )
    {
        return NULL;
    }

    return mPlugins[ pIndex ].mPlugin;
}
