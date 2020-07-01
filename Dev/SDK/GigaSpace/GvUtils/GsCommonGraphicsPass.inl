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
 ****************************** INLINE DEFINITION *****************************
 ******************************************************************************/

namespace GvUtils
{

/******************************************************************************
 * Get the depth buffer
 *
 * @return the depth buffer
 ******************************************************************************/
inline GLuint GsCommonGraphicsPass::getDepthBuffer() const
{
	return _depthBuffer;
}

/******************************************************************************
 * Get the color texture
 *
 * @return the color texture
 ******************************************************************************/
inline GLuint GsCommonGraphicsPass::getColorTexture() const
{
	return _colorTexture;
}

/******************************************************************************
 * Get the color render buffer
 *
 * @return the color render buffer
 ******************************************************************************/
inline GLuint GsCommonGraphicsPass::getColorRenderBuffer() const
{
	return _colorRenderBuffer;
}

/******************************************************************************
 * Get the depth texture
 *
 * @return the depth texture
 ******************************************************************************/
inline GLuint GsCommonGraphicsPass::getDepthTexture() const
{
	return _depthTexture;
}

/******************************************************************************
 * Get the framebuffer object
 *
 * @return the framebuffer object
 ******************************************************************************/
inline GLuint GsCommonGraphicsPass::getFrameBuffer() const
{
	return _frameBuffer;
}

/******************************************************************************
 * Get the width
 *
 * @return the width
 ******************************************************************************/
inline int GsCommonGraphicsPass::getBufferWidth() const
{
	return _width;
}

/******************************************************************************
 * Get the height
 *
 * @return the height
 ******************************************************************************/
inline int GsCommonGraphicsPass::getBufferHeight() const
{
	return _height;
}

/******************************************************************************
 * Tell wheter or not the pipeline uses image downscaling.
 *
 * @return the flag telling wheter or not the pipeline uses image downscaling
 ******************************************************************************/
inline bool GsCommonGraphicsPass::hasImageDownscaling() const
{
	return _hasImageDownscaling;
}

/******************************************************************************
 * Get the image downscaling width
 *
 * @return the image downscaling width 
 ******************************************************************************/
inline int GsCommonGraphicsPass::getImageDownscalingWidth() const
{
	return _imageDownscalingWidth;
}

/******************************************************************************
 * Get the image downscaling height
 *
 * @return the image downscaling width
 ******************************************************************************/
inline int GsCommonGraphicsPass::getImageDownscalingHeight() const
{
	return _imageDownscalingHeight;
}

/******************************************************************************
 * Get the type
 *
 * @return the type
 ******************************************************************************/
inline unsigned int GsCommonGraphicsPass::getType() const
{
	return _type;
}

} // namespace GvUtils
