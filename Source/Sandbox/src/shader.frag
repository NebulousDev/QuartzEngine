#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_POINT_LIGHTS 4

struct DirectionalLight
{
	vec4 direction;
	vec4 color;
};

struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

layout(binding = 1) uniform LightsUBO
{
	vec4 cameraPosition;
	DirectionalLight directional[MAX_DIRECTIONAL_LIGHTS];
	PointLight point[MAX_POINT_LIGHTS];
	uint directionalCount;
	uint pointCount;
}
lights;

layout(location = 0) in struct
{
	vec3 fragPos;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
	vec2 texCoord;
	mat3 tbn;
}
fragIn;

layout(binding = 2) uniform sampler2D diffuseTexSampler;
layout(binding = 3) uniform sampler2D normalTexSampler;
layout(binding = 4) uniform sampler2D specularTexSampler;

layout(location = 0) out vec4 fragOut;

vec3 CalcSurfaceNormal()
{
	vec3 texNormal = texture(normalTexSampler, fragIn.texCoord).xyz;
	vec3 normal = fragIn.tbn * normalize(texNormal * 2.0 - 1.0);

	return normal;
}

float CalcSurfaceSpecular()
{
	return 1.0f - texture(specularTexSampler, fragIn.texCoord).g;
}

vec3 CalcDirectionalLightsDiffuse(vec3 surfaceNormal)
{
	vec3 diffuse = vec3(0,0,0);

	for(uint i = 0; i < lights.directionalCount; i++)
	{
		vec3 lightDir = normalize(lights.directional[i].direction.xyz);  
		float factor = max(dot(surfaceNormal, lightDir), 0.0);
		diffuse += factor * lights.directional[i].color.xyz;
	}

	return diffuse;
}

vec3 CalcDirectionalLightsSpecular(vec3 surfaceNormal)
{
	const float shininess = 8;
	vec3 specular = vec3(0,0,0);

	vec3 camPos = -lights.cameraPosition.xyz;

	for(uint i = 0; i < lights.directionalCount; i++)
	{
		vec3 lightDir   = normalize(lights.directional[i].direction.xyz);
		vec3 viewDir    = normalize(lights.cameraPosition.xyz - fragIn.fragPos.xyz);
		vec3 halfwayDir = normalize(lightDir + viewDir);

		float spec = pow(max(dot(surfaceNormal, halfwayDir), 0.0), shininess);
		specular += spec * lights.directional[i].color.xyz;
	}

	return specular;
}

vec3 CalcPointLightsDiffuse(vec3 surfaceNormal)
{
	vec3 diffuse = vec3(0,0,0);

	for(uint i = 0; i < lights.pointCount; i++)
	{
		float dist = length(lights.point[i].position.xyz - fragIn.fragPos.xyz);

		float attenuation = 1.0 / (
			lights.point[i].attenuation.x + 
			lights.point[i].attenuation.y * dist + 
			lights.point[i].attenuation.z * (dist * dist));

		vec3 lightDir = normalize(lights.point[i].position.xyz - fragIn.fragPos.xyz);  
		float factor = max(dot(surfaceNormal, lightDir), 0.0);

		diffuse += factor * lights.point[i].color.xyz * attenuation;
	}

	return diffuse;
}

vec3 CalcPointLightsSpecular(vec3 surfaceNormal)
{
	const float shininess = 8;
	vec3 specular = vec3(0,0,0);

	vec3 camPos = -lights.cameraPosition.xyz;

	for(uint i = 0; i < lights.pointCount; i++)
	{
		float dist = length(lights.point[i].position.xyz - fragIn.fragPos.xyz);

		float attenuation = 1.0 / (
			lights.point[i].attenuation.x + 
			lights.point[i].attenuation.y * dist + 
			lights.point[i].attenuation.z * (dist * dist));

		vec3 lightDir   = normalize(lights.point[i].position.xyz - fragIn.fragPos.xyz);
		vec3 viewDir    = normalize(camPos - fragIn.fragPos.xyz);
		vec3 halfwayDir = normalize(lightDir + viewDir);

		float spec = pow(max(dot(surfaceNormal, halfwayDir), 0.0), shininess);
		specular += spec * lights.point[i].color.xyz * attenuation;
	}

	return specular;
}

void main()
{
	vec3 surfaceNormal = CalcSurfaceNormal();
	float surfaceSpecular = CalcSurfaceSpecular();

	vec3 directionalDiffuse = CalcDirectionalLightsDiffuse(surfaceNormal);
	vec3 directionalSpecular = CalcDirectionalLightsSpecular(surfaceNormal);

	vec3 pointDiffuse = CalcPointLightsDiffuse(surfaceNormal);
	vec3 pointSpecular = CalcPointLightsSpecular(surfaceNormal);

	vec3 diffuseResult = (directionalDiffuse + pointDiffuse) + (directionalSpecular + pointSpecular) * surfaceSpecular;

    fragOut = texture(diffuseTexSampler, fragIn.texCoord) * vec4(diffuseResult, 1.0);

	//fragOut = vec4(surfaceSpecular, 0, 0, 1.0);
}