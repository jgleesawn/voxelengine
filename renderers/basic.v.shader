#version 120
#extension GL_ARB_explicit_attrib_location : enable
//#extension GL_ARB_vertex_array_object : enable
//#extension GL_ARB_vertex_buffer_object : enable


layout(location = 0) in vec4 vertexCoord;

//layout(location = 0) uniform vec4 objPosition;
//layout(location = 1) uniform mat4 objRotation;

//layout(location = 2) uniform vec4 viewOffset;
//layout(location = 3) uniform mat4 viewRotation;

//Cheaper to perform multiplication once in program code.
//Didn't want to do matrix math there.
//Fix this.
//layout(location = 4) uniform mat4 Perspective;

uniform vec4 objPosition;
uniform mat4 objRotation;

uniform vec4 viewOffset;
uniform mat4 viewRotation;

uniform mat4 Perspective;

void main()
{
	vec4 vertexPos = vec4(vertexCoord.xyz, 1.0f);
	vertexPos = ( vertexPos * objRotation ) + objPosition;// + vec4(5.0, 5.0, -10.0, 0.0);
	gl_Position = ( vertexPos - viewOffset ) * (viewRotation * Perspective);
}
