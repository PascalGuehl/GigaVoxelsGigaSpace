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

#include "GsGraphics/GsGraphicsCore.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// System
#include <cassert>
#include <cstdio>

// STL
#include <iostream>

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

// GigaVoxels
using namespace GsGraphics;

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
GsGraphicsCore::GsGraphicsCore()
{
	/*glDispatchComputeEXT = (PFNGLDISPATCHCOMPUTEEXTPROC)wglGetProcAddress( "glDispatchCompute" );

	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)wglGetProcAddress( "glTexStorage2D" );*/
}

/******************************************************************************
 * Destructor
 ******************************************************************************/
GsGraphicsCore::~GsGraphicsCore()
{
}

/******************************************************************************
 * Print information about the device
 ******************************************************************************/
void GsGraphicsCore::printInfo()
{
	// Determine the OpenGL and GLSL versions
	const GLubyte* vendor = glGetString( GL_VENDOR );
	const GLubyte* renderer = glGetString( GL_RENDERER );
	const GLubyte* version = glGetString( GL_VERSION );
	const GLubyte* glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major;
	GLint minor;
	glGetIntegerv( GL_MAJOR_VERSION, &major );
	glGetIntegerv( GL_MINOR_VERSION, &minor );
	printf( "\n" );
	printf( "GL Vendor : %s\n", vendor );
	printf( "GL Renderer : %s\n", renderer );
	printf( "GL Version (string) : %s\n", version );
	printf( "GL Version (integer) : %d.%d\n", major, minor );
	printf( "GLSL Version : %s\n", glslVersion );
	
	// TO DO
	// - check for NVX_gpu_memory_info experimental OpenGL extension
	//
	// TO DO
	// - track dedicated real-time parameters
	GLint glGpuMemoryInfoDedicatedVidmemNvx;
	GLint glGpuMemoryInfoTotalAvailableMemoryNvx;
	GLint glGpuMemoryInfoCurrentAvailableVidmemNvx;
	GLint glGpuMemoryInfoEvictionCountNvx;
	GLint glGpuMemoryInfoEvictedMemoryNvx;
	glGetIntegerv( 0x9047, &glGpuMemoryInfoDedicatedVidmemNvx );
	glGetIntegerv( 0x9048, &glGpuMemoryInfoTotalAvailableMemoryNvx );
	glGetIntegerv( 0x9049, &glGpuMemoryInfoCurrentAvailableVidmemNvx );
	glGetIntegerv( 0x904A, &glGpuMemoryInfoEvictionCountNvx );
	glGetIntegerv( 0x904B, &glGpuMemoryInfoEvictedMemoryNvx );
	std::cout << "\nNVIDIA Memory Status" << std::endl;
	std::cout << "- GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX : " << glGpuMemoryInfoDedicatedVidmemNvx << " kB" <<  std::endl;
	std::cout << "- GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX : " << glGpuMemoryInfoTotalAvailableMemoryNvx << " kB" <<  std::endl;
	std::cout << "- GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX : " << glGpuMemoryInfoCurrentAvailableVidmemNvx << " kB" <<  std::endl;
	std::cout << "- GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX : " << glGpuMemoryInfoEvictionCountNvx << std::endl;
	std::cout << "- GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX : " << glGpuMemoryInfoEvictedMemoryNvx << " kB" << std::endl;

	// Compute Shaders
	std::cout << "\nOpenGL Compute Shader features" << std::endl;
	GLint glMAXCOMPUTEWORKGROUPINVOCATIONS;
	glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &glMAXCOMPUTEWORKGROUPINVOCATIONS );
	std::cout << "- GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS : " << glMAXCOMPUTEWORKGROUPINVOCATIONS << std::endl;
	/*GLint glMAXCOMPUTEWORKGROUPCOUNT[3];
	glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_COUNT, glMAXCOMPUTEWORKGROUPCOUNT );
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_COUNT : " << glMAXCOMPUTEWORKGROUPCOUNT[ 0 ] << " - " << glMAXCOMPUTEWORKGROUPCOUNT[ 1 ] << " - " << glMAXCOMPUTEWORKGROUPCOUNT[ 2 ] << std::endl;
	GLint glMAXCOMPUTEWORKGROUPSIZE[3];
	glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_SIZE, glMAXCOMPUTEWORKGROUPSIZE );
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE : " << glMAXCOMPUTEWORKGROUPSIZE[ 0 ] << " - " << glMAXCOMPUTEWORKGROUPSIZE[ 1 ] << " - " << glMAXCOMPUTEWORKGROUPSIZE[ 2 ] << std::endl;
*/
	GLint cx, cy, cz;
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cx );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &cy );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &cz );
	//fprintf( stderr, "Max Compute Work Group Count = %5d, %5d, %5d\n", cx, cy, cz );
	std::cout << "- GL_MAX_COMPUTE_WORK_GROUP_COUNT : " << cx << " - " << cy << " - " << cz << std::endl;

	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &cx );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &cy );
	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &cz );
	std::cout << "- GL_MAX_COMPUTE_WORK_GROUP_SIZE : " << cx << " - " << cy << " - " << cz << std::endl;
}
