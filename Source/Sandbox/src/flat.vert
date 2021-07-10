#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
//layout(location = 2) in vec3 inBinormal;
//layout(location = 3) in vec3 inBitangent;
layout(location = 2) in vec2 inTexCoord;

layout(set = 0, binding = 0) uniform PerFrameUBO
{
	mat4 view;
	mat4 projection;
	vec3 cameraPos;
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
}
vertOut;

void main()
{
    gl_Position			= (perFrame.projection * perFrame.view * perObject.model) * vec4(inPosition, 1.0);
	vertOut.cameraPos	= perFrame.cameraPos;
	vertOut.position	= (perObject.model * vec4(inPosition, 1.0)).xyz;
	vertOut.normal		= mat3(transpose(perObject.model)) * inNormal;
	vertOut.texCoord	= inTexCoord;

	// Mysterious fixes
	vertOut.cameraPos.x = -vertOut.cameraPos.x;
	vertOut.cameraPos.y = -vertOut.cameraPos.y;
	vertOut.position.z	= -vertOut.position.z;
}