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

// Distance from a point to a triangle
// http://www.geometrictools.com/LibFoundation/Distance/Wm4DistVector3Triangle3.h
__device__
__forceinline__ float trianglePointDistSqrKernel (float3 p, float3 v0, float3 v1, float3 v2, float3 &baryCoords){

	float3 kDiff = v0 - p;
	float3 kEdge0 = v1 - v0;
	float3 kEdge1 = v2 - v0;
	float fA00 = squaredLength(kEdge0);
	float fA01 = dot(kEdge0, kEdge1);
	float fA11 = squaredLength(kEdge1);
	float fB0 = dot(kDiff, kEdge0);
	float fB1 = dot(kDiff, kEdge1);
	float fC = squaredLength(kDiff);
	float fDet = fabs(fA00*fA11-fA01*fA01);
	float fS = fA01*fB1-fA11*fB0;
	float fT = fA01*fB0-fA00*fB1;
	float fSqrDistance;

	if (fS + fT <= fDet) {
		if (fS < (float)0.0) {
			if (fT < (float)0.0) {  // region 4
				if (fB0 < (float)0.0) {
					fT = (float)0.0;
					if (-fB0 >= fA00) {
						fS = (float)1.0;
						fSqrDistance = fA00+((float)2.0)*fB0+fC;
					} else {
						fS = -fB0/fA00;
						fSqrDistance = fB0*fS+fC;
					}
				} else {
					fS = (float)0.0;
					if (fB1 >= (float)0.0) {
						fT = (float)0.0;
						fSqrDistance = fC;
					} else if (-fB1 >= fA11) {
						fT = (float)1.0;
						fSqrDistance = fA11+((float)2.0)*fB1+fC;
					} else {
						fT = -fB1/fA11;
						fSqrDistance = fB1*fT+fC;
					}
				}
			} else { // region 3
				fS = (float)0.0;
				if (fB1 >= (float)0.0) {
					fT = (float)0.0;
					fSqrDistance = fC;
				} else if (-fB1 >= fA11) {
					fT = (float)1.0;
					fSqrDistance = fA11+((float)2.0)*fB1+fC;
				} else {
					fT = -fB1/fA11;
					fSqrDistance = fB1*fT+fC;
				}
			}
		} else if (fT < (float)0.0) { // region 5
			fT = (float)0.0;
			if (fB0 >= (float)0.0) {
				fS = (float)0.0;
				fSqrDistance = fC;
			} else if (-fB0 >= fA00) {
				fS = (float)1.0;
				fSqrDistance = fA00+((float)2.0)*fB0+fC;
			} else {
				fS = -fB0/fA00;
				fSqrDistance = fB0*fS+fC;
			}
		} else { // region 0
			// minimum at interior point
			float fInvDet = ((float)1.0)/fDet;
			fS *= fInvDet;
			fT *= fInvDet;
			fSqrDistance = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
				fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
		}
	} else {
		float fTmp0, fTmp1, fNumer, fDenom;

		if (fS < (float)0.0) { // region 2
			fTmp0 = fA01 + fB0;
			fTmp1 = fA11 + fB1;
			if (fTmp1 > fTmp0) {
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-2.0f*fA01+fA11;
				if (fNumer >= fDenom) {
					fS = (float)1.0;
					fT = (float)0.0;
					fSqrDistance = fA00+((float)2.0)*fB0+fC;
				} else {
					fS = fNumer/fDenom;
					fT = (float)1.0 - fS;
					fSqrDistance = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
						fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
				}
			} else {
				fS = (float)0.0;
				if (fTmp1 <= (float)0.0) {
					fT = (float)1.0;
					fSqrDistance = fA11+((float)2.0)*fB1+fC;
				} else if (fB1 >= (float)0.0) {
					fT = (float)0.0;
					fSqrDistance = fC;
				} else {
					fT = -fB1/fA11;
					fSqrDistance = fB1*fT+fC;
				}
			}
		} else if (fT < (float)0.0) { // region 6
			fTmp0 = fA01 + fB1;
			fTmp1 = fA00 + fB0;
			if (fTmp1 > fTmp0) {
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-((float)2.0)*fA01+fA11;
				if (fNumer >= fDenom) {
					fT = (float)1.0;
					fS = (float)0.0;
					fSqrDistance = fA11+((float)2.0)*fB1+fC;
				} else {
					fT = fNumer/fDenom;
					fS = (float)1.0 - fT;
					fSqrDistance = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
						fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
				}
			} else {
				fT = (float)0.0;
				if (fTmp1 <= (float)0.0) {
					fS = (float)1.0;
					fSqrDistance = fA00+((float)2.0)*fB0+fC;
				} else if (fB0 >= (float)0.0) {
					fS = (float)0.0;
					fSqrDistance = fC;
				} else {
					fS = -fB0/fA00;
					fSqrDistance = fB0*fS+fC;
				}
			}
		} else { // region 1
			fNumer = fA11 + fB1 - fA01 - fB0;
			if (fNumer <= (float)0.0) {
				fS = (float)0.0;
				fT = (float)1.0;
				fSqrDistance = fA11+((float)2.0)*fB1+fC;
			} else {
				fDenom = fA00-2.0f*fA01+fA11;
				if (fNumer >= fDenom) {
					fS = (float)1.0;
					fT = (float)0.0;
					fSqrDistance = fA00+((float)2.0)*fB0+fC;
				} else {
					fS = fNumer/fDenom;
					fT = (float)1.0 - fS;
					fSqrDistance = fS*(fA00*fS+fA01*fT+((float)2.0)*fB0) +
						fT*(fA01*fS+fA11*fT+((float)2.0)*fB1)+fC;
				}
			}
		}
	}

	// account for numerical round-off error
	if (fSqrDistance < (float)0.0)
	{
		fSqrDistance = (float)0.0;
	}

	/*m_kClosestPoint0 = *m_pkVector;
	m_kClosestPoint1 = m_pkTriangle->V[0] + fS*kEdge0 + fT*kEdge1;
	m_afTriangleBary[1] = fS;
	m_afTriangleBary[2] = fT;
	m_afTriangleBary[0] = (float)1.0 - fS - fT;*/

	baryCoords.x=(float)1.0 - fS - fT;
	baryCoords.y=fS;
	baryCoords.z=fT;

	return fSqrDistance;
}


