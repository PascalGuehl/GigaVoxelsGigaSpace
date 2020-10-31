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

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/


/**
 * This file defines all the events that may be used to monitor an application
 */

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( cpmApplicationDefaultFrameEvent )	/* Frame event, should not be removed */

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( application_userEvt0 )

/**
 * Main frame
 */
CUDAPM_DEFINE_EVENT( frame )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( app_init_frame )
CUDAPM_DEFINE_EVENT( app_post_frame )

/**
 * Copy data structure from device to host
 */
CUDAPM_DEFINE_EVENT( copy_dataStructure_to_host )

/**
 * Data Production Manager - Clear cache
 */
CUDAPM_DEFINE_EVENT( gpucache_clear )

/**
 * Cache Manager - clear cache
 */
CUDAPM_DEFINE_EVENT( gpucachemgr_clear_cpyAddr ) // not used...
CUDAPM_DEFINE_EVENT( gpucachemgr_clear_fillML )	// Temp masks
CUDAPM_DEFINE_EVENT( gpucachemgr_clear_fillTimeStamp ) // Timestamps

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_preRenderPass )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( vsrender_init_frame )  // not used...
CUDAPM_DEFINE_EVENT( vsrender_copyconsts_frame )

/**
 * Main rendering stage
 */
CUDAPM_DEFINE_EVENT( gv_rendering )
CUDAPM_DEFINE_EVENT( vsrender_initRays ) // not used...
CUDAPM_DEFINE_EVENT( vsrender_endRays )

/**
 * Data Production Management
 */
CUDAPM_DEFINE_EVENT( dataProduction_handleRequests )

/**
 * Manage requests
 */
CUDAPM_DEFINE_EVENT( dataProduction_manageRequests )
CUDAPM_DEFINE_EVENT( dataProduction_manageRequests_createMask ) // not used...
CUDAPM_DEFINE_EVENT( dataProduction_manageRequests_elemsReduction )
CUDAPM_DEFINE_EVENT( dataProduction_manageRequests_my_copy_if_0 ) // not used...
CUDAPM_DEFINE_EVENT( dataProduction_manageRequests_my_copy_if_1 ) // not used...
CUDAPM_DEFINE_EVENT( dataProduction_manageRequests_my_copy_if_2 ) // not used...

/**
 * Update timestamps
 */
CUDAPM_DEFINE_EVENT( cache_updateTimestamps )
CUDAPM_DEFINE_EVENT( cache_updateTimestamps_dataStructure )
CUDAPM_DEFINE_EVENT( cache_updateTimestamps_bricks )
CUDAPM_DEFINE_EVENT( cache_updateTimestamps_createMasks )
CUDAPM_DEFINE_EVENT( cache_updateTimestamps_threadReduc ) // not used...
CUDAPM_DEFINE_EVENT( cache_updateTimestamps_threadReduc1 )
CUDAPM_DEFINE_EVENT( cache_updateTimestamps_threadReduc2 )

/**
 * Production
 */
CUDAPM_DEFINE_EVENT( producer_nodes )
CUDAPM_DEFINE_EVENT( producer_bricks )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_updateSymbols )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucachemgr_createUpdateList_createMask )
CUDAPM_DEFINE_EVENT( gpucachemgr_createUpdateList_elemsReduction )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_nodes_manageUpdates )
CUDAPM_DEFINE_EVENT( gpucache_nodes_createMask )
CUDAPM_DEFINE_EVENT( gpucache_nodes_elemsReduction )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_nodes_subdivKernel )
CUDAPM_DEFINE_EVENT( gpucache_nodes_preLoadMgt )
CUDAPM_DEFINE_EVENT( gpucache_nodes_preLoadMgt_gpuProd )
CUDAPM_DEFINE_EVENT( gpuProdDynamic_preLoadMgtNodes_fetchRequestList )
CUDAPM_DEFINE_EVENT( gpuProdDynamic_preLoadMgtNodes_dataLoad )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_bricks_bricksInvalidation )
CUDAPM_DEFINE_EVENT( gpucache_bricks_copyTransfer )
CUDAPM_DEFINE_EVENT( gpucache_bricks_cpuSortRequests )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_bricks_manageUpdates )
CUDAPM_DEFINE_EVENT( gpucache_bricks_createMask )
CUDAPM_DEFINE_EVENT( gpucache_bricks_elemsReduction )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_bricks_getLocalizationInfo )
CUDAPM_DEFINE_EVENT( gpucache_bricks_gpuFetchBricks )
CUDAPM_DEFINE_EVENT( gpucache_bricks_gpuFetchBricks_constUL )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucache_bricks_loadBricks )
CUDAPM_DEFINE_EVENT( gpucache_bricks_manageConsts )
CUDAPM_DEFINE_EVENT( gpucache_bricks_updateOctreeBricks )
CUDAPM_DEFINE_EVENT( gpucache_bricks_preLoadMgt )
CUDAPM_DEFINE_EVENT( gpucache_bricks_preLoadMgt_gpuProd )
CUDAPM_DEFINE_EVENT( gpuProdDynamic_preLoadMgtData_fetchRequestList )
CUDAPM_DEFINE_EVENT( gpuProdDynamic_preLoadMgtData_dataLoad )
CUDAPM_DEFINE_EVENT( gpuProdDynamic_preLoadMgtData_dataLoad_elemLoop )
CUDAPM_DEFINE_EVENT( copyToTextureTest0 )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( gpucachemgr_updateSymbols )
CUDAPM_DEFINE_EVENT( gpucachemgr_updateTimeStampsCPU )

/**
 * ...
 */
CUDAPM_DEFINE_EVENT( cudakernelRenderGigaVoxels )
CUDAPM_DEFINE_EVENT( cudainternalRenderGigaVoxels )


CUDAPM_DEFINE_EVENT( gpucache_update_VBO )

		
CUDAPM_DEFINE_EVENT( gpucache_update_VBO_createMask )
CUDAPM_DEFINE_EVENT( gpucache_update_VBO_compaction )
CUDAPM_DEFINE_EVENT( gpucache_update_VBO_nb_pts )
CUDAPM_DEFINE_EVENT( gpucache_update_VBO_parallel_prefix_sum )
CUDAPM_DEFINE_EVENT( gpucache_update_VBO_update_VBO )

/**
 * Pre/Post frame
 * Map/unmap graphics resources
 */
CUDAPM_DEFINE_EVENT( vsrender_pre_frame )
CUDAPM_DEFINE_EVENT( vsrender_pre_frame_mapbuffers )
CUDAPM_DEFINE_EVENT( vsrender_post_frame )
CUDAPM_DEFINE_EVENT( vsrender_post_frame_unmapbuffers )

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ******************************** CLASS USED **********************************
 ******************************************************************************/

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/
