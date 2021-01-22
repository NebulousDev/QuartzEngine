#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inBinormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec2 inTexCoord;

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
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
	vec2 texCoord;
	mat3 tbn;
}
vertOut;

void main()
{
    gl_Position = (mvp.projection * mvp.view * mvp.model) * vec4(inPosition, 1.0);
	vertOut.fragPos = vec3(mvp.model * vec4(inPosition, 1.0));
	vertOut.normal = mat3(transpose(inverse(mvp.model))) * inNormal;
	vertOut.binormal = inBinormal;
	vertOut.tangent = inTangent;
	vertOut.texCoord = inTexCoord;

	vec3 T = normalize(vec3(mvp.model * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(mvp.model * vec4(inNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	vertOut.tbn = mat3(T, B, N);
}