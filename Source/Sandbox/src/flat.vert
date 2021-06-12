#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform UBO
{
	mat4 model;
	mat4 view;
	mat4 projection;
}
mvp;

layout(location = 0) out struct
{
	vec3 fragPos;
}
vertOut;

void main()
{
    gl_Position = (mvp.projection * mvp.view * mvp.model) * vec4(inPosition, 1.0);
	vertOut.fragPos = inPosition;
}