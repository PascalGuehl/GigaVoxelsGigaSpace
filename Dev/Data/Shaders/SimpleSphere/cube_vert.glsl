////////////////////////////////////////////////////////////////////////////////
//
// VERTEX SHADER
//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Publication: By-Example Point Process Texture Basis Functions
 * Authors: anonymous
 ******************************************************************************/

/******************************************************************************
 * Procedural vertex shader
 *
 * Only one triangle covering screen is rendered
 * - there is neither no VBO, nor VAO: only 1 triangle with 3 points is requested as a draw command in the GL pipeline
 * - based on gl_VertexID, one can deduce triangle geometry on-th-fly and send data to clip space
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

////////////////////////////////////////////////////////////////////////////////
// UNIFORM
////////////////////////////////////////////////////////////////////////////////

//uniform float uSize;
uniform mat4 uModelMatrix;

// Scene
uniform mat4 uSceneModelMatrix;
uniform mat4 uSceneViewMatrix;
uniform mat4 uSceneProjectionMatrix;

////////////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////////////
void main()
{
	// Compute vertex position
	int b = 1 << gl_VertexID;
	float x = float( ( 0x287a & b ) != 0 );
	float y = float( ( 0x02af & b ) != 0 );
	float z = float( ( 0x31e3 & b ) != 0 );
	
	//gl_PointSize = 20.0;
	
	// Send position to clip space
	//gl_Position = vec4( x, y, z, 1.0 );
	//mat4 modelMatrix = mat4( 1.0 );
	mat4 modelMatrix = mat4( 1.0 );
	//modelMatrix[ 3 ] = vec4( vec3( -0.5 ), 1.0 );
	//modelMatrix = mat4( 0.5 ) * modelMatrix;
	//modelMatrix = modelMatrix * uModelMatrix;
	modelMatrix = uSceneModelMatrix * modelMatrix * uModelMatrix;
	// mat4 viewMatrix = mat4( 1.0 );
	// mat4 projectionMatrix = mat4( 1.0 );
	mat4 viewMatrix = uSceneViewMatrix;
	mat4 projectionMatrix = uSceneProjectionMatrix;
	//gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( 0.0, 0.0, 0.0, 1.0 );
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( x, y, z, 1.0 );
}
