#version 120
#extension GL_ARB_explicit_attrib_location : enable
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
	vec4 vertexPos = vec4( 0.0f );
//depthMask is mpos.w
	int depthMask = position.w;//depthMask_in;
	float boxDim;
	while( mpos > 0 ) {
		boxDim = depthMask*resolution;
		vertexPos.xyz += boxDim*(mpos.xyz & 1);
//		vertexPos.z += boxDim*(mpos.x & 1);
//		vertexPos.y += boxDim*(mpos.y & 1);
//		vertexPos.x += boxDim*(mpos.z & 1);
		mpos.xyz >>= 1;
		depthMask <<= 1;
	}
	vertexPox.xyz += lowerLeftBound.xyz;
	vertexPos.w = 1.0f;
	vertexPos.xyz += vertexCoord.xyz * resolution / 2.0f; //*position.w would scale based on depthMask.
	gl_Position = ( vertexPos - viewOffset ) * (viewRotation * Perspective);
}
