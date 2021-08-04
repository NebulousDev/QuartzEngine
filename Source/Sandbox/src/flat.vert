#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "lights.shader"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexCoord;

layout(set = 0, binding = 0) uniform PerFrameUBO
{
	mat4 view;
	mat4 projection;
	vec3 cameraPos;
	Light lights[MAX_LIGHTS];
	uint lightCount;
}
perFrame;

layout(set = 1, binding = 0) uniform PerObjectUBO
{
	mat4 model;
}
perObject;

layout(location = 0) out struct
{
	vec3 cameraPos;
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	mat3 tbn;
}
vertOut;

void main()
{
	vec3 bitangent = cross(inNormal, inTangent);

	vec3 T = normalize(vec3(perObject.model * vec4(inTangent, 0.0)));
	vec3 B = normalize(vec3(perObject.model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(perObject.model * vec4(inNormal,  0.0)));

	vec4 modelPosition	= perObject.model * vec4(inPosition, 1.0);

	vertOut.cameraPos	= perFrame.cameraPos;
	vertOut.position	= modelPosition.xyz;
	vertOut.normal		= mat3(transpose(inverse(perObject.model))) * inNormal;
	vertOut.texCoord	= inTexCoord;
	vertOut.tbn			= mat3(T, B, N);
 
	gl_Position			= perFrame.projection * perFrame.view * modelPosition;
}