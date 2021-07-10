#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in struct
{
	vec3 cameraPos;
	vec3 position;
	vec3 normal;
	vec2 texCoord;
}
fragIn;

layout(binding = 1) uniform sampler2D diffuseSampler;

layout(location = 0) out vec4 fragOut;

const float PI = 3.14159265359;

float DistributionGGX(vec3 norm, vec3 halfDir, float roughness)
{
	float a2    = roughness * roughness * roughness * roughness;
    float NdotH = max(dot(norm, halfDir), 0.0);
    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    return GeometrySchlickGGX(max(dot(N, L), 0.0), roughness) * 
           GeometrySchlickGGX(max(dot(N, V), 0.0), roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow (1.0 - cosTheta, 5.0);
}  

void main()
{
	const vec3 lightPos		= vec3(0.0, 8.0, 0.0);
	const vec3 lightColor	= vec3(180.0, 180.0, 180.0);
	const float metallic	= 0.0;
	const float roughness	= 0.2;
	const float ambient		= 0.005;
	
	vec3 albedo		= texture(diffuseSampler, fragIn.texCoord).xyz;
	vec3 normal		= normalize(fragIn.normal);
	vec3 viewDir    = normalize(fragIn.cameraPos - fragIn.position);
	
	vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

	vec3 lightOut = vec3(0.0);

		vec3 lightDir		= normalize(lightPos - fragIn.position);
		vec3 halfDir		= normalize(viewDir + lightDir);
		float dist			= length(lightPos - fragIn.position);
		float attenuation	= 1.0 / (dist * dist);
		vec3 radiance		= lightColor * attenuation;
	
		float specular		= DistributionGGX(normal, halfDir, roughness);
		float diffuse		= GeometrySmith(normal, viewDir, lightDir, roughness);
		vec3 fresnel		= FresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);

		vec3 kS = fresnel;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 numerator		= specular * diffuse * fresnel;
		float denominator	= 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
		vec3 specOut		= numerator / max(denominator, 0.001);
            
		float NdotL = max(dot(normal, lightDir), 0.0);
		lightOut += (kD * albedo / PI + specOut) * radiance * NdotL;


	vec3 ambientOut = vec3(0.03) * albedo * ambient;
    vec3 color = ambient + lightOut;

	color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    fragOut = vec4(color, 1.0);
}