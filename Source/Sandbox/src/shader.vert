#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;

layout(binding = 0) uniform UBO
{
	mat4 mvp;
}
ubo;

layout(location = 0) out struct
{
	vec3 worldPosition;
}
vertOut;

void main()
{
    gl_Position = ubo.mvp * vec4(position, 1.0);
	vertOut.worldPosition = position;
}