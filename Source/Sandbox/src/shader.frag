#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in struct
{
<<<<<<< Updated upstream
	vec3 worldPosition;
=======
	vec2 texCoord;
>>>>>>> Stashed changes
}
fragIn;

layout(location = 0) out vec4 fragOut;
layout(binding = 1) uniform sampler2D sampler;

void main()
{
<<<<<<< Updated upstream
    fragOut = vec4(fragIn.worldPosition, 1.0);
=======
    fragOut = texture(sampler, texCoord);
>>>>>>> Stashed changes
}