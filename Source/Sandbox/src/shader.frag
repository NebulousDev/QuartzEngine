#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in struct
{
	vec3 normal;
	vec2 texCoord;
}
fragIn;

layout(binding = 1) uniform sampler2D diffuseTexSampler;

layout(location = 0) out vec4 fragOut;

void main()
{
    fragOut = texture(diffuseTexSampler, fragIn.texCoord);
}