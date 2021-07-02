#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(set = 0, binding = 0) uniform PerFrameUBO
{
	mat4 view;
	mat4 projection;
}
perFrame;

layout(set = 1, binding = 0) uniform PerObjectUBO
{
	mat4 model;
}
perObject;

layout(location = 0) out struct
{
	vec3 fragPos;
}
vertOut;

void main()
{
    gl_Position = (perFrame.projection * perFrame.view * perObject.model) * vec4(inPosition, 1.0);
	vertOut.fragPos = inNormal;
}