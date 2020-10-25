﻿// ////////////////////////////////////////////////////////////////////////////////
// //
// // FRAGMENT SHADER
// //
// // Fullscreen quad rendering
// //
// ////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////
// // VERSION
// ////////////////////////////////////////////////////////////////////////////////

// #version 400

// ////////////////////////////////////////////////////////////////////////////////
// // INPUT
// ////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////
// // UNIFORM
// ////////////////////////////////////////////////////////////////////////////////

// // Texture sampler for fullscreen quad rendering
// uniform sampler2DRect uTextureSampler;

// ////////////////////////////////////////////////////////////////////////////////
// // OUTPUT
// ////////////////////////////////////////////////////////////////////////////////

// layout (location = 0) out vec4 oColor;

// ////////////////////////////////////////////////////////////////////////////////
// // PROGRAM
// ////////////////////////////////////////////////////////////////////////////////
// void main()
// {
	// oColor = texelFetch( uTextureSampler, ivec2( gl_FragCoord.x, gl_FragCoord.y ) );
// }

////////////////////////////////////////////////////////////////////////////////
//
// FRAGMENT SHADER
//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Publication: By-Example Point Process Texture Basis Functions
 * Authors: anonymous
 ******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// VERSION
////////////////////////////////////////////////////////////////////////////////

// OpenGL
#version 460

////////////////////////////////////////////////////////////////////////////////
// INPUT
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

layout (location = 0) out vec4 oColor;

////////////////////////////////////////////////////////////////////////////////
// UNIFORM
////////////////////////////////////////////////////////////////////////////////

// Texture
//uniform sampler2D uTexture;

////////////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////////////
void main()
{
	oColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}
