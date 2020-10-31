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

#ifndef INTERSECTIONTESTSKERNEL_H
#define INTERSECTIONTESTSKERNEL_H

// GigaVoxels
#include "GvCore/GsCoreConfig.h"
#include "GvCore/GsVectorTypesExt.h"

namespace GvCore {

__device__
__forceinline__ float trianglePointDistSqrKernel(float3 p, float3 v0, float3 v1, float3 v2, float3 &baryCoords);

__device__
__forceinline__ uint triangleRayIntersectKernel(float &t, float tmin, float3 &rayStart, float3 &rayDir, float3 vert0, float3 vert1, float3 vert2, float3 &baryCoords);

__device__
__forceinline__ int triangleRayIntersect2Kernel(float &t, float tmin, float3 &rayStart, float3 &rayDir, float3 vert0, float3 vert1, float3 vert2, float3 &baryCoords);

__device__ __host__
inline float ccSqr(float n);

__device__
__forceinline__ bool sphereBoxIntersectKernel(float3 sphereCenter, float sphereRadius, float3 bboxMin, float3 bboxMax);

#include "GsIntersectionTestsKernel.inl"

} //namespace GvCore
#endif