//New
__device__
uint triangleRayIntersectKernel(float &t, float tmin, const float3 &rayStart, const float3 &rayDir, float3 vert0, float3 vert1, float3 vert2, float3 &baryCoords) {
  float u, v;

  float3 dir=rayDir;
  float3 orig=rayStart;

  const float EPSI=0.000000001f;

 /* Vec vert0=this->frame().inverseCoordinatesOf(p0);
  Vec vert1=this->frame().inverseCoordinatesOf(p1);
  Vec vert2=this->frame().inverseCoordinatesOf(p2);*/

  ////Ray triangle intersection from "Fast Minimum Storage RayTriangle Intersection" [MollerTrumbore97]

   //double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float3 edge1, edge2, pvec, tvec, qvec;
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   edge1= vert1-vert0;
   edge2= vert2-vert0;

   /* begin calculating determinant - also used to calculate U parameter */
   pvec= cross(dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = dot(edge1, pvec); //dot

   if (det > -EPSI && det < EPSI) //bug with degenerated triangles
	 return false;

   inv_det = 1.0f / det;

   /* calculate distance from vert0 to ray origin */
   tvec = orig - vert0;

   /* calculate U parameter and test bounds */
   u = dot(tvec, pvec) * inv_det; //dot
   if (u < 0.0f || u > 1.0f)
	 return 0;

   /* prepare to test V parameter */
   qvec= cross(tvec, edge1);

   /* calculate V parameter and test bounds */
   v = dot(dir, qvec) * inv_det; //dot
   if (v < 0.0f || u + v > 1.0f)
	 return 0;

   /* calculate t, ray intersects triangle */
   t = dot(edge2, qvec) * inv_det; //dot


  //float tmin=RTConfig::getInstance()->getRayTMin();// 0.0001;

  if(t>=tmin){
	  //hit.setNormal(cross(edge1, edge2));
	  // "Partie XI: Textures"
	  //hit.setUV(u, v );

	  baryCoords.x=(float)1.0 - u - v;
	  baryCoords.y=u;
	  baryCoords.z=v;

	  return 1;
  }

  return 0;
}

//http://www.geometrictools.com/LibFoundation/Intersection/Wm4IntrRay3Triangle3.cpp
__device__
int triangleRayIntersect2Kernel(float &t, float tmin, float3 &rayStart, float3 &rayDir, float3 vert0, float3 vert1, float3 vert2, float3 &baryCoords) {

	// compute the offset origin, edges, and normal
	float3 kDiff = rayStart - vert0;
	float3 kEdge1 = vert1 - vert0;
	float3 kEdge2 = vert2 - vert0;
	float3 kNormal = cross(kEdge1, kEdge2);

	const float ZERO_TOLERANCE=0.0000001f;

	// Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
	// E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
	//   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
	//   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
	//   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
	float fDdN = dot(rayDir, kNormal);
	float fSign;
	if (fDdN > ZERO_TOLERANCE)
	{
		fSign = 1.0f;
	}
	else if (fDdN < -ZERO_TOLERANCE)
	{
		fSign = -1.0f;
		fDdN = -fDdN;
	}
	else
	{
		// Ray and triangle are parallel, call it a "no intersection"
		// even if the ray does intersect.
		///m_iIntersectionType = IT_EMPTY;
		return 0;
	}

	float fDdQxE2 = fSign* dot( rayDir, cross(kDiff, kEdge2) );
	if (fDdQxE2 >= 0.0f)
	{
		float fDdE1xQ = fSign* dot(rayDir, cross(kEdge1, kDiff) );
		if (fDdE1xQ >= 0.0f)
		{
			if (fDdQxE2 + fDdE1xQ <= fDdN)
			{
				// line intersects triangle, check if ray does
				float fQdN = -fSign* dot(kDiff, kNormal);
				if (fQdN >= 0.0f)
				{
					// ray intersects triangle
					///m_iIntersectionType = IT_POINT;
					return 1;
				}
				// else: t < 0, no intersection
			}
			// else: b1+b2 > 1, no intersection
		}
		// else: b2 < 0, no intersection
	}
	// else: b1 < 0, no intersection

	//m_iIntersectionType = IT_EMPTY;
	return 0;
}

__device__ __host__
inline float ccSqr(float n){
	return n*n;
}
__device__
bool sphereBoxIntersectKernel (float3 sphereCenter, float sphereRadius, float3 bboxMin, float3 bboxMax) {
	
	float  r2 = sphereRadius*sphereRadius;



	float dmin = 0;
	{
		if( sphereCenter.x < bboxMin.x ) 
			dmin += ccSqr(sphereCenter.x - bboxMin.x ); 
		else if( sphereCenter.x > bboxMax.x ) 
			dmin += ccSqr( sphereCenter.x - bboxMax.x );	 

		if( sphereCenter.y < bboxMin.y ) 
			dmin += ccSqr(sphereCenter.y - bboxMin.y ); 
		else if( sphereCenter.y > bboxMax.y ) 
			dmin += ccSqr( sphereCenter.y - bboxMax.y );   

		if( sphereCenter.z < bboxMin.z ) 
			dmin += ccSqr(sphereCenter.z - bboxMin.z ); 
		else if( sphereCenter.z > bboxMax.z ) 
			dmin += ccSqr( sphereCenter.z - bboxMax.z );   

	}
	if( dmin <= r2 ) 
		return true;

	return false;
	

}
