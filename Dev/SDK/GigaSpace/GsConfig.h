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

#ifndef _GS_CONFIG_H_
#define _GS_CONFIG_H_

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

#define RENDER_USE_SWIZZLED_THREADS 1

#define USE_TESLA_OPTIMIZATIONS 0

// ---------------- PERFORMANCE MONITOR ----------------

/**
 * Performance monitor
 *
 * By default, the performance monitor is not used
 */
//#define USE_CUDAPERFMON 1

// ---------------- RENDERING ----------------

/**
 * Flag to tell wheter or not to use a dedicated stream for renderer
 */
//#define _GS_RENDERER_USE_STREAM_

// ---------------- DATA PRODUCTION MANAGEMENT ----------------

//#define GV_USE_PRODUCTION_OPTIMIZATION
#define GV_USE_PRODUCTION_OPTIMIZATION_INTERNAL

// ---------------- PIPELINE MANAGEMENT ----------------

/**
 * Optimization to use non-blocking calls to CUDA functions in the default CUDA NULL stream
 * allowing better host/device concurrency.
 *
 * TO DO : use only one "#define"
 */
//#define GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_GVCACHEMANAGER
//#define GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_DATAPRODUCTIONMANAGER
//#define GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_PRODUCER

/**
 * Reduce driver overhead and allow better host/device concurrency
 * by combining many calls to cudaMemcpyAsync() in a unique one.
 */
//#define GS_USE_OPTIMIZED_NON_BLOCKING_ASYNCHRONOUS_CALLS_PIPELINE_WITH_COMBINED_COPY

// ---------------- NODE MANAGEMENT ----------------

/**
 * Used to use multi-objects in the same data structure
 */
//#define GS_USE_MULTI_OBJECTS

/**
 * Used to node meta data
 */
//#define GS_USE_NODE_META_DATA

//#define GV_USE_BRICK_MINMAX

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/**
 * Dealing with "Warning: C4251"
 * http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
 */
//#define GS_PRAGMA_WARNING_PUSH_DISABLE		\
//#if defined _MSC_VER							\
//	#pragma warning( push )						\
//	#pragma warning( disable:4251 )				\
//#endif
//
//#define GS_PRAGMA_WARNING_POP					\
//#if defined _MSC_VER							\
//	#pragma warning( pop )						\
//#endif

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/

#endif // !_GS_CONFIG_H_
