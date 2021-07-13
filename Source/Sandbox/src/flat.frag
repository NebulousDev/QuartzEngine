#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in struct
{
	vec3 cameraPos;
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	mat3 tbn;
}
fragIn;

layout(set = 0, binding = 1) uniform sampler2D diffuseSampler;
layout(set = 0, binding = 2) uniform sampler2D normalSampler;
layout(set = 0, binding = 3) uniform sampler2D roughnessSampler;
layout(set = 0, binding = 4) uniform sampler2D metallicSampler;
layout(set = 0, binding = 5) uniform sampler2D ambientSampler;

layout(location = 0) out vec4 fragOut;

const float PI = 3.14159265359;

float DistributionGGX(float ndoth, float roughness)
{
	float a2    = roughness * roughness * roughness * roughness;
    float denom = (ndoth * ndoth * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float theta, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return theta / (theta * (1.0 - k) + k);
}
  
float GeometrySmith(float ndotl, float ndotv, float roughness)
{
    return GeometrySchlickGGX(ndotl, roughness) * 
           GeometrySchlickGGX(ndotv, roughness);
}

vec3 FresnelSchlick(float theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow (1.0 - theta, 5.0);
} 

void main()
{
	/* Temporary constants */
	const vec3 lightPos		= vec3(0.0, 12.0, 5.0);
	const vec3 lightColor	= vec3(10.0, 10.0, 10.0);

	/* Sample Textures */
	vec3  albedo	= texture(diffuseSampler,   fragIn.texCoord).xyz;
	vec3  normal	= texture(normalSampler,    fragIn.texCoord).xyz;
	float roughness	= texture(roughnessSampler, fragIn.texCoord).r;
	float metallic	= texture(metallicSampler,  fragIn.texCoord).r;
	float ambient	= texture(ambientSampler,   fragIn.texCoord).r;

	/* Apply gamma correction to albedo */
	albedo = pow(albedo, vec3(2.2));

	/* Apply normal mapping to normal vector */
	normal = normalize(fragIn.tbn * (normal * 2.0 - 1.0)); 

	/* Calculate view direction */
	vec3 viewDir = normalize(fragIn.cameraPos - fragIn.position);
	
	/* Fresnel base parameters */
	vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
	
	/* Calculate all light influence */
	vec3 lightOut = vec3(0.0);

	for(int i = 0; i < 1; i++)
	{
		/* Calculate vectors */
		vec3 lightDir		= normalize(lightPos - fragIn.position);
		vec3 halfDir		= normalize(viewDir + lightDir);

		/* Calculate radiance */
		float dist			= length(lightDir);
		float attenuation	= 1.0 / (dist * dist);
		vec3 radiance		= lightColor * attenuation;
	
		/* Calculate incidences */
		float hdotv = max(dot(halfDir, viewDir), 0.0);
		float ndotv = max(dot(normal, viewDir), 0.0);
		float ndotl = max(dot(normal, lightDir), 0.0);
		float ndoth = max(dot(normal, halfDir), 0.0);

		/* Calculate BRDF */
		float distribition	= DistributionGGX(ndoth, roughness);
		float diffuse		= GeometrySmith(ndotl, ndotv, roughness);
		vec3 fresnel		= FresnelSchlick(hdotv, F0);

		vec3 kS = fresnel;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 numerator		= distribition * diffuse * fresnel;
		float denominator	= 4.0 * ndotv * ndotl;
		vec3 specular		= numerator / max(denominator, 0.001);
        
		/* Add light radiance to total light */
		lightOut += (kD * albedo / PI + specular) * radiance * ndotl;
	}

	/* Ambient contribution */
	vec3 ambientOut = vec3(0.03) * albedo * ambient;
    vec3 color = ambientOut + lightOut;

	/* Gamma correction */
	color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

	/* Final color */
    fragOut = vec4(color, 1.0);
}