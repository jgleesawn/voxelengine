#version 330
//#extension GL_ARB_explicit_attrib_location : enable
//#extension GL_ARB_vertex_array_object : enable
//#extension GL_ARB_vertex_buffer_object : enable


layout(location = 0) in vec4 vertexCoord;
layout(location = 1) in uvec4 position;	//x,y,z,depthMask
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
	uvec3 mpos = position.xyz;
	vec4 vertexPos = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
//depthMask is mpos.w
//	uint depthMask = position.w;//depthMask_in;
//	float boxDim;
//	vertexPos.xyz += lowerLeftBound.xyz;

	vertexPos.xyz = vertexCoord.xyz*resolution/2.0f;
	vertexPos.xyz += resolution*vec3(position.xyz);
	vertexPos.xyz += lowerLeftBound.xyz;
//	vertexPos.xyz -= 50.0f;
//	vertexPos.xyz /= 100.0f;
//	vertexPos.xyz = vertexCoord.xyz;
//	vertexPos.xyz += vec3(1.0f, 0.0f, 0.0f) * gl_InstanceID;
//	vertexPos.z -= 10.0f;
//	vertexPos.z *= -1.0f;
	vertexPos.w = 1.0f;

//	vertexPos.xyz += vertexCoord.xyz * resolution / 2.0f; //*position.w would scale based on depthMask.

//	vertexPos.xyz += lowerLeftBound.xyz;
//	vertexPos.xyz += vertexCoord.xyz - vec3(0.0f, 0.0f, 10.0f); //*position.w would scale based on depthMask.
//	vertexPos.xyz += vec3(1.0f, 0.0f, 0.0f)*gl_InstanceID; //vec3(position.xyz)*100.0f;

//	vertexPos.xyz += vec3(position.xyz);
//	vertexPos.xyz += lowerLeftBound.xyz;

	gl_Position = ( vertexPos - viewOffset ) * (viewRotation * Perspective);
}
