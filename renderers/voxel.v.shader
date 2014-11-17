#version 330
//#extension GL_ARB_explicit_attrib_location : enable
//#extension GL_ARB_vertex_array_object : enable
//#extension GL_ARB_vertex_buffer_object : enable


layout(location = 0) in vec4 vertexCoord;
layout(location = 1) in vec4 position;	//x,y,z,depthMask
//default depthMask_in, should be 1.  Higher values draw larger boxes;

uniform vec4 lowerLeftBound;
uniform float resolution;
//uniform vec4 objPosition;
//uniform mat4 objRotation;
uniform vec4 viewOffset;
uniform mat4 viewRotation;

uniform mat4 Perspective;

void main()
{
	vec4 vertexPos = vec4( 0.0f, 0.0f, 0.0f, 0.0f );

	vertexPos.xyz = vertexCoord.xyz* position.w *resolution/2.0f;
	vertexPos.xyz += position.xyz;
//	vertexPos.xyz += resolution*position.xyz*256.0f;
//	vertexPos.xyz += lowerLeftBound.xyz;
	vertexPos.w = 1.0f;

	gl_Position = ( vertexPos - viewOffset ) * (viewRotation * Perspective);
}
